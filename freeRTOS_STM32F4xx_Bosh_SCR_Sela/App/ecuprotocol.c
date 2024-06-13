/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : ecuprotocol.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/18/2014
*
* Description        : CAN2 ECU protocol
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#define ECU_GLOBALS
#include "includes.h"
#include "scr.h"

#pragma location = "EXTCCRAM"
static commProcess_t EcuProto;

#define CANDATALEN 8

#define TRAP_MAP	  0x01
#define TRAP_ST		  0x02
#define TPAP_PARM	  0x04
#define TRAP_FACTOR	0x08
#define TRAP_TSV	  0x10
#define TRAP_MAF	  0x11

#define MAF_AUTO_POLLING	  0x01
#define T1_AUTO_POLLING	    0x02
#define T2_AUTO_POLLING	    0x04
#define P1_AUTO_POLLING	    0x08

INT8U CanTrapMode=TRAP_ST;
INT8U CmdAutoPollMode = 0;


void Can2Semaphore_Init(void)
{
	BSP_Can2Trap = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_Can2Trap);
}

void Init_CAN2commRxProc(int port)
{

	portENTER_CRITICAL();
	EcuProto.nPort = port;
	EcuProto.nTmr = 0;
	EcuProto.lastPacketlen = 0;
	EcuProto.last_CRC16 = 0;
	portEXIT_CRITICAL();

	CommReInit(port);
}


int MakePackMassage(CanTxMsg *ptarget, INT32U id, INT8U type, INT8U rtr, INT8U *src, INT8U len)
{
	int i=0;
	if(type == CAN_STAN)
		ptarget->StdId = id;
	else
		ptarget->ExtId = id;

	ptarget->IDE = type;
	ptarget->RTR = rtr;
	ptarget->DLC = len;

	for(i=0;i<len;i++)
	{
		ptarget->Data[i] = src[i];
	}
	return i;
	
}


int SendCanMassage(CanTxMsg *src)
{
	if(BSP_Can2Trap == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	//if((xSemaphoreTake(BSP_Can2Trap, ( portTickType ) 50 )  == pdTRUE) && CAN_GetFlagStatus(CAN2,CAN_FLAG_FOV0) == 0)
	if(xSemaphoreTake(BSP_Can2Trap, ( portTickType ) 50 )  == pdTRUE)
	{
		CAN_Transmit(CAN2, src);
		//CAN_ITConfig(CAN2, CAN_IT_TME,ENABLE);
		//printk("%d\n",CAN_GetFlagStatus(CAN2,CAN_FLAG_FF0));
		xSemaphoreGive(BSP_Can2Trap);
		return OK;
	}
	return ERROR;
}


#define CAN_PACKMIN		19

static commProcess_t *can2RxProcOS(commProcess_t *pdev)
{
	static INT8U	data;
	INT8U err=0;

	data = CommGetChar(pdev->nPort, 10, &err);
	if(err == COMM_NO_ERR)
	{
		if(data == 0x7E && pdev->bStart_flag==0){										// DecodePacket 전이기 때문에 무조건 7E는 start flag 이다.

			pdev->bStart_flag = 1;
			pdev->bEnd_flag = 0;
			pdev->nPacketCnt = 0;
			//vTaskDelay(1);

		}
		else if( ( 	(pdev->pRxBuf[pdev->nPacketCnt-1]==0x7F) && (data == 0x7F) )  && (pdev->bStart_flag)  &&  pdev->nPacketCnt >= CAN_PACKMIN )			// 시작플레그가 1이고 끝문자가 왔는가.
		{			
			pdev->bEnd_flag = 1;								// 끝 플레그를 1로 만든다.

			//vTaskDelay(1);
		}
		if ( pdev->nPacketCnt >= GUI_PACKET_BUFFSZ )
		{
			printk("start = %d, end = %d, packcount = %d\n",pdev->bStart_flag,pdev->bEnd_flag,pdev->nPacketCnt);
			pdev->nPacketCnt = 0;	//방어코드	  
		}
		pdev->pRxBuf[pdev->nPacketCnt++] = data;
	}
	else
	{
		if( !pdev->bEnd_flag )
			vTaskDelay(1);
	}
	if(pdev->bStart_flag && pdev->bEnd_flag && pdev->nPacketCnt)
	{
		memcpy(pdev->pRxBuf_dec,pdev->pRxBuf,pdev->nPacketCnt);
		pdev->bStart_flag = 0;
		pdev->bEnd_flag = 0;
		return pdev;
	}
	return 0;

}

INT8U MapSendCount=0;
INT8U MapTSVSendCount=0;
void StepSendTrap(void)
{
	INT16U SendCount=0;
	INT8U buf[20];
	INT16U buf16b;
	INT16S sbuf16b;
	INT32U buf32b;
	CanTxMsg       msg;
	double dtemp;
	static INT8U ParmSendCount = 1;
	static INT8U MafSendCount = 1;   
	static INT8U FactorSendCount=1;
	static INT16U TrapSendCount = 0x100;

	if(TmrChk(TMR_CANTRAPINTERVAL) == 0)
	{
		memset(buf,0x0,sizeof(buf));
		if( ((CanTrapMode & TRAP_MAP) == TRAP_MAP) && (map_delay > 10))
		{
				memcpy( (char*)buf, (char *)&ScrMap.AlpaMap.row[MapSendCount].col[0], CANDATALEN);
				SendCount = MakePackMassage(&msg, 0x161 + MapSendCount, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
        MapSendCount++;
        map_delay = 0;
        if(MapSendCount >= 72) {
          MapSendCount = 0;
          CanTrapMode &= ~TRAP_MAP;
          CanTrapMode |= TRAP_ST;          
        }
			TmrSetT(TMR_CANTRAPINTERVAL, 1);
			TmrStart(TMR_CANTRAPINTERVAL);      
		}
		else if( ((CanTrapMode & TRAP_TSV) == TRAP_TSV) && (map_delay > 10))
		{
      if(MapTSVSendCount < 6) {
				 memcpy( (char*)buf, (char *)&ScrMap.BaseTemp[MapTSVSendCount * 4], CANDATALEN);
				 SendCount = MakePackMassage(&msg, 0x1B1 + MapTSVSendCount, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
      }
      else {
         sbuf16b = MapTSVSendCount-6;
				 memcpy( (char*)buf, (char *)&ScrMap.BaseMaf[sbuf16b * 4], CANDATALEN);
				 SendCount = MakePackMassage(&msg, 0x1C1 + sbuf16b, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
      }
      MapTSVSendCount++;
      map_delay = 0;
      if(MapTSVSendCount >= 12) {
        MapTSVSendCount = 0;
				CanTrapMode &= ~TRAP_TSV;
				CanTrapMode |= TRAP_ST;         
      }
			TmrSetT(TMR_CANTRAPINTERVAL, 1);
			TmrStart(TMR_CANTRAPINTERVAL);         
		}
		else if( (CanTrapMode & TRAP_FACTOR) == TRAP_FACTOR )
		{
			if(FactorSendCount == 1) {
				memset(buf, 0x0, sizeof(buf));        
				buf32b = ScrFactor.Catalyst.Ka;
				memcpy(buf, (char *)&buf32b, 4);
				buf32b = ScrFactor.Catalyst.Kd;
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x121, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				FactorSendCount = 2;
      }
			else if(FactorSendCount == 2) {
				memset(buf, 0x0, sizeof(buf));
				buf32b = ScrFactor.Catalyst.Kr;
				memcpy(buf, (char *)&buf32b, 4);
				buf16b = ScrFactor.Catalyst.OFA;
				memcpy(&buf[4], (char *)&buf16b, 2);
				SendCount = MakePackMassage(&msg, 0x122, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				FactorSendCount = 3;
      }
			else if(FactorSendCount == 3) {
				memset(buf, 0x0, sizeof(buf));        
				buf32b = ScrFactor.Catalyst.S1;
				memcpy(buf, (char *)&buf32b, 4);
				buf16b = ScrFactor.Catalyst.S2;
				memcpy(&buf[4], (char *)&buf16b, 2);
				SendCount = MakePackMassage(&msg, 0x123, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				FactorSendCount = 4;
      }
			else if(FactorSendCount == 4) {
				memset(buf, 0x0, sizeof(buf));        
				buf16b = ScrFactor.Catalyst.L;
				memcpy(buf, (char *)&buf16b, 2);
        buf[2] = ScrFactor.Catalyst.Xsp;
        buf[3] = ScrFactor.Catalyst.Coeff;        
				buf32b = ScrFactor.Catalyst.ScrArea;
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x124, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				FactorSendCount = 5;
      }
			else if(FactorSendCount == 5) {
				memset(buf, 0x0, sizeof(buf));        
				buf32b = ScrFactor.Catalyst.Kl;
				memcpy(&buf[0], (char *)&buf32b, 4);
				buf32b = ScrFactor.Catalyst.Kp;
				memcpy(&buf[4], (char *)&buf32b, 4);        
				SendCount = MakePackMassage(&msg, 0x125, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				FactorSendCount = 1;
				CanTrapMode &= ~TRAP_FACTOR;
				CanTrapMode |= TRAP_ST;
      }
      
			TmrSetT(TMR_CANTRAPINTERVAL, 20);
			TmrStart(TMR_CANTRAPINTERVAL);
		}

		else if( (CanTrapMode & TPAP_PARM) == TPAP_PARM)
		{
      if(ParmSendCount == 1) {
				buf16b = SysSet.CutMaf;
				memcpy((char *)&buf[0], (char *)&buf16b, 2 );
				buf16b = SysSet.LowTankLevel;
				memcpy((char *)&buf[2], (char *)&buf16b,  2 );
				sbuf16b = SysSet.CutTankTemp;
				memcpy((char *)&buf[4], (char *)&sbuf16b,  2 );
				buf16b = SysSet.TankType;
				memcpy((char *)&buf[6], (char *)&buf16b,  2 );
				SendCount = MakePackMassage(&msg, 0x141, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				ParmSendCount = 2;
      }
			else if(ParmSendCount == 2) {
				buf16b = SysSet.LowT1;
				memcpy((char *)&buf[0], (char *)&buf16b, 2 );
				buf16b = SysSet.HighT1;
				memcpy((char *)&buf[2], (char *)&buf16b, 2 );
				buf16b = SysSet.LowP1;					
				memcpy((char *)&buf[4], (char *)&buf16b, 2 );
				buf16b = SysSet.HighP1;					
				memcpy((char *)&buf[6], (char *)&buf16b, 2 );
				SendCount = MakePackMassage(&msg, 0x142, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				ParmSendCount = 3;
      }
			else if(ParmSendCount == 3) {
				buf16b = SysSet.BatVoltageLow;
				memcpy((char *)&buf[0], (char *)&buf16b, 2 );
				buf[2] = SysSet.Temp_Ratio;
  		  buf[3] = SysSet.Scr_Mode;
        buf[4] = SysSet.Scr_Period;
				SendCount = MakePackMassage(&msg, 0x143, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				ParmSendCount = 4;
      }
			else if(ParmSendCount == 4) {
				buf16b = SysSet.ScrWaitTime;
				memcpy((char *)&buf[0], (char *)&buf16b, 2 );
        buf[2] = SysSet.ScrPurge;
				buf16b = SysSet.ScrPurgeTime;
				memcpy((char *)&buf[4], (char *)&buf16b, 2 );
        buf[6] = SysSet.ScrMafMode;
				SendCount = MakePackMassage(&msg, 0x144, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				ParmSendCount = 5;
      }
			else if(ParmSendCount == 5) {
				//추후 UI Parameter Setting ID 0x145을 사용할경우 아래을 사용해라...
				buf16b = SysSet.MafOffset;
				memcpy((char *)&buf[0], (char *)&buf16b, 2 );
				buf16b = SysSet.MafDiameter;
				memcpy((char *)&buf[2], (char *)&buf16b, 2 );
				buf16b = SysSet.MapOffset;
				memcpy((char *)&buf[4], (char *)&buf16b, 2);
        buf[6] = SysSet.Nozzle;
				SendCount = MakePackMassage(&msg, 0x145, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				ParmSendCount = 1;
				CanTrapMode &= ~TPAP_PARM;
				CanTrapMode |= TRAP_ST;
      }
			TmrSetT(TMR_CANTRAPINTERVAL, 20);
			TmrStart(TMR_CANTRAPINTERVAL);
		}
		else if( (CanTrapMode & TRAP_MAF) == TRAP_MAF)
		{
      if(MafSendCount == 1) {
        buf16b = ScrMaf.Adc[0];
        memcpy((char *)&buf[0], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[1];
        memcpy((char *)&buf[2], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[2];
        memcpy((char *)&buf[4], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[3];
        memcpy((char *)&buf[6], (char *)&buf16b, 2 );
        SendCount = MakePackMassage(&msg, 0x151, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				MafSendCount = 2;
      }
			else if(MafSendCount == 2) {
        buf16b = ScrMaf.Adc[4];
        memcpy((char *)&buf[0], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[5];
        memcpy((char *)&buf[2], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[6];
        memcpy((char *)&buf[4], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[7];
        memcpy((char *)&buf[6], (char *)&buf16b, 2 );
        SendCount = MakePackMassage(&msg, 0x152, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				MafSendCount = 3;
      }
			else if(MafSendCount == 3) {
        buf16b = ScrMaf.Adc[8];
        memcpy((char *)&buf[0], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Adc[9];
        memcpy((char *)&buf[2], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[0];
        memcpy((char *)&buf[4], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[1];
        memcpy((char *)&buf[6], (char *)&buf16b, 2 );
        SendCount = MakePackMassage(&msg, 0x153, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				MafSendCount = 4;
      }
			else if(MafSendCount == 4) {
        buf16b = ScrMaf.Value[2];
        memcpy((char *)&buf[0], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[3];
        memcpy((char *)&buf[2], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[4];
        memcpy((char *)&buf[4], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[5];
        memcpy((char *)&buf[6], (char *)&buf16b, 2 );
        SendCount = MakePackMassage(&msg, 0x154, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				MafSendCount = 5;
      }
			else if(MafSendCount == 5) {
        buf16b = ScrMaf.Value[6];
        memcpy((char *)&buf[0], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[7];
        memcpy((char *)&buf[2], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[8];
        memcpy((char *)&buf[4], (char *)&buf16b, 2 );
        buf16b = ScrMaf.Value[9];
        memcpy((char *)&buf[6], (char *)&buf16b, 2 );
        SendCount = MakePackMassage(&msg, 0x155, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				MafSendCount = 1;
				CanTrapMode &= ~TRAP_MAF;
				CanTrapMode |= TRAP_ST;
      }
			TmrSetT(TMR_CANTRAPINTERVAL, 20);
			TmrStart(TMR_CANTRAPINTERVAL);
		}    
		else if(CanTrapMode == TRAP_ST)
		{
      if(TrapSendCount == 0x98) {
				buf32b = (INT32U)(SysSts.curXk * 1000000.0);
				memcpy(buf, (char *)&buf32b, 4);
				buf32b = (INT32U)(SysSts.Xc * 1000000.0);
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x98, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x99;
      }
			else if(TrapSendCount == 0x99) {
				buf16b = (INT16U)(SysSts.H1k * 100.0);
				memcpy(buf, (char *)&buf16b, 2);
				buf16b = (INT16U)(SysSts.H2k * 100.0);
				memcpy(&buf[2], (char *)&buf16b, 2);
				buf16b = (INT16U)(SysSts.Yk * 100.0);
				memcpy(&buf[4], (char *)&buf16b, 2);        
				SendCount = MakePackMassage(&msg, 0x99, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x100;
      }
			else if(TrapSendCount == 0x100) {
				memcpy(buf, (char *)&SysSts.Version, 2);
				buf[2] = SysSts.SCRMode;
        buf[3] = SysSts.Map_X;
        buf[4] = SysSts.Map_Y;
        buf[5] = ScrFlag.Flushing;
				SendCount = MakePackMassage(&msg, 0x100, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x101;
      }
			else if(TrapSendCount == 0x101) {
				//buf16b = SysSts.SystemError;
				//memcpy(&buf[0], (char *)&buf16b, 2);
				//printk("\n error = [%04X]",buf16b);
				buf[0] = SysSts.SystemError.B;

				buf16b = SysSts.SystemCheck.W;
				memcpy(&buf[2], (char *)&buf16b, 2);
				//printk("  check = [%04X]",buf16b);

				buf16b = SysSts.SystemSignal.W;
				memcpy(&buf[4], (char *)&buf16b,2);
				//printk("  signal = [%04X]\n",buf16b);

				SendCount = MakePackMassage(&msg, 0x101, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x102;
      }
			else if(TrapSendCount == 0x102) {
				buf16b = SysSts.Noxppm[0];
				memcpy(&buf[0], (char *)&buf16b, 2);
				buf16b = SysSts.Noxppm[1];
				memcpy(&buf[2], (char *)&buf16b, 2);
				buf16b = SysSts.MafKg_H;        
				memcpy(&buf[4], (char *)&buf16b, 2);
				buf16b = SysSts.Maf_temp;           
        memcpy(&buf[6], (char *)&buf16b, 2);
        
				SendCount = MakePackMassage(&msg, 0x102, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x103;
      }
			else if(TrapSendCount == 0x103) {
				buf16b = SysSts.T1_temp;
				memcpy(buf, (char *)&buf16b, 2);

				buf16b = SysSts.T2_temp;
				memcpy(&buf[2], (char *)&buf16b, 2);

				buf16b = SysSts.Tavg_temp;
				memcpy(&buf[4], (char *)&buf16b,2);

				buf16b = SysSts.TankTemp;
				memcpy(&buf[6], (char *)&buf16b,2);
        
				SendCount = MakePackMassage(&msg, 0x103, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x104;
      }
			else if(TrapSendCount == 0x104) {
				buf32b = (INT32U)(SysSts.DosingRatehouer * 100.0);
				memcpy(buf, (char *)&buf32b, 4);
				buf32b = (INT32U)(SysSts.TotalDosingRate * 100.0);
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x104, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x105;
      }
			else if(TrapSendCount == 0x105) {
				buf32b = (INT32U)(SysSts.Kl * 1000000.0);
				memcpy(buf, (char *)&buf32b, 4);
				buf32b = (INT32U)(SysSts.Kp * 100000.0);
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x105, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x106;
      }
			else if(TrapSendCount == 0x106) {
				buf32b = (INT32U)(SysSts.Vk * 10000.0);
				memcpy(buf, (char *)&buf32b, 4);
				buf32b = (INT32U)(SysSts.Gamma * 10000.0);
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x106, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x107;
      }
			else if(TrapSendCount == 0x107) {
				buf16b = SysSts.BattVoltage;
				memcpy(buf, (char *)&buf16b, 2);
				buf16b = SysSts.TankLevelP;
				memcpy(&buf[2], (char *)&buf16b, 2);
				buf16b = SysSts.P1_bar;
				memcpy(&buf[4], (char *)&buf16b, 2);
				buf16b = SysSts.DosingDuty;
				memcpy(&buf[6], (char *)&buf16b, 2);
				SendCount = MakePackMassage(&msg, 0x107, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x108;
      }
			else if(TrapSendCount == 0x108) {
				buf32b = (INT32U)(SysSts.curXh * 1000000.0);
				memcpy(buf, (char *)&buf32b, 4);
				buf16b = SysSts.StatusAlpha;
				memcpy((char *)&buf[4], (char *)&buf16b, 2 );
				buf16b = SysSts.NoxReduction;	//저감율
				memcpy((char *)&buf[6], (char *)&buf16b, 2 );
				SendCount = MakePackMassage(&msg, 0x108, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x115;
      }
#if 0 // Control 에서 사용학 있음.
			else if(TrapSendCount == 0x110) {
				TrapSendCount = 0x111;
      }
			else if(TrapSendCount == 0x111) {
				TrapSendCount = 0x112;
      }
			else if(TrapSendCount == 0x112) {
				TrapSendCount = 0x113;
      }
			else if(TrapSendCount == 0x113) {
				TrapSendCount = 0x115;
]
#endif
			else if(TrapSendCount == 0x115) {
				if(ScrFlag.TimeRequest)
				{
					ScrFlag.TimeRequest = 0;
					buf[0]= 0;
					SendCount = MakePackMassage(&msg, 0x115, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				}
				TrapSendCount = 0x117;
      }
			else if(TrapSendCount == 0x117) {      
				sbuf16b = (INT16S)SysSts.Formula1;
				memcpy((char *)&buf[0], (char *)&sbuf16b, 2 );
				sbuf16b = (INT16S)SysSts.Formula2;
				memcpy((char *)&buf[2], (char *)&sbuf16b, 2 );
				sbuf16b = (INT16S)SysSts.Formula3;
				memcpy((char *)&buf[4], (char *)&sbuf16b, 2 );
				SendCount = MakePackMassage(&msg, 0x117, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
        TrapSendCount = 0x118;
      }
			else if(TrapSendCount == 0x118) {      
				buf32b = (INT32U)(SysSts.NH3Up * 100.0);
				memcpy(buf, (char *)&buf32b, 4);
				buf32b = (INT32U)(SysSts.NH3Dn * 100.0);
				memcpy(&buf[4], (char *)&buf32b, 4);
				SendCount = MakePackMassage(&msg, 0x118, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				TrapSendCount = 0x120;
      }
			else if(TrapSendCount == 0x120) {
				TrapSendCount = 0x12D;
      }
			else if(TrapSendCount == 0x12D) {
				if( CmdAutoPollMode )
					TrapSendCount = 0x133;
				else
					TrapSendCount = 0x98;
      }
			else if(TrapSendCount == 0x133) {
				if( (CmdAutoPollMode & MAF_AUTO_POLLING) == MAF_AUTO_POLLING)
				{
					memcpy((char *)&buf[0], (char *)&adcdata[ADC_AVR_MAF], 2 );
					dtemp = MafvoltageOffset(adcdata[ADC_AVR_MAF]);
					buf16b = (INT16U)(dtemp*100);
					memcpy((char *)&buf[2], (char *)&buf16b, 2 );
					SendCount = MakePackMassage(&msg, 0x133, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				}
				else if( (CmdAutoPollMode & T1_AUTO_POLLING) == T1_AUTO_POLLING)
				{
					buf[0] = 0;
					buf16b = adcdata[ADC_AVR_T1];
					memcpy(&buf[2], (char *)&buf16b, 2);
					SendCount = MakePackMassage(&msg, 0x137, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
					
				}
				else if( (CmdAutoPollMode & T2_AUTO_POLLING) == T2_AUTO_POLLING)
				{
					buf[0] = 1;
					buf16b = adcdata[ADC_AVR_T2];
					memcpy(&buf[2], (char *)&buf16b, 2);
					SendCount = MakePackMassage(&msg, 0x137, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				}
				else if( (CmdAutoPollMode & P1_AUTO_POLLING) == P1_AUTO_POLLING)
				{
					buf[0] = 1;
					buf16b = adcdata[ADC_AVR_P1];
					memcpy(&buf[2], (char *)&buf16b, 2);
					SendCount = MakePackMassage(&msg, 0x13B, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
				}
				TrapSendCount = 0x98;
      }
			TmrSetT(TMR_CANTRAPINTERVAL, 20);
			TmrStart(TMR_CANTRAPINTERVAL);
		}
		if (SendCount )
			SendCanMassage(&msg);
	}
}


INT8U dlMakePacket(INT8U *data,INT16U *totalsize,INT8U size,INT8U init)
{
	INT8U	ret = 0;
	static INT16U paddress=0;
	if(init == 0)
	{
		memcpy(	(char *)&Cdo.data[paddress], (char *)data, size);
		paddress += size;
		*totalsize = paddress;

		if(paddress >= 256 )
		{
			paddress = 0;
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	else
		paddress = 0;

	return ret;
}
void TaskCheckDownload(void)
{
	static INT8U Count=0;
	static INT32U PackCount=0;
	if(Cdo.mode == DL_PROGRESS)
	{
		if(PackCount != Cdo.expectedCount)
		{
			PackCount = Cdo.expectedCount;
		}
		else
		{
			Count++;
			if(Count > 5) //5초이상 응답이 없으면 ...초기화해라...
			{
				Count = 0;
				PackCount = 0;
				Cdo.mode = DL_NONE;
			}
		}
	}
}

char Nh3Up[4] ,  Nh3Dn[4];
INT8U CAN2_ECU_task(int port,INT8U wui,char * wui_data,int wui_len,INT8U *seq)
{
	commProcess_t  *pDev=NULL;
	canrxmsg_t     *ptPacket=NULL;
	pack_t		   *pmsg=NULL;
	CanTxMsg       msg;
  
	INT8U ch=0;
	INT8U buf[20];
	INT16U buf16b;
	INT16S sbuf16b;  
	INT32U buf32b;
	INT16U SendCount=0,ParserTotalCount=0;
	INT8U  errReport=0,ret;
	int sectCnt;
	int i, sectOffset;
	INT16U ramCRC=0;
	char *pDestAddr;
  INT8U  map_x;
	
	pDev = can2RxProcOS(&EcuProto);
	if(!pDev) {
		if(Cdo.mode == DL_NONE)
		{
			StepSendTrap();
		}
		return 0;
	}
	else{
		CommReInit(pDev->nPort);
		// 시작(2) + 끝(2) ==> 몸통을 제외한 4바이트만큼 빼면 실제받은 데이터의 사이즈
		pDev->nPacketCnt -= 4;
		// 여기까지 오면 정상. 패킷커멘드 분석시작.
		ptPacket = (canrxmsg_t *)&pDev->pRxBuf_dec[2];
		//GUI_hexdisp(&pDev->pRxBuf_dec[0],pDev->nPacketCnt+4);
    memset(buf, 0x0, sizeof(buf));
	}
  if(ptPacket->id == 0x10) {
		// 파일 사이즈 4Byte copy
		memcpy((char *)&(Cdo.ulFileSize), (char *)&ptPacket->data[0], sizeof(Cdo.ulFileSize));

		memcpy((char *)&Cdo.uSTotalPacketCount, (char *)&ptPacket->data[4], sizeof(Cdo.uSTotalPacketCount));
		//GUI_hexdisp((char *)&ptPacket->data[0], 8);
		Cdo.uSOnePacketSize = 8;//4Byte 단위 다운로드

		printk("\n##### CAN DownLoad Command #####\n");
		printk("File size          : %dBytes\n" ,Cdo.ulFileSize);
		printk("One Packet size    : %dBytes\n" ,Cdo.uSOnePacketSize);
		printk("Total Packet Count : %dPacket\n",Cdo.uSTotalPacketCount);
		printk("Last Packet Byte   : %dBytes\n" ,Cdo.ulFileSize % Cdo.uSOnePacketSize);
		// 전체 패킷 갯수 = (파일 사이즈 / 한 패킷 사이즈) + (파일 사이즈 % 한 패킷 사이즈) 
		Cdo.uSLastPacketSize = Cdo.ulFileSize / Cdo.uSOnePacketSize + (Cdo.ulFileSize % Cdo.uSOnePacketSize ? 1 : 0);
		if(Cdo.uSLastPacketSize == Cdo.uSTotalPacketCount)		// 전체 패킷 갯수와 총 패킷 사이즈가 같으면 	
		{
			printk("File size, Packet size, TotalPacket count Ok..\n");
			errReport = 0;
			Cdo.mode = DL_START;
			Cdo.expectedCount = 0;								// 다음 페킷 카운트의 숫자
			dlMakePacket(pmsg->data,&ParserTotalCount,0,1);//buffer 초기화


			memcpy((char *)tPrjcode.boot_seq,"down_gui",8);
			SPI_FLASH_PageErase(PROJECT_BASE);
			if(SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t))) 
			{
				/*-------------------------------------------------------------------------------*/	
				// 다운로드 파일 사이즈 만큼의 SPI Flash의 영역을 지운다.

				sectCnt = (Cdo.ulFileSize / SPI_FLASH_BLOCK)+1;
				if(sectCnt>FLASH_DOWN_BLOCKCOUNT) sectCnt=FLASH_DOWN_BLOCKCOUNT;
				sectOffset = DOWNLOAD_SECTOR;
				//printk("sectCnt = %d\n",sectCnt);

				// Block 크기 삭제 
				for(i=0;i<sectCnt;i++)
				{
					SPI_FLASH_BlockErase(sectOffset);
					sectOffset += SPI_FLASH_BLOCK;
					printk("Erase %X at 0x%05X\n",SPI_FLASH_BLOCK,sectOffset);
				}
				/*-------------------------------------------------------------------------------*/	
				Cdo.last_CRC16 = 0;	
				tPrjcode.file_size = Cdo.ulFileSize;
			}
			else
			{
				errReport = 1;
			}
		}
		else
		{
			errReport = 1;										// total packet size error
		}
		if(errReport != 0) 
		{
			Cdo.mode = DL_NONE;
			buf[0] = 0x01;//packet count Error
			SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			printk("Download Start Error....Send\n");
		}
		else
		{
			buf[0] = 0x00;//packet count Error
			SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			printk("Download Start Response....Send\n");
		}
  }
	else if(ptPacket->id == 0x11) {
		if(Cdo.mode != DL_START){
			return 0;
		}
		else
		{
			Cdo.mode = DL_PROGRESS;
			memcpy((char *)&Cdo.uCRC16, (char *)&ptPacket->data[0], sizeof( Cdo.uCRC16) );
			printk("Receive CRC Code = %04X\n",Cdo.uCRC16);
			buf[0] = 0x00;//packet count Error
			SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		}
  }

	else if(ptPacket->id == 0x12) {
		if(Cdo.mode != DL_PROGRESS){
			return 0;
		}
		pDestAddr = (char *)(Cdo.data);
			
		if(Cdo.expectedCount == Cdo.uSTotalPacketCount-1)
		{
			Cdo.mode = DL_END;
			Cdo.uSLastPacketSize = Cdo.ulFileSize % Cdo.uSOnePacketSize;
			//printk("last byte = %d\n",Cdo.uSLastPacketSize);

			if(Cdo.uSLastPacketSize == 0)						// 계산이 0이면 한패킷 풀로 보낸다
				Cdo.uSLastPacketSize = Cdo.uSOnePacketSize;

			GUI_hexdisp((char *)&ptPacket->data[0],Cdo.uSLastPacketSize);

			ret = dlMakePacket(&ptPacket->data[0],&ParserTotalCount,(INT8U)Cdo.uSLastPacketSize,0);
			//printk("last pack size = %d[%02X][%02X][%02X]\n",ParserTotalCount,pDestAddr[ParserTotalCount-1],pDestAddr[ParserTotalCount-2],pDestAddr[ParserTotalCount-3]);
			Cdo.WritePacketCount +=1;
			errReport = 1;
			if(ret == 1)	//256 Byte 이면....
			{
				SPI_FLASH_BufferWrite((INT8U *)pDestAddr, (INT32U)(DOWNLOAD_SECTOR + ((Cdo.WritePacketCount-1)* Cdo.WriteOnePacketSize)), Cdo.WriteOnePacketSize);
				Cdo.last_CRC16 = GenerateCRC(pDestAddr, Cdo.WriteOnePacketSize, Cdo.last_CRC16);
				//printk("[%d]0........................................................................Cdo.last_CRC16 = [%02X]\n",Cdo.WritePacketCount,Cdo.last_CRC16);
			}
			else
			{
				SPI_FLASH_BufferWrite((INT8U *)pDestAddr, (INT32U)(DOWNLOAD_SECTOR + ((Cdo.WritePacketCount-1)*Cdo.WriteOnePacketSize)), ParserTotalCount);
				Cdo.last_CRC16 = GenerateCRC(pDestAddr, ParserTotalCount, Cdo.last_CRC16);
				//printk("[%d]1........................................................................Cdo.last_CRC16 = [%02X]\n",Cdo.WritePacketCount,Cdo.last_CRC16);
			}
			tPrjcode.file_crc = Cdo.last_CRC16;

		}
		else
		{
			ret = dlMakePacket(&ptPacket->data[0],&ParserTotalCount,8,0);
			if(ret == 1)
			{
				Cdo.WritePacketCount +=1;

				// OnePacket 단위로 Write를 실행한다.... 
				SPI_FLASH_BufferWrite((INT8U *)pDestAddr, (INT32U)(DOWNLOAD_SECTOR + ((Cdo.WritePacketCount-1)* Cdo.WriteOnePacketSize)), Cdo.WriteOnePacketSize);
				Cdo.last_CRC16 = GenerateCRC(pDestAddr, Cdo.WriteOnePacketSize, Cdo.last_CRC16);
				//printk("[%d]........................................................................Cdo.last_CRC16 = [%02X]\n",Cdo.WritePacketCount,Cdo.last_CRC16);
				//if( (Cdo.WritePacketCount % 10) ==0 ) percent_display(Cdo.WritePacketCount,Cdo.uSTotalPacketCount);
			}
			if( (Cdo.expectedCount % 10) ==0 ) percent_display(Cdo.expectedCount,Cdo.uSTotalPacketCount);
		}
    buf[0] = 0x00;//CRC Ok
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		Cdo.expectedCount++;
  }

	else if(ptPacket->id == 0x13) {
		if(Cdo.mode != DL_END){
			return 0;
		}
		ramCRC = GetFlashAreaCRC(DOWNLOAD_SECTOR, Cdo.ulFileSize);
		if(ramCRC != Cdo.uCRC16)
		{
			printk("CRC Error....You = [%04X], Me =[%04X], Save = [%04X]\n",Cdo.uCRC16,ramCRC,tPrjcode.file_crc);
			errReport = 2;
			printk("Not matched\n");
			Cdo.dl_good_finished = 0;
			buf[0] = 0x01;//CRC Error
			SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			SendCanMassage(&msg);
		}
		else
		{
			printk("CRC Ok....You = [%04X], Me =[%04X], Save = [%04X]\n",Cdo.uCRC16,ramCRC,tPrjcode.file_crc);
			errReport = 1;
			Cdo.dl_good_finished = 1;
			buf[0] = 0x00;//CRC Ok
			SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			SendCanMassage(&msg);
			printk("Good\n");
		}
		if(errReport != 1) 		dl.mode = DL_NONE;

		if(Cdo.dl_good_finished == 1){

			memcpy((char*)tPrjcode.boot_seq,"backup", 6);
			SPI_FLASH_PageErase(PROJECT_BASE);
			SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));
			for(i=0;i<0xffff;i++);
			dbg_system_reset(); 
		}
  }
  
	else if(ptPacket->id == 0x98) {
    buf32b = (INT32U)(SysSts.curXk * 100.0);
		memcpy(buf, (char *)&buf32b, 4);
    buf32b = (INT32U)(SysSts.Xc * 100.0);
		memcpy(&buf[4], (char *)&buf32b, 4);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }
    
	else if(ptPacket->id == 0x99) {
    buf32b = (INT32U)(SysSts.H1k * 100.0);
		memcpy(buf, (char *)&buf32b, 4);
    buf32b = (INT32U)(SysSts.H2k * 100.0);
		memcpy(&buf[4], (char *)&buf32b, 4);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }
    
	else if(ptPacket->id == 0x100) {
		memcpy(buf, (char *)&SysSts.Version, 2);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	else if(ptPacket->id == 0x102) {
		buf16b = SysSts.Noxppm[0];
		memcpy(&buf[0], (char *)&buf16b, 2);
		buf16b = SysSts.Noxppm[1];
		memcpy(&buf[2], (char *)&buf16b, 2);
		buf16b = SysSts.MafKg_H;
    memcpy(&buf[4], (char *)&buf16b, 2);
    buf16b = SysSts.Maf_temp;
		memcpy(&buf[6], (char *)&buf16b, 2);      
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	else if(ptPacket->id == 0x103) {
		buf16b = SysSts.T1_temp + 273;
		memcpy(&buf[0], (char *)&buf16b, 2);

		buf16b = SysSts.T2_temp + 273;
		memcpy(&buf[2], (char *)&buf16b, 2);

		buf16b = SysSts.Tavg_temp + 273;
		memcpy(&buf[4], (char *)&buf16b, 2);

		buf16b = SysSts.TankTemp;
		memcpy(&buf[6], (char *)&buf16b, 2);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	else if(ptPacket->id == 0x104) {
		buf32b = (INT32U)(SysSts.DosingRatehouer * 100.0);
		memcpy(buf, (char *)&buf32b, 4);
		buf32b = (INT32U)(SysSts.TotalDosingRate * 100.0);
		memcpy(&buf[4], (char *)&buf32b, 4);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	else if(ptPacket->id == 0x105) {
		buf32b = (INT32U)(SysSts.Kl * 1000000.0);
		memcpy(buf, (char *)&buf32b, 4);
		buf32b = (INT32U)(SysSts.Kp * 1000000.0);
		memcpy(&buf[4], (char *)&buf32b, 4);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	else if(ptPacket->id == 0x106) {
		buf32b = (INT32U)(SysSts.Vk * 10000.0);
		memcpy(buf, (char *)&buf32b, 4);
		buf32b = (INT32U)(SysSts.Gamma * 10000.0);
		memcpy(&buf[4], (char *)&buf32b, 4);
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	else if(ptPacket->id == 0x107) {
		buf16b = SysSts.BattVoltage;
		memcpy(buf, (char *)&buf16b, 2);
		buf16b = SysSts.TankLevelP;
		memcpy(&buf[2], (char *)&buf16b, 2);
		buf16b = SysSts.P1_bar;
		memcpy(&buf[4], (char *)&buf16b, 2);
		buf16b = SysSts.DosingDuty;
		memcpy(&buf[6], (char *)&buf16b, 2);    
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }
    
	else if(ptPacket->id == 0x108) {
		buf32b = (INT32U)(SysSts.curXh * 100.0);
		memcpy(buf, (char *)&buf32b, 4);
		buf16b = SysSts.StatusAlpha;
		memcpy((char *)&buf[4], (char *)&buf16b, 2 );
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }

	/*  Control */
	else if(ptPacket->id == 0x110) {
		if((ptPacket->data[0] & 0x3) == 1)
      SysSts.SCRMode = MANUAL_MODE;
		else if((ptPacket->data[0] & 0x3) == 2)
      SysSts.SCRMode = ALPHA_MODE;
		else if((ptPacket->data[0] & 0x3) == 3)
      SysSts.SCRMode = DOSING_MODE;
    else
      SysSts.SCRMode = AUTO_MODE;
    
		if(OldMode != SysSts.SCRMode) {
			OldMode = SysSts.SCRMode;
			if(SysSts.SCRMode <= ALPHA_MODE) {
				FirstFlag = 0;
				//AlarmClear();
				SysSts.TotalDosingRate = 0.0;
				nStatus = SCR_CHECKMODE;
        SCR_Control_Off();
			}
		}
    
		if(ptPacket->data[1] & 0x01) {
      if(!SysSts.SystemSignal.Dosing) {
        PurgeSolFlag = 1;
        ScrFlag.Flushing = 1;
        ScrFlag.SupplyStart = 1;
        SysSts.SystemSignal.Supply = ON;
      }
    }
    else {
      ScrFlag.Flushing = 0;
      nStatus = SCR_CHECKMODE;      
    }
  }

	else if(ptPacket->id == 0x111) {
		if(SysSts.SCRMode == MANUAL_MODE) {
      memcpy( (char*)&buf16b, (char *)&ptPacket->data[0], 2);
      SysSts.mTavg = buf16b;
      memcpy( (char*)&buf16b, (char *)&ptPacket->data[2], 2);
      SysSts.mMaf = buf16b;
      memcpy( (char*)&buf16b, (char *)&ptPacket->data[4], 2);
      SysSts.mNoxppm[0] = buf16b;
      memcpy( (char*)&buf16b, (char *)&ptPacket->data[6], 2);
      SysSts.mNoxppm[1] = buf16b;
		}
  }

	else if(ptPacket->id == 0x112) {
		if(SysSts.SCRMode == DOSING_MODE)
		{
			if(ptPacket->data[0] == 1) { 
				SysSts.MDosingStart = 1;
        memcpy((char *)&buf32b, (char *)&ptPacket->data[4], 4);
        SysSts.MDosingRatehouer = ((double)buf32b/100.0);
        printk("Control Dosing  = %f g/h\n",SysSts.MDosingRatehouer);
      }
      else if(ptPacket->data[0] == 2) { 
				SysSts.MDosingStart = 2;        
        memcpy((char *)&buf32b, (char *)&ptPacket->data[4], 4);
        SysSts.MDosingDuty = ((double)buf32b/100.0);
        if(SysSts.MDosingDuty > 100.0) SysSts.MDosingDuty = 100.0;
        printk("Control Duty  = %f %\n",SysSts.MDosingDuty);
			}
			else {
				SysSts.MDosingStart = 0;
				printk("Dosing Stop\n");
			}
			SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		}
  }

	else if(ptPacket->id == 0x113) {
		if(ptPacket->data[0] == 1)
		{
      SysSts.DosingDuty = 0.0;
      SysSts.DosingRatehouer = 0.0; 
			SysSts.TotalDosingRate = 0.0;
			SysSts.preXk	= 0.0;
			SysSts.curXk	= 0.0;
			SysSts.preUk	= 0.0;
			SysSts.curUk	= 0.0;

			printk("Acc Clear\n");
		}
		if(ptPacket->data[1] == 1)
		{
			AlarmClear();
			printk("Alarm Clear\n");
		}
		//SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
  }
	else if(ptPacket->id == 0x114) {
		SysSts.AlphaSet = ptPacket->data[0];
		printk("Alpha Factor = %d\n",SysSts.AlphaSet);
		//SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
  }
	else if(ptPacket->id == 0x116) {
    memcpy( (char*)&RTC_Buf, (char *)&ptPacket->data, 8);
    RTC_Flag = 0x12;
  }
//	case 0x115:
//		ResponseTime((char *)&buf[0]);
//		SendCount = MakePackMassage(&msg, 0x116, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
//		break;
//	case 0x116:
//		TimeParsorSet((char *)&ptPacket->data[0]);
//		break;

	/*SCR Factor*/
	else if(ptPacket->id == 0x120) {
		if(ptPacket->data[0] == 1)
		{
			CanTrapMode = TRAP_FACTOR;
			SendCount = 0;
		}
  }
	else if(ptPacket->id == 0x121) {
		memcpy( (char*)&buf32b, (char *)&ptPacket->data[0], 4);
		ScrFactor.Catalyst.Ka = buf32b;
		//printk("set ka = %d\n",ScrFactor.Catalyst.Ka);
		memcpy( (char*)&buf32b, (char *)&ptPacket->data[4], 4);
		ScrFactor.Catalyst.Kd = buf32b;
		//printk("set kd = %d\n",ScrFactor.Catalyst.Kd);
		//SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x122) {
		memcpy( (char*)&buf32b, (char *)&ptPacket->data[0], 4);
		ScrFactor.Catalyst.Kr = buf32b;
		//printk("set kr = %d\n",ScrFactor.Catalyst.Kr);
		memcpy( (char*)&buf16b, (char *)&ptPacket->data[4], 2);
		ScrFactor.Catalyst.OFA = buf16b;
		//printk("set OFA = %d\n",ScrFactor.Catalyst.OFA_x100);
		//SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x123) {
		memcpy( (char*)&buf32b, (char *)&ptPacket->data[0], 4);
		ScrFactor.Catalyst.S1 = buf32b;
		//printk("set S1 = %d\n",ScrFactor.Catalyst.S1_x10); 
		memcpy( (char*)&buf16b, (char *)&ptPacket->data[4], 2);
		ScrFactor.Catalyst.S2 = buf16b;
		//printk("set S2 = %d\n",ScrFactor.Catalyst.S2_1000);
		//SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);//저장
  }
	else if(ptPacket->id == 0x124) {
		memcpy( (char*)&buf16b, (char *)&ptPacket->data[0], 2);
		ScrFactor.Catalyst.L = buf16b;
		//printk("set length = %d\n",ScrFactor.Catalyst.L);
		ScrFactor.Catalyst.Xsp = ptPacket->data[2];
		//printk("set length = %d\n",ScrFactor.Catalyst.Xsp);
		ScrFactor.Catalyst.Coeff = ptPacket->data[3];
		//printk("set length = %d\n",ScrFactor.Catalyst.Coeff);
		memcpy( (char*)&buf32b, (char *)&ptPacket->data[4], 4);
		ScrFactor.Catalyst.ScrArea = buf32b;
		//printk("set scr area = %d\n",ScrFactor.Catalyst.ScrArea_x100000);
		//SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);;//저장
  }
	else if(ptPacket->id == 0x125) {  
		memcpy((char *)&buf32b, (char *)&ptPacket->data[0], 4 );
		ScrFactor.Catalyst.Kl = buf32b;
		//printk("ScrKlFactor = %d\n",ScrFactor.Catalyst.Kl);
		memcpy((char *)&buf32b, (char *)&ptPacket->data[4], 4 );
		ScrFactor.Catalyst.Kp = buf32b;
		//printk("ScrKpFactor = %d\n",ScrFactor.Catalyst.Kp);
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);;//저장    
  }
 else if(ptPacket->id == 0x130) {
		if(ptPacket->data[0] == 1)
		{
			buf16b = ScrCalib.CalMaf[0].AdcLevel;
			memcpy((char *)&buf[0], (char *)&buf16b, 2 );
			buf16b = ScrCalib.CalMaf[1].AdcLevel;
			memcpy((char *)&buf[2], (char *)&buf16b, 2 );
			buf16b = ScrCalib.CalMaf[2].AdcLevel;
			memcpy((char *)&buf[4], (char *)&buf16b, 2 );
			buf16b = ScrCalib.CalMaf[3].AdcLevel;
			memcpy((char *)&buf[6], (char *)&buf16b, 2 );
			SendCount = MakePackMassage(&msg, 0x131, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			SendCanMassage(&msg);

			memset(buf, 0x0, sizeof(buf));
			buf16b = ScrCalib.CalMaf[4].AdcLevel;
			memcpy((char *)&buf[0], (char *)&buf16b, 2 );
			buf[3] = ScrCalib.CalMaf[0].Offset;
			buf[4] = ScrCalib.CalMaf[1].Offset;
			buf[5] = ScrCalib.CalMaf[2].Offset;
			buf[6] = ScrCalib.CalMaf[3].Offset;
			buf[7] = ScrCalib.CalMaf[4].Offset;
			SendCount = MakePackMassage(&msg, 0x132, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);

		}
		if(ptPacket->data[1] == 1)
		{
			CmdAutoPollMode |= MAF_AUTO_POLLING;	// Auto Polling On
		}
		else
		{
			CmdAutoPollMode &= ~MAF_AUTO_POLLING;	//Auto Polling Off
		}
  }

	else if(ptPacket->id == 0x131) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrCalib.CalMaf[0].AdcLevel = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		ScrCalib.CalMaf[1].AdcLevel = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		ScrCalib.CalMaf[2].AdcLevel = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		ScrCalib.CalMaf[3].AdcLevel = buf16b;
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		//FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x132) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrCalib.CalMaf[4].AdcLevel = buf16b;
		ScrCalib.CalMaf[0].Offset = (INT8S)ptPacket->data[3];
		ScrCalib.CalMaf[1].Offset = (INT8S)ptPacket->data[4];
		ScrCalib.CalMaf[2].Offset = (INT8S)ptPacket->data[5];
		ScrCalib.CalMaf[3].Offset = (INT8S)ptPacket->data[6];
		ScrCalib.CalMaf[4].Offset = (INT8S)ptPacket->data[7];
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
		FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x134) {
		ch = ptPacket->data[1];
		if(ptPacket->data[0] == 1)
		{
			buf[0] = ch;
      buf[1] = ScrCalib.Temp[ch].enable;
			buf16b = ScrCalib.Temp[ch].RangeLow;
			memcpy((char *)&buf[4], (char *)&buf16b, 2 );
			buf16b = ScrCalib.Temp[ch].RangeHigh;
			memcpy((char *)&buf[6], (char *)&buf16b, 2 );
			SendCount = MakePackMassage(&msg, 0x135, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			SendCanMassage(&msg);

			buf[0] = ch;
			buf16b = ScrCalib.Temp[ch].AdcLow;
			memcpy((char *)&buf[4], (char *)&buf16b, 2 );
			buf16b = ScrCalib.Temp[ch].AdcHigh;
			memcpy((char *)&buf[6], (char *)&buf16b, 2 );
			SendCount = MakePackMassage(&msg, 0x136, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		}
		if(ptPacket->data[2] == 1)	// auto folling On
		{
			if(ch == 0)
				CmdAutoPollMode |= T1_AUTO_POLLING;	// Auto Polling On
      else
        CmdAutoPollMode |= T2_AUTO_POLLING;	// Auto Polling On
		}
		else						// auto polling Off
		{
			if(ch == 0)
				CmdAutoPollMode &= ~T1_AUTO_POLLING;	// Auto Polling On
      else 
        CmdAutoPollMode &= ~T2_AUTO_POLLING;	// Auto Polling On
		}
  }
	else if(ptPacket->id == 0x135) {
		ch = ptPacket->data[0] & 0x03;
    ScrCalib.Temp[ch].enable = ptPacket->data[1];
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2);
		ScrCalib.Temp[ch].RangeLow = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2);
		ScrCalib.Temp[ch].RangeHigh = buf16b;
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x136) {
		ch = ptPacket->data[0];

		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2);
		ScrCalib.Temp[ch].AdcLow = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2);
		ScrCalib.Temp[ch].AdcHigh = buf16b;
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }
        
	else if(ptPacket->id == 0x138) {
		ch = ptPacket->data[1] & 0x01 ;
		if(ptPacket->data[0] == 1)
		{
			buf[0] = ch;
      buf[1] = ScrCalib.Supply.enable;
			buf16b = ScrCalib.Supply.RangeLow;
			memcpy((char *)&buf[4], (char *)&buf16b, 2 );
			buf16b = ScrCalib.Supply.RangeHigh;
			memcpy((char *)&buf[6], (char *)&buf16b, 2 );
			SendCount = MakePackMassage(&msg, 0x139, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			SendCanMassage(&msg);

			buf[0] = ch;
			buf16b = ScrCalib.Supply.AdcLow;
			memcpy((char *)&buf[4], (char *)&buf16b, 2 );
			buf16b = ScrCalib.Supply.AdcHigh;
			memcpy((char *)&buf[6], (char *)&buf16b, 2 );
			SendCount = MakePackMassage(&msg, 0x13A, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		}
		if(ptPacket->data[2] == 1)	// auto folling On
		{
				CmdAutoPollMode |= P1_AUTO_POLLING;	// Auto Polling On

		}
		else						// auto polling Off
		{
				CmdAutoPollMode &= ~P1_AUTO_POLLING;	// Auto Polling On
		}
  }
	else if(ptPacket->id == 0x139) {
		ch = ptPacket->data[0] & 0x01 ; 
    ScrCalib.Supply.enable = ptPacket->data[1];
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2);
		ScrCalib.Supply.RangeLow = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2);
		ScrCalib.Supply.RangeHigh = buf16b;
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x13A) {
		ch = ptPacket->data[0] & 0x01 ; 
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2);
		ScrCalib.Supply.AdcLow = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2);
		ScrCalib.Supply.AdcHigh = buf16b;
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }

	else if(ptPacket->id == 0x13C) {
		ch = ptPacket->data[0] & 0x03 ;    
    buf[0] = ch;  
    buf[1] = ScrCalib.NOx_enable[ch];
    SendCount = MakePackMassage(&msg, 0x13D, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
  }
  
	else if(ptPacket->id == 0x13D) {
		ch = ptPacket->data[0] & 0x03;
    ScrCalib.NOx_enable[ch] = ptPacket->data[1];
		SendCount = 0;
		FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);;//저장
  }
  
	else if(ptPacket->id == 0x140) {
		CanTrapMode = TPAP_PARM;
  }

	else if(ptPacket->id == 0x141) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		SysSet.CutMaf = buf16b;
		//printk("SysSet.CutMaf = %d\n",SysSet.CutMaf);
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		SysSet.LowTankLevel = buf16b;
		//printk("SysSet.LowTankLevel = %d\n",SysSet.LowTankLevel);
		memcpy((char *)&sbuf16b, (char *)&ptPacket->data[4], 2 );
		SysSet.CutTankTemp = sbuf16b;
		//printk("SysSet.CutTankTemp = %d\n",SysSet.CutTankTemp);
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		SysSet.TankType = buf16b;
		//printk("SysSet.TankType = %d\n",SysSet.TankType);
		FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
		SendCount = 0;
  }

	else if(ptPacket->id == 0x142) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		SysSet.LowT1 = buf16b;
		//printk("SysSet.TempLow = %d\n",SysSet.TempLow);
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		SysSet.HighT1 = buf16b;
		//printk("SysSet.TempHigh = %d\n",SysSet.TempHigh);
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		SysSet.LowP1 = buf16b;									//펌프 압력
		//printk("SysSet.LowTankLevel = %d\n",SysSet.LowP1);
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		SysSet.HighP1 = buf16b;									// 펌프압력
		//printk("SysSet.CutTankTemp = %d\n",SysSet.HighP1);
		FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
		SendCount = 0;
  }

	else if(ptPacket->id == 0x143) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		SysSet.BatVoltageLow = buf16b;
		//printk("SysSet.BatVoltageLow = %d\n",SysSet.BatVoltageLow);
		SysSet.Temp_Ratio = ptPacket->data[2];
    SysSet.Scr_Mode = ptPacket->data[3];
    SysSet.Scr_Period = ptPacket->data[4];
    if(SysSet.Scr_Period == 2)
      SCR_PERIOD = 250;
    else
      SCR_PERIOD = 500;
		FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
  }
        
	else if(ptPacket->id == 0x144) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		SysSet.ScrWaitTime = buf16b;
		printk("SCR Wait Time Set = %dMin\n",SysSet.ScrWaitTime);
    SysSet.ScrPurge = ptPacket->data[2];
    if(SysSet.ScrPurge)
      SysSts.SystemSignal.Purge = ON;
    else
      SysSts.SystemSignal.Purge = OFF;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		SysSet.ScrPurgeTime = buf16b;
    SysSet.ScrMafMode = ptPacket->data[6];    
		FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
  }

	else if(ptPacket->id == 0x145) {
		memcpy( (char*)&buf16b, (char *)&ptPacket->data[0], 2);    
    SysSet.MafOffset = buf16b;
    //printk("set MafOffset = %d\n",SysSet.MafOffset);
		memcpy( (char*)&buf16b, (char *)&ptPacket->data[2], 2);    
    SysSet.MafDiameter = buf16b;
    //printk("set MafDiameter = %d\n",SysSet.MafDiameter);    
		memcpy( (char*)&buf16b, (char *)&ptPacket->data[4], 2);    
    SysSet.MapOffset = buf16b;
    //printk("set MapOffset = %d\n",SysSet.MapOffset);     
    SysSet.Nozzle = ptPacket->data[6];
    //printk("set Nozzle = %d\n",SysSet.Nozzle);         
		FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
  }
        
	else if(ptPacket->id == 0x147) {
		SysSet.ScrTestMode = ptPacket->data[0];
    
		if(SysSet.ScrTestMode == 1)	//SCR 동작알고리즘 분사 off test mode가 1이면(분사하지 말아라....)
		{
      SysSts.SystemSignal.Operation = ON;	 // Disable
    }
    else
      SysSts.SystemSignal.Operation = OFF; // Enable
        
		printk("Test Mode = %d\n",SysSet.ScrTestMode);
		FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
  }
 else if(ptPacket->id == 0x150) {
		if(ptPacket->data[0] == 1) {
			CanTrapMode = TRAP_MAF;
			SendCount = 0;
		}      
  }

	else if(ptPacket->id == 0x151) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrMaf.Adc[0] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		ScrMaf.Adc[1] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		ScrMaf.Adc[2] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		ScrMaf.Adc[3] = buf16b;
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
  }

	else if(ptPacket->id == 0x152) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrMaf.Adc[4] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		ScrMaf.Adc[5] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		ScrMaf.Adc[6] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		ScrMaf.Adc[7] = buf16b;
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
  }

	else if(ptPacket->id == 0x153) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrMaf.Adc[8] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		ScrMaf.Adc[9] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		ScrMaf.Value[0] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		ScrMaf.Value[1] = buf16b;
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
  }

	else if(ptPacket->id == 0x154) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrMaf.Value[2] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		ScrMaf.Value[3] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		ScrMaf.Value[4] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		ScrMaf.Value[5] = buf16b;
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
  }

	else if(ptPacket->id == 0x155) {
		memcpy((char *)&buf16b, (char *)&ptPacket->data[0], 2 );
		ScrMaf.Value[6] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[2], 2 );
		ScrMaf.Value[7] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[4], 2 );
		ScrMaf.Value[8] = buf16b;
		memcpy((char *)&buf16b, (char *)&ptPacket->data[6], 2 );
		ScrMaf.Value[9] = buf16b;
		SendCount = MakePackMassage(&msg, ptPacket->id, CAN_STAN, CAN_RTR_DATA, &ptPacket->data[0], CANDATALEN);
    FRAM_page_readwrite(FRAM_SCR_MAF_SAVE, SAVE);//저장  
  } 
  
	else if(ptPacket->id == 0x160) {
		if(ptPacket->data[0] == 1)
		{
			CanTrapMode = TRAP_MAP;
      map_delay = 0;
		}
  }  
	else if(ptPacket->id >  0x160 && ptPacket->id < 0x1A9) {
    map_x = ptPacket->id - 0x161;
		memcpy( (char *)&ScrMap.AlpaMap.row[map_x].col[0],(char *)&ptPacket->data[0], CANDATALEN);
		SendCount = 0;
    if(map_x == 71) {
    	FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);//저장
    }
  }  
	else if(ptPacket->id == 0x1B0) {
		if(ptPacket->data[0] == 1)
		{
			CanTrapMode = TRAP_TSV;
		}
  }
	else if(ptPacket->id >  0x1B0 && ptPacket->id < 0x1B7) {
    map_x = ptPacket->id - 0x1B1;
		memcpy( (char *)&ScrMap.BaseTemp[map_x * 4],(char *)&ptPacket->data[0], CANDATALEN);
		SendCount = 0;
    FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);//저장    
  }
	else if(ptPacket->id >  0x1C0 && ptPacket->id < 0x1C7) {
    map_x = ptPacket->id - 0x1C1;
		memcpy( (char *)&ScrMap.BaseMaf[map_x * 4],(char *)&ptPacket->data[0], CANDATALEN);
		SendCount = 0;
    if(map_x == 5) {
    FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);//저장
    }
  }
	else if(ptPacket->id == 0x1E0) {               //NH3 센서 UP
    memcpy((char *)&Nh3Up, (char *)&ptPacket->data[0], 4 );
    SysSts.NH3Up = arrayToFloat((char *)&Nh3Up);
  }
	else if(ptPacket->id == 0x1E1) {                //NH3 센서 DOWN
    memcpy((char *)&Nh3Dn, (char *)&ptPacket->data[0], 4 );    
    SysSts.NH3Dn = arrayToFloat((char *)&Nh3Dn);    
  }
  //************************************************DPF
	else if(ptPacket->id == 0x1F0) {
    memcpy( (char*)&buf16b, (char *)&ptPacket->data[0], 2);
    SysSts.mTavg = buf16b;
    memcpy( (char*)&buf16b, (char *)&ptPacket->data[2], 2);
    SysSts.mMaf = buf16b;
    memcpy( (char*)&buf16b, (char *)&ptPacket->data[4], 2);
    SysSts.mNoxppm[0] = buf16b;
    memcpy( (char*)&buf16b, (char *)&ptPacket->data[6], 2);
    SysSts.mNoxppm[1] = buf16b;
    
    SysSet.ScrTestMode = ptPacket->data[0];
    if(SysSet.ScrTestMode == 1)
      SysSts.SystemSignal.Operation = ON;	
    else
      SysSts.SystemSignal.Operation = OFF;
  }
  else {
    printk("Not find ID");
  }
	if (SendCount )
		SendCanMassage(&msg);
	return 0;
}

void Can_RTC(INT8U id)
{
	CanTxMsg       msg;
  
  switch(id) {
  case 0x01:    // RTC Write
    MakePackMassage(&msg, 0x116, CAN_STAN, CAN_RTR_DATA, &RTC_Buf[0], CANDATALEN);
    SendCanMassage(&msg);
    break;
  case 0x02:    // RTC Read
    MakePackMassage(&msg, 0x115, CAN_STAN, CAN_RTR_DATA, &RTC_Buf[0], CANDATALEN);
    SendCanMassage(&msg);
    break;
  default:
    break;
  }
}


void Task_Can2(void *p_arg)
{
	sysinfo_t *s = (sysinfo_t *)p_arg 	;

	Init_CAN2commRxProc(CAN2_PORT);
	Can2Semaphore_Init();
			
	for(;;)
	{
		s->twatchEcuFlag = 1;
		CAN2_ECU_task(CAN2_PORT, 0, NULL, 0, NULL);
    if(RTC_Flag && (RTC_Flag < 0x10)) {
      Can_RTC(RTC_Flag);
      RTC_Flag = 0;
    }
		s->twatchEcuFlag = 0;
	}
}


INT16U dbg_CanUI_test(void)
{
	INT8U buf[10];
	sysinfo_t *s = &_sysinfo 	;
	CanTxMsg       msg;
	int len=0;

	memset(buf, 0x0, sizeof(buf));

	//if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	

	//on = comparison_num((char*)P_ARG(1));

	//int SendCanMassage(INT32U id, INT8U *src)
#if 0
	memset(buf, 0x0, sizeof(buf));
	memcpy(buf, (char *)&SysSts.Version, 2);
	len = MakePackMassage(&msg, 0x100, CAN_EXTN, CAN_RTR_DATA, buf, 8);
#else
	if(SysSts.Noxppm[0] > 0)
		memcpy(buf, (char *)&SysSts.Noxppm[0], 2);
	if(SysSts.Noxppm[1] > 0)
		memcpy(&buf[2], (char *)&SysSts.Noxppm[1], 2);
	memcpy(&buf[4], (char *)&SysSts.MafKg_H,2);
	len = MakePackMassage(&msg, 0x102, CAN_STAN, CAN_RTR_DATA, buf, 8);
#endif

	SendCanMassage(&msg);

	printk("[%d]system version = %02X,%02X..[%02X][%d]\n",len, buf[0],buf[1],s->Scrst->MafKg_H,s->Scrst->Noxppm[0]);

	return NO_DISPLAY;
}

/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : obdprotocol.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/18/2014
*
* Description        : CAN1 Nox Sensor protocol
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define OBD_GLOBALS
#define TANK_CAN_SUPPORT
#include "includes.h"

#pragma location = "EXTCCRAM"
static commProcess_t ObdProto;

void Init_CAN1commRxProc(int port)
{

	portENTER_CRITICAL();
	ObdProto.nPort = port;
	ObdProto.nTmr = 0;
	ObdProto.lastPacketlen = 0;
	ObdProto.last_CRC16 = 0;
	portEXIT_CRITICAL();

	CommReInit(port);
}

void NoxSensorOnOffCmd(INT8U OnOff)
{
	CanTxMsg msg;
//	char OnBuf[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x15};
//	char OffBuf[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x1A};
	char OnBuf[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x05};
	char OffBuf[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x0A};
	if(OnOff)
	{
		MakeCanMassage(&msg, 0x18FEDF00, CAN_EXTN, CAN_RTR_DATA, (INT8U *)&OnBuf, 8);
		CAN_Transmit(CAN1, &msg);
	}
	else
	{
		MakeCanMassage(&msg, 0x18FEDF00, CAN_EXTN, CAN_RTR_DATA, (INT8U *)&OffBuf, 8);
		CAN_Transmit(CAN1, &msg);
	}
}
// [0][1] : INT16S Nox(ppm) = (bits * 0.05)-200
// [2][3] : INT16U O2%      = (bits * 0.000514)-12
// [4.0~4.1]    : INT8U  Sensor Supply Status       = 0x00 : Not Inrange,        0x01 : Supply In range, 0x02 : Error, 0x03 : Not Available
// [4.2~4.3]    : INT8U  Sensor Temperature Status  = 0x00 : Not at temperature, 0x01 : At temperature,  0x02 : Error, 0x03 : Not Available
// [4.4~4.5]    : INT8U  Nox Signal Status		    = 0x00 : Not valid,          0x01 : Valid,           0x02 : Error, 0x03 : Not available
// [4.6~4.7]    : INT8U  O2 Signa Status			= 0x00 : Not valid,          0x01 : Valid,           0x02 : Error, 0x03 : Not available
INT16S NoxSensorBitToPpm(INT8U ch, char *target)
{
	INT16U t=0,z=0;
	double X=0.0,Y=0.0;
	t = target[0];
	z = target[1];
	z = z << 8;
	t = t | z;

	X = (double)t * (double)0.05;
	Y = X-200.0;
	if( Y < -200.0) Y = -200.0;
	else if(Y > 3012.75) Y = 3012.75;

	//SysSts.NoxErr[ch] = target[4] & 0x30;
	SysSts.NoxErr[ch] = target[4] ;
	return (INT16U)Y;
	
}

#define CAN_PACKMIN		19

static commProcess_t *can1RxProcOS(commProcess_t *pdev)
{
	static INT8U	data;
	INT8U err=0;


	data = CommGetChar(pdev->nPort, 0, &err);
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

void MakeCanMassage(CanTxMsg *ptarget, INT32U id, INT8U type, INT8U rtr, INT8U *src, INT8U len)
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

}
#define CAN_AVR_BUFF		2
#define CAN_MAXAVERAGE_IN	20

typedef struct {
	INT16S	buffer[100];
	INT16U	headIndex;
	INT16U	rearIndex;
	INT32S	sum;
}canrunavr_t;

#pragma location = "EXTCCRAM"
static canrunavr_t crunavr[CAN_AVR_BUFF];

INT16S crunningAverageNox(int ch,INT16S val)
{
	INT8U	AverageCount= 0;
	canrunavr_t *pAVR;
	pAVR = &crunavr[ch];										// 원하는 adc체널의 포인터를 얻어온다.

	AverageCount = CAN_MAXAVERAGE_IN;

	if ( pAVR->headIndex == AverageCount )
	{
		pAVR->headIndex = 0;
		pAVR->rearIndex = AverageCount-1;
	}
	pAVR->sum -= pAVR->buffer[pAVR->headIndex];
	pAVR->buffer[pAVR->headIndex] = val;
	pAVR->sum += pAVR->buffer[pAVR->headIndex];
	pAVR->headIndex++;
	return (INT16S) (pAVR->sum/AverageCount);
	
}

void CAN1_OBD_task(canrxmsg_t* ptPacket)
{
	INT16U			data[2];
	
	static INT16U AutoSend[2]={0,200};
  
	switch(ptPacket->id)
	{
	case 0x18F00E51:	//(5Pin to GND) NOx Input
    if(SysSts.SystemSignal.Noxact) {
      SysSts.NoxRxCount[0]++;
      if(SysSts.NoxRxCount[0] >= 65530) SysSts.NoxRxCount[0] = 1;
      
			if(AutoSend[0]++ >= 20)//400)
			{
				AutoSend[0] = 0;
				AutoSend[1] = 0;
				NoxSensorOnOffCmd(ON);
			}
			data[0] = NoxSensorBitToPpm(0, (char *)&ptPacket->data[0] );
			SysSts.Noxppm[0] = crunningAverageNox(0,data[0]);
			if( (SysSts.NoxErr[0] & 0x10) == 0)
				SysSts.Noxppm[0] = -210;

      if(SysSts.SCRMode == MANUAL_MODE) {
        SysSts.Noxppm[0] = SysSts.mNoxppm[0];
      }
		}
		break;
	case 0x18F00F52:	//(5Pin to NC) NOx Input
    if(SysSts.SystemSignal.Noxact) {
      SysSts.NoxRxCount[1]++;
      if(SysSts.NoxRxCount[1] >= 65530) SysSts.NoxRxCount[1] = 1;
      
			if(AutoSend[1]++ >= 20)//400)
			{
				AutoSend[0] = 0;
				AutoSend[1] = 0;
				NoxSensorOnOffCmd(ON);
			}
			data[1] = NoxSensorBitToPpm(1, (char *)&ptPacket->data[0] );
			SysSts.Noxppm[1] = crunningAverageNox(1,data[1]);
			if( (SysSts.NoxErr[1] & 0x10) == 0)
				SysSts.Noxppm[1] = -210;
      
      if(SysSts.SCRMode == MANUAL_MODE) {
        SysSts.Noxppm[1] = SysSts.mNoxppm[1];
      }
		}
		break;
		case 0x18FD9B3F:  //Urea Quality
//      if(ptPacket->data[1] == 0xFE) UreaQuality = 0;  //sensor failure
//      else UreaQuality = (INT8U)((double)ptPacket->data[1] * 0.25);
			break;
		case 0x18FE56A3:  //Urea Level
#ifdef TANK_CAN_SUPPORT
      if(ptPacket->data[0] == 0xFE) SysSts.TankLevelP = 0;  //sensor failure
      else SysSts.TankLevelP = (INT16U)((double)ptPacket->data[0] * 0.4);
      if(ptPacket->data[1] == 0xFE) SysSts.TankTemp = 0;  //sensor failure
      else SysSts.TankTemp = (INT16U)((double)ptPacket->data[1] - 40);
#endif
			break;      
	default:
		break;
	}
}


void Task_Can1(void *p_arg)
{

	sysinfo_t *s = (sysinfo_t *)p_arg 	;

	Init_CAN1commRxProc(CAN1_PORT);
  SysSts.SystemSignal.Noxact = 0;
			
	for(;;)
	{
		s->twatchObdFlag = 1;
    vTaskDelay(1);
		s->twatchObdFlag = 0;
	}
}


INT16U dbg_NOxSensorOnOff_test(void)
{
	INT8U			on=0;
	INT8U			flag=0;

	if ( CLI_GetArgCnt() < 2 ) return INV_PRM;	

	flag = comparison_num((char*)P_ARG(1));
	on = comparison_num((char*)P_ARG(2));

	SysSts.NoxSensorFlag[flag] = on;
	if(on)
	{
		printk("Nox sensor on Array = %d ..\n",flag);
	}
	else
	{
		printk("Nox Sensor off Array = %d..\n",flag);
	}
	return NO_DISPLAY;
}


INT16U dbg_NOxSensor_test(void)
{
	INT8U on = 0,i;
	CanTxMsg msg;
	char OnBuf[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,};
	//char OnBuf[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,};

	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	

	on = comparison_num((char*)P_ARG(1));
	OnBuf[7] = on;

	MakeCanMassage(&msg, 0x18FEDF00, CAN_EXTN, CAN_RTR_DATA, (INT8U *)&OnBuf, 8);
	CAN_Transmit(CAN1, &msg);
	printk("\n");
	for(i=0;i<8;i++)
		printk("[%02X]",OnBuf[i]);
	printk("\n");

	return NO_DISPLAY;
}

INT16U dbg_ob_debug_test(void)
{
	if ( obd_debug )
		obd_debug = 0;
	else
		obd_debug = 1;

	return NO_DISPLAY;
}

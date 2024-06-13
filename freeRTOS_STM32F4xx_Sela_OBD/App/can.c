/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : OBDProtocol.c
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

#define CAN_GLOBALS
#include "includes.h"

#define CANDATALEN 8

#define TRAP_0x010	0x010
#define TRAP_0x011	0x011
#define TRAP_0x012	0x012
#define TRAP_0x013	0x013


#define MEMSIZE_CK		  0x100 //256byte
#define ONEPACKET		    8

#define DOWNLOAD_CK		  0
#define DOWNLOAD_START	1
#define DOWNLOAD_CRC	  2
#define DOWNLOAD_PROG	  3
#define DOWNLOAD_END	  4
#define DOWNLOAD_WAIT	  5

#define RESPONSE_DELAY	100

#define DOWNLOAD_nCHECK	0
#define DOWNLOAD_nPROGR	1

INT32U  FileSize = 0;
INT8U   dlMode = 0;
INT16U  ramCRC;

/*
********************************************************************************
* Description : GUI_hexdisp
* Arguments   : data, size
* Return      : 
* Note        : hex display
******************************************************************************** 
*/
void GUI_hexdisp(char *data, int size)
{
	int i;

	for(i=0; i<size; i++){
		if(i % 20 == 0) printk("\n");
		printk("%02X ",data[i]);
	}
	printk("\n");
}

/*
********************************************************************************
* Description : Init_CANcommRxProc
* Arguments   : port
* Return      : 
* Note        : CAN Process
******************************************************************************** 
*/
void Init_CANcommRxProc(int port)
{

	portENTER_CRITICAL();
	CommReInit(port);
	portEXIT_CRITICAL();  
}

/*
********************************************************************************
* Description : CanSemaphore_Init
* Arguments   : None
* Return      : 
* Note        : CAN Semaphore Init
******************************************************************************** 
*/
void CanSemaphore_Init(void)
{
	BSP_CanSem = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_CanSem);
}

/*
********************************************************************************
* Description : MakePackMassage
* Arguments   : CanTxMsg *ptarget, INT32U id, INT8U type, INT8U rtr, INT8U *src, INT8U len
* Return      : 
* Note        : 
******************************************************************************** 
*/
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

/*
********************************************************************************
* Description : SendCanMassage
* Arguments   : CanTxMsg *src
* Return      : 
* Note        : 
******************************************************************************** 
*/
int SendCanMassage(CanTxMsg *src)
{
	if(BSP_CanSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_CanSem, ( portTickType ) 50 )  == pdTRUE)
	{
		CAN_Transmit(CAN1, src);
		xSemaphoreGive(BSP_CanSem);
		return OK;
	}
	return ERROR;
}

/*
********************************************************************************
* Description : CanTxFile
* Arguments   : None
* Return      : 
* Note        : 
******************************************************************************** 
*/
INT8U CanTxFile(void) 
{
	INT8U buf[8], ret=0;
	BYTE tbuf[256];
	INT16U quot, remain;
	INT32U i=0;
	UINT br;
	static INT8U nStatus = 0, CheckResponseCount = 0;
	CanTxMsg       msg;
  INT16U SendCount=0;
    
	switch(nStatus) 
	{
	case DOWNLOAD_CK:
		if(f_open(&fp1, "SCRPro.bin", FA_READ) == FR_OK) 
		{
			ret = DOWNLOAD_nPROGR;
			SendPacket = 0;
      nDlStatus = 0;
      ramCRC = 0;
			dlMode = DOWNLOAD_START;
			//printk("in SCRPro.bin SDCard...\n");
			nStatus = DOWNLOAD_START;
		}
		else
		{
			//if(fp1 != NULL)
			f_close(&fp1);
			ret = DOWNLOAD_nCHECK;
			nStatus = DOWNLOAD_WAIT;
			//printk("not SCRPro.bin SDCard...\n");
		}
	  break;
	case DOWNLOAD_START:
		memset(buf,0x0,sizeof(buf));
		FileSize = f_size(&fp1);
		TotalPacket = FileSize/ONEPACKET + (FileSize % ONEPACKET ? 1 : 0);

		buf[0] = FileSize & 0xFF;
		buf[1] = (FileSize >> 8) & 0xFF;
		buf[2] = (FileSize >> 16) & 0xFF;
		buf[3] = (FileSize >> 24) & 0xFF;

		buf[4] = TotalPacket & 0xFF;
		buf[5] = (TotalPacket >> 8) & 0xFF;
		buf[6] = (TotalPacket >> 16) & 0xFF;
		buf[7] = (TotalPacket >> 24) & 0xFF;

		memcpy(&buf[0], (char *)&FileSize,    4);
		memcpy(&buf[4], (char *)&TotalPacket, 4);
		printk("file size = %01lu\n",FileSize);
		printk("TotalPacket= %01lu\n",TotalPacket);
		printk("[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
		
		SendCount = MakePackMassage(&msg, 0x010, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		dlMode = DOWNLOAD_CRC;
		ret = DOWNLOAD_nPROGR;
		nStatus = DOWNLOAD_CRC;
		break;

	case DOWNLOAD_CRC:
		if(nDlStatus == 1)
		{
			CheckResponseCount = 0;
			quot = FileSize / MEMSIZE_CK;
			remain = FileSize % MEMSIZE_CK;
			for(i=0; i<quot; i++)
			{
				memset((char *)tbuf,0x00,sizeof(tbuf));
				f_lseek(&fp1,i*MEMSIZE_CK);
				f_read(&fp1, tbuf, MEMSIZE_CK, &br);
				ramCRC = GenerateCRC((INT8U*)tbuf, MEMSIZE_CK, ramCRC);
			}
			if(remain){
				memset((char *)tbuf,0x00,sizeof(tbuf));
				f_lseek(&fp1,i*MEMSIZE_CK);
				f_read(&fp1, tbuf, MEMSIZE_CK, &br);
				ramCRC = GenerateCRC((INT8U*)tbuf, remain, ramCRC);
			}
			memcpy(&buf[0], (char *)&ramCRC, 2);
			printk("CRC Code = [%04X]\n",ramCRC);
      SendCount = MakePackMassage(&msg, 0x011, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			dlMode = DOWNLOAD_PROG;
			ret = DOWNLOAD_nPROGR;
			nStatus = DOWNLOAD_PROG;
		}
		if(CheckResponseCount++ >= RESPONSE_DELAY)	//Start packet을 전송후 응답이 없으면...Wait상태로 돌아가라...
		{
			CheckResponseCount = 0;
			ret = DOWNLOAD_nCHECK;
			nStatus = DOWNLOAD_WAIT;
			f_close(&fp1);
		}
		break;

	case DOWNLOAD_PROG:
		if(nDlStatus == 2)
		{
		if(dlMode != DOWNLOAD_PROG) return 0;
		
		if(SendPacket == TotalPacket-1)
		{
			remain = FileSize % ONEPACKET;
			if(remain == 0) remain = ONEPACKET;

			f_lseek(&fp1,SendPacket *ONEPACKET);
			f_read(&fp1, buf,remain, &br);
      SendCount = MakePackMassage(&msg, 0x012, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			dlMode = DOWNLOAD_END;
			f_close(&fp1);
			nStatus = DOWNLOAD_END;
		}
		else
		{
			memset((char *)buf,0x00,sizeof(buf));
			f_lseek(&fp1,SendPacket *ONEPACKET);
			f_read(&fp1, buf, ONEPACKET, &br);
      SendCount = MakePackMassage(&msg, 0x012, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		}
		ret = DOWNLOAD_PROG;
		SendPacket++;
		}
		break;

	case DOWNLOAD_END:
		if(dlMode != DOWNLOAD_END) return 0;
		buf[0] = 0x01;
    SendCount = MakePackMassage(&msg, 0x013, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		ret = DOWNLOAD_PROG;
		printk("download end.....\n");
		nStatus = DOWNLOAD_WAIT;
		break;
	case DOWNLOAD_WAIT:
    nStatus = 0;
    Flags.DownloadFlag = 0;
		ret = DOWNLOAD_nCHECK;
		break;

	default:
		break;
	}
  if (SendCount ) SendCanMassage(&msg); 
	return ret;

}

/*
********************************************************************************
* Description : CanFotaFile
* Arguments   : None
* Return      : 
* Note        : 
******************************************************************************** 
*/
INT8U CanFotaFile(void) 
{
	INT8U buf[8], ret=0;
	BYTE tbuf[256];
	INT16U quot, remain;
	INT32U i=0;
	static INT8U nStatus = 0, CheckResponseCount = 0;
	CanTxMsg       msg;
  INT16U SendCount=0;
    
	switch(nStatus) 
	{
	case DOWNLOAD_CK:
    ret = DOWNLOAD_nPROGR;
    SendPacket = 0;
    nDlStatus = 0;
    ramCRC = 0;
    dlMode = DOWNLOAD_START;
    //printk("in SCRPro.bin SDCard...\n");
    nStatus = DOWNLOAD_START;
	  break;
	case DOWNLOAD_START:
		memset(buf,0x0,sizeof(buf));
		FileSize = ECUFW_Size;
		TotalPacket = FileSize/ONEPACKET + (FileSize % ONEPACKET ? 1 : 0);

		buf[0] = FileSize & 0xFF;
		buf[1] = (FileSize >> 8) & 0xFF;
		buf[2] = (FileSize >> 16) & 0xFF;
		buf[3] = (FileSize >> 24) & 0xFF;

		buf[4] = TotalPacket & 0xFF;
		buf[5] = (TotalPacket >> 8) & 0xFF;
		buf[6] = (TotalPacket >> 16) & 0xFF;
		buf[7] = (TotalPacket >> 24) & 0xFF;

		memcpy(&buf[0], (char *)&FileSize,    4);
		memcpy(&buf[4], (char *)&TotalPacket, 4);
		printk("file size = %01lu\n",FileSize);
		printk("TotalPacket= %01lu\n",TotalPacket);
		printk("[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
		
		SendCount = MakePackMassage(&msg, 0x010, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		dlMode = DOWNLOAD_CRC;
		ret = DOWNLOAD_nPROGR;
		nStatus = DOWNLOAD_CRC;
		break;

	case DOWNLOAD_CRC:
		if(nDlStatus == 1)
		{
			CheckResponseCount = 0;
			quot = FileSize / MEMSIZE_CK;
			remain = FileSize % MEMSIZE_CK;
			for(i=0; i<quot; i++)
			{
				memset((char *)tbuf,0x00,sizeof(tbuf));
				SPI_FLASH_BufferRead(tbuf, ECUDATA_SECTOR+(i*MEMSIZE_CK), sizeof(tbuf));
				ramCRC = GenerateCRC((INT8U*)tbuf, MEMSIZE_CK, ramCRC);
			}
			if(remain){
				memset((char *)tbuf,0x00,sizeof(tbuf));
				SPI_FLASH_BufferRead(tbuf, ECUDATA_SECTOR+(i*MEMSIZE_CK), sizeof(tbuf));
				ramCRC = GenerateCRC((INT8U*)tbuf, remain, ramCRC);
			}
			memcpy(&buf[0], (char *)&ramCRC, 2);
			printk("CRC Code = [%04X]\n",ramCRC);
      SendCount = MakePackMassage(&msg, 0x011, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			dlMode = DOWNLOAD_PROG;
			ret = DOWNLOAD_nPROGR;
			nStatus = DOWNLOAD_PROG;
		}
		if(CheckResponseCount++ >= RESPONSE_DELAY)	//Start packet을 전송후 응답이 없으면...Wait상태로 돌아가라...
		{
			CheckResponseCount = 0;
			ret = DOWNLOAD_nCHECK;
			nStatus = DOWNLOAD_WAIT;
		}
		break;

	case DOWNLOAD_PROG:
		if(nDlStatus == 2)
		{
		if(dlMode != DOWNLOAD_PROG) return 0;
    if(ramCRC != ECUFW_CRC16) return 0;
		
		if(SendPacket == TotalPacket-1)
		{
			remain = FileSize % ONEPACKET;
			if(remain == 0) remain = ONEPACKET;
      
      SPI_FLASH_BufferRead(buf, ECUDATA_SECTOR+(SendPacket*ONEPACKET), remain);
      SendCount = MakePackMassage(&msg, 0x012, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
			dlMode = DOWNLOAD_END;
			nStatus = DOWNLOAD_END;
		}
		else
		{
      memset((char *)buf,0x00,sizeof(buf));
      SPI_FLASH_BufferRead(buf, ECUDATA_SECTOR+(SendPacket*ONEPACKET), ONEPACKET);
      SendCount = MakePackMassage(&msg, 0x012, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		}
		ret = DOWNLOAD_PROG;
		SendPacket++;
		}
		break;

	case DOWNLOAD_END:
		if(dlMode != DOWNLOAD_END) return 0;
		buf[0] = 0x01;
    SendCount = MakePackMassage(&msg, 0x013, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
		ret = DOWNLOAD_PROG;
		printk("download end.....\n");
		nStatus = DOWNLOAD_WAIT;
		break;
	case DOWNLOAD_WAIT:
    nStatus = 0;
    Flags.DownloadFlag = 0;
		ret = DOWNLOAD_nCHECK;
		break;

	default:
		break;
	}
  if (SendCount ) SendCanMassage(&msg); 
	return ret;

}
/*
********************************************************************************
* Description : CAN_Decode
* Arguments   : info
* Return      : 
* Note        : CAN data process
******************************************************************************** 
*/
void CAN_Decode(void)
{
	CanTxMsg       msg;	
  INT8U buf[20];
	INT16U SendCount = 0,buf16b;
  INT16S sbuf16b;
	INT32U buf32b, addr;
  INT8U  map_x;

  if(Can_RxMsg.id < 0x100) {
    if(Can_RxMsg.id == 0x010) {
      if(Can_RxMsg.data[0] == 0x00)
      {
        nDlStatus = 1;				//Packet Count Ok...
        printk("DL Packet Count Ok....\n");
      }
      else
      {
        nDlStatus = 4;				//Packet Count Error.....
        printk("DL Packet Count Error....\n");
      }
    }
    else if(Can_RxMsg.id == 0x011) {
      if (Can_RxMsg.data[0] == 0x00)//CAN ID 0x11 Download Start(File CRC)
      {
        nDlStatus = 2;
        printk("DL CRC Response Ok....\n");
      }
      else
      {
        nDlStatus = 4;
        printk("DL CRC Error....\n");        
      }
    }
    else if(Can_RxMsg.id ==0x013) {
      if (Can_RxMsg.data[0] == 0x00) //CAN ID 0x13 Download End
      {
        ECUFW_flag = 0;
        nDlStatus = 3;
        printk("DL soucess Ok....\n");
      }
      else
      {
        nDlStatus = 4;
        printk("DL soucess Error....\n");
      }
    }
    else if(Can_RxMsg.id == 0x98) {//ID : 0x98
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      SecData.curXk = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      SecData.Xc = buf32b;
    }
    else if(Can_RxMsg.id == 0x99) {//ID : 0x99
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      SecData.H1k = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      SecData.H2k = buf32b;
    }
  }
  /////////////////////////////////////////ID 0x100~0x14F
  if(Can_RxMsg.id >= 0x100 && Can_RxMsg.id < 0x150) {
    if(Can_RxMsg.id == 0x100) {//ID : 0x100
       memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
       SecData.Ver   = buf16b;
    }
    else if(Can_RxMsg.id == 0x101) {
      SecData.Err.B = Can_RxMsg.data[0];
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      SecData.Chk.W = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      SecData.Sig.W = buf16b;     
    }
    else if(Can_RxMsg.id == 0x102) {
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
      SecData.NOxIn = (INT16S)buf16b;
      ReadValue.NOxIn = SecData.NOxIn;
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      SecData.NOxOut = (INT16S)buf16b;
      ReadValue.NOxOut = SecData.NOxOut;            
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      SecData.MAF = buf16b;
      ReadValue.MAF = SecData.MAF;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      SecData.Maf_temp = buf16b;
    }
    else if(Can_RxMsg.id == 0x103) {
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
      SecData.T1 = buf16b;
      ReadValue.TempIn = SecData.T1;
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      SecData.T2 = buf16b;
      ReadValue.TempOut = SecData.T2;
  		memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      SecData.Tavg = buf16b;
  		memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      SecData.TankTemp = buf16b;
    }
    else if(Can_RxMsg.id == 0x104) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      SecData.DosingRatehouer = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);  //테스트후 원상 복귀
      SecData.TotalDosingRate = buf32b;
      ReadValue.DosingRate = SecData.TotalDosingRate;
    }
    else if(Can_RxMsg.id == 0x105) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      SecData.Kl = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      SecData.Kp = buf32b;
    }
    else if(Can_RxMsg.id == 0x106) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      SecData.Vk = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      SecData.Gamma = buf32b;
    }
    else if(Can_RxMsg.id == 0x107) {
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
  		SecData.BV = buf16b;
      ReadValue.Vbat = SecData.BV;
 		  memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      SecData.TankLevelP = buf16b;
      ReadValue.UreaLevel = SecData.TankLevelP;
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      SecData.P1 = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      SecData.DosingDuty = buf16b;
    }
    else if(Can_RxMsg.id == 0x108) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      SecData.curXh = buf32b;   
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      SecData.StatusAlpha = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      SecData.Reduction = buf16b;
    }
    else if(Can_RxMsg.id == 0x115) {
      ResponseTime((char *)&buf[0]);
      SendCount = MakePackMassage(&msg, 0x116, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
    }
    else if(Can_RxMsg.id == 0x116) {
      TimeParsorSet((char *)&Can_RxMsg.data[0]);
    }
    else if(Can_RxMsg.id == 0x117) {
 		  memcpy((char *)&sbuf16b,&Can_RxMsg.data[0],2);
			SecData.Formula1 = sbuf16b;
 		  memcpy((char *)&sbuf16b,&Can_RxMsg.data[2],2);
			SecData.Formula2 = sbuf16b;
 		  memcpy((char *)&sbuf16b,&Can_RxMsg.data[4],2);
			SecData.Formula3 = sbuf16b;
    }
    else if(Can_RxMsg.id > 0x140 && Can_RxMsg.id < 0x145) {
      memcpy((char *)&Par_Buf[Par_Cnt*8],&Can_RxMsg.data[0],8);
      Par_Cnt++;
      Par_Time = 0;
    }
    else if(Can_RxMsg.id == 0x145) {
      memcpy((char *)&Par_Buf[32],&Can_RxMsg.data[0],8);
      Par_Cnt++;
      Par_Time = 0;
      if(Par_Cnt == 5 && Par_Flag) {
        Par_Flag++;
        Par_Cnt = 0;
      }
    }    
  }
  /////////////////////////////////////////ID 0x150~0x19F
  if(Can_RxMsg.id >= 0x150 && Can_RxMsg.id < 0x1E0) {
    if(Can_RxMsg.id > 0x160 && Can_RxMsg.id < 0x1A9) {
      map_x = Can_RxMsg.id - 0x161;
      memcpy((char *)&ScrMap.AlpaMap.row[map_x].col[0],&Can_RxMsg.data[0],8);
    }
    else if(Can_RxMsg.id > 0x1B0 && Can_RxMsg.id < 0x1B7) {
      map_x = Can_RxMsg.id - 0x1B1;
      memcpy((char *)&ScrMap.BaseTemp[map_x * 4],&Can_RxMsg.data[0],8);
    }
    else if(Can_RxMsg.id > 0x1C0 && Can_RxMsg.id < 0x1C7) {
      map_x = Can_RxMsg.id - 0x1C1;
      memcpy((char *)&ScrMap.BaseMaf[map_x * 4],&Can_RxMsg.data[0],8);
    }               
  }
  /////////////////////////////////////////ID 0x1D0~
  if(Can_RxMsg.id >= 0x1D0) {
    if(Can_RxMsg.id == 0x1D0) {
      memcpy(buf, &ModemInfo.LpIP, 8);
      SendCount = MakePackMassage(&msg, 0x1D0, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
    }
    else if(Can_RxMsg.id == 0x1D1) {
     memcpy((char *)&ModemInfo.LpIP, &Can_RxMsg.data[0], 4);
     memcpy((char *)&ModemInfo.LpPort, &Can_RxMsg.data[4], 4);
     addr = MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo;
     FRAMMultiWrite((u8 *)&ModemInfo.LpIP, addr, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
     SendCount = 0;
    }
  }
	if (SendCount )
		SendCanMassage(&msg);  
}

/*
********************************************************************************
* Description : Task_CAN
* Arguments   : *p_arg
* Return      : 
* Note        : CAN task process
******************************************************************************** 
*/
void Task_CAN(void *p_arg)
{
	sysinfo_t *s = (sysinfo_t *)p_arg 	;

	Init_CANcommRxProc(CAN1_PORT);
	CanSemaphore_Init();
  
			
	for(;;)
	{
		s->twatchCanFlag = 1;
    vTaskDelay(1);    
		s->twatchCanFlag = 0;
	}
}


/*
********************************************************************************
* Description : obd_debug
* Arguments   : *none
* Return      : 
* Note        : Nox Sensor On
******************************************************************************** 
*/
INT16U dbg_ob_debug_test(void)
{
	if ( obd_debug )
		obd_debug = 0;
	else
		obd_debug = 1;

	return NO_DISPLAY;
}
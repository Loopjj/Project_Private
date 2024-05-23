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
		FileSize = Setting.ECUFW_Size;
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
      if(ramCRC != Setting.ECUFW_CRC16) return 0;
      if(Setting.ECUFW_flag) for(i=0; i<100; i++){;;};  //DCU FOTA 업데이트시 CAN데이터 전송 시간지연
		
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
    Setting.ECUFW_flag = 2;
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
  INT8U buf[20], map_x;
	INT16U SendCount = 0, buf16b;
  INT16S sbuf16b;
	INT32U buf32b, addr;

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
        if(Setting.ECUFW_flag) {
          Setting.ECUFW_flag = 0;
          addr = DRV_SETTING_PARM + offsetof(st_Setting, ECUFW_flag);
          FRAMMultiWrite((u8 *)&Setting.ECUFW_flag, addr, 1);
        }
        nDlStatus = 3;
        printk("DL soucess Ok....\n");
      }
      else
      {
        nDlStatus = 4;
        printk("DL soucess Error....\n");
      }
    }
    else if(Can_RxMsg.id == 0x30) {
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[0], 2);
      ReadValue.DpfValue.T[0] =  buf16b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[2], 2);
      ReadValue.DpfValue.T[1] =  buf16b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[4], 2);
      ReadValue.DpfValue.T[2] =  buf16b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[6], 2);
      ReadValue.DpfValue.T[3] =  buf16b;
    }
    else if(Can_RxMsg.id == 0x31) {
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[0], 2);
      ReadValue.DpfValue.P =  buf16b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[2], 2);
      ReadValue.DpfValue.Rate =  buf16b;    
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[4], 2);
      //
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[6], 2);
      ReadValue.DpfValue.IHC =  buf16b;
    }
    else if(Can_RxMsg.id == 0x32) {
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[0], 2);
      ReadValue.DpfValue.f_REG =  buf16b;
      ReadValue.DpfValue.CarType = Can_RxMsg.data[2]; 
      ReadValue.DpfValue.FPD = Can_RxMsg.data[3]; 
      ReadValue.DpfValue.Error.B = Can_RxMsg.data[4]; 
      ReadValue.DpfValue.Check.B = Can_RxMsg.data[5];
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[6], 2);
      ReadValue.DpfValue.t_REG =  buf16b;      
    }
    else if(Can_RxMsg.id == 0x33) {
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[0], 2);
      ReadValue.DpfValue.n_REG =  buf16b;
      ReadValue.DpfValue.Sig.B = Can_RxMsg.data[2];
      ReadValue.DpfValue.Sts.B = Can_RxMsg.data[3];
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[4], 2);
      ReadValue.DpfValue.t_OffSCR =  buf16b;
      ReadValue.DpfValue.RegenStartKey = Can_RxMsg.data[6];
      ReadValue.DpfValue.Lock = Can_RxMsg.data[7];
    }
    else if(Can_RxMsg.id == 0x34) {
      memcpy( (char*)&buf32b, (char *)&Can_RxMsg.data[0], 4);
      ReadValue.DpfValue.DrvTime =  buf32b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[4], 2);
      ReadValue.DpfValue.baseP1 =  buf16b;
    } 
    else if(Can_RxMsg.id == 0x35) {
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[0], 2);
      ReadValue.DpfValue.f_SCR =  buf16b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[2], 2);
      ReadValue.DpfValue.t_SCR =  buf16b;
      memcpy( (char*)&buf16b, (char *)&Can_RxMsg.data[4], 2);
      ReadValue.DpfValue.n_SCR =  buf16b;
    }     
    else if(Can_RxMsg.id == 0x98) {//ID : 0x98
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      ScrValue.curXk = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      ScrValue.Xc = buf32b;
    }
    else if(Can_RxMsg.id == 0x99) {//ID : 0x99
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      ScrValue.H1k = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      ScrValue.H2k = buf32b;
    }
  }
  /////////////////////////////////////////ID 0x100~0x14F
  if(Can_RxMsg.id >= 0x100 && Can_RxMsg.id < 0x150) {
    if(Can_RxMsg.id == 0x100) {//ID : 0x100
//       memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
//       ReadValue.Ver   = buf16b;
       ReadValue.Ver = (Can_RxMsg.data[1] / 0x10*1000) + (Can_RxMsg.data[1] % 0x10*100) + (Can_RxMsg.data[0] / 0x10*10) + (Can_RxMsg.data[0] % 0x10);
       memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
       ReadValue.TankTemp = buf16b;      
    }
    else if(Can_RxMsg.id == 0x101) {
      ReadValue.Err.B = Can_RxMsg.data[0];
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      ReadValue.Chk.W = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      ReadValue.Sig.W = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      ReadValue.UreaQuality = buf16b;
    }
    else if(Can_RxMsg.id == 0x102) {
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
      ReadValue.NOxIn = (INT16S)buf16b;
      if(ReadValue.NOxIn < 0) ReadValue.NOxIn = 0;
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      ReadValue.NOxOut = (INT16S)buf16b;
      if(ReadValue.NOxOut < 0) ReadValue.NOxOut = 0;
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      ReadValue.MAF = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      ReadValue.Maf_temp = buf16b;
      if(ReadValue.Maf_temp < 0) ReadValue.Maf_temp = 0; 
    }
    else if(Can_RxMsg.id == 0x103) {
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
      ReadValue.TempIn = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      ReadValue.TempOut = buf16b;
  		memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      ReadValue.Tavg = buf16b;
  		//memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      //ReadValue.TankTemp = buf16b;
    }
    else if(Can_RxMsg.id == 0x104) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      ReadValue.DosingRatehouer = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);  //테스트후 원상 복귀
      ReadValue.TotalDosingRate = buf32b;
    }
    else if(Can_RxMsg.id == 0x105) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      ScrValue.Kl = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      ScrValue.Kp = buf32b;
    }
    else if(Can_RxMsg.id == 0x106) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      ScrValue.Vk = buf32b;
      memcpy((char *)&buf32b,&Can_RxMsg.data[4],4);
      ScrValue.Gamma = buf32b;
    }
    else if(Can_RxMsg.id == 0x107) {
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
  		ReadValue.BV = buf16b;
 		  //memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      ReadValue.UreaLevel = Can_RxMsg.data[2];
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      ReadValue.SupplyP = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      ReadValue.DosingDuty = buf16b;
    }
    else if(Can_RxMsg.id == 0x108) {
      memcpy((char *)&buf32b,&Can_RxMsg.data[0],4);
      ScrValue.curXh = buf32b;   
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      ReadValue.StatusAlpha = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[6],2);
      ReadValue.Reduction = buf16b;
    }
    else if(Can_RxMsg.id == 0x109) { 
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
      NoxO2[0] = buf16b;
      memcpy((char *)&buf16b,&Can_RxMsg.data[2],2);
      NoxO2[1] = buf16b;
      NoxFail[0] = Can_RxMsg.data[4];
      NoxFail[1] = Can_RxMsg.data[5];
      NoxErr[0] = Can_RxMsg.data[6];
      NoxErr[1] = Can_RxMsg.data[7];
    }
    else if(Can_RxMsg.id == 0x110) {
      NoxData[0][0] = Can_RxMsg.data[0];
      NoxData[0][1] = Can_RxMsg.data[1];
      NoxData[0][2] = Can_RxMsg.data[2];
      NoxData[1][0] = Can_RxMsg.data[4];
      NoxData[1][1] = Can_RxMsg.data[5];
      NoxData[1][2] = Can_RxMsg.data[6];
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
			ReadValue.Formula1 = sbuf16b;
 		  memcpy((char *)&sbuf16b,&Can_RxMsg.data[2],2);
			ReadValue.Formula2 = sbuf16b;
 		  memcpy((char *)&sbuf16b,&Can_RxMsg.data[4],2);
			ReadValue.Formula3 = sbuf16b;
    }
    else if(Can_RxMsg.id == 0x11A) {
			ReadValue.LimitStep = Can_RxMsg.data[0];
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
  /////////////////////////////////////////ID 0x160~0x1A9
  if(Can_RxMsg.id > 0x160 && Can_RxMsg.id < 0x1A9) {
    map_x = (INT8U)(Can_RxMsg.id - 0x161);
    memcpy( (char *)&ScrMap.AlpaMap.row[map_x].col[0], &Can_RxMsg.data[0], CANDATALEN);
		SendCount = 0;
    if(map_x == 71 && ServerRW) {
      ServerRW++;
      buf[0] = 0x01;
    	SendCount = MakePackMassage(&msg, 0x1B0, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
    }
  }
  /////////////////////////////////////////ID 0x1B0~0x1B7
  if(Can_RxMsg.id > 0x1B0 && Can_RxMsg.id < 0x1B7) {
    map_x = (INT8U)(Can_RxMsg.id - 0x1B1);
    memcpy( (char *)&ScrMap.BaseTemp[map_x * 4], &Can_RxMsg.data[0], CANDATALEN);
//    if(map_x == 5 && ((ServerRW & 0x03) == 0x03)) ServerRW++;
		SendCount = 0;
  }
  /////////////////////////////////////////ID 0x1C0~0x1C7
  if(Can_RxMsg.id > 0x1C0 && Can_RxMsg.id < 0x1C7) {
    map_x = (INT8U)(Can_RxMsg.id - 0x1C1);
    memcpy( (char *)&ScrMap.BaseMaf[map_x * 4], &Can_RxMsg.data[0], CANDATALEN);
    if(map_x == 5 && ServerRW) ServerRW++;
		SendCount = 0;
  }
  /////////////////////////////////////////ID 0x140~0x147
  if(Can_RxMsg.id > 0x140 && Can_RxMsg.id < 0x147) {
    map_x = (INT8U)(Can_RxMsg.id - 0x141);
    memcpy( (char *)&SysCtl+(map_x*8), &Can_RxMsg.data[0], CANDATALEN);
    if(map_x == 5 && ServerRW) {
      ServerRW++;
    }
		SendCount = 0;
  }
  /////////////////////////////////////////ID 0x200~
  if(Can_RxMsg.id >= 0x200) {
    if(Can_RxMsg.id == 0x200) {
      memcpy(buf, &ModemInfo.LpIP, 6);
      SendCount = MakePackMassage(&msg, 0x201, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
    }
    else if(Can_RxMsg.id == 0x202) {
     memcpy((char *)&ModemInfo.LpIP, &Can_RxMsg.data[0], 4);
     memcpy((char *)&ModemInfo.LpPort, &Can_RxMsg.data[4], 2);
     addr = MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo;
     FRAMMultiWrite((u8 *)&ModemInfo.LpIP, addr, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
    }
    else if(Can_RxMsg.id == 0x703) { //DPF Crack Signal
      ReadValue.DPF_Crack = Can_RxMsg.data[0];
    }
    else if(Can_RxMsg.id == 0x705) { //PM Sensor Error    
      memcpy((char *)&buf16b,&Can_RxMsg.data[0],2);
      ReadValue.PM1_Error.K = buf16b;  
      memcpy((char *)&buf16b,&Can_RxMsg.data[4],2);
      ReadValue.PM2_Error.K = buf16b;
    }    
//    else if(Can_RxMsg.id == 0x14FF0055) {           //PM 센서
//      memcpy((char *)&buf16b, &Can_RxMsg.data[0], 2 );
//      ReadValue.PM_Senser1 = buf16b;
//      memcpy((char *)&buf16b, &Can_RxMsg.data[2], 2 );
//      ReadValue.PM_Senser2 = buf16b;
//      memcpy((char *)&buf16b, &Can_RxMsg.data[4], 2 );
//      ReadValue.PM_Senser3 = buf16b;
//      memcpy((char *)&buf16b, &Can_RxMsg.data[6], 2 );
//      ReadValue.PM_Senser4 = buf16b;
//    }  
  }
	if (SendCount )
		SendCanMassage(&msg);  
}

/*
********************************************************************************
* Description : Server_TxData_Loop
* Arguments   : teCommand com, unsigned short idx, unsigned short cnt, unsigned int len, u8* data
* Return      : 
* Note        : CAN data process
******************************************************************************** 
*/
void Server_TxData_Loop(teCommand com, unsigned short idx, unsigned short cnt, unsigned int len, u8* data)
{
  Modem_TxData_Loop(com, idx, cnt, len, data);
  ServerRW = 0x00;
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
  INT8U buf[8];
  INT16U CANid;
  INT32U Tmrtime = 500;
	CanTxMsg msg;	
	sysinfo_t *s = (sysinfo_t *)p_arg 	;

	Init_CANcommRxProc(CAN1_PORT);
	CanSemaphore_Init();
  
	Flags1.RegenOBD = 0;
	Flags1.RegenOffOBD = 0;		
	for(;;)
	{
		s->twatchCanFlag = 1;
		if(TmrChk(TMR_CANTRAPINTERVAL) == 0) { // Slave로 GPS, 시간 전송 
      if(ServerRW) Tmrtime = 50;
      else Tmrtime = 500;
      
      if(ServerRW == 0x01 || ServerRW == 0x65) {      // DCU MAP Read
        buf[0] = 0x01;
        buf[1] = 0x00;
        CANid = 0x160;
        ServerRW++;
      }
      else if(ServerRW > 0x10 && ServerRW < 0x65) {   //DCU MAP Write:ServerRw+82
        memcpy( (char *)&buf, (char *)&ScrMap+((ServerRW-0x11)*8), CANDATALEN);
        if((ServerRW-0x11) < 6) {
          CANid = 0x1B1+(ServerRW-0x11);
        }
        else if((ServerRW-0x17) < 6) {
          CANid = 0x1C1+(ServerRW-0x17);
        }
        else if((ServerRW-0x1D) < 72) {
          CANid = 0x161+(ServerRW-0x1D);
        }
        ServerRW++;
      }
      else if(ServerRW == 0xA1 || ServerRW == 0xB7) { // DCU Parameter Read
        buf[0] = 0x01;
        buf[1] = 0x00;
        CANid = 0x140;
        ServerRW++;
      }
      else if(ServerRW > 0xB0 && ServerRW < 0xB7) {   //DCU Parameter Write:ServerRw+6
        Tmrtime = 100;
        memcpy( (char *)&buf, (char *)&SysCtl+((ServerRW-0xB1)*8), CANDATALEN);
        CANid = 0x141+(ServerRW-0xB1);
        ServerRW++;
      }
      else if(ServerRW == 0x04) {
          Server_TxData_Loop(CM_P1SD, 0, 1, sizeof(stScrMap), (u8*)&ScrMap);
      }
      else if(ServerRW == 0x68) {
          Server_TxData_Loop(CM_P1SC, 0, 1, sizeof(stScrMap), (u8*)&ScrMap);
      }
      else if(ServerRW == 0xA3) {
          Server_TxData_Loop(CM_P2SD, 0, 1, sizeof(stSysControl), (u8*)&SysCtl);
      }
      else if(ServerRW == 0xB9) {
          Server_TxData_Loop(CM_P2SC, 0, 1, sizeof(stSysControl), (u8*)&SysCtl);
      }
      else {
        buf[0] = ReadValue.Speed;
        buf[1] = GPS_Valid;
        buf[2] = s->tm_ptr.tm_year;
        buf[3] = s->tm_ptr.tm_mon;
        buf[4] = s->tm_ptr.tm_mday;
        buf[5] = s->tm_ptr.tm_hour;
        buf[6] = s->tm_ptr.tm_min;
        buf[7] = s->tm_ptr.tm_sec;
        CANid = 0x119;
        if(Flags1.RegenOBD || Flags1.RegenOffOBD) {
          if(Flags1.RegenOBD) buf[0] = 0x01;
          else if(Flags1.RegenOffOBD) buf[0] = 0x00;
          buf[1] = 0x00;
          CANid = 0x20;
          Flags1.RegenOBD = 0;
          Flags1.RegenOffOBD = 0;
        }
      }
      MakePackMassage(&msg, CANid, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
      SendCanMassage(&msg);
      TmrSetT(TMR_CANTRAPINTERVAL, Tmrtime);
      TmrStart(TMR_CANTRAPINTERVAL);
    }
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

/*
********************************************************************************
* Description : Send SaveFlag to SlaveOBD 
* Arguments   : *none
* Return      : 
* Note        : 1 Second Proccess 
******************************************************************************** 
*/
void SendSaveFlag(void)
{
  INT8U buf[8];
	CanTxMsg msg;	
 
   buf[0] = 1;
   MakePackMassage(&msg, 0x126, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
   SendCanMassage(&msg);	
}

/*
********************************************************************************
* Description : Send Reset Signal to Device 
* Arguments   : *none
* Return      : 
* Note        : Press Reset Button 
******************************************************************************** 
*/
void SendResetFlag(void)
{
  INT8U buf[8];
	CanTxMsg msg;	
 
   buf[0] = 1;
   MakePackMassage(&msg, 0x704, CAN_STAN, CAN_RTR_DATA, buf, CANDATALEN);
   SendCanMassage(&msg);	
}
/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : modem_tool.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : usb protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define MODEM_TOOL_GLOBALS
#include "includes.h"
#include "modem_Loop.h"
//#include "modem_Hanuri.h"
//#include "modem_GVN.h"
//#include "modem_DMS.h"

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

typedef struct{
	char cmd;
	char type;
	char systype;
	char msg;
}protocol_t;

#define MT_HREPORT				0x10
#define MT_HSET					  0x20
#define MT_LREQUEST				0x01
#define MT_LRESPONSE			0x02

#define MT_START_PACKET		0x7E
#define MT_END_PACKET			0x7F

#define MT_CHKSUM_ERR			0x01
#define MT_SYS_TYPE_ERR		0x02

#define MT_PACKET_BUFFSZ   1024

static char MtRxPacketData[MT_PACKET_BUFFSZ];

u8 MtRxStep, MtCheckSum;
u8 MtDecodeFlag, MtErrFlag, MtErrCode;
u16 MtRxIdx, MtPacketLen;
st_BufPara DTGPara;

/*
********************************************************************************
* Description : MtSendPacket
* Arguments   : cmd, type,	systype, *ptarget, *psource, dataLen
* Return      : 
* Note        : Send packet
******************************************************************************** 
*/
void MtSendPacket(char cmd, char type,	char systype, char *psource, int dataLen)
{
  char ptarget[64];
	char checksum=0,tmp=0;
	int nCount=0,i,packlen=dataLen + 3;

	ptarget[nCount++] = MT_START_PACKET;							// 0x7E
	
	tmp = packlen & 0xFF ;			// len
	ptarget[nCount++] = tmp ;
	checksum += tmp;

	tmp = (packlen >> 8) & 0xFF ;	// len
	ptarget[nCount++] = tmp;
	checksum += tmp;

	ptarget[nCount++] = cmd;
	checksum += cmd;

	ptarget[nCount++] = type;
	checksum += type;

	ptarget[nCount++] = systype;
	checksum += systype;

  UART_SendDataStr(CLI_UART_PORT, (u8*)ptarget, nCount);
  nCount = 0;
    
	for(i=0; i< dataLen; i++)
	{
		ptarget[nCount++] = psource[i];
		checksum +=psource[i];
    if(i > 0 && i%60 == 0) {
      UART_SendDataStr(CLI_UART_PORT, (u8*)ptarget, nCount);
      nCount = 0;
    }
	}
	ptarget[nCount++] = checksum;
	ptarget[nCount++] = MT_END_PACKET;								// 0x7F
  
  UART_SendDataStr(CLI_UART_PORT, (u8*)ptarget, nCount);
}

/*
********************************************************************************
* Description : MtRxProcess
* Arguments   : none
* Return      : 
* Note        : Rx process
******************************************************************************** 
*/
void MtRxProcess(u8 rdata)
{
  
  if(MtDecodeFlag || MtErrFlag) return;
  
  switch(MtRxStep) {
    case 0:
      if(rdata == MT_START_PACKET) {
          MtRxStep = 1;
          MtPacketLen = 0;
          MtCheckSum = 0;
      }
      MtRxIdx = 0;
      break;
      
    case 1:
      MtRxIdx++;
      if(MtRxIdx == 1)  {
        MtPacketLen = rdata;
        MtCheckSum += rdata;
      }
      if(MtRxIdx == 2) {
        MtPacketLen = (rdata<<8)+MtPacketLen;
        MtCheckSum += rdata;
        if(MtPacketLen > MT_PACKET_BUFFSZ) 
          MtRxStep = 0;
        else
          MtRxStep = 2;
        MtRxIdx = 0;
      }
      break;

    case 2:
      if(MtRxIdx < MT_PACKET_BUFFSZ || MtRxIdx <= MtPacketLen) {
        if(MtRxIdx < MtPacketLen) {
          MtRxPacketData[MtRxIdx++] = rdata;
          MtCheckSum += rdata;
        }
        else if(MtRxIdx == MtPacketLen) {
          if(MtCheckSum == rdata)
            MtRxStep = 3;
          else
            MtRxStep = 10;
        }
      }
      else {
        MtRxStep = 0;
        MtRxIdx = 0;
      }
      break;

    case 3:
      if(rdata == MT_END_PACKET) {
        MtDecodeFlag = 1;
        MtRxStep = 0;
      }
      MtRxIdx = 0;
      break;

    case 10:
      MtErrCode = MT_CHKSUM_ERR;
      MtErrFlag = 1;
      MtRxStep = 0;
      MtPacketLen = 0;
      MtRxIdx = 0;
      break;
      
  }
  
}

/*
********************************************************************************
* Description : MtRxDecode
* Arguments   : none
* Return      : 
* Note        : Rx Decode
******************************************************************************** 
*/
void MtRxDecode(void)
{
	protocol_t *ptPacket=NULL;
  u16 packetDataLen = 0;
	u8 nMsgType=0, rc = 0;
  u16 sendCount;
  u32 addr;//, size;

//  char dummy = 0;  
  
  if(MtErrFlag) {
    MtErrFlag = 0;
    MtTxBuff[0] = 0x00;
    MtTxBuff[1] = MtErrCode; 						        // error code
    MtTxBuff[2] = MtRxPacketData[0];					// receive command
    MtSendPacket(0xF0, 0x11, 0x04, MtTxBuff, 3);
  }
  
  if(MtDecodeFlag) {
    MtDecodeFlag = 0;
    
		ptPacket = (protocol_t *)&MtRxPacketData[0];
    packetDataLen = MtPacketLen-3;
    
    if(ptPacket->type == (MT_HREPORT | MT_LREQUEST))
      nMsgType = (MT_HREPORT | MT_LRESPONSE);
    else
      nMsgType = (MT_HSET | MT_LRESPONSE);
    
    memset((u8*)MtTxBuff, 0, 256);
    sendCount = 0;
      
    switch(ptPacket->cmd)
    {
    case 0x30:        // Test Start
      MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);    
      break;
    case 0x39:        // Serial Setting
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(Setting.Serial_No);
        memcpy(MtTxBuff, Setting.Serial_No, sendCount);
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	           
      }
      else {
        memset(Setting.Serial_No, 0, sizeof(Setting.Serial_No));
        memcpy(Setting.Serial_No, (u8 *)&(ptPacket->msg), packetDataLen);
        addr = DRV_SETTING_PARM + (u8 *)&Setting.Serial_No - (u8 *)&Setting;
        FRAMMultiWrite((u8 *)&Setting.Serial_No, addr, sizeof(Setting.Serial_No));          
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	
      }  
      break;
    case 0x40:        // Factory Reset
      MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	     
      Modem_Parameter_Reset(); 
      NVIC_SystemReset();
      break;
    case 0x49:
      CMPortEnable = ptPacket->msg;
      break;
    case 0x54:        // FOTA Request
      MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	    
      FotaFlag = 1;
      break;
    case 0x55:        // APN 설정
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(APN_Name);
        memcpy(MtTxBuff, APN_Name, sendCount);
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	           
//        MtReadAPNFlag = 1;
      }
      else {
        if(packetDataLen > 0 && packetDataLen < 32) {
          memset(APN_Name,0,32);
          memcpy(APN_Name, (u8 *)&(ptPacket->msg), packetDataLen);
          MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	     
        }
        FRAMMultiWrite((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
        MtChangeAPNFlag = 1;
     }  
     break;

    case 0x58:        // 접속 주기
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(u16);
        memcpy(MtTxBuff, (u8*)&ModemInfo.DataServerInterval, sendCount);
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	           
      }
      else {
        ModemInfo.DataServerInterval = 0;
        memcpy((u8 *)&ModemInfo.DataServerInterval, (u8 *)&(ptPacket->msg), sizeof(u16));
        sendCount = 0;
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	     
        addr = MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo;
        FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, addr, sizeof(ModemInfo.DataServerInterval));
      }  
      break;
    case 0x59:
      sendCount = 0;
      if(ptPacket->type == 0x11) {
        sendCount = 9;//sizeof(PCB_HW_Ver);
        memcpy(MtTxBuff, (u8*)&PCB_HW_Ver, sendCount);
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	           
      }    
      break;
    case 0x5A:
      sendCount = 0;
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(Fota_FW_Ver);
        memcpy(MtTxBuff, (u8*)&Fota_FW_Ver, sendCount);
        MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	           
      }    
      break;
    case 0x5D:
      DTG_SendFlag = ptPacket->msg;
      break;
    case 0x5E: 
      MtSendPacket(ptPacket->cmd, 0x12, 0x01, (char *)&CAN_Enable, sizeof(stCanEnable));
      break;
    case 0x63:        // 호스트 서버  설정
      if(ptPacket->type == 0x11) {
        if(ptPacket->systype == 0x01) {
          sendCount = sizeof(HostSvr.SvrName);
          memcpy(MtTxBuff, HostSvr.SvrName, sendCount);
          MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	           
        }
        else {
          sendCount = sizeof(u32);
          memcpy(MtTxBuff, (char*)&HostSvr.SvrPort, sendCount);
          MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	                   
        }
      }
      else {
        if(ptPacket->systype == 0x01) {
          if(packetDataLen>0 && packetDataLen<60) {
            memset(HostSvr.SvrName,0,60);
            memcpy(HostSvr.SvrName, (u8 *)&(ptPacket->msg), packetDataLen);
            sendCount = 0;
            MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	     
          }
        }
        else {
            memset((u8*)&HostSvr.SvrPort,0,sizeof(u32));
            memcpy((u8*)&HostSvr.SvrPort, (u8 *)&(ptPacket->msg), sizeof(u32));
            sendCount = 0;
            MtSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, MtTxBuff, sendCount);	     
        }
        if(Setting.ExtModemMode == LOOP_PROTOCOL) {
          rc = IpToInt(HostSvr.SvrName);
          if(rc) {            
            memcpy(&ModemInfo.LpIP,IP,sizeof(IP)); 
            addr = MODEM_INFO + offsetof(tsModemInfo,LpIP);   
            FRAMMultiWrite((u8 *)&ModemInfo.LpIP, addr, sizeof(IP)); 
            ModemInfo.LpPort =  HostSvr.SvrPort;
            addr = MODEM_INFO + offsetof(tsModemInfo,LpPort);   
            FRAMMultiWrite((u8 *)&ModemInfo.LpPort, addr, sizeof(ModemInfo.LpPort));  
          }        
        }        
        FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
        SendSettingFlag = 1;
      }
      break;
    }
  }
  else if(MtTxAPNFlag) {
    sendCount = sizeof(APN_Name);
    memcpy(MtTxBuff, APN_Name, sendCount);
    MtSendPacket(0x55, 0x12, 01, MtTxBuff, sendCount);	           
    MtTxAPNFlag = 0;
  }
}

/*
********************************************************************************
* Description : DTG_SecData_Send
* Arguments   : none
* Return      : 
* Note        : DTG Sec data send
******************************************************************************** 
*/
void DTG_SecData_Send(void)
{
  MtSendPacket(0x5D, 0x12, 0x01, (char*)&SecData, sizeof(tsSecData));
}

/*
********************************************************************************
* Description : ModemTool_Process
* Arguments   : none
* Return      : 
* Note        : Modem tool Process
******************************************************************************** 
*/
void ModemTool_Process(void)
{
  u8 rdata;
	u16 tmptail;
  

	while (CLI_RxHead != CLI_RxTail ) { 
    tmptail = (CLI_RxTail + 1) & CLI_RX_BUFFER_MASK;/* Calculate buffer index */
    
    CLI_RxTail = tmptail;                /* Store new index */
    
    rdata = CLI_RxBuf[tmptail];           /* Return data */    

    MtRxProcess(rdata);
  
    MtRxDecode();
  }
}




/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/

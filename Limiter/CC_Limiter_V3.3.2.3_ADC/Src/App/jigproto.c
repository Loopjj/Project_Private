/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : jigproto.c
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

#define JIGPROTOCOL_GLOBALS
#include "includes.h"
#include "modem_Loop.h"
#include "modem_Hanuri.h"
#include "modem_GVN.h"

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

#define JIGPROTO_HREPORT				0x10
#define JIGPROTO_HSET					  0x20
#define JIGPROTO_LREQUEST				0x01
#define JIGPROTO_LRESPONSE			0x02

#define JIGPROTO_START_PACKET				0x7E
#define JIGPROTO_END_PACKET					0x7F

#define JIGPROTO_CHKSUM_ERR					0x01
#define JIGPROTO_SYS_TYPE_ERR				0x02

#define JIG_PACKET_BUFFSZ   1024

static char JigRxPacketData[JIG_PACKET_BUFFSZ];

u8 JigRxStep, JigCheckSum;
u8 JigDecodeFlag, JigErrFlag, JigErrCode;
u16 JigRxIdx, JigPacketLen;
char JigTxBuff[200];


/*
********************************************************************************
* Description : JigSendPacket
* Arguments   : cmd, type,	systype, *ptarget, *psource, dataLen
* Return      : 
* Note        : Send packet
******************************************************************************** 
*/
void JigSendPacket(char cmd, char type,	char systype, char *psource, int dataLen)
{
  char ptarget[64];
	char checksum=0,tmp=0;
	int nCount=0,i,packlen=dataLen + 3;

	ptarget[nCount++] = JIGPROTO_START_PACKET;							// 0x7E
	
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
	ptarget[nCount++] = JIGPROTO_END_PACKET;								// 0x7F
  
  UART_SendDataStr(CLI_UART_PORT, (u8*)ptarget, nCount);
}


void JigRxProcess(u8 rdata)
{
  
  if(JigDecodeFlag || JigErrFlag) return;
  
  switch(JigRxStep) {
    case 0:
      if(rdata == JIGPROTO_START_PACKET) {
          JigRxStep = 1;
          JigPacketLen = 0;
          JigCheckSum = 0;
      }
      JigRxIdx = 0;
      break;
      
    case 1:
      JigRxIdx++;
      if(JigRxIdx == 1)  {
        JigPacketLen = rdata;
        JigCheckSum += rdata;
      }
      if(JigRxIdx == 2) {
        JigPacketLen = (rdata<<8)+JigPacketLen;
        JigCheckSum += rdata;
        if(JigPacketLen > JIG_PACKET_BUFFSZ) 
          JigRxStep = 0;
        else
          JigRxStep = 2;
        JigRxIdx = 0;
      }
      break;

    case 2:
      if(JigRxIdx < JIG_PACKET_BUFFSZ || JigRxIdx <= JigPacketLen) {
        if(JigRxIdx < JigPacketLen) {
          JigRxPacketData[JigRxIdx++] = rdata;
          JigCheckSum += rdata;
        }
        else if(JigRxIdx == JigPacketLen) {
          if(JigCheckSum == rdata)
            JigRxStep = 3;
          else
            JigRxStep = 10;
        }
      }
      else {
        JigRxStep = 0;
        JigRxIdx = 0;
      }
      break;

    case 3:
      if(rdata == JIGPROTO_END_PACKET) {
        JigDecodeFlag = 1;
        JigRxStep = 0;
      }
      JigRxIdx = 0;
      break;

    case 10:
      JigErrCode = JIGPROTO_CHKSUM_ERR;
      JigErrFlag = 1;
      JigRxStep = 0;
      JigPacketLen = 0;
      JigRxIdx = 0;
      break;
      
  }
  
}

void JigRxDecode(void)
{
	protocol_t *ptPacket=NULL;
  u16 packetDataLen = 0;
	u8 nMsgType=0;
  u16 sendCount;
  u16 addr;

  if(JigErrFlag) {
    JigErrFlag = 0;
    JigTxBuff[0] = 0x00;
    JigTxBuff[1] = JigErrCode; 						        // error code
    JigTxBuff[2] = JigRxPacketData[0];					// receive command
    JigSendPacket(0xF0, 0x11, 0x04, JigTxBuff, 3);
  }
  
  if(JigDecodeFlag) {
    JigDecodeFlag = 0;
    
		ptPacket = (protocol_t *)&JigRxPacketData[0];
    packetDataLen = JigPacketLen-3;
    
    if(ptPacket->type == (JIGPROTO_HREPORT | JIGPROTO_LREQUEST))
      nMsgType = (JIGPROTO_HREPORT | JIGPROTO_LRESPONSE);
    else
      nMsgType = (JIGPROTO_HSET | JIGPROTO_LRESPONSE);
    
    memset((u8*)JigTxBuff, 0, 200);
    sendCount = 0;
      
    switch(ptPacket->cmd)
    {
    case 0x30:        // Test Start
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);    
      break;
    case 0x40:        // Factory Reset
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
      Modem_Parameter_Reset(); 
      NVIC_SystemReset();
      break;
    case 0x50:        // ON Noti
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	    
      OnNotiFlag = 1;
      break;
    case 0x51:        // OFF Noti
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	    
      OffNotiFlag = 1;
      break;
    case 0x52:        // 단말기 품질 정보
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	    
      QualNotiFlag = 1;
      break;
    case 0x53:        // 모뎀 품질 정보 
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	    
      QualNotiFlag = 3;
      break;
    case 0x54:        // FOTA Request
      JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	    
      FotaFlag = 1;
      break;
    case 0x55:        // APN 설정
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(APN_Name);
        memcpy(JigTxBuff, APN_Name, sendCount);
        JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
//        JigReadAPNFlag = 1;
      }
      else {
        if(packetDataLen > 0 && packetDataLen < 32) {
          memset(APN_Name,0,32);
          memcpy(APN_Name, (u8 *)&(ptPacket->msg), packetDataLen);
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
        }
        FRAMMultiWrite((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
        JigChangeAPNFlag = 1;
     }  
     break;
    case 0x56:        // DMS 설정
      if(ptPacket->type == 0x11) {
        if(ptPacket->systype == 0x01) {
          sendCount = sizeof(DmSvr.SvrName);
          memcpy(JigTxBuff, DmSvr.SvrName, sendCount);
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
        }
        else {
          sendCount = sizeof(u32);
          memcpy(JigTxBuff, (char*)&DmSvr.SvrPort, sendCount);
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	                   
        }
      }
      else {
        if(ptPacket->systype == 0x01) {
          if(packetDataLen>0 && packetDataLen<60) {
            memset(DmSvr.SvrName,0,60);
            memcpy(DmSvr.SvrName, (u8 *)&(ptPacket->msg), packetDataLen);
            sendCount = 0;
            JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
          }
        }
        else {
            memset((u8*)&DmSvr.SvrPort,0,sizeof(u32));
            memcpy((u8*)&DmSvr.SvrPort, (u8 *)&(ptPacket->msg), sizeof(u32));
            sendCount = 0;
            JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
        }
        FRAMMultiWrite((u8 *)&DmSvr, DMS_SVR_INFO, sizeof(tsSvrInfo));
      }
      break;
    case 0x57:        // Quality 설정
      if(ptPacket->type == 0x11) {
        if(ptPacket->systype == 0x01) {
          sendCount = sizeof(QualSvr.SvrName);
          memcpy(JigTxBuff, QualSvr.SvrName, sendCount);
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
        }
        else {
          sendCount = sizeof(u32);
          memcpy(JigTxBuff, (u8*)&QualSvr.SvrPort, sendCount);
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	                   
        }
      }
      else {
        if(ptPacket->systype == 0x01) {
          if(packetDataLen>0 && packetDataLen<60) {
            memset(QualSvr.SvrName,0,60);
            memcpy(QualSvr.SvrName, (u8 *)&(ptPacket->msg), packetDataLen);
            sendCount = 0;
            JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
          }
        }
        else {
            memset((char*)&QualSvr.SvrPort,0,sizeof(u32));
            memcpy((u8*)&QualSvr.SvrPort, (u8 *)&(ptPacket->msg), sizeof(u32));
            sendCount = 0;
            JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
        }
        FRAMMultiWrite((u8 *)&QualSvr, QUAL_SVR_INFO, sizeof(tsSvrInfo));
      }
      break;
    case 0x58:        // 접속 주기
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(u16);
        memcpy(JigTxBuff, (u8*)&ModemInfo.DataServerInterval, sendCount);
        JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
      }
      else {
        ModemInfo.DataServerInterval = 0;
        memcpy((u8 *)&ModemInfo.DataServerInterval, (u8 *)&(ptPacket->msg), sizeof(u16));
        sendCount = 0;
        JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	     
        addr = MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo;
        FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, addr, sizeof(ModemInfo.DataServerInterval));
      }  
      break;
    case 0x59:
      sendCount = 0;
      if(ptPacket->type == 0x11) {
        sendCount = 9;//sizeof(PCB_HW_Ver);
        memcpy(JigTxBuff, (u8*)&PCB_HW_Ver, sendCount);
        JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
      }    
      break;
    case 0x5A:
      sendCount = 0;
      if(ptPacket->type == 0x11) {
        sendCount = sizeof(Fota_FW_Ver);
        memcpy(JigTxBuff, (u8*)&Fota_FW_Ver, sendCount);
        JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
      }    
      break;
    case 0x5B:
      if(ptPacket->type == 0x11) {
          sprintf(JigTxBuff, "HTTP://%s",URLInfo.URLName);
          sendCount = StringLen(JigTxBuff,200);
          if(URLInfo.Port > 0 && URLInfo.Port != 80) {
            sprintf(&JigTxBuff[sendCount], ":%u",URLInfo.Port);
            sendCount = StringLen(JigTxBuff,200);
          }
          if(URLInfo.SubDir[0] != 0) {
            sprintf(&JigTxBuff[sendCount], "/%s", URLInfo.SubDir);
            sendCount = StringLen(JigTxBuff,200);
          }
          if(URLInfo.CompanyID[0] != 0) {
            sprintf(&JigTxBuff[sendCount], "/%s", URLInfo.CompanyID);
            sendCount = StringLen(JigTxBuff,200);
          }
          if(URLInfo.DataDir[0] != 0) {
            sprintf(&JigTxBuff[sendCount], "/%s", URLInfo.DataDir);
            sendCount = StringLen(JigTxBuff,200);
          }
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
        }
        else {
          ServerDomain_Process_GVN((char *)&(ptPacket->msg), packetDataLen, 1);
          sendCount = 0;
          JigSendPacket(ptPacket->cmd, nMsgType, ptPacket->systype, JigTxBuff, sendCount);	           
        }
        break;
    }
  }
  else if(JigTxAPNFlag) {
    sendCount = sizeof(APN_Name);
    memcpy(JigTxBuff, APN_Name, sendCount);
    JigSendPacket(0x55, 0x12, 01, JigTxBuff, sendCount);	           
    JigTxAPNFlag = 0;
  }
}

void JIG_task(void)
{
  u8 rdata;
	u16 tmptail;
  

	while (CLI_RxHead != CLI_RxTail ) { 
    tmptail = (CLI_RxTail + 1) & CLI_RX_BUFFER_MASK;/* Calculate buffer index */
    
    CLI_RxTail = tmptail;                /* Store new index */
    
    rdata = CLI_RxBuf[tmptail];           /* Return data */    

    JigRxProcess(rdata);
  
    JigRxDecode();
  }
}




/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/

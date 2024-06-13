/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : guiproto.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : gui protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define GUIPROTOCOL_GLOBALS
#include "includes.h"


typedef union {
  char self;
  struct {  
    char simcard  :1;
    char modem    :1;
    char din1     :1;
    char din2     :1;
    char keyon    :1;
    char flash    :1;
    char fram     :1;
    char gps      :1;
  };
} resSelfTest;

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static char guiTxbuff[GUI_BUFFSZ];
static char guiRxbuff[GUI_BUFFSZ];

static commProcess_t GUIProto;

/*
********************************************************************************
* Description : Init_GUIcommRxProc
* Arguments   : port
* Return      : 
* Note        : GUI communication initialize
******************************************************************************** 
*/
void Init_GUIcommRxProc(int port)
{
	portENTER_CRITICAL();
	GUIProto.nPort = port;
	GUIProto.nTmr = 0;
	GUIProto.lastPacketlen = 0;
	GUIProto.last_CRC16 = 0;
	portEXIT_CRITICAL();

	CommReInit(port);
}

/*
********************************************************************************
* Description : RTC_ValSet
* Arguments   : char *data
* Return      : 
* Note        : RTC set
******************************************************************************** 
*/
void RTC_ValSet(INT8U *data)
{
  Time_t tmp_local;
	sysinfo_t *tmp_s = &_sysinfo 	;
	INT32U tmp_data;

	memcpy(&tmp_data, data, sizeof(tmp_data));
  
  tmp_local = gmtime_conv(tmp_data);
  tmp_s->tm_ptr.tm_sec  = tmp_local.tm_sec;
  tmp_s->tm_ptr.tm_min  = tmp_local.tm_min;
  tmp_s->tm_ptr.tm_hour = tmp_local.tm_hour;
  tmp_s->tm_ptr.tm_mday = tmp_local.tm_mday;
  tmp_s->tm_ptr.tm_mon  = tmp_local.tm_mon;
  tmp_s->tm_ptr.tm_year = tmp_local.tm_year;
  SetRTCTime(&tmp_s->tm_ptr);
}

/*
********************************************************************************
* Description : RTC_ValGet
* Arguments   : INT8U *pbuf
* Return      : 
* Note        : RTC get
******************************************************************************** 
*/
int RTC_ValGet(INT8U *pbuf)
{
	int	nCnt=0;  	
  sysinfo_t *s = &_sysinfo 	;  
  
  pbuf[0] = s->tm_ptr.tm_year;
  pbuf[1] = s->tm_ptr.tm_mon;
  pbuf[2] = s->tm_ptr.tm_mday;
  pbuf[3] = s->tm_ptr.tm_hour;
  pbuf[4] = s->tm_ptr.tm_min;
  pbuf[5] = s->tm_ptr.tm_sec;
  pbuf[6] = s->tm_ptr.tm_wday;    
  nCnt = 7;
  
  return 	nCnt;
}

/*
********************************************************************************
* Description : ADC_OnOff
* Arguments   : INT8U *pbuf, INT8U *data, int length
* Return      : 
* Note        : RTC get
******************************************************************************** 
*/
int ADC_OnOff(INT8U *data)
{
  if(!data[0]) Flags.GUIADCFlag = 0;
  else Flags.GUIADCFlag = 1;
  
  return 	1;
}

/*
********************************************************************************
* Description : Data_ValGet
* Arguments   : INT8U *pbuf, INT8U *data, int length
* Return      : 
* Note        : RTC get
******************************************************************************** 
*/
int Data_ValGet(INT8U *pbuf, INT8U *data, int len)
{
	int	nCnt;  	
  
  for(nCnt=0; nCnt<len; nCnt++) pbuf[0] = data[0];
  
  return 	nCnt;
}

/*
********************************************************************************
* Description : cal_checksum
* Arguments   : data, length
* Return      : 
* Note        : check sum
******************************************************************************** 
*/
INT8U cal_checksum(INT8U *pdata, int len)
{
	INT8U retSum=0;
	int nCount=0;

	for(nCount = 0; nCount < len; nCount++){
		retSum += pdata[nCount];
	}

	return retSum;
}

/*
********************************************************************************
* Description : DecodePacket
* Arguments   : *pbuff, *pTarget
* Return      : 
* Note        : decode packet
******************************************************************************** 
*/
int DecodePacket(char *pbuff, char *pTarget)
{
	INT8U checksum=0;

	int nLength=0;
	
	portENTER_CRITICAL();

	nLength = pbuff[1];
	nLength |= pbuff[2]<< 8;
	memcpy(pTarget, pbuff, nLength+5);

	checksum = cal_checksum((INT8U *)&pTarget[1],nLength +2);

	portEXIT_CRITICAL();

	if((char)checksum != (char)pTarget[nLength+3]){
		printk("Check sum error (%s) CMD:%02X me:%02X,you:%02X \n",__func__, \
      pTarget[4], (INT8U)checksum,(INT8U)pTarget[nLength-5]);
		return CHKSUM_ERR;
	}
	return 0;
}


/*
********************************************************************************
* Description : MakePacket
* Arguments   : cmd, type,	systype, *ptarget, *psource, dataLen
* Return      : 
* Note        : make packet
******************************************************************************** 
*/
int MakePacket(char cmd, char type,	char systype, char *ptarget, char *psource, int dataLen)
{
	char checksum=0,tmp=0;
	int nCount=0,i,packlen=dataLen + 3;

	ptarget[nCount++] = START_PACKET;							// 0x7E
	
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

	for(i=0; i< dataLen; i++)
	{
		ptarget[nCount++] = psource[i];
		checksum +=psource[i];
	}
	ptarget[nCount++] = checksum;
	ptarget[nCount++] = END_PACKET;								// 0x7F

	return nCount;
}

/*
********************************************************************************
* Description : commRxProcGui
* Arguments   : *pdev
* Return      : 
* Note        : External uart communication process
******************************************************************************** 
*/
static commProcess_t *commRxProcGui(commProcess_t *pdev)
{
	static INT8U	data;
	INT8U err,ret=0;
	int SendCount;
	static INT16U packlen = 0;
	char tempbuff[4]={0};
  
	data = CommGetChar(pdev->nPort, 10, &err);
	if(err == COMM_NO_ERR)
	{
		if(data == 0x7E && pdev->bStart_flag==0){						// DecodePacket 전이기 때문에 무조건 7E는 start flag 이다.

			pdev->bStart_flag = 1;
			pdev->bEnd_flag   = 0;
			pdev->nPacketCnt  = 0;
			vTaskDelay(1);

		}
		else if(pdev->nPacketCnt ==1)
			packlen = data;
		else if(pdev->nPacketCnt ==2 )
			packlen |= data<< 8;
		else if( pdev->nPacketCnt >= (packlen +4) && (data == 0x7F) )
		{
			pdev->bEnd_flag = 1;								              // 끝 플레그를 1로 만든다.
			vTaskDelay(1);
		}
		if ( pdev->nPacketCnt >= GUI_BUFFSZ )
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
		vTaskDelay(10);
		ret = DecodePacket(pdev->pRxBuf, pdev->pRxBuf_dec);
		if(ret)
		{
			pdev->bStart_flag = 0;
			pdev->bEnd_flag = 0;
			pdev->nPacketCnt = 0;
			if(ret == CHKSUM_ERR)
			{
				tempbuff[0] = 0x00;
				tempbuff[1] = 0x01; 								        // check sum error
				tempbuff[2] = pdev->pRxBuf_dec[1];					// receive command
			}
			else if(ret == SYS_TYPE_ERR)
			{
				tempbuff[0] = 0x00;
				tempbuff[1] = 0x04; 								        // check sum error
				tempbuff[2] = pdev->pRxBuf_dec[1];					// receive command
			}
			SendCount = MakePacket(0xF0, 0x11, 0x04, (char *)guiTxbuff, (char *)tempbuff, 3);
			CommPutCharStr(pdev->nPort, (INT8U *)guiTxbuff, SendCount);
			return NULL;
		}
		else
		{
			pdev->bStart_flag = 0;
			pdev->bEnd_flag = 0;
			return pdev;
		}
	}
	return 0;	

}

/*
********************************************************************************
* Description : GUI_task
* Arguments   : int port, INT8U wui, char *wui_data, int wui_len, INT8U *seq
* Return      : 
* Note        : GUI task process
******************************************************************************** 
*/
int GUI_parse_proc(void)
{
	commProcess_t *pDev=NULL;
	protocol_t *ptPacket=NULL;
	
	INT8U nMsgType=0;
	static INT16U SendCount=0;

/*-----------------------------------------------------------------------------------------------------*/	
  if(Flags.GUIDataFlag) {//2초 데이터
    memcpy((INT8U*)&guiRxbuff, (INT8U*)&ReadValue, sizeof(stREAD_VALUE));
    SendCount = sizeof(stREAD_VALUE);
    SendCount = MakePacket(0x41, 0x22, 0X01, (char *)guiTxbuff, (char *)guiRxbuff, SendCount);
		CommPutCharStr(GPS_PORT,(INT8U *)guiTxbuff,SendCount);
    if(Flags.GUIADCFlag) {//압력 ADC
      SendCount = MakePacket(0x47, 0x22, 0X01, (char *)guiTxbuff, (char *)&adcdata[ADC_AVR_P0], 2);
      CommPutCharStr(GPS_PORT,(INT8U *)guiTxbuff,SendCount);
    }
    Flags.GUIDataFlag = 0;
  }
    
	pDev = commRxProcGui(&GUIProto);
	if(!pDev) {
		return 0;
	}
	else{
		CommReInit(pDev->nPort);
		// 시작 + 커멘드 + 타입 + system + 몸통 + 체크섬 + 끝 ==> 몸통을 제외한 6바이트만큼 빼면 실제받은 데이터의 사이즈
		//vTaskDelay(1);
		pDev->nPacketCnt -= 8;
		// 여기까지 오면 정상. 패킷커멘드 분석시작.
		ptPacket = (protocol_t *)&pDev->pRxBuf_dec[3];
	}

/*-----------------------------------------------------------------------------------------------------*/	

	if(ptPacket->type == (TYPE_HREPORT | TYPE_LREQUEST))
		nMsgType = (TYPE_HREPORT | TYPE_LRESPONSE);
	else
		nMsgType = (TYPE_HSET | TYPE_LRESPONSE);
	

	switch(ptPacket->cmd)
	{
	case 0x35:        // RTC Set
		if(ptPacket->type == (TYPE_HSET | TYPE_LREQUEST)){
      RTC_ValSet((INT8U *)&(ptPacket->msg));
		}               // RTC Get
    memcpy(guiRxbuff, &RTC_LVal, sizeof(RTC_LVal)); 
		SendCount = MakePacket(ptPacket->cmd+1, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(RTC_LVal));      
		break;
	case 0x36:        // RTC Get
    memcpy(guiRxbuff, &RTC_LVal, sizeof(RTC_LVal)); 
		SendCount = MakePacket(ptPacket->cmd, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(RTC_LVal));	    
		break;
	case 0x40:        // Factory Reset
    DefaultSet();
		SendCount = 0;
		SendCount = MakePacket(ptPacket->cmd, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, SendCount);	     
		break;
	case 0x42:        // ADC ON/OFF
		if(ptPacket->type == (TYPE_HSET | TYPE_LREQUEST)){
      SendCount = ADC_OnOff((INT8U *)&(ptPacket->msg));
      guiRxbuff[0] = (char)Flags.GUIADCFlag;
		}
		SendCount = MakePacket(ptPacket->cmd, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, SendCount);	
		break;
//	case 0x43:        // P0 Cal Get
//    memcpy(guiRxbuff, &Fram.ScrCalib, sizeof(stScrCalib)); 
//		SendCount = MakePacket(ptPacket->cmd, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(stScrCalib));	    
//		break;
//	case 0x44:        // P0 Cal Set
//		if(ptPacket->type == (TYPE_HSET | TYPE_LREQUEST)){
//      memcpy(&Fram.ScrCalib, &(ptPacket->msg), sizeof(stScrCalib));
//      FRAM_Addr_ReadWrite(SCRCALIB_SAVE, SAVE);
//		}
//    memcpy(guiRxbuff, &Fram.ScrCalib, sizeof(stScrCalib)); 
//		SendCount = MakePacket(ptPacket->cmd-1, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(stScrCalib));	
//		break;
	case 0x45:        // IP&PORT Get
    memcpy(guiRxbuff, &ModemInfo.SrvIP, sizeof(ModemInfo.SrvIP) + sizeof(ModemInfo.SrvPort)); 
		SendCount = MakePacket(ptPacket->cmd, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(ModemInfo.SrvIP) + sizeof(ModemInfo.SrvPort));	      
		break;
	case 0x46:        // IP&PORT Set
		if(ptPacket->type == (TYPE_HSET | TYPE_LREQUEST)){
      memcpy(&ModemInfo.SrvIP, &(ptPacket->msg), sizeof(ModemInfo.SrvIP) + sizeof(ModemInfo.SrvPort));
      FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, MODEM_INFO + (u8 *)&ModemInfo.SrvIP - (u8 *)&ModemInfo, sizeof(ModemInfo.SrvIP)+sizeof(ModemInfo.SrvPort));
		}
    memcpy(guiRxbuff, &ModemInfo.SrvIP, sizeof(ModemInfo.SrvIP) + sizeof(ModemInfo.SrvPort));
		SendCount = MakePacket(ptPacket->cmd-1, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(ModemInfo.SrvIP) + sizeof(ModemInfo.SrvPort));
		break;
	case 0x47:        // P0 Data Send
    break;
	case 0x48:        // Modem Data Save Time Get
    memcpy(guiRxbuff, &ModemInfo.DataServerInterval, sizeof(ModemInfo.DataServerInterval)); 
		SendCount = MakePacket(ptPacket->cmd, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(ModemInfo.DataServerInterval));	      
		break;
	case 0x49:        // Modem Data Save Time Set
		if(ptPacket->type == (TYPE_HSET | TYPE_LREQUEST)){
      memcpy(&ModemInfo.DataServerInterval, &(ptPacket->msg), sizeof(ModemInfo.DataServerInterval));
      FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.DataServerInterval)); 
		}
    memcpy(guiRxbuff, &ModemInfo.DataServerInterval, sizeof(ModemInfo.DataServerInterval)); 
		SendCount = MakePacket(ptPacket->cmd-1, nMsgType, ptPacket->systype, (char *)guiTxbuff, (char *)guiRxbuff, sizeof(ModemInfo.DataServerInterval));	      
		break;
	}
/*-----------------------------------------------------------------------------------------------------*/	
	if(SendCount){
		CommPutCharStr(GPS_PORT,(INT8U *)guiTxbuff,SendCount);
	}
/*-----------------------------------------------------------------------------------------------------*/	
	return SendCount;
}
/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/

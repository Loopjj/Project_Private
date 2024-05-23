/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : modem.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : modem process 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define MODEM_GLOBALS
#include "includes.h"
#include "modem_Loop.h"
#include "modem_Hanuri.h"
#include "modem_GVN.h"
#include "modem_DMS.h"
#include "modem_VDIS.h"
#include "modem_ADAS.h"

#define MODEM_TYPE 1 // 0: M66,  1 : UC20/EC20


typedef enum {
  KT_OLLEH,
  SKTELECOM,
  LGUPLUS
}teTELECOM;


u32 SMSDecodeWaitCnt;

u8 ModemPowerOnFlag = 0;
u8 ModemInitCnt = 0;
u8 Modem_FotaFlag;


u8 TellRespFlag;
int TellRespNo;

u8 NetRespFlag;
int NetRespNo;

u8 NotiRespFlag;
int NotiRespNo;

u8 UsimState;
u8 PppState;
u16 KtfState;
u8 TldState;
u8 SmsState;

typedef struct {
	u16	 RingBufRxCtr;					/* Rx 링 버퍼에 저장된 데이터 수					*/
	u8	  *RingBufRxInPtr;				/* 다음 데이터를 저장할 곳을 가리키는 포인터		*/
	u8	  *RingBufRxOutPtr;				/* 다음에 읽어갈 데이터를 가리키는 포인터			*/
	u8	   RingBufRx[MODEM_COMM_RX_BUF_SIZE]; 	/* 수신용 링 버퍼 저장영역(Rx)						*/
} COMM_RX_RING_BUF;

static COMM_RX_RING_BUF ModemCommBuf;

u8 ModemServiceFaultCnt;
u8 ModemServiceFault;


//#define HEXTOBIN(ch) ((ch <= '9') ? ch - '0' : ch - ('A' - 10))



/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void ModemCommBuffCheck(void)
{
  if ((ModemCommBuf.RingBufRxInPtr != ModemCommBuf.RingBufRxOutPtr)) 
    RxBuffEmpty  = 0;
  else
    RxBuffEmpty  = 1;
}

void ModemCommInit(void)
{
	ModemCommBuf.RingBufRxCtr    = 0;
	ModemCommBuf.RingBufRxInPtr  = &ModemCommBuf.RingBufRx[0];
	ModemCommBuf.RingBufRxOutPtr = &ModemCommBuf.RingBufRx[0];
}

void ModemCommPutRxChar(u8 c)
{
	if (ModemCommBuf.RingBufRxCtr < MODEM_COMM_RX_BUF_SIZE) {	
		ModemCommBuf.RingBufRxCtr++;							 
		*ModemCommBuf.RingBufRxInPtr++ = c;						
		if (ModemCommBuf.RingBufRxInPtr >= &ModemCommBuf.RingBufRx[MODEM_COMM_RX_BUF_SIZE]) { 
			ModemCommBuf.RingBufRxInPtr = &ModemCommBuf.RingBufRx[0];
		}
	}
}

u8 ModemCommGetChar(u8 *err)
{
	u8 c;

	if ((ModemCommBuf.RingBufRxInPtr != ModemCommBuf.RingBufRxOutPtr))
	{
//    portENTER_CRITICAL();
    if(ModemCommBuf.RingBufRxCtr > 0)
      ModemCommBuf.RingBufRxCtr--;	

		c = *ModemCommBuf.RingBufRxOutPtr++;					  
		if (ModemCommBuf.RingBufRxOutPtr >= &ModemCommBuf.RingBufRx[MODEM_COMM_RX_BUF_SIZE]) {	
			ModemCommBuf.RingBufRxOutPtr = &ModemCommBuf.RingBufRx[0];
		}
		*err = MODEM_COMM_NO_ERR;
//    portEXIT_CRITICAL();
                
#ifdef EXT_MODEM_RX_LOG_ENABLE
    if(!FotaRespFlag && CMPortEnable)
      UART_SendData(DEBUG_UART_PORT,c);
#endif    
		return (c);
	}
	else
	{
		*err = MODEM_COMM_RX_EMPTY;
		return (0);
	}
}


/*
********************************************************************************
* Description : IsSamePassWord
* Arguments   : str1, str2, mlen, len, start
* Return      : 
* Note        : PassWord compare
******************************************************************************** 
*/
unsigned char IsSamePassWord(char* str1, char* str2, unsigned char mlen, unsigned char len, unsigned char* start)
{
  unsigned char idx = 0;
  
  for(int i = 0; i < mlen; i++) {
    if(str1[i] != ' ') {
      if(str1[i] == str2[idx]) {
        idx++;
        if(idx == len) {
          if(i+1 < mlen) {
            if(str1[i+1] != ' ')
              return 0;
          }
          *start = i+1;
          return 1;
        }
      }
      else break;
    }
  }
  return 0;
}

void TxStrToModem(char* str)
{
  u8 len;
  
  len = StringLen(str,100);
  UART_SendDataStr(&huart3,(u8*)str,len);
#ifdef EXT_MODEM_TX_LOG_ENABLE  
  if(CMPortEnable)
    UART_SendDataStr(DEBUG_UART_PORT,(u8*)str,len);
#endif    
}

#ifdef __USE_TXBUF_DELAY__
#define DELAY_BYTES   512
#endif
void TxDataStrToModem(u8* data, int len)
{
#ifdef __USE_TXBUF_DELAY__  
  int i;
  int dev, mod;
#endif  
  
  if(Setting.ExtModemModel == 2 && TcpState == 1) {  
    TxHexStrToModem(data, len);
  }
  else {
#ifdef __USE_TXBUF_DELAY__    
    dev = len / DELAY_BYTES;
    mod = len % DELAY_BYTES;
    for(i = 0; i < dev; i++) {
      UART_SendDataStr(&huart3, (uint8_t*)&data[i * DELAY_BYTES], DELAY_BYTES);
#ifdef EXT_MODEM_TX_LOG_ENABLE  
      if(CMPortEnable)
        UART_SendDataStr(DEBUG_UART_PORT,(uint8_t*)&data[i * DELAY_BYTES], DELAY_BYTES);
#endif         
      vTaskDelay(100);
    }
    if(mod) {
      UART_SendDataStr(&huart3, (uint8_t*)&data[dev * DELAY_BYTES], mod);
#ifdef EXT_MODEM_TX_LOG_ENABLE  
    if(CMPortEnable)
      UART_SendDataStr(DEBUG_UART_PORT,(uint8_t*)&data[dev * DELAY_BYTES], mod);
#endif       
    }
#else    
    UART_SendDataStr(&huart3,data,len);
#endif    
  }
  
#ifndef __USE_TXBUF_DELAY__  
#ifdef EXT_MODEM_TX_LOG_ENABLE  
  if(CMPortEnable)
    UART_SendDataStr(DEBUG_UART_PORT,data,len);
#endif    
#endif  
}

void TxCharToModem(char ch)
{
  UART_SendData(&huart3,ch);
#ifdef EXT_MODEM_TX_LOG_ENABLE  
  if(CMPortEnable)
    UART_SendData(DEBUG_UART_PORT,ch);
#endif    
}

#define MAX_SOCKET_TX_LEN 512
u8 TxResp, TxRespStep;
int TxMsgLen;
unsigned char TxHexStrToModem(u8* data, int len)
{
  int cnt, n, mod, i, j;
  u8 sdata1,sdata2;
  u32  current, previous;
  int idx = 0, msg_len = 0;
  u8 rdata, err, len_flag = 0, resp_flag = 0;
  u8 resp = 1;
  unsigned int timeout = 5000;
  
  n = len/MAX_SOCKET_TX_LEN;
  mod = len%MAX_SOCKET_TX_LEN;
  
  for(i = 0; i <= n; i++) {
    if(!(i == n && mod == 0)) {
      if(i == n) cnt = mod;
      else cnt = MAX_SOCKET_TX_LEN;
      ModemCommInit();
      printk("\r\n");
      TxStrToModem("AT@SOCKSND=0,1,\"");
      for(j = 0; j < cnt; j++) {
        sdata1 = (data[i*MAX_SOCKET_TX_LEN+j]>>4)&0x0F;
        sdata2 = (data[i*MAX_SOCKET_TX_LEN+j]&0x0F);
        if(sdata1 < 0x0A)
          TxCharToModem(sdata1+'0');
        else
          TxCharToModem(sdata1 - 0x0A +'A');
        if(sdata2 < 0x0A)
          TxCharToModem(sdata2+'0');
        else
          TxCharToModem(sdata2 - 0x0A +'A');
      }
      TxStrToModem("\"\r");
      
      msg_len = 0;
      len_flag = 0;
      resp_flag = 0;
      resp = 1;
      idx = 0;
      init_millis();
      previous = millis();
      memset(AT_Response, 0, 256);
      do{
        rdata = ModemCommGetChar(&err);
        if ( err == MODEM_COMM_NO_ERR ) {
          if(len_flag == 0 && resp_flag == 0) {
            if(idx < 255) 
              AT_Response[idx++] = rdata;
            else {
              resp = 1;        
              break;
            }
            if (strstr(AT_Response, "@SOCKSND:") != NULL)
              len_flag = 1;
          }
          else if(len_flag == 1 && resp_flag == 0) {
            if(rdata >= '0' && rdata <= '9') 
              msg_len = msg_len*10+(rdata-'0');
            else if(rdata == ',') 
              resp_flag = 1;
            else if(rdata != ' ') 
                break;
          }
          else if(resp_flag == 1) {
            if(rdata == '0' ||  rdata == '1')  {
              resp = rdata-'0';
              printk("\r\n");
              break;
            }
            else if(rdata != ' ') break;
          }
          else {
            break;//idle
          }
        }
        current = millis();
        vTaskDelay(1);
      }
      while(((current-previous) < timeout));
    }
    if(!(resp == 0 && msg_len == cnt)) {
      TxResp = resp;
      TxMsgLen = idx;
      TxRespStep = len_flag*10+resp_flag;
      printk("\r\nMODEM : Send TCP DATA ERROR\r\n"); 
      return 0;
    }
    if(i != n) vTaskDelay(100);
  }
   return 1;
}

/*
********************************************************************************
* Description : sendATcommand
* Arguments   : ATcommand, answer1, answer2, timeout
* Return      : 
* Note        : AT command
******************************************************************************** 
*/

u8 sendATcommand(char* ATcommand, char* expected_answer1,
        char* expected_answer2, unsigned int timeout){

	char data = 0;          
  u8 x=0,  answer=0, err;
  u32  current, previous;
  
  memset(AT_Response, 0, 256);
  
  ModemCommInit();
  // Send the AT command    
  TxStrToModem(ATcommand);

  x = 0;
  init_millis();
  previous = millis();

  // loop waits for the answer
  do{
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      AT_Response[x] = data;
      if(x < 255)  x++;
      else {
        printk("\r\nMODEM : sendATcommand 256 bytes over ERROR\r\n");              
        return 0;        
      }
      // check answer 1
      if (strstr(AT_Response, expected_answer1) != NULL)
      {
          answer = 1;
      }
      // check answer 2
      else if (strstr(AT_Response, expected_answer2) != NULL)
      {
          answer = 2;
      }
    }
    vTaskDelay(1);
    current = millis();
  }
  while((answer == 0) && ((current-previous) < timeout));

  return answer;
}

/*
********************************************************************************
* Description : sendATcommand
* Arguments   : ATcommand, answer1, answer2, timeout
* Return      : 
* Note        : AT command
******************************************************************************** 
*/
int8_t sendATcommand2(char* ATcommand, char expected_answer1, char* expected_answer2, unsigned int timeout){

	char data = 0;          
  u8 x=0,  answer=0, err;
  u32  current, previous;
  
  memset(AT_Response, 0, 256);
  
  ModemCommInit();

  // Send the AT command    
  TxStrToModem(ATcommand);    

  x = 0;
  init_millis();
  previous = millis();

  // loop waits for the answer
  do{
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      AT_Response[x] = data;
      if(x < 255)  x++;
      else {
        printk("\r\nMODEM : sendATcommand 256 bytes over ERROR\r\n");              
        return 0;        
      }
      // check answer 1
      if (data == expected_answer1)
      {
          answer = 1;
      }
      // check answer 2
      else if (strstr(AT_Response, expected_answer2) != NULL)
      {
          answer = 2;
      }
    }
    vTaskDelay(1);
    current = millis();
  }
  while((answer == 0) && ((current-previous) < timeout));

  return answer;
}
/*
********************************************************************************
* Description : AT_SendWithRecievedData
* Arguments   : cmd, cmdResp, valNum, timeout
* Return      : resp
* Note        : AT command + Recieve data
******************************************************************************** 
*/
int AT_SendWithRecieveData(char* cmd, char* cmdResp, u8 valNum, unsigned int timeout) 
{
  
  u32  current, previous;
  int i, idx = 0, val_idx = 0, msg_len = 0;
  u8 str_flag = 0, data, err, data_flag = 0;
  u8 resp = 0;

  memset(AT_Response, 0, 256);
  for(i = 0; i < 10; i++) {
    memset(ATCmdRxMsg[i], 0, 32);
  }

  ModemCommInit();
  
  TxStrToModem(cmd);    
  init_millis();
  previous = millis();
  do{
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      if(data_flag == 0) {
        if(idx < 255) {
          AT_Response[idx++] = data;
        }
        else {
          printk("\r\nMODEM : sendATcommand 256 bytes over ERROR\r\n");              
          resp = 0;        
          data_flag = 2;
        }
        if (strstr(AT_Response, cmdResp) != NULL)
          data_flag = 1;
      }
      else if(data_flag == 1) {
        if(data == '\r') {
            resp = 1;
            break;
        }
        else if(data == ',') {
          if(val_idx < valNum && val_idx < 9) {
            val_idx++;
            msg_len = 0;
          }
          else {
            resp = 1;
            data_flag = 2;
          }
          str_flag = 0;
        }
        else if(data == '\"') {
          if(str_flag) str_flag = 0;
          else str_flag = 1;
        }
        else {
          if(msg_len < 32) {
            if(!(msg_len == 0 && data == ' '))
              ATCmdRxMsg[val_idx][msg_len++] = data;
          }
        }
      }
      else {
        break;//idle
      }
    }
    current = millis();
    vTaskDelay(1);
  }
  while(((current-previous) < timeout));

  return resp;
}

/*
********************************************************************************
* Description : AT_SendWithRecievedData
* Arguments   : cmd, cmdResp, valNum, timeout
* Return      : resp
* Note        : AT command + Recieve data
******************************************************************************** 
*/
int AT_SendWithRecieveData2(char* cmd, char* cmdResp, u8 valNum, unsigned int timeout) 
{
  
  u32  current, previous;
  int i, idx = 0, val_idx = 0, msg_len = 0;
  u8 str_flag = 0, data, err, data_flag = 0;
  u8 resp = 0;

  memset(AT_Response, 0, 256);
  for(i = 0; i < 10; i++) {
    memset(ATCmdRxMsg[i], 0, 32);
  }

  ModemCommInit();
  
  TxStrToModem(cmd);    
  init_millis();
  previous = millis();
  data_flag = 1;
  do{
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      if(data_flag == 0) {
        if(idx < 255) {
          AT_Response[idx++] = data;
        }
        else {
          printk("\r\nMODEM : sendATcommand 256 bytes over ERROR\r\n");              
          resp = 0;        
          data_flag = 2;
        }
        if (strstr(AT_Response, cmdResp) != NULL) {
          resp = 1;        
          break;
        }
      }
      else if(data_flag == 1) {
        if(data == '\r' && (msg_len > 0 || val_idx > 0)) 
            data_flag = 0;
        else if(data == ',') {
          if(val_idx < valNum && val_idx < 9) {
            val_idx++;
            msg_len = 0;
          }
          else {
            resp = 1;
            data_flag = 2;
          }
          str_flag = 0;
        }
        else if(data == '\"') {
          if(str_flag) str_flag = 0;
          else str_flag = 1;
        }
        else {
          if(msg_len < 32) {
            if(!(msg_len == 0 && data == ' ') && data != '\r' && data != '\n')
              ATCmdRxMsg[val_idx][msg_len++] = data;
          }
        }
      }
      else {
        break;//idle
      }
    }
    current = millis();
    vTaskDelay(1);
  }
  while(((current-previous) < timeout));

  return resp;
}
/*
********************************************************************************
* Description : AT_SendWithRecievedData
* Arguments   : cmd, cmdResp, valNum, timeout
* Return      : resp
* Note        : AT command + Recieve data
******************************************************************************** 
*/
int AT_SendWithRecieveMultiRowData(char* cmd, char* cmdResp, u8 valNum, unsigned int timeout) 
{
  
  u32  current, previous;
  int i, idx = 0, val_idx = 0, msg_len = 0;
  u8  data, err, data_flag = 0;
  u8 resp = 0;

  memset(AT_Response, 0, 256);
  for(i = 0; i < 10; i++) {
    memset(ATCmdRxMsg[i], 0, 32);
  }

  ModemCommInit();
  
  TxStrToModem(cmd);    
  init_millis();
  previous = millis();
  do{
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      if(data_flag == 0) {
        if(idx < 255) {
          AT_Response[idx++] = data;
        }
        else {
          printk("\r\nMODEM : sendATcommand 256 bytes over ERROR\r\n");              
          resp = 0;        
          data_flag = 2;
        }
        if (strstr(AT_Response, cmdResp) != NULL)
          data_flag = 1;
      }
      else if(data_flag == 1) {
        if(data == '\r') {
          if(val_idx < valNum && val_idx < 9) {
            val_idx++;
            msg_len = 0;
          }
          else {
            resp = 1;
            data_flag = 2;
          }
        }
        else {
          if(data == '\n') msg_len = 0;
          else if(msg_len < 32)
            ATCmdRxMsg[val_idx][msg_len++] = data;
        }
      }
      else {
        break;//idle
      }
    }
    current = millis();
    vTaskDelay(1);
  }
  while(((current-previous) < timeout));

  return resp;
}

/*
********************************************************************************
* Description : Modem_PowerOff
* Arguments   : 
* Return      : 
* Note        : Modem Power Off
******************************************************************************** 
*/
void Modem_PowerOff(void)
{
  printk("\r\nMODEM : power off\r\n");  
  if(Setting.ExtModemModel == 0)   {
    sendATcommand("AT$$MODE=2\r", "OK", "ERROR", 1000);
    vTaskDelay(5000);
  }
  else if(Setting.ExtModemModel == 1)   {
    sendATcommand("AT$$HWRESET\r", "OK", "ERROR", 1000);
    vTaskDelay(3000);
  }
  else if(Setting.ExtModemModel == 2)   {
    sendATcommand("AT+CFUN=7\r\n", "OK", "ERROR", 1000);
    vTaskDelay(3000);
  }
  CDMA_POWER_OFF();
}

/*
********************************************************************************
* Description : Modem_PowerOn
* Arguments   : 
* Return      : 
* Note        : Modem Power ON
******************************************************************************** 
*/
unsigned char Modem_PowerOnCheck(void)
{
  u8 resp=0, sendcnt=0;

  printk("\r\nMODEM : Check Modem on\r\n");

  while(resp == 0 && sendcnt < 5) { 
    resp = sendATcommand("AT\r\n", "OK", "ERROR", 1000);
    sendcnt++;
    vTaskDelay(1);
  }

  return resp;
}
/*
********************************************************************************
* Description : Modem_PowerOn
* Arguments   : 
* Return      : 
* Note        : Modem Power ON
******************************************************************************** 
*/
void Modem_PowerOn(void)
{
  printk("\r\nMODEM : power on\r\n");
  
  CDMA_POWER_OFF();
  vTaskDelay(1000);
  CDMA_POWER_ON();

//  SecData.Status.ModemReset = 1;
}

/*
********************************************************************************
* Description : AT_SendWithRecieveCNUM
* Arguments   : timeout
* Return      : resp
* Note        : AT+CNUM  -> CNUM Data
******************************************************************************** 
*/
int AT_SendWithRecieveCNUM(unsigned int timeout) 
{
  int d = 1, i, cnt = 0;
  u8 resp = 0;
  u32 cnum=0;
  u16 addr;
  
  resp = AT_SendWithRecieveData("AT+CNUM\r\n", "+CNUM: ", 5, timeout);
  if(resp == 1) {
    memcpy(ModemInfo.MSISDN, ATCmdRxMsg[1], 16);
    if(ModemInfo.MSISDN[0] == '+') {
      ModemCTN[0] = '0';
      memcpy((u8*)&ModemCTN[1],(u8*)&ModemInfo.MSISDN[3],10);
    }
    else
      memcpy((u8*)ModemCTN,(u8*)ModemInfo.MSISDN,11);
    
    for(i = 29; i >= 0 ; i--) {
      if(ATCmdRxMsg[1][i] >= 0x30 && ATCmdRxMsg[1][i] <= 0x39) {
        cnum += (ATCmdRxMsg[1][i]-0x30)*d;
        d *= 10;
        cnt++;
        if(cnt >= 10) break;
      }
    }
    if(cnum > 1000000000 && cnt == 10) {
      if(cnum != ModemInfo.CNUM) {
        ModemInfo.CNUM = cnum;
        addr = MODEM_INFO + (u8 *)&ModemInfo.CNUM - (u8 *)&ModemInfo;
        FRAMMultiWrite((u8 *)&ModemInfo.CNUM, addr, sizeof(ModemInfo.CNUM));
      }
    }
  }
  
  return resp;
}

/*
********************************************************************************
* Description : Modem_SimCard
* Arguments   : void
* Return      : answer
* Note        : Modem Sim Card Reeady check
******************************************************************************** 
*/
unsigned char Modem_SimCard(void)
{
  printk("\r\nMODEM : simcard \r\n");
  
  u8 resp=0, sendcnt=0;

  while(resp == 0 && sendcnt < 5) { 
    resp = sendATcommand("AT+CPIN?\r\n", "+CPIN: READY", "+CPIN: NOT INSERTED", 5000); //Maximum Response Time 5s
    sendcnt++;
    vTaskDelay(1);
  }

  return resp;
}


/*
********************************************************************************
* Description : Modem_CREG_Check
* Arguments   : none
* Return      : resp
* Note        : CREG Check
******************************************************************************** 
*/
unsigned char Modem_CREG_Check(void)
{
  
  unsigned char resp;
  u8 val1, val2;
  
  resp=0;
  if(Setting.ExtModemModel == 0)  
    resp = AT_SendWithRecieveData("AT+CREG?\r\n","+CREG: ", 2, 5000);
  else
    resp = AT_SendWithRecieveData("AT+CGREG?\r\n","+CGREG: ", 2, 5000);
  val1 = AsciiToVal(ATCmdRxMsg[0]);
  val2 = AsciiToVal(ATCmdRxMsg[1]);
  if(resp) {
    TCP_Open = 0;
    TcpState = 0;
  }
  else {
    if(TcpState == 1 || TCP_Open)  Modem_SocketClose(0);
  }
  if(val2 == 1 || val2 == 5) {
    resp = 1;
    SecData.Status.Comm_Fault = 0;
  }
  else {
    printk("\r\nMODEM : CREG ERROR\r\n");   
    SecData.Status.Comm_Fault = 1;
    resp = 0;
  }
  
  printk("\r\nMODEM : Check CREG status %u / %u,%u\r\n", resp,val1,val2);
  return resp;
}


/*
********************************************************************************
* Description : Modem_CSQ_Check
* Arguments   : none
* Return      : resp
* Note        : Modem CSQ Check
******************************************************************************** 
*/
unsigned char Modem_CSQ_Check(void)
{
  unsigned char resp;
  
  resp=0;
  resp = AT_SendWithRecieveData("AT+CSQ\r\n","+CSQ: ", 2, 1000);
  u8 val1 = AsciiToVal(ATCmdRxMsg[0]);
  u8 val2 = AsciiToVal(ATCmdRxMsg[1]);
  
  if(resp != 1) {
    if(TcpState == 1 || TCP_Open)  Modem_SocketClose(0);
    printk("\r\nMODEM : CSQ ERROR\r\n");   
    SecData.Status.Comm_Fault = 1;
  }
  else {
    SecData.RSSI = val1;
    TCP_Open = 0;
    TcpState = 0;
    SecData.Status.Comm_Fault = 0;
  }
  
  printk("\r\nMODEM : Check CSQ status %u / %u\r\n", resp, val1);
  return resp;
}

/*
********************************************************************************
* Description : Modem_CREG_Check
* Arguments   : none
* Return      : resp
* Note        : CREG Check
******************************************************************************** 
*/
unsigned char Modem_State_Check(void)
{
  
  unsigned char resp;
  u8 i, val[7];
  
  if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1)  {
    resp=0;
    resp = AT_SendWithRecieveData("AT$$MODEM_STATE?\r","$$MODEM_STATE: ", 7, 1000);
    for(i = 0; i < 7; i++) {
      val[i] = AsciiToVal(ATCmdRxMsg[i]);
    }

    if(resp == 1 && val[0] == 0 && val[2] == 0 && val[3] == 0 && val[5] == 0  && val[6] == 0 ) {
      SecData.RSSI = val[1];
      TCP_Open = 0;
      TcpState = 0;
      SecData.Status.Comm_Fault = 0;
      resp = 1;
    }
    else {
      printk("\r\nMODEM : MODEM_STATE ERROR\r\n"); 
      if(val[6] != 0) {
        ModemServiceFaultCnt++;
        if(ModemServiceFaultCnt > 3)
          ModemServiceFault = 1;
      }
      SecData.Status.Comm_Fault = 1;
      resp = 0;
    }
    
    printk("\r\nMODEM : Check modem status %u,%u,%u,%u,%u,%u,%u\r\n", val[0], val[1], val[2], val[3], val[4], val[5], val[6]);
  }
  else {
    resp = Modem_CSQ_Check();
  }
  return resp;
}


/*
********************************************************************************
* Description : Modem_PPPOpen
* Arguments   : sw
* Return      : resp
* Note        : Modem ppp open
******************************************************************************** 
*/
unsigned char Modem_PPPOpen(void)
{
  unsigned char resp;
  
//  resp = sendATcommand("AT$$TCP_PPPOP\r", "OK", "ERROR", 5000);
  if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1)  {
    resp = AT_SendWithRecieveData("AT$$TCP_PPPOP\r","$$TCP_PPPOP: ", 1, 5000);
    if(resp == 1 && ATCmdRxMsg[0][0] == '0') {
      resp = 1;
//      PppState = 1;
    }
    else
      resp = 0;
    vTaskDelay(2000);
    if(resp == 1)
      return 1;
    else
      return 0;
  }
  else if(Setting.ExtModemModel == 2){
    if(PppState != 1) {
      resp = sendATcommand("AT@NETCONN=1\r", "OK", "ERROR", 5000);
      vTaskDelay(2000);
      if(resp == 1) {
        PppState = 3;
        return 1;
      }
      else
        return 0;
    }
    else
      return 1;
  }
   return 1;
}

/*
********************************************************************************
* Description : Modem_PPPClose
* Arguments   : sw
* Return      : resp
* Note        : Modem ppp open
******************************************************************************** 
*/
unsigned char Modem_PPPClose(void)
{
  unsigned char resp;
  
//  resp = sendATcommand("AT$$TCP_PPPCL\r", "OK", "ERROR", 5000);
  if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1)  {
    if(PppState != 1) return 1;
    resp = AT_SendWithRecieveData("AT$$TCP_PPPCL\r","$$TCP_PPPCL: ", 1, 5000);
    if(resp == 1 && ATCmdRxMsg[0][0] == '0')
      resp = 1;
    else
      resp = 0;
  }
  else if(Setting.ExtModemModel == 2){
    if(TcpState == 1) {
      resp = sendATcommand("AT@SOCKCLOSE=0\r", "OK", "ERROR", 5000);
      vTaskDelay(1000);
    }
    else  resp = 1;
//    if(PppState == 1) {
//      resp = sendATcommand("AT@NETCONN=0\r", "OK", "ERROR", 5000);
//      if(resp != 1) resp = 0;
//    }
//    else  resp = 1;
  }
  vTaskDelay(2000);
  if(resp == 1)
    return 1;
  else
    return 0;
}

u16 APNProcessCnt;

unsigned char APNProcess(unsigned char mode)
{
  unsigned char resp,rcnt, len;
  
  if(mode > 0) {
    if(ModemServiceFault)
      ModemStep = MODEM_POWER_ON;
    ModemServiceFaultCnt = 0;
    ModemServiceFault = 0;
  }
  if(mode == 0) {
    resp = AT_SendWithRecieveData("AT+CGDCONT?\r\n","+CGDCONT: ", 6, 1000);
    len = StringLen(ATCmdRxMsg[2],32);
    if(resp == 1 && IsSameString(APN_Name,ATCmdRxMsg[2],len) == 0) {
      memset((u8*)APN_Name, 0, sizeof(APN_Name));
      memcpy((u8*)APN_Name, (u8*)ATCmdRxMsg[2], len);
      FRAMMultiWrite((u8*)APN_Name, APN_NAME_INFO, sizeof(APN_Name));  
    }
    if(MtReadAPNFlag ) MtTxAPNFlag = 1;
  }
  else if(mode == 1) {
    Modem_PPPClose();
    memset(MdmTmpString,0,100);
    if(Setting.ExtModemModel == 1 || Setting.ExtModemModel == 2)  
      sprintf(MdmTmpString, "AT+CGDCONT=1,\"IPV4V6\",\"%s\"\r\n",APN_Name);
    else
      sprintf(MdmTmpString, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n",APN_Name);
    rcnt=0;  resp=0;
    while(resp == 0 && rcnt < AT_RETRY_CNT) { 
      resp = sendATcommand(MdmTmpString, "OK", "ERROR", 300); 
      vTaskDelay(1);
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      printk("\r\nMODEM : CGDCONT ERROR\r\n");
      return resp;
    }
    if(resp == 1) Modem_PPPOpen();
  }
  else if(mode == 2) {
    Modem_PPPClose();
    memset(MdmTmpString,0,100);
    if(Setting.ExtModemModel == 1 || Setting.ExtModemModel == 2)  
      sprintf(MdmTmpString, "AT+CGDCONT=1,\"IPV4V6\",\"%s\"\r\n",ComRxAPN_Name);
    else
      sprintf(MdmTmpString, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n",ComRxAPN_Name);
    rcnt=0;  resp=0;
    while(resp == 0 && rcnt < AT_RETRY_CNT) { 
      resp = sendATcommand(MdmTmpString, "OK", "ERROR", 300); 
      vTaskDelay(1);
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      printk("\r\nMODEM : CGDCONT ERROR\r\n");
      return resp;
    }
    PppState = 0;
    if(resp == 1)
      CLI_APN_ChangeFlag = 1;
    APNProcessCnt = 0;
    Modem_PPPOpen();
  }
  else if(mode == 3) {
    if(CLI_APN_ChangeFlag == 0) return 0;
    
    APNProcessCnt++;
    if(PppState == 1) {
      APNProcessCnt = 0;
      CLI_APN_ChangeFlag = 0;
      memset(APN_Name,0,32);
      memcpy((u8*)APN_Name,(u8*)ComRxAPN_Name, sizeof(ComRxAPN_Name));
      FRAMMultiWrite((u8*)APN_Name, APN_NAME_INFO, sizeof(APN_Name));    
    }
    else if(APNProcessCnt > 10000) {
      memset(MdmTmpString,0,100);
      if(Setting.ExtModemModel == 1 || Setting.ExtModemModel == 2)  
        sprintf(MdmTmpString, "AT+CGDCONT=1,\"IPV4V6\",\"%s\"\r\n",APN_Name);
      else
        sprintf(MdmTmpString, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n",APN_Name);
      rcnt=0;  resp=0;
      while(resp == 0 && rcnt < AT_RETRY_CNT) { 
        resp = sendATcommand(MdmTmpString, "OK", "ERROR", 300); 
        vTaskDelay(1);
        rcnt++;
      }  
      if(rcnt >= AT_RETRY_CNT) {
        printk("\r\nMODEM : CGDCONT ERROR\r\n");
        APNProcessCnt = 0;
        return resp;
      }
      APNProcessCnt = 0;
      CLI_APN_ChangeFlag = 0;
      Modem_PPPOpen();
    }
  }
  
  return resp;
}

unsigned char QoSAdust(unsigned char mode, unsigned short val)
{
  unsigned char resp,rcnt;
  u8 cid, tc;
  u16 up,down;
  
  if(Setting.ExtModemModel == 2)   return 1;  
  
  resp = AT_SendWithRecieveData("AT+CGEQREQ?\r\n","+CGEQREQ: ",4, 1000);
  cid = AsciiToVal(ATCmdRxMsg[0]);
  tc = AsciiToVal(ATCmdRxMsg[1]);
  up = AsciiToVal(ATCmdRxMsg[2]);
  down = AsciiToVal(ATCmdRxMsg[3]);
  
  if(mode == 0) { // Upload
    memset(MdmTmpString,0,100);
//    sprintf(MdmTmpString, "AT+CGEQREQ=1,\"IP\",4,%u,0,%u,0\r",val,val);
    sprintf(MdmTmpString, "AT+CGEQREQ=%u,%u,%u,%u\r\n",cid,tc,val,down);
    rcnt=0;  resp=0;
    while(resp == 0 && rcnt < AT_RETRY_CNT) { 
      resp = sendATcommand(MdmTmpString, "OK", "ERROR", 300); 
      vTaskDelay(1);
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      printk("\r\nMODEM : CGEQREQ Upload ERROR\r\n");
      return resp;
    }
  }
  else if(mode == 1) {  // Dnload
    memset(MdmTmpString,0,100);
//    sprintf(MdmTmpString, "AT+CGEQREQ=1,\"IP\",4,0,%u,0,%u\r",val,val);
    sprintf(MdmTmpString, "AT+CGEQREQ=%u,%u,%u,%u\r\n",cid,tc,up,val);
    rcnt=0;  resp=0;
    while(resp == 0 && rcnt < AT_RETRY_CNT) { 
      resp = sendATcommand(MdmTmpString, "OK", "ERROR", 300); 
      vTaskDelay(1);
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      printk("\r\nMODEM : CGEQREQ Dnload ERROR\r\n");
      return resp;
    }    
  }

  return resp;
}


int ServerIPCheck(void) 
{
  unsigned char rip[4] = {0,}, sip[4], idx = 0; 
  unsigned short port, sport, val = 0;
  unsigned char i, resp;
  
  resp=0;  

  if(Setting.ExtModemModel == 2)   {
    ServerIPCheckFlag = 0;
    return 1;   
  }
  
  resp = AT_SendWithRecieveData("AT$$TCP_ADDR?\r","[0] ", 1, 2000);
  if(resp == 1) {
    for(i = 0; i < 32; i++) {
      if(ATCmdRxMsg[0][i] >= '0' && ATCmdRxMsg[0][i] <= '9' )
        val = val*10 + (ATCmdRxMsg[0][i]-'0');
      else if(ATCmdRxMsg[0][i] == '.' || ATCmdRxMsg[0][i] == ':') {
        if(idx < 4 && val < 256)
          rip[idx++] = val;
          val = 0;
      }
      else if(ATCmdRxMsg[0][i] == 0) {
        if(idx == 4)
          port = val;
        break;
      }
    }
  }
  else
    ServerIPCheckFlag = 0;
  
  if(Setting.ExtModemMode == LOOP_PROTOCOL) {
    if(!LoopFotaFlag) {
      memcpy((u8*)sip, (u8*)&ModemInfo.LpIP, 4);
      sport = ModemInfo.LpPort ;
    }
  }
  else {
    memcpy((u8*)sip, (u8*)&ModemInfo.SrvIP, 4);
    sport = ModemInfo.SrvPort ;
  }
  
  if(port == sport && rip[0] == sip[0] && rip[1] == sip[1] && rip[2] == sip[2] && rip[3] == sip[3]) 
      ServerIPCheckFlag = 1;
  else
    ServerIPCheckFlag = 0;
  
  return resp;
}

unsigned char Modem_CheckTelecom(void)
{
  unsigned char resp = 0;
  u16 addr;

  resp = AT_SendWithRecieveData("AT+COPS?\r\n","+COPS: ", 4, 5000);
  if(resp == 1) {
    if(strstr(ATCmdRxMsg[2], "olleh") != NULL || (ATCmdRxMsg[2][0] == 'K' && ATCmdRxMsg[2][1] == 'T')) {
      if(Setting.ExtModemTelecom != 0) {
        Setting.ExtModemTelecom = 0;
        addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemTelecom); 
        FRAMMultiWrite((u8 *)&Setting.ExtModemTelecom, addr, sizeof(Setting.ExtModemTelecom));   
      }
    }
    else if(strstr(ATCmdRxMsg[2], "SK") != NULL) {
      if(Setting.ExtModemTelecom != 1) {
        Setting.ExtModemTelecom = 1;
        addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemTelecom); 
        FRAMMultiWrite((u8 *)&Setting.ExtModemTelecom, addr, sizeof(Setting.ExtModemTelecom));   
      }
      if(Setting.ExtModemMode > 0) {
        Setting.ExtModemMode = 0;
        addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
        FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));   
      }
    }
    else if(strstr(ATCmdRxMsg[2], "LG") != NULL) {
      if(Setting.ExtModemTelecom != 2) {
        Setting.ExtModemTelecom = 2;
        addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemTelecom); 
        FRAMMultiWrite((u8 *)&Setting.ExtModemTelecom, addr, sizeof(Setting.ExtModemTelecom));   
      }
      if(Setting.ExtModemMode > 0) {
        Setting.ExtModemMode = 0;
        addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
        FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));   
      }
    }
    return 1;
  }
  return 0;
}

/*
********************************************************************************
* Description : Modem_Initialize
* Arguments   : resp
* Return      : 
* Note        : Modem Initialize
******************************************************************************** 
*/
unsigned char Modem_Initialize(void)
{
  unsigned char resp, rcnt, apn_change = 0;
//  unsigned char len;
  u16 addr;
  
  if(TcpState == 1 || TCP_Open)  {
    Modem_SocketClose(0);
    vTaskDelay(1000); 
  }

  TCP_Open = 0;
  TcpState = 0;
  
  printk("\r\nMODEM : Initialize Start\r\n");

  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand("ATE0\r\n", "OK", "ERROR", 1000);
    vTaskDelay(1);
    rcnt++;
  }
  if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : ATE0 ERROR\r\n");
    return resp;
  }
  vTaskDelay(100);  

  printk("\r\n");
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = AT_SendWithRecieveData2("AT+GMM\r\n","OK", 1, 1000);
    vTaskDelay(1);
    rcnt++;
  }
  if(resp == 1 && ATCmdRxMsg[0][0] !=  0) {
    rcnt = StringLen(ATCmdRxMsg[0],20);
    memcpy(ModemVer,ATCmdRxMsg[0], rcnt);
    if(ModemVer[0] == 'T' && (ModemVer[2] == '2' || ModemVer[2] == '3')) {
      Setting.ExtModemModel = 0;
#ifdef __USE_LTE_ONLY_MODE__      
      return 0;
#endif      
    }
    else if(ModemVer[0] == 'T' && ModemVer[2] == 'X' && (ModemVer[3] == '2' || ModemVer[3] == '3')) {
      Setting.ExtModemModel = 0;
#ifdef __USE_LTE_ONLY_MODE__            
      return 0;      
#endif      
    }
    else if(ModemVer[0] == 'T' && (ModemVer[2] == '4' || ModemVer[2] == '5'))
      Setting.ExtModemModel = 1;
    else if(ModemVer[0] == 'A' && ModemVer[0] == 'M')
      Setting.ExtModemModel = 2;      
  }
  else if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : GMM ERROR\r\n");
  }
  if(Setting.ExtModemModel > 2) Setting.ExtModemModel = 0;
  vTaskDelay(100);
  
  printk("\r\n");
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand("AT+IPR=115200\r\n", "OK", "ERROR", 300); //Maximum Response Time 5s
    vTaskDelay(1);
    rcnt++;
  }  
  if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : IPR ERROR\r\n");
//    LED_2(RED);  
    return resp;
  }
  vTaskDelay(100);

  printk("\r\n");
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand("AT+CPIN?\r\n", "+CPIN: READY", "+CPIN: NOT INSERTED", 5000); //Maximum Response Time 5s
    vTaskDelay(1);
    rcnt++;
  }  
  if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : CPIN ERROR\r\n");
//    LED_2(RED);  
    return resp;
  }
  vTaskDelay(100);

  printk("\r\n");
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = AT_SendWithRecieveCNUM(1000); //Maximum Response Time 300ms
    vTaskDelay(1);
    rcnt++;
  }  
  if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : CNUM ERROR\r\n");    
    return resp;
  }
  printk("\r\nMODEM : CNUM %u / %u \r\n",resp, ModemInfo.CNUM);    
  vTaskDelay(100);
  
  printk("\r\n");
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand("AT+CNMI=2,1\r\n", "OK", "ERROR", 1000);
    vTaskDelay(1);
    rcnt++;
  }
  if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : CNMI ERROR\r\n");
    return resp;
  }
  vTaskDelay(100);

  printk("\r\n");
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand("AT+CMGF=1\r\n", "OK", "ERROR", 1000);
    vTaskDelay(1);
    rcnt++;
  }
  if(rcnt >= AT_RETRY_CNT) {
    printk("\r\nMODEM : CMGF ERROR\r\n");
    return resp;
  }
  vTaskDelay(100);

  printk("\r\n");
  Modem_CheckTelecom();  

//  printk("\r\n");
//  if(Setting.ExtModemTelecom == 0) {
//    len = StringLen(APN_Name,32);
//    if(len > 0) {
//      if(Setting.ExtModemModel == 0) {
//        if(IsSameString(APN_Name,"privatelte.ktfwing.com",len)){
//          memset((u8*)APN_Name, 0, sizeof(APN_Name));
//          sprintf(APN_Name, "biz.ktfwing.com");  
//          FRAMMultiWrite((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
//          apn_change = 1;
//        }
//      }
//      else {
//        if(IsSameString(APN_Name,"biz.ktfwing.com",len)) {
//          memset((u8*)APN_Name, 0, sizeof(APN_Name));
//          sprintf(APN_Name, "privatelte.ktfwing.com");   
//          FRAMMultiWrite((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
//          apn_change = 1;
//        }
//      }
//    }
////    if(ModemInfo.FactoryInit == 1 && apn_change == 0) apn_change = 1;
//  }
  
  if(apn_change) {
    resp = APNProcess(1);
    if(resp == 1 && ModemInfo.FactoryInit == 1) {
      ModemInfo.FactoryInit = 0;
      addr = MODEM_INFO + (u8 *)&ModemInfo.FactoryInit - (u8 *)&ModemInfo;
      FRAMMultiWrite((u8 *)&ModemInfo.FactoryInit, addr, sizeof(ModemInfo.FactoryInit));
    }
    vTaskDelay(100);
  }
  else {
    resp = APNProcess(0);
    vTaskDelay(100);
  }

  if(Setting.ExtModemTelecom == 0 && Setting.ExtModemModel == 1) {
    printk("\r\n");
    rcnt=0;  resp=0;
    while(resp == 0 && rcnt < AT_RETRY_CNT) { 
      resp = sendATcommand("AT$$SPC_LOCK=000000\r\n", "OK", "ERROR", 1000);
      vTaskDelay(1);
      rcnt++;
    }
    if(rcnt >= AT_RETRY_CNT) {
      printk("\r\nMODEM : SPC_LOCK ERROR\r\n");
      return resp;
    }
    vTaskDelay(100);
    
    printk("\r\n");
    rcnt=0;  resp=0;
    while(resp == 0 && rcnt < AT_RETRY_CNT) { 
      resp = sendATcommand("AT$$NETWORK_MODE=1\r\n", "OK", "ERROR", 1000);  //0 : 3G, 1 : LTE, 2 : 3G+LTE
      vTaskDelay(1);
      rcnt++;
    }
    if(rcnt >= AT_RETRY_CNT) {
      printk("\r\nMODEM : NETWORK_MODE ERROR\r\n");
      return resp;
    }
    vTaskDelay(100);
  }
//  ServerIPCheck();
  
//  vTaskDelay(100);

//  Modem_SMS_DeleteAll()

  printk("\r\n");
  vTaskDelay(1000);
  Modem_SMS_ReadAll();
  vTaskDelay(1000);
  
  return resp;
}


unsigned char Modem_GetIP(char* url, u8* ip)
{
  unsigned char val = 0, idx = 0; 
  unsigned char i, resp = 0, len = 0;

  memset(MdmTmpString,0,100);
  
  if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1)   {  
    sprintf(MdmTmpString,"AT$$TCP_DNIP=%s\r",url);  
    resp = AT_SendWithRecieveData(MdmTmpString,"$$TCP_DNIP: ", 2, 5000);
    if(resp == 1) {
      if(Setting.ExtModemModel == 1 && ATCmdRxMsg[1][0] == 0) {
        len = StringLen(ATCmdRxMsg[0], 16);
        if(len > 6)  memcpy(ATCmdRxMsg[1], ATCmdRxMsg[0], len);
      }
      if(len > 6 || ATCmdRxMsg[0][0] == '0') {
        for(i = 0; i < 16; i++) {
          if(ATCmdRxMsg[1][i] >= '0' && ATCmdRxMsg[1][i] <= '9' )
            val = val*10 + ATCmdRxMsg[1][i]-'0';
          else if(ATCmdRxMsg[1][i] == '.' || ATCmdRxMsg[1][i] == 0 || i == 15) {
            if(idx < 4) {
              ip[idx++] = val;
              val = 0;
            }
          }
          if(idx > 3) {
            break;
          }
        }
        return 1;
      }
    }
  }
  else if(Setting.ExtModemModel == 2) {
    sprintf(MdmTmpString,"AT@DNSQ=%s\r",url);  
    resp = AT_SendWithRecieveData(MdmTmpString,"@DNSQUERY:", 1, 5000);
    if(resp == 1) {
      for(i = 0; i < 16; i++) {
        if(ATCmdRxMsg[0][i] >= '0' && ATCmdRxMsg[0][i] <= '9' )
          val = val*10 + ATCmdRxMsg[0][i]-'0';
        else if(ATCmdRxMsg[0][i] == '.' || ATCmdRxMsg[0][i] == 0) {
          if(idx < 4) {
            ip[idx++] = val;
            val = 0;
          }
        }
        if(idx > 3) {
          break;
        }
      }
      return 1;
    }
  }
  return 0;
}

/*
********************************************************************************
* Description : Modem_SocketOpen
* Arguments   : sw
* Return      : resp
* Note        : Modem TCP socket open
******************************************************************************** 
*/

unsigned char Modem_SocketOpenIP(u8* ip, u16 port)
{
  
  unsigned char resp = 0;

  TCP_Open = 0;
  
  printk("\r\nMODEM : Socket Open ip:%u.%u.%u.%u port: %u\r\n", ip[0],ip[1],ip[2],ip[3], port);

  memset(MdmTmpString,0,100);

  if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1)   {    
    sprintf(MdmTmpString,"AT$$TCP_ADDR=0,%u,%u,%u,%u,%u\r",ip[0],ip[1],ip[2],ip[3],port);  
    resp = AT_SendWithRecieveData(MdmTmpString,"$$TCP_ADDR: ", 1, 1000);
    if(resp == 1 && ATCmdRxMsg[0][0] == '0')
      resp = 1;
    else
      resp = 0;
    if(resp == 1)
      vTaskDelay(100);
    else
      return resp;
    
    resp = AT_SendWithRecieveData("AT$$TCP_SCOP\r","$$TCP_SCOP: ", 1, 2000);
    if(resp == 1 && ATCmdRxMsg[0][0] == '0')
      resp = 1;
    else
      resp = 0;
  }
  else if(Setting.ExtModemModel == 2) {
    sprintf(MdmTmpString,"AT@SOCKOPEN=0,0,1,%u.%u.%u.%u,%u\r",ip[0],ip[1],ip[2],ip[3],port);  
    resp = sendATcommand(MdmTmpString, "OK", "ERROR", 2000);
    if(resp == 1) { 
      resp = 1;
      vTaskDelay(100);
    }
    else {
      resp = 0;
      return resp;
    }
  }
  
  if(resp == 1)  {
    TCP_Open = 1;
  }
  return resp;
  
}

void Modem_CheckConnectIP(u32 tmp_ip, u16 port, u8 mode)
{
  if(tmp_ip == 0 || port == 0) return;
  if(mode == 0) {
    if((tmp_ip != ModemInfo.LpIP || port != ModemInfo.LpPort) && !LoopFotaFlag) {
      ModemInfo.LpIP = tmp_ip;
      ModemInfo.LpPort = port;
      FRAMMultiWrite((u8 *)&ModemInfo.LpIP, MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
    }
  }
  else if(mode == 2) {
    if(tmp_ip != ModemInfo.SrvIP || port != ModemInfo.SrvPort) {
      ModemInfo.SrvIP = tmp_ip;
      ModemInfo.SrvPort = port;
      FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, MODEM_INFO + (u8 *)&ModemInfo.SrvIP - (u8 *)&ModemInfo, sizeof(ModemInfo.SrvIP)+sizeof(ModemInfo.SrvPort));
    }
  }    
}


unsigned char Modem_SocketOpenSvrName(char *svr_name, u16 port, u8 mode)
{
  
  unsigned char ip[4] = {0,}; 
  unsigned char resp = 0;
  u32 tmp_ip = 0;
  
  TCP_Open = 0;
  
  if(mode != 1) {
    printk("\r\nMODEM : Socket Open Start  %s, %u\r\n", svr_name, port);  
    
    resp = Modem_GetIP(svr_name, ip);
    if(resp == 1) 
      vTaskDelay(100);
    else
      return resp;

    memcpy((u8*)&tmp_ip,ip,4);
    Modem_CheckConnectIP(tmp_ip, port, mode);
  }
  else {
    memcpy(ip, (u8*)svr_name, 4);
  }

  resp = Modem_SocketOpenIP(ip, port);
  
  return resp;
  
}

/*
********************************************************************************
* Description : Modem_SocketClose
* Arguments   : sw
* Return      : resp
* Note        : Modem TCP socket close
******************************************************************************** 
*/
unsigned char Modem_SocketClose(int sw)
{
  u8 resp;
  
  if(FotaRespFlag) FotaRespFlag = 0;
  if(sw == 0)  {
    vTaskDelay(1000);
    
    ModemCommInit();
    if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1){
      TxStrToModem("AT$$TCP_SCCL\r");
      resp = 1; 
    }
    else if(Setting.ExtModemModel == 2){
      resp = sendATcommand("AT@SOCKCLOSE=0\r", "OK", "ERROR", 5000);      
      if(resp == 1) 
        resp = 1;
      else
        resp = 0;
    }
    if(resp == 1) {
      TCP_Open = 0;
      SocketConnectStep = SOCKET_CLOSE;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;  
      Urc.Cnt = 0;    
    }
    vTaskDelay(1000);
  }
  else {
    resp = Modem_PPPClose();
  }
  return resp;
}



void TellRespDecode(void) 
{
  if(TellRespNo > 499  && TellRespNo < 600)
    KtfState = TellRespNo;
  
  switch(TellRespNo) {
    case 40:
      UsimState = 2;
      break;
    case 41:
      UsimState = 1;
      break;
    case 42:
      UsimState = 3;
      break;
      
    case 70:
      SmsState = 1;
      break;

    case 92:
      PppState = 2;
      break;
      
    case 200:
      PppState = 0;    //Connect 
      break;      
    case 201:
      PppState = 2;    //Connect Reject
      break;
    case 204:
      PppState = 2;    //Connect Reject
      break;
    case 217:
      PppState = 3;    //Connect Close
      break;
      
    case 600:
      PppState = 1;   //Connect Sucess
      break;
    case 601:
      TcpState = 3;
      PppState = 3;   //Connect Close
      break;
    case 603:
      TcpState = 1;   //Connect Sucess
      break;
    case 604:
      TcpState = 2;   //Connect Fail
      break;
    case 605:
      TcpState = 3;   //Connect Close
      break;
    case 620:
      TcpState = 4;   //Listening
      break;
    case 640:
      TcpState = 5;   //Connect Closed event
      break;

    case 814:
      Modem_FotaFlag = 1;
      printk("\r\nMODEM : MODEM FOTA SMS\r\n");
      ModemWaitCnt = 0;
      ModemStep = MODEM_FOTA_IDLE;     //Modem Fota Idle
      break;
    case 810:
      printk("\r\nMODEM : MODEM FOTA START\r\n");
      ModemWaitCnt = 0;
      ModemStep = MODEM_FOTA_IDLE;     //Modem Fota Idle
      break;
    case 820:
      printk("\r\nMODEM : MODEM DOWNLOAD START\r\n");
      ModemWaitCnt = 0;
      ModemStep = MODEM_FOTA_IDLE;     //Modem Fota Idle
      break;
    case 866:
      printk("\r\nMODEM : MODEM FOTA RESET\r\n");
      NVIC_SystemReset();
//      ModemStep = MODEM_RESET_REQ;
      break;
    case 868:
      printk("\r\nMODEM : MODEM FOTA ERROR\r\n");
      ModemStep = MODEM_RESET_REQ;
      break;
    case 880:
      printk("\r\nMODEM : MODEM FOTA FAIL\r\n");
      ModemStep = MODEM_RESET_REQ;
      break;
      
    case 900:
      TldState = 1;   //Boot up
      ModemStep = MODEM_POWER_ON_IDLE;  
      if(ModemPowerOnFlag) ModemWaitCnt = 0;
      ModemPowerOnFlag = 1;
      Setting.ExtModemModel = 0;
      break;
    case 901:
      TldState = 2;   //Reset
      break;
  }
  TellRespFlag = 0;
  TellRespNo = 0;
}

/*
********************************************************************************
* Description : Modem_URC_Parser
* Arguments   : r
* Return      : none
* Note        : Modem URC Parser
******************************************************************************** 
*/

#define MAX_SOCKET_RX_LEN 512

u8 RxHexStrStep;
u16 RxHexStrCnt, RxHexStrLen;
u8 RxBinData;
char RxHexStr[10];
unsigned int RxHexStrData(unsigned char r)
{
  if(r == '@') RxHexStrStep = 0;
  switch(RxHexStrStep) {
    case 0:
      if(r == '@') {
        RxHexStrStep = 1;
        RxHexStrCnt = 0;
        memset((u8*)RxHexStr, 0, 10);
      }
      break;
    case 1:
      if(r == '=') {
        if (strstr(RxHexStr, "SOCKRCV") != NULL) {
          RxHexStrStep = 2;
          RxHexStrLen = 0;
          RxHexStrCnt = 0;
        }
      }
      else if(RxHexStrCnt < 9){
        RxHexStr[RxHexStrCnt++] = r;
      }
      else RxHexStrStep = 0;
      break;
    case 2:
      if(r == ',') {
        RxHexStrCnt++;
        if(RxHexStrCnt == 3) {
          if(RxHexStrLen > 0) {
            RxHexStrLen = 2*RxHexStrLen;
            RxHexStrCnt = 0;
            RxHexStrStep = 3;
          }
          else
            RxHexStrStep = 0;
        }
      }
      else if((r >= '0' && r <= '9')) {
        if(RxHexStrCnt == 0) {
          if(r != '0') RxHexStrStep = 0;
        }
        if(RxHexStrCnt == 1) {
          if(r != '1') RxHexStrStep = 0;
        }
        if(RxHexStrCnt == 2) {
          RxHexStrLen = RxHexStrLen*10+(r-'0');
          if(RxHexStrLen > MAX_SOCKET_RX_LEN) RxHexStrStep = 0;
        }
      }
      else if(r != ' ') RxHexStrStep = 0;
      break;
    case 3:
      if((r >= '0' && r <= '9') || (r >= 'A' && r <= 'F')) {
        if(RxHexStrCnt%2 == 0) RxBinData = 0;
        if(r >= '0' && r <= '9') RxBinData = RxBinData*0x10+(r-'0');
        else if(r >= 'A' && r <= 'F') RxBinData = RxBinData*0x10+(r-'A'+0x0A);
        RxHexStrCnt++;
        if(RxHexStrCnt > RxHexStrLen) RxHexStrStep = 0;
        else if(RxHexStrCnt%2 == 0) return RxBinData;
      }
      else if(RxHexStrCnt > 0) RxHexStrStep = 0;
      break;
  }
  return 0xFFFF;
}

u8 DmsApnPushRxFlag, DmsApnPushRxCnt;
char DmsPushApnName[32];
void Modem_URC_Parser(unsigned char r) //interrupt msg procedure
{
  unsigned int rdata;
  //u16 addr;
  
//  if(ModemStep > MODEM_SOCKET_STAT)  return;
  switch(Urc.Mode) {
    case URCMODE_IDLE:
      //idle
      break;
      
    case URCMODE_SMS:
      if(Urc.RespFlag)
        SMS_Process(r);
      SMSDecodeWaitCnt++;
      if(SMSDecodeWaitCnt > 10000) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      break;
      
    case URCMODE_HANURI:
      if(ModemStep != MODEM_ON_NOTI && ModemStep != MODEM_DATA_SERVER) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      if(Urc.RespFlag) {
        Modem_RxChar_Hanuri(r);
        return;
      }
      break;
      
    case URCMODE_LOOP:
      if(ModemStep != MODEM_DATA_SERVER && ModemStep != MODEM_LOOP_FOTA) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        if(Setting.ExtModemModel == 2) {
          rdata = RxHexStrData(r);
          if(rdata != 0xFFFF) {
            r = rdata&0xff;
            Modem_RxChar_Loop(r);
          }
        }
        else
          Modem_RxChar_Loop(r);
        return;
      }
      break;
      
    case URCMODE_DMS:
      if(ModemStep != MODEM_ON_NOTI && ModemStep != MODEM_FOTA_REQ) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
//        if(ModemStep == MODEM_DATA_SERVER)
//          Gpck.Step = ST_SPK_ST;
      }
      if(Urc.RespFlag) {
        DMS_Rx_Process(r);
        return;
      }
      break;
      
    case URCMODE_GVN:
      if(ModemStep != MODEM_DATA_SERVER) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        Modem_RxChar_GV(r);
      }
      break;

    case URCMODE_VDIS:
      if(ModemStep != MODEM_VDIS_SERVER) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        Modem_RxChar_Vdis(URCMODE_VDIS, r);
      }
      break;
      
    case URCMODE_BLE:      
      if(ModemStep != MODEM_BLE_SERVER) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        Modem_RxChar_Vdis(URCMODE_BLE, r);
      }
      break;
      
    case URCMODE_CHILD:      
      if(ModemStep != MODEM_CHILD_SERVER) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        Modem_RxChar_Vdis(URCMODE_CHILD, r);
      }
      break;
      
    case URCMODE_GET:
      if(ModemStep != MODEM_GET_SERVER) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        Modem_RxCharGet_Vdis(r);
      }
      break;
 
    case URCMODE_ADAS:
      if(ModemStep != MODEM_ON_ADAS && ModemStep != MODEM_SEND_ADAS && ModemStep != MODEM_EVENT_ADAS && ModemStep != MODEM_OFF_ADAS) {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }
      else if(Urc.RespFlag) {
        ADAS_Rx_Process(r);
      }
      break;      
  }  

  if(r == '\r' || r == '\n') {
    Urc.Cnt = 0;
    if(r == '\n')
      memset(Urc.Buff,0, URC_BUFF_MAX);
    if(DmsApnPushRxFlag) {
      if(DmsApnPushRxCnt > 0) {
        memset((u8*)ComRxAPN_Name, 0, sizeof(ComRxAPN_Name));
        memcpy((u8*)ComRxAPN_Name, DmsPushApnName, DmsApnPushRxCnt);
        printk("\r\n");
        APNProcess(2);  
        ModemStep = MODEM_DATA_IDLE;
      }
      DmsApnPushRxFlag = 0;
      printk("\r\n");
    }
  }
  else if(Urc.Cnt < URC_BUFF_MAX && !FotaRespFlag) {
    Urc.Buff[Urc.Cnt++] = r;
    if(!Urc.RespFlag || Modem_FotaFlag) {
      if (strstr(Urc.Buff, "+CMTI: ") != NULL)
      {
        if(Modem_FotaFlag == 0) {
          Urc.Mode = URCMODE_SMS;
          Urc.RespFlag = 1;
          memset(&SMSData,0, sizeof(tsSMSData));
          memset(Urc.Buff,0, URC_BUFF_MAX);
          SMSDecodeWaitCnt = 0;
        }
        return;
      }
      else if (strstr(Urc.Buff, "$$NOTI:34") != NULL || strstr(Urc.Buff, "$$A_BOOT_ALERT") != NULL)  
      {
        Setting.ExtModemModel = 1;
        TldState = 1;   //Boot up
        ModemStep = MODEM_POWER_ON_IDLE;  
        if(ModemPowerOnFlag) ModemWaitCnt = 0;
        ModemPowerOnFlag = 1;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "@NOTI:34") != NULL)  
      {
        Setting.ExtModemModel = 2;
        TldState = 1;   //Boot up
        ModemStep = MODEM_POWER_ON_IDLE;  
        if(ModemPowerOnFlag) ModemWaitCnt = 0;
        ModemPowerOnFlag = 1;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "@SOCKOPEN:0,0") != NULL)  
      {
        TcpState = 1;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "@SOCKOPEN:0,1") != NULL)  
      {
        TcpState = 2;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "@SOCKCLOSED:0,0") != NULL)  
      {
        TcpState = 3;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      if (TellRespFlag == 0)   {
        if(strstr(Urc.Buff, "$$TELL: ") != NULL) {
          TellRespFlag = 1;
          TellRespNo = 0;
          Urc.Cnt = 0;
        }
      }    
      else if(TellRespFlag) {
        if(TellRespFlag < 5) {
          if(r >= '0' && r <='9' && TellRespNo < 1000) {
            TellRespNo = TellRespNo*10 + (r-'0');
            TellRespFlag++;
          }
          else if(r == ','  && TellRespFlag > 1) {
            TellRespDecode();
          }
        }
        else  TellRespFlag = 0;
      }
      if (NetRespFlag == 0)   {
        if(strstr(Urc.Buff, "@NETSTI:") != NULL) {
          NetRespFlag = 1;
          NetRespNo = 0;
          Urc.Cnt = 0;
        }
      }    
      else if(NetRespFlag) {
        if(r >= '0' && r <='9') {
          NetRespNo =  (r-'0');
          if(NetRespNo == 0 || NetRespNo == 1) PppState = 3;
          if(NetRespNo == 2) PppState = 0;
          if(NetRespNo == 3) PppState = 1;
        }
        NetRespFlag = 0;
      }
    }
    if(ModemStep != MODEM_LOOP_FOTA && ModemStep != MODEM_FOTA_REQ) { 
      if (strstr(Urc.Buff, "$$SFOTA:01") != NULL)
      {
        QualPolTimeCnt = 0;
        QualNotiFlag = 2;
        QualNotiRetryCount = 0;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "$$SFOTA:07") != NULL)
      {
        FotaPolTimeCnt = 0;
        FotaFlag = 2;
        FotaRetryCount = 0;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "$$SFOTA:08") != NULL)
      {
        PushResetFlag = 1;
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        return;
      }
      else if (strstr(Urc.Buff, "$$SFOTA:10") != NULL)
      {
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        PushResetFlag = 2;
        return;
      }      
      else if (strstr(Urc.Buff, "$$SFOTA:12") != NULL)
      {
        if(TCP_Open) Modem_SocketClose(0);
        memset(Urc.Buff,0, URC_BUFF_MAX);
        Urc.Cnt = 0;
        DmsApnPushRxFlag = 1;
        DmsApnPushRxCnt = 0;
        memset(DmsPushApnName,0, sizeof(DmsPushApnName));
        return;
      }
      if(DmsApnPushRxFlag) {
        if(DmsApnPushRxCnt < sizeof(ComRxAPN_Name)) {
          if(r != ' ' && r != ',' && r != '*')
            DmsPushApnName[DmsApnPushRxCnt++] = r;
        }
        else
          DmsApnPushRxFlag = 0;
      }
    }
  }
  
  return;
}

/*
********************************************************************************
* Description : Modem_parse_proc
* Arguments   : 
* Return      : none
* Note        : Modem Rx data Parser
******************************************************************************** 
*/
void Modem_parse_proc(void)	
{
  int i;
  u8 err;
  char data = 0;
  
  for(i = 0; i < MODEM_COMM_RX_BUF_SIZE; i++)  {
    data = ModemCommGetChar(&err);
    if ( err == MODEM_COMM_NO_ERR ) {
      Modem_URC_Parser(data);
    }
    else break;    
  }
}

/*
********************************************************************************
* Description : Modem_StatusCheck
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
void Modem_SocketConnect(char* svr_name, u16 port, u8 mode)
{
//  if(socket == 0 && SocketConnectStep == SOCKET_CONNECT_START) {
//      SocketConnectStep = SOCKET_STATUS_CHECK;
//      SocketWaitCnt = 0;
//  }
  
  switch(SocketConnectStep) {
    case SOCKET_CONNECT_START:
      SocketWaitCnt++;
      if(SocketWaitCnt >= 2000) {
        SocketWaitCnt = 0;
        SocketRetryCnt = 0;
        SocketConnectStep = SOCKET_CREG_CHECK;
      }
      break;
      
    case SOCKET_CREG_CHECK:
      if(SocketWaitCnt == 0) {
        if(Modem_CREG_Check()==1) {
          SocketRetryCnt = 0;
          SocketWaitCnt = 0;
          SocketConnectStep = SOCKET_CSQ_CHECK;
        }
        else 
          SocketWaitCnt = 1;
      }
      else {
        SocketWaitCnt++;
        if(SocketWaitCnt >= 1000) {
          SocketWaitCnt = 0;
          SocketRetryCnt++;
          if(SocketRetryCnt > 2) {
            SocketRetryCnt = 0;
            SocketConnectStep = SOCKET_CREG_FAIL;
          }
        }
      }
      break;
    
    case SOCKET_CSQ_CHECK:
      if(SocketWaitCnt == 0) {
//        if(Modem_CSQ_Check()==1) {
        if(Modem_State_Check()==1) {
          SocketRetryCnt = 0;
          SocketWaitCnt = 0;
          if(SecData.RSSI >= 5 && SecData.RSSI < 40)  {
            if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1)
              SocketConnectStep = SOCKET_STATUS_CHECK; //SOCKET_OPEN;//SOCKET_STATUS_CHECK;
            else
              SocketConnectStep = SOCKET_OPEN;//SOCKET_STATUS_CHECK;
          }
          else 
            SocketConnectStep = SOCKET_CSQ_FAIL;
        }
        else 
          SocketWaitCnt = 1;
      }
      else {
        SocketWaitCnt++;
        if(SocketWaitCnt >= 1000) {
          SocketWaitCnt = 0;
          SocketRetryCnt++;
          if(SocketRetryCnt > 0) {
            SocketRetryCnt = 0;
            SocketConnectStep = SOCKET_CSQ_FAIL;
          }
        }
      }
      break;
      
    case SOCKET_STATUS_CHECK:
      if(SocketWaitCnt == 0) {
        if(PppState  != 1) {
          Modem_PPPOpen();
          SocketWaitCnt = 1;
        }
        else {
          SocketConnectStep = SOCKET_OPEN;
          SocketWaitCnt = 0;
        }
      }
      else {
        SocketWaitCnt++;
        if(PppState == 1) {  
          SocketWaitCnt = 0;
          SocketRetryCnt = 0;
          SocketConnectStep = SOCKET_OPEN;
        }
        else if(SocketWaitCnt >= 10000) {
          SocketWaitCnt = 0;
          SocketRetryCnt = 0;
          SocketConnectStep = SOCKET_FAIL;
        }
      }
      break;
      
    case SOCKET_OPEN:
      if(SocketWaitCnt == 0) {
        TcpState = 0;
        if(Modem_SocketOpenSvrName(svr_name, port, mode)) {
          SocketRetryCnt = 0;
          SocketWaitCnt = 0;
          SocketConnectStep = SOCKET_OPEN_WAIT;//SOCKET_CONNECT;
        }
        else 
          SocketWaitCnt = 1;
      }
      else {
        SocketWaitCnt++;
        if(SocketWaitCnt >= 1000) {
          SocketWaitCnt = 0;
          SocketRetryCnt++;
          if(SocketRetryCnt > 0) {
            SocketRetryCnt = 0;
            SocketConnectStep = SOCKET_FAIL;
          }
        }
      }
      break;
      
    case SOCKET_OPEN_WAIT:
      SocketWaitCnt++;
      if(TcpState == 1 || TcpState == 2 || TcpState == 3) {
        SocketRetryCnt = 0;
        SocketWaitCnt = 0;
        if(TcpState == 1 )
          SocketConnectStep = SOCKET_CONNECT;
        else
          SocketConnectStep = SOCKET_FAIL;
      }
      else if(SocketWaitCnt >= 30000) {
        SocketRetryCnt = 0;
        SocketWaitCnt = 0;
        SocketConnectStep = SOCKET_FAIL;
      }
      break;
      
    case SOCKET_CREG_FAIL:
      COM_Command_Char = 'G';
      break;

    case SOCKET_CSQ_FAIL:
      COM_Command_Char = 'Q';
      break;

    case SOCKET_CONNECT:      
      COM_Command_Char = 'C';
      break;

    case SOCKET_CLOSE:
      break;
      
    case SOCKET_FAIL:      
      COM_Command_Char = 'T';
      break;

    default : 
      SocketConnectStep = SOCKET_FAIL;
      break;  
  }                         
}

/*
********************************************************************************
* Description : Modem_ResetProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/

void Modem_ResetProcess(void)
{
  u8 resp = 0;

  if(Setting.ExtModemModel == 0)   
    resp = sendATcommand("AT$$MODE=3\r", "OK", "ERROR", 10000);
  else if(Setting.ExtModemModel == 1)   
    resp = sendATcommand("AT$$RESET\r", "OK", "ERROR", 1000);
  else if(Setting.ExtModemModel == 2)   
    resp = sendATcommand("AT@HWRESET\r", "OK", "ERROR", 1000);
  if(resp == 1) {
    ModemPowerOnFlag = 0;
    ModemInitCnt = 0;
    ModemWaitCnt = 0;
    Urc.Mode = URCMODE_IDLE;
    Urc.RespFlag = 0;
    Urc.Cnt = 0;
    ModemStep = MODEM_POWER_ON_IDLE;
  }
  else
    ModemStep = MODEM_POWER_ON;
}


/*******************************************************************************
* Function Name  : Modem_Parameter_Reset
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Modem_Parameter_Reset(void)
{
  u8 ip[4];
  u16 addr;

  Setting.ExtModemMode = EXT_MODEM_PROTOCOL_TYPE;  
  addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
  FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));   

  Setting.ExtModemVDIS = EXT_MODEM_VDIS_MODE;  
  addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemVDIS); 
  FRAMMultiWrite((u8 *)&Setting.ExtModemVDIS, addr, sizeof(Setting.ExtModemVDIS));

  Setting.ExtModemADAS = EXT_MODEM_ADAS_MODE;  
  addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemADAS); 
  FRAMMultiWrite((u8 *)&Setting.ExtModemADAS, addr, sizeof(Setting.ExtModemADAS));
  
  if(Setting.ExtModemVDIS != VDIS_BLE_MODE) {
    if(EXT_MODEM_SERVER == WELLTIZEN_SERVER)
      ModemInfo.DataServerInterval = 30;
    else
      ModemInfo.DataServerInterval = 180;
  }
  else 
    ModemInfo.DataServerInterval = 30;
  FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.DataServerInterval));          

  ModemInfo.DataInterval = 1;
  FRAMMultiWrite((u8 *)&ModemInfo.DataInterval, MODEM_INFO + (u8 *)&ModemInfo.DataInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.DataInterval));          

  ModemInfo.FactoryInit = 1;
  FRAMMultiWrite((u8 *)&ModemInfo.FactoryInit, MODEM_INFO + (u8 *)&ModemInfo.FactoryInit - (u8 *)&ModemInfo, sizeof(ModemInfo.FactoryInit));          
 
#ifdef ISG_ENABLE  
  memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
  sprintf(HostSvr.SvrName,"lb.gg-bus.com");
  HostSvr.SvrPort = 7777;
  FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));

  ip[0] = 133; ip[1] = 186; ip[2] = 152; ip[3] = 190; 
  
  memcpy(&ModemInfo.LpIP, ip, 4);
  ModemInfo.LpPort = HostSvr.SvrPort;
  FRAMMultiWrite((u8 *)&ModemInfo.LpIP, MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
#else
  if(EXT_MODEM_SERVER == LOOP_SERVER) {
    memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
    sprintf(HostSvr.SvrName,"lb.loop-iot.com");
    HostSvr.SvrPort = 7777;
    FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));

    ip[0] = 14; ip[1] = 63; ip[2] = 216; ip[3] = 18;      // Loop server
    
    memcpy(&ModemInfo.LpIP, ip, 4);
    ModemInfo.LpPort = 7777;
    FRAMMultiWrite((u8 *)&ModemInfo.LpIP, MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
  }
  else {
    memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
    sprintf(HostSvr.SvrName,"dtg.welltizen.kr");
    HostSvr.SvrPort = 52001;
    FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));

    ip[0] = 211; ip[1] = 239; ip[2] = 165; ip[3] = 100;   // Welltizen server
    
    memcpy(&ModemInfo.LpIP, ip, 4);
    ModemInfo.LpPort = 52001;
    FRAMMultiWrite((u8 *)&ModemInfo.LpIP, MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
  }
#endif  
  memset(FWDN_Svr.SvrName,0,60);
  if(EXT_MODEM_SERVER == LOOP_SERVER) {
    sprintf(FWDN_Svr.SvrName, "lb.loop-ota.com");
    FWDN_Svr.SvrPort = 7777;
  }
  else {
    sprintf(FWDN_Svr.SvrName, "ota.welltizen.com");
    FWDN_Svr.SvrPort = 7893;
  }
  FRAMMultiWrite((u8 *)&FWDN_Svr, OTA_FWDN_SVR, sizeof(tsSvrInfo));
  
  FRAMMultiRead((u8 *)&DataSaveAddr, DRV_SAVE_ADDR, sizeof(DataSaveAddr));   
  DataSentAddr = DataSaveAddr;
  FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));   
  
  SendData_StartTime = 0;
  FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
  
  Modem_Parameter_Reset_Hanuri();  
  
  Modem_Parameter_Reset_DMS();
  
  Modem_Parameter_Reset_GVN();  
  
  Modem_Parameter_Reset_ADAS();  
  
  if(Setting.ExtModemVDIS != NOT_USE_VDIS) 
    Modem_Parameter_Reset_Vdis();
}

void ModemInit(void)
{
  u8 ip[4];
  
  FRAMMultiRead((u8 *)&ModemInfo, MODEM_INFO, sizeof(tsModemInfo));

  if(ModemInfo.DataServerInterval < 10 || ModemInfo.DataServerInterval > 3600) {
    if(ModemInfo.DataServerInterval < 10 ) ModemInfo.DataServerInterval = 10;
    else ModemInfo.DataServerInterval = 180;
    FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.DataServerInterval));          
  }

  if(ModemInfo.DataInterval == 0 || ModemInfo.DataInterval > 60) {
    if(ModemInfo.DataInterval == 0 ) ModemInfo.DataInterval = 1;
    else ModemInfo.DataInterval = 60;
    FRAMMultiWrite((u8 *)&ModemInfo.DataInterval, MODEM_INFO + (u8 *)&ModemInfo.DataInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.DataInterval));          
  }
  
  if(ModemInfo.LpIP == 0 || ModemInfo.LpIP == 0xffffffff) {

#ifdef ISG_ENABLE 
    ip[0] = 133; ip[1] = 186; ip[2] = 152; ip[3] = 190; 
#else
    ip[0] = 14; ip[1] = 63; ip[2] = 216; ip[3] = 18; 
#endif    
    
    memcpy(&ModemInfo.LpIP, ip, 4);
    ModemInfo.LpPort = 7777;
    FRAMMultiWrite((u8 *)&ModemInfo.LpIP, MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));          
    memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
    memcpy(ip, &ModemInfo.LpIP, 4);
    sprintf(HostSvr.SvrName,"%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    HostSvr.SvrPort = ModemInfo.LpPort;
    FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
  }

  FRAMMultiRead((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
  if(HostSvr.SvrName[0] <= ' ' || HostSvr.SvrName[0] > 'z' || HostSvr.SvrPort == 0 || HostSvr.SvrPort >= 100000) {
    memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
    memcpy(ip, &ModemInfo.LpIP, 4);
    sprintf(HostSvr.SvrName,"%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    HostSvr.SvrPort = ModemInfo.LpPort;
    FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
  }
  
  FRAMMultiRead((u8 *)&FWDN_Svr, OTA_FWDN_SVR, sizeof(tsSvrInfo));
  if(FWDN_Svr.SvrName[0] <= ' ' || FWDN_Svr.SvrName[0] > 'z' || FWDN_Svr.SvrPort == 0 || FWDN_Svr.SvrPort >= 100000) {
    memset(FWDN_Svr.SvrName,0,60);
    if(EXT_MODEM_SERVER == LOOP_SERVER) {
      sprintf(FWDN_Svr.SvrName, "lb.loop-ota.com");
      FWDN_Svr.SvrPort = 7777;
    }
    else {
      sprintf(FWDN_Svr.SvrName, "ota.welltizen.com");
      FWDN_Svr.SvrPort = 7893;
    } 
    FRAMMultiWrite((u8 *)&FWDN_Svr, OTA_FWDN_SVR, sizeof(tsSvrInfo));
  }
//  memset(FWDN_Svr.SvrName,0,60);
//  sprintf(FWDN_Svr.SvrName, "211.235.246.5");
  
//  FRAMMultiRead((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
//  if(APN_Name[0] < ' ' || APN_Name[0] > 'z') {
//    memset(APN_Name,0,32);
//    if(Setting.ExtModemTelecom == 0) {
//      if(Setting.ExtModemModel == 0)
//        sprintf(APN_Name, "biz.ktfwing.com");   
//      else
//        sprintf(APN_Name, "privatelte.ktfwing.com");   
//    }
//    else if(Setting.ExtModemTelecom == 1) {
//      sprintf(APN_Name, "internet.sktelecom.com");   
//    }
//    else if(Setting.ExtModemTelecom == 2) {
//      sprintf(APN_Name, "m2m-router.lguplus.co.kr");   
//    }
//    FRAMMultiWrite((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
//  }
  FRAMMultiRead((u8 *)&APN_Name, APN_NAME_INFO, sizeof(APN_Name));
  sprintf(Fota_FW_Ver,"%1u.%1u.%1u.%1u", MAJOR, MINOR, RELEASE, BUILD);
  
  Modem_Init_Loop();

  Modem_Init_Hanuri();

  Modem_Init_DMS();
  
  Modem_Init_GVN();

  Modem_Init_Vdis();
  
  Modem_Init_ADAS();
  
  ModemCommInit();
  
  ModemStep = MODEM_POWER_ON;

  DataAddrToSend = DataSentAddr;  
  
#ifdef LOOP_OTA_SERVER_ENABLE  
  if(Setting.ExtModemMode != KT_GVN_PROTOCOL) 
    LoopFotaFlag = 1;
#endif  
}

void CheckAPNChange(void)
{
  if(MtReadAPNFlag) {
    APNProcess(0);
    MtReadAPNFlag = 0;
  }
  else if(MtChangeAPNFlag) {
    APNProcess(1);
    MtChangeAPNFlag = 0;
  }
  else if(CLI_APN_ChangeFlag) {
    APNProcess(3);
  }
}


void  Modem_KeyOffProcess(void)
{
//  if(ModemReadyFlag) {
    if(Setting.ExtModemVDIS == VDIS_BLE_MODE || Setting.ExtModemVDIS == VDIS_DTG_MODE)  {
      if(!Vdis_TxTerminateFlag && !Vdis_NeworkErrorFlag) {
        if(GetServerFlag == 0) {  ///분산서버 연결 되었을때...
          VdisServerFlag = 2;
          VdisAddrToSend = DataSaveAddr;
        }
        else {
          VdisServerFlag = 0;
          VdisBleServerFlag = 0;
          VdisChildServerFlag = 0;
        }
        if(Setting.ExtModemVDIS == VDIS_BLE_MODE) {// && Setting.ExtModemMode == VDIS_PROTOCOL_ONLY) 
          VdisBleEndFlag = 1;
          VdisChildEndFlag = 1;
        }
      }
    }
    
    if(Setting.ExtModemMode == LOOP_PROTOCOL)  {
      DataServerFlag = 2;
      DataAddrToSend = DataSaveAddr;
      BleEndFlag = 1;
    }
    else if(Setting.ExtModemMode == HANURI_PROTOCOL)  {
      VehStateFlag = 1;
      DataServerFlag = 2;
      DataAddrToSend = DataSaveAddr;
    }
    else if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {  
      if(!TxTerminateFlag && !NeworkErrorFlag && !SDRPolicyRetryDelayFlag) {
        DataServerFlag = 2;
        DataAddrToSend = DataSaveAddr;
      }
      else if(VdisServerFlag == 0 && VdisBleEndFlag == 0){
        if(Setting.ExtModemADAS != ADAS_DTG_MODE) {
          OffNotiFlag = 1;
          OffNotiRetryCount = 0;
        }
      }
    }
    
    if(Setting.ExtModemADAS == ADAS_DTG_MODE) {
      SendAdasFlag = 2;
      SendAdasRetryCount = 0;        
    }
//  }
}


u32 SecCnt;
void Modem_SecIntProcess(void)
{
  
  if(Start_Flag && !NotRegist_Flag) {  
    SecCnt++;
    if(SecCnt > 59) {
      SecCnt = 0;
    }        
    if(SecCnt%10 == 0)
      memset(&SecData.Status,0,sizeof(SecData.Status));

    if(Setting.ExtModemMode == LOOP_PROTOCOL)  
      Modem_SecInt_Loop();
    else if(Setting.ExtModemMode == HANURI_PROTOCOL)  
      Modem_SecInt_Hanuri();
    else if(Setting.ExtModemMode == KT_GVN_PROTOCOL) { 
      Modem_SecInt_DMS();
      Modem_SecInt_GVN();
    }
    if(Setting.ExtModemADAS == ADAS_DTG_MODE)
      Modem_SecInt_ADAS();
    if(Setting.ExtModemVDIS != NOT_USE_VDIS) 
      Modem_SecInt_Vdis();
    
  }
}
      
/*
********************************************************************************
* Description : Task_MOD
* Arguments   : none
* Return      : 
* Note        : task for modem
******************************************************************************** 
*/
//unsigned char sbuf[640];
void Task_MOD(void const * argument)
{
  ModemInit();
  
  for(;;)	{
    if(Start_Flag) {
      Modem_parse_proc();         
      
      switch(ModemStep) {
        
        case MODEM_POWER_ON:
          ModemPowerOnFlag = 0;
          ModemInitCnt = 0;
          Urc.Mode = URCMODE_IDLE;
          Urc.RespFlag = 0;
          Urc.Cnt = 0;
          Modem_FotaFlag = 0;
          Modem_PowerOn();
          ModemStep = MODEM_POWER_ON_IDLE;
          ModemWaitCnt = 0;
          break;
          
        case MODEM_POWER_ON_IDLE:
          ModemWaitCnt++;
          if(ModemWaitCnt < 300000) {  
            if(ModemWaitCnt > 30000) {
              if(ModemPowerOnFlag) {
                ModemWaitCnt = 0;
                ModemInitCnt = 0;
                ModemStep = MODEM_INITIALIZE;
              }
              else if((ModemWaitCnt %10000) == 0){
                if(Modem_PowerOnCheck())
                  ModemPowerOnFlag = 1;
              }
            }
            if((ModemWaitCnt%10000) == 0)  //1sec
              printk("\r\nMODEM : Power idle %u\r\n",ModemWaitCnt/1000);
          }
//          else
//              ModemStep = MODEM_POWER_ON;   // 5min 후 모뎀리셋.  --> 전화번호에러나 망연동이 안되면 중지 상태 유지..
          break;
          
        case MODEM_INITIALIZE:
          if(ModemWaitCnt == 0) {
            if(Modem_Initialize()==1) {
              ModemWaitCnt = 0;
              ModemInitCnt = 0;
              ModemReadyFlag = 1;
              ModemStep = MODEM_DATA_IDLE;    
//              if(Setting.ExtModemMode == KT_GVN_PROTOCOL && Prev_DataFlag)    DataServerFlag = 1;
            }
            else {
              ModemWaitCnt = 1;
            }
          }
          else {
            ModemWaitCnt++;
            if(ModemInitCnt < 2 && ModemWaitCnt > 10000) {           // 10초 후  추가 2회시도...
              ModemWaitCnt = 0;
              ModemInitCnt++;
            }
//            else if(ModemWaitCnt > 300000) {         // 5min 후 모뎀리셋.  --> 전화번호에러나 망연동이 안되면 중지 상태 유지..
//                ModemWaitCnt = 0;
//                ModemInitCnt = 0;
//                ModemStep = MODEM_POWER_ON;
//            }
          }
          break;
          
        case MODEM_DATA_IDLE:
          SocketConnectStep = SOCKET_CONNECT_START; 
          CheckAPNChange();
          
          if(!CLI_APN_ChangeFlag && !ModemServiceFault) {
            if(LoopFotaFlag) {
              LoopFotaStep = LOOP_FOTA_CONNECT;
              ModemStep = MODEM_LOOP_FOTA;
            }
            else {
              if(Setting.ExtModemMode == LOOP_PROTOCOL) 
                Modem_DataIdleProcess_Loop();          
              else if(Setting.ExtModemMode == HANURI_PROTOCOL)
                Modem_DataIdleProcess_Hanuri();
              else if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {
                Modem_DataIdleProcess_DMS();
                Modem_DataIdleProcess_GVN();
              }
              if(ModemStep == MODEM_DATA_IDLE) {  // 순서 바뀌면 추가
                if(Setting.ExtModemVDIS != NOT_USE_VDIS) {
                  Modem_DataIdleProcess_Vdis();
                }
                if(Setting.ExtModemADAS == ADAS_DTG_MODE) {
                  Modem_DataIdleProcess_ADAS();
                }
              }
            }
          }
          
          if(ModemStep == MODEM_DATA_IDLE) {
            ModemWaitCnt++;
            if((ModemWaitCnt % 10000) == 0) { //10sec
              printk("\r\nMODEM : Data idle %u\n",ModemWaitCnt/1000);
              Modem_CSQ_Check();
            }
          }
          else ModemWaitCnt = 0;
          break;

        case MODEM_GET_SERVER:
          if(Setting.ExtModemVDIS != NOT_USE_VDIS)
            Modem_GetSvrProcess_Vdis();
          else {
            ModemStep = MODEM_DATA_IDLE;
            GetServerFlag = 3; // 재전송 중지
          }
          break;

        case MODEM_VDIS_SERVER:
          if(Setting.ExtModemVDIS != NOT_USE_VDIS)
            Modem_VdisSvrProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            VdisServerFlag = 0;
          }
          break;
          
        case MODEM_BLE_SERVER:
          if(Setting.ExtModemVDIS == VDIS_BLE_MODE)
            Modem_BleSvrProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            VdisBleServerFlag = 0;
          }
          break;
          
        case MODEM_CHILD_SERVER:
          if(Setting.ExtModemVDIS == VDIS_BLE_MODE)
            Modem_ChildAlarmSvrProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            VdisChildServerFlag = 0;
          }
          break;
          
        case MODEM_DATA_SERVER:
          if(Setting.ExtModemMode == LOOP_PROTOCOL)
            Modem_DataSvrProcess_Loop();
          else if(Setting.ExtModemMode == HANURI_PROTOCOL)
            Modem_DataSvrProcess_Hanuri();
          else if(Setting.ExtModemMode == KT_GVN_PROTOCOL)
            Modem_DataSvrProcess_GVN();
          else {
            ModemStep = MODEM_DATA_IDLE;
            DataServerFlag = 0;
          }
          break;
          
        case MODEM_ON_NOTI:
          if(Setting.ExtModemMode == HANURI_PROTOCOL)
            Modem_OnNotiProcess_Hanuri();
          else if(Setting.ExtModemMode == KT_GVN_PROTOCOL)
            Modem_OnNotiProcess_DMS();
          else {
            ModemStep = MODEM_DATA_IDLE;
            OnNotiFlag = 0;
          }
          break;

        case MODEM_QUAL_NOTI:
          if(Setting.ExtModemMode == KT_GVN_PROTOCOL)
            Modem_QualNotiProcess_DMS();
          else {
            ModemStep = MODEM_DATA_IDLE;
            QualNotiFlag = 0;
          }
          break;

        case MODEM_OFF_NOTI:
          if(Setting.ExtModemMode == KT_GVN_PROTOCOL)
            Modem_OffNotiProcess_DMS();
          else {
            ModemStep = MODEM_DATA_IDLE;
            OffNotiFlag = 0;
          }
          break;
          
        case MODEM_FOTA_REQ:
          if(Setting.ExtModemMode == KT_GVN_PROTOCOL)
            Modem_FotaProcess_DMS();
          else {
            ModemStep = MODEM_DATA_IDLE;
            FotaFlag = 0;
          }
          break;
          
        case MODEM_LOOP_FOTA:
          Modem_LoopFotaProcess();
          break;
          
        case MODEM_ON_ADAS:
          if(Setting.ExtModemADAS == ADAS_DTG_MODE)
            Modem_OnAdasProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            OnAdasFlag = 0;
          }          
          break;
          
        case MODEM_SEND_ADAS:
          if(Setting.ExtModemADAS == ADAS_DTG_MODE)
            Modem_SendAdasProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            SendAdasFlag = 0;
          }           
          break;
          
        case MODEM_EVENT_ADAS:
          if(Setting.ExtModemADAS == ADAS_DTG_MODE)
            Modem_EventAdasProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            EventAdasFlag = 0;
          }           
          break;
          
        case MODEM_OFF_ADAS:
          if(Setting.ExtModemADAS == ADAS_DTG_MODE)
            Modem_OffAdasProcess();
          else {
            ModemStep = MODEM_DATA_IDLE;
            OffAdasFlag = 0;
          }           
          break;
          
        case MODEM_POWER_OFF:
          if(Power_Off_Flag) {
            ADAS_Trip_Count();
            Modem_PowerOff();
            ModemWaitCnt = 0;
            ModemOffFlag = 1;
            OffNotiFlag = 0;
          }
          else if(PushResetStart) {
            ADAS_Trip_Count();
            Modem_PowerOff();
            System_PowerOff();
          }
          ModemStep = MODEM_POWER_OFF_IDLE;
          break;

        case MODEM_POWER_OFF_IDLE:
          if(!Power_Off_Flag) {
            ModemStep = MODEM_POWER_ON;
          }
          break;
          
        case MODEM_RESET_REQ:
          Modem_ResetProcess();
          break;

        case MODEM_IDLE:
          ModemWaitCnt++;
          if(ModemWaitCnt > 30000) {  //30sec
            ModemWaitCnt = 0;
            ModemStep = MODEM_DATA_IDLE;    
          }
          break;
          
        case MODEM_FOTA_IDLE:
          ModemWaitCnt++;
          if(ModemWaitCnt > 180000) {  //180sec
            ModemWaitCnt = 0;
            Modem_FotaFlag = 0;
            Modem_ResetProcess();
          }
          else if(Modem_FotaFlag == 0) 
            Modem_FotaFlag = 1;
          break;
          
        default : //loop n~ err? = led red?
          ModemWaitCnt = 0;
          ModemStep = MODEM_POWER_ON_IDLE;
          break;
          
      }//switch(ModemStep) {
//      if(Power_Off_Mode && !DataServerFlag && !OffNotiFlag && ModemStep <= MODEM_DATA_IDLE)
//        ModemStep = MODEM_POWER_OFF;      
    }//if(Flags.StartFlag)
    osDelay(1);
  }//for(;;)	{
}

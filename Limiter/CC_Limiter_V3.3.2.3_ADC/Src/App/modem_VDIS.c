/*
*************************************** (C) COPYRIGHT 2015 LOOP ****************
*
* File Name          : modem.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 06/28/2016
*
* Description        : modem function 
*
********************************************************************************
*/
#define MODEM_VDIS_GLOBALS

#include "includes.h"
#include "modem_VDIS.h"
#include "modem_DMS.h"

typedef enum {
  BLE_SVR_DATACHECK,
  BLE_SVR_CONNECT,
  BLE_SVR_SEND_WAIT,
  BLE_SVR_SEND,
  BLE_SVR_RECIEVE_WAIT,
  BLE_SVR_CLOSE,
  BLE_SVR_FAIL,
  BLE_SVR_SUCCESS,
  BLE_SVR_RETRY_WAIT,
  BLE_SVR_IDLE
}teBLE_SVR_STATUS;

u8 BleSvrStep;
u32 BleSvrWaitCount;

typedef enum {
  CHILD_SVR_DATACHECK,
  CHILD_SVR_CONNECT,
  CHILD_SVR_SEND_WAIT,
  CHILD_SVR_SEND,
  CHILD_SVR_RECIEVE_WAIT,
  CHILD_SVR_CLOSE,
  CHILD_SVR_FAIL,
  CHILD_SVR_SUCCESS,
  CHILD_SVR_RETRY_WAIT,
  CHILD_SVR_IDLE
}teCHILD_SVR_STATUS;

u8 ChildSvrStep;
u32 ChildSvrWaitCount;

tsVDRPolicy VDRPolicy; 

u32 Vdis_SendTimeCnt, Get_SendTimeCnt;
u8 Vdis_RxMsg[100];
tsSecData Vdis_RD_Sec_Data, Vdis_Prev_RD_Sec_Data, Vdis_Last_Tx_Data;

char VdisHeader[128];
u16 VdisHeaderLen;

/*******************************************************************************
* Function Name  : Save_GetSvrData
* Description    : Save Get Server Info
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char GET_IP[60];
unsigned short GET_SendTime;
unsigned char GET_SaveTime; 
u32 GET_Port;
void Save_GetSvrData(void)
{
  u32 addr;
  u16 strLen;
  
  strLen = StringLen(GET_IP,60);
  if(!IsSameString((char*)VdisDataSvr.SvrName, (char*)GET_IP, strLen)) {
    memset((u8*)VdisDataSvr.SvrName, 0, sizeof(VdisDataSvr.SvrName));
    memcpy((u8*)VdisDataSvr.SvrName, (u8*)GET_IP, strLen);
    FRAMMultiWrite((u8 *)&VdisDataSvr.SvrName, VDIS_DATA_SVR, sizeof(VdisDataSvr.SvrName));
  }
  
  if(VdisDataSvr.SvrPort != GET_Port) {
  	VdisDataSvr.SvrPort = GET_Port;
    addr = VDIS_DATA_SVR + (u8 *)&VdisDataSvr.SvrPort - (u8 *)&VdisDataSvr;
    FRAMMultiWrite((u8 *)&VdisDataSvr.SvrPort, addr, sizeof(VdisDataSvr.SvrPort));
  }  
  
  if(VDRPolicy.DataServerInterval != GET_SendTime) {
		VDRPolicy.DataServerInterval = GET_SendTime;
    addr = VDIS_VDR_POLICY + (u8 *)&VDRPolicy.DataServerInterval - (u8 *)&VDRPolicy;          
    FRAMMultiWrite((u8 *)&VDRPolicy.DataServerInterval, addr, sizeof(VDRPolicy.DataServerInterval));
  }
  
  if(VDRPolicy.DataInterval != GET_SaveTime) {
    VDRPolicy.DataInterval = GET_SaveTime;
    addr = VDIS_VDR_POLICY + (u8 *)&VDRPolicy.DataInterval - (u8 *)&VDRPolicy;          
    FRAMMultiWrite((u8 *)&VDRPolicy.DataInterval, addr, sizeof(VDRPolicy.DataInterval));
  }  
}

/*
********************************************************************************
* Description : Modem_RxChar_Get
* Arguments   : data
* Return      : none
* Note        : Http receive process
******************************************************************************** 
*/
int GET_TmpNum;
u8 GET_Discard_Cnt;
u8 GET_Term_Cnt;

void Modem_RxCharGet_Vdis(unsigned char r) //interrupt msg procedure
{
  if((r != '\r') && (r != '\n')) {
    if(HttpRxStep >= HTTP_RX_DATA_SUCCESS) {
      if(r == '%') {
          HexEncodeFlag = 1;
          HexEncodeVal = 0;
      }
      else if(HexEncodeFlag) {
        if(r >= '0' && r <= '9') {
          HexEncodeVal = (HexEncodeVal<<4) + r-'0';
          HexEncodeFlag++;
        }
        else if(r >= 'A' && r <= 'F') {
          HexEncodeVal = (HexEncodeVal<<4) + r-'A'+10;
          HexEncodeFlag++;
        }
        else
          HexEncodeFlag = 0;
      }
      
      if(HexEncodeFlag) {
        if(HexEncodeFlag >= 3) {
          r = HexEncodeVal;
          HexEncodeFlag = 0;
        }
        else 
          return;
      }
    }
    
    if(HexEncodeFlag == 0 && HttpRxCnt < HTTP_RX_BUFF_MAX) 
      HttpRxStrBuff[HttpRxCnt++] = r;
  }
  
  switch(HttpRxStep) {
    case HTTP_RX_INIT:
      if(HttpRxCnt >= 8) {
        if (strstr(HttpRxStrBuff, "HTTP/1.1") != NULL) {
          HttpRxStep = HTTP_RX_RESP_CODE;
          HttpRxCnt = 0;
        }  
      }
      if(r == '\n') 
        HttpRxCnt = 0;
      break;
      
    case HTTP_RX_RESP_CODE:
      if(HttpRxCnt > 3) {       
        if(strstr(HttpRxStrBuff, "200") != NULL) {
          HttpRespCode = 200;
          GetSvrWaitCount = 0;
          GET_Discard_Cnt = 0;
          GET_Term_Cnt = 0;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "403") != NULL) {
          HttpRespCode = 403;
          GET_Discard_Cnt = 0;
          GET_Term_Cnt = 0;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "400") != NULL) {
          HttpRespCode = 400;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "404") != NULL) {
          HttpRespCode = 404;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "406") != NULL) {
          HttpRespCode = 406;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "500") != NULL) {
          HttpRespCode = 500;
          GET_Discard_Cnt = 0;
          GET_Term_Cnt = 0;
//          HttpNetErrCnt++;
//          if(HttpNetErrCnt >= VDRPolicy.RetryCnt) {
//            HttpNetErrCnt = 0;
//          }
        }
        else if(strstr(HttpRxStrBuff, "503") != NULL) {
          HttpRespCode = 503;
          GET_Discard_Cnt = 0;
          GET_Term_Cnt = 0;
//          HttpNetErrCnt++;
//          if(HttpNetErrCnt >= VDRPolicy.RetryCnt) {
//            HttpNetErrCnt = 0;
//          }
        }
        if(HttpRespCode > 0)
          HttpRxStep = HTTP_RX_LEN_WAIT;
      }
      if(r == '\n') 
        HttpRxCnt = 0;
      break;
      
    case HTTP_RX_LEN_WAIT:
      if(HttpRxCnt == 15) {       
        if (strstr(HttpRxStrBuff, "Content-Length:") != NULL)
        {
          HttpRxStep = HTTP_RX_LEN_VAL;
          HttpRxCnt = 0;
          HttpRxDataLen = 0;
        }  
      }
      if(r == '\n') 
        HttpRxCnt = 0;
      break;

    case HTTP_RX_LEN_VAL:
      if(r >= '0' && r <= '9') {
        HttpRxDataLen = (HttpRxDataLen*10) + (r-'0');
      }
      else if(r == '\n') {
        HttpRxStep = HTTP_RX_DATA_WAIT;
        HttpRxCnt = 0;
      }
      break;
     case HTTP_RX_DATA_WAIT:      //Wait Data
      if(r == '\n') {
        if(HttpRxCnt == 0) { 
          HttpRxLineCnt = 0;
          HttpRxDataCnt = 0;
          HttpRxCnt = 0;
          HttpRxOrCnt = 0;
          if(HttpRespCode == 200) 
            HttpRxStep = HTTP_RX_DATA_SUCCESS;
          else 
            HttpRxStep = HTTP_RX_DATA_FAIL;
        }
        HttpRxCnt = 0;
      }
      break;
      
    case HTTP_RX_DATA_SUCCESS:
      if(HttpRxLineCnt == 0) {        // v=
        if(!HttpRxOrCnt && HttpRxCnt == 2) {       
          if (strstr(HttpRxStrBuff, "v=") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {
          if(HttpRxCnt == 1) {
            GET_TmpNum = 0;
          }
          if(r >= '0' && r <= '9') {
            GET_TmpNum = (GET_TmpNum*10) + (r-'0');
          }
        }
      }
      else if(HttpRxLineCnt == 1) {    //    
        if(!HttpRxOrCnt && HttpRxCnt == 2) {               
          if (strstr(HttpRxStrBuff, "o=") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {        
          if(r == '\n') {
            if(HttpRxCnt < 100)
              memcpy(Vdis_RxMsg, HttpRxStrBuff, HttpRxCnt);
            HttpRxCnt = 0;
            HttpRxOrCnt = 0;
          }        
        }
      }    
      else if(HttpRxLineCnt == 2) {    // Commet   
        if(!HttpRxOrCnt && HttpRxCnt == 2) {               
          if (strstr(HttpRxStrBuff, "s=") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {        
          if(r == '\n') {
            if(HttpRxCnt < 100)
              memcpy(Vdis_RxMsg, HttpRxStrBuff, HttpRxCnt);
          }        
        }
      }    
      else if(HttpRxLineCnt == 3) {    // Dtg server IP   
        if(!HttpRxOrCnt && HttpRxCnt == 9) {               
          if (strstr(HttpRxStrBuff, "c=IN IP4 ") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {        
          if(r == '\n') {
            memset(GET_IP, 0, 60);
            memcpy(GET_IP, HttpRxStrBuff, HttpRxCnt);
          }
        }
      }      
      else if(HttpRxLineCnt == 4) {    // Dtg server port   
        if(!HttpRxOrCnt && HttpRxCnt == 14) {               
          if (strstr(HttpRxStrBuff, "m=application ") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {        
          if(HttpRxCnt == 1) {
            GET_Port = 0;
          }
          if(r >= '0' && r <= '9') {
            GET_Port = (GET_Port*10) + (r-'0');
          }          
        }
      }     
      else if(HttpRxLineCnt == 5) {    // Data Tx interval   
        if(!HttpRxOrCnt && HttpRxCnt == 10) {               
          if (strstr(HttpRxStrBuff, "a=vdr-blk:") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
            GET_SendTime = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {
          if(r >= '0' && r <= '9') {
            GET_SendTime = (GET_SendTime*10) + (r-'0');
          }          
        }
      } 
      
      else if(HttpRxLineCnt == 6) {    // Data save inerval 
        if(!HttpRxOrCnt && HttpRxCnt == 10) {               
          if (strstr(HttpRxStrBuff, "a=row-vdr:") != NULL)
          {
            HttpRxOrCnt = 1;
            HttpRxCnt = 0;
            GET_SaveTime = 0;
          }  
        }
        if(HttpRxOrCnt == 1) {
          if(r >= '0' && r <= '9') {
            GET_SaveTime = (GET_SaveTime*10) + (r-'0');
          }
        }
      }
      
      if(r == '\n') {
        HttpRxLineCnt++;
        HttpRxCnt = 0;
        HttpRxOrCnt = 0;
      }      
      HttpRxDataCnt++;
      
      if(HttpRxDataCnt >= HttpRxDataLen) {    // Data Length 더하기..
        Save_GetSvrData();
        ModemStep = MODEM_GET_SERVER; 
        GetSvrStep = GET_SVR_CLOSE;
        HttpRxStep = HTTP_RX_INIT;
        HttpRxDataCnt = 0;
        HttpRxDataLen = 0;
        HttpRxLineCnt = 0;
      }
      break;  
      
    case HTTP_RX_DATA_FAIL:      //idle 
      if(r == '\n') 
        HttpRxCnt = 0;
      HttpRxDataCnt++;
      if(HttpRxDataCnt >= HttpRxDataLen) {
        HttpRxStep = HTTP_RX_INIT;
        GetSvrWaitCount = 0;
        TCP_Open = 0;        
        ModemStep = MODEM_GET_SERVER; 
        GetSvrStep = GET_SVR_FAIL;
        HttpRxDataCnt = 0;
        HttpRxDataLen = 0;
        HttpRxLineCnt = 0;        
      }
      break;
  }
}  

/*
********************************************************************************
* Description : Modem_RxChar_GV
* Arguments   : data
* Return      : none
* Note        : Http receive process
******************************************************************************** 
*/
void Modem_RxChar_Vdis(u8 urc_mode, unsigned char r) //interrupt msg procedure
{
  if((r != '\r') && (r != '\n')) {
    if(HttpRxStep >= HTTP_RX_DATA_SUCCESS) {
      if(r == '%') {
          HexEncodeFlag = 1;
          HexEncodeVal = 0;
      }
      else if(HexEncodeFlag) {
        if(r >= '0' && r <= '9') {
          HexEncodeVal = (HexEncodeVal<<4) + r-'0';
          HexEncodeFlag++;
        }
        else if(r >= 'A' && r <= 'F') {
          HexEncodeVal = (HexEncodeVal<<4) + r-'A'+10;
          HexEncodeFlag++;
        }
        else
          HexEncodeFlag = 0;
      }
      
      if(HexEncodeFlag) {
        if(HexEncodeFlag >= 3) {
          r = HexEncodeVal;
          HexEncodeFlag = 0;
        }
        else 
          return;
      }
    }
    
    if(HexEncodeFlag == 0 && HttpRxCnt < HTTP_RX_BUFF_MAX) 
      HttpRxStrBuff[HttpRxCnt++] = r;
  }
  
  switch(HttpRxStep) {
    case HTTP_RX_INIT:
      if(HttpRxCnt >= 8) {
        if (strstr(HttpRxStrBuff, "HTTP/1.1") != NULL) {
          HttpRxStep = HTTP_RX_RESP_CODE;
          HttpRxCnt = 0;
//          HttpRespCode = 0;
        }  
      }
      if(r == '\n') 
        HttpRxCnt = 0;
      break;
      
    case HTTP_RX_RESP_CODE:
      if(HttpRxCnt > 3) {       
        if(strstr(HttpRxStrBuff, "200") != NULL) {
          HttpRespCode = 200;
          Vdis_NeworkErrorFlag = 0;
          HttpNetErrCnt = 0;
          if(urc_mode == URCMODE_VDIS)          
            VdisSvrWaitCount = 0;
          else if(urc_mode == URCMODE_BLE)          
            BleSvrWaitCount = 0;
          else
            ChildSvrWaitCount = 0;
          Vdis_Last_Tx_Data = Vdis_RD_Sec_Data;
        }
        else if(strstr(HttpRxStrBuff, "400") != NULL) { // 필수헤더 누락
          HttpRespCode = 400;
          HttpNetErrCnt = 0;
//          if(urc_mode == URCMODE_VDIS) 
//            Vdis_TxTerminateFlag = 1;
        }
        else if(strstr(HttpRxStrBuff, "403") != NULL) { // 허용되지 않은 단말기로 부터의 요청
          HttpRespCode = 403;
          HttpNetErrCnt = 0;
          Vdis_NeworkErrorFlag = 1;              
          Vdis_SendTimeCnt = 0;
//          if(urc_mode == URCMODE_VDIS)
//            Vdis_TxTerminateFlag = 1;
        }
        else if(strstr(HttpRxStrBuff, "406") != NULL) { // Content-Type 헤더에 없음
          HttpRespCode = 406;
          HttpNetErrCnt = 0;
          if(urc_mode == URCMODE_VDIS)
            Vdis_TxTerminateFlag = 1;
        }        
        else if(strstr(HttpRxStrBuff, "414") != NULL) { // http 헤더 메세지가 1024 초과
          HttpRespCode = 414;
          HttpNetErrCnt = 0;
//          if(urc_mode == URCMODE_VDIS) 
//            Vdis_TxTerminateFlag = 1;
        }
        else if(strstr(HttpRxStrBuff, "415") != NULL) { // Content-Type 지원하지 않느 타임
          HttpRespCode = 415;
          HttpNetErrCnt = 0;
          if(urc_mode == URCMODE_VDIS) 
            Vdis_TxTerminateFlag = 1;
        }
        else if(strstr(HttpRxStrBuff, "488") != NULL) { // Dtg 데이터가 시간순으로 정렬되지 않음
          HttpRespCode = 488;
          HttpNetErrCnt = 0;
//          if(urc_mode == URCMODE_VDIS) 
//            Vdis_TxTerminateFlag = 1;
        }          
        else if(strstr(HttpRxStrBuff, "493") != NULL) { // Dtg 헤더 규겨, 길이, 자동차 번호 인코딩이 안될때
          HttpRespCode = 493;
          HttpNetErrCnt = 0;
          if(urc_mode == URCMODE_VDIS) 
            Vdis_TxTerminateFlag = 1;
        }
        else if(strstr(HttpRxStrBuff, "500") != NULL) {
          HttpRespCode = 500;
          HttpNetErrCnt++;
          if(HttpNetErrCnt >= VDRPolicy.RetryCnt) {
            HttpNetErrCnt = 0;
            if(urc_mode == URCMODE_VDIS) {            
              Vdis_NeworkErrorFlag = 1;              
              Vdis_SendTimeCnt = 0;
            }
          }
        }
        else if(strstr(HttpRxStrBuff, "503") != NULL) {
          HttpRespCode = 503;
          HttpNetErrCnt++;
          if(HttpNetErrCnt >= VDRPolicy.RetryCnt) {
            HttpNetErrCnt = 0;
            if(urc_mode == URCMODE_VDIS) {
             Vdis_NeworkErrorFlag = 1;              
              Vdis_SendTimeCnt = 0;
            }
          }
        }
        else if(strstr(HttpRxStrBuff, "513") != NULL) { // Dtg 데이터가 시간순으로 정렬되지 않음
          HttpRespCode = 513;
          HttpNetErrCnt = 0;
        }          
        if(HttpRespCode > 0) {
          if(r == '\n') {
            HttpRxStep = HTTP_RX_DATA_WAIT;
            HttpRxCnt = 0;
          }          
        }
      }
      if(r == '\n') 
        HttpRxCnt = 0;
      break;
     
    case HTTP_RX_DATA_WAIT:      //Wait Data
      if(r == '\n') {
        HttpRxLineCnt = 0;
        HttpRxCnt = 0;
        if(HttpRespCode == 200) 
          HttpRxStep = HTTP_RX_DATA_SUCCESS;
        else 
          HttpRxStep = HTTP_RX_DATA_FAIL;
      }
      break;
      
    case HTTP_RX_DATA_SUCCESS:
      if(urc_mode == URCMODE_VDIS) {
        FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr)); 
        ModemStep = MODEM_VDIS_SERVER;
        if(VdisBleServerFlag || (VdisAddrToSend == VdisSentAddr))     // Ble Event면 데이터 전송 중지
          VdisSvrStep = VDIS_SVR_CLOSE; 
        else  {                // 미전송 데이터 기다림
          Modem_SocketClose(0);        
          VdisSvrStep = VDIS_SVR_SEND_WAIT; 
        }
        VdisSvrWaitCount = 0;
      }
      else if(urc_mode == URCMODE_BLE) {
        ModemStep = MODEM_BLE_SERVER;             
        BleSvrStep = BLE_SVR_CLOSE; 
        BleSvrWaitCount = 0;
      }
      else {
        ModemStep = MODEM_CHILD_SERVER;             
        ChildSvrStep = CHILD_SVR_CLOSE; 
        ChildSvrWaitCount = 0;        
      }
      HttpRxStep = HTTP_RX_INIT;
      HttpRxCnt = 0;
      break;  
      
    case HTTP_RX_DATA_FAIL:      //idle 
      HttpRxStep = HTTP_RX_INIT;
      if(urc_mode == URCMODE_VDIS) {
        if(HttpRespCode == 414 || HttpRespCode == 488 || HttpRespCode == 513) {
          FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr)); 
        }
        VdisSvrWaitCount = 0;
        ModemStep = MODEM_VDIS_SERVER;
        VdisSvrStep = VDIS_SVR_FAIL;
      }
      else if (urc_mode == URCMODE_BLE) {
        BleSvrWaitCount = 0;
        ModemStep = MODEM_BLE_SERVER;
        BleSvrStep = BLE_SVR_FAIL;
      }
      else {
        ChildSvrWaitCount = 0;
        ModemStep = MODEM_CHILD_SERVER;
        ChildSvrStep = CHILD_SVR_FAIL;        
      }
      HttpRxStep = HTTP_RX_INIT;
      HttpRxCnt = 0;
      break;
  }
}  


u32 VDR_Length, BLE_Length;
/*
********************************************************************************
* Description : Modem_HTTP_Send
* Arguments   : len, sw
* Return      : resp
* Note        : Modem HTTP Post
******************************************************************************** 
*/
void Modem_HTTP_Post(u8 data_mode)
{
  ModemCommInit();
  if(data_mode == 0) Urc.Mode = URCMODE_VDIS;
  else if(data_mode == 1) Urc.Mode = URCMODE_BLE;
  else Urc.Mode = URCMODE_CHILD;
  
  Urc.RespFlag = 1;
  Urc.Cnt = 0;
  HttpRxStep = HTTP_RX_INIT;
  HttpRxCnt = 0;
  HttpRespCode = 0;
  TxDataStrToModem((u8*)HttpHeaderBuff, HttpHeaderLen);
  TxDataStrToModem((u8*)VdisHeader, VdisHeaderLen);
  TxDataStrToModem(ModemTxBuff, VDR_Length);
}

/*
********************************************************************************
* Description : GetMakeHttpHeader
* Arguments   : none
* Return      : none
* Note        : Make http header
******************************************************************************** 
*/
void Modem_HTTP_Get(void)
{  
  HttpHeaderLen = 0;
  
  memset(HttpHeaderBuff, 0, 512);

  sprintf(HttpHeaderBuff, "GET ");
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "/mobile/cs-alloc");
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], " HTTP/1.1\r\n");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Host: %s:%d\r\n", VdisGetSvr.SvrName, VdisGetSvr.SvrPort);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "User-Agent: %s/%s\r\n", Model_Name, Fota_FW_Ver);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  //sprintf(&HttpHeaderBuff[HttpHeaderLen], "x-oid: %011d\r\n", 1221222520);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "x-oid: %011d\r\n", ModemInfo.CNUM);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Connection: close\r\n");    
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Accept: application/sdp\r\n\r\n");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);

  ModemCommInit();
  Urc.Mode = URCMODE_GET;
  Urc.RespFlag = 1;
  Urc.Cnt = 0;
  HttpRxStep = HTTP_RX_INIT;
  HttpRxCnt = 0;
  HttpRespCode = 0;
  TxDataStrToModem((u8*)HttpHeaderBuff, HttpHeaderLen);
}

/*
********************************************************************************
* Description : MakeHttpPostHeader
* Arguments   : len
* Return      : none
* Note        : Make http header
******************************************************************************** 
*/
void MakeHttpPostHeader(int len, u8 data_mode)
{  
  HttpHeaderLen = 0;
  
  memset(HttpHeaderBuff, 0, 512);

  sprintf(HttpHeaderBuff, "POST ");
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  //sprintf(&HttpHeaderBuff[HttpHeaderLen], "/mobile/vdr-report");
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "/");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], " HTTP/1.1\r\n");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Host: %s:%d\r\n", VdisDataSvr.SvrName, VdisDataSvr.SvrPort);
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "User-Agent: %s/%s\r\n", Model_Name, Fota_FW_Ver);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  //sprintf(&HttpHeaderBuff[HttpHeaderLen], "X-OID: %011d\r\n", 1221222520);  
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "x-oid: %011d\r\n", ModemInfo.CNUM);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Connection: close\r\n");    
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  if(data_mode == 0)
    sprintf(&HttpHeaderBuff[HttpHeaderLen], "BodyType: child/dtg\r\n");    
  else if(data_mode == 1)
    sprintf(&HttpHeaderBuff[HttpHeaderLen], "BodyType: child/event\r\n");    
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Content-Type: application/x-vdrb\r\n");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);  
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Content-Length: %d\r\n\r\n", len);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
}


/*
********************************************************************************
* Description : MakeSDRHeader
* Arguments   : sec_data
* Return      : none
* Note        : Make SDR header process
******************************************************************************** 
*/
void MakeVDRHeader(u8 opcode)
{
  char tmp[20], notype;
  
  ModemStrLen = 0;
  memset(ModemStrBuff, 0, 512);

  memset(tmp, 0, 20);                 // opcode
  if(opcode == 0)
    sprintf(tmp,"70");                // DTG Data
  else if(opcode == 1)
    sprintf(tmp,"01");                // BLE/RFID Event
  else  
    sprintf(tmp,"02");                // BELL Event
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
  ModemStrLen += 2;

  memset(tmp, 0, 20);
  if(opcode == 0)                       
    sprintf(tmp,"%06d",VDR_Length+36);  // body length //DTG Header 크기 포함..
  else 
    sprintf(tmp,"%06d",VDR_Length+2);  // body length
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 6);
  ModemStrLen += 6;

  memset(tmp, 0, 20);
  sprintf(tmp,"%06d",DailyData.TripCount);    // Day Total Sequence
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 6);
  ModemStrLen += 6;

  memset(tmp, 0, 20);               // 차대번호
  memset(tmp, '0', 17);  
  if(Setting.Car_No[0] != 0 && Setting.Car_No[0] != 0x20) {
    notype = StringLen((char*)Setting.Car_No, 17);
    if(notype < 17)    
      memcpy(&tmp[17-notype], Setting.Car_No, notype);
    else memcpy(tmp, Setting.Car_No, 17);      
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 17);
  ModemStrLen += 17;
  
  memset(tmp, 0, 20);
  sprintf(tmp,"17");                // 자동차 유형, 어린이 안심통학은 고정...특수여객 자동차..
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
  ModemStrLen += 2;
  
  memset(tmp, 0, 20);  
  memset(tmp, '#', 12);             // 자동차 등록 번호
  if(VRN[0] != 0 && VRN[0] != 0x20) {
    notype = StringLen((char*)VRN, 12);
    if(notype < 12)    
      memcpy(&tmp[12-notype], VRN, notype);
    else memcpy(tmp, VRN, 12);      
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 12);
  ModemStrLen+=12;
  
  memset(tmp, 0, 20);             // 운송업자 등록번호
  if(Setting.Company_Reg_No[0] == 0 || Setting.Company_Reg_No[0] == 0x20) 
    sprintf(tmp,"0000000000");
  else memcpy(tmp, Setting.Company_Reg_No,10);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 10);
  ModemStrLen += 10;  

  memset(tmp, '#', 20);             // 운전자 코드
  if(Setting.Drv_Code[0] != 0 && Setting.Drv_Code[0] != 0x20) {
    notype = StringLen((char*)Setting.Drv_Code, 18);
    if(notype < 18)    
      memcpy(&tmp[18-notype], Setting.Drv_Code, notype);
    else memcpy(tmp, Setting.Drv_Code, 18);
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 18);
  ModemStrLen += 18;

  memcpy(VdisHeader, (u8*)ModemStrBuff, ModemStrLen);
  VdisHeaderLen = ModemStrLen;
}


/*
********************************************************************************
* Description : MakeDTGHeader
* Arguments   : none
* Return      : none
* Note        : Make DTG header process
******************************************************************************** 
*/
void MakeDTGHeader(unsigned char keyon_flag, unsigned char keyoff_flag)
{
  rtc_time time;
  char tmp[20], notype;
  
  ModemStrLen = 0;
  memset(ModemStrBuff, 0, 512);

  memset(tmp, '#', 20);             // 장치 모델명
  if(Model_Name[0] != 0 && Model_Name[0] != 0x20) {
    notype = StringLen((char*)Model_Name, 20);    
    if(notype < 20)  
      memcpy(&tmp[20-notype], Model_Name, notype);
    else 
      memcpy(tmp, Model_Name, 20);
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 20);
  ModemStrLen += 20;
    
  time = gmtime_conv(TripStartTime);
  memset(tmp, 0, 20);
  sprintf(tmp,"%02u%02u%02u%02u%02u%02u%02u", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, 0);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 14);
  ModemStrLen += 14;  

  memset(tmp, 0, 20);   // 운행정보 코드 
  sprintf(tmp,"--");
  if(keyon_flag)    tmp[0] = 'S';
  if(keyoff_flag)    tmp[1] = 'E';
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
  ModemStrLen += 2;

  memcpy((u8*)&VdisHeader[VdisHeaderLen], (u8*)ModemStrBuff, ModemStrLen);
  VdisHeaderLen += ModemStrLen;
}

/*
********************************************************************************
* Description : MakeFactorList
* Arguments   : sec_data
* Return      : u8
* Note        : Make dtg body process
******************************************************************************** 
*/
u8 MakeDTGBodyList(tsSecData sec_data)
{
  rtc_time time;
  char tmp[20];
  u16 val;
  u32 lval;
  u8 scode;
  
  memset(ModemStrBuff, 0, 512);
  ModemStrLen = 0;
  
  memset(tmp, 0, 20);
  lval = (u32)sec_data.DTrip;   // Km
  if(lval > 9999)
    lval = 9999;
  sprintf(tmp,"%04u", lval);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 4);
  ModemStrLen += 4;

  memset(tmp, 0, 20);
  lval = (u32)sec_data.TTrip;  //Km
  if(lval > 9999999)
    lval = 9999999;
  sprintf(tmp,"%07u", lval);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 7);
  ModemStrLen += 7;

  time = gmtime_conv(sec_data.Time);
  sprintf(tmp,"%02u%02u%02u%02u%02u%02u%02u", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, 0);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 14);
  ModemStrLen += 14;

  memset(tmp, 0, 20);
  sprintf(tmp,"%03u",sec_data.Speed[0]);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 3);
  ModemStrLen += 3;

  memset(tmp, 0, 20);
  lval = (u32)sec_data.RPM;
  if(lval > 9999)
    lval = 9999;
  sprintf(tmp,"%04u",sec_data.RPM);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 4);
  ModemStrLen += 4;

  if(sec_data.Signal.Brake)
    ModemStrBuff[ModemStrLen] = '1';
  else
    ModemStrBuff[ModemStrLen] = '0';
  ModemStrLen++;

  memset(tmp, 0, 20);
  if(sec_data.Lon > 100000000 && sec_data.Lon < 180000000)
    sprintf(tmp,"%09u",sec_data.Lon);
  else
    sprintf(tmp,"000000000");
  
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 9);
  ModemStrLen += 9;

  memset(tmp, 0, 20);
  if(sec_data.Lat > 10000000 && sec_data.Lat < 90000000)
    sprintf(tmp,"%09u",sec_data.Lat);
  else 
    sprintf(tmp,"000000000");
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 9);
  ModemStrLen += 9;

  memset(tmp, 0, 20);
  sprintf(tmp,"%03u",sec_data.Azi);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 3);
  ModemStrLen += 3;

  if(sec_data.AccX >= 0) {
    ModemStrBuff[ModemStrLen] = '+';
    ModemStrLen++;
    val = sec_data.AccX;
  }
  else {
    ModemStrBuff[ModemStrLen] = '-';
    ModemStrLen++;
    val = -1*sec_data.AccX;
  }
  memset(tmp, 0, 20);
  sprintf(tmp,"%03u.%01u",val/10,val%10);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 5);
  ModemStrLen += 5;

  if(sec_data.AccY >= 0) {
    ModemStrBuff[ModemStrLen] = '+';
    ModemStrLen++;
    val = sec_data.AccY;
  }
  else {
    ModemStrBuff[ModemStrLen] = '-';
    ModemStrLen++;
    val = -1*sec_data.AccY;
  }
  memset(tmp, 0, 20);
  sprintf(tmp,"%03u.%01u",val/10,val%10);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 5);
  ModemStrLen += 5;
    
  //device status  
  if(sec_data.Status.GPS_Fault)         scode = 11;  // gps sensor
  else if(sec_data.Status.Speed_Fault)  scode = 12;  // speed sensor
  else if(sec_data.Status.RPM_Fault)    scode = 13;  // rpm sensor
  else if(sec_data.Status.Brake_Fault)  scode = 14;  // brake sensor
  else if(sec_data.Status.Input_Fault)  scode = 21;  // sensor input part
  else if(sec_data.Status.Output_Fault) scode = 22;  // sensor output part
  else if(sec_data.Status.Save_Fault)   scode = 31;  // data oputput part
  else if(sec_data.Status.Comm_Fault)   scode = 32;  // modem 
  else if(sec_data.Status.Dist_Fault)   scode = 41;  // ODO 
  else if(sec_data.Status.Power_Fault)  scode = 99;  // Battery
  else scode = 0;
  memset(tmp, 0, 20);  
  sprintf(tmp,"%02u", scode);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
  ModemStrLen += 2;
  
  if((VDR_Length + ModemStrLen) < MAX_MODEM_TX_BUFF-512) {
    memcpy((u8*)&ModemTxBuff[VDR_Length], (u8*)ModemStrBuff, ModemStrLen);  
    VDR_Length += ModemStrLen;
    return 1;
  }
  else return 2;
}

/*
********************************************************************************
* Description : MakeBLEBodyList
* Arguments   : sec_data
* Return      : u8
* Note        : Make BLE body process
******************************************************************************** 
*/
//u8 MakeBLEBodyList(tsBLE_Body data)
//{
//  char tmp[20];
//  rtc_time time;
//  
//  memset(ModemStrBuff, 0, 512);
//  ModemStrLen = 0;
//    
//  memset(tmp, 0, 20);
//  time = gmtime_conv(data.Time);
//  sprintf(tmp,"%04u%02u%02u%02u%02u%02u", (2000 + time.tm_year), time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 14);
//  ModemStrLen += 14;
//  
// if(data.CurTake > 1)  
//   data.CurTake = 1;
//  ModemStrBuff[ModemStrLen] = (data.CurTake + 0x30);
//  ModemStrLen++;
//
//  memset(tmp, 0, 20);
//  sprintf(tmp,"%02X%02X%02X%02X%02X%02X", data.ScanData.addr[5], data.ScanData.addr[4], data.ScanData.addr[3], data.ScanData.addr[2], data.ScanData.addr[1], data.ScanData.addr[0]);  
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 12);
//  ModemStrLen += 12;
//  
//  memset(tmp, 0, 20);
//  sprintf(tmp,"%03d", data.ScanData.rssi);
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 3);
//  ModemStrLen += 3;
//  
//  if((VDR_Length + ModemStrLen) < MAX_MODEM_TX_BUFF-512) {
//    memcpy((u8*)&ModemTxBuff[VDR_Length], (u8*)ModemStrBuff, ModemStrLen);  
//    VDR_Length += ModemStrLen;
//    return 1;
//  }
//  else return 2;
//}

/*
********************************************************************************
* Description : MakeRFCARDBodyList
* Arguments   : sec_data
* Return      : u8
* Note        : Make Rf Id Card body process
******************************************************************************** 
*/
//u8 MakeRFCARDBodyList(tsRFID_Data data)
//{
//  char tmp[20];
//  rtc_time time;
//  
//  memset(ModemStrBuff, 0, 512);
//  ModemStrLen = 0;
//    
//  memset(tmp, 0, 20);
//  time = gmtime_conv(data.Time);
//  sprintf(tmp,"%04u%02u%02u%02u%02u%02u", (2000 + time.tm_year), time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 14);
//  ModemStrLen += 14;
//  
// if(data.CurTake > 1)  
//   data.CurTake = 1;
//  ModemStrBuff[ModemStrLen] = (data.CurTake + 0x30);
//  ModemStrLen++;
//
//  memset(tmp, 0, 20);
//  sprintf(tmp,"##");
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
//  ModemStrLen += 2;
//    
//  memcpy(&ModemStrBuff[ModemStrLen], data.ID, 10);
//  ModemStrLen += 10;
//
//  memset(tmp, 0, 20);
//  sprintf(tmp,"000");
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 3);
//  ModemStrLen += 3;
//  
//  if((VDR_Length + ModemStrLen) < MAX_MODEM_TX_BUFF-512) {
//    memcpy((u8*)&ModemTxBuff[VDR_Length], (u8*)ModemStrBuff, ModemStrLen);  
//    VDR_Length += ModemStrLen;
//    return 1;
//  }
//  else return 2;
//}

/*
********************************************************************************
* Description : MakeVDRHeader_VdisDtg
* Arguments   : len, flag, keyoff
* Return      : none
* Note        : Make VDR Header process
******************************************************************************** 
*/
void MakeVDRHeader_VdisDtg(unsigned int vdr_len, unsigned char keyon_flag, unsigned char keyoff_flag)
{
  rtc_time time;
  char tmp[20], notype;
  u16 be_val;

  ModemStrLen = 0;
  memset(ModemStrBuff, 0, 512);

  memset(tmp, '#', 20);             // 장치 모델명
  if(Model_Name[0] != 0 && Model_Name[0] != 0x20) {
    notype = StringLen((char*)Model_Name, 20);    
    if(notype < 20)  
      memcpy(&tmp[20-notype], Model_Name, notype);
    else 
      memcpy(tmp, Model_Name, 20);
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 20);
  ModemStrLen += 20;

  memset(tmp, 0, 20);               // 차대번호
  memset(tmp, '0', 17);  
  if(Setting.Car_No[0] != 0 && Setting.Car_No[0] != 0x20) {
    notype = StringLen((char*)Setting.Car_No, 17);
    if(notype < 17)    
      memcpy(&tmp[17-notype], Setting.Car_No, notype);
    else memcpy(tmp, Setting.Car_No, 17);      
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 17);
  ModemStrLen += 17;
  
  memset(tmp, 0, 20);
  sprintf(tmp,"%2u",Car_Type_Code[Setting.Car_Reg_No[0]]);                // 자동차 유형
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
  ModemStrLen += 2;
  
  memset(tmp, 0, 20);  
  memset(tmp, '#', 12);             // 자동차 등록 번호
  if(VRN[0] != 0 && VRN[0] != 0x20) {
    notype = StringLen((char*)VRN, 12);
    if(notype < 12)    
      memcpy(&tmp[12-notype], VRN, notype);
    else memcpy(tmp, VRN, 12);      
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 12);
  ModemStrLen+=12;
  
  memset(tmp, '0', 20);             // 운송업자 등록번호
  if(Setting.Company_Reg_No[0] == 0 || Setting.Company_Reg_No[0] == 0x20) 
    sprintf(tmp,"0000000000");
  else memcpy(tmp, Setting.Company_Reg_No,10);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 10);
  ModemStrLen += 10;  

  memset(tmp, '#', 20);             // 운전자 코드
  if(Setting.Drv_Code[0] != 0 && Setting.Drv_Code[0] != 0x20) {
    notype = StringLen((char*)Setting.Drv_Code, 18);
    if(notype < 18)    
      memcpy(&tmp[18-notype], Setting.Drv_Code, notype);
    else memcpy(tmp, Setting.Drv_Code, 18);
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 18);
  ModemStrLen += 18;

  time = gmtime_conv(TripStartTime);
  memset(tmp, 0, 20);
  sprintf(tmp,"%02u%02u%02u%02u%02u%02u%02u", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, 0);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 14);
  ModemStrLen += 14;  

  memset(tmp, 0, 20);   // 운행정보 코드 
  sprintf(tmp,"--");
  if(keyon_flag)    tmp[0] = 'S';
  if(keyoff_flag)    tmp[1] = 'E';
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 3);
  ModemStrLen += 3;

  ModemStrBuff[ModemStrLen] = 0x01;  // Record 종류  1(0x01) VDR 만 포함 2(0x02) VDR VER 모두
  ModemStrLen  += 1;
  
  ModemStrBuff[ModemStrLen] = 0x00;  // VDR속성  0x00 고정값
  ModemStrLen  += 1;

  ModemStrBuff[ModemStrLen] = 0x00;  // VDR속성  Reserved
  ModemStrLen  += 1;
  
  be_val = SwapEndian16(vdr_len);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);  // VDR 데이터 길이
  ModemStrLen  += 2;

  memcpy(VdisHeader, (u8*)ModemStrBuff, ModemStrLen);
  VdisHeaderLen = ModemStrLen;
}

/*
********************************************************************************
* Description : MakeVDRData_VdisDtg
* Arguments   : sec_data
* Return      : u8
* Note        : Make VDR Data process
******************************************************************************** 
*/
u32 Pre_lon, Pre_lat;
u16 Pre_azi;
u8 MakeVDRData_VdisDtg(tsSecData sec_data)
{
  rtc_time time;
  char tmp[20];
  u16 val, be_val;
  s16 sval;
  u32 lval, be_lval;
  u8 scode;

  memset(ModemStrBuff, 0, 512);
  ModemStrLen = 0;

  time = gmtime_conv(sec_data.Time);
  if(time.tm_year < 17) return 0;
  sprintf(ModemStrBuff,"20%02u%02u%02u%02u%02u%02u00", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
  ModemStrLen = 17;  // 16char + 0x00

  lval = (u32)(sec_data.TTrip);
  be_lval = SwapEndian32(lval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;
  
  val = (u16)((sec_data.TTrip-lval)*1000);
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  val = (u16)(sec_data.DTrip);
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;
  
  val = sec_data.Speed[0];
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;
  
  val = sec_data.RPM;
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;
  
  ModemStrBuff[ModemStrLen] = sec_data.Signal.Brake;
  ModemStrLen++;

  if(sec_data.Lon > 100000000 && sec_data.Lon < 180000000 && !sec_data.Status.GPS_Fault) {
    lval = sec_data.Lon;
    Pre_lon = lval;    
  }
  else {
    lval = Pre_lon;
    sec_data.Status.GPS_Fault = 1;
  }
  be_lval = SwapEndian32(lval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;

  if(sec_data.Lat > 10000000 && sec_data.Lat < 90000000 && !sec_data.Status.GPS_Fault) {
    lval = sec_data.Lat;
    Pre_lat = lval;    
  }
  else {
    lval = Pre_lat;
    sec_data.Status.GPS_Fault = 1;
  }
  be_lval = SwapEndian32(lval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;
  
  if(!sec_data.Status.GPS_Fault) {
    val = sec_data.Azi;
    if(val == 0) val = 360;
    Pre_azi = val;    
  }
  else val = Pre_azi;
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  sval = sec_data.AccX*10;
  be_val = SwapEndian16(sval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  sval = sec_data.AccY*10;
  be_val = SwapEndian16(sval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  if(sec_data.Status.GPS_Fault)         scode = 11;  // gps sensor
  else if(sec_data.Status.Speed_Fault)  scode = 12;  // speed sensor
  else if(sec_data.Status.RPM_Fault)    scode = 13;  // rpm sensor
  else if(sec_data.Status.Brake_Fault)  scode = 14;  // brake sensor
  else if(sec_data.Status.Input_Fault)  scode = 21;  // sensor input part
  else if(sec_data.Status.Output_Fault) scode = 22;  // sensor output part
  else if(sec_data.Status.Save_Fault)   scode = 31;  // data oputput part
  else if(sec_data.Status.Comm_Fault)   scode = 32;  // modem 
  else if(sec_data.Status.Dist_Fault)   scode = 41;  // ODO 
  else if(sec_data.Status.Power_Fault)  scode = 99;  // Battery
  else scode = 0;
  memset(tmp, 0, 20);  
  sprintf(tmp,"%02u", scode);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
  ModemStrLen += 3;  // 2char + 0x00

  val = (u16)(sec_data.DFuel/100);  //ml -->x100ml
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  lval = (u32)(sec_data.TFuel/1000); //ml --> L
  be_lval = SwapEndian32(lval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;

  val = (u16)(sec_data.TFuel)%1000; //ml 
  be_val = SwapEndian16(val);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

#ifdef __VDR_SUPPORT_FUELRATE_  
  if((sec_data.Time == PrevTxSecData.Time+1) && (sec_data.TFuel > PrevTxSecData.TFuel))  {  //순간연료분사량  지원 
    lval = (u32)((sec_data.TFuel - PrevTxSecData.TFuel) * 1000); //ml --> ul
    if(lval > 999999) lval = 0;
  }
  else lval = 0;
  PrevTxSecData = sec_data; 
  be_lval = SwapEndian32(lval);
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;
#else
  be_lval = 0xFFFFFFFF;  //순간연료분사량  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;
#endif  
  be_val = 0xFFFF;  //가속패달  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;
  
  ModemStrBuff[ModemStrLen] = '-';  //변속레버  지원안함  
  ModemStrLen++;
  
  ModemStrBuff[ModemStrLen] = 0xFF;  //변속단  지원안함  
  ModemStrLen++;

  be_lval = 0xFFFFFFFF;  //토크  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;

  be_val = 0xFF7F;  //냉각수 온도  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  be_val = 0xFF7F;  //흡기 온도   지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;
  
  be_lval = 0xFFFFFFFF;  //누적 MAF  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;
  
  be_lval = 0xFFFFFFFF;  //순간MAF  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_lval, 4);
  ModemStrLen += 4;

  be_val = 0xFFFF;  //대기압  지원안함  
  memcpy((u8*)&ModemStrBuff[ModemStrLen], (u8*)&be_val, 2);
  ModemStrLen += 2;

  if((VDR_Length + ModemStrLen) < MAX_MODEM_TX_BUFF-512) {
    memcpy((u8*)&ModemTxBuff[VDR_Length], (u8*)ModemStrBuff, ModemStrLen);  
    VDR_Length += ModemStrLen;
    return 1;
  }
  else return 2;
}

/*
********************************************************************************
* Description : Make VDR Set
* Arguments   : none
* Return      : none
* Note        : Make VDR set process
******************************************************************************** 
*/

u32 VDRSetDataAddr;
u8 MakeVDRSet(void)
{
  u32 dlen = 0, dlen_max, dlen_total = 0;
  u16 i, data_cnt = 0, data_ok_cnt = 0;
  u8 size, resp = 0;
  u16  keyoff_cnt = 0;
  u8 keyon_flag = 0, keyoff_flag = 0;

  //VDRPolicy.DataServerInterval

  if(VDRPolicy.DataInterval == 0) VDRPolicy.DataInterval = 1;
  size = sizeof(tsSecData);
  dlen_max = 180*size*VDRPolicy.DataInterval;
  
  FRAMMultiRead((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));     
  
  if(Prev_VdisFlag) {   // 미전송 데이터가 있어..
    if(Prev_VdisAdrrToSend >= VdisSentAddr)
      dlen_total = Prev_VdisAdrrToSend-VdisSentAddr;
    else
      dlen_total = NAND_SIZE_DATA+Prev_VdisAdrrToSend-VdisSentAddr;
    
    if(dlen_total > DATA_TX_LIMIT) {
      if(Prev_VdisAdrrToSend >= VdisSentAddr) 
        VdisSentAddr = Prev_VdisAdrrToSend - DATA_TX_LIMIT;
      else
        VdisSentAddr = (NAND_SIZE_DATA + Prev_VdisAdrrToSend - DATA_TX_LIMIT)%NAND_SIZE_DATA;
      FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));     
      dlen_total = DATA_TX_LIMIT;
    }
    dlen = dlen_total;
    if(dlen < size)
      Prev_VdisFlag = 0;
  }
  
  if(!Prev_VdisFlag) {   // 미전송 데이터가 없어..
    if(VdisAddrToSend >= VdisSentAddr)
      dlen_total = VdisAddrToSend-VdisSentAddr;
    else
      dlen_total = NAND_SIZE_DATA+VdisAddrToSend-VdisSentAddr;
    
    if(dlen_total > DATA_TX_LIMIT) {
      if(VdisAddrToSend >= VdisSentAddr) 
        VdisSentAddr = VdisAddrToSend - DATA_TX_LIMIT;
      else
        VdisSentAddr = (NAND_SIZE_DATA + VdisAddrToSend - DATA_TX_LIMIT)%NAND_SIZE_DATA;
      FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));     
      dlen_total = DATA_TX_LIMIT;
    }
    dlen = dlen_total;
  }
  
  if(dlen > dlen_max) {
    dlen = dlen_max;
  }
  
  if((VdisSentAddr % NAND_SIZE_DATA)+dlen > NAND_SIZE_DATA) {
    dlen = NAND_SIZE_DATA - (VdisSentAddr % NAND_SIZE_DATA); 
  }
      
  if(dlen < size) {
    VdisSvrStep = VDIS_SVR_CLOSE;
  }
  else {
    VDR_Length = 0;
    
    VDRSetDataAddr = VdisSentAddr;
    data_cnt = dlen/size;
    memset((u8*)&Vdis_Prev_RD_Sec_Data, 0, sizeof(tsSecData));
    keyoff_cnt = 0;
    DTG_Data_Flash_ReadFlag = 1;
    
    for(i = 0; i < data_cnt; i++) {
      Nand_Read_SecData((u8*)&Vdis_RD_Sec_Data, VdisSentAddr, size);
      if(Vdis_RD_Sec_Data.Time > 0x58000000 && Vdis_RD_Sec_Data.Time < 0x80000000) {  //2017년 이후... 
        if(Vdis_RD_Sec_Data.Time > Vdis_Prev_RD_Sec_Data.Time && (VDRPolicy.DataInterval < 2  || (Vdis_RD_Sec_Data.Time%VDRPolicy.DataInterval) == 0)) {
          if(Vdis_RD_Sec_Data.Signal.KeyOn == 0) {
            keyoff_cnt++;
            keyoff_flag = 1;
          }
          else {
            if(Vdis_RD_Sec_Data.Signal.StartTrip || Vdis_RD_Sec_Data.Time == TripStartTime || keyoff_cnt > 0)
              keyon_flag = 1;
            keyoff_cnt = 0;
          }
          if(keyoff_cnt < 2) {
            if(data_ok_cnt > 0 && !IsSameDay(Vdis_Prev_RD_Sec_Data.Time, Vdis_RD_Sec_Data.Time))  {
              Vdis_RD_Sec_Data = Vdis_Prev_RD_Sec_Data;
              break;
            }
            if(Setting.ExtModemVDIS == VDIS_BLE_MODE)
              resp = MakeDTGBodyList(Vdis_RD_Sec_Data);  // DTG Body List
            else if(Setting.ExtModemVDIS == VDIS_DTG_MODE) 
              resp = MakeVDRData_VdisDtg(Vdis_RD_Sec_Data);
            else
              resp = 0;
            if(resp == 1) 
              data_ok_cnt++;
            else if(resp == 2) {
              Vdis_RD_Sec_Data = Vdis_Prev_RD_Sec_Data;
              break;
            }
          }
        }
      }
      VdisSentAddr = (VdisSentAddr+size)%NAND_SIZE_DATA;
      Vdis_Prev_RD_Sec_Data = Vdis_RD_Sec_Data;
    }
    DTG_Data_Flash_ReadFlag = 0;

    if(data_ok_cnt == 1 && keyoff_cnt > 0 && Vdis_Last_Tx_Data.Signal.KeyOn == 0) VDR_Length = 0;
    printk("\r\nMODEM : Tx Data Count : (%d, %d)\r\n",data_cnt, data_ok_cnt);
    if(VDR_Length > 0) {      
      if(Setting.ExtModemVDIS == VDIS_BLE_MODE) {  //VDIS BLE Scan
        MakeVDRHeader(0);                   // Header
        MakeDTGHeader(keyon_flag, keyoff_flag);                    // DTG Header
        MakeHttpPostHeader(VDR_Length+VdisHeaderLen, 0);
      }
      else if(Setting.ExtModemVDIS == VDIS_DTG_MODE) {  //VDIS DTG
        MakeVDRHeader_VdisDtg(VDR_Length, keyon_flag, keyoff_flag);
        MakeHttpPostHeader(VDR_Length+VdisHeaderLen, 2);
      }
      return 1;
    }
    else {
      FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));
      return 2;
    }
  }
  return 0;
}

/*
********************************************************************************
* Description : Make BLE Set
* Arguments   : none
* Return      : none
* Note        : Make VDR set process
******************************************************************************** 
*/

u8 MakeBLESet(void)
{
//  u16 i, ble_ok_cnt = 0;
//  u8 resp = 0;
//  char tmp[20];
//  
//  if((ReceiveMediaFlag == BLESCANNER && TxBLE_Index == 0) || (ReceiveMediaFlag == RFIDCARD && TxRFID_Index == 0)) {
//    BleSvrStep = BLE_SVR_CLOSE;
//  }
//  else {
//    VDR_Length = 0;
//    
//    if(ReceiveMediaFlag == BLESCANNER) {
//      for(i = 0; i < TxBLE_Index; i++) {
//        resp = MakeBLEBodyList(TxBLE_Body[i]);      // BLE Body List
//        if(resp == 1) 
//          ble_ok_cnt++;
//        else if(resp == 2) {
//          break;
//        }
//      }
//    }
//    else {
//      for(i = 0; i < TxRFID_Index; i++) {
//        resp = MakeRFCARDBodyList(TxRFID_Data[i]);  // RFCARD Body List
//        if(resp == 1) 
//          ble_ok_cnt++;
//        else if(resp == 2) {
//          break;
//        }
//      }
//    }
//    printk("\r\nMODEM : Tx Data Count : (%d)\r\n", ble_ok_cnt);
//
//    if(ble_ok_cnt > 0) {      
//      MakeVDRHeader(1);                   // Header
//      memset(tmp, 0, 20);                 // Ble count
//      if(ReceiveMediaFlag == BLESCANNER) {
//        if(ble_ok_cnt > SCAN_MAXNUM)
//          ble_ok_cnt = SCAN_MAXNUM;
//      }
//      else {
//        if(ble_ok_cnt > CONTACT_MAXNUM)
//          ble_ok_cnt = CONTACT_MAXNUM;
//      }
//      sprintf(tmp,"%02u", ble_ok_cnt);
//      memcpy((u8*)&VdisHeader[VdisHeaderLen], tmp, 2); 
//      VdisHeaderLen += 2;
//      MakeHttpPostHeader(VDR_Length+VdisHeaderLen, 1);
//      return 1;
//    }
//    else {
//      return 2;
//    }
//  }
  return 0;
}

/*
********************************************************************************
* Description : Make Child Alarm Set
* Arguments   : none
* Return      : none
* Note        : Make VDR set process
******************************************************************************** 
*/

u8 MakeChildAlarmSet(void)
{
//  int i;
//  u16 ble_ok_cnt = 0;
//  char tmp[20];
//  rtc_time time;
//  
//  VDR_Length = 0;
//  memset(ModemStrBuff, 0, 512);
//  ModemStrLen = 0;
//    
//  memset(tmp, 0, 20);
//  time = gmtime_conv(TxChildAlarm_Data.Time);
//  sprintf(tmp,"%04u%02u%02u%02u%02u%02u", (2000 + time.tm_year), time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
//  memcpy(&ModemStrBuff[ModemStrLen], tmp, 14);
//  ModemStrLen += 14;
//  
// if(TxChildAlarm_Data.Alarm > 1)  
//   TxChildAlarm_Data.Alarm  = 0;
//  ModemStrBuff[ModemStrLen] = (TxChildAlarm_Data.Alarm  + 0x30);
//  ModemStrLen++;
//
//  if(ReceiveMediaFlag == BLESCANNER) {
//    BLE_current_take();   // 현재 승차 인원
//    if(TxBLE_Index) {
//      for(i = 0; i < TxBLE_Index; i++) {
//        memset(tmp, 0, 20);
//        sprintf(tmp,"%02X%02X%02X%02X%02X%02X", TxBLE_Body[i].ScanData.addr[5], TxBLE_Body[i].ScanData.addr[4], TxBLE_Body[i].ScanData.addr[3], TxBLE_Body[i].ScanData.addr[2], TxBLE_Body[i].ScanData.addr[1], TxBLE_Body[i].ScanData.addr[0]);  
//        memcpy(&ModemStrBuff[ModemStrLen], tmp, 12);
//        ModemStrLen += 12;
//        ble_ok_cnt++;
//        if((VDR_Length + ModemStrLen) > MAX_MODEM_TX_BUFF - 512)
//          break;
//      }
//    }
//    else ble_ok_cnt = 0;
//  }
//  else {
//    RFID_current_take();   // 현재 승차 인원    
//    if(TxRFID_Index) {
//      for(i = 0; i < TxRFID_Index; i++) {
//        memset(tmp, 0, 20);
//        sprintf(tmp,"##");
//        memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
//        ModemStrLen += 2;
//        memcpy(&ModemStrBuff[ModemStrLen], TxRFID_Data[i].ID, 10);
//        ModemStrLen += 10;
//        ble_ok_cnt++;
//        if((VDR_Length + ModemStrLen) > MAX_MODEM_TX_BUFF - 512)
//          break;
//      }
//    }
//    else ble_ok_cnt = 0;
//  }
//  memcpy((u8*)&ModemTxBuff[VDR_Length], (u8*)ModemStrBuff, ModemStrLen);  
//  VDR_Length += ModemStrLen;
//  
//  printk("\r\nMODEM : Tx Data Count : (%d)\r\n", ble_ok_cnt);
//
//  MakeVDRHeader(2);                   // Header
//  memset(tmp, 0, 20);                 // CHILD count
//  sprintf(tmp,"%02u", ble_ok_cnt);
//  memcpy((u8*)&VdisHeader[VdisHeaderLen], tmp, 2); 
//  VdisHeaderLen += 2;
//  MakeHttpPostHeader(VDR_Length+VdisHeaderLen, 1);
  
  return 1;
}

/*
********************************************************************************
* Description : Modem_GetServerProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
u8 HttpGetErrorCnt;
void Modem_GetSvrProcess_Vdis(void)
{

  switch(GetSvrStep) {
    case GET_SVR_DATACHECK:
      GetSvrStep = GET_SVR_CONNECT;
      SocketConnectStep = SOCKET_CONNECT_START;
      SocketWaitCnt = 0;
      break;
 
    case GET_SVR_CONNECT:
      Modem_SocketConnect(VdisGetSvr.SvrName,VdisGetSvr.SvrPort,5);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          GetSvrStep = GET_SVR_SEND;
          GetSvrWaitCount = 0;
        }
        else {
          GetSvrStep = GET_SVR_FAIL;
        }
      }
      break;
      
    case GET_SVR_SEND_WAIT: 
      GetSvrWaitCount++;
      if(GetSvrWaitCount >= 5000) {
        GetSvrStep = GET_SVR_CONNECT;
      }
      break;
      
    case GET_SVR_SEND:
      if(TcpState != 1) {
          GetSvrStep = GET_SVR_FAIL;
          return;
      }
      Modem_HTTP_Get();
      GetSvrWaitCount = 0;
      GetSvrStep = GET_SVR_RECIEVE_WAIT;  
      break;
      
    case GET_SVR_RECIEVE_WAIT:
      GetSvrWaitCount++;
      if(GetSvrWaitCount > 30000) {  // 30초 대기
        GetSvrWaitCount = 0;
        GetSvrStep = GET_SVR_FAIL;
      }
      break;

    case GET_SVR_CLOSE:
      GetSvrWaitCount++;
      if(GetSvrWaitCount > 1000) {  //1sec
        Modem_SocketClose(0);
        GetSvrStep = GET_SVR_SUCCESS;
        GetSvrWaitCount = 0;
        HttpGetErrorCnt = 0;
      }
      break;

    case GET_SVR_FAIL:
      Modem_SocketClose(0);
      printk("\r\nMODEM : Get VDIS Server Fail\r\n");
      ModemWaitCnt = 0;
      if(HttpRespCode == 403 || HttpRespCode == 404 || HttpRespCode == 406) {  // 재전송 중지
        ModemWaitCnt = 0;
        GetSvrWaitCount = 0;
        ModemStep = MODEM_DATA_IDLE;
        GetServerFlag = 3; // 재전송 중지
      }
      else {
        GetServerRetryCount++;
        if(GetServerRetryCount >= VDRPolicy.RetryCnt) {
          GetServerRetryCount = 0;
          if(HttpRespCode == 400) { 
            ModemWaitCnt = 0;
            ModemStep = MODEM_DATA_IDLE;
          }
          else {      // 500, 503 재전송
            ModemWaitCnt = 0;
            ModemStep = MODEM_POWER_ON_IDLE;
          }
          GetSvrWaitCount = 0;
          GetServerFlag = 2;  // 5분뒤 다시시도      
          Modem_PPPClose();
        }
        else {
          GetSvrWaitCount = 0;
          ModemWaitCnt = 0;
          ModemStep = MODEM_IDLE;
        }
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case GET_SVR_SUCCESS:
      printk("\r\nMODEM : Get VDIS Server Success\r\n");
      if(!VdisBleServerFlag && !VdisServerFlag)
        Modem_PPPClose();
      ModemWaitCnt = 0;
      ModemStep = MODEM_DATA_IDLE;
      GetServerFlag = 0; ///분산서버 연결 되었을때...
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case GET_SVR_RETRY_WAIT:
      GetSvrWaitCount++;
      break;
    
    case GET_SVR_IDLE:
      ModemWaitCnt = 0; 
      GetServerFlag = 3; // 재전송 중지
      ModemStep = MODEM_DATA_IDLE;
      break;
        
    default : 
      GetSvrStep = GET_SVR_FAIL;
      break;  
  }
}


/*
********************************************************************************
* Description : Modem_DataProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
u8 Vdis_DataTxContinue;
void Modem_VdisSvrProcess(void)
{
  u8 resp = 0;
  
  if(!VdisServerFlag) {
    VdisSvrStep = VDIS_SVR_IDLE;
  }
  
  switch(VdisSvrStep) {
    case VDIS_SVR_DATACHECK:
      FRAMMultiRead((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr)); 
      if(VdisBleServerFlag || (VdisAddrToSend == VdisSentAddr)) { //event  or same addr
        VdisSvrStep = VDIS_SVR_CLOSE;
        VdisSvrWaitCount = 0;
        Vdis_DataTxContinue = 0;
      }
      else {
        resp = MakeVDRSet();
        if(resp == 1) {
          VdisSvrStep = VDIS_SVR_CONNECT;
          if(Vdis_DataTxContinue) {
            SocketConnectStep = SOCKET_STATUS_CHECK;
            Vdis_DataTxContinue = 0;
          }
          else 
            SocketConnectStep = SOCKET_CONNECT_START;
          SocketRetryCnt = 0;
          SocketWaitCnt = 0;
        }
        else if(resp == 2 && (VdisAddrToSend != VdisSentAddr)) 
          VdisSvrStep = VDIS_SVR_SEND_WAIT;
        else
          VdisSvrStep = VDIS_SVR_CLOSE;
      }
      break;
 
    case VDIS_SVR_CONNECT:
      Modem_SocketConnect(VdisDataSvr.SvrName, VdisDataSvr.SvrPort,6);   
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          VdisSvrStep = VDIS_SVR_SEND;
          VdisSvrWaitCount = 0;
        }
        else
          VdisSvrStep = VDIS_SVR_FAIL;
      }
      break;
      
    case VDIS_SVR_SEND_WAIT: 
      VdisSvrWaitCount++;
      if(VdisSvrWaitCount >= 5000) {  // 5초
        VdisSvrStep = VDIS_SVR_DATACHECK;
        Vdis_DataTxContinue = 1;
      }
      break;
      
    case VDIS_SVR_SEND:
      if(TcpState != 1) {
          VdisSvrStep = VDIS_SVR_FAIL;
          return;
      }
      Modem_HTTP_Post(0);
      VdisSvrStep = VDIS_SVR_RECIEVE_WAIT;  
      VdisSvrWaitCount = 0;
      break;
      
    case VDIS_SVR_RECIEVE_WAIT:
      VdisSvrWaitCount++;
      if(VdisSvrWaitCount > 30000) {  //30초
        VdisSvrWaitCount = 0;
        if(VdisServerRetryCount >= VDRPolicy.RetryCnt)
          Vdis_NeworkErrorFlag = 1;
        VdisSvrStep = VDIS_SVR_FAIL;
      }
      break;

    case VDIS_SVR_CLOSE:
      VdisSvrWaitCount++;
      if(VdisSvrWaitCount > 1000) {  //1sec
        Modem_SocketClose(0);
        VdisSvrStep = VDIS_SVR_SUCCESS;
        VdisSvrWaitCount = 0;
        HttpGetErrorCnt = 0;
      }
      break;

    case VDIS_SVR_FAIL:
      Modem_SocketClose(0);
      printk("\r\nMODEM : Data Server Fail\r\n");
      ModemWaitCnt = 0;
      if(Vdis_TxTerminateFlag || Vdis_NeworkErrorFlag || HttpRespCode == 414 || HttpRespCode == 488 || HttpRespCode == 513) {
        VdisSvrWaitCount = 0;
        VdisServerFlag = 0;        
        Modem_PPPClose();
        ModemWaitCnt = 0;
        ModemStep = MODEM_DATA_IDLE;
      }
      else {
        VdisServerRetryCount++;
        if(VdisServerRetryCount >= VDRPolicy.RetryCnt) {
          VdisServerRetryCount = 0;
          if(VdisServerFlag == 2 && !VdisBleServerFlag && !DataServerFlag && !BleServerFlag) {
#ifdef __USE_DMS_SERVER__          
            OffNotiFlag = 1;  //NetKTI Test
            OffNotiRetryCount = 0;
            ModemStep = MODEM_DATA_IDLE;
#else
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
#endif          
          }
          else { 
            if(HttpRespCode == 400) { // 재전송 중지
              ModemWaitCnt = 0;
              ModemStep = MODEM_DATA_IDLE;
            }
            else {      // 500  재전송
              Vdis_SendTimeCnt = 0;
              ModemWaitCnt = 0;
              ModemStep = MODEM_POWER_ON_IDLE;
            }
          }        
          VdisSvrWaitCount = 0;
          VdisServerFlag = 0;        
          Modem_PPPClose();
        }
        else {
          VdisSvrWaitCount = 0;
          ModemWaitCnt = 0;
          ModemStep = MODEM_IDLE;
        }
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case VDIS_SVR_SUCCESS:
      printk("\nMODEM : Data send finished\n");
      if(!VdisBleServerFlag)
        Modem_PPPClose();
      ModemWaitCnt = 0;
      if(VdisServerFlag == 2 && !VdisBleServerFlag && !DataServerFlag && !BleServerFlag) {
#ifdef __USE_DMS_SERVER__          
        OffNotiFlag = 1;  //NetKTI Test
        OffNotiRetryCount = 0;
        ModemStep = MODEM_DATA_IDLE;
#else        
        Modem_PPPClose();
        ModemStep = MODEM_POWER_OFF;
#endif        
      }
      else 
        ModemStep = MODEM_DATA_IDLE;
      VdisServerFlag = 0;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case VDIS_SVR_IDLE:
      ModemWaitCnt = 0;
      ModemStep = MODEM_DATA_IDLE;
      break;
           
    default : 
      VdisSvrStep = VDIS_SVR_FAIL;
      break;  
  }
}

/*
********************************************************************************
* Description : Modem_BleProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
void Modem_BleSvrProcess(void)
{
  u8 resp = 0;

  if(!VdisBleServerFlag) {
    BleSvrStep = BLE_SVR_IDLE;
  }
  
  switch(BleSvrStep) {
    case BLE_SVR_DATACHECK:
      resp = MakeBLESet();
      if(resp == 1) {
        BleSvrStep = BLE_SVR_CONNECT;
        SocketConnectStep = SOCKET_CONNECT_START;
        SocketRetryCnt = 0;
        SocketWaitCnt = 0;
      }
      else if(resp == 2) 
        BleSvrStep = BLE_SVR_SEND_WAIT;
      else
        BleSvrStep = BLE_SVR_CLOSE;
      break;
 
    case BLE_SVR_CONNECT:
      Modem_SocketConnect(VdisDataSvr.SvrName,VdisDataSvr.SvrPort,6);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          BleSvrStep = BLE_SVR_SEND;
        }
        else
          BleSvrStep = BLE_SVR_FAIL;
      }
      BleSvrWaitCount = 0;
      break;
      
    case BLE_SVR_SEND_WAIT: 
      BleSvrWaitCount++;
      if(BleSvrWaitCount >= 5000) {
        BleSvrStep = BLE_SVR_DATACHECK;
      }
      break;
      
    case BLE_SVR_SEND:
      if(TcpState != 1) {
          BleSvrStep = BLE_SVR_FAIL;
          return;
      }
      Modem_HTTP_Post(1);
      BleSvrWaitCount = 0;
      BleSvrStep = BLE_SVR_RECIEVE_WAIT;  
      break;
      
    case BLE_SVR_RECIEVE_WAIT:
      BleSvrWaitCount++;
      if(BleSvrWaitCount > 30000) {  //30초
        BleSvrWaitCount = 0;
        BleSvrStep = BLE_SVR_FAIL;
      }
      break;

    case BLE_SVR_CLOSE:
      BleSvrWaitCount++;
      if(BleSvrWaitCount > 1000) {  //1sec
        Modem_SocketClose(0);
        BleSvrStep = BLE_SVR_SUCCESS;
        BleSvrWaitCount = 0;
        HttpGetErrorCnt = 0;
      }
      break;

    case BLE_SVR_FAIL:
      Modem_SocketClose(0);
      printk("\r\nMODEM : Data Server Fail\r\n");
      ModemWaitCnt = 0;
      VdisBleRetryCount++; // 이건 어디선가..꼭 초기화 해줘야함........bleserverflag 셋시..
      if(VdisBleRetryCount >= VDRPolicy.RetryCnt) {
        VdisBleRetryCount = 0;
        if(VdisBleEndFlag && !VdisServerFlag && !DataServerFlag && !BleServerFlag) {
          Modem_PPPClose();
          ModemStep = MODEM_POWER_OFF;
        }
        else { 
          if(HttpRespCode == 400 || HttpRespCode == 403 || HttpRespCode == 404 || HttpRespCode == 415 || HttpRespCode == 493 || HttpRespCode == 414 || HttpRespCode == 488) { // 재전송 중지
            ModemWaitCnt = 0;
            ModemStep = MODEM_DATA_IDLE;
          }
          else {          // 500, 513 재전송
            ModemWaitCnt = 0;
            ModemStep = MODEM_POWER_ON_IDLE;
          }
        }
        BleSvrWaitCount = 0;
        VdisBleServerFlag = 0;        
        Modem_PPPClose();
      }
      else {
        BleSvrWaitCount = 0;
        ModemStep = MODEM_IDLE;
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case BLE_SVR_SUCCESS:
      printk("\nMODEM : VDIS BLE send finished\n");
      if(!VdisServerFlag)
        Modem_PPPClose();
      ModemWaitCnt = 0;
      if(VdisBleEndFlag && !VdisServerFlag && !DataServerFlag && !BleServerFlag) {
        Modem_PPPClose();
        ModemStep = MODEM_POWER_OFF;
        VdisBleServerFlag = 0;
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
        return;
      }
      ModemStep = MODEM_DATA_IDLE;
      VdisBleServerFlag = 0;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case BLE_SVR_RETRY_WAIT:
      BleSvrWaitCount++;
      break;
        
    case BLE_SVR_IDLE:
      ModemWaitCnt = 0;
      if(VdisBleEndFlag) {
        Modem_PPPClose();
        ModemStep = MODEM_POWER_OFF;        
      }
      else
        ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      BleSvrStep = BLE_SVR_FAIL;
      break;  
  }
}

/*
********************************************************************************
* Description : Modem_ChildAlarmProcess
* Arguments   : none
* Return      : 
* Note        : Child alarm process
******************************************************************************** 
*/
void Modem_ChildAlarmSvrProcess(void)
{
//  u8 resp = 0;
//
//  if(!VdisChildServerFlag) {
//    ChildSvrStep = CHILD_SVR_IDLE;
//  }
//  
//  switch(ChildSvrStep) {
//    case CHILD_SVR_DATACHECK:
//      resp = MakeChildAlarmSet();
//      if(resp == 1) {
//        ChildSvrStep = CHILD_SVR_CONNECT;
//        SocketConnectStep = SOCKET_CONNECT_START;
//        SocketRetryCnt = 0;
//        SocketWaitCnt = 0;
//      }
//      else if(resp == 2) 
//        ChildSvrStep = CHILD_SVR_SEND_WAIT;
//      else
//        ChildSvrStep = CHILD_SVR_CLOSE;
//      break;
// 
//    case CHILD_SVR_CONNECT:
//      Modem_SocketConnect(VdisDataSvr.SvrName,VdisDataSvr.SvrPort,6);
//      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
//        if(SocketConnectStep == SOCKET_CONNECT) {
//          ChildSvrStep = CHILD_SVR_SEND;
//        }
//        else
//          ChildSvrStep = CHILD_SVR_FAIL;
//      }
//      ChildSvrWaitCount = 0;
//      break;
//      
//    case CHILD_SVR_SEND_WAIT: 
//      ChildSvrWaitCount++;
//      if(ChildSvrWaitCount >= 5000) {
//        ChildSvrStep = CHILD_SVR_DATACHECK;
//      }
//      break;
//      
//    case CHILD_SVR_SEND:
//      if(TcpState != 1) {
//          ChildSvrStep = CHILD_SVR_FAIL;
//          return;
//      }
//      Modem_HTTP_Post(2);
//      ChildSvrWaitCount = 0;
//      ChildSvrStep = CHILD_SVR_RECIEVE_WAIT;  
//      break;
//      
//    case CHILD_SVR_RECIEVE_WAIT:
//      ChildSvrWaitCount++;
//      if(ChildSvrWaitCount > 30000) {  //30초
//        ChildSvrWaitCount = 0;
//        ChildSvrStep = CHILD_SVR_FAIL;
//      }
//      break;
//
//    case CHILD_SVR_CLOSE:
//      ChildSvrWaitCount++;
//      if(ChildSvrWaitCount > 1000) {  //1sec
//        Modem_SocketClose(0);
//        ChildSvrStep = CHILD_SVR_SUCCESS;
//        ChildSvrWaitCount = 0;
//        HttpGetErrorCnt = 0;
//      }
//      break;
//
//    case CHILD_SVR_FAIL:
//      Modem_SocketClose(0);
//      printk("\r\nMODEM : Data Server Fail\r\n");
//      ModemWaitCnt = 0;
//      ChildSvrWaitCount++; // 이건 어디선가..꼭 초기화 해줘야함........childserverflag 셋시..
//      if(VdisChildRetryCount >= VDRPolicy.RetryCnt) {
//        VdisChildRetryCount = 0;
//        if(VdisChildEndFlag) {
//          Modem_PPPClose();
//          ModemStep = MODEM_POWER_OFF;
//        }
//        else { 
//          if(HttpRespCode == 400 || HttpRespCode == 403 || HttpRespCode == 404 || HttpRespCode == 415 || HttpRespCode == 493 || HttpRespCode == 414 || HttpRespCode == 488) { // 재전송 중지
//            ModemWaitCnt = 0;
//            ModemStep = MODEM_DATA_IDLE;
//          }
//          else {          // 500, 513 재전송
//            ModemWaitCnt = 0;
//            ModemStep = MODEM_POWER_ON_IDLE;
//          }
//        }
//        ChildSvrWaitCount = 0;
//        VdisChildServerFlag = 0;        
//        Modem_PPPClose();
//      }
//      else {
//        ChildSvrWaitCount = 0;
//        ModemStep = MODEM_IDLE;
//      }
//      Urc.Mode = URCMODE_IDLE;
//      Urc.RespFlag = 0;
//      Urc.Cnt = 0;
//      break;
//
//    case CHILD_SVR_SUCCESS:
//      printk("\nMODEM : VDIS Child send finished\n");
//      if(!VdisServerFlag)
//        Modem_PPPClose();
//      ModemWaitCnt = 0;
//      if(VdisChildEndFlag) {
//        Modem_PPPClose();
//        ModemStep = MODEM_POWER_OFF;
//        VdisChildServerFlag = 0;
//        Urc.Mode = URCMODE_IDLE;
//        Urc.RespFlag = 0;
//        Urc.Cnt = 0;
//        return;
//      }
//      ModemStep = MODEM_DATA_IDLE;
//      VdisChildServerFlag = 0;
//      Urc.Mode = URCMODE_IDLE;
//      Urc.RespFlag = 0;
//      Urc.Cnt = 0;
//      break;
//
//    case CHILD_SVR_RETRY_WAIT:
//      ChildSvrWaitCount++;
//      break;
//        
//    case CHILD_SVR_IDLE:
//      ModemWaitCnt = 0;
//      if(VdisChildEndFlag) {
//        Modem_PPPClose();
//        ModemStep = MODEM_POWER_OFF;        
//      }
//      else
//        ModemStep = MODEM_DATA_IDLE;
//      break;
//      
//    default : 
//      ChildSvrStep = CHILD_SVR_FAIL;
//      break;  
//  }
}

void Modem_SecInt_Vdis(void)
{
  u32 daddr = 0, data_cnt;
  
  if(VDRPolicy.DataServerInterval > 9 && VDRPolicy.DataServerInterval < 10000) {
    Vdis_SendTimeCnt++;  
    if(GetServerFlag == 0) {  ///분산서버 연결 되었을때...
      if(Vdis_NeworkErrorFlag) {          //500,503에러면 
        if(Vdis_SendTimeCnt >= 1800) {       //30분 대기
          if(!VdisServerFlag && !Vdis_TxTerminateFlag && !Power_Off_Flag) {
            GetServerFlag = 1;  ///분산서버 다시연결..
            Vdis_NeworkErrorFlag = 0;
//            VdisServerFlag = 1;
//            VdisAddrToSend = DataSaveAddr;
//            VdisServerRetryCount = 0;
            Vdis_SendTimeCnt = 0;
          }
        }
      }
      else {                            //500,503 에러가 아니면 데이터 전송
        daddr = (u32)(DataSaveAddr-VdisSentAddr);
        data_cnt = daddr/sizeof(tsSecData);
        if((Vdis_SendTimeCnt >= VDRPolicy.DataServerInterval) || (data_cnt >= VDRPolicy.DataServerInterval)) {
          if(!VdisServerFlag && !Vdis_TxTerminateFlag && !Power_Off_Flag) {
            VdisServerFlag = 1;
            VdisAddrToSend = DataSaveAddr;
            VdisServerRetryCount = 0;
            Vdis_SendTimeCnt = 0;
          }
        }
      }
    }
    else if(GetServerFlag == 2){  // 분산서버 연결 Retry 실패시 5분뒤 다시시도 
      Get_SendTimeCnt++;
      if(Get_SendTimeCnt > 300) {
        GetServerFlag = 1;  ///분산서버 연결 시도...
        Get_SendTimeCnt = 0;
      }
    }
  }
}

void Modem_Parameter_Reset_Vdis(void)
{
  FRAMMultiRead((u8 *)&DataSaveAddr, DRV_SAVE_ADDR, sizeof(DataSaveAddr));   
  VdisSentAddr = DataSaveAddr;
  FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));   
  
  memset(VdisGetSvr.SvrName, 0, sizeof(VdisGetSvr.SvrName));
#ifdef __USE_VDIS_TEST_SERVER__
  sprintf(VdisGetSvr.SvrName, "ks.loop-iot.com");
  //sprintf(VdisGetSvr.SvrName, "183.99.59.169");
  VdisGetSvr.SvrPort = 80;
#else  
  sprintf(VdisGetSvr.SvrName, "vslb.ts2020.kr");
  VdisGetSvr.SvrPort = 80;
#endif  
  FRAMMultiWrite((u8 *)&VdisGetSvr, VDIS_GET_SVR, sizeof(tsSvrInfo));
  
  memset((u8*)&VDRPolicy,0,sizeof(tsVDRPolicy));
  VDRPolicy.DataServerInterval = 300;
  VDRPolicy.DataInterval = 1;
  VDRPolicy.RetryDelay = 10;
  VDRPolicy.RetryCnt = 2;
  VDRPolicy.DiscardCnt = 3;
  VDRPolicy.TermCnt = 3;
  FRAMMultiWrite((u8 *)&VDRPolicy, VDIS_VDR_POLICY, sizeof(tsVDRPolicy));  
}

void Modem_Init_Vdis(void)
{
  u8 modify;
  
  FRAMMultiRead((u8 *)&VDRPolicy, VDIS_VDR_POLICY, sizeof(tsVDRPolicy));  
  modify = 0;

  if(VDRPolicy.DataServerInterval == 0 || VDRPolicy.DataServerInterval > 3600) {
    VDRPolicy.DataServerInterval = 180;
    modify = 1;
  }
  if(VDRPolicy.DataInterval == 0 || VDRPolicy.DataInterval > 180) {
    VDRPolicy.DataInterval = 1;
    modify = 1;
  }
  if(VDRPolicy.RetryDelay == 0 || VDRPolicy.RetryDelay > 10) {
    VDRPolicy.RetryDelay = 1;
    modify = 1;
  }
  if(VDRPolicy.RetryCnt > 2) {
    VDRPolicy.RetryCnt = 2;
    modify = 1;
  }
  if(VDRPolicy.DiscardCnt > 3) {
    VDRPolicy.DiscardCnt = 3;
    modify = 1;
  }
  if(VDRPolicy.TermCnt > 3) {
    VDRPolicy.TermCnt = 3;
    modify = 1;
  }
  if(modify)
    FRAMMultiWrite((u8 *)&VDRPolicy, VDIS_VDR_POLICY, sizeof(tsVDRPolicy));  

  FRAMMultiRead((u8 *)&VdisGetSvr, VDIS_GET_SVR, sizeof(tsSvrInfo));
  if(VdisGetSvr.SvrName[0] < ' ' || VdisGetSvr.SvrName[0] > 'z' || VdisGetSvr.SvrPort == 0 || VdisGetSvr.SvrPort > 20000) {
    memset(VdisGetSvr.SvrName, 0, sizeof(VdisGetSvr.SvrName));
#ifdef __USE_VDIS_TEST_SERVER__
    //sprintf(VdisGetSvr.SvrName, "ks.loop-iot.com");
    sprintf(VdisGetSvr.SvrName, "183.99.59.169");
    VdisGetSvr.SvrPort = 80;
#else  
    sprintf(VdisGetSvr.SvrName, "vslb.ts2020.kr");
    VdisGetSvr.SvrPort = 80;
#endif     
    FRAMMultiWrite((u8 *)&VdisGetSvr, VDIS_GET_SVR, sizeof(tsSvrInfo));
  }
    
#ifndef __USE_VDIS_TEST_SERVER__       
  //if(strstr(VdisGetSvr.SvrName, "ks.loop-iot.com") != NULL) {
  if(strstr(VdisGetSvr.SvrName, "183.99.59.169") != NULL) {  
    memset(VdisGetSvr.SvrName, 0, sizeof(VdisGetSvr.SvrName));
    sprintf(VdisGetSvr.SvrName, "vslb.ts2020.kr");
    VdisGetSvr.SvrPort = 80;
    FRAMMultiWrite((u8 *)&VdisGetSvr, VDIS_GET_SVR, sizeof(tsSvrInfo));
  }
#endif      
  VdisAddrToSend = VdisSentAddr;

  if(DataSaveAddr != VdisSentAddr) {
    Prev_VdisFlag = 1;
    Prev_VdisAdrrToSend = DataSaveAddr; 
  }

//  Setting.ExtModemVDIS = EXT_MODEM_VDIS_MODE;  
  if(Setting.ExtModemVDIS != NOT_USE_VDIS)
    GetServerFlag = 1; ///분산서버 연결 시도...
  GetServerRetryCount = 0;
}

void Modem_DataIdleProcess_Vdis(void)
{
  if(GetServerFlag == 1) {   ///분산서버 연결 시도...
    GetSvrStep = GET_SVR_DATACHECK;
    GetSvrWaitCount = 0;
    ModemStep = MODEM_GET_SERVER;    
  }
  else if(GetServerFlag == 0) {  ///분산서버 연결 되었을때...
    if(VdisChildServerFlag && Setting.ExtModemVDIS == VDIS_BLE_MODE) {
      ChildSvrStep = CHILD_SVR_DATACHECK;
      ChildSvrWaitCount = 0;
      ModemStep = MODEM_CHILD_SERVER;    
    }     
    else if(VdisBleServerFlag && Setting.ExtModemVDIS == VDIS_BLE_MODE) {
      BleSvrStep = BLE_SVR_DATACHECK;
      BleSvrWaitCount = 0;
      ModemStep = MODEM_BLE_SERVER;    
    } 
    else if(VdisServerFlag) {
      VdisSvrStep = VDIS_SVR_DATACHECK;
      VdisSvrWaitCount = 0;
      ModemStep = MODEM_VDIS_SERVER;    
    } 
    else if(Setting.ExtModemVDIS == VDIS_BLE_MODE){
      if(VdisBleEndFlag && ModemWaitCnt > 5000) {
        printk("\r\nMODEM : Time's up \n");
        Modem_PPPClose();
        ModemStep = MODEM_POWER_OFF;
      }
    }  
  }
}




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
#define MODEM_GVN_GLOBALS
#include "includes.h"
#include "modem_GVN.h"
#include "modem_DMS.h"


int GV_TmpNum;
u8 GV_PolicySaveFlag;
u8 GV_TmpFactorEnable[MAX_FACTOR_ID_CNT];
u8 GV_DataContFlag;
tsSecData GV_RD_Sec_Data, GV_Prev_RD_Sec_Data;
tsSecData GV_Last_Tx_Data;

u8 CLI_RxMsg[100];
u32 GV_SendTimeCnt;

void Modem_RxChar_GV(unsigned char r) //interrupt msg procedure
{
  int i;
  u16 addr;
  
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
          DataSvrWaitCount = 0;
          NeworkErrorFlag = 0;
          GV_Discard_Cnt = 0;
          GV_Term_Cnt = 0;
          HttpNetErrCnt = 0;
          if(!Prev_DataFlag) 
            SDR_Policy_State = 0;
          GV_Last_Tx_Data = GV_RD_Sec_Data;          
        }
        else if(strstr(HttpRxStrBuff, "403") != NULL) {
          HttpRespCode = 403;
          GV_Discard_Cnt = 0;
          GV_Term_Cnt = 0;
          HttpNetErrCnt = 0;
          TxTerminateFlag = 1;
          DataServerFlag = 0;
        }
        else if(strstr(HttpRxStrBuff, "404") != NULL) {
          HttpRespCode = 404;
          GV_Discard_Cnt = 0;
          GV_Term_Cnt = 0;
          HttpNetErrCnt = 0;
          TxTerminateFlag = 1;
          DataServerFlag = 0;
        }
        else if(strstr(HttpRxStrBuff, "400") != NULL) {
          HttpRespCode = 400;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "480") != NULL) {
          HttpRespCode = 480;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "481") != NULL) {
          HttpRespCode = 481;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "482") != NULL) {
          HttpRespCode = 482;
          HttpNetErrCnt = 0;
        }
        else if(strstr(HttpRxStrBuff, "500") != NULL) {
          HttpRespCode = 500;
          GV_Discard_Cnt = 0;
          GV_Term_Cnt = 0;
          HttpNetErrCnt++;
          if(HttpNetErrCnt >= SDRPolicy.RetryCnt) {
            HttpNetErrCnt = 0;
            NeworkErrorFlag = 1;
            GV_SendTimeCnt = 0;
          }
        }
        else if(strstr(HttpRxStrBuff, "501") != NULL) {
          HttpRespCode = 501;
          GV_Discard_Cnt = 0;
          GV_Term_Cnt = 0;
          HttpNetErrCnt++;
          if(HttpNetErrCnt >= SDRPolicy.RetryCnt) {
            HttpNetErrCnt = 0;
            NeworkErrorFlag = 1;
            GV_SendTimeCnt = 0;
          }
        }         
        else if(strstr(HttpRxStrBuff, "503") != NULL) {
          HttpRespCode = 503;
          GV_Discard_Cnt = 0;
          GV_Term_Cnt = 0;
          HttpNetErrCnt++;
          if(HttpNetErrCnt >= SDRPolicy.RetryCnt) {
            HttpNetErrCnt = 0;
            NeworkErrorFlag = 1;
            GV_SendTimeCnt = 0;
          }
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
          GV_PolicySaveFlag = 0;
          HttpRxCnt = 0;
          HttpRxOrCnt = 0;
          if(HttpRespCode == 200) {
            CLI_No = 0;
            CLI_Cmd = 0;
            CLI_State = 0;
            HttpRxStep = HTTP_RX_DATA_SUCCESS;
          }
          else 
            HttpRxStep = HTTP_RX_DATA_FAIL;
        }
        HttpRxCnt = 0;
      }
      break;
      
    case HTTP_RX_DATA_SUCCESS:
      if(r == '|') {
        HttpRxCnt = 0;
        HttpRxOrCnt++;
      }
      if(HttpRxLineCnt == 0) {    //CLI     
        if(HttpRxOrCnt == 0) {
          if(HttpRxCnt == 1) {
            GV_TmpNum = 0;
          }
          if(r >= '0' && r <= '9') {
            GV_TmpNum = (GV_TmpNum*10) + (r-'0');
          }
        }
        else if(HttpRxOrCnt == 1) {
          if(HttpRxCnt == 0) {
            CLI_No = GV_TmpNum;
            memset(CLI_RxMsg, 0, 100);
          }
          else if(strstr(HttpRxStrBuff, "0.0.1") != NULL) {
            CLI_Cmd = 1;
          }
          else if(strstr(HttpRxStrBuff, "0.0.2") != NULL) {
            CLI_Cmd = 2;
          }
          else if(strstr(HttpRxStrBuff, "0.0.3") != NULL) {
            CLI_Cmd = 3;
          }
          else if(strstr(HttpRxStrBuff, "0.0.4") != NULL) {
            CLI_Cmd = 4;
          }
          else if(strstr(HttpRxStrBuff, "0.0.5") != NULL) {
            CLI_Cmd = 5;
          }
          else if(strstr(HttpRxStrBuff, "0.0.6") != NULL) {
            CLI_Cmd = 6;
          }
          else if(strstr(HttpRxStrBuff, "0.0.7") != NULL) {
            CLI_Cmd = 7;
          }
          else if(strstr(HttpRxStrBuff, "0.0.8") != NULL) {
            CLI_Cmd = 8;
          }
          else if(strstr(HttpRxStrBuff, "0.0.9") != NULL) {
            CLI_Cmd = 9;
          }
          else if(strstr(HttpRxStrBuff, "0.2.1") != NULL) {
            CLI_Cmd = 21;
          }
          else if(strstr(HttpRxStrBuff, "0.1.0") != NULL) {
            CLI_Cmd = 10;
          }
          else if(strstr(HttpRxStrBuff, "0.1.1") != NULL) {
            CLI_Cmd = 11;
          }
          if(CLI_Cmd > 0)
            CLI_State = 1;
        }
        else if(HttpRxOrCnt == 2) {
          if(r == '\n') {
            if(HttpRxCnt < 100)
              memcpy(CLI_RxMsg, HttpRxStrBuff, HttpRxCnt);
          }
        }
      }
      else if(HttpRxLineCnt == 1) {    // Msg Sequency   
      }
      else if(HttpRxLineCnt == 2) {    // Msg List
      }   
      else if(HttpRxLineCnt == 3) {    // SDR 수집정책  
        if(HttpRxOrCnt < 7) {
          if(HttpRxCnt == 1) {
            GV_TmpNum = 0;
          }
          if(r >= '0' && r <= '9') {
            GV_TmpNum = (GV_TmpNum*10) + (r-'0');
          }
        }
        if(HttpRxCnt == 0) {
          if(HttpRxOrCnt == 1) {
            if(SDRPolicy.PolicyNum != GV_TmpNum) {
              SDRPolicy.PolicyNum = GV_TmpNum;
              GV_PolicySaveFlag = 1;
            }
          }
          else if(HttpRxOrCnt == 2) {
            if(SDRPolicy.TxInterval != GV_TmpNum) {
              SDRPolicy.TxInterval = GV_TmpNum;
              ModemInfo.DataServerInterval = SDRPolicy.TxInterval;
              GV_PolicySaveFlag = 2;
            }
          }
          else if(HttpRxOrCnt == 3) {
            if(SDRPolicy.DataInterval != GV_TmpNum) {
              SDRPolicy.DataInterval = GV_TmpNum;
              if(GV_PolicySaveFlag == 0) GV_PolicySaveFlag = 1;
            }
          }
          else if(HttpRxOrCnt == 4) {
            if(SDRPolicy.RetryDelay != GV_TmpNum) {
              SDRPolicy.RetryDelay = GV_TmpNum;
              if(GV_PolicySaveFlag == 0) GV_PolicySaveFlag = 1;
            }
          }
          else if(HttpRxOrCnt == 5) {
            if(SDRPolicy.RetryCnt != GV_TmpNum) {
              SDRPolicy.RetryCnt = GV_TmpNum;
              if(GV_PolicySaveFlag == 0) GV_PolicySaveFlag = 1;
            }
          }
          else if(HttpRxOrCnt == 6) {
            if(SDRPolicy.DiscardCnt != GV_TmpNum) {
              SDRPolicy.DiscardCnt = GV_TmpNum;
              if(GV_PolicySaveFlag == 0) GV_PolicySaveFlag = 1;
            }
          }
          else if(HttpRxOrCnt == 7) {
            if(SDRPolicy.TermCnt != GV_TmpNum) {
              SDRPolicy.TermCnt = GV_TmpNum;
              if(GV_PolicySaveFlag == 0) GV_PolicySaveFlag = 1;
            }
            memset(GV_TmpFactorEnable, 0, MAX_FACTOR_ID_CNT);
          }
        }
        if(HttpRxOrCnt >= 7 && HttpRxCnt == 4) {
          if(HttpRxStrBuff[0] == 'A' && HttpRxStrBuff[1] == '0') {
            GV_TmpNum = (HttpRxStrBuff[2]-'0')*10+(HttpRxStrBuff[3]-'0');
            if(GV_TmpNum < MAX_FACTOR_ID_CNT)
              GV_TmpFactorEnable[GV_TmpNum] = 1;
          }
        }
      }
      else if(HttpRxLineCnt == 4) {    // Keep Alive 정책  
      }
      else if(HttpRxLineCnt == 5) {   // Reserved  
      }
      if(r == '\n') {
        HttpRxLineCnt++;
        HttpRxCnt = 0;
        HttpRxOrCnt = 0;
      }      
      HttpRxDataCnt++;
      if(HttpRxDataCnt >= HttpRxDataLen) {
        FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
//        Modem_SocketClose(0);
//        vTaskDelay(1000);
        ModemStep = MODEM_DATA_SERVER; 
        DataSvrStep = DATA_SVR_SEND_WAIT;
        HttpRxStep = HTTP_RX_INIT;
        if(GV_TmpFactorEnable[0] == 1) {
          for(i = 0; i < MAX_FACTOR_ID_CNT; i++) {
            if(SDRPolicy.FactorIDEnable[i] != GV_TmpFactorEnable[i]) {
              SDRPolicy.FactorIDEnable[i] = GV_TmpFactorEnable[i];
              if(GV_PolicySaveFlag == 0) GV_PolicySaveFlag = 1;
            }
          }
        }
        if(GV_PolicySaveFlag) {
          FRAMMultiWrite((u8 *)&SDRPolicy, GVN_SDR_POLICY, sizeof(tsSDR_Policy));     
          if(GV_PolicySaveFlag == 2) {
            addr = MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo;
            FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, addr, sizeof(ModemInfo.DataServerInterval));
          }
          SDR_Policy_State = 2;
        }
        if(ModemInfo.ServerChange) {
          ModemInfo.ServerChange = 0;
          addr = MODEM_INFO + (u8 *)&ModemInfo.ServerChange - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.ServerChange, addr, sizeof(ModemInfo.ServerChange));
        }
        if(ModemInfo.ServerConnected != 1) {
          ModemInfo.ServerConnected = 1;
          addr = MODEM_INFO + (u8 *)&ModemInfo.ServerConnected - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.ServerConnected, addr, sizeof(ModemInfo.ServerConnected));
        }
        if(BkURLInfo.BackupFlag) {
          memcpy((u8*)&BkURLInfo,(u8*)&URLInfo, sizeof(tsURLInfo));
          BkURLInfo.BackupFlag = 0;
          FRAMMultiWrite((u8*)&BkURLInfo, GVN_BK_URL_INFO, sizeof(tsURLInfo));  
        }
        DataSvrWaitCount = 0;
      }
      break;  
      
    case HTTP_RX_DATA_FAIL:      //idle 
      if(r == '\n') 
        HttpRxCnt = 0;
      HttpRxDataCnt++;
      if(HttpRxDataCnt >= HttpRxDataLen) {
        HttpRxStep = HTTP_RX_INIT;
        DataSvrWaitCount = 0;
//        Modem_SocketClose(0);
//        vTaskDelay(1000);
        ModemStep = MODEM_DATA_SERVER; 
        DataSvrStep = DATA_SVR_FAIL;
        if(HttpRespCode == 403) {
          DataSentAddr = DataAddrToSend;
          FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
        }
        DataSvrWaitCount = 0;
      }
      break;
  }
}  


u32 SDR_StartAddr;
u32 SDR_Length;

void MakeHttpHeader(int len, u8 http_mode)
{  
  HttpHeaderLen = 0;

#ifdef __USE_LOOP_SERVER_GVN_TEST__    
  memset(HttpHeaderBuff, 0, 512);
  sprintf(HttpHeaderBuff, "POST /TFM_CT/LOOP000001/vid-rp HTTP/1.1\r\n");
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Host: 211.235.246.5:3000\r\n");    
#else  
  memset(HttpHeaderBuff, 0, 512);
  sprintf(HttpHeaderBuff, "POST ");
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  if(URLInfo.SubDir[0] != 0) {
    sprintf(&HttpHeaderBuff[HttpHeaderLen], "/%s", URLInfo.SubDir);
    HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  }
  if(URLInfo.CompanyID[0] != 0) {
    sprintf(&HttpHeaderBuff[HttpHeaderLen], "/%s", URLInfo.CompanyID);
    HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  }
  if(URLInfo.DataDir[0] != 0) {
    sprintf(&HttpHeaderBuff[HttpHeaderLen], "/%s", URLInfo.DataDir);
    HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  }
  sprintf(&HttpHeaderBuff[HttpHeaderLen], " HTTP/1.1\r\n");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Host: %s:%d\r\n", URLInfo.URLName, URLInfo.Port);  
#endif  
  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Content-Type: text/plain; charset=EUC-KR\r\n");  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);

  sprintf(&HttpHeaderBuff[HttpHeaderLen], "Content-Length: %d\r\n", len);  
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
  
//  if(http_mode)
//    sprintf(&HttpHeaderBuff[HttpHeaderLen], "Connection: keep-alive\r\n\r\n");    
////    sprintf(&HttpHeaderBuff[HttpHeaderLen], "\r\n");    
//  else 
    sprintf(&HttpHeaderBuff[HttpHeaderLen], "Connection: close\r\n\r\n");    
    
  HttpHeaderLen = StringLen(HttpHeaderBuff,512);
}

char TripTime[12];
void MakeSDRHeader(tsSecData sec_data)
{
  int len, i;//, idx;
  rtc_time time;
  char tmp[20];
  
  ModemStrLen = 0;
  memset(ModemStrBuff, 0, 512);

//  if(Prev_DataFlag || SDR_Policy_State == 1) {
  if(!Prev_DataFlag && SDR_Policy_State == 1) {
    memset(tmp, 0, 20);
    sprintf(tmp,"%011d",ModemInfo.CNUM);
    len = StringLen(tmp,20);
    memcpy(ModemStrBuff, tmp, len);
    ModemStrLen+=len;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  if(!Prev_DataFlag && SDR_Policy_State == 1) {
    memset(tmp, 0, 20);
    len = StringLen((char*)Model_Name_KT,15);
    memcpy(tmp, Model_Name_KT,len);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
  
  if(!Prev_DataFlag && SDR_Policy_State == 1) {
    memset(tmp, 0, 20);
    memcpy(tmp, Fota_FW_Ver,10);
    len = StringLen(tmp,10);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  if((!Prev_DataFlag && SDR_Policy_State == 1) || SDRPolicy.PolicyNum > 0) {
    memset(tmp, 0, 20);
    if(Setting.Company_Reg_No[0] == 0 || Setting.Company_Reg_No[0] == 0x20) 
      sprintf(tmp,"0000000000");
    else
      memcpy(tmp, Setting.Company_Reg_No, sizeof(Setting.Company_Reg_No));
    len = StringLen(tmp,13);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  memset(tmp, 0, 20);
  len = 0;
  for(i = 0; i < 17; i++) {
    if((Setting.Car_No[i] >= '0' && Setting.Car_No[i] <= '9') || (Setting.Car_No[i] >= 'A' && Setting.Car_No[i] <= 'Z')) {
      tmp[len++] = Setting.Car_No[i];
    }
  }
  if(len == 0) { 
    sprintf(tmp,"00000000000000000");
    len = 17;
  }
  memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
  ModemStrLen+=len;
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
  
  if(!Prev_DataFlag && SDR_Policy_State == 1) {
    memset(tmp, 0, 20);
    if(VRN[0] == 0 || VRN[0] == 0x20) 
      sprintf(tmp,"000000000000");
    else {
      memcpy(tmp, VRN, sizeof(VRN));
    }
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  memset(tmp, 0, 20);
  if(Setting.Drv_Code[0] == 0 || Setting.Drv_Code[0] == 0x20) 
    sprintf(tmp,"000000000010000000");
  else
    memcpy(tmp, Setting.Drv_Code, sizeof(Setting.Drv_Code));
  len = StringLen(tmp,20);
  memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
  ModemStrLen+=len;
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  if(!Prev_DataFlag && SDR_Policy_State == 1) {
    time = gmtime_conv(sec_data.Time);
    memset(tmp, 0, 20);
    sprintf(tmp,"%02u%02u%02u%02u%02u%02u", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, 12);
    memcpy(TripTime, tmp, 12);
    ModemStrLen+=12;
  }
//  else {
//    memcpy(&ModemStrBuff[ModemStrLen], TripTime, 12);
//    ModemStrLen+=12;
//  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
 
  if(CLI_State == 1) {
    memset(tmp, 0, 20);
    sprintf(tmp,"%u|P| ", CLI_No);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  else if(CLI_State == 2) {
    memset(tmp, 0, 20);
    sprintf(tmp,"%u|S|OK!", CLI_No);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  else if(CLI_State == 3) {
    memset(tmp, 0, 20);
    sprintf(tmp,"%u|F|ERROR!", CLI_No);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
  
  if(!Prev_DataFlag) {
    if(SDR_Policy_State == 1) { // 시동 On
      if(SDRPolicy.PolicyNum == 0) {
        ModemStrBuff[ModemStrLen] = 'N';
        ModemStrLen++;
      }
      else {
        memset(tmp, 0, 20);
        sprintf(tmp,"%u", SDRPolicy.PolicyNum);
        len = StringLen(tmp,4);
        memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
        ModemStrLen+=len;
      }
    }
    else if(SDR_Policy_State == 2) {
      ModemStrBuff[ModemStrLen] = 'S';
      ModemStrLen++;
    }
    else if(SDR_Policy_State == 3) {
      ModemStrBuff[ModemStrLen] = 'F';
      ModemStrLen++;
    }
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
  
  if(!Prev_DataFlag && SDR_Policy_State == 1) {
    ModemStrBuff[ModemStrLen] = 'U';  ModemStrLen++;
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
  
  if(SDRPolicy.FactorIDEnable[0] == 0) SDRPolicy.FactorIDEnable[0] = 1;   //필수 항목
  if(SDRPolicy.FactorIDEnable[1] == 0) SDRPolicy.FactorIDEnable[1] = 1;   //필수 항목
  if(SDRPolicy.FactorIDEnable[2] == 0) SDRPolicy.FactorIDEnable[2] = 1;   //필수 항목
  if(SDRPolicy.FactorIDEnable[20] == 0) SDRPolicy.FactorIDEnable[20] = 1; //필수 항목
  
  sprintf(&ModemStrBuff[ModemStrLen],"A000");
  ModemStrLen += 4;
  for(i = 1; i < MAX_FACTOR_ID_CNT; i++) {
    if(SDRPolicy.FactorIDEnable[i]) {
      sprintf(&ModemStrBuff[ModemStrLen],"|A%03u", i);
      ModemStrLen += 5;
    }
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;

  memcpy(ModemTxBuff, (u8*)ModemStrBuff, ModemStrLen);
  SDR_Length = ModemStrLen;
}

u8 ContDataFlag;
tsSecData PrevTxSecData;
u8 MakeFactorList(tsSecData sec_data)
{
  int len, i;
  rtc_time time;
  char tmp[20];
  u16 val;
  u32 lval;
  double fval;
  s16 sval;

  memset(ModemStrBuff, 0, 512);
  ModemStrLen = 0;
  
  if(sec_data.Time-PrevTxSecData.Time == SDRPolicy.DataInterval) ContDataFlag = 1;
  else ContDataFlag = 0;
  
  time = gmtime_conv(sec_data.Time);
  if(time.tm_year < 17) return 0;
  sprintf(ModemStrBuff,"%02u%02u%02u%02u%02u%02u", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
  ModemStrLen = 12;
  
  if(SDRPolicy.FactorIDEnable[1]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(sec_data.Lon > 100000000 && sec_data.Lon < 180000000) {
      memset(tmp, 0, 20);
      sprintf(tmp,"%u",sec_data.Lon);
      len = StringLen(tmp,20);
      memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
      ModemStrLen+=len;
    }
  }
  if(SDRPolicy.FactorIDEnable[2]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(sec_data.Lat > 10000000 && sec_data.Lat < 90000000) {
      memset(tmp, 0, 20);
      sprintf(tmp,"%u",sec_data.Lat);
      len = StringLen(tmp,20);
      memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
      ModemStrLen+=len;
    }
  }
  if(SDRPolicy.FactorIDEnable[3]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    lval = (u32)(sec_data.DTrip*1000); //m
    sprintf(tmp,"%u.%03u",lval/1000,(lval%1000));
//    sprintf(tmp,"%08.3f",sec_data.Trip);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[4]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    lval = (u32)(sec_data.TTrip*1000);  //m
    sprintf(tmp,"%u.%03u",lval/1000,(lval%1000));
//    sprintf(tmp,"%011.3lf",sec_data.TTrip);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[5]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    sprintf(tmp,"%u",sec_data.Speed[0]);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[6]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    sprintf(tmp,"%u",sec_data.RPM);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[7]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(sec_data.Signal.Brake)
      ModemStrBuff[ModemStrLen] = '1';
    else
      ModemStrBuff[ModemStrLen] = '0';
    ModemStrLen++;
  }
  if(SDRPolicy.FactorIDEnable[8]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    sprintf(tmp,"%u",sec_data.Azi);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[9]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
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
    sprintf(tmp,"%u.%01u",val/10,val%10);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[10]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
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
    sprintf(tmp,"%u.%01u",val/10,val%10);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[11]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    lval = (u32)(sec_data.DFuel); //ml
    sprintf(tmp,"%u.%03u",lval/1000,(lval%1000));
//    sprintf(tmp,"%07.3f",sec_data.Fuel);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[12]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    lval = (u32)(sec_data.TFuel);  //ml
    sprintf(tmp,"%u.%03u",lval/1000,(lval%1000));
//    sprintf(tmp,"%010.3lf",sec_data.TFuel);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  if(SDRPolicy.FactorIDEnable[13]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    fval = 0.0;
//    if(ContDataFlag && sec_data.TFuel > 0) {   //순간 연비
//      if(sec_data.TFuel > PrevTxSecData.TFuel) {
//        fval = ((sec_data.TTrip - PrevTxSecData.TTrip)/(sec_data.TFuel - PrevTxSecData.TFuel))*1000.0;  //km/l
//        if(fval > 99.9)  fval = 99.9;
//      }
//    }
    if(sec_data.Fuel > 0) {   // Trip 연비
      fval = (sec_data.Trip/sec_data.Fuel)*1000.0;  //km/l
      if(fval > 99.9)  fval = 99.9;
    }
    val = (u16)(fval*10.0);  // km/l    
    sprintf(tmp,"%u.%01u",val/10,(val%10));
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }
  for(i = 14; i < 20; i++) {
    if(SDRPolicy.FactorIDEnable[i]) {
      ModemStrBuff[ModemStrLen] = '|';
      ModemStrLen++;
      ModemStrBuff[ModemStrLen] = '0';
      ModemStrLen++;
    }
  }
  if(SDRPolicy.FactorIDEnable[20]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(sec_data.Signal.KeyOn)
      ModemStrBuff[ModemStrLen] = '1';
    else
      ModemStrBuff[ModemStrLen] = '0';
    ModemStrLen++;
  }     
  if(SDRPolicy.FactorIDEnable[21]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(sec_data.BatVolt > 999)
      val = 999;
    else
      val = sec_data.BatVolt;
    memset(tmp, 0, 20);
    sprintf(tmp,"%u.%01u",val/10,(val%10));
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }     
  for(i = 22; i < 28; i++) {
    if(SDRPolicy.FactorIDEnable[i]) {
      ModemStrBuff[ModemStrLen] = '|';
      ModemStrLen++;
      ModemStrBuff[ModemStrLen] = '0';
      ModemStrLen++;
    }
  }
  if(SDRPolicy.FactorIDEnable[28]){
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(SecData.Signal.Temp1) {
      if(sec_data.Temp1 > 999)
        sval = 999;
      else if(sec_data.Temp1 < -999)
        sval = -999;
      else
        sval = sec_data.Temp1;
      memset(tmp, 0, 20);
      sprintf(tmp,"%+.1f",(double)sval/10.0);
      len = StringLen(tmp,20);
      memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
      ModemStrLen+=len;    
    }
    else {
      ModemStrBuff[ModemStrLen] = '0';
      ModemStrLen++;
    }
  }     
  if(SDRPolicy.FactorIDEnable[29]){
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(SecData.Signal.Temp2) {
      if(sec_data.Temp2 > 999)
        sval = 999;
      else if(sec_data.Temp2 < -999)
        sval = -999;
      else
        sval = sec_data.Temp2;
      memset(tmp, 0, 20);
      sprintf(tmp,"%+.1f",(double)sval/10.0);
      len = StringLen(tmp,20);
      memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
      ModemStrLen+=len;      
    }
    else {
      ModemStrBuff[ModemStrLen] = '0';
      ModemStrLen++;
    }
  }     
  if(SDRPolicy.FactorIDEnable[30]){
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    ModemStrBuff[ModemStrLen] = '0';
    ModemStrLen++;    
  }
  if(SDRPolicy.FactorIDEnable[31]){
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
  }     
  if(SDRPolicy.FactorIDEnable[32]) {  //RSSI
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    memset(tmp, 0, 20);
    sprintf(tmp,"%u",sec_data.RSSI);
    len = StringLen(tmp,20);
    memcpy(&ModemStrBuff[ModemStrLen], tmp, len);
    ModemStrLen+=len;
  }     
  if(SDRPolicy.FactorIDEnable[33]) {
    ModemStrBuff[ModemStrLen] = '|';
    ModemStrLen++;
    if(sec_data.Status.GPS_Fault) {
      sprintf(tmp,"51");
      memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
      ModemStrLen+=2;
    }
    if(sec_data.Status.Short_Fault) {     //Temp Open
      if(sec_data.Status.GPS_Fault) {
        ModemStrBuff[ModemStrLen] = ',';
        ModemStrLen++;        
      }
      sprintf(tmp,"61");
      memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
      ModemStrLen+=2;
    }
    if(sec_data.Status.Open_Fault) {      //Temp Short
      if(sec_data.Status.GPS_Fault || sec_data.Status.Short_Fault) {      
        ModemStrBuff[ModemStrLen] = ',';
        ModemStrLen++;        
      }
      sprintf(tmp,"71");
      memcpy(&ModemStrBuff[ModemStrLen], tmp, 2);
      ModemStrLen+=2;
    }
  }     
  for(i = 34; i < MAX_FACTOR_ID_CNT; i++) {
    if(SDRPolicy.FactorIDEnable[i]) {
      ModemStrBuff[ModemStrLen] = '|';
      ModemStrLen++;
      ModemStrBuff[ModemStrLen] = '0';
      ModemStrLen++;
    }
  }
  ModemStrBuff[ModemStrLen] = '\r';  ModemStrLen++;
  ModemStrBuff[ModemStrLen] = '\n';  ModemStrLen++;
  
  PrevTxSecData = sec_data; 
  if(SDR_Length+ModemStrLen < MAX_MODEM_TX_BUFF-512) {
    memcpy(&ModemTxBuff[SDR_Length], (u8*)ModemStrBuff, ModemStrLen);  
    SDR_Length += ModemStrLen;  
    return 1;
  }
  else return 2;
}

u32 SDRSetDataAddr;
u8 MakeSDRSet(void)
{
  u32 dlen = 0, dlen_max, dlen_total = 0;
  u16 i, data_cnt = 0, data_ok_cnt = 0;
  u8 size, resp = 0;
  u16  keyoff_cnt = 0;
  
  GV_DataContFlag = 0;
  HttpRxStep = HTTP_RX_INIT;
  HttpRxCnt = 0;
  HttpRespCode = 0;
  
  size = sizeof(tsSecData);
//  if(ModemInfo.DataServerInterval > 180)
    dlen_max = 180*size*SDRPolicy.DataInterval;
//  else
//    dlen_max = ModemInfo.DataServerInterval*size;
  
  if(GV_DiscardFlag) {
    FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
    GV_DiscardFlag = 0;
  }
  else
    FRAMMultiRead((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
  
  printk("\r\nMODEM : Data Save Address : %d Data Sent Address %d\r\n",DataSaveAddr,DataSentAddr); 
  
  if(Prev_DataFlag) {
    if(Prev_DataAdrrToSend >= DataSentAddr)
      dlen_total = Prev_DataAdrrToSend-DataSentAddr;
    else {
      if(DataSaveAddr < DataSentAddr && Prev_DataAdrrToSend < DATA_TX_LIMIT)
        dlen_total = NAND_SIZE_DATA+Prev_DataAdrrToSend-DataSentAddr;
      else    dlen_total = 0;
    }

    printk("\r\nMODEM : Prev Data Address To Send : %d \r\n",Prev_DataAdrrToSend); 
    
    if(dlen_total > DATA_TX_LIMIT) {
      if(Prev_DataAdrrToSend >= DataSentAddr) 
        DataSentAddr = Prev_DataAdrrToSend - DATA_TX_LIMIT;
      else
        DataSentAddr = (NAND_SIZE_DATA + Prev_DataAdrrToSend - DATA_TX_LIMIT)%NAND_SIZE_DATA;
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
      dlen_total = DATA_TX_LIMIT;
    }
    dlen = dlen_total;
    if(dlen < size)
      Prev_DataFlag = 0;
  }
  
  if(!Prev_DataFlag) {
    if(DataAddrToSend >= DataSentAddr)
      dlen_total = DataAddrToSend-DataSentAddr;
    else {
      if(DataSaveAddr < DataSentAddr && DataAddrToSend < DATA_TX_LIMIT)
        dlen_total = NAND_SIZE_DATA+DataAddrToSend-DataSentAddr;
      else    dlen_total = 0;
    }

    printk("\r\nMODEM : Data Address To Send : %d \r\n",DataAddrToSend); 
    
    if(dlen_total > DATA_TX_LIMIT) {
      if(DataAddrToSend >= DataSentAddr) 
        DataSentAddr = DataAddrToSend - DATA_TX_LIMIT;
      else
        DataSentAddr = (NAND_SIZE_DATA + DataAddrToSend - DATA_TX_LIMIT)%NAND_SIZE_DATA;
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
      dlen_total = DATA_TX_LIMIT;
    }
    dlen = dlen_total;
  }
  
  if(dlen > dlen_max) {
    dlen = dlen_max;
    GV_DataContFlag = 1;
  }
  if((DataSentAddr % NAND_SIZE_DATA)+dlen > NAND_SIZE_DATA) {
    dlen = NAND_SIZE_DATA - (DataSentAddr % NAND_SIZE_DATA); 
    GV_DataContFlag = 1;
  }
      
  if(dlen >= size) {
    SDR_Length = 0;
    SDRSetDataAddr = DataSentAddr;
    data_cnt = dlen/size;
    keyoff_cnt = 0;
    memset((u8*)&GV_Prev_RD_Sec_Data, 0, sizeof(tsSecData));
    DTG_Data_Flash_ReadFlag = 1;
    
    for(i = 0; i < data_cnt; i++) {
      Nand_Read_SecData((u8*)&GV_RD_Sec_Data, DataSentAddr, size);
      if(GV_RD_Sec_Data.Time > 0x58000000 && GV_RD_Sec_Data.Time < 0x80000000) {  //2017년 이후... 
        if(GV_RD_Sec_Data.Time > GV_Prev_RD_Sec_Data.Time && (SDRPolicy.DataInterval < 2  || (GV_RD_Sec_Data.Time%SDRPolicy.DataInterval) == 0)) {
          if(GV_RD_Sec_Data.Signal.KeyOn == 0) 
            keyoff_cnt++;
          else
            keyoff_cnt = 0;
          if(keyoff_cnt < 2) {
            if(data_ok_cnt == 0) {
              if(GV_Last_Tx_Data.Time > 0) {
                if(!IsSameDay(GV_Last_Tx_Data.Time, GV_RD_Sec_Data.Time))  SDR_Policy_State = 1;
              }
              MakeSDRHeader(GV_RD_Sec_Data);
            }
            else {
              if(!IsSameDay(GV_Prev_RD_Sec_Data.Time, GV_RD_Sec_Data.Time))  {
                GV_RD_Sec_Data = GV_Prev_RD_Sec_Data;
                SDR_Policy_State = 1;
                break;
              }
            }
            resp = MakeFactorList(GV_RD_Sec_Data);
            if(resp == 1) 
              data_ok_cnt++;
            else if(resp == 2) {
              GV_RD_Sec_Data = GV_Prev_RD_Sec_Data;
              break;
            }
          }
        }
      }
      DataSentAddr = (DataSentAddr+size)%NAND_SIZE_DATA;
      GV_Prev_RD_Sec_Data = GV_RD_Sec_Data;
    }
    if(data_ok_cnt == 1 && keyoff_cnt > 0 && GV_Last_Tx_Data.Signal.KeyOn == 0) SDR_Length = 0;
    
    DTG_Data_Flash_ReadFlag = 0;
    printk("\r\nMODEM : Tx Data Count : (%d, %d)\r\n",data_cnt, data_ok_cnt);

    if(SDR_Length > 0) {      
      MakeHttpHeader(SDR_Length, GV_DataContFlag);
      return 1;
    }
    else {
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
      return 2;
    }
  }
  return 0;
}

void PostSDRSet(void)
{
    ModemCommInit();
    Urc.Mode = URCMODE_GVN;
    Urc.RespFlag = 1;
    Urc.Cnt = 0;
    HttpRxStep = HTTP_RX_INIT;
    HttpRxCnt = 0;
    HttpRespCode = 0;
    TxDataStrToModem((u8*)HttpHeaderBuff, HttpHeaderLen);
//    vTaskDelay(1);
    TxDataStrToModem(ModemTxBuff, SDR_Length);
    DataSvrStep = DATA_SVR_RECIEVE_WAIT;  
}

/*
********************************************************************************
* Description : ServerDomain_Process
* Arguments   : none
* Return      : 
* Note        : Server Domin Process
******************************************************************************** 
*/
u8 ServerDomain_Process_GVN(char *serverDamain, u8 len, u8 mode) 
{
  u8 rc;
  u8 i;
  u8 str_len, svr_name_flag, val_flag;
  u32 val;   
  tsURLInfo tmpURLInfo;
  u16 addr;
  
  if(len > 0 && len < 100) {
    svr_name_flag = 0;
    str_len = 0;
    val = 0;
    val_flag = 0;
    memset((u8*)MdmTmpString, 0, 100);
    memset((u8*)&tmpURLInfo,0, sizeof(tsURLInfo));

    if(ModemInfo.ServerConnected) {
      memset((u8*)&BkURLInfo,0, sizeof(tsURLInfo));
      memcpy((u8*)&BkURLInfo,(u8*)&URLInfo, sizeof(tsURLInfo));
      BkURLInfo.BackupFlag = 1;
      FRAMMultiWrite((u8*)&BkURLInfo, GVN_BK_URL_INFO, sizeof(tsURLInfo));  
      ModemInfo.ServerConnected = 0;
      addr = MODEM_INFO + (u8 *)&ModemInfo.ServerConnected - (u8 *)&ModemInfo;
      FRAMMultiWrite((u8 *)&ModemInfo.ServerConnected, addr, sizeof(ModemInfo.ServerConnected));      
    }
    
    for(i=0; i < len; i++) {
      if(i == 0) {
        if(strstr(serverDamain, "http://") != NULL || strstr(serverDamain, "HTTP://") != NULL)
          i = 7;
      }
      if(svr_name_flag == 0){
        if(serverDamain[i] != '/') {
          if(serverDamain[i] == ':') 
            val_flag = 1;
          else {
            if(val_flag == 0)  
              MdmTmpString[str_len++] = serverDamain[i]; 
            else {
              if(serverDamain[i] >= '0' && serverDamain[i] <= '9')
                val = val*10 + serverDamain[i]-'0';
            }
          }
        }
        else if(str_len < 60){
          memcpy((u8*)tmpURLInfo.URLName, MdmTmpString, 60);
          memset((u8*)MdmTmpString, 0, 100);
          str_len = 0;
          tmpURLInfo.Port = 80;
          if(val_flag == 1) {
            tmpURLInfo.Port = val;
            val = 0;
            val_flag = 0;
          }
          svr_name_flag++;
        }
      }
      else if(svr_name_flag == 1){
        if(serverDamain[i] != '/')
          MdmTmpString[str_len++] = serverDamain[i]; 
        else if(str_len < 20){
          memcpy((u8*)tmpURLInfo.SubDir, MdmTmpString, 20);
          memset((u8*)MdmTmpString, 0, 100);
          str_len = 0;
          svr_name_flag++;
        }
      }
      else if(svr_name_flag == 2){
        if(serverDamain[i] != '/')
          MdmTmpString[str_len++] = serverDamain[i]; 
        else if(str_len < 20){
          memcpy((u8*)tmpURLInfo.CompanyID, MdmTmpString, 20);
          memset((u8*)MdmTmpString, 0, 100);
          str_len = 0;
          svr_name_flag++;
        }
      }
      else if(svr_name_flag == 3){
        if(serverDamain[i] != '/')
          MdmTmpString[str_len++] = serverDamain[i]; 
        if(i == len-1) {
          memcpy((u8*)tmpURLInfo.DataDir, MdmTmpString, 20);
          memset((u8*)MdmTmpString, 0, 100);
          str_len = 0;
          svr_name_flag++;
        }
      }
    }
    if(svr_name_flag == 0) {
      memcpy((u8*)tmpURLInfo.URLName, MdmTmpString, 60);
      memset((u8*)MdmTmpString, 0, 100);
      tmpURLInfo.Port = 80;
      if(val_flag == 1) {
        tmpURLInfo.Port = val;
      }
    }
    else if(svr_name_flag == 1) {
      memcpy((u8*)tmpURLInfo.SubDir, MdmTmpString, 20);
      memset((u8*)MdmTmpString, 0, 100);
    }
    else if(svr_name_flag == 2) {
      memcpy((u8*)tmpURLInfo.CompanyID, MdmTmpString, 20);
      memset((u8*)MdmTmpString, 0, 100);
    }
    else if(svr_name_flag == 3) {
      memcpy((u8*)tmpURLInfo.DataDir, MdmTmpString, 20);
      memset((u8*)MdmTmpString, 0, 100);
    }
    FRAMMultiWrite((u8*)&tmpURLInfo, GVN_URL_INFO, sizeof(tsURLInfo));    
    if(mode == 0) {    //update
      memset((u8*)&URLInfo,0, sizeof(tsURLInfo));
      memcpy((u8*)&URLInfo,(u8*)&tmpURLInfo, sizeof(tsURLInfo));
      memset((u8*)Setting.Company_Name, 0, 12);
      memcpy(Setting.Company_Name, URLInfo.CompanyID, 12); 
      FRAMMultiWrite((u8 *)Setting.Company_Name, DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting, 12);
      ModemInfo.ServerChange = 1;
      FRAMMultiWrite((u8 *)&ModemInfo.ServerChange, MODEM_INFO + (u8 *)&ModemInfo.ServerChange - (u8 *)&ModemInfo, sizeof(ModemInfo.ServerChange));
    }
    else if(mode == 1) {   //Power off후 적용
      URLChangeFlag = 1;
      vTaskDelay(3000);
      Power_Mode(0);
    }
    rc = 2;
  }
  else rc = 3;
  
  return rc;
}

void CLI_Process_GVN(void)
{
  u8 len, resp = 0, i;
  u32 val; 
  u8 str_len, svr_name_flag;
  u16 addr;
  
  CLI_State = 3;
  switch(CLI_Cmd) {
    case 1:  //APN 변경
      len = StringLen((char*)CLI_RxMsg,100);
      if(len < 32) {
        memset((u8*)ComRxAPN_Name, 0, sizeof(ComRxAPN_Name));
        memcpy((u8*)ComRxAPN_Name, CLI_RxMsg, len);
        resp = APNProcess(2);  
      }
      if(resp == 1) CLI_State = 2;
      else CLI_State = 3;
      break;
    case 2:  //QOS Upload
      len = StringLen((char*)CLI_RxMsg,100);
      if(len < 8) {
        val = 0;
        for(i = 0; i < len; i++)
          if(CLI_RxMsg[i] >= '0' && CLI_RxMsg[i] <= '9')
            val = val*10 + CLI_RxMsg[i]-'0';
        if(val >= 8 && val <= 3648) {
          resp = QoSAdust(0, val);
        }
      }
      if(resp) CLI_State = 2;
      else CLI_State = 3;
      break;
    case 3:  //QOS Download
      len = StringLen((char*)CLI_RxMsg,100);
      if(len < 8) {
        val = 0;
        for(i = 0; i < len; i++)
          if(CLI_RxMsg[i] >= '0' && CLI_RxMsg[i] <= '9')
            val = val*10 + CLI_RxMsg[i]-'0';
        if(val >= 8 && val <= 3648) {
          resp = QoSAdust(1, val);
        }
      }
      if(resp) CLI_State = 2;
      else CLI_State = 3;
      break;
    case 4:  //Server Domain
      len = StringLen((char*)CLI_RxMsg,100);
      CLI_State = ServerDomain_Process_GVN((char*)CLI_RxMsg, len, 1);
      break;
    case 5:  //Firmware Download Domain
      len = StringLen((char*)CLI_RxMsg,100);
      if(len < 60) {
        str_len = 0;
        svr_name_flag = 0;
        val = 0;
        memset((u8*)MdmTmpString, 0, 100);
        for(i=0; i < len; i++) {
          if(svr_name_flag == 0){
            if(CLI_RxMsg[i] != ':')
              MdmTmpString[str_len++] = CLI_RxMsg[i]; 
            else if(str_len < 60){
              memcpy((u8*)FWDN_Svr.SvrName, MdmTmpString, 60);
              memset((u8*)MdmTmpString, 0, 100);
              str_len = 0;
              svr_name_flag++;
            }
          }
          else if(svr_name_flag == 1){
            if(CLI_RxMsg[i] >= '0' && CLI_RxMsg[i] <= '9')
              val = val*10 + CLI_RxMsg[i]-'0';
            else if(val > 0 && val < 10000){
              FWDN_Svr.SvrPort = val; 
              svr_name_flag++;
            }
          }
        }
        if(svr_name_flag > 0) {
          if(svr_name_flag >= 1) {
            if(val > 0 && val < 10000)
              FWDN_Svr.SvrPort = val; 
          }
          FRAMMultiWrite((u8*)&FWDN_Svr, OTA_FWDN_SVR, sizeof(tsSvrInfo));    
          CLI_State = 2;
        }
      }
      else CLI_State = 3;
      break;
    case 6:  //Company Reg. No.
      len = StringLen((char*)CLI_RxMsg,100);
      if(len <= sizeof(Setting.Company_Reg_No)) {
        memcpy(Setting.Company_Reg_No, CLI_RxMsg, sizeof(Setting.Company_Reg_No));
        addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Reg_No - (u8 *)&Setting;
        FRAMMultiWrite((u8 *)&Setting.Company_Reg_No, addr, sizeof(Setting.Company_Reg_No)); 
        CLI_State = 2;
      }
      else CLI_State = 3;
      break;
    case 7:  //VRN
      len = StringLen((char*)CLI_RxMsg,100);
      if(len > 12 || len < 8) {
        CLI_State = 3;
        return;
      }
      if(VrnToCarRegNo(CLI_RxMsg, len))
        CLI_State = 2;
      else
        CLI_State = 3;
      break;
    case 8:  //Driver ID
      len = StringLen((char*)CLI_RxMsg,100);
      if(len <= sizeof(Setting.Drv_Code)) {
        memcpy(Setting.Drv_Code, CLI_RxMsg, sizeof(Setting.Drv_Code));

        if(Setting.Drv_Code[10] >= '1' && Setting.Drv_Code[10] <= '9') {
          Selected_Driver = Setting.Drv_Code[10]-'1';
        }
        else {
          Selected_Driver = 0;
          Setting.Drv_Code[10] = '1';
        }
        addr = DRV_SETTING_PARM + (u8 *)&Setting.Drv_Code - (u8 *)&Setting;
        FRAMMultiWrite((u8 *)&Setting.Drv_Code, addr, sizeof(Setting.Drv_Code)); 
        FRAMMultiWrite((u8 *)&Selected_Driver, SELECTED_DRIVER, 1);  
        memcpy(Driver_Info[Selected_Driver].Code, Setting.Drv_Code, 10);
        memcpy(Driver_Info[Selected_Driver].Num, &Setting.Drv_Code[11], 7);
        addr = DRIVER_INFO+Selected_Driver*sizeof(st_DRIVER_INFO);
        FRAMMultiWrite((u8 *)&Driver_Info[Selected_Driver], addr, sizeof(st_DRIVER_INFO));
        CLI_State = 2;
      }
      else CLI_State = 3;
      break;
    case 21:  //Total Trip Setting
      len = StringLen((char*)CLI_RxMsg,100);
      val = 0;
      if(len < 7) {
        for(i = 0; i < len; i++)
          if(CLI_RxMsg[i] >= '0' && CLI_RxMsg[i] <= '9')
            val = val*10+CLI_RxMsg[i]-'0';
        printk("\r\nMODEM : CLI Trip Setting : %dKm\r\n",val);
        DailyData.TTrip = (double)val;
        FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
//        SecData.TTrip = DailyData.TTrip;
//        Val.TTrip = DailyData.TTrip;
//        Val.TTripOld = Val.TTrip;          
        CLI_State = 2;
      }
      else CLI_State = 3;
      break;
    case 10:  //Reboot DTG + MDT
      CLI_RxMsg[0] = 1;
      CLI_Msg.CLI_No = CLI_No;  
      CLI_Msg.CLI_Cmd = CLI_Cmd;  
      CLI_Msg.CLI_State = 1;  
      FRAMMultiWrite((u8 *)&CLI_Msg, GVN_CLI_MSG, sizeof(tsCLI_Msg)); 
      Modem_ResetProcess();
      NVIC_SystemReset();
      break;
    case 11:  //Reboot MDT
      CLI_Msg.CLI_No = CLI_No;  
      CLI_Msg.CLI_Cmd = CLI_Cmd;  
      CLI_Msg.CLI_State = 1;  
      FRAMMultiWrite((u8 *)&CLI_Msg, GVN_CLI_MSG, sizeof(tsCLI_Msg)); 
      Modem_ResetProcess();
      NVIC_SystemReset();
//      CLI_RxMsg[0] = 1;
//      DTG_TxData(CM_PRMW, 100, 0, 1, CLI_RxMsg);        
//      CLI_State = 2;
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
void Modem_DataSvrProcess_GVN(void)
{
  u8 resp;

  if(!DataServerFlag || Setting.ExtModemMode != KT_GVN_PROTOCOL) {
    DataSvrStep = DATA_SVR_IDLE;
  }
  
  switch(DataSvrStep) {
    case DATA_SVR_DATACHECK:
      if(GV_DataContFlag)
        SocketConnectStep = SOCKET_CREG_CHECK;
      else
        SocketConnectStep = SOCKET_CONNECT_START;
      
      resp = MakeSDRSet();
      if(resp == 1) 
        DataSvrStep = DATA_SVR_CONNECT;
      else if(resp == 2 && (DataAddrToSend != DataSentAddr)) {
        DataSvrStep = DATA_SVR_SEND_WAIT;
        DataSvrWaitCount = 0;
      }
      else
        DataSvrStep = DATA_SVR_CLOSE;
      SocketWaitCnt = 0;
      SocketRetryCnt = 0;
      DataSvrWaitCount = 0;
      break;
      
    case DATA_SVR_CONNECT:
      Modem_SocketConnect(URLInfo.URLName, URLInfo.Port, 2);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          DataSvrStep = DATA_SVR_SEND;
          vTaskDelay(100);
          DataSvrWaitCount = 0;
        }
        else {
          Modem_SocketClose(0);
          DataSvrStep = DATA_SVR_FAIL;
        }
      }
      break;

    case DATA_SVR_SEND_WAIT: 
      DataSvrWaitCount++;
      if(DataSvrWaitCount >= 2000) {
        DataSvrStep = DATA_SVR_DATACHECK;
      }
      break;
      
    case DATA_SVR_SEND:
      if(TcpState != 1) {
          DataSvrStep = DATA_SVR_FAIL;
          return;
      }
      DataSvrWaitCount = 0;
      PostSDRSet();
      break;
      
    case DATA_SVR_RECIEVE_WAIT:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 60000) {  //60sec
        DataSvrWaitCount = 0;
        Modem_SocketClose(0);
        DataSvrStep = DATA_SVR_FAIL;
      }
      break;

    case DATA_SVR_FAIL_WAIT:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 60000) {  //10sec
        Modem_SocketClose(0);
        DataSvrStep = DATA_SVR_FAIL;
      }
      break;
      
    case DATA_SVR_CLOSE:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 1000) {  //1sec
        if(TCP_Open ==  1) {      
          if(Modem_SocketClose(0)==1) {
            DataSvrStep = DATA_SVR_SUCCESS;
          }
          else {
            DataSvrStep = DATA_SVR_FAIL;
          }
        }
        else {
          DataSvrStep = DATA_SVR_SUCCESS;
        }
      }
      break;

    case DATA_SVR_FAIL:
      printk("\r\nMODEM : Data Send Fail\r\n");
      ModemWaitCnt = 0;
      DataServerRetryCount++;
      if(DataServerRetryCount > SDRPolicy.RetryCnt) {
        DataServerRetryCount = 0;
        if(DataServerFlag == 2) {
#ifdef __USE_DMS_SERVER__       
          OnNotiFlag = 0;
          if(VdisServerFlag != 2 && SendAdasFlag != 2 && OffAdasFlag == 0) {  
            OffNotiFlag = 1;  
            OffNotiRetryCount = 0;
          }
          ModemStep = MODEM_DATA_IDLE;
#else
          if(VdisServerFlag != 2) {
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
          }
          else
            ModemStep = MODEM_DATA_IDLE;
#endif          
        }
        else {
          if(HttpRespCode == 400 || HttpRespCode == 480 || HttpRespCode == 481 || HttpRespCode == 482) { 
            SDRPolicyRetryDelayFlag = 1;
            SDRPolicyRetryDelayCnt = 0;
          }
          else if(HttpRespCode == 0) {
            GV_SendTimeCnt = 0;
            if(BkURLInfo.BackupFlag == 1) {
              BkURLInfo.BackupFlag = 0;
              memcpy((u8*)&URLInfo,(u8*)&BkURLInfo, sizeof(tsURLInfo));
              FRAMMultiWrite((u8 *)&URLInfo, GVN_URL_INFO, sizeof(tsURLInfo));
              FRAMMultiWrite((u8*)&BkURLInfo, GVN_BK_URL_INFO, sizeof(tsURLInfo));    
              memset((u8*)Setting.Company_Name, 0, 12);
              memcpy(Setting.Company_Name, URLInfo.CompanyID, 12); 
              FRAMMultiWrite((u8 *)Setting.Company_Name, DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting, 12);
            }              
          }
          ModemWaitCnt = 0;
          ModemStep = MODEM_POWER_ON_IDLE;
          Modem_PPPClose();
        }
        DataSvrWaitCount = 0;
        DataServerFlag = 0;        
      }
      else {
        DataSvrWaitCount = 0;
        ModemStep = MODEM_IDLE;
      }      
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;      
      COM_Command_Char = 'F';
      break;

    case DATA_SVR_SUCCESS:
      printk("\r\nMODEM : Data Send Sucess\r\n");
      if(DataServerFlag == 2) {
        if(DataSentAddr == DataSaveAddr) {
#ifdef __USE_DMS_SERVER__          
          OnNotiFlag = 0;
          if(VdisServerFlag != 2 && SendAdasFlag != 2 && OffAdasFlag == 0) {  
            OffNotiFlag = 1;  
            OffNotiRetryCount = 0;
          }
          ModemStep = MODEM_DATA_IDLE;
#else        
          if(VdisServerFlag != 2) {
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
          }
          else
            ModemStep = MODEM_DATA_IDLE;
#endif        
          DataServerFlag = 0;
        }
        else {
          DataAddrToSend = DataSaveAddr;
          ModemStep = MODEM_DATA_IDLE;
        }
      }
      else {
        Modem_PPPClose();
        ModemStep = MODEM_DATA_IDLE;
        DataServerFlag = 0;
      }
      ModemWaitCnt = 0;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      COM_Command_Char = 'S';
      break;

    case DATA_SVR_IDLE:
      if(DataServerFlag == 2) {
#ifdef __USE_DMS_SERVER__          
        OnNotiFlag = 0;
        if(VdisServerFlag != 2 && SendAdasFlag != 2 && OffAdasFlag == 0) {  
          OffNotiFlag = 1;  
          OffNotiRetryCount = 0;
        }
        ModemStep = MODEM_DATA_IDLE;
#else        
        if(VdisServerFlag != 2) {
          Modem_PPPClose();
          ModemStep = MODEM_POWER_OFF;
        }
        else
          ModemStep = MODEM_DATA_IDLE;
#endif        
      }
      else
        ModemStep = MODEM_DATA_IDLE;
      ModemWaitCnt = 0;
      DataServerFlag = 0;
      break;
      
    default : 
      DataSvrStep = DATA_SVR_FAIL;
      break;  
  }
}


void Modem_SecInt_GVN(void)
{
  u32 daddr = 0, data_cnt;
  
  if(Setting.ExtModemMode != KT_GVN_PROTOCOL) return;
  
  if(ModemInfo.DataServerInterval > 9 && ModemInfo.DataServerInterval < 10000) {
    if(SDRPolicyRetryDelayFlag) {
      SDRPolicyRetryDelayCnt++;
      if(SDRPolicyRetryDelayCnt > SDRPolicy.RetryDelay*60) {  //Min x 60sec
        GV_Discard_Cnt++;
        if(GV_Discard_Cnt >= SDRPolicy.DiscardCnt) {
          GV_Discard_Cnt = 0;
          GV_DiscardFlag = 1;
        }
        GV_Term_Cnt++;
        if(GV_Term_Cnt >= SDRPolicy.TermCnt) {
          GV_Term_Cnt = 0;
          TxTerminateFlag = 1;
        }
        if(!TxTerminateFlag) {
          GV_SendTimeCnt = 0;
          DataServerFlag = 1;
          DataServerRetryCount = 0;              
        }
        SDRPolicyRetryDelayFlag = 0;
      }
    }
    else {
      GV_SendTimeCnt++;  
      if(NeworkErrorFlag) {
        if(GV_SendTimeCnt >= 1800) {
          if(!DataServerFlag && !TxTerminateFlag && !Power_Off_Flag) {
            DataServerFlag = 1;
            DataAddrToSend = DataSaveAddr;
            DataServerRetryCount = 0;
            GV_SendTimeCnt = 0;
          }
        }
      }
      else {
        daddr = (u32)(DataSaveAddr-DataSentAddr);
        data_cnt = daddr/sizeof(tsSecData);
        if((GV_SendTimeCnt >= ModemInfo.DataServerInterval) && (data_cnt >= ModemInfo.DataServerInterval)) {
          if(!DataServerFlag && !TxTerminateFlag && !Power_Off_Flag) {
            DataServerFlag = 1;
            DataAddrToSend = DataSaveAddr;
            DataServerRetryCount = 0;
            GV_SendTimeCnt = 0;
          }
        }
      }
    }
  }
}


void Modem_URLChangeReset(void)
{
  ModemInfo.ServerChange = 1;
  FRAMMultiWrite((u8 *)&ModemInfo.ServerChange, MODEM_INFO + (u8 *)&ModemInfo.ServerChange - (u8 *)&ModemInfo, sizeof(ModemInfo.ServerChange));
  NVIC_SystemReset();
}

void  Modem_ChangeURLInfo(void)
{
  if(StringLen((char*)Setting.Company_Name,12) == 10) {    
    if(!IsSameString((char*)Setting.Company_Name,URLInfo.CompanyID, 12)) {
      memset((u8*)URLInfo.CompanyID, 0, 20);
      memcpy(URLInfo.CompanyID, Setting.Company_Name, 10); 
      FRAMMultiWrite((u8 *)URLInfo.CompanyID, GVN_URL_INFO + (u8 *)&URLInfo.CompanyID - (u8 *)&URLInfo, 10);
      memcpy((u8*)&BkURLInfo, (u8*)&URLInfo, sizeof(tsURLInfo));
      BkURLInfo.BackupFlag = 0;
      FRAMMultiWrite((u8*)&BkURLInfo, GVN_BK_URL_INFO, sizeof(tsURLInfo));    
      Prev_DataFlag = 0;
      SDR_Policy_State = 1;
      ModemInfo.ServerChange = 1;
      FRAMMultiWrite((u8 *)&ModemInfo.ServerChange, MODEM_INFO + (u8 *)&ModemInfo.ServerChange - (u8 *)&ModemInfo, sizeof(ModemInfo.ServerChange));
    }
  }
}

void Modem_Parameter_Reset_GVN(void)
{
  memset(URLInfo.URLName,0,sizeof(tsURLInfo));    
#ifdef __USE_GVN_TEST_SERVER__   
  sprintf(URLInfo.URLName, "14.63.248.9");              // KT 인증  GVN 서버. //HTTP://14.63.248.9/TFM_CT/LOOP000001/vid-rp
#else  
  sprintf(URLInfo.URLName, "vehicle.biz.olleh.com");    // KT 상용  GiGA FMS GVN 서버.   
#endif  
  sprintf(URLInfo.SubDir, "TFM_CT");
  sprintf(URLInfo.CompanyID, "LOOP000001");
  sprintf(URLInfo.DataDir, "vid-rp");
  URLInfo.Port = 80;
  FRAMMultiWrite((u8 *)&URLInfo, GVN_URL_INFO, sizeof(tsURLInfo));

  memcpy((u8*)&BkURLInfo,(u8*)&URLInfo, sizeof(tsURLInfo));
  FRAMMultiWrite((u8 *)&BkURLInfo, GVN_BK_URL_INFO, sizeof(tsURLInfo));
  
  memset((u8*)&SDRPolicy,0,sizeof(tsSDR_Policy));
  SDRPolicy.PolicyNum = 0;
  SDRPolicy.TxInterval = 180;
  SDRPolicy.DataInterval = 1;
  SDRPolicy.RetryDelay = 10;
  SDRPolicy.RetryCnt = 3;
  SDRPolicy.DiscardCnt = 3;
  SDRPolicy.TermCnt = 3;
  SDRPolicy.FactorIDEnable[0] = 1;   //필수 항목
  SDRPolicy.FactorIDEnable[1] = 1;   //필수 항목
  SDRPolicy.FactorIDEnable[2] = 1;   //필수 항목
  SDRPolicy.FactorIDEnable[20] = 1; //필수 항목
  FRAMMultiWrite((u8 *)&SDRPolicy, GVN_SDR_POLICY, sizeof(tsSDR_Policy));

  memset((u8*)&CLI_Msg,0,sizeof(tsCLI_Msg));
  FRAMMultiWrite((u8 *)&CLI_Msg, GVN_CLI_MSG, sizeof(tsCLI_Msg)); 
}


void Modem_Init_GVN(void)
{
  u8 modify = 0;
  
  FRAMMultiRead((u8 *)&ModemInfo, MODEM_INFO, sizeof(tsModemInfo));

  if(DataSaveAddr != DataSentAddr && ModemInfo.ServerChange == 0 && ModemInfo.ServerConnected == 1) {
    Prev_DataFlag = 1;
    Prev_DataAdrrToSend = DataSaveAddr; 
  }
  
  FRAMMultiRead((u8 *)&URLInfo, GVN_URL_INFO, sizeof(tsURLInfo));
//  if(URLInfo.URLName[0] < ' ' || URLInfo.URLName[0] > 'z') {
//#if 0
//    memset(URLInfo.URLName,0,60);
//    sprintf(URLInfo.URLName, "loopiot.com");
//    memset((u8*)URLInfo.SubDir, 0, 20);
//    sprintf(URLInfo.SubDir, "io/kt");
//    memset((u8*)URLInfo.CompanyID, 0, 20);
//    memset((u8*)URLInfo.DataDir, 0, 20);
//#else
//    memset(URLInfo.URLName,0,60);
//#ifdef __USE_GVN_TEST_SERVER__   
//    sprintf(URLInfo.URLName, "14.63.248.9");              // KT 인증  GVN 서버.
//#else    
//    sprintf(URLInfo.URLName, "vehicle.biz.olleh.com");    // KT 상용  GiGA FMS GVN 서버.    
//#endif    
//    memset((u8*)URLInfo.SubDir, 0, 20);
//    sprintf(URLInfo.SubDir, "TFM_CT");
//    memset((u8*)URLInfo.CompanyID, 0, 20);
//    sprintf(URLInfo.CompanyID, "LOOP000001");
//    memset((u8*)URLInfo.DataDir, 0, 20);
//    sprintf(URLInfo.DataDir, "vid-rp");
//#endif  
//    URLInfo.Port = 80;
//    FRAMMultiWrite((u8 *)&URLInfo, GVN_URL_INFO, sizeof(tsURLInfo));
//  }
  
  FRAMMultiRead((u8 *)&BkURLInfo, GVN_BK_URL_INFO, sizeof(tsURLInfo));
  
  modify = 0;
  FRAMMultiRead((u8 *)&SDRPolicy, GVN_SDR_POLICY, sizeof(tsSDR_Policy));
  if(SDRPolicy.PolicyNum == 0) {
    Prev_DataFlag = 0;
    Prev_DataAdrrToSend = 0; 
  }
  if(SDRPolicy.TxInterval == 0 || SDRPolicy.TxInterval > 3600) {
    SDRPolicy.TxInterval = 180;
    modify = 1;
  }
  if(SDRPolicy.DataInterval == 0 || SDRPolicy.DataInterval > 180) {
    SDRPolicy.DataInterval = 1;
    modify = 1;
  }
  if(SDRPolicy.RetryDelay == 0 || SDRPolicy.RetryDelay > 10) {
    SDRPolicy.RetryDelay = 1;
    modify = 1;
  }
  if(SDRPolicy.RetryCnt > 3) {
    SDRPolicy.RetryCnt = 3;
    modify = 1;
  }
  if(SDRPolicy.DiscardCnt > 3) {
    SDRPolicy.DiscardCnt = 3;
    modify = 1;
  }
  if(SDRPolicy.TermCnt > 3) {
    SDRPolicy.TermCnt = 3;
    modify = 1;
  }
  if(SDRPolicy.FactorIDEnable[0] == 0) {
    SDRPolicy.FactorIDEnable[0] = 1;   //필수 항목
    modify = 1;
  }
  if(SDRPolicy.FactorIDEnable[1] == 0) {
    SDRPolicy.FactorIDEnable[1] = 1;   //필수 항목
    modify = 1;
  }
  if(SDRPolicy.FactorIDEnable[2] == 0) {
    SDRPolicy.FactorIDEnable[2] = 1;   //필수 항목
    modify = 1;
  }
  if(SDRPolicy.FactorIDEnable[20] == 0) {
    SDRPolicy.FactorIDEnable[20] = 1; //필수 항목
    modify = 1;
  }
  if(modify)
    FRAMMultiWrite((u8 *)&SDRPolicy, GVN_SDR_POLICY, sizeof(tsSDR_Policy));

  FRAMMultiRead((u8 *)&CLI_Msg, GVN_CLI_MSG, sizeof(tsCLI_Msg)); 
  
  if(CLI_Msg.CLI_Cmd > 0 &&  CLI_Msg.CLI_State == 1) {
    CLI_No = CLI_Msg.CLI_No;  
    CLI_Cmd = CLI_Msg.CLI_Cmd;  
    CLI_State = 2;  
  }

  if(CLI_Msg.CLI_No != 0 || CLI_Msg.CLI_Cmd != 0 || CLI_Msg.CLI_State != 0) {
    CLI_Msg.CLI_No = 0;  
    CLI_Msg.CLI_Cmd = 0;  
    CLI_Msg.CLI_State = 0;  
    FRAMMultiWrite((u8 *)&CLI_Msg, GVN_CLI_MSG, sizeof(tsCLI_Msg)); 
  }
  
  SDR_Policy_State = 1;
  if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {
    if(!IsSameString((char*)Setting.Company_Name, (char*)URLInfo.CompanyID, 12)) { 
      if(URLInfo.CompanyID[9] >= '0' && URLInfo.CompanyID[9] <= 'Z') {
        memset((u8*)Setting.Company_Name, 0, 12);
        memcpy(Setting.Company_Name, URLInfo.CompanyID, 12); 
        FRAMMultiWrite((u8 *)Setting.Company_Name, DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting, 12);
      }
    }
  }
}

void Modem_DataIdleProcess_GVN(void)
{
  if(ModemStep == MODEM_DATA_IDLE) {
    if(DataServerFlag) {
      DataSvrStep = DATA_SVR_DATACHECK;
      DataSvrWaitCount = 0;
      ModemStep = MODEM_DATA_SERVER;    
    }
    else if(CLI_State == 1) {
      CLI_Process_GVN();
    }
  }
}



/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/
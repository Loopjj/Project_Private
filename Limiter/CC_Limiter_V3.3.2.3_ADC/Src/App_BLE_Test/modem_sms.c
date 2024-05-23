/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : modem_sms.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 03/27/2018
*
* Description        : modem sms decode
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define MODEM_SMS_GLOBALS
#include "includes.h"
#include "modem_Loop.h"
#include "modem_Hanuri.h"
#include "modem_GVN.h"
#include "modem_DMS.h"

u8 MsgIdx;
u16 MsgData;
int SMS_No, CommaCnt;
u8 SMS_No_Flag, RxSmsCnt;

u32 DataFileIp;
u16 DataFilePort;
time_t DataFileStart, DataFileEnd;
u8 DataFileSendFlag;
char RespMsg[80];
char RcvNum [20];


/*
********************************************************************************
* Description : Modem_SMSSend
* Arguments   : snum, msg, len
* Return      : void
* Note        : Send SMS Data
******************************************************************************** 
*/
char TxSmsResp[10];
void Modem_SMSSend(char* snum, char* msg, u8 len)
{
  int cnt = 0, x;
  u8 data,err,msg_flag = 0;
  
  memset(MdmTmpString,0,100); 
  sprintf(MdmTmpString,"AT+CMGS=\"%s\"\r\n",snum);   
  ModemCommInit();
  TxStrToModem(MdmTmpString);
  
  do {
    vTaskDelay(1);
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      if(data == '>') {
        msg_flag = 1;
        break;        
      }
    }
    cnt++;
  }while (cnt < 300);
  if(msg_flag) {
    TxStrToModem(msg);
    printk("\r\nMODEM : SMS Send %s\r\n", msg);
  }
  else
    printk("\r\nMODEM : SMS Send Fail\r\n");
  vTaskDelay(100);
  TxCharToModem(0x1A);  // ctrl+Z : 0x1A
  memset(TxSmsResp, 0, 10);
  x = 0;
  cnt = 0;
  do{
    data = ModemCommGetChar(&err);
   	if ( err == MODEM_COMM_NO_ERR ) {
      if(data == '\r' || data == '\n') x = 0;
      if(x < 10) {
        TxSmsResp[x++] = data;
        if((strstr(TxSmsResp, "OK") != NULL) || (strstr(TxSmsResp, "ERROR") != NULL))
            break;
      }
    }
    vTaskDelay(1);
    cnt++;
  }while (cnt < 3000);
  vTaskDelay(1000);
}

/*
********************************************************************************
* Description : Modem_SMS_DeleteAll
* Arguments   : none
* Return      : resp
* Note        : SMS delete all
******************************************************************************** 
*/
unsigned char  Modem_SMS_DeleteAll(void)
{
  u8 rcnt,resp;
  
  rcnt=0;  resp=0;
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand("AT+CMGD=,4\r\n", "OK", "ERROR", 300);
    vTaskDelay(1);
    rcnt++;
  }

  if(resp == 1)
    printk("\r\nMODEM : SMS deleted all OK\r\n");
  else
    printk("\r\nMODEM : SMS deleted all Error\r\n");
  return resp;
}

unsigned char  Modem_SMS_Delete(u8 idx)
{
  u8 rcnt,resp;
  
  rcnt=0;  resp=0;

  memset(MdmTmpString,0,100);
  sprintf(MdmTmpString, "AT+CMGD=%d\r\n",idx);
  while(resp == 0 && rcnt < AT_RETRY_CNT) { 
    resp = sendATcommand(MdmTmpString, "OK", "ERROR", 300);
    vTaskDelay(1);
    rcnt++;
  }

  if(resp == 1)
    printk("\r\nMODEM : SMS[%d] deleted OK\r\n", idx);
  else
    printk("\r\nMODEM : SMS[%d] deleted Error\r\n", idx);
  return resp;
}

void SMS_Process(u8  r)
{
  u8 resp;
  
  switch(SMSData.Step) {
    case 0:
      if(r == ',') {
        SMSData.Step = 1;
        MsgIdx = 0;
        SMS_No = 0;
        SMS_No_Flag = 0;
      }
      break;
    case 1: 
      if(r == '\r') {
        memset(MdmTmpString,0,100);
        sprintf(MdmTmpString, "AT+CMGR = %d\r\n",SMS_No);
        resp = AT_SendWithRecieveData(MdmTmpString,"+CMGR: ", 2, 2000);
        if(resp == 1) {
          SMSData.RCnt = StringLen(ATCmdRxMsg[1],32);
          memcpy(SMSData.RcvNum,ATCmdRxMsg[1],SMSData.RCnt);
          SMSData.Step = 8;  
        }
//        ModemCommInit();
//        sprintf(MdmTmpString, "AT+CMGR = %d\r\n",SMS_No);
//        TxStrToModem(MdmTmpString);
//        CommaCnt = 0;
//        SMSData.Step = 2;
      }
      else { 
        if(r >= '0' && r <= '9' && SMS_No_Flag < 2) {
          SMS_No = SMS_No*10+(r-'0');
          SMS_No_Flag = 1;
        }
        else
          SMS_No_Flag = 2;
      }
      break;
    case 2:
      if(r == ',') {
        CommaCnt++;
        if(CommaCnt >= 1) {
          CommaCnt = 0;
          SMSData.Step = 3;
        }
      }
      break;
    case 3:
      if(r == '\"') {
        SMSData.Step = 4;
        SMSData.RCnt = 0;
        memset(SMSData.RcvNum, 0, sizeof(SMSData.RcvNum));
      }
      break;
    case 4:
      if(r == '\"') {
        SMSData.Step = 5;
      }
      else {
        if(SMSData.RCnt < sizeof(SMSData.RcvNum))
          SMSData.RcvNum[SMSData.RCnt++] = r;
      }
      break;
    case 5:
      if(r == ',') {
        CommaCnt++;
        if(CommaCnt >= 2) {
          CommaCnt = 0;
          SMSData.Step = 6;
        }
      }
      break;
    case 6:
      if(r == '\"') {
        SMSData.Step = 7;
        SMSData.DCnt = 0;
        memset(SMSData.Date, 0, sizeof(SMSData.Date));
      }
      break;
    case 7:
      if(r == '\"') {
        SMSData.Step = 8;
      }
      else {
        if(SMSData.DCnt < sizeof(SMSData.Date))
          SMSData.Date[SMSData.DCnt++] = r;
      }
      break;
    case 8:
      if(r == '\n') {
        SMSData.Step = 9;
        SMSData.MCnt = 0;
        MsgIdx = 0;
        MsgData = 0;
        memset(SMSData.Msg, 0, sizeof(SMSData.Msg));
      }
      break;
    case 9:
      if(r != '\n' && r != '\r') {
        if(Setting.ExtModemTelecom == 1) {
          if(r >= '0' && r <= 'F')  {
            if((MsgIdx%2) == 0) {
              MsgData = 0;
              MsgData = HEXTOBIN(r)<<4;
            }
            else {
              MsgData += HEXTOBIN(r);
              if(SMSData.MCnt < sizeof(SMSData.Msg)) 
                SMSData.Msg[SMSData.MCnt++] = MsgData;
            }
            MsgIdx++;
          }
          else SMSData.Step = 100;
        }
        else if(SMSData.MCnt < sizeof(SMSData.Msg)) {
            SMSData.Msg[SMSData.MCnt++] = r;
        }
      }
      else {
        DecodeSMS(1);
        SMSData.Step = 0;
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;  
        Urc.Cnt = 0;
      }
      break;

    case 100:
      SMSData.Step = 0;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;  
      Urc.Cnt = 0;
      break;
  }
}

void Modem_SMS_ReadAll(void)
{
  u32  current, previous;
  int idx = 0;
  u8 str_flag = 0, data, err, data_flag = 0, val = 0;
  u8 resp = 0, msg_cnt = 0;

  memset(AT_Response, 0, 256);

  ModemCommInit();
  
  MsgIdx = 0;
  RxSmsCnt = 0;
  TxStrToModem("AT+CMGL=\"REC UNREAD\"\r\n");    
  init_millis();
  previous = millis();
  do{
    data = ModemCommGetChar(&err);
    if ( err == MODEM_COMM_NO_ERR ) {
      if(data_flag == 0) {
        if(data == '\n') idx = 0;
        else {
          if(idx < 256) {
            AT_Response[idx++] = data;
          }
          else {
            printk("\r\nMODEM : Read SMS  %u complete\r\n", msg_cnt);          
            break;
          }
          if (strstr(AT_Response, "+CMGL: ") != NULL) {
            data_flag = 1;
            SMS_No = 0; 
            val = 0;
          }
          else if (strstr(AT_Response, "OK") != NULL) {
            resp = 1;
            break;
          }
        }
      }
      else if(data_flag == 1) {
        if(data == ',')  {
          SMS_No = val;
          data_flag = 2;
        }
        else if(data >= '0' && data <='9'){
          val = val*10+(data-'0');
        }
      }
      else if(data_flag == 2) {
        if(data == ',') {
          data_flag = 3;
          SMSData.RCnt = 0;
          memset(SMSData.RcvNum, 0, 20);
        }
      }
      else if(data_flag == 3) {
        if(data == ',') {
          data_flag = 4;  
          str_flag = 0;
        }
        else if(data >= '0' && data <='9'){
          if(SMSData.RCnt < 20)
            SMSData.RcvNum[SMSData.RCnt++] = data; 
        }
      }
      else if(data_flag == 4) {
        if(data == '\n') {
          if(str_flag == 0) {
            str_flag = 1;
            SMSData.MCnt = 0;
            memset(SMSData.Msg, 0, 80);
            MsgIdx = 0;
          }
          else {
            DecodeSMS(0);
            msg_cnt++;
            data_flag = 0;
            idx = 0;
            memset(AT_Response, 0, 256);
            previous = millis();
          }
        }
        else if(data != '\r'){
          if(str_flag == 1) {
            if(Setting.ExtModemTelecom == 1) {
              if(data >= '0' && data <= 'F')  {
                if((MsgIdx%2) == 0) {
                  MsgData = 0;
                  MsgData = HEXTOBIN(data)<<4;
                }
                else {
                  MsgData += HEXTOBIN(data);
                  if(SMSData.MCnt < 80) 
                    SMSData.Msg[SMSData.MCnt++] = MsgData;
                }
                MsgIdx++;
              }
            }
            else if(SMSData.MCnt < 80)
              SMSData.Msg[SMSData.MCnt++] = data; 
          }
        }
      }
    }
    current = millis();
    vTaskDelay(1);
  }
  while(((current-previous) < 5000));
  if(RxSmsCnt > 0) {
    memset(RespMsg,0,80);
    sprintf(RespMsg,"Process Recieved %d Message", RxSmsCnt);
    Modem_SMSSend(RcvNum,RespMsg, StringLen(RespMsg,80));
  }
  if(resp == 1 && msg_cnt > 0)
    Modem_SMS_DeleteAll();
}
/*
********************************************************************************
* Description : DecodeSMS
* Arguments   : none
* Return      : none
* Note        : Decode SMS
******************************************************************************** 
*/
void DecodeSMS(u8 msg_delete)
{
  u8 i, start = 0, cmd = 0;
  u32 val = 0;
  u8  val_cnt, val_item;
  u32 ip=0;
  u8 answer_flag = 0;
  u32 addr;
  u8  size;
  
  u32 port = 0;
  u16 svr_datatime = 0;
  u32 odo_val = 0;
  
  u16 data_savetime_idle = 0;

  u16 dtg_datatime = 0;

  u16 rpm_factor = 0;
  u16 k_factor = 0;
  u8 brake_factor = 0;

  u8 car_reg_no[12] = {0,};
  u8 car_vin[17] = {0,};
  u8 drv_code[18] = {0,};
  u8 company_reg_no[10] = {0,};
  u8 kidx,ridx;
  u8 company_id[10] = {0,};   

  u8 start_date[6] = {0,};
  u8 end_date[6] = {0,};
  rtc_time tmp_time;
  u8 ext_mdm_mode, vdis_mode;
  u8 reset_type;

  if(SMSData.Msg[0] == '&')  {
    if(SMSData.Msg[2] == ',') {
      cmd = SMSData.Msg[1]-'0';
      start = 3;
    }
    else if(SMSData.Msg[3] == ',') {
      cmd = (SMSData.Msg[1]-'0')*10+(SMSData.Msg[2]-'0');
      start = 4;
    }
  }
  else if(SMSData.Msg[0] == '#') {
    if(IsSameString2(SMSData.Msg, "#SETODOOFFSET ", SMSData.MCnt, 14, &start)){
        cmd = 3;
    }
    if(IsSameString2(SMSData.Msg, "#SETSVRINTERVAL ", SMSData.MCnt, 16, &start)){
        cmd = 21;
    }
    if(IsSameString2(SMSData.Msg, "#SETRVFACTOR ", SMSData.MCnt, 13, &start)){
        cmd = 22;
    }
    if(IsSameString2(SMSData.Msg, "#SETVEHINFO ", SMSData.MCnt, 12, &start)){
        cmd = 23;
    }
    if(IsSameString2(SMSData.Msg, "#LOOPMODE", SMSData.MCnt, 9, &start)){
        cmd = 100;
    }
    else if(IsSameString2(SMSData.Msg, "#SETPTMODE", SMSData.MCnt, 10, &start)){
        cmd = 110;
    }
    else if(IsSameString2(SMSData.Msg, "#SETGVNSVR", SMSData.MCnt, 10, &start)){
        cmd = 120;
    }
    else if(IsSameString2(SMSData.Msg, "#SETDMSSVR", SMSData.MCnt, 10, &start)){
        cmd = 130;
    }
    else if(IsSameString2(SMSData.Msg, "#SETQUALSVR", SMSData.MCnt, 11, &start)){
        cmd = 140;
    }
    else if(IsSameString2(SMSData.Msg, "#REQRESET", SMSData.MCnt, 9, &start)){
        cmd = 150;
    }
    else if(IsSameString2(SMSData.Msg, "#SETAPNNAME", SMSData.MCnt, 11, &start)){
        cmd = 160;
    }
    else if(IsSameString2(SMSData.Msg, "#SETHOSTSVR", SMSData.MCnt, 11, &start)){
        cmd = 170;
    }
    else if(IsSameString2(SMSData.Msg, "#SETVDISSVR", SMSData.MCnt, 11, &start)){
        cmd = 180;
    }
    else if(IsSameString2(SMSData.Msg, "#SETADASSVR", SMSData.MCnt, 11, &start)){
        cmd = 185;
    }  
    else if(IsSameString2(SMSData.Msg, "#LOOPFOTA", SMSData.MCnt, 9, &start)){
        cmd = 190;
    }
    else if(IsSameString2(SMSData.Msg, "#LOOPFWVER", SMSData.MCnt, 10, &start)){
        cmd = 200;
    }
    else if(IsSameString2(SMSData.Msg, "#REQSTATUS", SMSData.MCnt, 10, &start)){
        cmd = 210;
    }
    else if(IsSameString2(SMSData.Msg, "#SETCOMPANYID", SMSData.MCnt, 13, &start)){
        cmd = 220;
    }    
    else if(IsSameString2(SMSData.Msg, "#REQISGPARM", SMSData.MCnt, 11, &start)){
        cmd = 250;
    }
    else if(IsSameString2(SMSData.Msg, "#REQISGPOINT", SMSData.MCnt, 12, &start)){
        cmd = 251;
    }
    else if(IsSameString2(SMSData.Msg, "#SETISGPARM", SMSData.MCnt, 11, &start)){
        cmd = 252;
    }
    else if(IsSameString2(SMSData.Msg, "#SETISGPOINT", SMSData.MCnt, 12, &start)){
        cmd = 253;
    }
  }
  
  val_item = 0;
  val = 0;
  val_cnt = 0;
  
  switch(cmd) {
    case 1:  //Hanuri ip setting Hanuri Protocol로 전환   &1,#ipSet#@,111.222.111.222,1234,0
      if(IsSameString(&SMSData.Msg[start], "#ipSet#@", 8)) {
        start += 8+1;
        for(i = start; i < SMSData.MCnt; i++) {
          if(SMSData.Msg[i] == ',') {
            if(val_item == 0 && val_cnt == 3)
              ip += (val<<(val_cnt*8));
            else if(val_item == 1)
              port = val;
            val_item++;
            val = 0;
            val_cnt = 0;
          }     
          else {
            if(val_item == 0) {
              if(SMSData.Msg[i] == '.') {
                ip += (val<<(val_cnt*8));
                val_cnt++;
                val = 0;
              }
              else if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  val = val*10+(SMSData.Msg[i]-'0');
              }
              else {
                break;
              }
            }            
            else if(val_item == 1) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  val = val*10+(SMSData.Msg[i]-'0');
              }
              else {
                break;
              }
            }
            else if(val_item == 2) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  answer_flag = SMSData.Msg[i]-'0';
              }
            }
          }
        }
        if(val_item == 2) {
          if(ip != ModemInfo.SrvIP || port != ModemInfo.SrvPort) {
            ModemInfo.SrvIP = ip;
            ModemInfo.SrvPort = port;
            addr = MODEM_INFO + (u8 *)&ModemInfo.SrvIP - (u8 *)&ModemInfo;
            FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, addr, sizeof(ModemInfo.SrvIP)+sizeof(ModemInfo.SrvPort));
          }
          ServerIPCheckFlag = 0;
          Setting.ExtModemMode = HANURI_PROTOCOL;
          SvrSvcError = 0;
          addr = DRV_SETTING_PARM + (u8 *)&Setting.ExtModemMode - (u8 *)&Setting;
          FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));            
          if(answer_flag == 1) {
            SvrEventFlag = 1;
            SvrEventCode = 1;
          }
        }
      }
      break;
      
    case 2:  //Hanuri  MDT  주기보고... DTG주기보고와 관계없음..    &2,300,300,0
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            svr_datatime = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0 || val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        if(svr_datatime != ModemInfo.VehStateInterval) {
          ModemInfo.VehStateInterval = svr_datatime;
          addr = MODEM_INFO + (u8 *)&ModemInfo.VehStateInterval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.VehStateInterval, addr, sizeof(ModemInfo.VehStateInterval));
        }
        if(answer_flag == 1) {
          SvrEventFlag = 1;
          SvrEventCode = 2;
        }
      }
      break;
      
  case 3:  //누적거리(ODO) 설정     &3,123456789,0   -->(123456.789 km) 또는  #SETODOOFFSET 123456,0  -->123456 km)
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            odo_val = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(start == 3) {
          if((odo_val/1000) != (int)DailyData.TTrip) {
            DailyData.TTrip = odo_val/1000;
            FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
            Val.TTrip = DailyData.TTrip;
            Val.TTripOld = Val.TTrip;
          }
          if(answer_flag == 1) {
            SvrEventFlag = 1;
            SvrEventCode = 3;
          }
        }
        else {
          if(odo_val != (int)DailyData.TTrip) {
            DailyData.TTrip = odo_val/1000;
            FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
            Val.TTrip = DailyData.TTrip;
            Val.TTripOld = Val.TTrip;
          }
          if(answer_flag == 1 && msg_delete == 1) {
            memset(RespMsg,0,80);
            sprintf(RespMsg,"ODO : %u km", (int)DailyData.TTrip);
            Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          }
          else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        }
      }      
      break;
      
    case 4:  //Hanuri 단말기 상태 보고..   &4,0
      if(SMSData.Msg[start] >= '0' && SMSData.Msg[start] <= '9') {
        answer_flag = (SMSData.Msg[start]-'0');
        if(answer_flag == 1) {
          SvrEventFlag = 1;
          SvrEventCode = 4;
        }
      }
      break;
      
    case 10: //Hanuri 단말기 Reset..       &10,m2m%eksakffltpt*@,0
      if(IsSameString(&SMSData.Msg[start], "m2m%eksakffltpt*@", 17)) {
        ModemStep = MODEM_POWER_ON;
      }
      break;
      
    case 11: //Hanuri  MDT  주기보고...   &11,300,300,0
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            svr_datatime = val;
          else if(val_item == 2)
            data_savetime_idle = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0 || val_item == 1 || val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }            
          else if(val_item == 3) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 3) {
        if(svr_datatime != ModemInfo.VehStateInterval || data_savetime_idle != ModemInfo.VehStateInterval_Idle) {
          ModemInfo.VehStateInterval = svr_datatime;
          ModemInfo.VehStateInterval_Idle = data_savetime_idle;
          addr = MODEM_INFO + (u8 *)&ModemInfo.VehStateInterval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.VehStateInterval, addr, sizeof(ModemInfo.VehStateInterval)+sizeof(ModemInfo.VehStateInterval_Idle));
        }
        if(answer_flag == 1) {
          SvrEventFlag = 1;
          SvrEventCode = 32;
        }
      }
      break;
      
    case 21: // DTG  주기보고...   &21,180,0  또는 #SETSVRINTERVAL 180,0
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            dtg_datatime = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0 ) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(dtg_datatime != ModemInfo.DataServerInterval) {
          ModemInfo.DataServerInterval = dtg_datatime;
          addr = MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, addr, sizeof(ModemInfo.DataServerInterval));
        }
        if(answer_flag == 1) {
          if(start == 4) {
            SvrEventFlag = 1;
            SvrEventCode = 201;
          }
          else if(msg_delete == 1) {
            memset(RespMsg,0,80);
            sprintf(RespMsg,"Data Sever Interval %u sec", ModemInfo.DataServerInterval);
            Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          }
          else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        }
      }
      break;
      
    case 22:  //교정인자 설정   RPM Factor, K-Factor , Brake Factor  &22,200,5096,1,0  또는  #SETRVFACTOR 200,5096,1,0 
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            rpm_factor = val;
          else if(val_item == 1)
            k_factor = val;
          else if(val_item == 2)
            brake_factor = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item < 3 ) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }            
          else if(val_item == 3) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 3) {
        if(rpm_factor != Setting.RPM_Factor || k_factor != Setting.K_Factor || brake_factor  != Setting.Brake_Type ) {
          if(rpm_factor > 0)
            Setting.RPM_Factor = rpm_factor;
          if(k_factor > 0)
            Setting.K_Factor = k_factor;
//          if(brake_factor > 0)
//            Setting.Brake_Type = brake_factor;
          addr = DRV_SETTING_PARM + (u8 *)&Setting.K_Factor - (u8 *)&Setting;
          FRAMMultiWrite((u8 *)&Setting.K_Factor, addr, sizeof(Setting.K_Factor)+sizeof(Setting.RPM_Factor));
//          addr = DRV_SETTING_PARM + (u8 *)&Setting.Brake_Type - (u8 *)&Setting;
//          FRAMMultiWrite((u8 *)&Setting.Brake_Type, addr, sizeof(Setting.Brake_Type));
        }
        if(answer_flag == 1) {
          if(start == 4) {
            SvrEventFlag = 1;
            SvrEventCode = 203;
          }
          else if(msg_delete == 1) {
            memset(RespMsg,0,80);
            sprintf(RespMsg,"RPM Factor %u K-Factor %u", Setting.RPM_Factor, Setting.K_Factor);
            Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          }
          else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        }
      }
      break;
      
    case 23: // 차량 정보    VRN,VIN,DriverCode,Company_No     &23,경기11가1111,12345678901234567,112233441234567890,1234567890,0
                                                                                         //또는 #SETVEHINFO 경기11가1111,12345678901234567,112233441234567890,1234567890,0
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          val_item++;
          val = 0;
          val_cnt = 0;
        }     
        else {
          if(val_item == 0 ) {
            if(val_cnt < 12) 
              car_reg_no[val_cnt++] =  SMSData.Msg[i];
          }            
          else if(val_item == 1 ) {
            if(val_cnt < 17) 
              car_vin[val_cnt++] =  SMSData.Msg[i];
          }            
          else if(val_item == 2 ) {
            if(val_cnt < 18) 
              drv_code[val_cnt++] =  SMSData.Msg[i];
          }            
          else if(val_item == 3 ) {
            if(val_cnt < 10) 
              company_reg_no[val_cnt++] =  SMSData.Msg[i];
          }            
          else if(val_item == 4) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 4) {
        if(car_reg_no[0] > 0) {
          if((car_reg_no[0] >= '0' && car_reg_no[0] <= '9')  && (car_reg_no[1] >= '0' && car_reg_no[1] <= '9')){
            val_cnt = 0;                              //가나다...
            for(i = 0; i < sizeof(Car_No_Class)/2; i++) {
              if(car_reg_no[2] == Car_No_Class[i][0] && car_reg_no[3] == Car_No_Class[i][1]) {
                kidx = i;
                val_cnt++;
                break;
              }
            }
                                         //car no.2
            for(i = 0; i < 4; i++)
              if(car_reg_no[4+i] >= '0' && car_reg_no[4+i] <= '9')
                val_cnt++;
            if(val_cnt == 5) {
              Setting.Car_Reg_No[0] = 0;
              Setting.Car_Reg_No[1] = 0; 
              Setting.Car_Reg_No[2] = car_reg_no[0]-0x30;  
              Setting.Car_Reg_No[3] = car_reg_no[1]-0x30;  
              Setting.Car_Reg_No[4] = kidx;  
              Setting.Car_Reg_No[5] = car_reg_no[4]-0x30;  
              Setting.Car_Reg_No[6] = car_reg_no[5]-0x30;  
              Setting.Car_Reg_No[7] = car_reg_no[6]-0x30;  
              Setting.Car_Reg_No[8] = car_reg_no[7]-0x30;
              addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_Reg_No - (u8 *)&Setting;
              FRAMMultiWrite(Setting.Car_Reg_No, addr, sizeof(Setting.Car_Reg_No)-3);
              memcpy(VRN, car_reg_no, 12);
            }
          }
          else {   //지역번호 있을때
                                    //지역번호
            val_cnt = 0;
            for(i = 0; i < sizeof(Car_No_Region)/4; i++) {
              if(IsSameString((char*)&car_reg_no[0], (char*)Car_No_Region[i],4)) {
                val_cnt++;
                ridx = i;
                break;
              }
            }
            for(i = 0; i < 2; i++)
              if(car_reg_no[4+i] >= '0' && car_reg_no[4+i] <= '9')
                val_cnt++;
            
            for(i = 0; i < sizeof(Car_No_Class)/2; i++) {
              if(car_reg_no[6] == Car_No_Class[i][0] && car_reg_no[7] == Car_No_Class[i][1]) {
                kidx = i;
                val_cnt++;
                break;
              }
            }
                                         //car no.2
            for(i = 0; i < 4; i++)
              if(car_reg_no[8+i] >= '0' && car_reg_no[8+i] <= '9')
                val_cnt++;

            if(val_cnt == 8) {
              Setting.Car_Reg_No[0] = 0;
              Setting.Car_Reg_No[1] = ridx; 
              Setting.Car_Reg_No[2] = car_reg_no[4]-0x30;  
              Setting.Car_Reg_No[3] = car_reg_no[5]-0x30;  
              Setting.Car_Reg_No[4] = kidx;  
              Setting.Car_Reg_No[5] = car_reg_no[8]-0x30;  
              Setting.Car_Reg_No[6] = car_reg_no[9]-0x30;  
              Setting.Car_Reg_No[7] = car_reg_no[10]-0x30;  
              Setting.Car_Reg_No[8] = car_reg_no[11]-0x30;
              addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_Reg_No - (u8 *)&Setting;
              FRAMMultiWrite(Setting.Car_Reg_No, addr, sizeof(Setting.Car_Reg_No)-3);
              memcpy(VRN, car_reg_no, 12);
            }
          }
        }
        if(car_vin[0] > 0) {
          if(!IsSameString((char*)car_vin, (char*)Setting.Car_No, sizeof(Setting.Car_No))) {
            memcpy(Setting.Car_No, car_vin, sizeof(Setting.Car_No));
            addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_No - (u8 *)&Setting;
            FRAMMultiWrite(Setting.Car_No, addr, sizeof(Setting.Car_No));
          }
        }
        if(drv_code[0] > 0) {
          if(!IsSameString((char*)drv_code, (char*)Setting.Drv_Code, sizeof(Setting.Drv_Code))) {
            memcpy(Setting.Drv_Code, drv_code, sizeof(Setting.Drv_Code));
            addr = DRV_SETTING_PARM + (u8 *)&Setting.Drv_Code - (u8 *)&Setting;
            FRAMMultiWrite(Setting.Drv_Code, addr, sizeof(Setting.Drv_Code));
          }
        }
        if(company_reg_no[0] > 0) {
          if(!IsSameString((char*)company_reg_no, (char*)Setting.Company_Reg_No, sizeof(Setting.Company_Reg_No))) {
            memcpy(Setting.Company_Reg_No, company_reg_no, sizeof(Setting.Company_Reg_No));
            addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Reg_No - (u8 *)&Setting;
            FRAMMultiWrite(Setting.Company_Reg_No, addr, sizeof(Setting.Company_Reg_No));
          }
        }
        if(answer_flag == 1) {
          if(start == 4) {
            SvrEventFlag = 1;
            SvrEventCode = 204;
          }
          else if(msg_delete == 1) {
            memset(RespMsg,0,80);
            kidx = 0;
            if(Setting.Car_Reg_No[1] > 0  && Setting.Car_Reg_No[1] < 18) {
              memcpy(&RespMsg[kidx], Car_No_Region[Setting.Car_Reg_No[1]],4);
              kidx += 4;
            }
            for(i = 2; i < 4; i++) {
              if(Setting.Car_Reg_No[i] <= 9) {
                RespMsg[kidx] = Setting.Car_Reg_No[i] + '0';
                kidx++;
              }
            }
            if(Setting.Car_Reg_No[4] < 51) {
              memcpy(&RespMsg[kidx], Car_No_Class[Setting.Car_Reg_No[4]],2);
              kidx += 2;
            }
            for(i = 5; i < 9; i++) {
              if(Setting.Car_Reg_No[i] <= 9) {
                RespMsg[kidx] = Setting.Car_Reg_No[i] + '0';
                kidx++;
              }
            }
            RespMsg[kidx] =','; kidx++;
            for(i = 0; i < 17; i++) {
              if((Setting.Car_No[i] >= '0'  && Setting.Car_No[i] <= '9') || (Setting.Car_No[i] >= 'A'  && Setting.Car_No[i] <= 'z')) {
                RespMsg[kidx] = Setting.Car_No[i];
                kidx++;
              }
            }
            RespMsg[kidx] =','; kidx++;
            for(i = 0; i < 18; i++) {
              if(Setting.Drv_Code[i] >= '0'  && Setting.Drv_Code[i] <= '9') {
                RespMsg[kidx] = Setting.Drv_Code[i];
                kidx++;
              }
            }
            RespMsg[kidx] =','; kidx++;
            for(i = 0; i < 10; i++) {
              if(Setting.Company_Reg_No[i] >= '0'  && Setting.Company_Reg_No[i] <= '9') {
                RespMsg[kidx] = Setting.Company_Reg_No[i];
                kidx++;
              }
            }
            Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          }
          else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        }
      }
      break;
      
    case 24:   // Hanuri  DTG 파일 전송 메세지 &24,111.222.111.222,1234,YYMMDD,YYMMDD
      ip = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0 && val_cnt == 3)
            ip += (val<<(val_cnt*8));
          else if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
          val_cnt = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] == '.') {
              ip += (val<<(val_cnt*8));
              val_cnt++;
              val = 0;
            }
            else if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else {
              break;
            }
          }
          else if(val_item == 2 ) {
            if(val_cnt < 18) 
              start_date[val_cnt++] =  SMSData.Msg[i];
          }            
          else if(val_item == 3 ) {
            if(val_cnt < 10) 
              end_date[val_cnt++] =  SMSData.Msg[i];
          }            
        }
      }
      if(val_item == 3) {
        if(ip > 0) {
          DataFileIp = ip;
        }
        if(port >  0) {
          DataFileIp = ip;
        }
        if(start_date[0] > 0) {
          tmp_time.tm_year = (start_date[0]-'0')*10+(start_date[1]-'0');
          tmp_time.tm_mon = (start_date[2]-'0')*10+(start_date[3]-'0');
          tmp_time.tm_mday = (start_date[4]-'0')*10+(start_date[5]-'0');
          tmp_time.tm_hour = 0 ;
          tmp_time.tm_min = 0 ;
          tmp_time.tm_sec = 0 ;
          DataFileStart = RTCValTotime(tmp_time);
          DataFileSendFlag = 1;
        }
        if(end_date[0] >  0) {
          tmp_time.tm_year = (end_date[0]-'0')*10+(end_date[1]-'0');
          tmp_time.tm_mon = (end_date[2]-'0')*10+(end_date[3]-'0');
          tmp_time.tm_mday = (end_date[4]-'0')*10+(end_date[5]-'0');
          tmp_time.tm_hour = 0 ;
          tmp_time.tm_min = 0 ;
          tmp_time.tm_sec = 0 ;
          DataFileEnd = RTCValTotime(tmp_time);
        }
      }
      break;
      
    case 25:   //Hanuri sever ip setting Hanuri Protocol로 전환   &25,#ipSet#@, , ,111.222.111.222,1234,0
      ip = 0;
      port = 0;
      if(IsSameString(&SMSData.Msg[start], "#ipSet#@", 8)) {
        start += 8+1;
        for(i = start; i < SMSData.MCnt; i++) {
          if(SMSData.Msg[i] == ',') {
            if(val_item == 2 && val_cnt == 3)
              ip += (val<<(val_cnt*8));
            else if(val_item == 3)
              port = val;
            val_item++;
            val = 0;
            val_cnt = 0;
          }     
          else {
            if(val_item == 2) {
              if(SMSData.Msg[i] == '.') {
                ip += (val<<(val_cnt*8));
                val_cnt++;
                val = 0;
              }
              else if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  val = val*10+(SMSData.Msg[i]-'0');
              }
              else {
                break;
              }
            }            
            else if(val_item == 3) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  val = val*10+(SMSData.Msg[i]-'0');
              }
              else {
                break;
              }
            }
            else if(val_item == 4) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  answer_flag = (SMSData.Msg[i]-'0');
              }
            }
          }
        }
        if(val_item == 4) {
          if(ip != ModemInfo.SrvIP || port != ModemInfo.SrvPort) {
            ModemInfo.SrvIP = ip;
            ModemInfo.SrvPort = port;
            addr = MODEM_INFO + (u8 *)&ModemInfo.SrvIP - (u8 *)&ModemInfo;
            FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, addr, sizeof(ModemInfo.SrvIP)+sizeof(ModemInfo.SrvPort));
          }
          ServerIPCheckFlag = 0;
          Setting.ExtModemMode = HANURI_PROTOCOL;
          SvrSvcError = 0;
          addr = DRV_SETTING_PARM + (u8 *)&Setting.ExtModemMode - (u8 *)&Setting;
          FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));            
          if(answer_flag == 1) {
            SvrEventFlag = 1;
            SvrEventCode = 1;
          }
        }
      }
      break;      
    case 100: //#LOOPMODE 211.235.246.5:7777,1
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ':' || SMSData.Msg[i] == ',') {
          if(val_item == 0 && val_cnt == 3)
            ip += (val<<(val_cnt*8));
          else if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
          val_cnt = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] == '.') {
              ip += (val<<(val_cnt*8));
              val_cnt++;
              val = 0;
            }
            else if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ){
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        ModemInfo.LpIP = ip;
        ModemInfo.LpPort = port;
        ServerIPCheckFlag = 0;
        addr = MODEM_INFO + (u8 *)&ModemInfo.LpIP - (u8 *)&ModemInfo;
        FRAMMultiWrite((u8 *)&ModemInfo.LpIP, addr, sizeof(ModemInfo.LpIP)+sizeof(ModemInfo.LpPort));
        Setting.ExtModemMode = LOOP_PROTOCOL; 
        SvrSvcError = 0;
        memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
        sprintf(HostSvr.SvrName,"%u.%u.%u.%u",(ModemInfo.LpIP%0x100), ((ModemInfo.LpIP>>8)%0x100), ((ModemInfo.LpIP>>16)%0x100), ((ModemInfo.LpIP>>24)%0x100));
        HostSvr.SvrPort = ModemInfo.LpPort;
        FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
        SendSettingFlag = 1;
        if(answer_flag == 1) {
          addr = DRV_SETTING_PARM + (u8 *)&Setting.ExtModemMode - (u8 *)&Setting;
          FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));                    
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"LOOP MODE %u.%u.%u.%u:%u , %u", (ModemInfo.LpIP%0x100), ((ModemInfo.LpIP>>8)%0x100), ((ModemInfo.LpIP>>16)%0x100), ((ModemInfo.LpIP>>24)%0x100), ModemInfo.LpPort, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
        }
        else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }
      break;      
      
    case 110:  //#SETPTMODE 2,1  --> GVN Mode
      //1. 0 : loop protocol
      //   1 : hanuri
      //   2 : gvn
      //   3 : vdis
      //2. 0 : no save
      //   1 : save
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            ext_mdm_mode = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(ext_mdm_mode != Setting.ExtModemMode && ext_mdm_mode <= MAX_EXT_MODEM_PROTOCOL) {
          Setting.ExtModemMode = ext_mdm_mode;
        }
        if(answer_flag == 1) {
          addr = DRV_SETTING_PARM + (u8 *)&Setting.ExtModemMode - (u8 *)&Setting;
          FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));
          ModemInfo.ServerChange = 1;
          FRAMMultiWrite((u8 *)&ModemInfo.ServerChange, MODEM_INFO + (u8 *)&ModemInfo.ServerChange - (u8 *)&ModemInfo, sizeof(ModemInfo.ServerChange));          
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"Ext. Modem Mode %u, %u", Setting.ExtModemMode, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
        }
        else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;      
      
    case 120:   //#SETGVNSVR #svrSet#@ HTTP://14.63.248.9:80/TFM_CT/LOOP000001/vid-rp,1
      //1. data path
      //2. 0 : 기존서버 처리 안함
      //   1 : 기존서버 완료 후 변경
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;
      memset(MdmTmpString,0,100);
      val_cnt = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"GVN Server %s, %u", MdmTmpString, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          Modem_SMS_Delete(SMS_No);
        }
        else if(msg_delete == 0){RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        if(val_cnt > 5 && val_cnt < 100 && answer_flag < 2) {
          ServerDomain_Process_GVN(MdmTmpString, val_cnt, answer_flag);
        }
      }      
      break;

    case 130: //#SETDMSSVR #svrSet#@ devicefota-tb-dm.show.co.kr,8002,0 
      //1. data path
      //2. port
      //3. 0 : 변경
      //   1 : 변경후 리셋
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;
      memset(MdmTmpString,0,100);
      val_cnt = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        if(val_cnt > 5 && val_cnt < 100 && port > 0 && port < 100000) {
          memset(DmSvr.SvrName, 0, sizeof(DmSvr.SvrName));
          memcpy(DmSvr.SvrName, MdmTmpString, val_cnt);
          DmSvr.SvrPort = port;
          FRAMMultiWrite((u8 *)&DmSvr, DMS_SVR_INFO, sizeof(tsSvrInfo));
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"DM Server %s:%d, %u", DmSvr.SvrName, DmSvr.SvrPort, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          Modem_SMS_Delete(SMS_No);
          if(answer_flag == 1) {
            NVIC_SystemReset();
          }
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;
      
    case 140:  //#SETQUALSVR #svrSet#@ devicefota-tb-quality.show.co.kr,8002,0 
      //1. data path
      //2. port
      //3. 0 : 변경
      //   1 : 변경후 리셋
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;
      memset(MdmTmpString,0,100);
      val_cnt = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        if(val_cnt > 5 && val_cnt < 100 && port > 0 && port < 100000) {
          memset(QualSvr.SvrName, 0, sizeof(QualSvr.SvrName));
          memcpy(QualSvr.SvrName, MdmTmpString, val_cnt);
          QualSvr.SvrPort = port;
          FRAMMultiWrite((u8 *)&QualSvr, QUAL_SVR_INFO, sizeof(tsSvrInfo));
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"QUAL Server %s:%d, %u", QualSvr.SvrName, QualSvr.SvrPort, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          Modem_SMS_Delete(SMS_No);
          if(answer_flag == 1) {
            NVIC_SystemReset();
          }
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;
      
    case 150:  
                    //#REQRESET #reqReset#@  0,0 --> Reset MODEM Only  
                    //#REQRESET #reqReset#@  1,0 --> Reset DTG+MODEM,  
                    //#REQRESET #reqReset#@  1,1 --> SentAddr=SaveAddr and Reset DTG+MODEM
      // #reqReset#@ : password
      //1. 0 : reset MODEM RESET
      //   1 : reset MODEM+DTG
      //2. 0 : No Save SentAddr 
      //.  1 : Save SentAddr = DataAddr (only for  MODEM+DTG reset  1,1)
      //.  2 : Save VdisAddr = DataAddr (only for  MODEM+DTG reset  1,1)
      //.  3 : Save SentAddr = VdisAddr = DataAddr (only for  MODEM+DTG reset  1,1)
      if(!IsSameString(&SMSData.Msg[start+1], "#reqReset#@", 11)) 
        break;
      start += 12+1;
      reset_type = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 0)
            reset_type = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"RESET %u, %u", reset_type, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          if(reset_type == 1) Modem_SMS_Delete(SMS_No);
          if(reset_type == 0) {
            if(answer_flag < 2) 
              Modem_ResetProcess();
          }
          else if(reset_type == 1) {
            if(answer_flag == 1 || answer_flag == 3) {
              DataSentAddr = DataSaveAddr;
              FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
            }
            if(answer_flag == 2 || answer_flag == 3) {
              VdisSentAddr = DataSaveAddr;
              FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));     
            }
            NVIC_SystemReset();
          }
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }  
      break;  
    case 160:  //#SETAPNNAME #svrSet#@ apnname.com,0 
      //1. APN 이름.
      //2. 0 : Jig식 APN변경 백업없음...
      //   1 : CLI 방식 APN변경 백업있음..
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;
      memset(MdmTmpString,0,100);
      val_cnt = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(answer_flag == 0) {
          memset((u8*)APN_Name, 0, sizeof(APN_Name));
          memcpy((u8*)APN_Name, MdmTmpString, val_cnt);
          APNProcess(1);  
        }
        else if(answer_flag == 1) {
          memset((u8*)ComRxAPN_Name, 0, sizeof(ComRxAPN_Name));
          memcpy((u8*)ComRxAPN_Name, MdmTmpString, val_cnt);
          APNProcess(2);  
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          if(answer_flag == 0)
            sprintf(RespMsg,"Req. APN %s, %u", APN_Name, answer_flag);
          else
            sprintf(RespMsg,"Req. APN %s, %u", ComRxAPN_Name, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;        
    case 170:  //#SETHOSTSVR #svrSet#@ lb.loop-iot.com,80,0 
      //1. Server Name
      //2. Port
      //3. 0 : 변경
      //   1 : 변경 && DataSentAddr = DataSaveAddr and reset
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;
      memset(MdmTmpString,0,100);
      val_cnt = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        if(val_cnt > 5 && val_cnt < 100 && port > 0 && port < 100000) {
          memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
          memcpy(HostSvr.SvrName, MdmTmpString, val_cnt);
          HostSvr.SvrPort = port;
          FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
          SendSettingFlag = 1;
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"Host Server %s, %u, %u", HostSvr.SvrName, HostSvr.SvrPort, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          if(answer_flag == 1) {
            DataSentAddr = DataSaveAddr;
            FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr)); 
            NVIC_SystemReset();
          }
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;            
    case 180:  //#SETVDISSVR #svrSet#@ vslb.ts2020.kr,80,1,1 
      //1. Server Name
      //2. Port
      //3. VDIS_MODE 0: NOT_USE_VDIS, 1: VDIS_BLE_MODE, 2 : VDIS_DTG_MODE,  VdisSentAddr = DataSaveAddr 
      //4. 0 : Not Reset
      //   1 : Reset
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;
      memset(MdmTmpString,0,100);
      val_cnt = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                vdis_mode = (SMSData.Msg[i]-'0');
            }
          }
          else if(val_item == 3) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 3) {
        if(val_cnt > 5 && val_cnt < 100 && port > 0 && port < 100000) {
          memset(VdisGetSvr.SvrName, 0, sizeof(VdisGetSvr.SvrName));
          memcpy(VdisGetSvr.SvrName, MdmTmpString, val_cnt);
          VdisGetSvr.SvrPort = port;
          FRAMMultiWrite((u8 *)&VdisGetSvr, VDIS_GET_SVR, sizeof(tsSvrInfo));
          if(vdis_mode < 3) {
            Setting.ExtModemVDIS = vdis_mode;  
            addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemVDIS); 
            FRAMMultiWrite((u8 *)&Setting.ExtModemVDIS, addr, sizeof(Setting.ExtModemVDIS));   
          }
          VdisSentAddr = DataSaveAddr;
          FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr));     
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"VDIS Server %s, %u, %u, %u", VdisGetSvr.SvrName, VdisGetSvr.SvrPort, Setting.ExtModemVDIS, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          if(answer_flag == 1) {
            NVIC_SystemReset();
          }
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;      
      
    case 185:  //#SETADASSVR #svrSet#@ 14.63.151.156,44005,1 
      //1. Servername:port
      //2. 0 : 일시적 이용 (Fram 저장안함.)
      //   1 : 지속적 이용 (Fram 저장함.)
      memset(MdmTmpString,0,100);
      start += 1;
      val_cnt = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ':' || SMSData.Msg[i] == ',') {
          if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        if(val_cnt > 5 && val_cnt < 100 && port > 0 && port < 100000) {
          memset(AdasSvr.SvrName, 0, sizeof(AdasSvr.SvrName));
          memcpy(AdasSvr.SvrName, MdmTmpString, val_cnt);
          AdasSvr.SvrPort = port;
          if(answer_flag == 1)
            FRAMMultiWrite((u8 *)&AdasSvr, ADAS_SVR_INFO, sizeof(tsSvrInfo));
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"ADAS Server %s, %u, %u", AdasSvr.SvrName, AdasSvr.SvrPort, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break; 
      
  case 190:   //#LOOPFOTA lb.loop-ota.com:7777,1
      //1. Servername:port
      //2. 0 : 일시적 이용 (Fram 저장안함.)
      //   1 : 지속적 이용 (Fram 저장함.)
      memset(MdmTmpString,0,100);
      start += 1;
      val_cnt = 0;
      port = 0;
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ':' || SMSData.Msg[i] == ',') {
          if(val_item == 1)
            port = val;
          val_item++;
          val = 0;
        }     
        else {
          if(val_item == 0) {
            if(SMSData.Msg[i] != ' '  && val_cnt < 100) {
                MdmTmpString[val_cnt++] = SMSData.Msg[i];
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                val = val*10+(SMSData.Msg[i]-'0');
            }
            else if(SMSData.Msg[i] != ' ' ) {
              break;
            }
          }            
          else if(val_item == 2) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 2) {
        if(val_cnt > 5 && val_cnt < 100 && port > 0 && port < 10000) {
          memset(FWDN_Svr.SvrName, 0, sizeof(FWDN_Svr.SvrName));
          memcpy(FWDN_Svr.SvrName, MdmTmpString, val_cnt);
          FWDN_Svr.SvrPort = port;
          LoopFotaFlag = 2;
          if(answer_flag == 1)
            FRAMMultiWrite((u8 *)&FWDN_Svr, OTA_FWDN_SVR, sizeof(tsSvrInfo));
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg,"Fota Server %s, %u, %u", FWDN_Svr.SvrName, FWDN_Svr.SvrPort, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      }      
      break;      
  case 200:   //#LOOPFWVER
      if(msg_delete == 1) {
        memset(RespMsg,0,80);
        sprintf(RespMsg, "%s %s %s", Model_Name, PCB_HW_Ver, Fota_FW_Ver);
        Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
      }
      else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      break;      
  case 210:   //#REQSTATUS
      if(msg_delete == 1) {
        memset(RespMsg,0,80);
        sprintf(RespMsg, "STATUS %u,%u,%u,%u,%u,%u", Power_Off_Mode, TxEvent_Count, ModemStep, DataServerFlag, GetServerFlag, VdisServerFlag);
        Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
      }
      else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      break;
  case 220: //#SETCOMPANYID LOOP000001,0
      if(!IsSameString(&SMSData.Msg[start+1], "#svrSet#@", 9)) 
        break;
      start += 10+1;      
      for(i = start; i < SMSData.MCnt; i++) {
        if(SMSData.Msg[i] == ',') {
          val_item++;
          val = 0;
          val_cnt = 0;          
        }     
        else {
          if(val_item == 0) {
            if(val_cnt < 10) 
              company_id[val_cnt++] =  SMSData.Msg[i];
              size = val_cnt;
          }            
          else if(val_item == 1) {
            if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                answer_flag = (SMSData.Msg[i]-'0');
            }
          }
        }
      }
      if(val_item == 1) {
        if(answer_flag == 1) {
          if(size == 10) {
            memcpy((u8*)&Setting.Company_Name, company_id, size);      
            addr = DRV_SETTING_PARM + offsetof(st_Setting, Company_Name);
            FRAMMultiWrite((u8*)&Setting.Company_Name, addr, size);
            if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {
              Modem_ChangeURLInfo();
            }          
          }
        }
        if(msg_delete == 1) {
          memset(RespMsg,0,80);
          sprintf(RespMsg, "CompanyID %s, HTTP://vehicle.biz.olleh.com/TFM_CT/%s/vid-rp, %u", Setting.Company_Name, URLInfo.CompanyID, answer_flag);
          Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
        }
        else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}        
      }      
      break;      
  case 250:   //#REQISGPARM
      if(msg_delete == 1) {
        memset(RespMsg,0,80);
        sprintf(RespMsg, "ISGPARM %u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
                                        , IsgSetting.IsgMode, IsgSetting.IdleTimeLimit, IsgSetting.BatVoltLimit
                                        , IsgSetting.CoolTempLimit, IsgSetting.InclineLimit, IsgSetting.PBrakeMode
                                        , IsgSetting.AirPressMode, IsgSetting.NGearMode, IsgSetting.GpsMode, IsgSetting.GeoFenceCnt);
        Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
      }
      else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
      break;      
  case 251:   //#REQISGPOINT 1
      if(msg_delete == 1) {
        memset(RespMsg,0,80);
        for(i = start; i < SMSData.MCnt; i++) {
          if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') 
              val = val*10+(SMSData.Msg[i]-'0');
        }
        if(val < 10) {
          sprintf(RespMsg, "ISGPOINT %u,%u,%u,%u,%u"
                                        , IsgSetting.GeoFenceCnt, val, IsgSetting.GeoFence[val].Lat
                                        , IsgSetting.GeoFence[val].Lon, IsgSetting.GeoFence[val].Radius);
        }
        else sprintf(RespMsg, "ISGPOINT INDEX ERROR");
        Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
      }
      else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
  case 252:   //#SETISGPARM 1,180,230,0,4,0,1,0,1,1,1
      {
        memcpy((u8*)&TmpIsgSetting, (u8*)&IsgSetting, sizeof(stIsgSetting));
        for(i = start; i < SMSData.MCnt; i++) {
          if(SMSData.Msg[i] == ',') {
            if(val_item == 0)      TmpIsgSetting.IsgMode = val;
            else if(val_item == 1) TmpIsgSetting.IdleTimeLimit = val;
            else if(val_item == 2) TmpIsgSetting.BatVoltLimit = val;
            else if(val_item == 3) TmpIsgSetting.CoolTempLimit = val;
            else if(val_item == 4) TmpIsgSetting.InclineLimit = val;
            else if(val_item == 5) TmpIsgSetting.PBrakeMode = val;
            else if(val_item == 6) TmpIsgSetting.AirPressMode = val;
            else if(val_item == 7) TmpIsgSetting.NGearMode = val;
            else if(val_item == 8) TmpIsgSetting.GpsMode = val;
            else if(val_item == 9) TmpIsgSetting.GeoFenceCnt = val;
            val_item++;
            val = 0;
          }     
          else {
            if(val_item < 10 ) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  val = val*10+(SMSData.Msg[i]-'0');
              }
              else if(SMSData.Msg[i] != ' ') {
                break;
              }
            }            
            else if(val_item == 10) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  answer_flag = (SMSData.Msg[i]-'0');
              }
            }
          }
        }
        if(val_item == 10) {
          if(TmpIsgSetting.IsgMode < 2 && TmpIsgSetting.IdleTimeLimit < 999 && TmpIsgSetting.BatVoltLimit < 999 
             && TmpIsgSetting.CoolTempLimit < 99 && TmpIsgSetting.InclineLimit < 99 && TmpIsgSetting.PBrakeMode < 3 
             && TmpIsgSetting.AirPressMode <= 3 && TmpIsgSetting.NGearMode <= 3 && TmpIsgSetting.GpsMode < 2 
             && TmpIsgSetting.GeoFenceCnt < 11 && answer_flag == 1) {
            memcpy((u8*)&IsgSetting, (u8*)&TmpIsgSetting, sizeof(stIsgSetting));
            FRAMMultiWrite((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
          }
          if(msg_delete == 1) {
            memset(RespMsg,0,80);
            sprintf(RespMsg, "ISGPARM %u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
                                            , IsgSetting.IsgMode, IsgSetting.IdleTimeLimit, IsgSetting.BatVoltLimit
                                            , IsgSetting.CoolTempLimit, IsgSetting.InclineLimit, IsgSetting.PBrakeMode
                                            , IsgSetting.AirPressMode, IsgSetting.NGearMode, IsgSetting.GpsMode, IsgSetting.GeoFenceCnt);
            Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          }
          else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        }      
      }
      break;            
  case 253:   //#SETISGPOINT 1,0,37393597,126970410,300,1
      {
        u8 pidx;
        memcpy((u8*)&TmpIsgSetting, (u8*)&IsgSetting, sizeof(stIsgSetting));
        for(i = start; i < SMSData.MCnt; i++) {
          if(SMSData.Msg[i] == ',') {
            if(val_item == 0) TmpIsgSetting.GeoFenceCnt = val;
            if(val_item == 1) pidx = val;
            if(val_item == 2 && pidx < 10) TmpIsgSetting.GeoFence[pidx].Lat = val;
            if(val_item == 3 && pidx < 10) TmpIsgSetting.GeoFence[pidx].Lon = val;
            if(val_item == 4 && pidx < 10) TmpIsgSetting.GeoFence[pidx].Radius = val;
            val_item++;
            val = 0;
          }     
          else {
            if(val_item < 5 ) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  val = val*10+(SMSData.Msg[i]-'0');
              }
              else if(SMSData.Msg[i] != ' ') {
                break;
              }
            }            
            else if(val_item == 5) {
              if(SMSData.Msg[i] >= '0' && SMSData.Msg[i] <= '9') {
                  answer_flag = (SMSData.Msg[i]-'0');
              }
            }
          }
        }
        if(val_item == 10 && pidx < 10) {
          if(TmpIsgSetting.GeoFenceCnt < 11 && TmpIsgSetting.GeoFence[pidx].Lat < 40000000 && TmpIsgSetting.GeoFence[pidx].Lon < 132000000  
             && TmpIsgSetting.GeoFence[pidx].Radius < 1000 && answer_flag == 1) {
            memcpy((u8*)&IsgSetting, (u8*)&TmpIsgSetting, sizeof(stIsgSetting));
            FRAMMultiWrite((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
          }
          if(msg_delete == 1) {
            memset(RespMsg,0,80);
            sprintf(RespMsg, "ISGPOINT %u,%u,%u,%u,%u"
                                            , IsgSetting.GeoFenceCnt, pidx, IsgSetting.GeoFence[pidx].Lat
                                            , IsgSetting.GeoFence[pidx].Lon, IsgSetting.GeoFence[pidx].Radius);
            Modem_SMSSend(SMSData.RcvNum,RespMsg, StringLen(RespMsg,80));
          }
          else if(msg_delete == 0) {RxSmsCnt++; memcpy(RcvNum, SMSData.RcvNum, sizeof(RcvNum));}
        }      
      }
      break;            
  }
  if(msg_delete == 1) {
    Modem_SMS_Delete(SMS_No);
  }
  else if(msg_delete == 2)
    Modem_SMS_DeleteAll();
}



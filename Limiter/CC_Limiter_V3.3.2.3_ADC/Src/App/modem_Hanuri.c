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
#define MODEM_HANURI_GLOBALS
#include "includes.h"
#include "modem_Hanuri.h"

#define MSG_ID_TERMINAL_INFO 0x01
#define MSG_ID_DTG                   0x02
#define MSG_ID_MDT                   0x03
#define MSG_ID_VEHICLE_STATE 0x04
#define MSG_ID_FW_UPGRADE    0x05
#define MSG_ID_FW_UP_RESP    0x06
#define MSG_ID_DTG_FILE_INFO  0x07
#define MSG_ID_DTG_FILE           0x08
#define MSG_ID_MSG_RESP        0xFF


tsSvrMsgHeader  SvrMsgHeader;
tsSvrDTGHeader  SvrDTGHeader;
tsSvrDTGData  SvrDTGData;
tsSvrVehState  SvrVehState;
tsSecData ComSecData;

u8 HanuriRxStep;
u32 HanuriRxCnt, HanuriRxDataLen;
tsSvrMsgHeader  HanuriRxHeader;
u8 HanuriRxResp, HanuriRxMarker, HanuriRxRespErrorCnt;

u8 Data_Tx_Hanuri_Continue;


void Modem_RxDecode_Hanuri(void)
{
  if(HanuriRxResp == 1) {
    if(ModemStep == MODEM_DATA_SERVER) {
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
      printk("\r\nMODEM : Ack SentAddr(%u) / DataAddr(%u) = %u\r\n", DataSentAddr, DataAddrToSend, DataAddrToSend - DataSentAddr);
      ModemStep = MODEM_DATA_SERVER; 
      DataSvrStep = DATA_SVR_CLOSE;
      DataSvrWaitCount = 0;
    }
    else if(ModemStep == MODEM_ON_NOTI) {
       OnNotiWaitCount = 0;
       OnNotiStep = ON_NOTI_CLOSE;
    }
    HanuriRxRespErrorCnt = 0;
  }           
  else if(HanuriRxResp == 101){
    SvrSvcError = 1;
  }
  else if(HanuriRxResp == 102){
    HanuriRxRespErrorCnt++;
    if(HanuriRxRespErrorCnt >= 3)
      SvrSvcError = 1;
  }
  else if(HanuriRxResp == 106) {
    HanuriRxRespErrorCnt = 0;
  }
  else {
    HanuriRxRespErrorCnt = 0;
  }
}


int Modem_RxChar_Hanuri(unsigned char r)
{

  HanuriRxCnt++;
 // printk("step%d %d", HanuriRxStep, HanuriRxCnt);
  switch(HanuriRxStep) {
    case 0:
      if(HanuriRxCnt == 1)
        HanuriRxHeader.PrtocolId = r*10;
      else if(HanuriRxCnt == 2) {
        HanuriRxHeader.PrtocolId += r;
        //if(HanuriRxHeader.PrtocolId == 0x0002) {
          HanuriRxStep = 1;
        //}
      }
      else
        HanuriRxCnt = 0;
      break;
      
    case 1:
      if(HanuriRxCnt == 3) {// && r == MSG_ID_MSG_RESP) {
        HanuriRxHeader.MsgId = r;
        HanuriRxStep = 2;
      }
      else{
        HanuriRxCnt = 0;
        HanuriRxStep = 0;
      }
      break;
      
    case 2:
      if(HanuriRxCnt == 4) {
        HanuriRxHeader.SvcId = r;
        if(HanuriRxHeader.SvcId == 0x02)
          HanuriRxStep = 3;
      }
      else{
        HanuriRxCnt = 0;
        HanuriRxStep = 0;
      }
      break;
      
    case 3:
      if(HanuriRxCnt > 4 && HanuriRxCnt < 8) {
        HanuriRxHeader.MsgLen[HanuriRxCnt-5] = r;
        if(HanuriRxCnt == 7) {
          HanuriRxDataLen = (HanuriRxHeader.MsgLen[0]<<16)+(HanuriRxHeader.MsgLen[1]<<8)+HanuriRxHeader.MsgLen[2];
          HanuriRxMarker = HanuriRxDataLen&0x01;
          HanuriRxDataLen = HanuriRxDataLen>>1;
          if(HanuriRxDataLen == 1)
            HanuriRxStep = 4;
        }
      }
      else{
        HanuriRxCnt = 0;
        HanuriRxStep = 0;
      }
      break;
      
    case 4:
      if(HanuriRxCnt == 8) {
        if(HanuriRxDataLen == 1) {// && HanuriRxMarker == 1) {
          HanuriRxResp = r;
          Modem_RxDecode_Hanuri();  
        }
      }
      HanuriRxCnt = 0;
      HanuriRxStep = 0;
      break;
      
  }
  
  return 0;
}

#define DATA_COUNT_LIMIT_2HOUR  7200  // 3600*2

u8 MsgTxBuff[128];
int Modem_TxData_Hanuri(u8 msg_id, u8 marker)
{
  u32 msg_len, data_cnt, i, data_total, data_cycle, n, data_cycle_max;
  u8 data_size, scode;
  
  ModemCommInit();
  Urc.Mode = URCMODE_HANURI;
  Urc.RespFlag = 1;
  Urc.Cnt = 0;
  
  HanuriRxCnt = 0;
  HanuriRxStep = 0;
  switch(msg_id) {
    case MSG_ID_TERMINAL_INFO:
      memset(MsgTxBuff, 0, 128);
      sprintf((char*)MsgTxBuff,"%s",Model_Name);  
      memcpy(&MsgTxBuff[90],ModemVer , 7);  
      memcpy(&MsgTxBuff[40],Setting.Serial_No , sizeof(Setting.Serial_No));  
      sprintf((char*)&MsgTxBuff[80],"%1u.%1u.%1u.%1u", MAJOR,MINOR,RELEASE,BUILD);  
      memcpy(&MsgTxBuff[90],&ModemVer[7] , 5);  
      memcpy(&MsgTxBuff[100],ModemCTN , 11);  
      memcpy(&MsgTxBuff[111],VRN , 12);  
      SvrMsgHeader.PrtocolId = 0x0200;
      SvrMsgHeader.SvcId = 0x02;
      SvrMsgHeader.MsgId = MSG_ID_TERMINAL_INFO;
      msg_len = (123<<1)+marker;
      SvrMsgHeader.MsgLen[0] = (msg_len>>16)&0xff;
      SvrMsgHeader.MsgLen[1] = (msg_len>>8)&0xff;
      SvrMsgHeader.MsgLen[2] = msg_len&0xff;
      TxDataStrToModem((u8*)&SvrMsgHeader, 7);
      TxDataStrToModem(MsgTxBuff, 123);
        printk("\r\nMODEM : Marker : %u Terminal Info\n",marker);
      break;
      
    case MSG_ID_DTG:
      Data_Tx_Hanuri_Continue = 0;
      data_total = 0;
      if(DataAddrToSend >= DataSentAddr) 
        data_total = (DataAddrToSend-DataSentAddr)/sizeof(tsSecData);
      else 
        data_total = (NAND_SIZE_DATA+DataAddrToSend-DataSentAddr)/sizeof(tsSecData);

      if(data_total > DATA_COUNT_LIMIT_2HOUR) {
        if(DataAddrToSend >= DataSentAddr) 
          DataSentAddr = DataAddrToSend - DATA_COUNT_LIMIT_2HOUR*sizeof(tsSecData);
        else
          DataSentAddr = (NAND_SIZE_DATA + DataAddrToSend - DATA_COUNT_LIMIT_2HOUR*sizeof(tsSecData))%NAND_SIZE_DATA;
        FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
        data_total = DATA_COUNT_LIMIT_2HOUR;
      }
      
      data_cycle = data_total/ModemInfo.DataServerInterval;
      if(data_total%ModemInfo.DataServerInterval) data_cycle++;
      
      if(data_cycle > 5) {
        data_cycle_max = 5;
        Data_Tx_Hanuri_Continue = 1;
      }
      else 
        data_cycle_max = data_cycle;
      for(n = 1; n <= data_cycle_max; n++) {
        if(n == data_cycle) {
          if(data_total%ModemInfo.DataServerInterval)
            data_cnt = data_total%ModemInfo.DataServerInterval;
          else
            data_cnt = ModemInfo.DataServerInterval;
        }
        else {
          data_cnt = ModemInfo.DataServerInterval;
        }
        if(n == data_cycle_max)
          marker = 1;
        else
          marker = 0;
        if(data_cnt > 0) {
          Nand_Read_SecData((u8*)&ComSecData, DataSentAddr, sizeof(tsSecData)); 
          SvrDTGHeader.Tid = SwapEndian32(ModemInfo.CNUM);
          SvrDTGHeader.TripCount = ComSecData.TripCount;
          if(Setting.DTG_Type == DTG_GREEN){
            data_size = sizeof(tsSvrDTGData);
            SvrDTGHeader.Option[0] = 0x80;
          }
          else
            data_size = sizeof(tsSvrDTGData)-12;
          SvrDTGHeader.Option[1] = 0;
          SvrDTGHeader.Option[2] = 0;
          sprintf((char*)SvrDTGHeader.Model,"%s",Model_Name);  
          memcpy(SvrDTGHeader.VIN,Setting.Car_No, 17);  
          SvrDTGHeader.VehicleType = Car_Type_Code[Setting.Car_Reg_No[0]];
          memcpy(SvrDTGHeader.VRN,VRN, 12);  
          memcpy(SvrDTGHeader.BRN,Setting.Company_Reg_No, 10);  
          memcpy(SvrDTGHeader.DCode,Setting.Drv_Code, 18);  
          SvrDTGHeader.DataCount = SwapEndian16(data_cnt);
          
          SvrMsgHeader.PrtocolId = 0x0200;
          SvrMsgHeader.SvcId = 0x02;
          SvrMsgHeader.MsgId = MSG_ID_DTG;
          msg_len = sizeof(tsSvrDTGHeader)+data_cnt*data_size;
          msg_len = (msg_len<<1)+marker;
          SvrMsgHeader.MsgLen[0] = (msg_len>>16)&0xff;
          SvrMsgHeader.MsgLen[1] = (msg_len>>8)&0xff;
          SvrMsgHeader.MsgLen[2] = msg_len&0xff;
          TxDataStrToModem((u8*)&SvrMsgHeader, 7);
          TxDataStrToModem((u8*)&SvrDTGHeader, sizeof(tsSvrDTGHeader));
          for(i = 0; i < data_cnt; i++) {
            if(i > 0)
              Nand_Read_SecData((u8*)&ComSecData, DataSentAddr, sizeof(tsSecData)); 
            SvrDTGData.Time = SwapEndian32(ComSecData.Time-3600*9);
            //printk("\nTime : %u \n",ComSecData.Time);
            SvrDTGData.MSec = 0;
            SvrDTGData.DTrip = SwapEndian32((u32)(ComSecData.DTrip*1000.0));
            SvrDTGData.TTrip = SwapEndian32((u32)(ComSecData.TTrip*1000.0));
            SvrDTGData.Trip = SwapEndian32((u32)(ComSecData.Trip*1000.0));
            SvrDTGData.Speed = ComSecData.Speed[0];
            SvrDTGData.RPM = SwapEndian16(ComSecData.RPM);
            SvrDTGData.Brake = 0;
            if(ComSecData.Signal.Brake) SvrDTGData.Brake = 0x80;
            //SvrDTGData.Brake = ComSecData.Signal.Brake;
            SvrDTGData.Lon = SwapEndian32(ComSecData.Lon*10);
            SvrDTGData.Lat = SwapEndian32(ComSecData.Lat*10);
            SvrDTGData.Azi = SwapEndian16(ComSecData.Azi);
            SvrDTGData.AccX = SwapEndian16((s16)ComSecData.AccX);
            SvrDTGData.AccY = SwapEndian16((s16)ComSecData.AccY);
            scode = 0;
            if(ComSecData.Status.GPS_Fault)         scode = 11;
            else if(ComSecData.Status.Speed_Fault)  scode = 12;
            else if(ComSecData.Status.RPM_Fault)    scode = 13;
            else if(ComSecData.Status.Brake_Fault)  scode = 14;
            else if(ComSecData.Status.Input_Fault)  scode = 21;
            else if(ComSecData.Status.Output_Fault) scode = 22;
            else if(ComSecData.Status.Save_Fault)   scode = 31;
            else if(ComSecData.Status.Comm_Fault)   scode = 32;
            else if(ComSecData.Status.Dist_Fault)   scode = 41;
            else if(ComSecData.Status.Power_Fault)  scode = 99;
            SvrDTGData.Status = scode;
            SvrDTGData.DFuel = SwapEndian32((u32)(ComSecData.DFuel));
            SvrDTGData.TFuel = SwapEndian32((u32)(ComSecData.TFuel));
            SvrDTGData.Fuel = SwapEndian32((u32)(ComSecData.Fuel));

            TxDataStrToModem((u8*)&SvrDTGData, data_size);
            DataSentAddr = (DataSentAddr+sizeof(tsSecData))%NAND_SIZE_DATA;
          }
        }
        printk("\r\nMODEM : Marker : %u Data Len(%u) / SentAddr(%u) / DataAddr(%u) = %u\n",marker, data_cnt, DataSentAddr, DataAddrToSend, DataAddrToSend - DataSentAddr);
        vTaskDelay(200);
      }
      break;
      
    case MSG_ID_MDT:
      break;
      
    case MSG_ID_VEHICLE_STATE:
      memcpy(SvrVehState.VRN,VRN, 12);  
      SvrVehState.Tid = SwapEndian32(ModemInfo.CNUM);
      SvrVehState.Option[0] = 0;
      SvrVehState.Option[1] = 0;      
      SvrVehState.Option[2] = 0;      
      SvrVehState.Time = SwapEndian32(RTC_LVal-3600*9);
      SvrVehState.MSec = 0;
      
      data_size = sizeof(tsSvrVehState);
      
      if(SvrEventFlag) {
        SvrVehState.EventCode = SwapEndian16(SvrEventCode);
        SvrEventFlag = 0;
        SvrEventCode = 5;
      }
      else
        SvrVehState.EventCode = 0x0500;
      
      SvrMsgHeader.PrtocolId = 0x0200;
      SvrMsgHeader.SvcId = 0x02;
      SvrMsgHeader.MsgId = MSG_ID_VEHICLE_STATE;
      msg_len = (data_size<<1)+marker;
      SvrMsgHeader.MsgLen[0] = (msg_len>>16)&0xff;
      SvrMsgHeader.MsgLen[1] = (msg_len>>8)&0xff;
      SvrMsgHeader.MsgLen[2] = msg_len&0xff;
      TxDataStrToModem((u8*)&SvrMsgHeader, 7);
      TxDataStrToModem((u8*)&SvrVehState, data_size);
    
      printk("\r\nMODEM : Marker : %u Vehicle State\n",marker);
      break;
      
    case MSG_ID_FW_UPGRADE:
      break;
    case MSG_ID_FW_UP_RESP:
      break;
    case MSG_ID_DTG_FILE_INFO:
      break;
    case MSG_ID_DTG_FILE:
      break;
    case MSG_ID_MSG_RESP:
      break;
  }
  return 0;
}

/*
********************************************************************************
* Description : Modem_DataProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/

void Modem_DataSvrProcess_Hanuri(void)
{
  if(!DataServerFlag || Setting.ExtModemMode != HANURI_PROTOCOL) {
    DataSvrStep = DATA_SVR_IDLE;
  }
  
  switch(DataSvrStep) {
    case DATA_SVR_DATACHECK:
      if(DataAddrToSend == DataSentAddr)  //same addr
        DataSvrStep = DATA_SVR_CLOSE;
      else {
        if(TCP_Open == 1){
          DataSvrStep = DATA_SVR_SEND;
        }
        else {
          if(Data_Tx_Hanuri_Continue)
            DataSvrStep = DATA_SVR_SEND_WAIT;
          else {
            DataSvrStep = DATA_SVR_CONNECT;
            SocketConnectStep = SOCKET_CONNECT_START;
            SocketRetryCnt = 0;
          }
        }
      }
      DataSvrWaitCount = 0;
      break;
      
    case DATA_SVR_CONNECT:
      Modem_SocketConnect((char*)&ModemInfo.SrvIP,ModemInfo.SrvPort,1);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          DataSvrStep = DATA_SVR_SEND;
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
      if(DataSvrWaitCount >= 3000) {
        Data_Tx_Hanuri_Continue = 0;
        SocketConnectStep = SOCKET_CONNECT_START;
        SocketRetryCnt = 0;
        DataSvrStep = DATA_SVR_CONNECT;
        VehStateFlag = 0;
      }
      break;
      
    case DATA_SVR_SEND:
      if(TcpState != 1) {
          DataSvrStep = DATA_SVR_FAIL;
          return;
      }
      DataSvrWaitCount = 0;
      
      FRAMMultiRead((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
      
      if(DataAddrToSend == DataSentAddr)
        DataSvrStep = DATA_SVR_CLOSE;
      else {
        if(VehStateFlag) {
          if(DataServerFlag == 2) {
            SvrEventFlag = 1;
            SvrEventCode = 27;
          }
          Modem_TxData_Hanuri(MSG_ID_VEHICLE_STATE, 0);
          vTaskDelay(1000);
        }
        Modem_TxData_Hanuri(MSG_ID_DTG, 1);
        DataSvrStep = DATA_SVR_RECIEVE_WAIT;          
      }
      break;
      
    case DATA_SVR_RECIEVE_WAIT:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 20000) {  //10sec
        DataSvrWaitCount = 0;
        Modem_SocketClose(0);
        if(HanuriRxResp == 200) 
          DataSvrStep = DATA_SVR_FAIL_WAIT;
        else
          DataSvrStep = DATA_SVR_FAIL;
      }
      break;

    case DATA_SVR_FAIL_WAIT:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 60000) {  //10sec
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
      if(DataServerFlag == 2) {   //Power Off Mode
        if(DataServerRetryCount >= 2) {
          if(VdisServerFlag != 2) {
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
          }
          else
            ModemStep = MODEM_DATA_IDLE;
          DataServerRetryCount = 0;
          ModemWaitCnt = 0;
          DataServerFlag = 0;
          VehStateFlag = 0;
        }
        else {
          ModemWaitCnt = 0;
          ModemStep = MODEM_IDLE;
        }
      }
      else {                     
        if(DataServerRetryCount >= 3) {
          ModemStep = MODEM_DATA_IDLE;
          DataServerRetryCount = 0;
          ModemWaitCnt = 0;
          DataServerFlag = 0;
          VehStateFlag = 0;
          Modem_PPPClose();
        }
        else {
          ModemWaitCnt = 0;
          if(DataServerRetryCount == 1)
            ModemStep = MODEM_IDLE;
          else {
            ModemStep = MODEM_POWER_ON_IDLE;
          }
        }
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;      
      COM_Command_Char = 'F';
      break;

    case DATA_SVR_SUCCESS:
      printk("\r\nMODEM : Data Send Sucess\r\n");
      if(Data_Tx_Hanuri_Continue == 0) {      
        if(DataServerFlag == 2) {
          if(VdisServerFlag != 2) {
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
          }
          else
            ModemStep = MODEM_DATA_IDLE;
        }
        else {
          Modem_PPPClose();
          ModemStep = MODEM_DATA_IDLE;
        }
        DataServerFlag = 0;
        VehStateFlag = 0;
      }
      else
        ModemStep = MODEM_DATA_IDLE;
      ModemWaitCnt = 0;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      COM_Command_Char = 'S';
      break;

    case DATA_SVR_IDLE:
      if(DataServerFlag == 2) {
        if(VdisServerFlag != 2) {
          Modem_PPPClose();
          ModemStep = MODEM_POWER_OFF;
        }
        else
          ModemStep = MODEM_DATA_IDLE;
      }
      else
        ModemStep = MODEM_DATA_IDLE;
      ModemWaitCnt = 0;
      DataServerFlag = 0;
      VehStateFlag = 0;
      break;
      
    default : 
      DataSvrStep = DATA_SVR_FAIL;
      break;  
  }
}

/*
********************************************************************************
* Description : Modem_OnNotiProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
void Modem_OnNotiProcess_Hanuri(void)
{

  if(!OnNotiFlag || Setting.ExtModemMode != HANURI_PROTOCOL) {
    OnNotiStep = ON_NOTI_IDLE;
    return;
  }
  switch(OnNotiStep) {
    case ON_NOTI_CONNECT:
      Modem_SocketConnect((char*)&ModemInfo.SrvIP,ModemInfo.SrvPort,1);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          OnNotiStep = ON_NOTI_SEND;
          OnNotiWaitCount = 0;
        }
        else {
          Modem_SocketClose(0);
          OnNotiStep = ON_NOTI_FAIL;
        }
      }
      break;
      
    case ON_NOTI_SEND:
      ModemCommInit();
      Urc.Mode = URCMODE_HANURI;
      Urc.RespFlag = 1;
      Urc.Cnt = 0;
      HanuriRxStep = 0;
      HanuriRxCnt = 0;
      Modem_TxData_Hanuri(MSG_ID_TERMINAL_INFO, 0);
      vTaskDelay(1);
      SvrEventFlag = 1;
      SvrEventCode = 26;
      Modem_TxData_Hanuri(MSG_ID_VEHICLE_STATE, 1);
      OnNotiWaitCount = 0;
      OnNotiRespFlag = 1;
      OnNotiStep = ON_NOTI_RECIEVE_WAIT;
      break;
      
    case ON_NOTI_RECIEVE_WAIT:
      OnNotiWaitCount++;
      if(OnNotiWaitCount > 10000 ) {
        OnNotiWaitCount = 0;
        Modem_SocketClose(0);
        OnNotiStep = ON_NOTI_FAIL;
      }
      break;

    case ON_NOTI_CLOSE:
      if(Modem_SocketClose(0)==1) {
        OnNotiStep = ON_NOTI_SUCCESS;
      }
      else
        OnNotiStep = ON_NOTI_FAIL;
      OnNotiRespFlag = 0;
      break;

    case ON_NOTI_FAIL:
      printk("\r\nMODEM : On-Notification error\r\n");
      ModemWaitCnt = 0;
      OnNotiRespFlag = 0;
      OnNotiRetryCount++;
      if(OnNotiRetryCount >= 3) {
        OnNotiFlag = 0;
        OnNotiRetryCount = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON;
        Modem_PPPClose();
      }
      else {
        ModemWaitCnt = 0;
        if(OnNotiRetryCount == 1)
          ModemStep = MODEM_IDLE;
        else 
          ModemStep = MODEM_POWER_ON_IDLE; 
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case ON_NOTI_SUCCESS:
      printk("\r\nMODEM : On-Notification Success\r\n");
      Modem_PPPClose();
      OnNotiRetryCount = 0;
      OnNotiRespFlag = 0;
      OnNotiFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      ModemWaitCnt = 0;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      COM_Command_Char = 'O';
      break;

    case ON_NOTI_IDLE:
      ModemWaitCnt = 0;
      OnNotiFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      OnNotiStep = ON_NOTI_FAIL;
      break;  
  }
}

u32 Hanuri_SendTimeCnt;
void Modem_SecInt_Hanuri(void)
{
  u32 daddr = 0, data_cnt;

  if(Setting.ExtModemMode != HANURI_PROTOCOL) return;

  if(ModemInfo.DataServerInterval > 9 && ModemInfo.DataServerInterval < 10000) {
    Hanuri_SendTimeCnt++;
    daddr = (u32)(DataSaveAddr-DataSentAddr);
    data_cnt = daddr/sizeof(tsSecData);
    if(Hanuri_SendTimeCnt >= ModemInfo.DataServerInterval && (data_cnt >= ModemInfo.DataServerInterval)) {
      Hanuri_SendTimeCnt = 0;
      if(DataServerFlag == 0) {
        DataServerFlag = 1;
        DataServerRetryCount = 0;
      }
      VehStateFlag = 1;
      DataAddrToSend = DataSaveAddr;
    }
  }
}

void Modem_Parameter_Reset_Hanuri(void)
{
  u8 ip[4];

  ModemInfo.VehStateInterval = 180;
  FRAMMultiWrite((u8 *)&ModemInfo.VehStateInterval, MODEM_INFO + (u8 *)&ModemInfo.VehStateInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.VehStateInterval));          
  ModemInfo.VehStateInterval_Idle = 180;
  FRAMMultiWrite((u8 *)&ModemInfo.VehStateInterval_Idle, MODEM_INFO + (u8 *)&ModemInfo.VehStateInterval_Idle - (u8 *)&ModemInfo, sizeof(ModemInfo.VehStateInterval_Idle));          
  
  ip[0] = 14; ip[1] = 63; ip[2] = 216; ip[3] = 18; 
  
  memcpy(&ModemInfo.SrvIP, ip, 4);
  ModemInfo.SrvPort = 7777;
  FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, MODEM_INFO + (u8 *)&ModemInfo.SrvIP - (u8 *)&ModemInfo, sizeof(ModemInfo.SrvIP)+sizeof(ModemInfo.SrvPort));          
}

void Modem_Init_Hanuri(void)
{
  u8 ip[4];
  
  FRAMMultiRead((u8 *)&ModemInfo, MODEM_INFO, sizeof(tsModemInfo));

  if(ModemInfo.VehStateInterval > 9999) {
    ModemInfo.VehStateInterval = 60;
    FRAMMultiWrite((u8 *)&ModemInfo.VehStateInterval, MODEM_INFO + (u8 *)&ModemInfo.VehStateInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.VehStateInterval));          
  }
  if(ModemInfo.VehStateInterval_Idle > 9999) {
    ModemInfo.VehStateInterval_Idle = 180;
    FRAMMultiWrite((u8 *)&ModemInfo.VehStateInterval_Idle, MODEM_INFO + (u8 *)&ModemInfo.VehStateInterval_Idle - (u8 *)&ModemInfo, sizeof(ModemInfo.VehStateInterval_Idle));          
  }
  if(ModemInfo.SrvIP == 0 || ModemInfo.SrvIP == 0xffffffff) {
    ip[0] = 14; ip[1] = 63; ip[2] = 216; ip[3] = 18; 
    memcpy(&ModemInfo.SrvIP, ip, 4);
    ModemInfo.SrvPort = 7777;
    FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, MODEM_INFO + (u8 *)&ModemInfo.SrvIP - (u8 *)&ModemInfo, sizeof(ModemInfo.SrvIP)+sizeof(ModemInfo.SrvPort));          
  }
  
  OnNotiFlag = 1;
  OnNotiRetryCount = 0;
}

void Modem_DataIdleProcess_Hanuri(void)
{
  if(DataServerFlag) {
    if(SvrSvcError) {
      DataServerFlag = 0;
      VehStateFlag = 0;
    }
    else {
      DataSvrStep = DATA_SVR_DATACHECK;
      DataSvrWaitCount = 0;
      ModemStep = MODEM_DATA_SERVER;    
    }
  }
  else if(OnNotiFlag) {
    OnNotiStep = ON_NOTI_CONNECT;
    OnNotiWaitCount = 0;
    ModemStep = MODEM_ON_NOTI;    
  }
}

/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/
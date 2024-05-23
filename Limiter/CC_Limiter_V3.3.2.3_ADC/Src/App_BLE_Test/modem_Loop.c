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
#define MODEM_LOOP_GLOBALS
#include "includes.h"
#include "modem_Loop.h"

#define MAX_SEND_COUNT_PER_CONNECT 10

tsRpck Gpck;
tsFWIF FW_Info;
u8 crc_same_flag;
int SendCountPerConnect; 

/*
********************************************************************************
* Description : Modem_RxChar
* Arguments   : r
* Return      : 
* Note        : rx modem char
******************************************************************************** 
*/
int Modem_RxChar_Loop(unsigned char r)
{
  //Pck is gloval variable
  //Pck is make a new instance per connection c++
#ifdef AT_ACK_PRINT
  if(r <= 'z')  UART_SendData(DEBUG_UART_PORT, r);
#endif
  unsigned int pstep = 0;
  
  if(Gpck.Step < ST_CRC16)
    Gpck.CalcCRC16 = getcrc16c(Gpck.CalcCRC16, r); // getcrc16b(Gpck.CalcCRC16,r);

  switch(Gpck.Step) {
    case ST_SPK_ST:
      if(r == 0xCC) {
        Gpck.CalcCRC16 = 0;
        Gpck.CalcCRC16 = getcrc16c(Gpck.CalcCRC16, r); //getcrc16b(Gpck.CalcCRC16,r);
        Gpck.Step = ST_SPK_CF;
      }
      break;
    case ST_SPK_CF:
      if(r == 0xCF) {
        Gpck.Header.DeviceID = 0;
        Gpck.Cnt = 0;
        Gpck.Step = ST_DEVICE_ID;
      }
      else {
        pstep = Gpck.Step;
        Gpck.Step = ST_SPK_ST;
      }
      break;
    case ST_DEVICE_ID:
      Gpck.Header.DeviceID += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Header.DeviceID)) {
        Gpck.Cnt = 0;
        Gpck.Header.IdentityNo = 0;
        if(Gpck.Header.DeviceID < DV_MAX) {
          Gpck.Step = ST_IDENTITY_NO;
        }
        else {
          pstep = Gpck.Step;
          Gpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_IDENTITY_NO:
      Gpck.Header.IdentityNo += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Header.IdentityNo)) {
        Gpck.Header.Command = 0;
        Gpck.Cnt = 0;
//        if(Gpck.Header.IdentityNo < 1300000000) {
          Gpck.Step = ST_COMMAND;
//        }
//        else {
//          pstep = Gpck.Step;
//          Gpck.Step = ST_SPK_ST;
//        }
      }
      break;
    case ST_COMMAND:
      Gpck.Header.Command += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Header.Command)) {
        Gpck.Header.Index = 0;
        Gpck.Cnt = 0;
        Gpck.Step = ST_INDEX;
      }
      break;
    case ST_INDEX:
      Gpck.Header.Index += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Header.Index)) {
        Gpck.Header.Count = 0;
        Gpck.Cnt = 0;
        Gpck.Step = ST_COUNT;
//        if(Gpck.Header.Index <= INDEX_MAX) {
//          Gpck.Step = ST_COUNT;
//        }
//        else {
//          pstep = Gpck.Step;
//          Gpck.Step = ST_SPK_ST;
//        }
      }
      break;
    case ST_COUNT:
      Gpck.Header.Count += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Header.Count)) {
        Gpck.Header.Length = 0;
        Gpck.Cnt = 0;
        Gpck.Step = ST_LENGTH;
//        if(Gpck.Header.Count <= COUNT_MAX) {
//          Gpck.Step = ST_LENGTH;
//        }
//        else {
//          pstep = Gpck.Step;
//          Gpck.Step = ST_SPK_ST;
//        }
      }
      break;
    case ST_LENGTH:
      Gpck.Header.Length += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Header.Length)) {
        Gpck.Cnt = 0;
        memset(Gpck.Data,0,sizeof(Gpck.Data));
        if(Gpck.Header.Length <= LENGTH_MAX) {
          Gpck.Step = ST_DATA;
          if(Gpck.Header.Length == 0) {
            Gpck.Cnt = 0;
            Gpck.Footer.CRC16 = 0;
            Gpck.Step = ST_CRC16;
          }          
        }
        else {
          pstep = Gpck.Step;
          Gpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_DATA:
      Gpck.Data[Gpck.Cnt++] = r;
      if(Gpck.Cnt >= Gpck.Header.Length) {
        Gpck.Cnt = 0;
        Gpck.Footer.CRC16 = 0;
        Gpck.Step = ST_CRC16;
      }
      break;
    case ST_CRC16:
      Gpck.Footer.CRC16 += r<<((Gpck.Cnt++)*8);
      if(Gpck.Cnt >= sizeof(Gpck.Footer.CRC16)) {
        if(Gpck.CalcCRC16 == Gpck.Footer.CRC16) {
          Gpck.Step = ST_EPK_ST;//ST_PROCESSING;
        }
        else {
          pstep = Gpck.Step;
          Gpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_EPK_ST:
      if(r == 0xCF) {
        Gpck.Step = ST_EPK_CF;
      }
      else {
        pstep = Gpck.Step;
        Gpck.Step = ST_SPK_ST;
      }
      break;
    case ST_EPK_CF:
      if(r == 0xD0) {
        Gpck.Step = ST_PROCESSING;
        //while
      }
      else {
        pstep = Gpck.Step;
        Gpck.Step = ST_SPK_ST;
      }
      break;
    case ST_PROCESSING:
      //decode after Gpck.Step is initialized
      break;
  }

  Modem_RxDecode_Loop();
  return pstep;
}

/*
********************************************************************************
* Description : Modem_TxData
* Arguments   : ...
* Return      : 
* Note        : tx modem data
******************************************************************************** 
*/
//u8 bufff[200];  
teCommand TxCommand;
void Modem_TxData_Loop(teCommand com, unsigned short idx, unsigned short cnt, unsigned int len, u8* data)
{  
  ModemCommInit();
  Gpck.Step = 0;
  Urc.Mode = URCMODE_LOOP;
  Urc.RespFlag = 1;
  Urc.Cnt = 0;
  if(com == CM_FWIF || com == CM_FWSD) FotaRespFlag = 1;
  else FotaRespFlag = 0;
  
  TxCommand = com;
  tsPckHeader hd;
  tsPckFooter ft;
  hd.SPK = 0xCFCC;
#ifdef ISG_ENABLE  
  hd.DeviceID = DV_ISG;
#else  
  hd.DeviceID = DV_DTG;
#endif  
  hd.IdentityNo = ModemInfo.CNUM ;
  hd.Command = com;//CommandStrToInt(cstr);
  hd.Index = idx;
  hd.Count = cnt;
  hd.Length = len;
  TxDataStrToModem((u8*)&hd, sizeof(hd));  
  TxDataStrToModem(data, len);
  ft.CRC16 = 0;
  ft.CRC16 = getcrc16(ft.CRC16, (u8*)&hd, sizeof(hd));
  ft.CRC16 = getcrc16(ft.CRC16, data, hd.Length); 
  ft.EPK = 0xD0CF;
  TxDataStrToModem((u8*)&ft, sizeof(ft));   
  Gpck.Delay = 0;
}

/*
********************************************************************************
* Description : Modem_RxDecode
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
u8 NandBufferingIdx;
void Modem_RxDecode_Loop(void)
{
  if(Gpck.Step < ST_PROCESSING) return;
  
  static unsigned char crc_same_flag;
  unsigned char sd[10];
  u32 addr =0;
  
  switch(Gpck.Header.Command) {
    case CM_INFO: 
      break;
      
    case CM_FWIF: 
      memcpy(&FW_Info, Gpck.Data, sizeof(tsFWIF));
      sd[0] = 1;
      Modem_TxData_Loop(CM_FWIF, Gpck.Header.Index, Gpck.Header.Count, 1, sd);
      DataSvrWaitCount = 0;
      NandBufferingIdx = 0;
      memset(ModemBuff,0,NAND_PAGE_SIZE);
      break;
      
    case CM_FWSD: 
      {
        u32 naddr = NAND_PROGRAM_ADDR + (Gpck.Header.Index*FW_Info.DivisionSize);        
        u16 nsize = FW_Info.DivisionSize;
        if(Gpck.Header.Index < Gpck.Header.Count) {  
//          Nand_Write_Buffer(Gpck.Data, naddr, nsize);
          NandBufferingIdx = (Gpck.Header.Index%(NAND_PAGE_SIZE/nsize));
          memcpy(&ModemBuff[NandBufferingIdx*nsize], Gpck.Data, nsize);
          if((NandBufferingIdx+1)*nsize >= NAND_PAGE_SIZE || Gpck.Header.Index == (Gpck.Header.Count-1)) {
            naddr = NAND_PROGRAM_ADDR + ((Gpck.Header.Index-NandBufferingIdx)*FW_Info.DivisionSize);
            Nand_Write_Buffer(ModemBuff, naddr, NAND_PAGE_SIZE);
            memset(ModemBuff,0,NAND_PAGE_SIZE);
          }
        }
        else {
          crc_same_flag = 0;
          u16 ccrc=0;
          nsize = FW_Info.Size%FW_Info.DivisionSize;
          //Nand_Write_Buffer(Gpck.Data, naddr, nsize);
          for(int i = 0; i < Gpck.Header.Count; i++) {
            naddr = NAND_PROGRAM_ADDR + i*FW_Info.DivisionSize;
            if(i < Gpck.Header.Count-1) { 
              nsize = FW_Info.DivisionSize; 
              Nand_Read_Buffer(ModemBuff, naddr, nsize);
            }
            else {
              nsize = FW_Info.Size%FW_Info.DivisionSize; 
              Nand_Read_Buffer(ModemBuff, naddr, nsize);              
            }
            ccrc =  getcrc16(ccrc, ModemBuff, nsize);
          }
          if(ccrc == FW_Info.CRC16) {
            crc_same_flag = 1;
          }
        }  
        
        sd[0] = 1;
        Modem_TxData_Loop(CM_FWSD, Gpck.Header.Index, Gpck.Header.Count, 1, sd);      
        DataSvrWaitCount = 0;
      }
      break;
    case CM_FWAK:      
      Modem_TxData_Loop(CM_FWAK, Gpck.Header.Index, Gpck.Header.Count, 1, (u8*)&crc_same_flag); 
      Modem_SocketClose(0);
      if(crc_same_flag) {  
        FW_Info.Size = FW_Info.Size+(4-(FW_Info.Size%4));  //Fota Size를 4의 배수로..
        RTC_WriteBackupRegister(RTC_BKP_DR0, 10);  
        RTC_WriteBackupRegister(RTC_BKP_DR1, FW_Info.Size);         
                
        lcd_clear();
        lcd_printf(4,1,0,"Found New Ver");
        lcd_printf(4,2,0,"System Reset");   
        lcd_printf(4,3,0,"LOOP FOTA");   
        if(LoopFotaFlag == 2) {  ///SMS Fota
          Modem_SocketClose(0);
          IWDG_ReloadCounter();
          delay_ms(2000);
          LoopFotaFlag = 0;
          Modem_SMSSend(SMSData.RcvNum, "FOTA Recieve Success!", 21); 
        }
        IWDG_ReloadCounter();
        delay_ms(2000);
        NVIC_SystemReset();
      }
      else {
        ModemStep = MODEM_DATA_SERVER; 
        DataSvrStep = DATA_SVR_FAIL;
        DataSvrWaitCount = 0;
        LoopFotaFlag = 0;
        Modem_SocketClose(0);
        addr = DRV_SETTING_PARM + (u8 *)&Setting.ExtModemMode - (u8 *)&Setting;
        FRAMMultiRead((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode)); 
      }
      break;
      
    case CM_CLSE:
      LoopFotaFlag = 0;
      Modem_SocketClose(0);
      if(!DataServerFlag && !BleServerFlag && !GetServerFlag && !VdisServerFlag && !VdisBleServerFlag) 
        Modem_PPPClose();
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    case CM_DTSC:  
      if(ModemStep == MODEM_DATA_SERVER){
        DataSvrStep = DATA_SVR_SEND;
        DataSvrWaitCount = 0;
        DataServerRetryCount = 0;
        if(SendSettingFlag) SendSettingFlag = 0;
        SendCountPerConnect = 0;
      }
      else if(ModemStep == MODEM_LOOP_FOTA && LoopFotaFlag) {
        Modem_TxData_Loop(CM_DTSS, 0, 0, sizeof(tsSecData), (u8*)&SecData);
      }
      break;      
    case CM_DTSS:  
      {        
        if(ModemStep == MODEM_LOOP_FOTA && LoopFotaFlag) {
          LoopFotaFlag = 0;
          Modem_SocketClose(0);
          break;
        }
        if(Gpck.Header.Length == 1) {
          if(Gpck.Data[0]) {
            FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
            printk("\r\nMODEM : Ack SentAddr(%u) / DataAddr(%u) = %u\r\n", DataSentAddr, DataAddrToSend, DataAddrToSend - DataSentAddr);
          }
          SendCountPerConnect++;
          ModemStep = MODEM_DATA_SERVER;
          if(DataSentAddr != DataAddrToSend && SendCountPerConnect >= MAX_SEND_COUNT_PER_CONNECT) {
            Modem_SocketClose(0);
            DataSvrStep = DATA_SVR_SEND_WAIT;
            SendCountPerConnect = 0;
          }
          else
            DataSvrStep = DATA_SVR_SEND;
          if(TxCommand == CM_EVSS) {
            DataSvrWaitCount = 0;
            BleServerFlag = 0;
          }
          DataSvrWaitCount = 0;
          DataServerRetryCount = 0;
        }           
      }
      break;   
    case CM_EVSS:  
      if(ModemStep == MODEM_DATA_SERVER) {
        DataSvrStep = DATA_SVR_SEND; 
        DataSvrWaitCount = 0;
        DataServerRetryCount = 0;
        BleServerFlag = 0;
      }
      break;   
    case CM_NOTI:
      {
        if(Gpck.Header.Length == 96) {
          tsNOTI_Msg* noti = (tsNOTI_Msg*)Gpck.Data;
          u8 i;
          for(i = 0; i < 80; i++) {
            if(noti->Msg[i] == '\0')
              break;
          }
          Modem_SocketClose(0);
          vTaskDelay(1000);
          Modem_SMSSend(noti->Tel,noti->Msg, i); 
          ModemStep = MODEM_DATA_SERVER; 
          DataSvrWaitCount = 0;
        }
      }
      break;   
    case CM_PRMW:
      {
        st_Setting *hd;
        hd = (st_Setting*)Gpck.Data;
        if(hd->Drv_Name[0] != 0xff) memcpy(Setting.Drv_Name,hd->Drv_Name,sizeof(Setting.Drv_Name));
        if(hd->Drv_Code[0] != 0xff) memcpy(Setting.Drv_Code,hd->Drv_Code,sizeof(Setting.Drv_Code));
        if(hd->Car_Reg_No[0] < MAX_CAR_TYPE) Setting.Car_Reg_No[0] = hd->Car_Reg_No[0];
        if(hd->Car_Reg_No[1] < MAX_CAR_REGION) {
          if(hd->Car_Reg_No[4] < MAX_CAR_CLASS) {
               memcpy(&Setting.Car_Reg_No[1], &hd->Car_Reg_No[1], 8);
          }
        } 
        if(hd->Car_Reg_No[1] != 0xff) memcpy(&Setting.Car_Reg_No[1],&hd->Car_Reg_No[1],sizeof(Setting.Car_Reg_No)-1);
        if(hd->Car_No[0] != 0xff) memcpy(Setting.Car_No,hd->Car_No,sizeof(Setting.Car_No));
        if(hd->Company_Name[0] != 0xff) {
          memcpy(Setting.Company_Name,hd->Company_Name,sizeof(Setting.Company_Name));
          Modem_ChangeURLInfo();
        }
        if(hd->Company_Reg_No[0] != 0xff) memcpy(Setting.Company_Reg_No,hd->Company_Reg_No,sizeof(Setting.Company_Reg_No));
        if(hd->Form_Approval_No[0] != 0xff) memcpy(Setting.Form_Approval_No,hd->Form_Approval_No,sizeof(Setting.Form_Approval_No));
        if(hd->K_Factor != 0xffff && hd->K_Factor > 0) Setting.K_Factor = hd->K_Factor;
        if(hd->RPM_Factor != 0xffff && hd->RPM_Factor > 0) Setting.RPM_Factor = hd->RPM_Factor;
        if(hd->FW_Type != 0xff && hd->FW_Type > 0) Setting.FW_Type = hd->FW_Type;
        if(hd->Set_Direction != 0xff) Setting.Set_Direction = hd->Set_Direction;
        if(hd->Can_Enable != 0xff) Setting.Can_Enable = hd->Can_Enable;
        if(hd->Can_Type != 0xff) Setting.Can_Type = hd->Can_Type;
        if(hd->Car_Idx != 0xff) Setting.Car_Idx = hd->Car_Idx;
        if(hd->Car_SetFactor != 0xff) Setting.Car_SetFactor = hd->Car_SetFactor;
        if(hd->Brake_Type != 0xff) Setting.Brake_Type = hd->Brake_Type;
        if(hd->DAC_Speed != 0xff) Setting.DAC_Speed = hd->DAC_Speed;
        if(hd->DAC_RPM != 0xff) Setting.DAC_RPM = hd->DAC_RPM;
        if(hd->DTG_Type != 0xff) Setting.DTG_Type = hd->DTG_Type;
        if(hd->ODO_Offeset != 0xffffffff) Setting.ODO_Offeset = hd->ODO_Offeset;
        if(hd->Fuel_Factor > 0 && hd->Fuel_Factor < 1000) {
          Calib_F_Factor = hd->Fuel_Factor;
          Setting.Fuel_Factor = Calib_F_Factor;
          FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);
        }              
        if(hd->K_Factor_Percent > 0 && hd->K_Factor_Percent < 0xff) 
          Setting.K_Factor = Setting.K_Factor*100/hd->K_Factor_Percent;                
        if(hd->RPM_Factor_Percent > 0 && hd->RPM_Factor_Percent < 0xff) 
          Setting.RPM_Factor = Setting.RPM_Factor*hd->RPM_Factor_Percent/100;           
        if(hd->Fuel_Factor_Percent > 0 && hd->Fuel_Factor_Percent < 0xff) {
          Calib_F_Factor = Calib_F_Factor*100/hd->Fuel_Factor_Percent;
          Setting.Fuel_Factor = Calib_F_Factor;
          FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);                
        }
                
        if(hd->ExtModemMode != 0xff) Setting.ExtModemMode = hd->ExtModemMode;
//        if(hd->ExtModemTelecom != 0xff) Setting.ExtModemTelecom = hd->ExtModemTelecom;
//        if(hd->ExtModemModel != 0xff) Setting.ExtModemModel = hd->ExtModemModel;
        if(hd->FileOutput != 0xff) Setting.FileOutput = hd->FileOutput;
        if(hd->ExtModemVDIS != 0xff) Setting.ExtModemVDIS = hd->ExtModemVDIS;
        
        FRAMMultiWrite((u8 *)&Setting, DRV_SETTING_PARM, sizeof(Setting));

        if(hd->ODO_Offeset != 0xffffffff) {
          DailyData.TTrip = (double)hd->ODO_Offeset;
          FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
          Val.TTrip = DailyData.TTrip;
          Val.TTripOld = Val.TTrip;
        }
        
        Modem_TxData_Loop(CM_PRMW, 0, 1, sizeof(st_Setting), (u8*)&Setting);  
        ModemStep = MODEM_DATA_SERVER; 
        DataSvrWaitCount = 0;
        
        Modem_SocketClose(0);
        lcd_clear();
        lcd_printf(4,1,0,"Found New Param");
        lcd_printf(4,2,0,"System Reset");   
        vTaskDelay(2000);
        NVIC_SystemReset();  
      }
      break;   
    case CM_MDIR:  
      {
        memset(ModemBuff,0,sizeof(tsModemInfo)+sizeof(tsSvrInfo));
        memcpy(ModemBuff, (u8*)&ModemInfo, sizeof(tsModemInfo));
        memcpy(&ModemBuff[sizeof(tsModemInfo)], (u8*)&HostSvr, sizeof(tsSvrInfo));
        Modem_TxData_Loop(CM_MDIR, 0, 1, sizeof(tsModemInfo)+sizeof(tsSvrInfo), ModemBuff);  
      }
      break;
    case CM_MDIW:  
      {
        tsModemInfo *mi;
        u8 change = 0;
        tsSvrInfo *si;
          
        mi = (tsModemInfo*)Gpck.Data;
//        if(mi->SrvIP != 0xffffffff) {ModemInfo.SrvIP = mi->SrvIP; change = 1;}
//        if(mi->SrvPort != 0xffff) {ModemInfo.SrvIP = mi->SrvPort; change = 1;}
//        if(mi->CNUM != 0xffffffff) ModemInfo.CNUM = mi->CNUM;
//        if(mi->MSISDN[0] != 0xff) memcpy((u8*)ModemInfo.MSISDN,(u8*)mi->MSISDN,sizeof(ModemInfo.MSISDN));
        if(mi->DataServerInterval > 0 &&  mi->DataServerInterval <= 3600) {ModemInfo.DataServerInterval = mi->DataServerInterval; change = 1;}
//        if(mi->VehStateInterval != 0xffff) ModemInfo.VehStateInterval = mi->VehStateInterval;
//        if(mi->VehStateInterval_Idle != 0xffff) ModemInfo.VehStateInterval_Idle = mi->VehStateInterval_Idle;
//        if(mi->LpIP != 0xffffffff) {ModemInfo.LpIP = mi->LpIP; change = 1;}
//        if(mi->LpPort != 0xffff) {ModemInfo.LpIP = mi->LpPort; change = 1;}
//        if(mi->FotaPollingIntrval != 0xffff) ModemInfo.FotaPollingIntrval = mi->FotaPollingIntrval;
//        if(mi->QualPollingIntrval != 0xffff) ModemInfo.QualPollingIntrval = mi->QualPollingIntrval;
//        if(mi->FactoryInit != 0xff) ModemInfo.FactoryInit = mi->FactoryInit;
//        if(mi->ServerChange != 0xff) ModemInfo.ServerChange = mi->ServerChange;
//        if(mi->ServerConnected != 0xff) ModemInfo.ServerConnected = mi->ServerConnected;
        if(mi->DataInterval > 0 && mi->DataInterval <= 600) {ModemInfo.DataInterval = mi->DataInterval; change = 1;}
        if(change)
          FRAMMultiWrite((u8 *)&ModemInfo, MODEM_INFO, sizeof(tsModemInfo));
        
        change = 0;
        si = (tsSvrInfo*)(&Gpck.Data[sizeof(tsModemInfo)]);
        if(si->SvrName[0] > ' ' && si->SvrName[0] <= 'z' && !IsSameString(si->SvrName, HostSvr.SvrName, sizeof(HostSvr.SvrName))) {
          memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
          memcpy(HostSvr.SvrName, si->SvrName, sizeof(HostSvr.SvrName));
          change = 1;
        }
        if(si->SvrPort > 0 && si->SvrPort < 100000 && si->SvrPort != HostSvr.SvrPort) {
          HostSvr.SvrPort = si->SvrPort;  
          change = 1;
        }
        if(change) {
          FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
          SendSettingFlag = 1;
        }

        memset(ModemBuff,0,sizeof(tsModemInfo)+sizeof(tsSvrInfo));
        memcpy(ModemBuff, (u8*)&ModemInfo, sizeof(tsModemInfo));
        memcpy(&ModemBuff[sizeof(tsModemInfo)], (u8*)&HostSvr, sizeof(tsSvrInfo));
        Modem_TxData_Loop(CM_MDIW, 0, 1, sizeof(tsModemInfo)+sizeof(tsSvrInfo), ModemBuff);  
      }
      break;
    case CM_ISIR:  
      Modem_TxData_Loop(CM_ISIR, 0, 1, sizeof(stIsgSetting), (u8*)&IsgSetting);  
      break;
    case CM_ISIW:  
      {
        stIsgSetting *is;
        u8 change = 0, i;
        is = (stIsgSetting*)Gpck.Data;
        if(is->IsgMode < 2) {IsgSetting.IsgMode = is->IsgMode; change = 1;}
        if(is->IdleTimeLimit < 999) {
          if(is->IdleTimeLimit == 0) IsgSetting.IdleTimeLimit = 180;
          else IsgSetting.IdleTimeLimit = is->IdleTimeLimit; 
          change = 1;
        }
        if(is->BatVoltLimit < 300) {
          if(is->BatVoltLimit == 0) IsgSetting.BatVoltLimit = 230;
          else IsgSetting.BatVoltLimit = is->BatVoltLimit; 
          change = 1;
        }
        if(is->CoolTempLimit < 200) {IsgSetting.CoolTempLimit = is->CoolTempLimit; change = 1;}
        if(is->InclineLimit < 90) {
          if(is->InclineLimit == 0) IsgSetting.InclineLimit = 4;
          else IsgSetting.InclineLimit = is->InclineLimit; 
          change = 1;
        }
        if(is->PBrakeMode < 3) {IsgSetting.PBrakeMode = is->PBrakeMode; change = 1;}
        if(is->AirPressMode <= 3) {IsgSetting.AirPressMode = is->AirPressMode; change = 1;}
        if(is->NGearMode <= 3) {IsgSetting.NGearMode = is->NGearMode; change = 1;}
        if(is->GpsMode < 2) {IsgSetting.GpsMode = is->GpsMode; change = 1;}
        if(is->GeoFenceCnt < 11) {IsgSetting.GeoFenceCnt = is->GeoFenceCnt; change = 1;}
        for(i = 0; i < 10; i++) {
          if(is->GeoFence->Lat < 40000000 && is->GeoFence->Lon < 132000000 && is->GeoFence->Radius < 1000) {
            memcpy((u8*)&IsgSetting.GeoFence[i],(u8*)&is->GeoFence[i],sizeof(stGeoFence)); 
            change = 1;
          }
        }
        if(is->IdleFuelRate != 0xffff) {IsgSetting.IdleFuelRate = is->IdleFuelRate; change = 1;}
        if(is->FirstOnCheckTime < 0xff) {IsgSetting.FirstOnCheckTime = is->FirstOnCheckTime; change = 1;}
        if(is->IsgSkipTime < 0xff) {IsgSetting.IsgSkipTime = is->IsgSkipTime; change = 1;}
        if(change)
          FRAMMultiWrite((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));  
        Modem_TxData_Loop(CM_ISIW, 0, 1, sizeof(stIsgSetting), (u8*)&IsgSetting);  
      }      
      break;
    case CM_DYSS:  
      {
        u32 day_cnt = 0;
        u32 sday_cnt, eday_cnt; 
        
        if(Gpck.Header.Length == 1) {
          sday_cnt = SendData_StartTime/86400;
          eday_cnt = SendData_EndTime/86400;
          if(Gpck.Data[0]) {
            printk("\r\nMODEM : Ack SentDayCount(%u) / LastDayCount(%u) = %u\r\n", sday_cnt, eday_cnt, eday_cnt - sday_cnt);
          }
          ModemStep = MODEM_DATA_SERVER;
          day_cnt = CalDayCnt(SendData_EndTime, SendData_StartTime);
          if(day_cnt == 0 || SendData_EndTime < SendData_StartTime) {
            SendDailyDataFlag = 0;
            SendData_StartTime = 0;
            SendData_EndTime = 0;
          }
          else {
            SendData_StartTime = (sday_cnt+1)*86400;
          }
          FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
          DataSvrStep = DATA_SVR_SEND;
          DataSvrWaitCount = 0;
          DataServerRetryCount = 0;
        }           
      }      
      break;
  }  
  Gpck.Step = 0;
  Gpck.Delay = 0;
}

void Modem_SendData_Loop(u8 mode)
{
  u32 rfidcardnum;
  unsigned long dlen, i,j, data_cnt, size, data_ok_cnt;  
  stLpEventData ev_data;
  u32 day_cnt, sday_cnt, eday_cnt;
  
  DataSvrWaitCount = 0;
  
  if(mode == 0) {
    memcpy(&Setting.Lat, &SecData.Lat, 10);
    Modem_TxData_Loop(CM_INFO, 0, 1, sizeof(st_Setting), (u8*)&Setting);  
  }
  else if(mode == 1) {
    if(DataAddrToSend >= DataSentAddr)
      dlen = DataAddrToSend-DataSentAddr;
    else
      dlen = NAND_SIZE_DATA+DataAddrToSend-DataSentAddr;
    if(dlen > DATA_TX_LIMIT) {
      if(DataAddrToSend >= DataSentAddr) 
        DataSentAddr = DataAddrToSend - DATA_TX_LIMIT;
      else
        DataSentAddr = (NAND_SIZE_DATA + DataAddrToSend - DATA_TX_LIMIT)%NAND_SIZE_DATA;
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
      dlen = DATA_TX_LIMIT;
    }
    if(dlen > MODEM_SENDDATA_MAX) {
      dlen = MODEM_SENDDATA_MAX;
    }
    
    if((DataSentAddr % NAND_SIZE_DATA)+dlen > NAND_SIZE_DATA)
      dlen = NAND_SIZE_DATA - (DataSentAddr% NAND_SIZE_DATA); 
    
    if(dlen) { 
      size = sizeof(tsSecData);
      data_cnt = dlen/size;
      data_ok_cnt = 0;
      memset(ModemBuff, 0, dlen);
      for(i = 0; i < data_cnt; i++) {
        Nand_Read_SecData((u8*)&RD_Sec_Data, DataSentAddr, size);
        if(RD_Sec_Data.Time > 0x58000000 && RD_Sec_Data.Time < 0x80000000) {  //2017년 이후... 
//          if(RD_Sec_Data.Time > Prev_RD_Sec_Data.Time && (ModemInfo.DataInterval < 2  || (RD_Sec_Data.Time%ModemInfo.DataInterval) == 0)) {
            memcpy((u8*)&ModemBuff[data_ok_cnt*size], (u8*)&RD_Sec_Data, size);
            data_ok_cnt++;
//          }
//          Prev_RD_Sec_Data = RD_Sec_Data;
            if(SendData_StartTime == 0) {
              SendData_StartTime = RD_Sec_Data.Time-(RD_Sec_Data.Time%86400);
              FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
            }
            if(RD_Sec_Data.Time > SendData_EndTime) SendData_EndTime = RD_Sec_Data.Time;
        }
        DataSentAddr = (DataSentAddr+size)%NAND_SIZE_DATA;
      }
      printk("\r\nMODEM : Data Cnt(%u) / SentAddr(%u) / DataAddr(%u) = %u\n",data_ok_cnt, DataSentAddr, DataAddrToSend, DataAddrToSend - DataSentAddr);
      
      if(data_ok_cnt > 0) {      
        dlen = data_ok_cnt*size;
        Gpck.Step = 0;
        Modem_TxData_Loop(CM_DTSS, (DataSentAddr % 0x10000), (DataAddrToSend % 0x10000), dlen, ModemBuff);  
      }
      else {
        FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
        DataSvrStep = DATA_SVR_SEND;
        DataSvrWaitCount = 0;
      }
    }
  }
  else if(mode == 2) {
    if(ReceiveMediaFlag == BLESCANNER) {
      if(TxBLE_Index == 0) {
        BleServerFlag = 0;
      }
      else {
        if(TxBLE_Index > SCAN_MAXNUM)
          TxBLE_Index = SCAN_MAXNUM;
        
        size = sizeof(stLpEventData);
        dlen = TxBLE_Index*size;
        memset(ModemBuff, 0, dlen);
        
        for(i = 0; i < TxBLE_Index; i++) {
          ev_data.Time = TxBLE_Body[i].Time;
          ev_data.Type = BLESCANNER;
          for(j = 0; j < 6; j++)          // MAC Adress 바꿈.
            ev_data.MAC_Addr[j] = TxBLE_Body[i].ScanData.addr[5-j];
          ev_data.IO_Val = TxBLE_Body[i].CurTake;
          ev_data.RSSI = TxBLE_Body[i].ScanData.rssi;
          if(TxBLE_Body[i].CurTake == 2)
            ev_data.Power = TxEvent[i].SubRssi.RssiAvg;
          else 
            ev_data.Power = 0;
          ev_data.Lat = TxBLE_Body[i].Lat; 
          ev_data.Lon = TxBLE_Body[i].Lon; 
          ev_data.Azi = TxBLE_Body[i].Azi; 
          ev_data.Speed = TxBLE_Body[i].Speed; 
          memcpy((u8*)&ModemBuff[i*size], (u8*)&ev_data, size);
          printk("\r\nMODEM : Rssi (%d) / Avg Rssi (%d) \n",ev_data.RSSI, ev_data.Power);
        }
        printk("\r\nMODEM : Event Data (%u) / Event Count (%u) \n",dlen, TxBLE_Index);
        Gpck.Step = 0;
        Modem_TxData_Loop(CM_EVSS, 0, 0, dlen, ModemBuff);  
      }
    }
    else {
      if(TxRFID_Index == 0) {
        BleServerFlag = 0;
      }
      else {
        if(TxRFID_Index > CONTACT_MAXNUM)
          TxRFID_Index = CONTACT_MAXNUM;
        
        size = sizeof(stLpEventData);
        dlen = TxRFID_Index*size;
        memset(ModemBuff, 0, dlen);
        
        for(i = 0; i < TxRFID_Index; i++) {
          ev_data.Time = TxRFID_Data[i].Time;
          ev_data.Type = RFIDCARD;
          memset((u8*)ev_data.MAC_Addr, 0, 6);
          memcpy((u8*)RFID_Str, TxRFID_Data[i].ID, 10);
          rfidcardnum = (u32)(atoi(RFID_Str));
          memcpy((u8*)ev_data.MAC_Addr, (u8*)&rfidcardnum, 4);
          ev_data.RSSI = 0;
          ev_data.Power = 0;
          ev_data.IO_Val = TxRFID_Data[i].CurTake; 
          ev_data.Lat = TxRFID_Data[i].Lat; 
          ev_data.Lon = TxRFID_Data[i].Lon; 
          ev_data.Azi = TxRFID_Data[i].Azi; 
          ev_data.Speed = TxRFID_Data[i].Speed; 
          memcpy((u8*)&ModemBuff[i*size], (u8*)&ev_data, size);
        }
        printk("\r\nMODEM : Event Data (%u) / Event Count (%u) \n",dlen, TxRFID_Index);      
        Gpck.Step = 0;
        Modem_TxData_Loop(CM_EVSS, 0, 0, dlen, ModemBuff);  
      }
    }
  }
  else if(mode == 3) {
    FRAMMultiRead((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
    day_cnt = CalDayCnt(RTC_LVal, SendData_StartTime);
    dlen = 0;
    for(i = 0; i <= day_cnt; i++) {
      IWDG_ReloadCounter();  
      memset((u8*)&TmpDailyData, 0, sizeof(stDailyData)); 
      Nand_Read_DailyData(day_cnt-i, (u8*)&TmpDailyData);
      if(TmpDailyData.Time >= SendData_StartTime && TmpDailyData.Time <= SendData_EndTime) {
        dlen = sizeof(stDailyData);
        sday_cnt = SendData_StartTime/86400;
        eday_cnt = SendData_EndTime/86400;
        printk("\r\nMODEM : SendDailyData SendDayCount(%u) / LastDayCount(%u)\r\n", sday_cnt, eday_cnt);
        Modem_TxData_Loop(CM_DYSS, 0, 0, dlen, (u8*)&TmpDailyData);  
        break;
      }
    }
    if(dlen == 0) {
      SendDailyDataFlag = 0;
      DataSvrStep = DATA_SVR_SEND;
      DataSvrWaitCount = 0;
      SendData_StartTime = 0;
      FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
    }
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
u8 DataTxContinue;
void Modem_DataSvrProcess_Loop(void)
{

  if(Setting.ExtModemMode != LOOP_PROTOCOL) {
    DataSvrStep = DATA_SVR_IDLE;
  }
  
  switch(DataSvrStep) {
    case DATA_SVR_DATACHECK:
      if(DataAddrToSend == DataSentAddr && !SendSettingFlag && !BleServerFlag) { //same addr
        TxCommand = CM_DTSS;
        DataSvrStep = DATA_SVR_CLOSE;
        DataTxContinue = 0;
      }
      else {
        DataSvrStep = DATA_SVR_CONNECT;
        if(DataTxContinue) {
          SocketConnectStep = SOCKET_STATUS_CHECK;
          DataTxContinue = 0;
        }
        else
          SocketConnectStep = SOCKET_CONNECT_START;
        SocketRetryCnt = 0;
        SocketWaitCnt = 0;
        SendCountPerConnect = 0;
      }
      DataSvrWaitCount = 0;
      break;
      
    case DATA_SVR_CONNECT:
      Modem_SocketConnect(HostSvr.SvrName, HostSvr.SvrPort, 0);
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
        DataSvrStep = DATA_SVR_DATACHECK;
        DataTxContinue = 1;
      }
      break;
      
    case DATA_SVR_SEND:
      if(TcpState != 1) {
        DataSvrStep = DATA_SVR_FAIL;
        return;
      }
      DataSvrWaitCount = 0;
      
      if(SendSettingFlag) {
        Modem_SendData_Loop(0);
        DataSvrStep = DATA_SVR_RECIEVE_WAIT;          
      }
      else if(BleServerFlag){
        Modem_SendData_Loop(2);
        DataSvrStep = DATA_SVR_RECIEVE_WAIT; 
        DataTxContinue = 0;
      }
      else if(DataServerFlag){
        FRAMMultiRead((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
        if(DataAddrToSend != DataSentAddr) {
          DataSvrStep = DATA_SVR_RECIEVE_WAIT;          
          Modem_SendData_Loop(1);
        }
        else {
          if(SendDailyDataFlag){
            DataSvrStep = DATA_SVR_RECIEVE_WAIT;          
            Modem_SendData_Loop(3);
          }
          else {
            TxCommand = CM_DTSS;
            DataSvrStep = DATA_SVR_CLOSE;
            DataTxContinue = 0;
          }
        }
      }
      else {
        DataSvrStep = DATA_SVR_CLOSE;
        DataTxContinue = 0;
      }
      break;
      
    case DATA_SVR_RECIEVE_WAIT:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 20000) {  //20sec
        DataSvrWaitCount = 0;
        Modem_SocketClose(0);
        DataSvrStep = DATA_SVR_FAIL;
      }
      break;

    case DATA_SVR_CLOSE:
      DataSvrWaitCount++;
      if(DataSvrWaitCount > 1000) {  //1sec
        Modem_SocketClose(0);
        DataSvrStep = DATA_SVR_SUCCESS;
        DataSvrWaitCount = 0;
      }
      break;

    case DATA_SVR_FAIL:
      printk("\r\nMODEM : Data Send Fail\r\n");
      ModemWaitCnt = 0;
      DataServerRetryCount++;
      if(DataServerFlag == 2 && TxCommand == CM_DTSS) {   //Power Off Mode
        if(DataServerRetryCount >= 2) {
          if(!VdisServerFlag && !VdisBleServerFlag) {
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
          }
          else
            ModemStep = MODEM_DATA_IDLE;
          DataServerRetryCount = 0;
          ModemWaitCnt = 0;
          DataServerFlag = 0;
          SendDailyDataFlag = 0;
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
          if(TxCommand == CM_EVSS) {
            BleServerFlag = 0;
            if(BleEndFlag && DataServerFlag == 0)
              ModemStep = MODEM_POWER_OFF;              
          }
          else {
            DataServerFlag = 0;
            SendDailyDataFlag = 0;
          }
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
      if(DataServerFlag == 2) {
        if(BleServerFlag)
          ModemStep = MODEM_DATA_IDLE;
        else {
          if(!VdisServerFlag && !VdisBleServerFlag) { 
            Modem_PPPClose();
            ModemStep = MODEM_POWER_OFF;
          }
          else
            ModemStep = MODEM_DATA_IDLE;
        }
      }
      else {
        if(TxCommand == CM_EVSS && BleEndFlag && !DataServerFlag && !VdisServerFlag && !VdisBleServerFlag) 
          ModemStep = MODEM_POWER_OFF;              
        else
          ModemStep = MODEM_DATA_IDLE;
        Modem_PPPClose();
      }
      if(TxCommand != CM_EVSS) {
        DataServerFlag = 0;
        SendDailyDataFlag = 0;
      }
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
      SendDailyDataFlag = 0;
      break;
      
    default : 
      DataSvrStep = DATA_SVR_FAIL;
      break;  
  }
}

void Modem_LoopFotaProcess(void)
{
  
  if(!LoopFotaFlag) {
    Modem_SocketClose(0);
    ModemStep = MODEM_DATA_IDLE;
    return;
  }
  switch(LoopFotaStep) {
    case LOOP_FOTA_CONNECT:
      Modem_SocketConnect(FWDN_Svr.SvrName, FWDN_Svr.SvrPort, 0);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          LoopFotaStep = LOOP_FOTA_WAIT;
          Modem_SendData_Loop(0);
        }
        else {
          LoopFotaFlag = 0;
          Modem_SocketClose(0);
          if(!DataServerFlag && !BleServerFlag && !GetServerFlag && !VdisServerFlag && !VdisBleServerFlag) 
            Modem_PPPClose();
        }
      }
      break;
      
    case LOOP_FOTA_WAIT:
      Gpck.Delay++;
      if(Gpck.Delay > 10000) {  // 10sec
        LoopFotaFlag = 0;
        Gpck.Delay = 0;
        Modem_SocketClose(0);
        if(!DataServerFlag && !BleServerFlag && !GetServerFlag && !VdisServerFlag && !VdisBleServerFlag) 
          Modem_PPPClose();
        ModemStep = MODEM_DATA_IDLE;
      }
      break;
      
    default : 
      LoopFotaStep = LOOP_FOTA_WAIT;
      break;  
  }
}


void Modem_Init_Loop(void)
{
  if(Setting.ExtModemMode == LOOP_PROTOCOL)  {
    SendSettingFlag = 1;
//    DataServerFlag  = 1; 
  }
}

u32 Loop_SendTimeCnt;
void Modem_SecInt_Loop(void)
{
  u32 daddr = 0, data_cnt;

  if(Setting.ExtModemMode != LOOP_PROTOCOL) return;

  if(ModemInfo.DataServerInterval > 9 && ModemInfo.DataServerInterval < 10000) {
    Loop_SendTimeCnt++;
    daddr = (u32)(DataSaveAddr-DataSentAddr);
    data_cnt = daddr/sizeof(tsSecData);
    if(Loop_SendTimeCnt >= ModemInfo.DataServerInterval && (data_cnt >= ModemInfo.DataServerInterval)) {
      Loop_SendTimeCnt = 0;
      if(DataServerFlag == 0 && !Power_Off_Flag) {
        DataServerFlag = 1;
        DataServerRetryCount = 0;
      }
      DataAddrToSend = DataSaveAddr;
    }
  }
}

void Modem_DataIdleProcess_Loop(void)
{
  if(DataServerFlag) {
    DataSvrStep = DATA_SVR_DATACHECK;
    DataSvrWaitCount = 0;
    ModemStep = MODEM_DATA_SERVER; 
//    SendData_StartTime = 0;
    SendData_EndTime = 0;
#ifdef ISG_ENABLE    
    SendDailyDataFlag = 1;  
#endif    
  }
  else if(BleServerFlag) {
    DataServerRetryCount = 0;
    DataSvrStep = DATA_SVR_DATACHECK;
    DataSvrWaitCount = 0;
    ModemStep = MODEM_DATA_SERVER;    
  }  
}





/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/
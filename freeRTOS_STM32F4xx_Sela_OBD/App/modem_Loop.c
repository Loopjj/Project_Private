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
u8 Retry_FWSD = 0;
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
  if(r <= 'z')  put_ch_console(r);
#endif
  unsigned int pstep = 0;
  unsigned char sd[10];
  
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
          if(Gpck.Header.Command == CM_FWSD) Retry_FWSD = 0;
          Gpck.Step = ST_EPK_ST;//ST_PROCESSING;
        }
        else {
          pstep = Gpck.Step;
          if(Gpck.Header.Command == CM_FWSD && Retry_FWSD < 10) Gpck.Step = ST_RETRY;
          else Gpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_EPK_ST:
      if(r == 0xCF) {
        if(Gpck.Header.Command == CM_FWSD) Retry_FWSD = 0;
        Gpck.Step = ST_EPK_CF;
      }
      else {
        pstep = Gpck.Step;
        if(Gpck.Header.Command == CM_FWSD && Retry_FWSD < 10) Gpck.Step = ST_RETRY;
        else Gpck.Step = ST_SPK_ST;
      }
      break;
    case ST_EPK_CF:
      if(r == 0xD0) {
        if(Gpck.Header.Command == CM_FWSD) Retry_FWSD = 0;
        Gpck.Step = ST_PROCESSING;
        //while
      }
      else {
        pstep = Gpck.Step;
        if(Gpck.Header.Command == CM_FWSD && Retry_FWSD < 10) Gpck.Step = ST_RETRY;
        else Gpck.Step = ST_SPK_ST;
      }
      break;
    case ST_RETRY:
      Retry_FWSD ++;
      sd[0] = 1;
      if(Gpck.Header.Index) Modem_TxData_Loop(CM_FWSD, Gpck.Header.Index-1, Gpck.Header.Count, 1, sd);
      else Modem_TxData_Loop(CM_INFO, 0, 1, sizeof(st_Setting), (u8*)&Setting);
      DataSvrWaitCount = 0;
      Gpck.Step = ST_SPK_ST;
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
  hd.DeviceID = DV_SCR2;
 
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
INT8U send_flag=0;
void Modem_RxDecode_Loop(void)
{
	int sectCnt;
	int i, sectOffset;
	u16 ramCRC=0;
  
  if(Gpck.Step < ST_PROCESSING) return;
  
  static unsigned char crc_same_flag;
  unsigned char sd[10];
  u32 addr =0;
  
  switch(Gpck.Header.Command) {
    case CM_INFO: 
      break;
      
    case CM_FWIF: 
      memcpy(&FW_Info, Gpck.Data, sizeof(tsFWIF));
      printk("\n##### DownLoad Command #####\n");
      printk("File size          : %dBytes\n",FW_Info.Size);
      printk("One Packet size    : %dBytes\n",FW_Info.DivisionSize);
      printk("Download Code Check: 0x%04x\n",FW_Info.CRC16);      
      
      memcpy((char *)tPrjcode.boot_seq,"down_gui",8);
      //SPI_FLASH_PageErase(PROJECT_BASE);
      if(FW_Info.Type == 4 || FW_Info.Type == 5){      
        if(SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t))) 
        {
          /*-------------------------------------------------------------------------------*/	
          // 다운로드 파일 사이즈 만큼의 SPI Flash의 영역을 지운다.
          sectCnt = (FW_Info.Size / SPI_FLASH_BLOCK)+1;
          if(sectCnt > FLASH_DOWN_BLOCKCOUNT) sectCnt = FLASH_DOWN_BLOCKCOUNT;
          
            if(FW_Info.Type == 4)
              sectOffset = DOWNLOAD_SECTOR;
            else if(FW_Info.Type == 5)
              sectOffset = ECUDATA_SECTOR;

          // Block 크기 삭제 
          for(i=0;i<sectCnt;i++)
          {
            SPI_FLASH_BlockErase(sectOffset);
            sectOffset += SPI_FLASH_BLOCK;
            printk("Erase %X at 0x%05X\n",SPI_FLASH_BLOCK,sectOffset);
          }
          /*-------------------------------------------------------------------------------*/	
          tPrjcode.file_size = FW_Info.Size;
        }
    
        sd[0] = 1;
        Flags.SecIntFlag = 0;
        Modem_TxData_Loop(CM_FWIF, Gpck.Header.Index, Gpck.Header.Count, 1, sd);
        printk("\nMODEM : Firmware Info Recieve Ok\n");      
        DataSvrWaitCount = 0;
      }
      break;
      
    case CM_FWSD: 
      {
        u32 naddr;        
        if(FW_Info.Type == 4 || FW_Info.Type == 5) {        
          if(FW_Info.Type == 4)
            naddr = DOWNLOAD_SECTOR + (Gpck.Header.Index * FW_Info.DivisionSize);
          else if(FW_Info.Type == 5)
            naddr = ECUDATA_SECTOR + (Gpck.Header.Index * FW_Info.DivisionSize);       
          u16 nsize = FW_Info.DivisionSize;
          if(Gpck.Header.Index < Gpck.Header.Count) {          
            SPI_FLASH_BufferWrite(Gpck.Data, naddr, nsize);
            send_flag = 5;
          }
          else {
            crc_same_flag = 0;
            u16 ccrc=0;
            nsize = FW_Info.Size % FW_Info.DivisionSize;
  //          SPI_FLASH_BufferWrite(Gpck.Data, naddr, nsize);
            for(int i = 0; i < Gpck.Header.Count; i++) {
              if(FW_Info.Type == 4)
                naddr = DOWNLOAD_SECTOR + (i * FW_Info.DivisionSize);
              else if(FW_Info.Type == 5)
                naddr = ECUDATA_SECTOR + (i * FW_Info.DivisionSize);
              
              if(i < (Gpck.Header.Count - 1)) { 
                nsize = FW_Info.DivisionSize; 
                SPI_FLASH_BufferRead(ModemBuff, naddr, nsize);
              }
              else {
                nsize = FW_Info.Size % FW_Info.DivisionSize; 
                SPI_FLASH_BufferRead(ModemBuff, naddr, nsize);              
              } 
              ccrc =  GenerateCRC(ModemBuff, nsize, ccrc);
            }
            if(ccrc == FW_Info.CRC16) {
              // 마지막 계산된 CRC 값 저장 -> PROJECT_BASE 번지 
              tPrjcode.file_crc = ccrc;

              // Flash에 저장된 데이터에 대한 CRC 값 Display
              if(FW_Info.Type == 4)
                ramCRC = GetFlashAreaCRC(DOWNLOAD_SECTOR, FW_Info.Size);
              else if(FW_Info.Type == 5)
                ramCRC = GetFlashAreaCRC(ECUDATA_SECTOR, FW_Info.Size);
              printk("Check wrote flash fileCRC: [%04X]\n", (INT16U)ramCRC);

              //SPI_FLASH_PageErase(PROJECT_BASE);
              SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));					
              crc_same_flag = 1;
            }
          }  
          sd[0] = 1;
          Modem_TxData_Loop(CM_FWSD, Gpck.Header.Index, Gpck.Header.Count, 1, sd);
          printk("\nMODEM : Firmware Recieve %u / %u\n", Gpck.Header.Index, Gpck.Header.Count);        
          DataSvrWaitCount = 0;
        }
      }
      break;
    case CM_FWAK:
      if(FW_Info.Type == 4 || FW_Info.Type == 5) {      
        Modem_TxData_Loop(CM_FWAK, Gpck.Header.Index, Gpck.Header.Count, 1, (u8*)&crc_same_flag); 
        Modem_SocketClose(0);
        if(FW_Info.Type == 4 && crc_same_flag) {  
          FW_Info.Size = FW_Info.Size+(4-(FW_Info.Size%4));  //Fota Size를 4의 배수로..
          printk("\nMODEM : Firmware Ack Send\n", Gpck.Header.Index, Gpck.Header.Count);
          memcpy((char*)tPrjcode.boot_seq,"backup",6);
          //SPI_FLASH_PageErase(PROJECT_BASE);
          SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));        
                     
          if(LoopFotaFlag == 2) {  ///SMS Fota
            Modem_SocketClose(0);
            IWDG_ReloadCounter();
            delay_ms(2000);
            LoopFotaFlag = 0;
            Modem_SMSSend(SMSData.RcvNum, "FOTA Recieve Success!", 21); 
          }
          IWDG_ReloadCounter();
          delay_ms(2000);
          dbg_system_reset(); //ResetCtrl=1; 
        }
        else if(FW_Info.Type == 5 && crc_same_flag) {
          Setting.ECUFW_Size = FW_Info.Size;
          Setting.ECUFW_flag = 1;  
          addr = DRV_SETTING_PARM + offsetof(st_Setting, ECUFW_Size);
          FRAMMultiWrite((u8 *)&Setting.ECUFW_Size, addr, 3);
          keyflag.hidden=3;
          keyflag.setcnt=2;
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
      }
      break;
      
    case CM_CLSE:
      LoopFotaFlag = 0;
      Modem_SocketClose(0);
      if(!DataServerFlag) 
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
        Modem_TxData_Loop(CM_DTSS, 0, 0, sizeof(stREAD_VALUE), (u8*)&ReadValue);
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
          DataSvrWaitCount = 0;
          DataServerRetryCount = 0;
        }           
      }
      break;
    case CM_PRMW:
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
    case CM_DYSS:      
      break;
  }  
//  Gpck.Step = 0;
  Gpck.Delay = 0;
}

void Modem_SendData_Loop(u8 mode)
{
  unsigned long dlen, i, data_cnt, size, data_ok_cnt;  
  
  DataSvrWaitCount = 0;
  
  if(mode == 0) {
    memcpy(&Setting.Lat, &ReadValue.LT, 10);
    Modem_TxData_Loop(CM_INFO, 0, 1, sizeof(st_Setting), (u8*)&Setting);  
  }
  else if(mode == 1) {
    if(DataAddrToSend >= DataSentAddr)
      dlen = DataAddrToSend-DataSentAddr;
    else
      dlen = FLASH_DATA_SIZE+DataAddrToSend-DataSentAddr;
    if(dlen > DATA_TX_LIMIT) {
      if(DataAddrToSend >= DataSentAddr) 
        DataSentAddr = DataAddrToSend - DATA_TX_LIMIT;
      else
        DataSentAddr = (FLASH_DATA_SIZE + DataAddrToSend - DATA_TX_LIMIT)%FLASH_DATA_SIZE;
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
      dlen = DATA_TX_LIMIT;
    }
    if(dlen > MODEM_SENDDATA_MAX) {
      dlen = MODEM_SENDDATA_MAX;
    }
    
    if((DataSentAddr % FLASH_DATA_SIZE)+dlen > FLASH_DATA_SIZE)
      dlen = FLASH_DATA_SIZE - (DataSentAddr% FLASH_DATA_SIZE); 
    
    if(dlen) { 
      size = sizeof(stREAD_VALUE);
      data_cnt = dlen/size;
      data_ok_cnt = 0;
      memset(ModemBuff, 0, dlen);
      for(i = 0; i < data_cnt; i++) {
        SPI_FLASH_BufferRead((u8*)&RD_Sec_Data, DataSentAddr, size);
        if(RD_Sec_Data.TM > 0x58000000 && RD_Sec_Data.TM < 0x80000000) {  //2017년 이후... 
            memcpy((u8*)&ModemBuff[data_ok_cnt*size], (u8*)&RD_Sec_Data, size);
            data_ok_cnt++;
            if(SendData_StartTime == 0) {
              SendData_StartTime = RD_Sec_Data.TM-(RD_Sec_Data.TM%86400);
              FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
            }
            if(RD_Sec_Data.TM > SendData_EndTime) SendData_EndTime = RD_Sec_Data.TM;
        }
        DataSentAddr = (DataSentAddr+size)%FLASH_DATA_SIZE;
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
      if(DataAddrToSend == DataSentAddr && !SendSettingFlag ) { //same addr
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
          Modem_PPPClose();
          ModemStep = MODEM_POWER_OFF;
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
          DataServerFlag = 0;
          SendDailyDataFlag = 0;
          Modem_PPPClose();
          Modem_ResetProcess();
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
      if(DataServerFlag == 2)
        ModemStep = MODEM_POWER_OFF;
      else
        ModemStep = MODEM_DATA_IDLE;
      Modem_PPPClose();
      DataServerFlag = 0;
      SendDailyDataFlag = 0;

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
          vTaskSuspend(xHandle_can); // CAN Task stop
          LoopFotaStep = LOOP_FOTA_WAIT;
          Modem_SendData_Loop(0);
        }
        else {
          LoopFotaFlag = 0;
          Modem_SocketClose(0);
          if(!DataServerFlag) 
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
        if(!DataServerFlag) 
          Modem_PPPClose();
        ModemStep = MODEM_DATA_IDLE;
        Flags.SecIntFlag = 1;
        vTaskResume(xHandle_can); // CAN Task start
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
  }
}

void Modem_SecInt_Loop(void)
{
  u32 daddr = 0, data_cnt;

  if(Setting.ExtModemMode != LOOP_PROTOCOL) return;

  if(ModemInfo.DataServerInterval > 9 && ModemInfo.DataServerInterval < 10000) {
    Loop_SendTimeCnt += SENDTIME;
    daddr = (u32)(DataSaveAddr-DataSentAddr);
    data_cnt = (daddr/sizeof(stREAD_VALUE))*SENDTIME;
    if(Loop_SendTimeCnt >= ModemInfo.DataServerInterval && (data_cnt >= ModemInfo.DataServerInterval)) {
      Loop_SendTimeCnt = 0;
      if(!DataServerFlag && ModemPowerOnFlag && SecData.Sig.KeyOn) {
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
    SendData_EndTime = 0;
#ifdef ISG_ENABLE    
    SendDailyDataFlag = 1;  
#endif    
  }
}


/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/
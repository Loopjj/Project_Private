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
#define MODEM_DMS_GLOBALS
#include "includes.h"
#include "modem_DMS.h"

#define FOTA_BUFFER_SIZE 2048
#define FOTA_PKG_DELAY 1000

typedef enum {
  QUAL_NOTI_GETDATA,
  QUAL_NOTI_CONNECT,
  QUAL_NOTI_SEND,
  QUAL_NOTI_CLOSE,
  QUAL_NOTI_FAIL,
  QUAL_NOTI_SUCCESS,
  QUAL_NOTI_IDLE
}teQUAL_NOTI_STATUS;

typedef enum {
  OFF_NOTI_CONNECT,
  OFF_NOTI_SEND,
  OFF_NOTI_CLOSE,
  OFF_NOTI_FAIL,
  OFF_NOTI_SUCCESS,
  OFF_NOTI_IDLE
}teOFF_NOTI_STATUS;

typedef enum {
  FOTA_CONNECT,
  FOTA_SEND,
  FOTA_RECIEVE_WAIT,
  FOTA_CLOSE,
  FOTA_FAIL,
  FOTA_SUCCESS,
  FOTA_IDLE
}teFOTA_STATUS;

u8 QualNotiStep;
int QualNotiWaitCount;
u8 OffNotiStep;
int OffNotiWaitCount;
u8 FotaStep;
int FotaWaitCount;
u16 FotaRxBufferSize;

__packed typedef struct {
  unsigned short OpCode;
  unsigned int   BodyLength;
  char           ProtocolVersion[3];
  char           CTN[11];
}tsDMSHeader;

__packed typedef struct {
  unsigned short OpCode;
  unsigned int   BodyLength;
  char           ReturnCode[3];
  unsigned short FotaAccTime;
  unsigned short QinfoAccTime;
  unsigned char  CustomTagLen;
  char           CustomTag[30];
  unsigned char  EchoTagLen;
  char           EchoTag[30];
}tsOnNotiResp;

__packed typedef struct {
  unsigned short OpCode;
  unsigned int   BodyLength;
  char           ReturnCode[3];
  char           Reset[5];
  unsigned short FotaAccTime;
  unsigned short QinfoAccTime;
  unsigned char  CrcLen;
  char           CrcVal[10];
  unsigned int   FotaLength;
  unsigned char  CustomTagLen;
  char           CustomTag[30];
  unsigned char  EchoTagLen;
  char           EchoTag[30];
}tsFotaResp;



unsigned char Fota_Reset;

u8 DMS_RxStep;
unsigned int DMS_RxCnt;
unsigned int DMS_RxLen;
u8 DMS_RxBuff[100];

#define REQ_PUSH 0
#define REQ_POLLING 1
typedef enum {
  DMS_ON_NOTI,
  DMS_OFF_NOTI,
  DMS_FOTA,
  DMS_QUAL_INFO,
  DMS_MODEM_QUAL_INFO
}teDMS_OPCODE;

char DMS_TxMsg[256];
u8 CpuUse,MemUse,ChkHW;

tsOnNotiResp OnNotiResp;
tsFotaResp FotaResp;


u8 DMS_DecodeStep;
unsigned long DMS_MsgIdx; 
unsigned long DMS_DecodeLen;

unsigned long FotaSaveLen, FotaRxLen;

u16 FotaRxCRC,FotaMemCRC;
u16 FotaBuffIdx;

/*
********************************************************************************
* Description : GetModemQualData
* Arguments   : qual_msg, timeout
* Return      : resp
* Note        : ATKTDEVSTAT + Recieve data
******************************************************************************** 
*/
char Qual_Msg[200];
unsigned char Qual_Msg_Len;
int GetModemQualData(unsigned int timeout) 
{
  
  u32  current, previous;
  int msg_len = 0;
  u8 idx = 0, data, data_flag = 0,err;
  char cmdResp[15] = {0};
  u8 resp = 0;

  memset(AT_Response, 0, 256);
  memset(Qual_Msg, 0, 200);

  ModemCommInit();
  
  sprintf(cmdResp, "KTDEVSTAT:");
  TxStrToModem("ATKTDEVSTAT\r\n");    
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
          printk("\r\nMODEM : ATKTDEVSTAT 100 bytes over ERROR\r\n");              
          resp = 0;        
          data_flag = 2;
        }
        if (strstr(AT_Response, cmdResp) != NULL)
          data_flag = 1;
      }
      else if(data_flag == 1) {
        if(data == ',') {
          if(msg_len > 0 && msg_len < 199)
            Qual_Msg[msg_len++] = '|';
        }
        else {
          if(msg_len == 0) {
            if(data >= '0' && data <= '9') {
              Qual_Msg[msg_len++] = data;
            }
          }
          else if(msg_len < 199) {
            if(data > 0x1F && data < 0x7E) {
              Qual_Msg[msg_len++] = data;
            }
            else {
              data_flag = 2;
            }
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
  
  Qual_Msg_Len = msg_len;
  resp = msg_len;
  return resp;
}

u32 Prev_ProgressFotaRxByte;
void DecodeDMS(u8 r)
{
  u32 fota_size, save_len, rsize;
  
  if(DMS_MsgIdx < 100)
    DMS_RxBuff[DMS_MsgIdx++] = r;
  DMS_RxCnt++;
  if(OnNotiRespFlag) {
    switch(DMS_DecodeStep) {
      case 0:
        if(DMS_MsgIdx >= 6) {
          memcpy((u8*)&OnNotiResp,DMS_RxBuff, 6);
          if(OnNotiResp.OpCode != 0x0002) {
            DMS_RxStep = 0;
            DMS_MsgIdx = 0;
            Urc.Mode = URCMODE_IDLE;
            Urc.RespFlag = 0;
            Urc.Cnt = 0;
            DMS_DecodeStep = 100;
            OnNotiStep = ON_NOTI_CLOSE;            
          }
          else {
            DMS_DecodeLen = 6;
            DMS_MsgIdx = 0;
            DMS_DecodeStep++;
          }
        }
        break;
      case 1:
        if(DMS_MsgIdx >= 8) {
          memcpy((u8*)OnNotiResp.ReturnCode,DMS_RxBuff,8);
          DMS_DecodeLen += 8;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 2:
        if(DMS_MsgIdx >= OnNotiResp.CustomTagLen) {
          memcpy((u8*)OnNotiResp.CustomTag,DMS_RxBuff,OnNotiResp.CustomTagLen);
          DMS_DecodeLen += OnNotiResp.CustomTagLen;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 3:
        if(DMS_MsgIdx >= 1) {
          OnNotiResp.EchoTagLen = DMS_RxBuff[0];
          DMS_DecodeLen += 1;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 4:
        if(DMS_MsgIdx >= OnNotiResp.EchoTagLen) {
          memcpy((u8*)OnNotiResp.EchoTag,DMS_RxBuff,OnNotiResp.EchoTagLen);
          DMS_DecodeLen += OnNotiResp.EchoTagLen;
          DMS_RxStep = 0;
          DMS_MsgIdx = 0;
          Urc.Mode = URCMODE_IDLE;
          Urc.RespFlag = 0;
          Urc.Cnt = 0;
          DMS_DecodeStep = 100;
          OnNotiStep = ON_NOTI_CLOSE;
        }
        break;
    }
  }
  else if(FotaRespFlag) {
    switch(DMS_DecodeStep) {
      case 0:
        if(DMS_MsgIdx >= 6) {
          memcpy((u8*)&FotaResp,DMS_RxBuff, 6);
          DMS_DecodeLen = 6;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 1:
        if(DMS_MsgIdx >= 12) {
          memcpy((u8*)FotaResp.ReturnCode,DMS_RxBuff, 12);
          DMS_DecodeLen += 12;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 2:
        if(DMS_MsgIdx >= 1) {
          FotaResp.CrcLen = DMS_RxBuff[0];
          DMS_DecodeLen += 1;
          DMS_MsgIdx = 0;
          if(FotaResp.CrcLen > 0)
            DMS_DecodeStep++;
          else
            DMS_DecodeStep+=2;
        }
        break;
      case 3:
        if(DMS_MsgIdx >= FotaResp.CrcLen) {
          memcpy((u8*)FotaResp.CrcVal,DMS_RxBuff, FotaResp.CrcLen);
          DMS_DecodeLen += FotaResp.CrcLen;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 4:
        if(DMS_MsgIdx >= 4) {
          memcpy((u8*)&FotaResp.FotaLength,DMS_RxBuff, 4);
          DMS_DecodeLen += 4;
          DMS_MsgIdx = 0;
          FotaRxLen = 0;
          FotaSaveLen = 0;
          FotaEnable = 0;
          if(FotaResp.FotaLength > 0) {
            if(strstr(FotaResp.ReturnCode, "200") != NULL) {
              FotaEnable = 1;
              memset(ModemBuff,0,NAND_PAGE_SIZE);
              FotaBuffIdx = 0;
              FotaRxCRC = 0;
              FotaMemCRC = 0;
              Prev_ProgressFotaRxByte = 0;
            }
            DMS_DecodeStep++;
          }
          else DMS_DecodeStep+=2;
        }
        break;
      case 5:
        FotaRxLen++;
        DMS_DecodeLen++;
        if(FotaBuffIdx < MODEM_TMPBUFF_SIZE)  ModemBuff[FotaBuffIdx++] = r;
        if(((DMS_DecodeLen%FotaRxBufferSize == 0) && FotaBuffIdx >= NAND_PAGE_SIZE) || FotaRxLen >= FotaResp.FotaLength) {
          if(FotaBuffIdx < NAND_PAGE_SIZE) save_len = FotaBuffIdx;
          else save_len = NAND_PAGE_SIZE;
          if(FotaEnable) {
            if(FotaSaveLen < FotaResp.FotaLength) {      
              Nand_Write_Buffer(ModemBuff, NAND_PROGRAM_ADDR + FotaSaveLen, save_len);
              FotaSaveLen += save_len;
              FotaRxCRC = getcrc16(FotaRxCRC, ModemBuff, save_len); 
              if(FotaBuffIdx > save_len) {
                FotaBuffIdx -= save_len;
                memcpy(&ModemBuff[0], &ModemBuff[save_len], FotaBuffIdx);
              }
              else
                FotaBuffIdx = 0;
            }
          }
          if(FotaRxLen >= FotaResp.FotaLength) {
            if(FotaSaveLen < FotaResp.FotaLength && FotaBuffIdx > 0) {
              Nand_Write_Buffer(ModemBuff, NAND_PROGRAM_ADDR + FotaSaveLen, FotaBuffIdx);
              FotaSaveLen += FotaBuffIdx;
              FotaRxCRC = getcrc16(FotaRxCRC, ModemBuff, FotaBuffIdx); 
            }
            DMS_DecodeStep++;
            DMS_MsgIdx = 0;
          }
          FotaWaitCount = 0;
        }
        if(DMS_DecodeLen%FotaRxBufferSize == 0) {
          rsize = ProgressFotaRxByte-Prev_ProgressFotaRxByte;
          ModemCommBuffCheck();
          printk("FOTA Progress %d/%d, %d, [%d(%d)]\r\n",FotaRxLen, FotaResp.FotaLength, RxBuffEmpty, ProgressFotaRxByte, rsize);
          Prev_ProgressFotaRxByte = ProgressFotaRxByte;
        }
        break;
              
      case 6:
        if(DMS_MsgIdx >= 1) {
          FotaResp.CustomTagLen = DMS_RxBuff[0];
          DMS_DecodeLen += 1;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 7:
        if(DMS_MsgIdx >= FotaResp.CustomTagLen) {
          memcpy((u8*)FotaResp.CustomTag, DMS_RxBuff, FotaResp.CustomTagLen);
          DMS_DecodeLen += FotaResp.CustomTagLen;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 8:
        if(DMS_MsgIdx >= 1) {
          FotaResp.EchoTagLen = DMS_RxBuff[0];
          DMS_DecodeLen += 1;
          DMS_MsgIdx = 0;
          DMS_DecodeStep++;
        }
        break;
      case 9:
        if(DMS_MsgIdx >= FotaResp.EchoTagLen) {
          memcpy((u8*)FotaResp.EchoTag, DMS_RxBuff, FotaResp.EchoTagLen);
          DMS_DecodeLen += FotaResp.EchoTagLen;
          DMS_RxStep = 0;
          DMS_MsgIdx = 0;
          Urc.Mode = URCMODE_IDLE;
          Urc.RespFlag = 0;
          Urc.Cnt = 0;
          if(FotaEnable) {
            if(FotaResp.EchoTagLen == 10  &&  strstr(FotaResp.EchoTag, "0x00000000") != NULL) {
              FotaMemCRC = 0;
              u16 nsize = NAND_PAGE_SIZE;
              u16 ncnt = FotaResp.FotaLength/nsize;
              u16 mod = FotaResp.FotaLength%nsize;
              u32 naddr;
              for(int i = 0; i < ncnt; i++) {
                naddr = NAND_PROGRAM_ADDR + (i * nsize);
                Nand_Read_Buffer(ModemBuff, naddr, nsize);
                FotaMemCRC =  getcrc16(FotaMemCRC, ModemBuff, nsize); 
              }
              if(mod > 0) {
                naddr = NAND_PROGRAM_ADDR + (ncnt * nsize);
                Nand_Read_Buffer(ModemBuff, naddr, mod);
                FotaMemCRC =  getcrc16(FotaMemCRC, ModemBuff, mod); 
              }
              
              if(FotaRxCRC == FotaMemCRC) {
                printk("Check wrote flash fileCRC: [%04X]\n", FotaMemCRC);
                fota_size = FotaResp.FotaLength+(4-(FotaResp.FotaLength%4));  //Fota Size를 4의 배수로..
                RTC_WriteBackupRegister(RTC_BKP_DR0, 10);  
                RTC_WriteBackupRegister(RTC_BKP_DR1, fota_size);  
                lcd_clear();
                lcd_printf(4,1,0,"Found New Ver");
                lcd_printf(4,2,0,"System Reset");   
                lcd_printf(4,3,0,"DMS FOTA");   

                vTaskDelay(1000);
                Fota_Reset = 1;
              }
            }
          }
          FotaEnable = 0; FotaRespFlag = 0;
          if(Save_SecData_Cnt > 0 && Power_Off_Flag) {
            Save_SecData_Flag = 1;
            Nand_Write_SaveData();
          }
          else vTaskDelay(1000);
          DMS_DecodeStep = 100;
          FotaStep = FOTA_CLOSE;
        }
        break;
    }
  }
}

/*
********************************************************************************
* Description : DMS_Rx_Process
* Arguments   : r
* Return      : none
* Note        : DMS Rx Data process
******************************************************************************** 
*/
u8 Test_Err;
void DMS_Rx_Process(u8  r)
{
  if(!OnNotiRespFlag && !FotaRespFlag) return;

  if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {  
    DecodeDMS(r);
    return;
  }
  switch(DMS_RxStep) {
    case 0:
      if(r == ',') {
        DMS_RxStep = 1;
        DMS_RxCnt = 0;
        DMS_RxLen = 0;
      }
      break;
    case 1: 
      if(r == '\r') {
        DMS_RxStep = 2;
      }
      else { 
        if(r >= '0' && r <= '9') {
          DMS_RxLen *= 10;
          DMS_RxLen += r-'0';
        }
//        else {
//            Urc.Mode = URCMODE_IDLE;
//            Urc.RespFlag = 0;
//            Urc.Cnt = 0;
//            Test_Err = r;
//        }
      }
      break;
    case 2:
      if(r == '\n') {
        if(DMS_RxLen > 0 && DMS_RxLen <= 1024)
          DMS_RxStep = 3;
        else {
          Urc.Mode = URCMODE_IDLE;
          Urc.RespFlag = 0;
          Urc.Cnt = 0;
          Test_Err = 2;
        }
      }
      else {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
        Test_Err = 3;
      }
      break;
    case 3:
      if(DMS_RxCnt < DMS_RxLen) {
          DecodeDMS(r);
          if(DMS_RxCnt == DMS_RxLen){
            DMS_RxStep = 0;
            Urc.Mode = URCMODE_IDLE;
            Urc.RespFlag = 0;
            Urc.Cnt = 0;
          }
      }
      else {
        DMS_RxStep = 0;
        Test_Err = 4;
      }
      break;
  }
}

/*
********************************************************************************
* Description : Modem_SocketSend
* Arguments   : len, sw
* Return      : resp
* Note        : Modem TCP socket open
******************************************************************************** 
*/
unsigned char Modem_DMSSend(int op, unsigned char req_type, unsigned char qual_type)
{
  unsigned char resp = 0; //,rcnt = 0;
  unsigned int mlen = 0, size, tmp = 0;
  tsDMSHeader dms_header;
  unsigned char modem_type = 0;
  
  if(Setting.ExtModemMode == KT_GVN_PROTOCOL)  {
    if(Setting.ExtModemModel == 0) modem_type = 1;
  }
  else return 0;

  memset(DMS_TxMsg,0,256);
  if(op == DMS_ON_NOTI) {
    dms_header.OpCode = 0x1000;
    sprintf(dms_header.ProtocolVersion,"1.0");
    memcpy((u8*)dms_header.CTN,(u8*)ModemCTN,11);
    mlen = 20;

    DMS_TxMsg[mlen] = StringLen((char*)Model_Name_KT,15);
    memcpy(&DMS_TxMsg[mlen+1], Model_Name_KT, DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    DMS_TxMsg[mlen] = 7;
    memcpy(&DMS_TxMsg[mlen+1], Fota_FW_Ver, DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    DMS_TxMsg[mlen] = 14;
    if(Setting.Serial_No[0] < '0' || Setting.Serial_No[0] > 'z' )
      memset((u8*)&DMS_TxMsg[mlen+1],0x30,DMS_TxMsg[mlen]);
    else
      memcpy((u8*)&DMS_TxMsg[mlen+1],Setting.Serial_No,DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    DMS_TxMsg[mlen] = 10;
    sprintf(&DMS_TxMsg[mlen+1],"0x00000000");
    mlen += (DMS_TxMsg[mlen]+1);

    dms_header.BodyLength = mlen-6;
    memcpy((u8*)DMS_TxMsg, (u8*)&dms_header,20);
  }
  else if(op == DMS_OFF_NOTI) {
    dms_header.OpCode = 0x1002;
    sprintf(dms_header.ProtocolVersion,"1.0");
    memcpy((u8*)dms_header.CTN,(u8*)ModemCTN,11);
    mlen = 20;

    DMS_TxMsg[mlen] = StringLen((char*)Model_Name_KT,15);
    memcpy(&DMS_TxMsg[mlen+1], Model_Name_KT, DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    DMS_TxMsg[mlen] = 14;
    if(Setting.Serial_No[0] < '0' || Setting.Serial_No[0] > 'z' )
      memset((u8*)&DMS_TxMsg[mlen+1],0x30,DMS_TxMsg[mlen]);
    else
      memcpy((u8*)&DMS_TxMsg[mlen+1],Setting.Serial_No,DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    dms_header.BodyLength = mlen-6;
    memcpy((u8*)DMS_TxMsg, (u8*)&dms_header,20);
  }
  else if(op == DMS_FOTA) {
    dms_header.OpCode = 0x0001;
    sprintf(dms_header.ProtocolVersion,"1.0");
    memcpy((u8*)dms_header.CTN,(u8*)ModemCTN,11);
    mlen = 20;

    DMS_TxMsg[mlen] = StringLen((char*)Model_Name_KT,15);
    memcpy(&DMS_TxMsg[mlen+1], Model_Name_KT, DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    if(req_type == REQ_PUSH)
      sprintf(&DMS_TxMsg[mlen],"PUSH");
    else if(req_type == REQ_POLLING)
      sprintf(&DMS_TxMsg[mlen],"POLLING");
    mlen += 7;
    
    DMS_TxMsg[mlen] = 7;
    memcpy(&DMS_TxMsg[mlen+1], Fota_FW_Ver, DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    
    if(Setting.ExtModemModel == 0 || Setting.ExtModemModel == 1) 
      FotaRxBufferSize = FOTA_BUFFER_SIZE;
    else
      FotaRxBufferSize = 512;
    tmp = FotaRxBufferSize;
    memcpy(&DMS_TxMsg[mlen],&tmp,2);
    mlen += 2;
    tmp = FOTA_PKG_DELAY;
    memcpy(&DMS_TxMsg[mlen],&tmp,2);
    mlen += 2;
    
    DMS_TxMsg[mlen] = 14;
    if(Setting.Serial_No[0] < '0' || Setting.Serial_No[0] > 'z' )
      memset((u8*)&DMS_TxMsg[mlen+1],0x30,DMS_TxMsg[mlen]);
    else
      memcpy((u8*)&DMS_TxMsg[mlen+1],Setting.Serial_No,DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    DMS_TxMsg[mlen] = 10;
    sprintf(&DMS_TxMsg[mlen+1],"0x00000000");
    mlen += (DMS_TxMsg[mlen]+1);

    dms_header.BodyLength = mlen-6;
    memcpy((u8*)DMS_TxMsg, (u8*)&dms_header,20);
  }
  else if(op == DMS_QUAL_INFO) {
    dms_header.OpCode = 0x0100;
    sprintf(dms_header.ProtocolVersion,"1.0");
    memcpy((u8*)dms_header.CTN,(u8*)ModemCTN,11);
    mlen = 20;

    DMS_TxMsg[mlen] = StringLen((char*)Model_Name_KT,15);
    memcpy(&DMS_TxMsg[mlen+1], Model_Name_KT, DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    if(req_type == REQ_PUSH)
      sprintf(&DMS_TxMsg[mlen],"PUSH");
    else if(req_type == REQ_POLLING)
      sprintf(&DMS_TxMsg[mlen],"POLLING");
    mlen += 7;
    
    if(qual_type == 0)
      DMS_TxMsg[mlen] = 'A';
    else 
      DMS_TxMsg[mlen] = 'M';
    mlen += 1;

    if(modem_type == 0)
      DMS_TxMsg[mlen] = 'L';
    else 
      DMS_TxMsg[mlen] = 'W';
    mlen += 1;
    
    size = Qual_Msg_Len;
    DMS_TxMsg[mlen] = size;
    if(size > 0) 
      memcpy((u8*)&DMS_TxMsg[mlen+1],(u8*)Qual_Msg,size);
    mlen += (size+1);
    
    memset(MdmTmpString,0,100);
    memcpy((u8*)&tmp,(u8*)&SecData.Status,2);
    CpuUse = 40;
    MemUse = 80;
    ChkHW = 0;
    sprintf(MdmTmpString,"%d|%d|%d|ERR%04X",CpuUse,MemUse,ChkHW,tmp);
    size = StringLen(MdmTmpString,100);
    DMS_TxMsg[mlen] = size;
    memcpy((u8*)&DMS_TxMsg[mlen+1],(u8*)MdmTmpString,size);
    mlen += (size+1);
    
    DMS_TxMsg[mlen] = 14;
    if(Setting.Serial_No[0] < '0' || Setting.Serial_No[0] > 'z' )
      memset((u8*)&DMS_TxMsg[mlen+1],0x30,DMS_TxMsg[mlen]);
    else
      memcpy((u8*)&DMS_TxMsg[mlen+1],Setting.Serial_No,DMS_TxMsg[mlen]);
    mlen += (DMS_TxMsg[mlen]+1);

    dms_header.BodyLength = mlen-6;
    memcpy((u8*)DMS_TxMsg, (u8*)&dms_header,20);
  }
  else if(op == DMS_MODEM_QUAL_INFO) {
    dms_header.OpCode = 0x0101;
    sprintf(dms_header.ProtocolVersion,"1.0");
    memcpy((u8*)dms_header.CTN,(u8*)ModemCTN,11);
    mlen = 20;

    if(modem_type == 0)
      DMS_TxMsg[mlen] = 'L';
    else 
      DMS_TxMsg[mlen] = 'W';
    mlen += 1;
    
    size = Qual_Msg_Len;
    DMS_TxMsg[mlen] = size;
    if(size > 0) 
      memcpy((u8*)&DMS_TxMsg[mlen+1],(u8*)Qual_Msg,size);
    mlen += (size+1);

    dms_header.BodyLength = mlen-6;
    memcpy((u8*)DMS_TxMsg, (u8*)&dms_header,20);
  }  
  resp = 0;
  if(mlen > 0) {
    if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {
      if(op == DMS_ON_NOTI || op == DMS_FOTA)  {
        ModemCommInit();
        Urc.Mode = URCMODE_DMS;
        Urc.RespFlag = 1;
        Urc.Cnt = 0;
        memset(Urc.Buff,0, URC_BUFF_MAX);
      }
      else {
        Urc.Mode = URCMODE_IDLE;
        Urc.RespFlag = 0;
        Urc.Cnt = 0;
      }

      DMS_RxStep = 0;
      DMS_RxCnt = 0;
      DMS_RxLen = 0;
      DMS_MsgIdx = 0;
      DMS_DecodeStep = 0;
      DMS_DecodeLen = 0; 
      if(op == DMS_ON_NOTI) {
        memset((u8*)&OnNotiResp,0, sizeof(tsOnNotiResp));
      }
      if(op == DMS_FOTA) {
        memset((u8*)&FotaResp,0, sizeof(tsFotaResp));
      }
      
      TxDataStrToModem((u8*)DMS_TxMsg, mlen);
      
      resp = 1;
    }
    else {
//      memset(MdmTmpString,0,100);
//      sprintf(MdmTmpString,"AT+QISEND=0,%u\r\n",mlen); 
//      while(resp == 0 && rcnt < AT_RETRY_CNT) { 
//        resp = sendATcommand2(MdmTmpString, '>', "ERROR", 2000); //Maximum Response Time 300ms
//        vTaskDelay(1);
//        rcnt++;
//      }
//      if(rcnt >= AT_RETRY_CNT) {
//        if(resp == 0) {
//          UART_PutDataStr3((u8*)DMS_TxMsg, mlen);
//        }
//        printk("\r\nMODEM : AT QISEND ERROR\r\n");    
//        return resp;
//      }
//      UART_PutDataStr3((u8*)DMS_TxMsg, mlen);
    }
    printk("\r\nMODEM : Socket Send (DMS) %u\n", resp);
  }  
  return resp;
}

/*
********************************************************************************
* Description : Modem_OnNotiProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
void Modem_OnNotiProcess_DMS(void)
{
  u16 addr;

  if(!OnNotiFlag || Setting.ExtModemMode != KT_GVN_PROTOCOL) {
    OnNotiStep = ON_NOTI_IDLE;
    return;
  }
  switch(OnNotiStep) {
    case ON_NOTI_CONNECT:
      Modem_SocketConnect(DmSvr.SvrName, DmSvr.SvrPort, 3);
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
      if(Modem_DMSSend(DMS_ON_NOTI,0,0)==1) {
        OnNotiStep = ON_NOTI_RECIEVE_WAIT;
        OnNotiWaitCount = 0;
        OnNotiRespFlag = 1;
      }
      else {
        if(Modem_SocketClose(0)==1)
          OnNotiStep = ON_NOTI_CONNECT;
        else {
          OnNotiStep = ON_NOTI_FAIL;
        }
      }
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
        ModemStep = MODEM_IDLE;
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      break;

    case ON_NOTI_SUCCESS:
      printk("\r\nMODEM : On-Notification Success\r\n");
      if(strstr(OnNotiResp.ReturnCode, "210") != NULL) {
        if(OnNotiResp.FotaAccTime < 10000 && ModemInfo.FotaPollingIntrval != OnNotiResp.FotaAccTime) {
          ModemInfo.FotaPollingIntrval = OnNotiResp.FotaAccTime;
          addr = MODEM_INFO + (u8 *)&ModemInfo.FotaPollingIntrval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.FotaPollingIntrval, addr, sizeof(ModemInfo.FotaPollingIntrval));
        }
        if(OnNotiResp.QinfoAccTime < 10000 && ModemInfo.QualPollingIntrval != OnNotiResp.QinfoAccTime) {
          ModemInfo.QualPollingIntrval = OnNotiResp.QinfoAccTime;
          addr = MODEM_INFO + (u8 *)&ModemInfo.QualPollingIntrval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.QualPollingIntrval, addr, sizeof(ModemInfo.QualPollingIntrval));
        }
      }

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


/*
********************************************************************************
* Description : Modem_QualNotiProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/

void Modem_QualNotiProcess_DMS(void)
{
  u8 req, op;
  
  if(!QualNotiFlag || Setting.ExtModemMode != KT_GVN_PROTOCOL) {
    QualNotiStep = QUAL_NOTI_IDLE;
    return;
  }
  switch(QualNotiStep) {
    case QUAL_NOTI_GETDATA:
      if(TcpState != 1 || QualNotiWaitCount > 5000) {
        QualNotiWaitCount = 0;
        GetModemQualData(3000); 
        QualNotiStep = QUAL_NOTI_CONNECT;
      }
      else {
        QualNotiWaitCount++;
      }
      break;
      
    case QUAL_NOTI_CONNECT:
      Modem_SocketConnect(QualSvr.SvrName, QualSvr.SvrPort,4);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          QualNotiStep = QUAL_NOTI_SEND;
        }
        else
          QualNotiStep = QUAL_NOTI_FAIL;
      }
      break;
      
    case QUAL_NOTI_SEND:
      if(QualNotiFlag == 1)  req = REQ_POLLING;
      else    req = REQ_PUSH;
      if(QualNotiFlag == 3) op = DMS_MODEM_QUAL_INFO;
      else    op = DMS_QUAL_INFO;
      if(Modem_DMSSend(op,req,0)==1) {
        QualNotiStep = QUAL_NOTI_CLOSE;//QUAL_NOTI_SEND_WAIT;
        QualNotiWaitCount = 0;
      }
      else {
        if(Modem_SocketClose(0)==1)
          QualNotiStep = QUAL_NOTI_CONNECT;
        else {
          QualNotiStep = QUAL_NOTI_FAIL;
        }
      }
      break;
      
    case QUAL_NOTI_CLOSE:
      if(Modem_SocketClose(0)==1) {
        QualNotiStep = QUAL_NOTI_SUCCESS;
      }
      else
        QualNotiStep = QUAL_NOTI_FAIL;
      break;

    case QUAL_NOTI_FAIL:
      printk("\r\nMODEM : Qual-Notification error\r\n");
      ModemWaitCnt = 0;
      QualNotiRetryCount++;
      if(QualNotiRetryCount >= 3) {
        QualNotiFlag = 0;
        QualNotiRetryCount = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON;
        Modem_PPPClose();
      }
      else {
        ModemWaitCnt = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      }
      break;

    case QUAL_NOTI_SUCCESS:
      printk("\r\nMODEM : Qual-Notification Success\r\n");
      ModemWaitCnt = 0;
      QualNotiFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      if(!FotaFlag)  Modem_PPPClose();
      break;

    case QUAL_NOTI_IDLE:
      ModemWaitCnt = 0;
      QualNotiFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      QualNotiStep = QUAL_NOTI_FAIL;
      break;  
  }
}

/*
********************************************************************************
* Description : Modem_OffNotiProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
void Modem_OffNotiProcess_DMS(void)
{
  if(!OffNotiFlag || Setting.ExtModemMode != KT_GVN_PROTOCOL) {
    OffNotiStep = OFF_NOTI_IDLE;
    return;
  }
  switch(OffNotiStep) {
    case OFF_NOTI_CONNECT:
      Modem_SocketConnect(DmSvr.SvrName, DmSvr.SvrPort, 3);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          OffNotiStep = OFF_NOTI_SEND;
        }
        else
          OffNotiStep = OFF_NOTI_FAIL;
      }
      break;
      
    case OFF_NOTI_SEND:
      if(Modem_DMSSend(DMS_OFF_NOTI,0,0)==1) {
        OffNotiStep = OFF_NOTI_CLOSE;
        OffNotiWaitCount = 0;
      }
      else {
        if(Modem_SocketClose(0)==1)
          OffNotiStep = OFF_NOTI_CONNECT;
        else {
          OffNotiStep = OFF_NOTI_FAIL;
        }
      }
      break;
      
    case OFF_NOTI_CLOSE:
      if(Modem_SocketClose(0)==1) {
        OffNotiStep = OFF_NOTI_SUCCESS;
      }
      else
        OffNotiStep = OFF_NOTI_FAIL;
      break;

    case OFF_NOTI_FAIL:
      printk("\r\nMODEM : Off-Notification error\r\n");
      ModemWaitCnt = 0;
      OffNotiRetryCount++;
      if(OffNotiRetryCount >= 3) {
        OffNotiRetryCount = 0;
        Modem_PPPClose();
        ModemStep = MODEM_POWER_OFF;
      }
      else {
        ModemWaitCnt = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      }
      break;

    case OFF_NOTI_SUCCESS:
      printk("\r\nMODEM : Off-Notification Success\r\n");
      ModemWaitCnt = 0;
      OffNotiFlag = 0;
//      OffNotiStep = OFF_NOTI_CONNECT;
      Modem_PPPClose();
      ModemStep = MODEM_POWER_OFF;
      break;

    case OFF_NOTI_IDLE:
      ModemWaitCnt = 0;
      OffNotiFlag = 0;
//      OffNotiStep = OFF_NOTI_CONNECT;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      OffNotiStep = OFF_NOTI_FAIL;
      break;  
  }
}

/*
********************************************************************************
* Description : Modem_FotaProcess
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
u8 FotaType;
void Modem_FotaProcess_DMS(void)
{
  u8 req;
  u16 addr;
  
  if(!FotaFlag || Setting.ExtModemMode != KT_GVN_PROTOCOL) {
    FotaStep = FOTA_IDLE;
    return;
  }
  switch(FotaStep) {
    case FOTA_CONNECT:
      FotaType = 0;
      Modem_SocketConnect(DmSvr.SvrName, DmSvr.SvrPort, 3);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          FotaStep = FOTA_SEND;
        }
        else
          FotaStep = FOTA_FAIL;
      }
      break;
      
    case FOTA_SEND:
      if(FotaFlag == 1)  req = REQ_POLLING;
      else    req = REQ_PUSH;
      FotaType = FotaFlag;
      if(Modem_DMSSend(DMS_FOTA,req,0)==1) {
        FotaStep = FOTA_RECIEVE_WAIT;
        FotaWaitCount = 0;
        FotaRespFlag = 1;
        ProgressFotaRxByte = 0;
      }
      else {
        if(Modem_SocketClose(0)==1)
          FotaStep = FOTA_CONNECT;
        else {
          FotaStep = FOTA_FAIL;
        }
      }
      break;
      
    case FOTA_RECIEVE_WAIT:
      FotaWaitCount++;
      if(FotaWaitCount > 10000 ) {
        if(FotaRxLen > 0) {
          ModemCommBuffCheck();
          printk("FOTA Progress %d/%d, %d, [%d(%d)]\r\n",FotaRxLen, FotaResp.FotaLength, RxBuffEmpty, ProgressFotaRxByte, ProgressFotaRxByte-Prev_ProgressFotaRxByte);
        }
        else
          printk("FOTA Sever - No Response\r\n");
        FotaRespFlag = 0;
        FotaEnable = 0;
        FotaWaitCount = 0;
        Modem_SocketClose(0);
        FotaStep = FOTA_FAIL;
      }
      break;

    case FOTA_CLOSE:
      if(Modem_SocketClose(0)==1) {
        FotaStep = FOTA_SUCCESS;
      }
      else
        FotaStep = FOTA_FAIL;
      break;

    case FOTA_FAIL:
      printk("\r\nMODEM : FOTA Request error\r\n");
      FotaRespFlag = 0;
      FotaEnable = 0;
      ModemWaitCnt = 0;
      FotaRetryCount++;
      if(FotaRetryCount >= 1) {
        if(FotaFlag == 2 && FotaType == 1)
          FotaRetryCount = 0;
        else
          FotaFlag = 0;
        QualNotiRetryCount = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON;
        Modem_PPPClose();
      }
      else {
        ModemWaitCnt = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      }
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;      
      break;

    case FOTA_SUCCESS:
      if(FotaRxCRC == FotaMemCRC || FotaResp.FotaLength == 0)
        printk("\r\nMODEM : FOTA Request Success\r\n");
      if((strstr(FotaResp.ReturnCode, "200") != NULL) || (strstr(FotaResp.ReturnCode, "204") != NULL)) {
        if(FotaResp.FotaAccTime < 10000 && ModemInfo.FotaPollingIntrval != FotaResp.FotaAccTime) {
          ModemInfo.FotaPollingIntrval = FotaResp.FotaAccTime;
          addr = MODEM_INFO + (u8 *)&ModemInfo.FotaPollingIntrval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.FotaPollingIntrval, addr, sizeof(ModemInfo.FotaPollingIntrval));
        }
        if(FotaResp.QinfoAccTime < 10000 && ModemInfo.QualPollingIntrval != FotaResp.QinfoAccTime) {
          ModemInfo.QualPollingIntrval = FotaResp.QinfoAccTime;
          addr = MODEM_INFO + (u8 *)&ModemInfo.QualPollingIntrval - (u8 *)&ModemInfo;
          FRAMMultiWrite((u8 *)&ModemInfo.QualPollingIntrval, addr, sizeof(ModemInfo.QualPollingIntrval));
        }
      }
      ModemWaitCnt = 0;
      if(FotaFlag == 2 && FotaType == 1)
        FotaRetryCount = 0;
      else
        FotaFlag = 0;
      FotaRespFlag = 0;
      FotaEnable = 0;
      ModemStep = MODEM_DATA_IDLE;
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
      Modem_PPPClose();
      if(Fota_Reset)
        NVIC_SystemReset();
      break;

    case FOTA_IDLE:
      ModemWaitCnt = 0;
      FotaFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      FotaStep = FOTA_FAIL;
      break;  
  }
}

void Modem_SecInt_DMS(void)
{
#ifdef __USE_DMS_SERVER__          
  if(!Power_Off_Flag) {
    FotaPolTimeCnt++;
    if(ModemInfo.FotaPollingIntrval > 0 && ModemInfo.FotaPollingIntrval < 10000) {
      if(FotaPolTimeCnt >= ModemInfo.FotaPollingIntrval*60) {
        FotaPolTimeCnt = 0;
//              FotaFlag = 1;
        FotaRetryCount = 0;
      }
    }
    QualPolTimeCnt++;
    if(ModemInfo.QualPollingIntrval > 0 && ModemInfo.QualPollingIntrval < 10000) {
      if(QualPolTimeCnt >= ModemInfo.QualPollingIntrval*60) {
        QualPolTimeCnt = 0;
        QualNotiFlag = 1;  
        QualNotiRetryCount = 0;
      }
    }
  }
#endif
}

void Modem_Parameter_Reset_DMS(void)
{
  ModemInfo.FotaPollingIntrval = 0;
  FRAMMultiWrite((u8 *)&ModemInfo.FotaPollingIntrval, MODEM_INFO + (u8 *)&ModemInfo.FotaPollingIntrval - (u8 *)&ModemInfo, sizeof(ModemInfo.FotaPollingIntrval));          
  ModemInfo.QualPollingIntrval = 0;
  FRAMMultiWrite((u8 *)&ModemInfo.QualPollingIntrval, MODEM_INFO + (u8 *)&ModemInfo.QualPollingIntrval - (u8 *)&ModemInfo, sizeof(ModemInfo.QualPollingIntrval));          

  memset(DmSvr.SvrName, 0, sizeof(DmSvr.SvrName));
#ifdef __USE_DMS_TEST_SERVER__  
  sprintf(DmSvr.SvrName,"devicefota-tb-dm.show.co.kr");         // KT 인증  DM 서버.
  DmSvr.SvrPort = 8002;
#else  
  sprintf(DmSvr.SvrName,"devicefota-dm.show.co.kr");            // KT 상용  DM 서버.
  DmSvr.SvrPort = 8002;
#endif  
  FRAMMultiWrite((u8 *)&DmSvr, DMS_SVR_INFO, sizeof(tsSvrInfo));

  memset(QualSvr.SvrName, 0, sizeof(QualSvr.SvrName));
#ifdef __USE_DMS_TEST_SERVER__  
  sprintf(QualSvr.SvrName,"devicefota-tb-quality.show.co.kr");              // KT 인증  Quality 서버.
  QualSvr.SvrPort = 8002;
#else  
  sprintf(QualSvr.SvrName,"devicefota-quality-automotive.show.co.kr");      // KT 상용  Quality 서버.
  QualSvr.SvrPort = 8002;
#endif  
  FRAMMultiWrite((u8 *)&QualSvr, QUAL_SVR_INFO, sizeof(tsSvrInfo));
}

void Modem_Init_DMS(void)
{
  FRAMMultiRead((u8 *)&ModemInfo, MODEM_INFO, sizeof(tsModemInfo));
  
  if(ModemInfo.FotaPollingIntrval > 9999) {
    ModemInfo.FotaPollingIntrval = 0;
    FRAMMultiWrite((u8 *)&ModemInfo.FotaPollingIntrval, MODEM_INFO + (u8 *)&ModemInfo.FotaPollingIntrval - (u8 *)&ModemInfo, sizeof(ModemInfo.FotaPollingIntrval));          
  }
  if(ModemInfo.QualPollingIntrval > 9999) {
    ModemInfo.QualPollingIntrval = 0;
    FRAMMultiWrite((u8 *)&ModemInfo.QualPollingIntrval, MODEM_INFO + (u8 *)&ModemInfo.QualPollingIntrval - (u8 *)&ModemInfo, sizeof(ModemInfo.QualPollingIntrval));          
  }

  FRAMMultiRead((u8 *)&DmSvr, DMS_SVR_INFO, sizeof(tsSvrInfo));
  if(DmSvr.SvrName[0] < ' ' || DmSvr.SvrName[0] > 'z' || DmSvr.SvrPort == 0 || DmSvr.SvrPort > 20000) {
    memset(DmSvr.SvrName, 0, sizeof(DmSvr.SvrName));
#ifdef __USE_DMS_TEST_SERVER__ 
    sprintf(DmSvr.SvrName,"devicefota-tb-dm.show.co.kr");         // KT 인증  DM 서버.
    DmSvr.SvrPort = 8002;
#else    
    sprintf(DmSvr.SvrName,"devicefota-dm.show.co.kr");            // KT 상용  DM 서버.
    DmSvr.SvrPort = 8002;
#endif    
    FRAMMultiWrite((u8 *)&DmSvr, DMS_SVR_INFO, sizeof(tsSvrInfo));
  }

  FRAMMultiRead((u8 *)&QualSvr, QUAL_SVR_INFO, sizeof(tsSvrInfo));
  if(QualSvr.SvrName[0] < ' ' || QualSvr.SvrName[0] > 'z' || QualSvr.SvrPort == 0 || QualSvr.SvrPort > 20000) {
    memset(QualSvr.SvrName, 0, sizeof(QualSvr.SvrName));
#ifdef __USE_DMS_TEST_SERVER__ 
    sprintf(QualSvr.SvrName,"devicefota-tb-quality.show.co.kr");              // KT 인증  Quality 서버.
    QualSvr.SvrPort = 8002;
#else    
    sprintf(QualSvr.SvrName,"devicefota-quality-automotive.show.co.kr");      // KT 상용  Quality 서버.
    QualSvr.SvrPort = 8002;
#endif    
    FRAMMultiWrite((u8 *)&QualSvr, QUAL_SVR_INFO, sizeof(tsSvrInfo));
  }
  
#ifdef __USE_DMS_SERVER__          
  OnNotiFlag = 1;  //NetKTI Test
  QualNotiFlag = 1;
  FotaFlag = 1;   
#else  
  ModemInfo.QualPollingIntrval = 0;
  ModemInfo.FotaPollingIntrval = 0;  
#endif  
  OnNotiRetryCount = 0;
  QualNotiRetryCount = 0;
  QualPolTimeCnt = 0;
  FotaPolTimeCnt = 0;
  FotaRetryCount = 0;
  FotaRxBufferSize = FOTA_BUFFER_SIZE;
}

void Modem_DataIdleProcess_DMS(void)
{
#ifdef __USE_DMS_SERVER__          
  if(DataServerFlag) return;
  
  if(PushResetFlag) {
    if(PushResetFlag == 1)
      Modem_ResetProcess(); 
    else if(PushResetFlag == 2) {
      //OffNotiFlag = 1;
      OffAdasFlag = 1;
      PushResetStart = 1;
//      Modem_PowerOff();
//      NVIC_SystemReset();
    }
    PushResetFlag = 0;
    return;
  }
  
  if(OnNotiFlag) {
    OnNotiStep = ON_NOTI_CONNECT;
    OnNotiWaitCount = 0;
    ModemStep = MODEM_ON_NOTI;    
  }
  else if(QualNotiFlag) {
    QualNotiStep = QUAL_NOTI_GETDATA;//QUAL_NOTI_CONNECT;
    QualNotiWaitCount = 0;
    ModemStep = MODEM_QUAL_NOTI;    
  }
  else if(OffNotiFlag) {
    OffNotiStep = OFF_NOTI_CONNECT;
    OffNotiWaitCount = 0;
    ModemStep = MODEM_OFF_NOTI;    
  }
  else if(FotaFlag) {
    FotaStep = FOTA_CONNECT;
    FotaWaitCount = 0;
    ModemStep = MODEM_FOTA_REQ;    
  }
  else {
    if(Setting.ExtModemMode == KT_GVN_PROTOCOL && Prev_DataFlag)    DataServerFlag = 1;
  }
#endif                    
}



/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/
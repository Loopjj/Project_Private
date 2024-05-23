/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : test_jig.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 06/03/2018
*
* Description        : uart 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define TEST_JIG_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
#define TJ_COM_BUFFER_SIZE 4096

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 TJ_RxCommand;
u8 TJ_RxStep,TJ_SyncCnt;
u16 TJ_RxLength, TJ_RxDataCnt;
u8 TJ_LenType;
u8 TJ_ChkSum;
u8 TJ_Decode_Flag;
u8 TJ_ComBuf[TJ_COM_BUFFER_SIZE];
u8 Wait_flag = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void TestJIgTxData(u8 type, u8 command, u8 *data, u16 len)
{
    u8 sdata[10], tdata, check_sum;
    u16 cnt = 0;
    u8 ls[2];

    sdata[cnt++] = 0xc6;
    sdata[cnt++] = 0xc6;
    
    if(type == 1) {
      sdata[cnt++] = 0x7e;
      sdata[cnt++] = len;
      sdata[cnt++] = command;
      check_sum = len^command;            //check_sum = len^command 
    }
    else {
      sdata[cnt++] = 0x7A;
      memcpy(ls,&len,sizeof(len));
      sdata[cnt++] = ls[0];
      sdata[cnt++] = ls[1];
      check_sum = ls[0];
      check_sum ^= ls[1];

      sdata[cnt++] = command;
      check_sum ^= command;
    }
    for(int i = 0; i < cnt; i++)           
      UART_SendData(&huart3, sdata[i]);
    for(int i = 0; i < len; i++) {
        tdata = *data++;
        check_sum ^= tdata;                  //check_sum ^ = tdata(64byte)
        UART_SendData(&huart3, tdata);
    }    
    check_sum++;
    UART_SendData(&huart3, check_sum);
    UART_SendData(&huart3, 0x7d);
}
/*
********************************************************************************
* Description : Decode RxData (PC UI) 
* Arguments   : None
* Return      : 
* Note        : Decode RxData from PC UI 
******************************************************************************** 
*/
void DecodeRxData(unsigned char rdata)
{          
    switch(TJ_RxStep) {
        case 0:
          if(rdata == 0xc6) {
            TJ_SyncCnt++;}
          else              TJ_SyncCnt = 0;
          if(TJ_SyncCnt == 2) {
             TJ_RxStep  = 1;
             TJ_SyncCnt = 0;
          }
          break;
          
        case 1:
          if(rdata == 0x7e || rdata == 0x7A) {
            TJ_RxStep = 2;
            TJ_RxLength = 0;
            TJ_ChkSum = 0;
            if(rdata == 0x7E)      TJ_LenType = 1;
            else if(rdata == 0x7A) TJ_LenType = 2;
          }
          else TJ_RxStep = 0;
          break;
          
        case 2:
          TJ_ChkSum ^= rdata;
          TJ_RxLength += rdata<<(8*TJ_SyncCnt) ;
          TJ_SyncCnt++;
          if(TJ_SyncCnt == TJ_LenType) {
            if(TJ_RxLength <= TJ_COM_BUFFER_SIZE)
              TJ_RxStep = 3;
            else 
              TJ_RxStep = 0;
            TJ_SyncCnt = 0;            
          }
          break;
          
        case 3:
          TJ_ChkSum ^= rdata;
          TJ_RxCommand = rdata;
          TJ_RxDataCnt = 0;
          if(TJ_RxLength > 0)
            TJ_RxStep = 4;
          else
            TJ_RxStep = 5;
          break;
          
        case 4:
          TJ_ChkSum ^= rdata;
          TJ_ComBuf[TJ_RxDataCnt++] = rdata;
          if(TJ_RxDataCnt == TJ_RxLength)
            TJ_RxStep = 5;
          break;
          
        case 5:
          if(TJ_ChkSum+1 == rdata)
            TJ_RxStep = 6;
          else 
            TJ_RxStep = 0;
          break;
          
        case 6:
          TJ_RxStep = 0;
          TJ_SyncCnt = 0;
          if(rdata == 0x7d) 
            TJ_Decode_Flag = 1;               
          break;
    }
     Decode_Command();
}
unsigned char Test[6];
/*
********************************************************************************
* Description : Decode RxData (PC UI) 
* Arguments   : None
* Return      : 
* Note        : Decode RxData from PC UI 
******************************************************************************** 
*/
void Decode_Command(void)
{
  INT16U limit, ad_Val, temp_Val;
  double Cal_Val = 0;
  
  switch(TJ_RxCommand) {
    case 0x48: // 출력신호 데이터
      Pedal_ADC[0] = (Test[0]<<8) + Test[1];
      Pedal_ADC[1] = (Test[2]<<8) + Test[3];
      Pedal_ADC[2] = (Test[4]<<8) + Test[5];
      if(Pedal_ADC[0] < 300) Pedal_ADC[2] |= 0x02;
      if(Pedal_ADC[1] < 300) Pedal_ADC[2] |= 0x20;
      limit = Pedal_ADC[0] - Pedal_ADC[1];
      if(limit > 0 && limit <= 15) Pedal_ADC[1] = Pedal_ADC[0];
      if(Pedal_ADC[0] <= 300) {
        Pedal_ADC[0] = perPedal_ADC[0];
        Pedal_ADC[1] = perPedal_ADC[1];
      }
      if(SecData.UreaAccFlag) {
        temp_Val = (INT16U)((1.0 - ((double)Pedal_ADC[1]/(double)Pedal_ADC[0])) * 10000.0);
        if(SecData.UreaAccFlag == 1) {
          if(temp_Val > 1150 || temp_Val < 1000) {
            Cal_Val = (double)(temp_Val % 100) / 10000.0;
            ad_Val = (INT16U)((double)Pedal_ADC[0]-((double)Pedal_ADC[0]*(0.1 + Cal_Val)));
            Pedal_ADC[1] = ad_Val;
          }
        }
        else if(SecData.UreaAccFlag == 2) {
          if(temp_Val > 2650 || temp_Val < 2500) {
            Cal_Val = (double)(temp_Val % 100) / 10000.0;
            ad_Val = (INT16U)((double)Pedal_ADC[0]-((double)Pedal_ADC[0]*(0.25 + Cal_Val)));
            Pedal_ADC[1] = ad_Val;
          }
        }
      }
      else {
        Pedal_ADC[1] = Pedal_ADC[0];
      }
      perPedal_ADC[0] = Pedal_ADC[0];
      perPedal_ADC[1] = Pedal_ADC[1];
      break;
      
    case 0x60: // 테스트 모드 종료 
       AccTestFlag = 0;
       CheckUreaAlarm();
      break;
      
    case 0x30: //Test Step 0 정상  
       AccTestFlag = 1;    
       UreaAlarm.Quality = 0;
       UreaAlarm.Status = 0;
       UreaAlarm.SysErr = 0;
       UreaTestFlag = 0;

  if(!UreaTestFlag) { // Testflag 가 0일경우 
    UreaAlarm.Level = 0;
    if(UreaAccControl.Level != 0) {
      DO1_RESET();
      DO2_RESET();
      SecData.UreaAccFlag = 0;
      UreaAlarmLapTime = 0;
      UreaAccControl.Level = 0;
    }
  }
      break;
      
    case 0x40: //Test Step 1 
       AccTestFlag = 1;    
       UreaAlarm.Quality = 0;
       UreaAlarm.Status = 0;
       UreaAlarm.SysErr = 0;
       UreaTestFlag = 1;
       UreaAlarm.Level = 2;

    if(UreaTestFlag == 1) {
      if(UreaAccControl.Level != 1) {
        DO1_SET();
        DO2_RESET();
        SecData.UreaAccFlag = 1;
        UreaAlarmLapTime = 0;
        UreaAccControl.Level = 1;
        UreaBuzzerFlag = 0;
      }
    }       
      //SecData.UreaAccFlag = 1;
      break;      

    case 0x50: //Test Step 2    
      AccTestFlag = 1;
      UreaAlarm.Quality = 0;
      UreaAlarm.Status = 0;
      UreaAlarm.SysErr = 0;
      UreaTestFlag = 2;
      UreaAlarm.Level = 2;

      if(UreaAccControl.Level != 2) {
        DO2_SET();
        DO1_RESET();
        SecData.UreaAccFlag = 2;
#ifdef TIME_HOUR
        UreaAlarmLapTime = Setting.UreaAlHr1*3600;
#else
        UreaAlarmLapTime = Setting.UreaAlHr1*60;
#endif
        UreaAccControl.Level = 2;
        UreaBuzzerFlag = 0;
      }
       
      //SecData.UreaAccFlag = 2;
      break;
    case 0x70: // Parameter Read Command
      Wait_flag = 1;
      TestJIgTxData(1, 0x3B,(unsigned char*) &Setting,sizeof(Setting));
      //TestJIgTxData(1, 0x1A,(unsigned char*) &SecData,64);
      Wait_flag = 0;
      break;

    case 0xFF: // Parameter Write Command
      Setting.UreaLvlHi = TJ_ComBuf[1];
      Setting.UreaLvlLo = TJ_ComBuf[2];
      Setting.UreaLvlAL = TJ_ComBuf[3];
      Setting.UreaConHi = TJ_ComBuf[4];
      Setting.UreaConLo = TJ_ComBuf[5];
      Setting.UreaAlHr1 = TJ_ComBuf[6];
      Setting.UreaAlHr2 = TJ_ComBuf[7];
      Setting.UreaBuzInt = TJ_ComBuf[8];
     F_Parameter = 1;
      break;      
  }
}
/*
********************************************************************************
* Description : SendSecData    
* Arguments   : None
* Return      : 
* Note        : Send SecData to UI  
******************************************************************************** 
*/
void Send_SecData(void)  //100ms마다 
{    
  if(!Wait_flag) 
  TestJIgTxData(1, 0x1A,(unsigned char*) &SecData,64);
  //TestJIgTxData(1, 0x3B,(unsigned char*) &Setting,sizeof(Setting));
   
//  TJ_RxLength = 0;
//  TJ_RxCommand = 0;
//  TJ_RxDataCnt = 0;
//  TJ_Decode_Flag = 0; 
}
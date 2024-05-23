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

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void TestJIgTxData(u8 type, u8 command, u8 *data, u16 len)
{
//    u8 sdata[4096+7], check_sum;
    u8 sdata[10], tdata, check_sum;
    u16 cnt = 0;
    u8 ls[2];

    sdata[cnt++] = 0xc6;
    sdata[cnt++] = 0xc6;
    
    if(type == 1) {
      sdata[cnt++] = 0x7e;
      sdata[cnt++] = len;
      sdata[cnt++] = command;
      check_sum = len^command;
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
      UART_SendData(&huart6, sdata[i]);
    for(int i = 0; i < len; i++) {
        tdata = *data++;
        check_sum ^= tdata;
        UART_SendData(&huart6, tdata);
//        sdata[cnt] = *data++;
//        check_sum ^= sdata[cnt];
//        cnt++;
    }    
    check_sum++;
    UART_SendData(&huart6, check_sum);
    UART_SendData(&huart6, 0x7d);
//    sdata[cnt++] = check_sum;
//    sdata[cnt++] = 0x7d;
    
//    for(int i = 0; i < cnt; i++)  UART_PutChar3(sdata[i]);
}

void TestJigRxData(unsigned char rdata)
{
    if(TJ_Decode_Flag) return;
    
    switch(TJ_RxStep) {
        case 0:
          if(rdata == 0xc6) TJ_SyncCnt++;
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
//    if(!TJ_Decode_Flag) UART_PutChar3(rdata);
      
}

void TestJigDecodeData(void)
{     
  u32 addr;
  u32 i;  
  u16 rcnt = 0;
  u32 size;
  u8 ep, txcnt;
  u8 dtgtype;
  u32 tmp1, tmp2;
 
  if(!TJ_Decode_Flag) return;
  
  switch(TJ_RxCommand) {
    case 0x10:         
      TestJIgTxData(TJ_LenType, 0x10, TJ_ComBuf,1);
      break;
    case 0x20:    
      TestJIgTxData(TJ_LenType, 0x20, TJ_ComBuf,1);
      memset(TJ_ComBuf,0xFF,256);
      lcd_clear();
      lcd_printf(4,0,0,"Initial Parameter");
      lcd_printf(4,1,0,"-FRAM");    
      lcd_printf(60,1,0,"-Event");    
      lcd_printf(4,2,0,"-Factor");    

      lcd_draw_rect(4,80,3);      
      for(i = 0; i < 256; i++)  {//eeprom 모두 초기화
        FRAMMultiWrite(TJ_ComBuf, i*32, 32);
        lcd_fill_box(4, (i*84/256), 3, 4);    
        lcd_printf(84,3,0, "%d%s",(i*100/256),"%");
        ep = i/2;
        TestJIgTxData(TJ_LenType, 0x20, (u8 *)&ep,1);
        IWDG_ReloadCounter(); 
      }          
      for(i = 0; i < 10; i++) //event header remove
        Nand_Write_Buffer(TJ_ComBuf, NAND_SIZE_DATA + i*NAND_BLOCK_SIZE, sizeof(st_Setting));
      
      TestJIgTxData(TJ_LenType, 0x20, TJ_ComBuf,1);
      NVIC_SystemReset();  
      break;
      
    case 0x30:
      UpdateTimeToGPSTime();
      TestJIgTxData(TJ_LenType, 0x30, (u8 *)&Setting, sizeof(st_Setting)-32);
      break;
      
    case 0x31:
      TestJIgTxData(TJ_LenType, 0x31, (u8 *)&Setting, sizeof(st_Setting)-32);
      break;
    case 0x32:
      rcnt=0;
      tmp1 = 0xCCCCCCCC;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],&RTC_Val,6); rcnt+=8;
      TJ_ComBuf[rcnt] = 0; rcnt++;
      tmp1 = (u32)(DailyData.TTrip*(double)Setting.K_Factor);
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,4); rcnt+=4;
      tmp1 = (u32)(DailyData.Trip*(double)Setting.K_Factor);
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,4); rcnt+=4;
      tmp1 = (u32)(DailyData.DTrip*(double)Setting.K_Factor);
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.LapTime,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.DLapTime,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.Drv_Time,4); rcnt+=4;
      memset(&TJ_ComBuf[rcnt], 0, 32); rcnt+=32;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.Fuel,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.DFuel,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.TFuel,8); rcnt+=8;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.Evt_Count,1); rcnt++;
      memset(&TJ_ComBuf[rcnt], 0, 42); rcnt+=42;
      tmp1 = getcrc16b(TJ_ComBuf,rcnt);
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,2); rcnt+=2;

      TestJIgTxData(TJ_LenType, 0x32, TJ_ComBuf, rcnt);
      break;
    case 0x33:
      rcnt=0;
      tmp1 = (u32)(DailyData.DTrip*(double)Setting.K_Factor);
      tmp2 = (u32)(DailyData.TTrip*(double)Setting.K_Factor);
      
      memcpy(&TJ_ComBuf[rcnt],(u8*)&RTC_Val,6); rcnt+=8;
      
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DataSaveAddr,4); rcnt+=4;      
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,4); rcnt+=4;      
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp2,4); rcnt+=4;			
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.DFuel,4); rcnt+=4;			
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.TFuel,8); rcnt+=8;
			
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DataSaveAddr,4); rcnt+=4;      
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp1,4); rcnt+=4;      
      memcpy(&TJ_ComBuf[rcnt],(u8*)&tmp2,4); rcnt+=4;			
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.DFuel,4); rcnt+=4;			
      memcpy(&TJ_ComBuf[rcnt],(u8*)&DailyData.TFuel,8); rcnt+=8;
      
      TestJIgTxData(TJ_LenType, 0x33, TJ_ComBuf,rcnt);
//      TestJIgTxData(0x33, (u8 *)&DataSaveAddr,4);
      break;
      
    case 0x35: {
        st_Setting *hd = (st_Setting*)TJ_ComBuf;
        
        //ch1        
        addr = DRV_SETTING_PARM + offsetof(st_Setting,Drv_Name);
        size = offsetof(st_Setting,FW_Type) - offsetof(st_Setting,Drv_Name);
        FRAMMultiWrite(hd->Drv_Name, addr, size);
        
        //ch3
        addr = DRV_SETTING_PARM + offsetof(st_Setting,Set_Direction);
        size = offsetof(st_Setting,reserved11) - offsetof(st_Setting,Set_Direction);
        FRAMMultiWrite((u8*)&hd->Set_Direction, addr, size);
        
        //ch5
        addr = DRV_SETTING_PARM + offsetof(st_Setting,ODO_Offeset);
        size = sizeof(hd->ODO_Offeset);
        FRAMMultiWrite((u8*)&hd->ODO_Offeset, addr, size);              
        if(hd->ODO_Offeset != 0xffffffff) {
          DailyData.TTrip = (double)hd->ODO_Offeset;
          FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
          Val.TTrip = DailyData.TTrip;
          Val.TTripOld = Val.TTrip;
        }        
        
        addr = DRV_SETTING_PARM + offsetof(st_Setting,Fuel_Factor);
        size = offsetof(st_Setting,TripCount) - offsetof(st_Setting,Fuel_Factor);
        FRAMMultiWrite((u8*)&hd->Fuel_Factor, addr, size);      
      }
      break;
     
      
    case 0x40:
      if(Setting.Serial_No[0] != 0xFF) {//초기화상태가 아니라면
        memcpy(TJ_ComBuf, Setting.Serial_No, sizeof( Setting.Serial_No));   //버퍼에 기존값   
        dtgtype = Setting.DTG_Type;
      }
      
      memcpy((u8 *)&Setting, TJ_ComBuf, sizeof(st_Setting)-32); //받은헤더 카피
      
      if(TJ_ComBuf[0] > 0) {
        memcpy(Setting.Serial_No, TJ_ComBuf, sizeof(Setting.Serial_No));   //기존시리얼 재입력
        Setting.DTG_Type = dtgtype;
      }
      
      memcpy(TJ_ComBuf, (u8 *)&Setting, sizeof(st_Setting));      //버퍼에 모두 카피
        
      
      for(i = 0; i < (sizeof(st_Setting)-32)/8 ; i++)
        FRAMMultiWrite(&TJ_ComBuf[i*32], DRV_SETTING_PARM + i*32, 32);
      Drv_Regist_Cnt++;
      FRAMByteWrite(Drv_Regist_Cnt,DRV_REGIST_CNT);
      Selected_Driver = 0;
      FRAMByteWrite(Selected_Driver, SELECTED_DRIVER); 

      memcpy((u8 *)&Driver_Info[0].Name, (u8 *)&Setting.Drv_Name, 10);
      memcpy((u8 *)&Driver_Info[0].Code, (u8 *)&Setting.Drv_Code,10);
      memcpy((u8 *)&Driver_Info[0].Num, (u8 *)&Setting.Drv_Code[11], 7);
             
      FRAMMultiWrite((u8 *)&Driver_Info[0], DRIVER_INFO, sizeof(st_DRIVER_INFO));
            
      TestJIgTxData(TJ_LenType, 0x40, (u8 *)&Setting, sizeof(st_Setting)-32);
      NVIC_SystemReset();        
      break;

    case 0x41:      
      memcpy((u8 *)&Setting, TJ_ComBuf, sizeof(st_Setting)-32); //받은헤더 카피
      FRAMMultiWrite((u8 *)&Setting, DRV_SETTING_PARM, sizeof(st_Setting)-32);    
      TestJIgTxData(TJ_LenType, 0x41, (u8 *)&Setting, sizeof(st_Setting)-32);
      break;     
      
    case 0x50:
      rcnt=0;
      
      memcpy(&TJ_ComBuf[rcnt],&RTC_Val,6); rcnt+=6;
      TJ_ComBuf[rcnt] = 0; rcnt++;
      TJ_ComBuf[rcnt] = SecData.Speed[0]; rcnt++;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.RPM,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Lat,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Lon,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Azi,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.AccX,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.AccY,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Signal,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Status,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Fuel,4); rcnt+=4;

      TestJIgTxData(TJ_LenType, 0x50, TJ_ComBuf, 32);
      break;
      
    case 0x60:
      Setting.Can_Enable = 1; //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Can_Enable - (u8 *)&Setting; //1.4.0.0
      FRAMByteWrite(Setting.Can_Enable, addr); //1.4.0.0
    //  Can_Type = 0;
    //  FRAMByteWrite(Can_Type, CAN_TYPE);  
      Setting.Can_Type = 4; //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Can_Type - (u8 *)&Setting; //1.4.0.0
      FRAMByteWrite(Setting.Can_Type, addr); //1.4.0.0
    
      Setting.CanProtocol = CAN_ID_EXT;
      Setting.CanMode = CAN_Mode_Silent;    
      Setting.CanBaudrate = 250;
                        
      Setting.SpeedType = SIG_CAN;
      Setting.RPM_Type = SIG_CAN;
      Setting.Brake_Type = SIG_CAN;
      Setting.ODO_Type = SIG_CAN;
      Setting.FuelType = SIG_CAN;
      
      TestJigCAN2_OK = 0;
      TestJigCAN2 = 1;
      CAN_Configuration();
      TestJIgTxData(TJ_LenType, 0x60, TJ_ComBuf, 1);

      break;
      
    case 0x70: //can
      rcnt=0;
      
      memcpy(&TJ_ComBuf[rcnt],&RTC_Val,6); rcnt+=6;
      TJ_ComBuf[rcnt] = 0; rcnt++;
      TJ_ComBuf[rcnt] = SecData.Speed[0]; rcnt++;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.RPM,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Lat,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Lon,4); rcnt+=4;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Azi,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.AccX,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.AccY,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Signal,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Status,2); rcnt+=2;
      memcpy(&TJ_ComBuf[rcnt],(u8*)&SecData.Fuel,4); rcnt+=4;
      TJ_ComBuf[rcnt] = TestJigCAN2_OK;
      
      TestJIgTxData(TJ_LenType, 0x70, TJ_ComBuf, 33);
      break;
      
    case 0x80:  //usb write
#ifdef EVENT_DATA_SAVE_ENABLE
      if(USB_Detect_Flag && !Evt.Save_Flag) {
#else        
      if(USB_Detect_Flag) {
#endif        
        USB_JigTest_Flag = 1;    
      }
      break;
 
    case 0x90:  //nand usb compare
#ifdef EVENT_DATA_SAVE_ENABLE
      if(!USB_DataDL_Flag && USB_Detect_Flag && !Evt.Save_Flag) {
#else        
      if(!USB_DataDL_Flag && USB_Detect_Flag) {
#endif        
        USB_JigCompare_Flag = 1;
      }      
      break;
      
    case 0xA0:  // DI / DO self test
      DO1_RESET();
      vTaskDelay(10); 
      if(DI1_SIG())
        TJ_ComBuf[0] = 0;
      else    
        TJ_ComBuf[0] = 1; 

      DO2_RESET();
      vTaskDelay(10); 
      if(DI2_SIG())
        TJ_ComBuf[1] = 0;
      else 
        TJ_ComBuf[1] = 1;

      TestJIgTxData(TJ_LenType, 0xA0, TJ_ComBuf, 2);
      break;
      
    case 0xB0:  //BLE module check
      TJ_ComBuf[0] = BLE_InitFLag;
      TestJIgTxData(TJ_LenType, 0xB0, TJ_ComBuf, 1);        
      break;

    case 0xC0:  //Uart Test
      txcnt = 0;      
      while(TestJigComm[0] == 0 && txcnt < 5) {
        UART_SendDataStr(&huart2,"test",4);
        vTaskDelay(10);       
        txcnt++;
      }
      TJ_ComBuf[0] = TestJigComm[0];
      
      txcnt = 0;
      while(TestJigComm[1] == 0 && txcnt < 5) {      
        UART_SendDataStr(&huart3,"test",4);      
        vTaskDelay(10);       
        txcnt++;
      }
      TJ_ComBuf[1] = TestJigComm[1];
      
      txcnt = 0;
      while(TestJigComm[2] == 0 && txcnt < 5) {      
        UART_SendDataStr(&huart5,"test",4);      
        vTaskDelay(10);
        txcnt++;        
      }
      TJ_ComBuf[2] = TestJigComm[2];

      TestJIgTxData(TJ_LenType, 0xC0, TJ_ComBuf, 3);        
      break;
    
    case 0xD0:  //DTG Initialize
      TestJIgTxData(TJ_LenType, 0xD0, TJ_ComBuf, 1);
      Parameter_Reset();   
      TJ_ComBuf[0] = 0xff;
      TestJIgTxData(TJ_LenType, 0xD0, TJ_ComBuf, 1);
      
      Setting.FactoryNumber = 1;
      addr = DRV_SETTING_PARM + offsetof(st_Setting,FactoryNumber);
      FRAMMultiWrite((u8*)&Setting.FactoryNumber, addr, 1);  
      NVIC_SystemReset(); 
      break;
  }
  
  TJ_RxLength = 0;
  TJ_RxCommand = 0;
  TJ_RxDataCnt = 0;
  TJ_Decode_Flag = 0;
}
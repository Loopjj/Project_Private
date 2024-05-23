/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : dtg.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : dtg process 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define DTG_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
#define SPEED_SIG_TIMING  5  //5
#define RPM_SIG_TIMING    2  //2

#define TIMESHIFT_SEC 9*3600   //한국시간 계산을 위한 9시간 SHIFT 초단위
#define LOW_LIMT_OF_TIME  0x58000000  //2017년 이후값.

#define GX_EVENT_VAL 1126  //G sensor 이벤트 설정값
#define GY_EVENT_VAL  511  

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 PrevGSpeed;
unsigned long PrevLat, PrevLon;
time_t PrevSecTime; 
unsigned int GPSNotValidCount;

u8  GPS_Time_Sync;
u8  GPS_Time_NoSyncCnt;
u32 RTC_GPS_Val;

const float Car_Factor[] = {
  1.0,   //default
  1.15,  //emight 2.5ton
  1.07,  //megatruck 4.5ton
  1.00,   //trago 
  1.07,   //prima
  1.00,   //EXT1 
  1.00,   //EXT2 
  1.00   //EXT3 
};

u32 Evt_SubSecond;

st_Setting TmpSetting;

u8 SecDataTimeBuffer[7];

u8 IsgSkipFlag;
u16 IsgSkipCount;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : System_PowerOff
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void System_PowerOff(void) 
{
  POWER_ENABLE_OFF();
  
  for(;;) {
    __NOP();
  }
}

/*******************************************************************************
* Function Name  : Parameter_Reset
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Parameter_Reset(void)
{
//  u8 tmp[256] = {0,};
  u32 i, len;
  u16 addr;
  char str[20];
  
  memset(Nand_Save_Buff, 0, NAND_PAGE_SIZE);
  memset(RTC_BK_Addr, 0, sizeof(stRTC_Backup));       
    
  FRAMMultiRead((u8 *)&Setting, DRV_SETTING_PARM, sizeof(st_Setting));  
  
  lcd_clear();
  lcd_printf(4,0,0,"Initial Parameter");
  lcd_printf(4,1,0,"-FRAM");    
  lcd_printf(60,1,0,"-Event");    
  lcd_printf(4,2,0,"-Factor");    
  
  lcd_draw_rect(4,80,3);      
  for(i = 0; i < 256; i++)  {            //Fram 모두 초기화  8Kbyte
    FRAMMultiWrite(Nand_Save_Buff, i*32, 32);
    lcd_fill_box(4, (i*84/256), 3, 4);    
    lcd_printf(84,3,0, "%d%s",(i*100/256),"%");
    IWDG_ReloadCounter(); 
    delay_ms(100);
  }      

  for(i = 0; i < 10; i++) //event header remove
    Nand_Write_Buffer(Nand_Save_Buff, NAND_SIZE_DATA + i*NAND_BLOCK_SIZE, sizeof(st_Setting));
  
  memset((u8 *)&DailyData, 0, sizeof(stDailyData));
  FRAMMultiWrite((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));

  Setting.FW_Type = CDMA_TYPE;
  addr = DRV_SETTING_PARM + offsetof(st_Setting, FW_Type);
  FRAMByteWrite(Setting.FW_Type, addr);

  addr = DRV_SETTING_PARM + &Setting.FW_Ver[4] - (u8 *)&Setting; 
  FRAMByteWrite(Setting.FW_Ver[4], addr);  
  FRAMByteWrite(Setting.FW_Ver[5], addr+1); 
  
  memcpy(Setting.Form_Approval_No, (u8*)Approval_No, 10);
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Form_Approval_No - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.Form_Approval_No, addr, sizeof(Setting.Form_Approval_No));
  
  memset(Setting.Model_No, ' ', sizeof(Setting.Model_No));
  len = StringLen((char*)Model_Name, 15);
  memcpy(Setting.Model_No, (u8*)Model_Name, len);
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Model_No - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.Model_No, addr, sizeof(Setting.Model_No));
  IWDG_ReloadCounter(); 
  
  if(Setting.Serial_No[0] == 0 ){ ///Jig Test하고 관련있음..
    memset(str,0,20);
    sprintf(str, "1LDT3B18000001");
    memcpy(Setting.Serial_No, (u8*)str,sizeof(Setting.Serial_No));
  }
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Serial_No - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.Serial_No, addr, sizeof(Setting.Serial_No));
  
  Setting.DTG_Type = DTG_GREEN;  
  addr = DRV_SETTING_PARM + (u8 *)&Setting.DTG_Type - (u8 *)&Setting;
  FRAMByteWrite(Setting.DTG_Type, addr);
    
  GS_Offset[0] = 0; GS_Offset[1] = 0; GS_Offset[2] = GSENSOR_1G_VAL;  
  FRAMMultiWrite((u8 *)&GS_Offset, OFFSET_GX, sizeof(GS_Offset));    
  
  Setting.K_Factor   = 5096;
  addr = DRV_SETTING_PARM + (u8 *)&Setting.K_Factor - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.K_Factor, addr, 2);

  Setting.RPM_Factor = 300;
  addr = DRV_SETTING_PARM + (u8 *)&Setting.RPM_Factor - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.RPM_Factor, addr, 2);
  
  Contrast = CONTRAST_DEFAULT;
  FRAMByteWrite(Contrast, SET_CONTRAST);
  Main_Disp_Mode = 0;
  FRAMByteWrite(Main_Disp_Mode, MAIN_DISPLAY);  
  
  Setting.Can_Enable = SIG_LINE; 
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Can_Enable - (u8 *)&Setting; 
  FRAMByteWrite(Setting.Can_Enable, addr); 

  if(Setting.Can_Type == 0 || Setting.Can_Type > 5) Setting.Can_Type = 4; 
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Can_Type - (u8 *)&Setting; 
  FRAMByteWrite(Setting.Can_Type, addr); 
  
  if(Setting.CanProtocol != CAN_ID_STD) Setting.CanProtocol = CAN_ID_EXT; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,CanProtocol); 
  FRAMByteWrite(Setting.CanProtocol, addr); 
  
  if(Setting.CanBaudrate != 500) Setting.CanBaudrate = 250; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,CanBaudrate); 
  FRAMMultiWrite((u8*)&Setting.CanBaudrate, addr,2); 

  if(Setting.CanMode != CAN_Mode_Normal) Setting.CanMode = CAN_Mode_Silent; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,CanMode); 
  FRAMByteWrite(Setting.CanMode, addr); 
  
  Setting.SpeedType = SIG_CAN; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,SpeedType); //3.4
  FRAMMultiWrite((u8*)&Setting.SpeedType, addr,sizeof(Setting.SpeedType)); 
  
  Setting.RPM_Type = SIG_CAN; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,RPM_Type); //3.4
  FRAMMultiWrite((u8*)&Setting.RPM_Type, addr,sizeof(Setting.RPM_Type)); 
  
  Setting.Brake_Type = SIG_CAN; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,Brake_Type); //3.4
  FRAMMultiWrite((u8*)&Setting.Brake_Type, addr,sizeof(Setting.Brake_Type)); 
  
  Setting.ODO_Type = SIG_CAN; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,ODO_Type); //3.4
  FRAMMultiWrite((u8*)&Setting.ODO_Type, addr,sizeof(Setting.ODO_Type)); 
  
  Setting.FuelType = SIG_CAN; 
  addr = DRV_SETTING_PARM + offsetof(st_Setting,FuelType); //3.4
  FRAMMultiWrite((u8*)&Setting.FuelType, addr,sizeof(Setting.FuelType)); 
  
  Setting.Car_Idx = 0; //1.4.0.0
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_Idx - (u8 *)&Setting; //1.4.0.0
  FRAMByteWrite(Setting.Car_Idx, addr); //1.4.0.0
  
  Setting.DAC_Speed = 4;
  Setting.DAC_RPM   = 4;
  addr = DRV_SETTING_PARM + (u8 *)&Setting.DAC_Speed - (u8 *)&Setting; //1.4.0.0
  FRAMByteWrite(Setting.DAC_Speed, addr); //1.4.0.0
  addr = DRV_SETTING_PARM + (u8 *)&Setting.DAC_RPM - (u8 *)&Setting; //1.4.0.0
  FRAMByteWrite(Setting.DAC_RPM, addr); //1.4.0.0  
  DAC_Write(0, Setting.DAC_Speed);  
  DAC_Write(1, Setting.DAC_RPM);  

  Setting.ExtModemMode = 0;
  Setting.ExtModemTelecom = 0;
  Setting.ExtModemModel = 0;
  
  addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode);
  FRAMMultiWrite((u8*)&Setting.ExtModemMode, addr,sizeof(Setting.ExtModemMode)+sizeof(Setting.ExtModemTelecom)+sizeof(Setting.ExtModemModel)); 
 
  if(Setting.FW_Type == MDT_LG)
    Setting.FileOutput = 1;
  else
    Setting.FileOutput = 0;
  
  addr = DRV_SETTING_PARM + offsetof(st_Setting,FileOutput);
  FRAMMultiWrite((u8*)&Setting.FileOutput, addr,sizeof(Setting.FileOutput)); 
 
  
  Calib_F_Factor = 100;
  FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);    
  
  Setting.Dist_Percent = 1000;
  addr = DRV_SETTING_PARM + offsetof(st_Setting,Dist_Percent); 
  FRAMMultiWrite((u8 *)&Setting.Dist_Percent, addr, sizeof(Setting.Dist_Percent)); 
  
  Idle_RPM = 650;
  FRAMMultiWrite((u8 *)&Idle_RPM, IDLE_RPM, 2);

  Backligth_Sel = 3;
  FRAMByteWrite(Backligth_Sel, BACKLIGHT_SEL); 

  Modem_Parameter_Reset();
  
  memset((u8*)&IsgSetting, 0, sizeof(stIsgSetting));
  IsgSetting.IdleTimeLimit = 180;
  IsgSetting.BatVoltLimit = 230;
  IsgSetting.CoolTempLimit = 50;
  IsgSetting.InclineLimit = 4;
  IsgSetting.PBrakeMode = 0;
  IsgSetting.AirPressMode = 1;
  IsgSetting.NGearMode = 3;
  IsgSetting.GpsMode = 1;
  IsgSetting.GeoFenceCnt = 0;
  for(i = 0; i < 10; i++)
    IsgSetting.GeoFence[i].Radius = 200;
  FRAMMultiWrite((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
  
  ADASPckNo.AdasSerialNo = 0;
  ADASPckNo.AdasTripNo = 0; 
  FRAMMultiWrite((u8 *)&ADASPckNo, ADAS_HEADER, sizeof(tsADASPckNo));    
}

/*
********************************************************************************
* Description : Parameter_Init
* Arguments   : None
* Return      : 
* Note        : Parameter init
******************************************************************************** 
*/  
void Parameter_Init(void)
{  
  u8 ver[6];

  FRAMMultiRead((u8 *)&ver, FIRMWARE_VER, 6);
  
  u32 i, fdate;
  u16 addr, size;  
  u8 cnt = 0; //1.4.0.0

  Nand_Save_Buff =  (u8*)BKPSRAM_BASE;
  RTC_BK_Addr = (stRTC_Backup*)(RTC_BASE + 0x50);    

  Power_Off_Flag = 0;
  
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 10)
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0);
  
//- Start Initial FRAM -//
  FRAMMultiRead((u8 *)&fdate, FIRMWARE_DATE, 4);

  FRAMMultiRead((u8 *)&ver, FIRMWARE_VER, 6);
  if(!(ver[0] == MAJOR && ver[1] == MINOR && ver[2] == RELEASE && ver[3] == BUILD && ver[4] == FACTORY1 && ver[5] == FACTORY2)) {
    if(!(ver[4] == FACTORY1 && ver[5] == FACTORY2)) {      
      Parameter_Reset(); 
    }

    addr = DRV_SETTING_PARM + offsetof(st_Setting,UpdateCount);  //offsetof(stFRAM,Header) + offsetof(st_Setting,UpdateCount);
    FRAMMultiRead((u8*)&Setting.UpdateCount, addr, 4);  
    Setting.UpdateCount++;
    FRAMMultiWrite((u8*)&Setting.UpdateCount, addr, 4);  
    
    ver[0] = MAJOR; ver[1] = MINOR; ver[2] = RELEASE; ver[3] = BUILD;
    ver[4] = FACTORY1; ver[5] = FACTORY2; 
    FRAMMultiWrite((u8 *)&ver, FIRMWARE_VER, 6);  
    FRAMMultiWrite((u8 *)&fdate, FIRMWARE_DATE, 4); //1.4.0.0    
    
    addr = DRV_SETTING_PARM + (u8 *)&Setting.FW_Ver - (u8 *)&Setting; 
    size = sizeof(Setting.FW_Ver)-2; //1.4.0.0
    FRAMMultiWrite(ver, addr, size);    
    addr = DRV_SETTING_PARM + (u8 *)&Setting.FW_Date - (u8 *)&Setting;
    size = sizeof(Setting.FW_Date);
    FRAMMultiWrite((u8 *)&fdate, addr, size);     
    IWDG_ReloadCounter();  
  }

  FRAMByteRead(&Contrast, SET_CONTRAST); 
  if(Contrast > CONTRAST_MAX || Contrast < CONTRAST_MIN) Contrast = CONTRAST_DEFAULT;//0x2D;

  FRAMByteRead(&Main_Disp_Mode, MAIN_DISPLAY);
  FRAMByteRead(&Backligth_Sel, BACKLIGHT_SEL); //1.4.0.0

  u32 saddr = 0;
  FRAMMultiRead((u8 *)&saddr, DRV_SAVE_ADDR, 4);   
  if(saddr%sizeof(tsSecData) > 0 || saddr > NAND_SIZE_DATA) 
    saddr = RTC_BK_Addr->Save_Addr;
  DataSaveAddr = saddr;

  FRAMMultiRead((u8 *)&saddr, DRV_SENT_ADDR, 4);   
  if(saddr%sizeof(tsSecData) > 0 || saddr > NAND_SIZE_DATA) 
    saddr = DataSaveAddr; //RTC_BK_Addr->Sent_Addr;
  DataSentAddr = saddr;
  
  FRAMMultiRead((u8 *)&saddr, VDIS_SENT_ADDR, 4);   
  if(saddr%sizeof(tsSecData) > 0 || saddr > NAND_SIZE_DATA) 
    saddr = DataSaveAddr;
  VdisSentAddr = saddr;

  BK_Sram_Addr = DataSaveAddr%NAND_PAGE_SIZE;  
  
#ifdef EVENT_DATA_SAVE_ENABLE
  FRAMMultiRead((u8 *)&Evt.Idx, EVT_IDX , 1);
#endif  
  
  FRAMMultiRead((u8 *)&GS_Offset, OFFSET_GX, sizeof(GS_Offset));   //2.1.3  
  
  FRAMMultiRead((u8 *)&Setting, DRV_SETTING_PARM, sizeof(st_Setting));  
  Setting.Identity = 0xDDDDDDDD;

//  if(Setting.Can_Enable && Setting.CanProtocol == CAN_ID_EXT 
//  && Setting.CanBaudrate == 250 && Setting.ODO_Type && Setting.K_Factor != 8) {
//    Setting.K_Factor = 8;
//    addr = DRV_SETTING_PARM + offsetof(st_Setting,K_Factor); 
//    FRAMMultiWrite((u8 *)&Setting.K_Factor, addr, sizeof(Setting.K_Factor)); 
//  }  
//  if(Setting.Can_Enable && Setting.SpeedType && !Setting.ODO_Type && Setting.K_Factor != 36000 && Setting.Can_Type != 5) {
//    Setting.K_Factor = 36000;
//    addr = DRV_SETTING_PARM + offsetof(st_Setting,K_Factor); 
//    FRAMMultiWrite((u8 *)&Setting.K_Factor, addr, sizeof(Setting.K_Factor)); 
//  }
//  if(Setting.Can_Enable && Setting.Car_Idx == 4 && Setting.K_Factor != 36000) {
//    Setting.K_Factor = 36000;
//    addr = DRV_SETTING_PARM + offsetof(st_Setting,K_Factor); 
//    FRAMMultiWrite((u8 *)&Setting.K_Factor, addr, sizeof(Setting.K_Factor)); 
//  }
  
  if(!IsSameString((char*)Setting.Form_Approval_No, (char*)Approval_No, 10)) {
    memcpy(Setting.Form_Approval_No, (u8*)Approval_No, 10);
    addr = DRV_SETTING_PARM + (u8 *)&Setting.Form_Approval_No - (u8 *)&Setting;
    FRAMMultiWrite((u8 *)&Setting.Form_Approval_No, addr, sizeof(Setting.Form_Approval_No));
  }
  
  size = StringLen((char*)Model_Name, 15);
  if(!IsSameString((char*)Setting.Model_No, (char*)Model_Name, size)) {
    memset(Setting.Model_No, ' ', sizeof(Setting.Model_No));
    memcpy(Setting.Model_No, (u8*)Model_Name, size);
    addr = DRV_SETTING_PARM + (u8 *)&Setting.Model_No - (u8 *)&Setting;
    FRAMMultiWrite((u8 *)&Setting.Model_No, addr, sizeof(Setting.Model_No));
  }
  
  if(Setting.FW_Type != CDMA_TYPE) {//2.2.5.1
    Setting.FW_Type = CDMA_TYPE;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, FW_Type);
    FRAMByteWrite(Setting.FW_Type, addr);
  }
  
#ifdef DTG_TYPE_GREEN
  if(Setting.DTG_Type == DTG_NORMAL) {
    Setting.DTG_Type = DTG_GREEN;  
    addr = DRV_SETTING_PARM + offsetof(st_Setting, FW_Type);
    FRAMByteWrite(Setting.FW_Type, addr);    
  }
#else
  if(Setting.DTG_Type == DTG_GREEN) {
    Setting.DTG_Type = DTG_NORMAL;  
    addr = DRV_SETTING_PARM + offsetof(st_Setting, FW_Type);
    FRAMByteWrite(Setting.FW_Type, addr);    
  }    
#endif

    
//  if(Setting.DAC_Speed > 9) {//1.4.0.0
//    Setting.DAC_Speed = 4;  //1.4.0.0
//    addr = DRV_SETTING_PARM + (u8 *)&Setting.DAC_Speed - (u8 *)&Setting;  //1.4.0.0
//    FRAMByteWrite(Setting.DAC_Speed, addr);  //1.4.0.0
//  }
//  DAC_Write(0, Setting.DAC_Speed);  //1.4.0.0
//  if(Setting.DAC_RPM > 9) {//1.4.0.0
//    Setting.DAC_RPM = 4;  //1.4.0.0
//    addr = DRV_SETTING_PARM + (u8 *)&Setting.DAC_RPM - (u8 *)&Setting;  //1.4.0.0
//    FRAMByteWrite(Setting.DAC_RPM, addr);  //1.4.0.0      
//  }
//  DAC_Write(1, Setting.DAC_RPM);    //1.4.0.0
  
  if(Setting.CanBaudrate != 250 && Setting.CanBaudrate != 500) {
    Setting.CanBaudrate = 250;
    addr = DRV_SETTING_PARM + offsetof(st_Setting,CanBaudrate); 
    FRAMMultiWrite((u8 *)&Setting.CanBaudrate, addr, sizeof(Setting.CanBaudrate)); 
  }
  if(Setting.CanProtocol != CAN_ID_STD && Setting.CanProtocol != CAN_ID_EXT) {
    Setting.CanProtocol = CAN_ID_EXT;
    addr = DRV_SETTING_PARM + offsetof(st_Setting,CanProtocol); 
    FRAMMultiWrite((u8 *)&Setting.CanProtocol, addr, sizeof(Setting.CanProtocol)); 
  }  
  if(Setting.CanMode != CAN_Mode_Normal && Setting.CanMode != CAN_Mode_Silent) {
    Setting.CanMode = CAN_Mode_Silent;
    addr = DRV_SETTING_PARM + offsetof(st_Setting,CanMode); 
    FRAMMultiWrite((u8 *)&Setting.CanMode, addr, sizeof(Setting.CanMode)); 
  }  
  
  FRAMMultiRead((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);   
  if(Calib_F_Factor == 0 || Calib_F_Factor > 999) {
    Calib_F_Factor = 100;
    FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);      
  }
  
  if(Setting.Dist_Percent == 0 || Setting.Dist_Percent > 9999) {    
    Setting.Dist_Percent = 1000;
    addr = DRV_SETTING_PARM + offsetof(st_Setting,Dist_Percent); 
    FRAMMultiWrite((u8 *)&Setting.Dist_Percent, addr, sizeof(Setting.Dist_Percent)); 
  }

  for(i = 0; i < sizeof(Setting.Car_No); i++) { 
    if(Setting.Car_No[i] < 0x30 || Setting.Car_No[i] > 0x5A) 
      Setting.Car_No[i] = 0x20;    
  }  

  for(i = 0; i < sizeof(Setting.Company_Reg_No); i++) { 
    if(Setting.Company_Reg_No[i] < 0x30 || Setting.Company_Reg_No[i] > 0x5A) 
      Setting.Company_Reg_No[i] = 0x20;    
  }
 
  FRAMMultiRead((u8 *)&Drv_Regist_Cnt, DRV_REGIST_CNT, 1);
  if(!Drv_Regist_Cnt) NotRegist_Flag = 1;
  
  for(i = 1; i < sizeof(Setting.Car_Reg_No)-3; i++) //1.4.0.0
    if(Setting.Car_Reg_No[i] == 0x00)  cnt++; //1.4.0.0
  if(cnt == sizeof(Setting.Car_Reg_No)-4) NotRegist_Flag = 2; //1.4.0.0
    
  FRAMMultiRead((u8 *)&Selected_Driver, SELECTED_DRIVER, 1);  
  for(i = 0; i < Drv_Regist_Cnt; i++) {
    addr = DRIVER_INFO+i*sizeof(st_DRIVER_INFO);
    FRAMMultiRead((u8 *)&Driver_Info[i], addr, sizeof(st_DRIVER_INFO));
    if(i == Selected_Driver) {
      memcpy(Setting.Drv_Code, Driver_Info[i].Code , 10);
      Setting.Drv_Code[10] = i+0x31;
      memcpy(&Setting.Drv_Code[11], Driver_Info[i].Num , 7);
      addr = DRV_SETTING_PARM + offsetof(st_Setting,Drv_Code); 
      FRAMMultiWrite((u8 *)&Setting.Drv_Code, addr, sizeof(Setting.Drv_Code)); 
    }
    addr = DRIVER_STS+(i*sizeof(st_DRIVER_STS));
    FRAMMultiRead((u8*)&Driver_Sts[i], addr, sizeof(st_DRIVER_STS)); 
  }
    
  FRAMMultiRead((u8 *)&Dnld_Hist_Cnt, DNLD_HIST_CNT, 1);
  for(i = 0; i < 5; i++)
    FRAMMultiRead((u8 *)&Dnld_Hist[i], DNLD_HIST + i*6, 6);  
  
  FRAMMultiRead((u8 *)&Idle_RPM, IDLE_RPM, 2);
  if(Idle_RPM > 1500 || Idle_RPM == 0) Idle_RPM = 650;

  RTC_GetDateTime();

  FRAMMultiRead((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));
  if(isnan(DailyData.DFuel))
    DailyData.DFuel = 0;
  if(isnan(DailyData.TFuel))
    DailyData.TFuel = 0;

  DailyData_Change();

//  SecData.Trip = DailyData.Trip;
//  SecData.Fuel = DailyData.Fuel;
  SecData.DTrip = DailyData.DTrip;
  SecData.TTrip = DailyData.TTrip;
  SecData.DFuel = DailyData.DFuel;
  SecData.TFuel = DailyData.TFuel;
  
  SecData.Lat = DailyData.Lat;
  SecData.Lon = DailyData.Lon;
  SecData.Azi = DailyData.Azi;  

  BLE_CheckVehicleStop = 1;
  BLE_StopPoint.Time = RTC_LVal;
  BLE_StopPoint.Lat = SecData.Lat; 
  BLE_StopPoint.Lon = SecData.Lon; 
  BLE_StopPoint.Azi = SecData.Azi; 
  BLE_StopPoint.Speed = 0; 
  
  GPS_Fault = 1;  
  SecData.Status.GPS_Fault = GPS_Fault;
//  SecData.Signal.KeyOn = 1;

  Val.TTrip = DailyData.TTrip;// - (double)Setting.ODO_Offeset;
  Val.TTripOld = Val.TTrip;
    
  memset((u8*)&CAN_Val, 0, sizeof(stCanVal));

  if(Setting.K_Factor == 0) Setting.K_Factor = 5096;
  if(NotRegist_Flag != 2) {
    if(Setting.Car_Reg_No[1]) {
      VRN[0] = Car_No_Region[Setting.Car_Reg_No[1]][0];
      VRN[1] = Car_No_Region[Setting.Car_Reg_No[1]][1];
      VRN[2] = Car_No_Region[Setting.Car_Reg_No[1]][2];
      VRN[3] = Car_No_Region[Setting.Car_Reg_No[1]][3];
      VRN[4] = Setting.Car_Reg_No[2]+0x30;
      VRN[5] = Setting.Car_Reg_No[3]+0x30;
      VRN[6] = Car_No_Class[Setting.Car_Reg_No[4]][0];
      VRN[7] = Car_No_Class[Setting.Car_Reg_No[4]][1];
      VRN[8] = Setting.Car_Reg_No[5]+0x30;
      VRN[9] = Setting.Car_Reg_No[6]+0x30;
      VRN[10] = Setting.Car_Reg_No[7]+0x30;
      VRN[11] = Setting.Car_Reg_No[8]+0x30;      
    }
    else {
      VRN[0] = Setting.Car_Reg_No[2]+0x30;
      VRN[1] = Setting.Car_Reg_No[3]+0x30;
      VRN[2] = Car_No_Class[Setting.Car_Reg_No[4]][0];
      VRN[3] = Car_No_Class[Setting.Car_Reg_No[4]][1];
      VRN[4] = Setting.Car_Reg_No[5]+0x30;
      VRN[5] = Setting.Car_Reg_No[6]+0x30;
      VRN[6] = Setting.Car_Reg_No[7]+0x30;
      VRN[7] = Setting.Car_Reg_No[8]+0x30;  
      VRN[8] = 0;  
      VRN[9] = 0;  
      VRN[10] = 0;  
      VRN[11] = 0;  
    }
  }
  memcpy(&Setting.Time[0], (u8 *)&RTC_Val, 6);
  if(Setting.FW_Type == MDT_LG)
    memset(MDT_Min_Buff,0xFF,sizeof(MDT_Min_Buff));

  if(Setting.FW_Type == WIFI_LOOP) {
    FRAMMultiRead((u8*)&WifiNandAddr, WIFI_NAND_ADDR,4); 
    u32 paddr = DataSaveAddr - (DataSaveAddr%NAND_PAGE_SIZE);  
    if(WifiNandAddr > paddr) {
      WifiNandAddr = paddr;
      FRAMMultiWrite((u8*)&WifiNandAddr, WIFI_NAND_ADDR,4);   
    }
  }
  
  FRAMMultiRead((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
  if(SendData_StartTime >= RTC_LVal)  {
    SendData_StartTime = 0;
    FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
  }
  
#ifdef SENT_ADDR_INIT
  FRAMMultiRead((u8 *)&DataSaveAddr, DRV_SAVE_ADDR, sizeof(DataSaveAddr));   
  DataSentAddr = DataSaveAddr;
  FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr)); 
  VdisSentAddr = DataSaveAddr;
  FRAMMultiWrite((u8 *)&VdisSentAddr, VDIS_SENT_ADDR, sizeof(VdisSentAddr)); 
  SendData_StartTime = 0;
  FRAMMultiWrite((u8*)&SendData_StartTime, LAST_SENT_DAY, sizeof(SendData_StartTime));
#endif

  FRAMMultiRead((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
  if(IsgSetting.IdleTimeLimit == 0)  {
    IsgSetting.IdleTimeLimit = 180;
    addr = ISG_SETTING_PARM + offsetof(stIsgSetting,IdleTimeLimit); 
    FRAMMultiWrite((u8 *)&IsgSetting.IdleTimeLimit, addr, sizeof(IsgSetting.IdleTimeLimit)); 
  }
  if(IsgSetting.BatVoltLimit == 0)  {
    IsgSetting.BatVoltLimit = 230;
    addr = ISG_SETTING_PARM + offsetof(stIsgSetting,BatVoltLimit); 
    FRAMMultiWrite((u8 *)&IsgSetting.BatVoltLimit, addr, sizeof(IsgSetting.BatVoltLimit)); 
  }
  if(IsgSetting.InclineLimit == 0)  {
    IsgSetting.InclineLimit = 4;
    addr = ISG_SETTING_PARM + offsetof(stIsgSetting,InclineLimit); 
    FRAMMultiWrite((u8 *)&IsgSetting.InclineLimit, addr, sizeof(IsgSetting.InclineLimit)); 
  }  
  
  Adas_Char = ' ';
  Prev_Adas_Char = ' ';
  
  if(RTC_LVal > RTC_BK_Addr->LastOnTime && IsgSetting.FirstOnCheckTime > 0 && IsgSetting.IsgSkipTime > 0) {
    if(RTC_LVal - RTC_BK_Addr->LastOnTime > IsgSetting.FirstOnCheckTime*3600) {
      IsgSkipFlag = 1;
      IsgSkipCount = 0;
    }
  }
}

/**
  * @brief  Evt_Data_Save
  *         2048 : Sram_Data_Idx = 0~2047 loop
  * @param  None
  * @retval None
  */
#ifdef EVENT_DATA_SAVE_ENABLE
void Event_Data_Update(void)
{ 
  u16 i, idx;
  u32 speed, rpm;
  u8 brake = 0;
  
  if(NotRegist_Flag || !Start_Flag) return;
  
  if(CAN_Enable.Speed || CAN_Enable.Speed2)
    speed = CAN_Val.Speed;
  else {
    speed = SecData.Speed[0];
  }

  if(CAN_Enable.RPM || CAN_Enable.RPM2)
    rpm = CAN_Val.RPM;
  else {
    rpm = SecData.RPM;
  }
  
  if(CAN_Enable.Brake || CAN_Enable.Brake2)
    brake = CAN_Val.Brake;  
  else {
    if((Setting.Brake_Type == NORMAL_BRAKE && !BRAKE_SIG()) 
       || (Setting.Brake_Type == INVERSE_BRAKE && BRAKE_SIG())) 
      brake = 1;
    else             
      brake = 0;
  }
  
  if(speed > 255) speed = 255;
  if(rpm < 100) rpm = 0;
  else if(rpm > 9999) rpm = 9999;
    

  Evt.Data[Evt.Data_Idx].Speed   = (u8)speed;
  Evt.Data[Evt.Data_Idx].RPM     = (u16)rpm;
  Evt.Data[Evt.Data_Idx].Brake   = brake;
  Evt.Data[Evt.Data_Idx].G_X  = GS_Val_Conv[0];
  Evt.Data[Evt.Data_Idx].G_Y  = GS_Val_Conv[1];

  idx = Evt.Data_Idx/100;
  Evt.Lat[idx] = SecData.Lat;
  Evt.Lon[idx] = SecData.Lon;
  Evt.Azi[idx] = SecData.Azi;   
  Evt.Status[idx]  = SecData.Status;  
  
  if(Evt.Save_Flag && Evt.Save_Data_Idx < 2000) {
    Evt.Save_Data[Evt.Save_Data_Idx].Speed = Evt.Data[Evt.Data_Idx].Speed;
    Evt.Save_Data[Evt.Save_Data_Idx].RPM = Evt.Data[Evt.Data_Idx].RPM;
    Evt.Save_Data[Evt.Save_Data_Idx].Brake = Evt.Data[Evt.Data_Idx].Brake;
    Evt.Save_Data[Evt.Save_Data_Idx].G_X = Evt.Data[Evt.Data_Idx].G_X;
    Evt.Save_Data[Evt.Save_Data_Idx].G_Y = Evt.Data[Evt.Data_Idx].G_Y;
    idx = Evt.Save_Data_Idx/100;
    Evt.Save_Lat[idx] = Evt.Lat[Evt.Data_Idx/100];
    Evt.Save_Lon[idx] = Evt.Lon[Evt.Data_Idx/100];
    Evt.Save_Azi[idx] = Evt.Azi[Evt.Data_Idx/100];    
    Evt.Save_Status[idx] = Evt.Status[Evt.Data_Idx/100];
    Evt.Save_Data_Idx++;
  }
  
  Evt.Data_Idx = (Evt.Data_Idx + 1)%1000;
  

  if(Evt.Save_Flag == 0 && !KEY_OFF()) {
    // x : +/-1.2 1024*1.1 = 1126
    // y : +/-0.5 1024*0.5 = 512
    if(GS_Val[0] > GX_EVENT_VAL || GS_Val[0] < -1*GX_EVENT_VAL || GS_Val[1] > GY_EVENT_VAL || GS_Val[1] < -1*GY_EVENT_VAL) {  
      SecData.Signal.Event = 1;      
      DailyData.Evt_Count++;
      Evt_SubSecond = Evt.Data_Idx%100;
      for(i = 0; i < 1000; i++) {
        idx = (Evt.Data_Idx+i)%1000;
        Evt.Save_Data[i].Speed = Evt.Data[idx].Speed;
        Evt.Save_Data[i].RPM = Evt.Data[idx].RPM;
        Evt.Save_Data[i].Brake = Evt.Data[idx].Brake;
        Evt.Save_Data[i].G_X = Evt.Data[idx].G_X;
        Evt.Save_Data[i].G_Y = Evt.Data[idx].G_Y;
      }
      for(i = 0; i < 10; i++) {
        Evt.Save_Lat[i] = Evt.Lat[i];
        Evt.Save_Lon[i] = Evt.Lon[i];
        Evt.Save_Azi[i] = Evt.Azi[i];        
        Evt.Save_Status[i] = Evt.Status[i];        
      }
      Evt.Save_Data_Idx = 1000;
      Evt.Save_Flag = 1;
    }
  }
}


/**
  * @brief  Evt_After_Data_Update
  *
  * 낸드 뒤쪽 10 block 사용
  * @param  None
  * @retval None
  */
u8 EventTmpBuffer[NAND_PAGE_SIZE];
u8 Event_Time[7];
void Event_Data_Save(void)
{ 
  u32 i,j, idx;
  u16 hsize, esize, nsize, tmp;  
  u8 *time;
  st_DATA stemp;
  
  if(Evt.Save_Flag == 0 || NotRegist_Flag) return;
  
  esize = sizeof(st_DATA);  
  nsize = NAND_PAGE_SIZE/esize; //16
  hsize = sizeof(st_Setting);             //256
  memset((u8*)&stemp, 0, esize);
  memset(EventTmpBuffer, 0, NAND_PAGE_SIZE);

  if(Evt.Save_Flag == 1) {    //- start header write
    FRAMMultiRead((u8 *)&TmpSetting, DRV_SETTING_PARM, hsize); 
    TmpSetting.Identity = 0xEEEEEEEE;
    time = (u8 *)&RTC_Val;
    for(i = 0; i < 6; i++) {
      TmpSetting.Time[i] = *time++;
      Event_Time[i] = TmpSetting.Time[i];
    }
    TmpSetting.Time[6] = Evt_SubSecond;
    Event_Time[6] = TmpSetting.Time[6];

    memcpy(TmpSetting.Drv_Name, Driver_Info[Selected_Driver].Name, 10);
    memcpy(TmpSetting.Drv_Code, Driver_Info[Selected_Driver].Code, 10);
    TmpSetting.Drv_Code[10] = Selected_Driver + 0x31;
    memcpy(&TmpSetting.Drv_Code[11], Driver_Info[Selected_Driver].Num, 7);
    TmpSetting.ODO_Pulse_Cnt   = (u32)(SecData.TTrip*(float)TmpSetting.K_Factor);
    TmpSetting.DTrip_Pulse_Cnt = (u32)(SecData.DTrip*(float)TmpSetting.K_Factor);
    TmpSetting.Drv_Time        = DailyData.LapTime;
    
    if(Evt.Idx > 9) Evt.Idx = 0;
    Evt.Nand_Addr = NAND_EVENT_DATA_ADDR + Evt.Idx*NAND_BLOCK_SIZE;

    memcpy(EventTmpBuffer, (u8*)&TmpSetting, hsize);
    tmp = (NAND_PAGE_SIZE-hsize)/esize;
    for(i = 0; i < tmp; i++) {
      idx = i/100;
      for(j = 0; j < 7; j++)     
        stemp.Time[j] = Event_Time[j]; 
      stemp.Speed = Evt.Save_Data[i].Speed;
      stemp.RPM = Evt.Save_Data[i].RPM;
      stemp.G_X = Evt.Save_Data[i].G_X;
      stemp.G_Y = Evt.Save_Data[i].G_Y;
      stemp.Lat = Evt.Save_Lat[idx];
      stemp.Lon = Evt.Save_Lon[idx];
      stemp.Azi = Evt.Save_Azi[idx];
      stemp.Signal.Brake = Evt.Save_Data[i].Brake;
      stemp.Status = Evt.Save_Status[idx];          
      memcpy(&EventTmpBuffer[hsize+i*esize], (u8*)&stemp, esize);
    }
    Nand_Write_Buffer(EventTmpBuffer, Evt.Nand_Addr, NAND_PAGE_SIZE);
    Evt.Nand_Addr = Evt.Nand_Addr+NAND_PAGE_SIZE;
    Evt.Nand_Data_Idx = (NAND_PAGE_SIZE-hsize)/esize; //240
    Evt.Save_Flag = 2;
  }
  else if(Evt.Save_Flag == 2) {    //- start TmpSetting write
    if(Evt.Save_Data_Idx > Evt.Nand_Data_Idx+nsize) {
      for(i = 0; i < nsize; i++) {
        idx = (i+Evt.Nand_Data_Idx)/100;
        for(j = 0; j < 7; j++)     
          stemp.Time[j] = Event_Time[j]; 
        stemp.Speed = Evt.Save_Data[i+Evt.Nand_Data_Idx].Speed;
        stemp.RPM = Evt.Save_Data[i+Evt.Nand_Data_Idx].RPM;
        stemp.G_X = Evt.Save_Data[i+Evt.Nand_Data_Idx].G_X;
        stemp.G_Y = Evt.Save_Data[i+Evt.Nand_Data_Idx].G_Y;
        stemp.Lat = Evt.Save_Lat[idx];
        stemp.Lon = Evt.Save_Lon[idx];
        stemp.Azi = Evt.Save_Azi[idx];
        stemp.Signal.Brake = Evt.Save_Data[i+Evt.Nand_Data_Idx].Brake;
        stemp.Status = Evt.Save_Status[idx];          
        memcpy(&EventTmpBuffer[i*esize], (u8*)&stemp, esize);
      }
      Nand_Write_Buffer(EventTmpBuffer, Evt.Nand_Addr, NAND_PAGE_SIZE);
      Evt.Nand_Addr = Evt.Nand_Addr+NAND_PAGE_SIZE;
      Evt.Nand_Data_Idx = Evt.Nand_Data_Idx+nsize;
    }
    else if(Evt.Save_Data_Idx >= 2000) {
      nsize = Evt.Save_Data_Idx-Evt.Nand_Data_Idx;
      for(i = 0; i < nsize; i++) {
        idx = (i+Evt.Nand_Data_Idx)/100;
        for(j = 0; j < 7; j++)     
          stemp.Time[j] = Event_Time[j]; 
        stemp.Speed = Evt.Save_Data[i+Evt.Nand_Data_Idx].Speed;
        stemp.RPM = Evt.Save_Data[i+Evt.Nand_Data_Idx].RPM;
        stemp.G_X = Evt.Save_Data[i+Evt.Nand_Data_Idx].G_X;
        stemp.G_Y = Evt.Save_Data[i+Evt.Nand_Data_Idx].G_Y;
        stemp.Lat = Evt.Save_Lat[idx];
        stemp.Lon = Evt.Save_Lon[idx];
        stemp.Azi = Evt.Save_Azi[idx];
        stemp.Signal.Brake = Evt.Save_Data[i+Evt.Nand_Data_Idx].Brake;
        stemp.Status = Evt.Save_Status[idx];          
        memcpy(&EventTmpBuffer[i*esize], (u8*)&stemp, esize);
      }
      tmp = nsize*esize;
      Nand_Write_Buffer(EventTmpBuffer, Evt.Nand_Addr, tmp);
      Evt.Idx++;
      Evt.Idx = Evt.Idx%10;
      Evt.Save_Data_Idx = 0;
      Evt.Nand_Data_Idx = 0;
      Evt.Save_Flag = 0;
      FRAMByteWrite(Evt.Idx, EVT_IDX); 
      SecData.Signal.Event = 0;      
      if(Power_Off_Flag && USB_Detect_Flag) {     
#ifdef DOWNLOAD_TRIP_DATA_TO_USB
        if(TripStartTime > 0) {
          lcd_clear();
          Menu_DisplayTitle(0,0,0,0); //다운로드
          DownloadDisplayInit();
          USB_DataDL_Flag = 1;
          USB_DataDL_LastTrip = 1;       
        }
#endif        
        USB_DrvVeh_Flag = 1;
        USB_DrvVeh_Mode = 1;
      }
      else if(Power_Off_Flag) Power_Off();
    }
  }
}
#endif



/*
********************************************************************************
* Description : UpdateTimeToGPSTime
* Arguments   : None
* Return      : 
* Note        :  UpdateTimeToGPSTime
******************************************************************************** 
*/       
void UpdateTimeToGPSTime(void)
{  
  rtc_time rtc_val;

  if(GPS_Time_Sync)  {
    rtc_val = gmtime_conv(RTC_GPS_Val);
    RTC_SetDateTime((u8 *)&rtc_val);
    GPS_Time_Sync = 0;
  }
}

/*
********************************************************************************
* Description : CheckGPSTimeSyc
* Arguments   : None
* Return      : 
* Note        :  CheckGPSTimeSyc
******************************************************************************** 
*/       
u32 Prev_rmctime;
u32 GPS_SyncOKTime;
void CheckGPSTimeSyc(void)
{
  rtc_time gps_time;
  u32 diff = 0;
  
  if(GPS_Valid && rmc.time != Prev_rmctime) {
    gps_time.tm_hour = rmc.time/10000;
    gps_time.tm_min  = (rmc.time/100)%100; 
    gps_time.tm_sec  = rmc.time%100;
    gps_time.tm_year = rmc.date%100;
    gps_time.tm_mon  = (rmc.date/100)%100;
    gps_time.tm_mday = rmc.date/10000;
    RTC_GPS_Val = RTCValTotime(gps_time)+TIMESHIFT_SEC;
    diff = abs(RTC_GPS_Val-RTC_LVal);
    if(RTC_GPS_Val > LOW_LIMT_OF_TIME) {
      if(diff > 3) {
        GPS_Time_Sync = 1; 
        if(diff > 86400 && GPS_SyncOKTime < 60)   UpdateTimeToGPSTime();   //하루이상 시간차일때 바로 업데이트 하고 그렇지 않으면 키오프시 업데이트..
      }
      else GPS_SyncOKTime++; //GPS 와 시간이 3초이내 차이일 경우 카운트 
    }
    Prev_rmctime = rmc.time;
    GPS_Time_NoSyncCnt = 0;
  }  
  else {
    if(GPS_Time_Sync) {
      GPS_Time_NoSyncCnt++;
      if(GPS_Time_NoSyncCnt > 30) GPS_Time_Sync = 0;  //약 3초 
    }
  }
}

/*
********************************************************************************
* Description : Parameter_Save
* Arguments   : None
* Return      : 
* Note        : Parameter Save
******************************************************************************** 
*/       
void Parameter_Save(void)
{  
  Nand_Header_Flag = 0;  

  if(Contrast_Flag)
    FRAMByteWrite(Contrast, SET_CONTRAST);  

  if(NotRegist_Flag) return;  
  FRAMByteWrite(Main_Disp_Mode, MAIN_DISPLAY);
  FRAMByteWrite(Backligth_Sel, BACKLIGHT_SEL); //1.4.0.0

//      RTC_BK_Addr->Trip    = DailyData.Trip;
//      RTC_BK_Addr->DTrip    = DailyData.DTrip;
//      RTC_BK_Addr->TTrip    = DailyData.TTrip;
//      RTC_BK_Addr->Fuel    = DailyData.Fuel;
//      RTC_BK_Addr->DFuel    = DailyData.DFuel;
//      RTC_BK_Addr->TFuel    = DailyData.TFuel;

  FRAMMultiWrite((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));
  UpdateTimeToGPSTime();
}

/*
********************************************************************************
* Description : Day_Change
* Arguments   : None
* Return      : 
* Note        : Day Change
******************************************************************************** 
*/    
void Day_Change(void)
{  
  if(Day_Change_Flag) {
    Setting.Day_Change++;
    DailyData_Change();  
    Day_Change_Flag = 0;
  }  
}

/*
********************************************************************************
* Description : DailyData_Change
* Arguments   : None
* Return      : 
* Note        : Daily Data Change
******************************************************************************** 
*/
void DailyData_Change(void)
{     
  u8 dfday = 0;
  rtc_time r;
  
  if(NotRegist_Flag) return;

  if(RTC_Val.tm_year < 10 && DailyData.Time < RTC_LVal) return;
  
  if(!IsSameDay(RTC_LVal+1, DailyData.Time)) dfday = 1;

  if(dfday) {
    if(DailyData.NoOfSecData > 0) {
      Nand_Write_DailyData();
      DailyData.Index++;
    }
    DailyData.Time = RTC_LVal+1;
    r = gmtime_conv(DailyData.Time);
    memcpy(&Setting.Time[0], (u8 *)&r, 6);
    
    DailyData.NandStartAddress = DataSaveAddr;
    DailyData.NoOfSecData = 0;
    DailyData.TripCount = 1;
    DailyData.Trip = 0;
    DailyData.DTrip = 0;
    DailyData.Fuel = 0;
    DailyData.DFuel = 0;
    DailyData.LapTime = 0;
    DailyData.DLapTime = 0;

    DailyData.Idle_Time = 0;
    DailyData.IdleStop_Time = 0;
    DailyData.IdleStop_Cnt = 0;
    
    DailyData.Evt_Count = 0;
    FRAMMultiWrite((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));
    
    SecData.Trip = 0;
    SecData.Fuel = 0;
    SecData.DTrip = 0;
    SecData.DFuel = 0;

    ADASPckNo.AdasTripNo = 0;
    FRAMMultiWrite((u8 *)&ADASPckNo, ADAS_HEADER, sizeof(tsADASPckNo));
    if(!TripAdasFlag) {
      TripAdasFlag = 1;
      OffAdasFlag = 1;
    }
  }
  
  Nand_Header_Flag = 1;
}  

/*******************************************************************************
* Function Name  : MDTDataUpdate
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MDT_DataUpdate(void)
{ 
  u8 min, sec, i;
  u16 tmp;


  min = RTC_Val.tm_min%4;
  sec = RTC_Val.tm_sec;

  MDT_Min_Buff[min][sec].Speed  = SecData.Speed[0];
  
  MDT_Min_Buff[min][sec].RPM    = SecData.RPM;

  tmp = *((u16*)&SecData.Status);
  if(Setting.FW_Type == MDT_LG) {
    MDT_Min_Buff[min][sec].Status = tmp;
    MDT_Min_Buff[min][sec].AUX    = *((u8*)&SecData.Signal)<<1;
    if(!KEY_OFF()) {
      MDT_Key_Off_Time=0;
    }
  }
  else  {
    tmp = tmp<<1;
#ifdef EVENT_DATA_SAVE_ENABLE
    MDT_Min_Buff[min][sec].Status = tmp|(Evt.Save_Flag<<15);
#else
    MDT_Min_Buff[min][sec].Status = tmp;
#endif    
    MDT_Min_Buff[min][sec].AUX    = (u8)SecData.Signal.Brake;
    if(Setting.DTG_Type == DTG_GREEN) {
      MDT_Min_Buff[min][sec].DFuel    = (u32)SecData.DFuel;
      MDT_Min_Buff[min][sec].TFuel    = (u32)SecData.TFuel;
    }
  }

  MDT_Min_Buff[min][sec].Lat    = SecData.Lat;
  MDT_Min_Buff[min][sec].Lon    = SecData.Lon;
  MDT_Min_Buff[min][sec].Azi    = SecData.Azi;  
  
  MDT_DTrip[min] = (u32)(SecData.DTrip*10.0);
  MDT_ODO[min] = (u32)SecData.TTrip;
  
  if(sec == 0) {
    MDT_MTrip[min] = 0.0;
    MDT_MFuel[min] = 0.0;
    MDT_AEBS[min] = 0;
    MDT_LDWS1[min] = 0;
    MDT_LDWS2[min] = 0;
  }

  MDT_MTrip[min] += SecMTrip;
  MDT_MFuel[min] += SecFuelRate;
  MDT_DFuel[min] = (u32)(SecData.DFuel);
  MDT_TFuel[min] = (u32)(SecData.TFuel);
  MDT_AEBS[min] |= CAN_Val.AEBS;
  MDT_LDWS1[min] |= CAN_Val.LDWS1;
  MDT_LDWS2[min] |= CAN_Val.LDWS2;  

  memcpy(MDT_SecDataTimeBuffer,&RTC_Val,6);                                        
  MDT_SecDataTimeBuffer[6] = 0;    
  for(i = sec+1; i < 60; i++) 
    memcpy((u8*)&MDT_Min_Buff[min][i], (u8*)&MDT_Min_Buff[min][sec], sizeof(st_MDT_DATA));
  MDT_LastDataTime = RTC_LVal;
}


/*
********************************************************************************
* Description : CheckIsg
* Arguments   : 
* Return      : 
* Note        : IdlingStopCheck
******************************************************************************** 
*/
u8 IsgAccOffTime, IsgAccOffFault;
u16 InclineAngle;
u8 IsgGpsPointFlag;
short CoolantTemp;
#define ISC_DO_OFF_TIME 5
#define ISC_LOW_RPM_LIMIT 100
#define ISC_UPPER_RPM_LIMIT 1000
#define ISC_KEYON_CHECK_RPM  300
u8 DO1_Check;
void CheckIsg(void)
{      
  if(IsgAccOff > 0 ) {   //Idling Stop후 다시 시동...
    if(IsgAccOff == 1) {
      SecData.IsgStatus = 30;
      if(IsgAccOffTime >= ISC_DO_OFF_TIME) {
        DO1_RESET();
        IsgAccOff = 2;
      }
    }
    else if(IsgAccOff == 2) {
      if(IsgAccOffTime >= ISC_DO_OFF_TIME + 5) {
        if(SecData.RPM > ISC_KEYON_CHECK_RPM) {   ///Idle stop 실패
          SecData.IsgStatus = 50;
          IsgAccOffFault = 1;
          IsgAccOff = 0;
        }
        else {
          SecData.IsgStatus = 40;
          DailyData.IdleStop_Cnt++;  
          IsgAccOff = 3;
        }
      }
    }
    else if(IsgAccOff == 3) {
      if(SecData.RPM > ISC_KEYON_CHECK_RPM) {
        IdleTimeCount = 0;
        IsgAccOff = 0;
        IsgAccOffTime = 0;
      }
      else {
        IdleTimeCount++;
      }
    }
    DO1_Check = DO1_CHECK();
    if(IsgAccOff > 1 && DO1_Check) DO1_RESET();
    if(IsgAccOffTime < 255)      IsgAccOffTime++;
    return;
  }
  else {
    DO1_Check = DO1_CHECK();
    if(DO1_Check) DO1_RESET();
  }
  
  if(IsgAccOffFault || VehMaintenanceFlag || IsgSkipFlag) { ///Idle stop 실패시 계속 시도하지 않고 다음 상황에서  다시 시도.. --> 다음 KeyOn시다시시도
    if(IsgSkipFlag) {
      IsgSkipCount++;
      if(IsgSkipCount > IsgSetting.IsgSkipTime*60) {
        IsgSkipFlag = 0;
        IsgSkipCount = 0;
      }
    }
    if(VehMaintenanceFlag)  
      SecData.IsgStatus = 99;
    else if(IsgSkipFlag) 
      SecData.IsgStatus = 90;
    IdleTimeCount = 0;
    IsgAccOff = 0;
    IsgAccOffTime = 0;
    return;
  }

  if(IsgSetting.IsgMode == 1) {
    SecData.IsgStatus = 1;
    if(SecData.Speed[0] == 0 && !DpfFlag) {
      SecData.IsgStatus = 2;
      if(SecData.RPM > ISC_LOW_RPM_LIMIT && SecData.RPM  < ISC_UPPER_RPM_LIMIT) {
        SecData.IsgStatus = 3;
        if(SecData.BatVolt  > IsgSetting.BatVoltLimit) {
          SecData.IsgStatus = 4;
          if(CoolantTemp > IsgSetting.CoolTempLimit*10) {
            SecData.IsgStatus = 5;
            if(InclineAngle < IsgSetting.InclineLimit*10) {
              SecData.IsgStatus = 6;
              if(SecData.Signal.PBrake == 1) {
                SecData.IsgStatus = 7;
                if(SecData.Signal.NGear == 1) {
                  SecData.IsgStatus = 8;
                  if(SecData.Signal.HpAirAlarm == 0) {
                    SecData.IsgStatus = 9;
//                    if(!IsgSetting.GpsMode || IsgGpsPointFlag) {
                    if(IsgSetting.GpsMode && IsgGpsPointFlag) {
                      SecData.IsgStatus = 10;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else {
    SecData.IsgStatus = 0;
  }
  if(SecData.IsgStatus == 10) {
    IdleTimeCount++;
    if(IdleTimeCount > IsgSetting.IdleTimeLimit) {
      if(IsgAccOff == 0) {
        DO1_SET();
        SecData.IsgStatus = 20;
        IsgAccOff = 1;
        IsgAccOffTime = 0;
      }
    }
  }
  else {
    IdleTimeCount = 0;
    IsgAccOff = 0;
    IsgAccOffTime = 0;
  }
}

/*******************************************************************************
* Function Name  : DrvDataUpdate
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
tsSecData PrevSecData;
double SpeedTrip, CanTrip, PrevCanOdo;
u8 SpeedTripEnable, PreSpeed;
u16 TestRPM;
void DrvDataUpdate(void)
{ 
	sysinfo_t *s = &_sysinfo 	;
  
  u32 speed = 0, rpm = 0;
  double dist = 0;
  double carf;
  u8 brake = 0;
  unsigned long lat, lon;
  u32 speed_pulse_count = 0;
  double trip_delta = 0;
  int deltaspeed = 0;
  
  if(s->pSpeed.pulse_period > 10000 && s->pSpeed.pulse_count > 0) 
    speed = (u32)(((double)s->pSpeed.pulse_count-1)/((double)s->pSpeed.pulse_period*0.00005)*3600.0/(double)Setting.K_Factor+0.5);
  else speed = 0;
  speed_pulse_count = s->pSpeed.pulse_count ;
  s->pSpeed.pulse_count = 0;
  
  if(s->pRpm.pulse_period > 10000 && s->pRpm.pulse_count > 0)
   rpm   = (u32)(((double)s->pRpm.pulse_count-1)/((double)s->pRpm.pulse_period*0.00005)*60.0/((double)Setting.RPM_Factor/100.0)+0.5);
  else rpm = 0;
  s->pRpm.pulse_count = 0; 

  if(speed > 255) speed = 255;
  if(rpm < 100 || rpm > 6000) rpm = 0;
  
#if 1  //for ISG Test
  if(rpm == 0) rpm = TestRPM;
#endif
  
  if((Setting.Brake_Type == NORMAL_BRAKE && !BRAKE_SIG()) 
     || (Setting.Brake_Type == INVERSE_BRAKE && BRAKE_SIG())) 
    brake = 1;
  else             
    brake = 0;
  
  if(Setting.Can_Enable && (CAN_Enable.Speed || CAN_Enable.Speed2) && speed == 0)
    speed = CAN_Val.Speed;

  if((CAN_Enable.RPM || CAN_Enable.RPM2) && rpm == 0)
    rpm = CAN_Val.RPM;

  if(CAN_Enable.Brake || CAN_Enable.Brake2)
    brake = CAN_Val.Brake;

  if (Setting.Can_Enable  && (CAN_Enable.ODO || CAN_Enable.ODO2) && Setting.ODO_Offeset == 0) {
    if(SpeedTrip > 0.3  && CanTrip < 0.01) {
      if(!SpeedTripEnable) {
        SpeedTripEnable = 1;
        Val.TTrip += SpeedTrip;
      }
      else
        Val.TTrip += speed/3600.0;   
    }
    else {
      Val.TTrip = CAN_Val.ODO;     
    }
    SpeedTrip +=  (double)speed/3600.0;
    if(CAN_Val.ODO - PrevCanOdo > 0 && CAN_Val.ODO-PrevCanOdo < 1)
      CanTrip += CAN_Val.ODO - PrevCanOdo;
    PrevCanOdo = CAN_Val.ODO;
  }
  else {
    if(!Setting.Can_Enable && Setting.K_Factor > 0)  
      dist = (double)speed_pulse_count/(double)Setting.K_Factor;
    else 
      dist = (double)speed/3600.0;
    Val.TTrip += dist;    
  }

  if(Val.TTrip > Val.TTripOld) {
    trip_delta = Val.TTrip-Val.TTripOld;
    if(trip_delta < 1.0) {
      SecData.Trip += trip_delta;//Val.TTrip-Val.TTripOld;
      SecData.DTrip += trip_delta;//Val.TTrip-Val.TTripOld;
    }
    else trip_delta = 0;
    SecData.Status.Dist_Fault = 0;
  }
  else if(Val.TTrip < Val.TTripOld) {
    SecData.Status.Dist_Fault = 1;
  }
  SecMTrip = trip_delta*1000.0;
//  if(DailyData.TTrip < 1.0 && Setting.ODO_Offeset == 0)
//    DailyData.TTrip = Val.TTrip;
//  else
    DailyData.TTrip = DailyData.TTrip+trip_delta; //Val.TTrip + (double)Setting.ODO_Offeset;  
  Val.TTripOld = Val.TTrip;

  Val.FuelRate = 0;
  if(CAN_Enable.FuelRate)  {
    if(Setting.Car_SetFactor > 50 && Setting.Car_SetFactor < 150) 
      carf = (double)Setting.Car_SetFactor/100.0;
    else               
      carf = Car_Factor[Setting.Car_Idx]; //1.4.0.0
    if(CAN_Val.FuelRateCnt > 0) 
      Val.FuelRate = CAN_Val.FuelRateSum/(double)CAN_Val.FuelRateCnt/1000.0*carf; //1.4.0.0    
    Val.FuelRate = Val.FuelRate*(double)Calib_F_Factor/100.0;
    CAN_Val.FuelRateSum = 0;
    CAN_Val.FuelRateCnt = 0; 
  }  
  
  if(Val.FuelRate > 0) {
    DailyData.TFuel += Val.FuelRate;
    SecData.Fuel += Val.FuelRate;
    SecData.DFuel += Val.FuelRate;
    Val.FuelDist = (double)(speed/3.6)/Val.FuelRate*10.0;
  }
  SecFuelRate = Val.FuelRate; 
  
  SecData.Time = RTC_LVal;
  SecData.Speed[0] = (u8)speed;
  if(SecData.Speed[1] < SecData.Speed[0])
    SecData.Speed[1] = SecData.Speed[0];
  SecData.RPM = (u16)rpm;
  SecData.Signal.Brake = brake & 0x01;

  BLE_speed_check(speed);

  if(Setting.Can_Enable) { 
    if(SecData.RPM == 0 || CAN_Key_Off_Time > 2 || KEY_OFF() || URLChangeFlag) 
      SecData.Signal.KeyOn = 0;
    else
      SecData.Signal.KeyOn = 1;
    
    if(SecData.RPM == 0) CAN_Key_Off_Time++;
    else CAN_Key_Off_Time = 0;
  }
  else {
    if(KEY_OFF() || URLChangeFlag) 
      SecData.Signal.KeyOn = 0;
    else 
      SecData.Signal.KeyOn = 1;
  }
  
  if(GPS_Valid) {
    GPS_Speed_Val = (u32)(rmc.speed  * 1.852);
    // GPS 데이터 
    lat = (unsigned long)(gga.latitude  * 1000000.0);
    lon = (unsigned long)(gga.longitude * 1000000.0);
    if((SecData.Time - PrevSecTime) < 2) {
      if(abs(PrevLat-lat) < 50000) {
        SecData.Lat = lat;
      }
      else SecData.Lat = 0;
      if(abs(PrevLon-lon) < 50000) {
        SecData.Lon = lon;
      }
      else SecData.Lon = 0;
    }
    else {
      SecData.Lat = 0;
      SecData.Lon = 0;
    }
 
    SecData.Azi  = (unsigned short)rmc.course;
    PrevSecTime = SecData.Time;
    PrevLat = lat; 
    PrevLon = lon;
    GPSNotValidCount = 0;
  }
  else {
    if(GPSNotValidCount < 60)   GPSNotValidCount++;
    if(!SecData.Signal.KeyOn || GPSNotValidCount >= 60) {
      SecData.Lat = 0;
      SecData.Lon = 0;
      SecData.Azi = 0;
      GPS_Speed_Val = 0;
    }
  }

  if(SecData.Lat == 0 || SecData.Lon == 0) 
    GPS_Fault = 1;
  
  if(GPS_Fault) {
    SecData.Lat = DailyData.Lat;
    SecData.Lon = DailyData.Lon;
    SecData.Azi = DailyData.Azi;
  }
  
  SecData.GPS_Status = GPS_Valid_Char;
  SecData.GPS_Speed = GPS_Speed_Val;
  
  if(speed > 0) {
    if(rpm == 0)
      SecData.Status.RPM_Fault = 1;
    else
      SecData.Status.RPM_Fault = 0;
    SecData.Status.Speed_Fault = 0;
  }
  else if(GPS_Speed_Val > 30) {
    SecData.Status.Speed_Fault = 1;
  }
  SecData.Status.GPS_Fault = GPS_Fault;
    
  SecData.AccX = (s16)((double)GS_Val_Max[0]*1.08);
  SecData.AccY = (s16)((double)GS_Val_Max[1]*1.08);
  for(int i = 0; i < 3; i++)
    GS_Val_Max[i] = GS_Val_Conv[i];

  SecData.DriverIndex = Selected_Driver;

  SecData.TFuel = DailyData.TFuel;  
  SecData.TTrip = DailyData.TTrip;
  
  if(CAN_Enable.AEBS) {
    SecData.AEBS = CAN_Val.AEBS;  
    if((SecData.AEBS & 0x0F) == 0)  SecData.AEBS |= 0x01; //AEBS 신호  정상 상태
  }
                                    //bit 1,2,3,4               
                                    //0010 운전자가 스위치로 AEBS 시스템을 끈 것
                                    //0011 AEBS 작동 대기의 정상 상태
                                    //0101 단순 경보 상태
                                    //0110 부분 제동 동작 상태
                                    //0111 풀제동 동작 상태 
                                    //bit 5,6,7,8 for Mobile Eye ADAS
                                                       //bit 5: Mobile eye FCW ON 
                                                       //bit 6: Mobile eye Peds_FCW ON 
                                                       //bit 7,8: Mobile eye Head way warning level (0~2 단계)
  else if(UartADAS_Enable) {
    if(UartADAS_FCWS == 0x31) {     //FCW 1차경고
      SecData.AEBS |= 0x01;         //AEBS 신호  정상 상태      
      SecData.AEBS |= (0x01<<4);    //FCW_ON
      SecData.AEBS |= (0x01<<6);    //FCW_1단계
    }
    else if(UartADAS_FCWS == 0x32) {//FCW 2차경고 
      SecData.AEBS |= 0x01;         //AEBS 신호  정상 상태      
      SecData.AEBS |= (0x01<<4);    //FCW_ON
      SecData.AEBS |= (0x02<<6);    //FCW_2단계
    }
    else if(UartADAS_FCWS == 0x30) {//FCW 경고 없음
      SecData.AEBS |= 0x01;         //AEBS 신호  정상 상태              
      SecData.AEBS &= ~(0x01<<4);   //FCW_ON 삭제
      SecData.AEBS &= ~(0x03<<6);   //FCW_1,2단계 삭제
    }
    else SecData.AEBS = 0;
  }
  else SecData.AEBS = 0;

  if(CAN_Enable.LDWS) {
    SecData.LDWS = (((CAN_Val.LDWS2>>4)&0x0F)+(CAN_Val.LDWS1&0xF0));
    if((SecData.LDWS & 0x03) == 0)  SecData.LDWS |= 0x01; //LDWS 신호  정상 상태
  }
                                    //bit1,2  Reserved
                                    //bit3,4  차선이탈 경고 On/Off  (00 표시등 On (LDWS 꺼짐) , 01 표시등 Off (LDWS 켜짐))
                                    //bit5,6  Right Lane (00 : 이탈없음  01: 차선이탈경고, 10:  경고등Error  11: Not Available)
                                    //bit7,8  Left Lane (00 : 이탈없음  01: 차선이탈경고, 10:  경고등Error  11: Not Available)  
  else if(UartADAS_Enable) {
    if(UartADAS_LDWS == 0x31) {
      SecData.LDWS |= 0x01;         //LDWS 신호  정상 상태      
      SecData.LDWS |= (0x01<<2);    //LDWS ON
      SecData.LDWS |= (0x01<<6);    //left 차선이탈
    }
    else if(UartADAS_LDWS == 0x32) {
      SecData.LDWS |= 0x01;         //LDWS 신호  정상 상태      
      SecData.LDWS |= (0x01<<2);    //LDWS ON
      SecData.LDWS |= (0x01<<4);    //right 차선이탈
    }
    else if(UartADAS_LDWS == 0x30) {
      SecData.LDWS |= 0x01;         //LDWS 신호  정상 상태
      SecData.LDWS |= (0x01<<2);    //LDWS ON
    }
    else
      SecData.LDWS = 0;             //LDWS OFF, 차선이탈 경고없음         
  }
  else SecData.LDWS = 0;

  if((SecData.LDWS & 0x30) == 0x10) Adas_Char = 'R';
  else if((SecData.LDWS & 0xC0) == 0x40) Adas_Char = 'L';
  
  else if((SecData.AEBS & 0x10) == 0x10) {
    if((SecData.AEBS&0xC0) == 0x40)    Adas_Char = '1';
    else if((SecData.AEBS&0xC0) == 0x80)    Adas_Char = '2';
    else    Adas_Char = 'F';
  }
  else  if((SecData.AEBS & 0x20) == 0x20) Adas_Char = 'P';
  else if(CAN_Enable.AEBS || CAN_Enable.LDWS || UartADAS_Enable) Adas_Char = 'A';
  else Adas_Char = ' ';
  
#ifdef USE_CAN2_ADAS
  CAN_Enable.AEBS = 0;
  CAN_Enable.LDWS = 0;
#endif  
  
#ifndef ISG_ENABLE  
  if(DI1_SIG()) SecData.Signal.Door = 1;
  else SecData.Signal.Door = 0;
  
  SecData.IsgStatus = RxEvent_Count;
    
  if(TempStatus.TempEnable) {
    if(TempStatus.Ch1Enable) {
      SecData.Temp1 = Temp_dVal[0];
      SecData.Signal.Temp1 = 1;
    }
    else {
      SecData.Temp1 = 0;
      SecData.Signal.Temp1 = 0;
    }
    if(TempStatus.Ch2Enable) {
      SecData.Temp2 = Temp_dVal[1];
      SecData.Signal.Temp2 = 1;
    }
    else {
      SecData.Temp2 = 0;
      SecData.Signal.Temp2 = 0;
    }
    
    if(TempStatus.Ch1Short || TempStatus.Ch2Short)
      SecData.Status.Short_Fault = 1;
    else 
      SecData.Status.Short_Fault = 0;

    if(TempStatus.Ch1Open || TempStatus.Ch2Open)
      SecData.Status.Open_Fault = 1;
    else 
      SecData.Status.Open_Fault = 0;
  }
  else {
    SecData.Temp1 = 0;
    SecData.Signal.Temp1 = 0;
    SecData.Temp2 = 0;
    SecData.Signal.Temp2 = 0;
  }
#else  
  SecData.Signal.Temp1 =  0;
  SecData.Signal.Temp2 =  0;
  if(CAN_Enable.CoolantTemp)
    SecData.Temp1 = CAN_Val.CoolantTemp*10;
  else
    SecData.Temp1 = IsgSetting.CoolTempLimit*10+10;
  CoolantTemp = SecData.Temp1;

  if(CAN_Enable.DPF) DpfFlag = CAN_Val.DPF;
  else DpfFlag = 0;
    
  double gval;
  
  gval = (double)GS_Val[0]/(double)(GSENSOR_1G_VAL);
  if(gval > 0.999) gval = 1;
  SecData.Temp2 = (short)((asin(gval)*180.0/3.141592) * 10.0);
  InclineAngle = abs(SecData.Temp2);
  
  if(IsgSetting.PBrakeMode == 0) {
    if(DI1_SIG()) SecData.Signal.PBrake = 1;
    else SecData.Signal.PBrake = 0;
  }
  else if(IsgSetting.PBrakeMode == 1) {
    if(DI1_SIG()) SecData.Signal.PBrake = 0;
    else SecData.Signal.PBrake = 1;
  }
  else if(IsgSetting.PBrakeMode == 2) {
    if(CAN_Enable.PBrake) {
      if(CAN_Val.PBrake) SecData.Signal.PBrake = 1;
      else SecData.Signal.PBrake = 0;
    }
    else SecData.Signal.PBrake = 0;
  }
  else SecData.Signal.PBrake = 0;

  if(IsgSetting.AirPressMode == 0) {
    if(DI2_SIG()) SecData.Signal.HpAirAlarm = 1;
    else SecData.Signal.HpAirAlarm = 0;
  }
  else if(IsgSetting.AirPressMode == 1) {
    if(DI2_SIG()) SecData.Signal.HpAirAlarm = 0;
    else SecData.Signal.HpAirAlarm = 1;
  }
  else if(IsgSetting.AirPressMode == 2) {
    if(CAN_Enable.LPAlarm) {
      if(CAN_Val.LPAlarm) SecData.Signal.HpAirAlarm = 1;
      else SecData.Signal.HpAirAlarm = 0;
    }
    else SecData.Signal.HpAirAlarm = 0;
  }
  else SecData.Signal.HpAirAlarm = 0;

  if(IsgSetting.NGearMode == 0) {
    if(DI1_SIG()) SecData.Signal.NGear = 1;
    else SecData.Signal.NGear = 0;
  }
  else if(IsgSetting.NGearMode == 1) {
    if(DI1_SIG()) SecData.Signal.NGear = 0;
    else SecData.Signal.NGear = 1;
  }
  else if(IsgSetting.NGearMode == 2) {
    if(CAN_Enable.NGear) {
      if(CAN_Val.NGear) SecData.Signal.NGear = 1;
      else SecData.Signal.NGear = 0;
    }
    else SecData.Signal.NGear = 0;
  }
  else SecData.Signal.NGear = 1;
  
  
  double x_dist,y_dist, gps_dist;
  u32 tmp_dist;
  u8 i;
  if(IsgSetting.GpsMode && GPS_Valid) {
    IsgGpsPointFlag = 0;
    for(i = 0; i < IsgSetting.GeoFenceCnt; i++) {
      tmp_dist = abs(SecData.Lon - IsgSetting.GeoFence[i].Lon);
      x_dist = (double)tmp_dist*0.088;
      tmp_dist = abs(SecData.Lat - IsgSetting.GeoFence[i].Lat);
      y_dist = (double)tmp_dist*0.111;
      gps_dist = sqrt((x_dist*x_dist)+(y_dist*y_dist));
      if(gps_dist < IsgSetting.GeoFence[i].Radius) {
        IsgGpsPointFlag = 1;
        break;
      }
    }
  }
  else
    IsgGpsPointFlag = 0;

  if(SecData.Signal.KeyOn || IsgAccOff)
    CheckIsg();
  else
    IdleTimeCount = 0;

  if((SecData.Speed[0] == 0 && SecData.RPM > ISC_LOW_RPM_LIMIT) || IsgAccOff > 0)
    DailyData.Idle_Time++;  
  
  if(SecData.IsgStatus == 40) {
    DailyData.IdleStop_Time++; 
    IdleStop_Time++;
    if(IsgSetting.IdleFuelRate == 0) IsgFuelSaveFlag = 1;
  }
  else IdleStop_Time = 0;
  
  if(SecData.IsgStatus == 10)  {
    IsgSetting.IdleFuelRate = (u16)(Val.FuelRate*1000);  
  }
#endif  
  
  DailyData.Trip = SecData.Trip;  
  DailyData.DTrip = SecData.DTrip;  
  DailyData.Fuel = SecData.Fuel;  
  DailyData.DFuel = SecData.DFuel; 
  
  DailyData.DLapTime++;  
  DailyData.Drv_Time++;
  DailyData.LapTime++;  
  DailyData.Lat = SecData.Lat;
  DailyData.Lon = SecData.Lon;
  DailyData.Azi = SecData.Azi;  

  // ADAS Trip 평균/최고 속도
  TripLapTimeCnt++;
  TripSpeedSum += SecData.Speed[0];
  ADASPckData.TripAvgSpeed = TripSpeedSum / TripLapTimeCnt;
  if(SecData.Speed[0] > ADASPckData.TripMaxSpeed)
    ADASPckData.TripMaxSpeed = SecData.Speed[0];
  
  // ADAS 구간별 운행 시간
  if(SecData.Speed[0] <= 40) ADASPckData.Speed40TIme++;
  else if (SecData.Speed[0] <= 60) ADASPckData.Speed4060TIme++;
  else if (SecData.Speed[0] <= 80) ADASPckData.Speed6080TIme++;
  else if (SecData.Speed[0] <= 100) ADASPckData.Speed80100TIme++;
  else ADASPckData.Speed100TIme++;

  // ADAS GPS (소숫점 5자리)
  ADASPckData.AdasGPSLon = (unsigned long)(gga.longitude * 100000.0);
  ADASPckData.AdasGPSLat = (unsigned long)(gga.latitude  * 100000.0);
  
  // ADAS 급가속, 급출발 횟수, 과속시간
  deltaspeed = (int)(SecData.Speed[0] - PreSpeed);
  if(deltaspeed > 10)
    ADASPckData.OverSpeedCnt++;
  if(deltaspeed < -10)    
    ADASPckData.UnderSpeedCnt++;
  if(SecData.Speed[0] > 100)
    ADASPckData.OverSpeedTIme++;
  PreSpeed = SecData.Speed[0];
  
  memcpy(SecDataTimeBuffer,&RTC_Val,6);                                        
  SecDataTimeBuffer[6] = 0;
  
  if(SecData.RPM > 0 || SecData.Signal.KeyOn || PrevSecData.RPM > 0 || PrevSecData.Signal.KeyOn || IsgAccOff) {
    if(TripStartTime == 0 && RTC_LVal-RTC_BK_Addr->LastOnTime > 1) {
      SecData.Trip = 0;
      SecData.Fuel = 0;
      DailyData.Trip = 0;
      DailyData.Fuel = 0;
      DailyData.LapTime = 0;
      TripStartTime = RTC_LVal;
      SecData.Signal.StartTrip = 1;
      DailyData.TripCount++;
      SecData.TripCount = DailyData.TripCount;
    }
    else
      SecData.Signal.StartTrip = 0;
#ifdef EVENT_DATA_SAVE_ENABLE    
    if(Save_SecData_Cnt < 10) {
#else
    if(Save_SecData_Cnt < 300) {
#endif      
      memcpy((u8*)&Save_SecData_Buff[Save_SecData_Cnt++], (u8*)&SecData, sizeof(tsSecData));
    }
    
    if(!Save_SecData_Flag && !FotaEnable && !FotaRespFlag) {
        Save_SecData_Flag = 1;
    }
    RTC_BK_Addr->LastOnTime = RTC_LVal;
  }
  
  if(Setting.FW_Type == MDT_S1 || Setting.FW_Type == MDT_LG)
    MDT_DataUpdate();
  
  PrevSecData = SecData;
}


/*
********************************************************************************
* Description : Signal Proess
* Arguments   : none
* Return      : 
* Note        : RPM, Speed
******************************************************************************** 
*/
void Signal_Process(void)  
{ 
	sysinfo_t *s = &_sysinfo 	;

  s->pSpeed.nloop_count++;
  s->pRpm.nloop_count++;

  if(s->pSpeed.pulse_flag)
  {
    if( SPEED_SIG() ) 
      s->pSpeed.pulse_flag = 0;   //speed low 
    else
    {
      s->pSpeed.pulse_flag++;
      if(s->pSpeed.pulse_flag > SPEED_SIG_TIMING )
      {
        s->pSpeed.pulse_count++;
        s->pSpeed.pulse_flag = 0;
        if(s->pSpeed.pulse_count == 1)
        {
          s->pSpeed.nloop_count = 0;
          s->pSpeed.pulse_period = 0;
        }
        s->pSpeed.pulse_period = s->pSpeed.nloop_count;
      }
    }
  }
  
  if(s->pRpm.pulse_flag)
  {
    if( RPM_SIG() ) 
    {
      s->pRpm.pulse_flag++;
      if(s->pRpm.pulse_flag > RPM_SIG_TIMING )
      {
        s->pRpm.pulse_count++;
        s->pRpm.pulse_flag = 0;
        if(s->pRpm.pulse_count == 1)
        {
          s->pRpm.nloop_count = 0;
          s->pRpm.pulse_period = 0;
        }
        s->pRpm.pulse_period = s->pRpm.nloop_count;
      }
    }
    else
      s->pRpm.pulse_flag = 0;     //rpm low 
  }    
}


/*
********************************************************************************
* Description : Power_Off
* Arguments   : none
* Return      : 
* Note        : Key Off 
******************************************************************************** 
*/

void Power_Off(void)
{      
  IWDG_ReloadCounter();

  if(Power_Off_Mode) return;
  
  lcd_clear();
  lcd_printf(0,1,0, "Key Off");

  if(Start_Flag) {   
    if(USB_Detect_Flag) 
      lcd_printf(0,2,0, "Save Data");
    Parameter_Save();  
  }

  if(Setting.Can_Enable && !URLChangeFlag) { 
    CAN_Key_Off = 1;
  }
  
  USB_POWER_OFF();
  GPS_POWER_OFF();
  lcd_clear();
  lcd_backlight(0);
  SecData.Signal.KeyOn = 0;  
    
  if(Setting.FW_Type == EXT_MODEM) {
    Modem_KeyOffProcess();
  }
  Power_Off_Mode = 1;
}

/*
********************************************************************************
* Description : Power_Mode
* Arguments   : mode 0: off 1:on
* Return      : 
* Note        : Key On/Off  진입
******************************************************************************** 
*/
void Power_Mode(u8 mode)
{      

  IWDG_ReloadCounter();
  if(mode) {    
    lcd_backlight(Backligth_Sel*10);   // 1: ON 0 : OFF
    CDMA_POWER_ON();
    USB_POWER_ON();
    GPS_POWER_ON();
    Parameter_Init();   
    Disp_Init();
    CAN_Key_Off_Time = 0;
    CAN_Key_Off = 0;
    Save_SecData_Flag = 0;
  }
  else {
    if(!Power_Off_Flag) {
      Power_Off_Flag = 1;
#ifdef EVENT_DATA_SAVE_ENABLE
      if(Evt.Save_Flag) {
        lcd_clear();
        lcd_printf(0,1,0,"Event Saving");        
      }
      else if(USB_Detect_Flag) {
#else
      if(USB_Detect_Flag) {
#endif        
#ifdef DOWNLOAD_TRIP_DATA_TO_USB
        if(TripStartTime > 0) {
          lcd_clear();
          Menu_DisplayTitle(0,0,0,0); //다운로드
          DownloadDisplayInit();
          USB_DataDL_Flag = 1;
          USB_DataDL_LastTrip = 1;       
        }
#endif        
        USB_DrvVeh_Flag = 1;
        USB_DrvVeh_Mode = 1;
      }
      else {
        Power_Off();
      }
    }
  } 
}
    
/*
********************************************************************************
* Description : CheckPowerOff
* Arguments   : 
* Return      : 
* Note        : Power Off  Signal check
******************************************************************************** 
*/
u32 Dtg_On_Time;
u8 OffServerFlag = 0;
void CheckPowerOff(void)
{      
    if(!Power_Off_Mode) {          //Key-Off를 감시함
      if(KEY_OFF() && SecData.RPM < KEY_OFF_RPM_LOW_LIMIT) {
        Key_Off_Time++;
      }
      else {
        Key_Off_Time = 0;
        OffServerFlag = 0;
        TakeSendFlag = 0;
      }
      
      if((Setting.Can_Enable && CAN_Key_Off_Time > CAN_KEY_OFF_WAIT_LIMIT) || (KEY_OFF() && Key_Off_Time > DEFAULT_KEY_OFF_WAIT_LIMIT)) { //캔신호가 들어오지 않거나 키오프 신호후 3초뒤  꺼짐  
#ifdef TAKE_SEND_EVENT        
        if(Setting.ExtModemMode == LOOP_PROTOCOL) {          
          if(Key_Off_Time > BLE_SCAN_TAKE_WAIT_LIMIT) {
            if(ReceiveMediaFlag == BLESCANNER)
              BLE_take_send();          // 29분뒤 승차인원 전송
            else
              RFID_take_send();
          }
        } 
#endif        
        if(Key_Off_Time > BLE_SCAN_KEY_OFF_WAIT_LIMIT)
          TakeSendFlag = 1;
       
        if(((TxEvent_Index == 0 && Contact_Index == 0) || TakeSendFlag) && ChildAlarmEnable != 1) {
          Power_Mode(0); 
          Key_Off_Time = 0;
        }
        else if((TxEvent_Index != 0 || Contact_Index != 0) && !DataServerFlag) {
          if(!OffServerFlag && Setting.ExtModemMode == LOOP_PROTOCOL) {
            DataServerFlag = 1;
            DataAddrToSend = DataSaveAddr;
            OffServerFlag = 1;
          }
        }
        BLE_AddEnable = 0;
        BLE_SpeedCheckDist = 0;
      }
    }
    else {                            //PowerOff 실행 
      if(Setting.Can_Enable) {  //CAN입력시 
                                           // KEY_OFF() 일때 NVIC_SystemReset() 으로 리셋되어 KEY On 신호가 없어서 바로 꺼짐. 
                                           //POWER_ENABLE_OFF를 써도 되나 상시전원을 감안해서 리셋으로 함.
        Key_Off_Time++; 
        if(Display_Mode != 0) Display_Mode = 0;
        
        if(Setting.FW_Type == MDT_S1) {   // S1 KeyOff 에서 60초  넘으면 꺼짐
          if(KEY_OFF() && RTC_LVal > MDT_LastDataTime+MDT_S1_KEY_OFF_WAIT_LIMIT ) 
            System_PowerOff();    
        }
        else  if(Setting.FW_Type == MDT_LG) {  // LG KeyOff 에서 600초 넘으면 꺼짐
          if(KEY_OFF() && CAN_Key_Off_Time > MDT_LG_KEY_OFF_WAIT_LIMIT) 
            System_PowerOff(); 
        }
        else if(Setting.FW_Type == EXT_MODEM) {   // Ext 모뎀 데이터 전송 완료 및 모뎀 꺼짐 확인이 완료되거나  KeyOff 에서 180초  넘으면 꺼짐
          if(FotaFlag) Key_Off_Time = 0;
          if((DataServerFlag == 0 && ModemOffFlag == 1 && VdisBleServerFlag == 0) || (Key_Off_Time > KeyOffWaitTimeLimit) || (!ModemReadyFlag && Dtg_On_Time > 40000)) {
            if(KEY_OFF() || URLChangeFlag) {
              if(URLChangeFlag) 
                Modem_URLChangeReset();
              else
                System_PowerOff();  
            }
          }
        }
        else {
          if(KEY_OFF() && Key_Off_Time > DEFAULT_KEY_OFF_WAIT_LIMIT) 
            System_PowerOff(); 
        }
        
        if(KeyOKLongFlag || (!CAN_Key_Off && !KEY_OFF()) ) {    // 기다리는 중에 키온신호가 들어오거나 OK 버튼을 길게 누르면  다시 켜짐..
          NVIC_SystemReset(); 
        }
      }
      else {   //Line 입력시 
        if(Setting.FW_Type == MDT_S1) {
          if(RTC_LVal > MDT_LastDataTime+MDT_S1_KEY_OFF_WAIT_LIMIT)  // S1 KeyOff 에서 60초  넘으면 꺼짐
            System_PowerOff();
        }
        else if(Setting.FW_Type == MDT_LG) {  // LG KeyOff 에서 600초 넘으면 꺼짐
          if(MDT_Key_Off_Time > MDT_LG_KEY_OFF_WAIT_LIMIT) 
            System_PowerOff();
        }
        else if(Setting.FW_Type == EXT_MODEM) {  // Ext 모뎀 데이터 전송 완료 및 모뎀 꺼짐 확인이 완료되거나  KeyOff 에서 180초  넘으면 꺼짐
          if(!FotaFlag) Key_Off_Time++;
          if((DataServerFlag == 0  && ModemOffFlag == 1 && VdisBleServerFlag == 0) || (Key_Off_Time > KeyOffWaitTimeLimit) ||  (!ModemReadyFlag && Dtg_On_Time > 40000)) {
            if(URLChangeFlag) 
              Modem_URLChangeReset();
            else
              System_PowerOff();
          }
        }
        else
          System_PowerOff();         // 통신환경이 없을 경우 바로 꺼짐
        
        if(!KEY_OFF() && !URLChangeFlag) {    // 기다리는 중에 키온신호가 들어오면 다시 켜짐..
          NVIC_SystemReset();
        }
      }
   }
   Dtg_On_Time++;
}
/*
********************************************************************************
* Description : Task_DTG function
* Arguments   : none
* Return      : 
* Note        : RPM, Speed
******************************************************************************** 
*/
void Task_DTG(void const * argument)
{
  /* USER CODE BEGIN Task_DTG */
  int task_count = 0;
  
  I2C1_ACC_Init();  
  KeyOffWaitTimeLimit = 10*60*1000;    //milisec  10분 후에 무조건 꺼짐...
  PreSpeed = SecData.Speed[0];  
  /* Infinite loop */
  for(;;)
  {
    if((task_count%1000) == 0) {     //100msec
      I2C1_ACC_Read();
      SecData.BatVolt = BatteryVoltage();
      CheckGPSTimeSyc();
    }
    if(!Power_Off_Mode) {
      if(Start_Flag > 1) {
        
#ifdef EVENT_DATA_SAVE_ENABLE
        Event_Data_Save();
#endif
        Nand_Write_SaveData();
        Day_Change();
        if(IsgFuelSaveFlag == 1) {
          FRAMMultiWrite((u8*)&IsgSetting.IdleFuelRate, ISG_SETTING_PARM + offsetof(stIsgSetting, IdleFuelRate), sizeof(IsgSetting.IdleFuelRate));
          IsgFuelSaveFlag = 2;
        }
      }
#ifdef DIGITAL_CHECK_ENABLE
      CheckDigitalInput();
#else
      if(Setting.ExtModemVDIS == VDIS_BLE_MODE)      
        CheckSleepingChildAlarm();       
#endif      
    }
    CheckPowerOff();
    IWDG_ReloadCounter();     
    task_count++;
    if(task_count > 999) {
      WATCHDOG_TOGGLE();      
      task_count = 0;
    }
    osDelay(1);
  }
  /* USER CODE END Task_DTG */
}
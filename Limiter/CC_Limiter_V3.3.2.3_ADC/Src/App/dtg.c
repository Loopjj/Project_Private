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

#define GX_EVENT_VAL 127  //G sensor 이벤트 설정값
#define GY_EVENT_VAL  53  
    // x : +/-1.2 G  98.1*1.2*1.08 = 127
    // y : +/-0.5 G  98.1*0.5 *1.08= 53

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 PrevGSpeed;
unsigned long PrevLat, PrevLon;
time_t PrevSecTime; 
unsigned int GPSNotValidCount;

u8  GPS_Time_Sync;
u8  GPS_Time_NoSyncCnt;
u32 RTC_GPS_Val;
u16 DELAY_1,DELAY_2,DELAY_3;

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
  u32 i, len;
  u16 addr;
  char str[20], set_size = 0;
  
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
  
  GS_Offset[0] = 0; GS_Offset[1] = 0; GS_Offset[2] = GSENSOR_1G_VAL;  
  FRAMMultiWrite((u8 *)&GS_Offset, OFFSET_GX, sizeof(GS_Offset));    
  
  Contrast = CONTRAST_DEFAULT;
  FRAMByteWrite(Contrast, SET_CONTRAST);
  Main_Disp_Mode = 0;
  FRAMByteWrite(Main_Disp_Mode, MAIN_DISPLAY);  
  
  memset(Setting.Company_Name, 0x20, 12);
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.Company_Name, addr, sizeof(Setting.Company_Name));
  
  Setting.ExtModemMode = 0;
  Setting.ExtModemTelecom = 0;
  Setting.ExtModemModel = 0;
  
  addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode);
  FRAMMultiWrite((u8*)&Setting.ExtModemMode, addr,sizeof(Setting.ExtModemMode)+sizeof(Setting.ExtModemTelecom)+sizeof(Setting.ExtModemModel)); 
 
  Backligth_Sel = 3;
  FRAMByteWrite(Backligth_Sel, BACKLIGHT_SEL); 

  Setting.t_DelayP = PDELAYTIME;
  set_size += sizeof(Setting.t_DelayP);
  
  Setting.t_Buzzer = 1;
  set_size += sizeof(Setting.t_Buzzer);
  
  if(Setting.UserNumber == USER_CERACOMB) {
    Setting.P1L_Set = 350;
    Setting.P1H_Set = 400;
  }
  else {
    Setting.P1L_Set = 500;
    Setting.P1H_Set = 700;
  }
  set_size += sizeof(Setting.P1L_Set);
  set_size += sizeof(Setting.P1H_Set);
  
  Setting.Prangehigh = PRANGEHIGH;
  set_size += sizeof(Setting.Prangehigh);
  
  Setting.Prangelow = PRANGELOW;
  set_size += sizeof(Setting.Prangelow);
  
  Setting.P1CalHigh = PCalHigh;
  set_size += sizeof(Setting.P1CalHigh);
  
  Setting.P1CalLow = PCalLow;
  set_size += sizeof(Setting.P1CalLow);
  
  Setting.T1Offset = T_Offset;
  set_size+= sizeof(Setting.T1Offset);
  
  if(Setting.UserNumber == USER_CERACOMB) {
    Setting.TLL_Set = 10;
    Setting.TLH_Set = 15;
  }
  else if(Setting.UserNumber == USER_ILJIN35L || Setting.UserNumber == USER_ILJIN50L) {
    Setting.TLL_Set = 0;
    Setting.TLH_Set = 30;
  }
  else {
    Setting.TLL_Set = 0;
    Setting.TLH_Set = 15;
  }
  set_size+= sizeof(Setting.TLL_Set);
  set_size+= sizeof(Setting.TLH_Set);
  IWDG_ReloadCounter();
  
  Setting.t_DelayTL = TLDELAYTIME;
  set_size+= sizeof(Setting.t_DelayTL);
  
  Setting.LED = 0x1000;
  set_size+= sizeof(Setting.LED);
  
  Setting.IdlePress = 0;
  set_size+= sizeof(Setting.IdlePress);
  SecData.basePress = Setting.IdlePress;
  memset(P_BUF, 0, 12*2);
  
	Setting.PAlarm.B = 0;
  set_size+= sizeof(Setting.PAlarm);
  
  if(Setting.UserNumber >= USER_MAX) Setting.UserNumber = 0;
  set_size+= sizeof(Setting.UserNumber);
  
	Setting.CanBPS = 0;
  set_size+= sizeof(Setting.CanBPS);

	Setting.UreaLvlHi = 20;
  set_size+= sizeof(Setting.UreaLvlHi);
	Setting.UreaLvlLo = 10;
  set_size+= sizeof(Setting.UreaLvlLo);
	Setting.UreaLvlAL = 2;
  set_size+= sizeof(Setting.UreaLvlAL);

	Setting.UreaConHi = 40;
  set_size+= sizeof(Setting.UreaConHi);
	Setting.UreaConLo = 26;
  set_size+= sizeof(Setting.UreaConLo);
  
	Setting.UreaAlHr1 = 10;
  set_size+= sizeof(Setting.UreaAlHr1);
	Setting.UreaAlHr2 = 20;
  set_size+= sizeof(Setting.UreaAlHr2);
	Setting.UreaBuzInt = 10;
  set_size+= sizeof(Setting.UreaBuzInt);
	Setting.UreaAlEn = 1;
  set_size+= sizeof(Setting.UreaAlEn);
  
  addr = DRV_SETTING_PARM + offsetof(st_Setting, t_DelayP); 
  FRAMMultiWrite((u8 *)&Setting.t_DelayP, addr, set_size);
  IWDG_ReloadCounter();
  
  Modem_Parameter_Reset();
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
  
  BK_Sram_Addr = DataSaveAddr%NAND_PAGE_SIZE;  
  
  FRAMMultiRead((u8 *)&GS_Offset, OFFSET_GX, sizeof(GS_Offset));   //2.1.3  
  
  FRAMMultiRead((u8 *)&Setting, DRV_SETTING_PARM, sizeof(st_Setting));  
  Setting.Identity = 0xDDDDDDDD;

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
  
  for(i = 0; i < sizeof(Setting.Car_No); i++) { 
    if(Setting.Car_No[i] < 0x30 || Setting.Car_No[i] > 0x5A) 
      Setting.Car_No[i] = 0x20;    
  }  

  for(i = 0; i < sizeof(Setting.Company_Reg_No); i++) { 
    if(Setting.Company_Reg_No[i] < 0x30 || Setting.Company_Reg_No[i] > 0x5A) 
      Setting.Company_Reg_No[i] = 0x20;    
  }
  
  cnt = 0;
  size = sizeof(Setting.PinCode);
  for(i = 0; i < size; i++) { 
    if(Setting.PinCode[i] < 0x30 || Setting.PinCode[i] > 0x39) cnt++;
  }
  if(cnt) {
    BLE_PinCode_Generation();
    for(i = 0; i < size; i++) {
      Setting.PinCode[i] = BLE_PinCode[i] + 0x30;
    }
    addr = DRV_SETTING_PARM + (u8 *)&Setting.PinCode - (u8 *)&Setting;
    FRAMMultiWrite((u8 *)Setting.PinCode, addr, size);
  }
  
  if(Setting.t_DelayP > 9999) {
    Setting.t_DelayP = PDELAYTIME;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, t_DelayP);
    FRAMMultiWrite((u8 *)&Setting.t_DelayP, addr, sizeof(Setting.t_DelayP));
  }
  if(Setting.t_Buzzer > 10) {
    Setting.t_Buzzer = 1;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, t_Buzzer);
    FRAMMultiWrite((u8 *)&Setting.t_Buzzer, addr, sizeof(Setting.t_Buzzer));
  }
  if(Setting.P1L_Set > 1000) {
    if(Setting.UserNumber == USER_CERACOMB) Setting.P1L_Set = 350;
    else Setting.P1L_Set = 500;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, P1L_Set);
    FRAMMultiWrite((u8 *)&Setting.P1L_Set, addr, sizeof(Setting.P1L_Set));
  }
  if(Setting.P1H_Set > 1000) {
    if(Setting.UserNumber == USER_CERACOMB) Setting.P1H_Set = 400;
    else Setting.P1H_Set = 700;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, P1H_Set);
    FRAMMultiWrite((u8 *)&Setting.P1H_Set, addr, sizeof(Setting.P1H_Set));
  }
  if(Setting.Prangehigh > 1000) {
    Setting.Prangehigh = PRANGEHIGH;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, Prangehigh);
    FRAMMultiWrite((u8 *)&Setting.Prangehigh, addr, sizeof(Setting.Prangehigh));
  }
  IWDG_ReloadCounter();
  
  if(Setting.Prangelow > 1000) {
    Setting.Prangelow = PRANGELOW;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, Prangelow);
    FRAMMultiWrite((u8 *)&Setting.Prangelow, addr, sizeof(Setting.Prangelow));
  }
  if(Setting.P1CalHigh > 4095) {
    Setting.P1CalHigh = PCalHigh;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, P1CalHigh);
    FRAMMultiWrite((u8 *)&Setting.P1CalHigh, addr, sizeof(Setting.P1CalHigh));
  }
  if(Setting.P1CalLow > 4095) {
    Setting.P1CalLow = PCalLow;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, P1CalLow);
    FRAMMultiWrite((u8 *)&Setting.P1CalLow, addr, sizeof(Setting.P1CalLow));
  }
  if(Setting.T1Offset > 1000) {
    Setting.T1Offset = T_Offset;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, T1Offset);
    FRAMMultiWrite((u8 *)&Setting.T1Offset, addr, sizeof(Setting.T1Offset));
  }
  if(Setting.TLL_Set > 100) {
    if(Setting.UserNumber == USER_CERACOMB) Setting.TLL_Set = 10;
    else if(Setting.UserNumber == USER_ILJIN35L || Setting.UserNumber == USER_ILJIN50L) Setting.TLL_Set = 0;
    else Setting.TLL_Set = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, TLL_Set);
    FRAMMultiWrite((u8 *)&Setting.TLL_Set, addr, sizeof(Setting.TLL_Set));
  }
  IWDG_ReloadCounter();
  
  if(Setting.TLH_Set > 100) {
    if(Setting.UserNumber == USER_ILJIN35L || Setting.UserNumber == USER_ILJIN50L) Setting.TLH_Set = 30;
    else Setting.TLH_Set = 15;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, TLH_Set);
    FRAMMultiWrite((u8 *)&Setting.TLH_Set, addr, sizeof(Setting.TLH_Set));
  }
  if(Setting.t_DelayTL > 9999) {
    Setting.t_DelayTL = TLDELAYTIME;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, t_DelayTL);
    FRAMMultiWrite((u8 *)&Setting.t_DelayTL, addr, sizeof(Setting.t_DelayTL));
  }
  if(Setting.Prangehigh > 1000) {
    Setting.Prangehigh = PRANGEHIGH;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, Prangehigh);
    FRAMMultiWrite((u8 *)&Setting.Prangehigh, addr, sizeof(Setting.Prangehigh));
  }
  if(Setting.Prangelow > 1000) {
    Setting.Prangelow = PRANGELOW;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, Prangelow);
    FRAMMultiWrite((u8 *)&Setting.Prangelow, addr, sizeof(Setting.Prangelow));
  }
  if(Setting.PAlarm.B > 0x0F) {
    Setting.PAlarm.B = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, PAlarm);
    FRAMMultiWrite((u8 *)&Setting.PAlarm.B, addr, sizeof(Setting.PAlarm));
  }
  IWDG_ReloadCounter();
  
  if(Setting.IdlePress > 1000) {
    Setting.IdlePress = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, IdlePress);
    FRAMMultiWrite((u8 *)&Setting.IdlePress, addr, sizeof(Setting.IdlePress));
  }
  for(i=0; i<12; i++) P_BUF[i] = Setting.IdlePress;
  SecData.basePress = Setting.IdlePress;
  if(Setting.UserNumber > USER_MAX) {
    Setting.UserNumber = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UserNumber);
    FRAMMultiWrite((u8 *)&Setting.UserNumber, addr, sizeof(Setting.UserNumber));
  }
  if(Setting.CanBPS > 0x11) {
    Setting.CanBPS = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, CanBPS);
    FRAMMultiWrite((u8 *)&Setting.CanBPS, addr, sizeof(Setting.CanBPS));
  }
  
  if(Setting.UreaLvlHi  == 0 || Setting.UreaLvlHi > 99) {
    Setting.UreaLvlHi = 20;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaLvlHi);
    FRAMMultiWrite((u8 *)&Setting.UreaLvlHi, addr, sizeof(Setting.UreaLvlHi));
  }
  if(Setting.UreaLvlLo  == 0 || Setting.UreaLvlLo > 99) {
    Setting.UreaLvlLo = 10;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaLvlLo);
    FRAMMultiWrite((u8 *)&Setting.UreaLvlLo, addr, sizeof(Setting.UreaLvlLo));
  }  
  if(Setting.UreaLvlAL  == 0 || Setting.UreaLvlAL > 99) {
    Setting.UreaLvlAL = 2;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaLvlAL);
    FRAMMultiWrite((u8 *)&Setting.UreaLvlAL, addr, sizeof(Setting.UreaLvlAL));
  }  
  if(Setting.UreaConHi  == 0 || Setting.UreaConHi > 99) {
    Setting.UreaConHi = 40;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaConHi);
    FRAMMultiWrite((u8 *)&Setting.UreaConHi, addr, sizeof(Setting.UreaConHi));
  }  
  if(Setting.UreaConLo  == 0 || Setting.UreaConLo > 99) {
    Setting.UreaConLo = 26;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaAlHr1);
    FRAMMultiWrite((u8 *)&Setting.UreaAlHr1, addr, sizeof(Setting.UreaAlHr1));
  }  
  if(Setting.UreaAlHr1  == 0 || Setting.UreaAlHr1 > 99) {
    Setting.UreaAlHr1 = 10;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaConLo);
    FRAMMultiWrite((u8 *)&Setting.UreaConLo, addr, sizeof(Setting.UreaConLo));
  }  
  if(Setting.UreaAlHr2  == 0 || Setting.UreaAlHr2 > 99) {
    Setting.UreaAlHr2 = 20;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaAlHr2);
    FRAMMultiWrite((u8 *)&Setting.UreaAlHr2, addr, sizeof(Setting.UreaAlHr2));
  }  
  if(Setting.UreaBuzInt  == 0 || Setting.UreaBuzInt > 99) {
    Setting.UreaBuzInt = 10;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UreaBuzInt);
    FRAMMultiWrite((u8 *)&Setting.UreaBuzInt, addr, sizeof(Setting.UreaBuzInt));
  }  
  
  IWDG_ReloadCounter();
  
  RTC_GetDateTime();

  GPS_Fault = 1;  

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

  FRAMMultiRead((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));
  DailyData_Change();
  SecData.LT = DailyData.Lat;
  SecData.LN = DailyData.Lon;
  SecData.CO = DailyData.Azi;  
  
  FRAMMultiRead((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));

  UreaAlarm = RTC_BK_Addr->UreaAlarm;
  PrevUreaAlarm = UreaAlarm;
  UreaAlarmLapTime = RTC_BK_Addr->UreaAlarmLapTime;
  UreaAccControl = RTC_BK_Addr->UreaAccControl;
  if(UreaAccControl.Level == 2 || UreaAccControl.Quality == 2 || UreaAccControl.Status == 2 || UreaAccControl.SysErr == 2) {
    DO2_SET();
    DO1_RESET();
    SecData.UreaAccFlag = 2;
  }
  else if(UreaAccControl.Level == 1 || UreaAccControl.Quality == 1 || UreaAccControl.Status == 1 || UreaAccControl.SysErr == 1) {
    DO1_SET();
    DO2_RESET();
    SecData.UreaAccFlag = 1;
  }
  else {
    DO1_RESET();
    DO2_RESET();
    SecData.UreaAccFlag = 0;
  }
}

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
        //UpdateTimeToGPSTime();   //Gps 시간과 3초이상 차이날경우 바로 업데이트 한다. 
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

  FRAMByteWrite(Main_Disp_Mode, MAIN_DISPLAY);
  FRAMByteWrite(Backligth_Sel, BACKLIGHT_SEL); //1.4.0.0

  FRAMMultiWrite((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));
  //UpdateTimeToGPSTime();
}

u8 UreaLevelAlarmEnableCnt[4];
u8 UreaQualAlarmEnableCnt[2];
u8 UreaStatusAlarmEnableCnt[2];
u8 UreaSysErrAlarmEnableCnt[2];

stUreaAlarm UreaAlarmTest; //5

void CheckUreaAlarm(void)
{
  u8 i;
  
  if(DailyData.LapTime > 180) {
    if(SecData.UreaLevel < Setting.UreaLvlAL) {
      if(PrevUreaAlarm.Level != UreaAlarm.Level)
        UreaLevelAlarmEnableCnt[3] = 0;
      if(UreaAlarm.Level < 3) 
        UreaLevelAlarmEnableCnt[3]++;
      if(UreaAlarm.Level < 3 && UreaLevelAlarmEnableCnt[3] > 60) {   //60-> 240
        UreaLevelAlarmEnableCnt[3] = 0;
        UreaAlarm.Level = 3;
      }
      UreaLevelAlarmEnableCnt[2] = 0;
      DELAY_1 = 0;
    }
    else if(SecData.UreaLevel < Setting.UreaLvlLo) {
      if(PrevUreaAlarm.Level != UreaAlarm.Level)
        UreaLevelAlarmEnableCnt[2] = 0;
      if(UreaAlarm.Level < 2) 
        UreaLevelAlarmEnableCnt[2]++;
      if(UreaAlarm.Level < 2 && UreaLevelAlarmEnableCnt[2] > 60) { //60-> 240 
        UreaLevelAlarmEnableCnt[2] = 0;
        UreaAlarm.Level = 2;
      }
      UreaLevelAlarmEnableCnt[1] = 0;
      UreaLevelAlarmEnableCnt[3] = 0;
      DELAY_1 = 0;
    }
    else {
      if(SecData.UreaLevel < Setting.UreaLvlHi) {
        if(PrevUreaAlarm.Level != UreaAlarm.Level)
          UreaLevelAlarmEnableCnt[1] = 0;
        if(UreaAlarm.Level != 1) 
          UreaLevelAlarmEnableCnt[1]++;
        if(UreaAlarm.Level != 1 && UreaLevelAlarmEnableCnt[1] > 60) { //60-> 240
          UreaLevelAlarmEnableCnt[1] = 0;
          UreaAlarm.Level = 1;
        }
        UreaLevelAlarmEnableCnt[0] = 0;
        UreaLevelAlarmEnableCnt[2] = 0;
        DELAY_1 = 0;
      }
      else {
        if(UreaAlarm.Level == 2 || UreaAlarm.Level == 3) {
          DELAY_1++;
          if(DELAY_1 >= 300)  {
            UreaAlarm.Level = 0;
            DELAY_1 = 0;
          }
        }
        else {
          if(PrevUreaAlarm.Level != UreaAlarm.Level)
            UreaLevelAlarmEnableCnt[0] = 0;
          if(UreaAlarm.Level > 0 && SecData.UreaLevel > Setting.UreaLvlHi)
            UreaLevelAlarmEnableCnt[0]++;
          else if(SecData.UreaLevel > Setting.UreaLvlHi) {
            for(i = 0; i < 4; i++)
              if(UreaLevelAlarmEnableCnt[i]) UreaLevelAlarmEnableCnt[i] = 0;
          }
          if(UreaLevelAlarmEnableCnt[0] > 3) { //3-> 240
            UreaAlarm.Level = 0;
            UreaLevelAlarmEnableCnt[0] = 0;
          }
        }
      }
    }
    
    if(CAN_Enable.Concentration) {
      if(SecData.Concentration < Setting.UreaConLo || SecData.Concentration > Setting.UreaConHi)  {
  //       SecData.Chk.Concentration  = 1; //Urea Quality CheckCode = 1
        if(PrevUreaAlarm.Quality != UreaAlarm.Quality)
          UreaQualAlarmEnableCnt[1] = 0;
        if(UreaAlarm.Quality < 1)
          UreaQualAlarmEnableCnt[1]++;
        if(UreaAlarm.Quality < 1 && UreaQualAlarmEnableCnt[1] > 60) {
          UreaQualAlarmEnableCnt[1] = 0;
          UreaAlarm.Quality = 1;
        }
      }
      else {
  //         SecData.Chk.Concentration  = 0; //Urea Quality CheckCode = 0 
        if(PrevUreaAlarm.Quality != UreaAlarm.Quality)
          UreaQualAlarmEnableCnt[0] = 0;
        if(UreaAlarm.Quality > 0)
          UreaQualAlarmEnableCnt[0]++;
        if(UreaQualAlarmEnableCnt[0] > 3) {
          UreaAlarm.Quality = 0;
          UreaQualAlarmEnableCnt[0] = 0;
        }
      }
    }

    if(!CAN_Enable.Concentration || !CAN_Enable.NOxIn || !CAN_Enable.NOxOut) {
      if(PrevUreaAlarm.Status != UreaAlarm.Status)
        UreaStatusAlarmEnableCnt[1] = 0;
      if(UreaAlarm.Status < 1)
        UreaStatusAlarmEnableCnt[1]++;
      if(UreaAlarm.Status < 1 && UreaStatusAlarmEnableCnt[1] > 60) {
        UreaStatusAlarmEnableCnt[1] = 0;
        UreaAlarm.Status = 1;
      }
    }
    else {
      if(PrevUreaAlarm.Status != UreaAlarm.Status)
        UreaStatusAlarmEnableCnt[0] = 0;
      if(UreaAlarm.Status > 0)
        UreaStatusAlarmEnableCnt[0]++;
      if(UreaStatusAlarmEnableCnt[0] > 3) {
        UreaAlarm.Status = 0;
        UreaStatusAlarmEnableCnt[0] = 0;
      }
    }
  
//    if(UreaSysErrFlag) {
    if(Sig.Noxact) {
      if(NOxValidFlag[0] || NOxValidFlag[1]) {
        if(PrevUreaAlarm.SysErr != UreaAlarm.SysErr)
          UreaSysErrAlarmEnableCnt[1] = 0;
        if(UreaAlarm.SysErr < 1)
          UreaSysErrAlarmEnableCnt[1]++;
        if(UreaAlarm.SysErr < 1 && UreaSysErrAlarmEnableCnt[1] > 60) {
          UreaSysErrAlarmEnableCnt[1] = 0;
          UreaAlarm.SysErr = 1;
        }
      }
      else {
        if(PrevUreaAlarm.SysErr != UreaAlarm.SysErr)
          UreaSysErrAlarmEnableCnt[0] = 0;
        if(UreaAlarm.SysErr > 0)
          UreaSysErrAlarmEnableCnt[0]++;
        if(UreaSysErrAlarmEnableCnt[0] > 3) {
          UreaAlarm.SysErr = 0;
          UreaSysErrAlarmEnableCnt[0] = 0;
        }
      }
    }
  }

  if(PrevUreaAlarm.Level != UreaAlarm.Level || PrevUreaAlarm.Quality != UreaAlarm.Quality || PrevUreaAlarm.Status != UreaAlarm.Status || PrevUreaAlarm.SysErr != UreaAlarm.SysErr) 
    Display_Flag = 1;
  PrevUreaAlarm = UreaAlarm;
  RTC_BK_Addr->UreaAlarm = UreaAlarm;
  
  if(UreaAlarm.Level > 1 || UreaAlarm.Quality == 1 || UreaAlarm.Status == 1 || UreaAlarm.SysErr == 1) {
    if(UreaAlarm.Quality == 1 || UreaAlarm.Status == 1 || UreaAlarm.SysErr == 1) UreaAlarmLapTime++;
    
#ifdef TIME_HOUR
    if(UreaAlarmLapTime > Setting.UreaAlHr2*3600) //20hr
#else
    if(UreaAlarmLapTime > Setting.UreaAlHr2*60) //20hr
#endif
    {
//      if(UreaAlarm.Level == 2)    UreaAccControl.Level = 2;
      if(UreaAlarm.Quality == 1)  UreaAccControl.Quality = 2;
      if(UreaAlarm.Status == 1)   UreaAccControl.Status = 2;
      if(UreaAlarm.SysErr == 1)   UreaAccControl.SysErr = 2;
    }
#ifdef TIME_HOUR
    else if(UreaAlarmLapTime > Setting.UreaAlHr1*3600)  //10hr
#else
    else if(UreaAlarmLapTime > Setting.UreaAlHr1*60)  //10hr
#endif
    {
//      if(UreaAlarm.Level == 2)    UreaAccControl.Level = 2;
      if(UreaAlarm.Quality == 1)  UreaAccControl.Quality = 1;
      if(UreaAlarm.Status == 1)   UreaAccControl.Status = 1;
      if(UreaAlarm.SysErr == 1)   UreaAccControl.SysErr = 1;
    }
    else {
//      if(UreaAlarm.Level == 2)    UreaAccControl.Level = 1;
      if(UreaAlarm.Quality == 1)  UreaAccControl.Quality = 0;
      if(UreaAlarm.Status == 1)   UreaAccControl.Status = 0;
      if(UreaAlarm.SysErr == 1)   UreaAccControl.SysErr = 0;
    }
    if(UreaAlarm.Level == 2) UreaAccControl.Level = 1;
    else if(UreaAlarm.Level == 3) UreaAccControl.Level = 2;
  }
  else {
    UreaAlarmLapTime = 0;
    UreaAccControl.Level = 0;
    UreaAccControl.Quality = 0;
    UreaAccControl.Status = 0;
    UreaAccControl.SysErr = 0;
    DELAY_1 = 0;
    DELAY_2 = 0;
    DELAY_3 = 0;
  }
  
  if(SecData.UreaAccFlag) {
    if(!UreaAccControl.Level && !UreaAccControl.Quality && !UreaAccControl.Status && !UreaAccControl.SysErr) {
      DO1_RESET();
      DO2_RESET();
      SecData.UreaAccFlag = 0;
    }      
  }
  RTC_BK_Addr->UreaAccControl = UreaAccControl;
  RTC_BK_Addr->UreaAlarmLapTime = UreaAlarmLapTime;
  
  if((UreaAlarmLapTime%2) && Display_Mode == 0)
    lcd_backlight(0);
  else
    lcd_backlight(Backligth_Sel*10);
}

void UreaAccTest(void)
{
  UreaAlarm.Quality = 0;
  UreaAlarm.Status = 0;
  UreaAlarm.SysErr = 0;
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
  else {
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
    else if(UreaTestFlag == 2) {
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
    }
  }
}

void CheckUreaBuzzer(void)
{
  if(UreaAccControl.Level == 2 || UreaAccControl.Quality == 2 || UreaAccControl.Status == 2 || UreaAccControl.SysErr == 2) {
    if(UreaBuzzerFlag < 4) UreaBuzzerTime = 0;
    UreaBuzzerFlag = 4;
  }
  else if(UreaAccControl.Level == 1 || UreaAccControl.Quality == 1 || UreaAccControl.Status == 1 || UreaAccControl.SysErr == 1) {
    if(UreaBuzzerFlag < 3) UreaBuzzerTime = 0;
    UreaBuzzerFlag = 3;
  }
  else if(UreaAlarm.Level == 2 || UreaAlarm.Quality == 1 || UreaAlarm.Status == 1 || UreaAlarm.SysErr == 1) {
    if(UreaBuzzerFlag < 2) UreaBuzzerTime = 0;
    UreaBuzzerFlag = 2;
  }
  else if(UreaAlarm.Level == 1) {
    UreaBuzzerFlag = 1;
  }
  else {
    UreaBuzzerFlag = 0;
    UreaBuzzerTime = 0;
    BuzzerOnFlag = 0;
  }
  
  if(UreaBuzzerFlag) {
    if(UreaBuzzerTime == 0) {
      BuzzerOnFlag = 1;
    }
    UreaBuzzerTime++;
    if(UreaBuzzerTime >= Setting.UreaBuzInt*60) 
      UreaBuzzerTime = 0;
  }
}

void CheckBuzzerOn(void)
{
  if(Power_Off_Mode) {
    return;
  }
  if(BuzzerOnFlag) {
    if((BuzzerTimeCnt%2))
      BUZZER_OFF();
    else {
      BUZZER_ON();
      BuzzerOnCnt++;
      if(BuzzerOnCnt >= UreaBuzzerFlag) {
        BuzzerOnCnt = 0;
        BuzzerOnFlag = 0;
        BuzzerTimeCnt = 0;
      }
    }
    BuzzerTimeCnt++;
  }
  else {
    BUZZER_OFF();
    BuzzerOnCnt = 0;
    BuzzerTimeCnt = 0;
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

void DrvDataUpdate(void)
{ 
  unsigned long lat, lon;

  SecData.TM = RTC_LVal;
  
  if(GPS_Valid) {
    GPS_Speed_Val = (u32)(rmc.speed  * 1.852);
    // GPS 데이터 
    lat = (unsigned long)(gga.latitude  * 1000000.0);
    lon = (unsigned long)(gga.longitude * 1000000.0);
    if((SecData.TM - PrevSecTime) < 5) {
      if(abs(PrevLat-lat) < 50000) {
        SecData.LT = lat;
      }
      else SecData.LT = 0;
      if(abs(PrevLon-lon) < 50000) {
        SecData.LN = lon;
      }
      else SecData.LN = 0;
    }
    else {
      SecData.LT = 0;
      SecData.LN = 0;
    }
 
    SecData.CO  = (unsigned short)rmc.course;
    PrevSecTime = SecData.TM;
    PrevLat = lat; 
    PrevLon = lon;
    GPSNotValidCount = 0;
    SecData.SysCode.Gps = 0;
  }
  else {
    if(GPSNotValidCount < 60)   GPSNotValidCount++;
    if(KEY_OFF()  || GPSNotValidCount >= 60) {
      SecData.LT = 0;
      SecData.LN = 0;
//      SecData.AT = 0;
      GPS_Speed_Val = 0;
    }
    SecData.SysCode.Gps = 1;
  }
  
  if(rmc.warn =='A') SecData.SysCode.Warn = 1;
  else SecData.SysCode.Warn = 0;
//  SecData.GPS_hdop = (INT8U) gga.hdop * 10;
  
//  SecData.Speed = GPS_Speed_Val;

  
  if(SecData.LT == 0 || SecData.LN== 0) 
    GPS_Fault = 1;
  
  if(GPS_Fault) {
    SecData.LT = DailyData.Lat;
    SecData.LN = DailyData.Lon;
    SecData.CO = DailyData.Azi;
  }
  
  TmpSecData.basePress = Pedal_ADC[0];
  TmpSecData.DpfTemp = Pedal_ADC[1];
  if(Save_SecData_Cnt < 300) {
    memcpy((u8*)&Save_SecData_Buff[Save_SecData_Cnt++], (u8*)&SecData, sizeof(tsSecData));
  }
  
  if(!Save_SecData_Flag && !FotaEnable && !FotaRespFlag) {
      Save_SecData_Flag = 1;
  }
  
  DailyData.Lat = SecData.LT;
  DailyData.Lon = SecData.LN;
  DailyData.Azi = SecData.CO;  
  
  DailyData.LapTime++;
  DailyData.DLapTime++;
  
  if(TripStartTime == 0 && RTC_LVal-RTC_BK_Addr->LastOnTime > 1) {
    TripStartTime = RTC_LVal;
    DailyData.LapTime = 0;
    DailyData.TripCount++;
  }
  
  RTC_BK_Addr->LastOnTime = RTC_LVal;
  
  PrevSecData = SecData;
  
  if(!AccTestFlag) CheckUreaAlarm();
  else UreaAccTest();
  CheckUreaBuzzer();
  
  
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
    DailyData.LapTime = 0;
    DailyData.DLapTime = 0;
    DailyData.TripCount = 1;
    
    FRAMMultiWrite((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData));
  }
  
  Nand_Header_Flag = 1;
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
//	sysinfo_t *s = &_sysinfo 	;
//
//  s->pSpeed.nloop_count++;
//  s->pRpm.nloop_count++;
//
//  if(s->pSpeed.pulse_flag)
//  {
//    if( SPEED_SIG() ) 
//      s->pSpeed.pulse_flag = 0;   //speed low 
//    else
//    {
//      s->pSpeed.pulse_flag++;
//      if(s->pSpeed.pulse_flag > SPEED_SIG_TIMING )
//      {
//        s->pSpeed.pulse_count++;
//        s->pSpeed.pulse_flag = 0;
//        if(s->pSpeed.pulse_count == 1)
//        {
//          s->pSpeed.nloop_count = 0;
//          s->pSpeed.pulse_period = 0;
//        }
//        s->pSpeed.pulse_period = s->pSpeed.nloop_count;
//      }
//    }
//  }
//  
//  if(s->pRpm.pulse_flag)
//  {
//    if( RPM_SIG() ) 
//    {
//      s->pRpm.pulse_flag++;
//      if(s->pRpm.pulse_flag > RPM_SIG_TIMING )
//      {
//        s->pRpm.pulse_count++;
//        s->pRpm.pulse_flag = 0;
//        if(s->pRpm.pulse_count == 1)
//        {
//          s->pRpm.nloop_count = 0;
//          s->pRpm.pulse_period = 0;
//        }
//        s->pRpm.pulse_period = s->pRpm.nloop_count;
//      }
//    }
//    else
//      s->pRpm.pulse_flag = 0;     //rpm low 
//  }    
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

  USB_POWER_OFF();
#ifndef USE_GPS_TO_BUZZER    
  GPS_POWER_OFF();
#endif  
  lcd_clear();
  lcd_backlight(0);
    
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
#ifndef USE_GPS_TO_BUZZER    
    GPS_POWER_ON();
#endif    
    Parameter_Init();   
    Disp_Init();
    Save_SecData_Flag = 0;
  }
  else {
    if(!Power_Off_Flag) {
      Power_Off_Flag = 1;
//      if(USB_Detect_Flag) {
//#ifdef DOWNLOAD_TRIP_DATA_TO_USB
//        if(TripStartTime > 0) {
//          lcd_clear();
//          Menu_DisplayTitle(0,0,0,0); //다운로드
//          DownloadDisplayInit();
//          USB_DataDL_Flag = 1;
//          USB_DataDL_LastTrip = 1;       
//        }
//#endif        
//      }
//      else {
        Power_Off();
//      }
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
void CheckPowerOff(void)
{      
    if(!Power_Off_Mode) {          //Key-Off를 감시함
      if(KEY_OFF()) {
        Key_Off_Time++;
        if(Key_Off_Time > DEFAULT_KEY_OFF_WAIT_LIMIT) { //키오프 신호후 3초뒤  꺼짐  
          Power_Mode(0); 
          Key_Off_Time = 0;
        }
      }
      else {
        Key_Off_Time = 0;
      }
    }
    else {                            //PowerOff 실행 
      if(Setting.FW_Type == EXT_MODEM) {  // Ext 모뎀 데이터 전송 완료 및 모뎀 꺼짐 확인이 완료되거나  KeyOff 에서 180초  넘으면 꺼짐
        if(!FotaFlag) Key_Off_Time++;
        if((DataServerFlag == 0  && ModemOffFlag == 1) || (Key_Off_Time > KeyOffWaitTimeLimit) ||  (!ModemReadyFlag && Dtg_On_Time > 40000)) {
            System_PowerOff();
        }
      }
      else
        System_PowerOff();         // 통신환경이 없을 경우 바로 꺼짐
      
      if(!KEY_OFF()) {               // 기다리는 중에 키온신호가 들어오면 다시 켜짐..
        NVIC_SystemReset();
      }
   }
   Dtg_On_Time++;
}

/*
********************************************************************************
* Description : SaveData
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void FileSaveData(void)
{

 	if(open_append(&fp, F_Name)==FR_OK) {
    f_printf(&fp, "\n");
	                   // Date		
		f_printf(&fp, "%01d%01d.%01d%01d.%01d%01d\t",    
                 	  RTC_Val.tm_year/10, RTC_Val.tm_year%10, RTC_Val.tm_mon/10, RTC_Val.tm_mon%10, RTC_Val.tm_mday/10, RTC_Val.tm_mday%10);
                   // Time
		f_printf(&fp, "%01d%01d:%01d%01d:%01d%01d\t",    
                 		RTC_Val.tm_hour/10, RTC_Val.tm_hour%10, RTC_Val.tm_min/10, RTC_Val.tm_min%10, RTC_Val.tm_sec/10, RTC_Val.tm_sec%10);
   
    f_printf(&fp, "%01d\t%01d\t%01d\t%01d\t%02d.%02d\t%01d\t%01d\t%01d\t%01d.%01d\t%02d\t%01d.%03d\t%01d.%03d\t%01d\t%01d\t%01d\t%01d",
                    SecData.NOxIn, SecData.NOxOut,SecData.TempIn, SecData.TempOut, SecData.DosingRate/100, SecData.DosingRate%100, SecData.UreaLevel,SecData.MAF,  
                    SecData.Concentration, SecData.Vbat/10, SecData.Vbat%10, SecData.UreaAccFlag, Pedal_ADC[0]/1000, Pedal_ADC[0]%1000, Pedal_ADC[1]/1000, Pedal_ADC[1]%1000, SecData.Err.B, SecData.Chk.W, UreaAlarmLapTime, SecData.Speed);

		f_close(&fp);
  	SaveOkFlag = 1;
	}
  else {
  	SaveOkFlag = 0;  
  }
}
/*
********************************************************************************
* Description : FileCreate
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
unsigned char FileCreate(void)
{   
  memset((INT8U *)F_Name, 0x00, 12);
    
	F_Name[0] = (RTC_Val.tm_year/10) + 0x30;	// convert to ascii code
	F_Name[1] = (RTC_Val.tm_year%10) + 0x30;
	F_Name[2] = (RTC_Val.tm_mon/10) + 0x30;	// convert to ascii code
	F_Name[3] = (RTC_Val.tm_mon%10) + 0x30;
	F_Name[4] = (RTC_Val.tm_mday/10) + 0x30;	// convert to ascii code
	F_Name[5] = (RTC_Val.tm_mday%10) + 0x30;
	F_Name[6] = '.';	
	F_Name[7] = 'X';
	F_Name[8] = 'L';	
	F_Name[9] = 'S';
 
  
  if(f_open(&fp,F_Name,FA_OPEN_EXISTING) != FR_OK) { //file MUST exist for reading
   	if(f_open(&fp,F_Name,FA_CREATE_ALWAYS| FA_WRITE) == FR_OK)
 	  {
      f_printf(&fp, "Date\tTime\t");
      f_printf(&fp, "NOx-up\tNOx-dn\tT-up\tT4\tDosingRate\tUreaLevel\t");
      f_printf(&fp, "MAF\tUreaQuality\tVbat\tLimit Step\tInV\tOutV\tError\tCheck\tLapTime\tSpeed\t");
      f_printf(&fp, "Limiter V%01d.%01d.%01d",MAJOR,MINOR,RELEASE);
		  SaveOkFlag = 1;
	    f_close(&fp);	      	
		  return 1;
	  }
	  else{
 	    SaveOkFlag = 0;    	
	    return 0;
	  }
  }
  else {
	  f_close(&fp);
  	FileSaveData();
  	return 1; 
  }       		
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
  u8  size;
  u16 addr;
  /* USER CODE BEGIN Task_DTG */
  int task_count = 0;
  
  I2C1_ACC_Init();  
  KeyOffWaitTimeLimit = 10*60*1000;    //milisec  10분 후에 무조건 꺼짐...
  AccTestFlag = 0;
//  PreSpeed = SecData.Speed[0];  
  /* Infinite loop */
  for(;;)
  {
    if((task_count%100) == 0) {     //100msec 
      I2C1_ACC_Read();
      Send_SecData(); 
      Send_LimitStep();
      if(F_Parameter) {
       addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaLvlHi - (u8 *)&Setting;
       size = sizeof(Setting.UreaLvlHi) + sizeof(Setting.UreaLvlLo) + sizeof(Setting.UreaLvlAL)
         + sizeof(Setting.UreaConHi) + sizeof(Setting.UreaConLo) + sizeof(Setting.UreaAlHr1) + sizeof(Setting.UreaAlHr2) + sizeof(Setting.UreaBuzInt); 
       FRAMMultiWrite((u8 *)&Setting.UreaLvlHi, addr, size);
       F_Parameter = 0;
      }
    }
    if((task_count%1000) == 0) {     //1000msec
      if(!CAN_Enable.Vbat)
        SecData.Vbat = BatteryVoltage(); 
        //Send_LimitStep();
      CheckGPSTimeSyc();
      CheckBuzzerOn();
      if(LimitDelay < RTC_DELAY) LimitDelay++;
    }
    if(!Power_Off_Mode) {
      if(Start_Flag > 1) {
        Nand_Write_SaveData();
        Day_Change();
      }
    }
    if(USB_Detect_Flag && !USB_DataDL_Flag && LimitDelay >= RTC_DELAY) { //5초 후 저장시작 
      if(USB_IntFlag) {
        if(PreDay != RTC_Val.tm_mday) {
          FileCreate();
          PreDay = RTC_Val.tm_mday;          
        }
        FileSaveData(); 
        USB_IntFlag = 0;       
      }
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
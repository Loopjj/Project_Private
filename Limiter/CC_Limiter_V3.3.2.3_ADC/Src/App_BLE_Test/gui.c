/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : gui.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : gui process 
*
*********************************************************************************************************
*/
#define GUI_GLOBALS
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "ui_bmp.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void DAC_Write(u8 mode, u8 data) 
{
}

#define LONG_KEY_LIMIT      5
#define KEY_OK              0x40
#define KEY_UP              0x20
#define KEY_DOWN            0x10
#define KEY_CANCEL          0x08
#define KEY_LONG            0X80
#define KEY_MASK            0x78   //~(SW_1_Pin|SW_2_Pin|SW_3_Pin|SW_4_Pin)
#define KEY_PORT            GPIOG
#define DISPLAY_FREQ        1000 //1SEC


const u8 ACode[37] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 
                                 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 
                                 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x20};

const u8 rtc_days_in_month[] = {
  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

u8 Menu_Disp_Mode, Info_Disp_Mode, Setting_Disp_Mode;
unsigned int  Gui_Count; 


u8 GuiTmpData[256];

int Btn_UpDn_Cnt, Btn_UpDn_Old, Btn_UpDn_Sel;
u8  Btn_Right_Cnt;
u8  *Bitmap_MenuMode_Title[20], *Bitmap_SettingMode_Title[22]; 
u8  Tmp_Buff[3][20];
u8  Disp_GSen_Val;
u8  Disp_Alert;

u8 Key_Val;
u8 Key_ESC_Flag;
u8 Key_Flag;

u8 Old_Key = 0, Long_Key_Cnt = 0, Long_Key_Flag = 0;

u8 Isg_Disp_Mode;
u8 IsgParmView;
u8 IsgGpsMenuMode, IsgGeoFenceIdx;

void gui_delay_us(unsigned int nCount)
{
  nCount *= 15; //cortex m4
  for(; nCount!=0; nCount--) {
  }
}


void Key_MainMode(void)
{
  int i, j;
  
  if(KeyOKLongFlag) KeyOKLongFlag = 0;

  switch(Key_Val) {   
    case KEY_OK:      
      if(Power_Off_Mode && Setting.Can_Enable) 
        KeyOKLongFlag = 1;
      else{
        if(NotRegist_Flag == 1) { 
          if(Drv_Regist_Cnt) NVIC_SystemReset();
          for(i = 0; i < 10; i++) {
            Tmp_Buff[0][i] = 0x20;
            Tmp_Buff[1][i] = 0x30;
            Tmp_Buff[2][i] = 0x30;
          }                
          Display_Mode = 1;
          Menu_Disp_Mode = 3;
          Btn_UpDn_Cnt = 0;
          Btn_Right_Cnt = 0;
          Display_Flag = 1;
        }
        else if(NotRegist_Flag == 2) { 
          for(i = 0; i < 10; i++) {
            Tmp_Buff[0][i] = 0x00;
            Tmp_Buff[1][i] = 0x00;
            Tmp_Buff[2][i] = 0x00;
          }                
          Display_Mode = 2;
          Setting_Disp_Mode = 4;
          Btn_UpDn_Cnt = 0;
          Btn_Right_Cnt = 0;
          Display_Flag = 1;
        }
        else {
          Display_Mode = 1;
          Menu_Disp_Mode = 0;
          Btn_UpDn_Cnt = 0;
          Btn_Right_Cnt = 0;
          Display_Flag = 1;
        }
      }
      break;

    case KEY_UP:      
      if(Main_Disp_Mode > 0) 
        Main_Disp_Mode--;
      else
        Main_Disp_Mode = 8;
      Display_Flag = 1;
      break;
      
    case KEY_DOWN:      
      Main_Disp_Mode++;
      if(Main_Disp_Mode > 8) Main_Disp_Mode = 0; 
      Display_Flag = 1;
      break;
      
    case KEY_CANCEL:      
      if(Power_Off_Mode && Setting.Can_Enable) 
        KeyOKLongFlag = 1;
      break;

    case (KEY_UP|KEY_LONG):      
      if(Contrast < CONTRAST_MAX)
        lcd_contrast(Contrast++);  //CONTRAST VAL 0X00 ~ 0X3F //new
      break;
      
    case (KEY_DOWN|KEY_LONG):      
      if(Contrast > CONTRAST_MIN)
        lcd_contrast(Contrast--);  //CONTRAST VAL 0X00 ~ 0X3F //new
      break;
      
    case (KEY_CANCEL|KEY_LONG):    
      if(Power_Off_Mode && Setting.Can_Enable) 
        KeyOKLongFlag = 1;
#ifdef ISG_ENABLE
      else if(Main_Disp_Mode == 2) {
        if(!IsgParmView)  IsgParmView = 1;
        else  IsgParmView = 0;
        lcd_clear();
      }
#endif
      else {
        Backligth_Sel = (Backligth_Sel+1)%4; //1.4.0.0
        lcd_backlight(Backligth_Sel*10);
      }
      break;
      
    case (KEY_OK|KEY_LONG):      
      if(Power_Off_Mode && Setting.Can_Enable) 
        KeyOKLongFlag = 1;
      else {
        for(i = 0; i < 10; i++) {
          Tmp_Buff[0][i] = 0x20;
          Tmp_Buff[1][i] = 0x30;
          Tmp_Buff[2][i] = 0x30;
        }    
        Display_Mode = 1;
        Menu_Disp_Mode = 0;    //drv select
        Btn_UpDn_Cnt = -3;
        Btn_Right_Cnt = 0;    
        Display_Flag = 1;  
      }
      break;

    case (KEY_OK|KEY_UP|KEY_LONG):      
      for(i = 0; i < 3; i++) 
        for(j = 0; j < 20; j++) 
          Tmp_Buff[i][j] = 0;   
      Display_Mode = 2;
      Setting_Disp_Mode = 0;
      Btn_Right_Cnt = 0;
      Btn_UpDn_Cnt = 0;
      Btn_UpDn_Old = 0xff;
      Display_Flag = 1;  
      break;

    default : break;
  }
}
const u8 BacklightFont[4][5] = {
  {0x00,0x00,0x00,0x00,0x00},
  {0xDB,0x3C,0xFF,0x3C,0xDB},
  {0xDB,0x3C,0xFF,0x24,0xDB},
  {0xDB,0x24,0xE7,0x24,0xDB},
};

u8 Prev_Backligth_Sel;
u8 Prev_GPS_Valid_Char;
u8 Prev_USB_Detect_Flag;
u8 Prev_COM_Command_Char;
u8 Prev_Brake_Signal;
u8 Prev_EventSaveFlag;
u8 Prev_IsgAccOff;
u8 Prev_DI1_Signal;
u8 Prev_DI2_Signal;
void Display_CheckItem(void)
{
  if(Display_Flag || Backligth_Sel != Prev_Backligth_Sel) {
    if(Backligth_Sel < 4)  lcd_bitmap(0, 0, 5, 8, 0, (u8*)BacklightFont[Backligth_Sel]);
    else                   lcd_printf(0,0,0," "); 
    Prev_Backligth_Sel = Backligth_Sel;
  }
  
  if(Display_Flag || GPS_Valid_Char != Prev_GPS_Valid_Char) {
    lcd_printf(8,0,0,"%c", GPS_Valid_Char);
    Prev_GPS_Valid_Char = GPS_Valid_Char;
  }

  if(Display_Flag || USB_Detect_Flag != Prev_USB_Detect_Flag) {
    if(USB_Detect_Flag) lcd_printf(16,0,0,"U");
    else                lcd_printf(16,0,0," ");
    Prev_USB_Detect_Flag = USB_Detect_Flag;
  }
  
  if(Display_Flag || COM_Command_Char != Prev_COM_Command_Char) {
    lcd_printf(24,0,0,"%c", COM_Command_Char);
    Prev_COM_Command_Char = COM_Command_Char;
  }
  
  if(Display_Flag || SecData.Signal.Brake != Prev_Brake_Signal) {
    if(SecData.Signal.Brake) lcd_printf(32,0,0,"B");
    else                     lcd_printf(32,0,0," ");
    Prev_Brake_Signal = SecData.Signal.Brake;
  }
#ifdef EVENT_DATA_SAVE_ENABLE
  if(Display_Flag || Evt.Save_Flag != Prev_EventSaveFlag) {
    if(Evt.Save_Flag) lcd_printf(40,0,0,"E");
    else              lcd_printf(40,0,0," ");
    Prev_EventSaveFlag = Evt.Save_Flag;
  }
#else  
  if(Display_Flag || Adas_Char != Prev_Adas_Char) {
    lcd_printf(40,0,0,"%c", Adas_Char);
    Prev_Adas_Char = Adas_Char;
  }  
#endif

#ifdef ISG_ENABLE
  if(Display_Flag || IsgAccOff != Prev_IsgAccOff) {
    if(IsgAccOff > 1) lcd_printf(48,0,0,"I");
    else              lcd_printf(48,0,0," ");
    Prev_IsgAccOff = IsgAccOff;
  }  
#else  
 #ifdef DIGITAL_CHECK_ENABLE
  if(Display_Flag ||  SecData.Signal.Door != Prev_DI2_Signal) {
    if(Prev_DI2_Signal) lcd_printf(48,0,0,"D");
    else                lcd_printf(48,0,0," ");
    Prev_DI2_Signal =  SecData.Signal.Door;
  }  
  
  if(Display_Flag ||  SecData.Signal.CheckChild != Prev_DI1_Signal) {
    if(Prev_DI1_Signal) lcd_printf(48,0,0,"C");
    else                lcd_printf(48,0,0," ");
    Prev_DI1_Signal =  SecData.Signal.CheckChild;
  }  
 #endif  
#endif  
}

/**
  * @brief  Main_Display
  * @param  None
  * @retval None
  */
rtc_time Gui_RTC_Val;
u8 Gui_Speed;
u16 Gui_RPM;
u16 Gui_BatVolt;
u32 Gui_TTrip;
u32 Gui_DTrip;
u32 Gui_Trip;
u32 Gui_TFuel;
u32 Gui_DFuel;
u32 Gui_Fuel;
u32 Gui_TFuel;
u32 Gui_DFuel;
u32 Gui_Fuel;
u32 Gui_FuelDist;
u32 Gui_LapTime;
u32 Gui_DLapTime;
u32 Gui_Lat;
u32 Gui_Lon;
void Main_Display(void)
{
  u32 temp, temp2;
  
  if(NotRegist_Flag) {
    if(Drv_Regist_Cnt && NotRegist_Flag == 1) {
      lcd_puts_big(0,0,0,"    등록완료    ");
      lcd_puts_big(0,2,0,"다시시작해주세요");
    }
    else {
      lcd_puts_big(0,0,0," 운전자  등록후 ");
      lcd_puts_big(0,2,0,"  사용바랍니다  ");
    }
    return;
  }

  switch(Main_Disp_Mode)  {
    case 0: //속도,RPM,Battery,총누적주행거리 
      Display_CheckItem();
      if(Display_Flag || RTC_Val.tm_mon != Gui_RTC_Val.tm_mon || RTC_Val.tm_mday != Gui_RTC_Val.tm_mday || RTC_Val.tm_hour != Gui_RTC_Val.tm_hour || RTC_Val.tm_min != Gui_RTC_Val.tm_min) {
        lcd_printf(62,0,0,"%02u-%02u %02u:%02u", RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min);
        Gui_RTC_Val = RTC_Val;
      }
      if(Display_Flag || SecData.Speed[0] != Gui_Speed) {
        lcd_big_printf(4,1,0,"%3u", SecData.Speed[0]);
        lcd_printf(32,2,0,"km/h");
        Gui_Speed = SecData.Speed[0];
      }
      if(Display_Flag || abs(SecData.RPM-Gui_RPM) > 1) {
        lcd_big_printf(70,1,0,"%4u", SecData.RPM);
        lcd_printf(106,2,0,"rpm");
        Gui_RPM = SecData.RPM;
      }
      if(Display_Flag || abs(SecData.BatVolt-Gui_BatVolt) > 1) {
        temp = SecData.BatVolt;
        lcd_printf(2,3,0,"%2u.%01u V       ", temp/10, temp%10);
        Gui_BatVolt = SecData.BatVolt;
        lcd_printf(68,3,0,"%7d", temp);
        lcd_printf(112,3,0,"Km");
        Gui_TTrip = temp;
      }
      temp = (u32)(SecData.TTrip);
      if(Display_Flag || temp != Gui_TTrip) {
        lcd_printf(68,3,0,"%7d", temp);
        lcd_printf(112,3,0,"Km");
        Gui_TTrip = temp;
      }
      break;
    case 1: //속도,RPM 
      Display_CheckItem();
      if(Display_Flag || RTC_Val.tm_mon != Gui_RTC_Val.tm_mon || RTC_Val.tm_mday != Gui_RTC_Val.tm_mday || RTC_Val.tm_hour != Gui_RTC_Val.tm_hour || RTC_Val.tm_min != Gui_RTC_Val.tm_min) {
        lcd_printf(62,0,0,"%02u-%02u %02u:%02u", RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min);
        Gui_RTC_Val = RTC_Val;
      }
      if(Display_Flag || SecData.Speed[0] != Gui_Speed) {
        lcd_big_printf(4,2,0,"%3u", SecData.Speed[0]);
        lcd_printf(32,3,0,"km/h");
        Gui_Speed = SecData.Speed[0];
      }
      if(Display_Flag || abs(SecData.RPM-Gui_RPM) > 1) {
        lcd_big_printf(70,2,0,"%4u", SecData.RPM);
        lcd_printf(106,3,0,"rpm");
        Gui_RPM = SecData.RPM;
      }
      break;
    case 2: //Battery,총누적주행거리 , 온도 
#ifndef ISG_ENABLE       
      if(Display_Flag || abs(SecData.BatVolt-Gui_BatVolt) > 1) {
        lcd_printf(0,1,0,"B-VOLT");
        temp = SecData.BatVolt;
        lcd_big_printf(74,0,0,"%2u.%01u", temp/10, temp%10);
        lcd_printf(108,1,0,"V");
        Gui_BatVolt = SecData.BatVolt;
      }
      if(TempStatus.TempEnable) {
        lcd_printf(0,3,0,"CHA");
        lcd_printf(66,3,0,"CHB");
        if(TempStatus.Ch1Enable) { 
          if(SecData.Temp1 < 0)
            lcd_big_printf(22,2,0,"-%02d.%01u", (-SecData.Temp1)/10,(-SecData.Temp1)%10);
          else 
            lcd_big_printf(22,2,0,"+%02d.%01u", SecData.Temp1/10,SecData.Temp1%10);
        }
        else if(TempStatus.Ch1Short) 
          lcd_big_printf(22,2,0,"SHORT");
        else if(TempStatus.Ch1Open) 
          lcd_big_printf(22,2,0," OPEN");
        
        if(TempStatus.Ch2Enable) {
          if(SecData.Temp2 < 0)
            lcd_big_printf(86,2,0,"-%02d.%01u", (-SecData.Temp2)/10,(-SecData.Temp2)%10);
          else
            lcd_big_printf(86,2,0,"+%02d.%01u", SecData.Temp2/10,SecData.Temp2%10);
        }
        else if(TempStatus.Ch2Short) 
          lcd_big_printf(86,2,0,"SHORT");
        else if(TempStatus.Ch2Open) 
          lcd_big_printf(86,2,0," OPEN");
      }
      else {
        temp = (u32)(SecData.TTrip);
        if(Display_Flag || temp != Gui_TTrip) {
          lcd_printf(0,3,0,"ODO");
          lcd_big_printf(50,2,0,"%7u", temp);
          lcd_printf(108,3,0,"Km");
          Gui_TTrip = temp;
        }
      }
#else
      if(IsgParmView) {
        lcd_printf(0,0,0,"ISG: %u,%2u", IsgSetting.IsgMode, SecData.IsgStatus);
        lcd_printf(64,0,0,"I-T: %4u", IdleTimeCount);
        lcd_printf(0,1,0,"B-V: %2u.%01u", SecData.BatVolt/10, SecData.BatVolt%10);
        lcd_printf(65,1,0,"C-T: %d.%u  ", SecData.Temp1/10, abs(SecData.Temp1)%10);
        lcd_printf(0,2,0,"INC: %d.%u  ", SecData.Temp2/10, abs(SecData.Temp2)%10);
        lcd_printf(64,2,0,"P-B:  %u", SecData.Signal.PBrake);
        lcd_printf(0,3,0,"N-G:  %u", SecData.Signal.NGear);
        lcd_printf(64,3,0,"H-A:  %u", SecData.Signal.HpAirAlarm);     
      }
      else {
        lcd_printf(0,1,0,"I-TIME");
        temp = DailyData.Idle_Time/3600;
        temp2 = (DailyData.Idle_Time%3600)/60;
        lcd_big_printf(58,0,0,"%2u:%02u:%02u", temp, temp2, (DailyData.Idle_Time%60));
        lcd_printf(0,3,0,"I-STOP");
        temp = DailyData.IdleStop_Time/3600;
        temp2 = (DailyData.IdleStop_Time%3600)/60;
        lcd_printf(70,2,0,"%2u:%02u:%02u", temp, temp2, (DailyData.IdleStop_Time%60));
        lcd_printf(80,3,0,"(%4d)", DailyData.IdleStop_Cnt);
      }
#endif         
      break;
    case 3: //D-LapTime, D-Trip  
      if(Display_Flag || DailyData.DLapTime/60 != Gui_DLapTime/60) {
        lcd_printf(0,1,0,"D-TIME");
        temp = DailyData.DLapTime/3600;
        temp2 = (DailyData.DLapTime%3600)/60;
        lcd_big_printf(58,0,0,"%2u:%02u", temp, temp2);
        Gui_DLapTime = DailyData.DLapTime;
      }
      temp = (u32)(SecData.DTrip*10.0); 
      if(Display_Flag || temp != Gui_DTrip) {
        lcd_printf(0,3,0,"D-TRIP");
        lcd_big_printf(50,2,0,"%4u.%01u", temp/10, temp%10);
        lcd_printf(100,3,0,"Km");
        Gui_DTrip = temp;
      }
      break;
    case 4: //LapTime, Trip  
      if(Display_Flag || DailyData.LapTime/60 != Gui_LapTime/60) {
        lcd_printf(0,1,0,"C-TIME");
        temp = DailyData.LapTime/3600;
        temp2 = (DailyData.LapTime%3600)/60;
        lcd_big_printf(58,0,0,"%2u:%02u", temp, temp2);
        Gui_LapTime = DailyData.LapTime;
      }
      temp = (u32)(SecData.Trip*10.0); 
      if(Display_Flag || temp != Gui_Trip) {
        lcd_printf(0,3,0,"C-TRIP");
        lcd_big_printf(50,2,0,"%4u.%01u", temp/10, temp%10);
        lcd_printf(100,3,0,"Km");
        Gui_Trip = temp;
      }
      break;
    case 5: //D-FUEL, T-FUEl  
      temp = (u32)SecData.DFuel; 
      if(Display_Flag || temp != Gui_DFuel) {
        lcd_printf(0,1,0,"D-FUEL");
        lcd_big_printf(40,0,0,"%6u.%03u", temp/1000, temp%1000);
        lcd_printf(122,1,0,"L");
        Gui_DFuel = temp;
      }
      temp = (u32)SecData.TFuel; 
      if(Display_Flag || temp != Gui_TFuel) {
        lcd_printf(0,3,0,"T-FUEL");
        lcd_big_printf(40,2,0,"%6u.%03u", temp/1000, temp%1000);
        lcd_printf(122,3,0,"L");
        Gui_TFuel = temp;
      }
      break;
    case 6: //FUEL-EF, FUEL-RT  
      temp = (u32)Val.FuelDist; 
      if(Display_Flag || temp != Gui_FuelDist) {
        lcd_printf(0,1,0,"FUEL-EF");
        lcd_big_printf(62,0,0,"%3u.%01u", temp/10, temp%10);
        lcd_printf(104,1,0,"Km/L");
        Gui_FuelDist = temp;
      }
      temp = (u32)SecData.Fuel; 
      if(Display_Flag || temp != Gui_Fuel) {
        lcd_printf(0,3,0,"FUEL-RT");
        lcd_big_printf(46,2,0,"%5u.%03u", temp/1000, temp%1000);
        lcd_printf(122,3,0,"L");
        Gui_Fuel = temp;
      }
      break;
    case 7: //GPS  
      if(Display_Flag || SecData.Lat != Gui_Lat) {
        lcd_printf(0,1,0,"LAT");
        lcd_big_printf(32,0,0,"%3u.%06u", SecData.Lat/1000000, SecData.Lat%1000000);
        Gui_Lat = SecData.Lat;
      }
      if(Display_Flag || SecData.Lon != Gui_Lon) {
        lcd_printf(0,3,0,"LON");
        lcd_big_printf(32,2,0,"%3u.%06u", SecData.Lon/1000000, SecData.Lon%1000000);
        Gui_Lon = SecData.Lon;
      }
      break;
    case 8: //시간
      Display_CheckItem();
      if(Display_Flag || RTC_Val.tm_mon != Gui_RTC_Val.tm_mon || RTC_Val.tm_mday != Gui_RTC_Val.tm_mday || RTC_Val.tm_hour != Gui_RTC_Val.tm_hour || RTC_Val.tm_min != Gui_RTC_Val.tm_min) {
        lcd_big_printf(20,2,0,"%02u-%02u %02u:%02u", RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min);
        Gui_RTC_Val = RTC_Val;
      }
      break;
    default:
      break;
  }   
}

/**
  * @brief  DownloadDisplayInit
  * @param  None
  * @retval None
  */
void DownloadDisplayInit(void)
{
    lcd_puts_big(0,2,0,"                "); //init
    lcd_puts_big(88,2,0,"  0");
    lcd_putc_big(112,2,0,'%');
    lcd_draw_rect(0,80,3);        
}

/**
  * @brief  Selected_Driver_Conversion
  * @param  None
  * @retval None
  */
void Selected_Driver_Conversion(void)
{
  int i;
  u16 size;
  u16 addr;
  
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Drv_Name - (u8 *)&Setting; // car regist start addr
  size = sizeof(Setting.Drv_Name);
  FRAMMultiWrite((u8 *)&Driver_Info[Selected_Driver].Name, addr, size);        
  
  size = sizeof(Driver_Info[Selected_Driver].Code);
  for(i = 0; i < size; i++)
    GuiTmpData[i] = Driver_Info[Selected_Driver].Code[i];
  GuiTmpData[size] = Selected_Driver + 0x31;
  for(i = 0; i < sizeof(Driver_Info[Selected_Driver].Num); i++)
    GuiTmpData[i+size+1] = Driver_Info[Selected_Driver].Num[i];              
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Drv_Code - (u8 *)&Setting; // car regist start addr
  size = sizeof(Setting.Drv_Code);   
  memcpy(Setting.Drv_Code, GuiTmpData, size);
  FRAMMultiWrite(Setting.Drv_Code, addr, size);  
  
  Driver_Sts[Selected_Driver].LastRegTime = RTC_LVal;
  Driver_Sts[Selected_Driver].LastLapTime = 0;
  Driver_Sts[Selected_Driver].RegCount++;
  addr = DRIVER_STS+(Selected_Driver*sizeof(st_DRIVER_STS));
  FRAMMultiWrite((u8*)&Driver_Sts[Selected_Driver], addr, sizeof(st_DRIVER_STS)); 
  SendSettingFlag = 1;
}
/**
  * @brief  Add_New_Driver
  * @param  None
  * @retval None
  */
void Add_New_Driver(st_DRIVER_INFO *driver, u8 selset, u8 mode)
{
  u8 idx = 0xff, len, addflag = 0;
  time_t min_time;
  u16 addr;
  int i;
  
  if(Drv_Regist_Cnt < 9) {  
    len = StringLen((char*)driver->Num,7);  
    if(len == 0) return;  //사번없으면 return
    for(i = 0; i < 9; i++) {  //같은 사번 검색
      if(IsSameString((char*)driver->Num, (char*)Driver_Info[i].Num, 7)) { // 같은사번있으면 거기에 업데이트
        idx = i;
        break;
      }
    }
    if(i == 9) {  // 같은 사번 없을때
      idx = Drv_Regist_Cnt;
      Drv_Regist_Cnt++;
      FRAMByteWrite(Drv_Regist_Cnt, DRV_REGIST_CNT);
      addflag = 1;
    }
  }
  else {
    if(mode == 1) {
      min_time = RTC_LVal;
      idx = 0;
      for(i = 8; i >= 0; i--) {
        if(Driver_Sts[i].LastRegTime < min_time) {
          min_time = Driver_Sts[i].LastRegTime;
          idx = i;
        }
      }
      addflag = 1;
    }
    if(Drv_Regist_Cnt > 9) {
      Drv_Regist_Cnt = 9;
      FRAMByteWrite(Drv_Regist_Cnt, DRV_REGIST_CNT);
    }
  }
  if(idx < 9) {
    memcpy((u8*)&Driver_Info[idx], (u8*)driver, sizeof(st_DRIVER_INFO));
    addr = DRIVER_INFO+(idx*sizeof(st_DRIVER_INFO));
    FRAMMultiWrite((u8*)&Driver_Info[idx], addr, sizeof(st_DRIVER_INFO));
    if(addflag) {
      memset((u8*)&Driver_Sts[idx], 0, sizeof(st_DRIVER_STS));
      addr = DRIVER_STS+(idx*sizeof(st_DRIVER_STS));
      FRAMMultiWrite((u8*)&Driver_Sts[idx], addr, sizeof(st_DRIVER_STS)); 
    }
    if(selset) {
      if(Selected_Driver != idx || addflag) {
        Selected_Driver = idx;
        FRAMByteWrite(Selected_Driver, SELECTED_DRIVER); 
        Selected_Driver_Conversion();        
      }
      USB_DrvVeh_Flag = 1;
      USB_DrvVeh_Mode = 0;
    }
  }
}


/**
  * @brief  Key_MenuMode_OK
  * @param  None
  * @retval None
  */
void Key_MenuMode_OK(void)
{
  int i;
  u8  tmp, size;
  u16 addr;
  
  switch(Menu_Disp_Mode) {//- switch SDisplay_Mode(0,3,4,5) start-// 
    case 0: //sub menu index
      Btn_UpDn_Cnt = 0;
      Btn_Right_Cnt = 0;
      Menu_Disp_Mode = Btn_UpDn_Sel + 1; 
      if(Menu_Disp_Mode == 3) {
        for(i = 0; i < 10; i++) {
          Tmp_Buff[0][i] = 0x20;
          Tmp_Buff[1][i] = 0x30;
          Tmp_Buff[2][i] = 0x30;
        }
      }
      else if(Menu_Disp_Mode == 4){               
        FRAMByteRead(&Selected_Driver, SELECTED_DRIVER);
      }
      else if(Menu_Disp_Mode > 6 && Menu_Disp_Mode < 14){
        memset(Tmp_Buff[0], 0 ,20);
        if(Menu_Disp_Mode == 7) {  
          addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_Reg_No - (u8 *)&Setting; 
          size = sizeof(Setting.Car_Reg_No);
          FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
        }
        else if(Menu_Disp_Mode == 8) {  
          addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting;
          size = sizeof(Setting.Company_Name);
          FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
        }
        else if(Menu_Disp_Mode == 9) {  
          addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Reg_No - (u8 *)&Setting;
          size = sizeof(Setting.Company_Reg_No);
          FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
        }
        else if(Menu_Disp_Mode == 10) {  
          addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_No - (u8 *)&Setting;
          size = sizeof(Setting.Car_No);
          FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
        }
        else if(Menu_Disp_Mode == 12) {  
          addr = DRV_SETTING_PARM + (u8 *)&Setting.Form_Approval_No - (u8 *)&Setting;
          size = sizeof(Setting.Form_Approval_No);
          FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
        }
        else if(Menu_Disp_Mode == 13) {  
          addr = DRV_SETTING_PARM + (u8 *)&Setting.Serial_No - (u8 *)&Setting;
          size = sizeof(Setting.Serial_No);
          FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
        }
      }
#ifdef ISG_ENABLE       
      else if(Menu_Disp_Mode == 16) {  
        Display_Mode = 3;
        Isg_Disp_Mode = 0;
        FRAMMultiRead((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
        memcpy((u8*)&TmpIsgSetting, (u8*)&IsgSetting, sizeof(stIsgSetting));
        Read_IsgMode();
      }
      else if(Menu_Disp_Mode == 17) {  
        Display_Mode = 3;
        Isg_Disp_Mode = 11;
        IsgGpsMenuMode = 0;
        IsgGeoFenceIdx = 10;
        FRAMMultiRead((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
        memcpy((u8*)&TmpIsgSetting, (u8*)&IsgSetting, sizeof(stIsgSetting));
        Read_IsgMode();
      }
      else if(Menu_Disp_Mode == 18) {  
        Display_Mode = 3;
        Isg_Disp_Mode = 14;
      }
#endif      
      
      break;
      
    case 1: //download
//      DownloadByDateFlag = 0;
      if(USB_Detect_Flag && !USB_DataDL_Flag) {
        DownloadDisplayInit();
        USB_DataDL_Flag = 1;         
      }
      break;
      
    case 2: //download history
      break;
      
    case 3: //driver regist func
      if(Drv_Regist_Mode < 3) {
        Btn_UpDn_Cnt = 0;
        Btn_Right_Cnt = 0;
        Drv_Regist_Mode++;
      }
      else {
        Menu_Disp_Mode = 0;
        Drv_Regist_Mode = 0; 
  
        if(NotRegist_Flag) {
          Btn_UpDn_Cnt = 0;
          Display_Mode = 0;
        }
        else {
          Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
        }
      }
      break;
      
    case 4: //driver select func
      Selected_Driver = abs(Btn_UpDn_Cnt);
      FRAMByteWrite(Selected_Driver, SELECTED_DRIVER);
      Selected_Driver_Conversion();
      break;
      
    case 5: //driver delete func
      if(Drv_Regist_Cnt > 0) {
        tmp = abs(Btn_UpDn_Cnt) + 1;
        addr = DRIVER_INFO + sizeof(st_DRIVER_INFO)*tmp;
        size = (9 - tmp)*sizeof(st_DRIVER_INFO);
        FRAMMultiRead(GuiTmpData, addr, size);
        FRAMMultiWrite(GuiTmpData, addr - sizeof(st_DRIVER_INFO), size);

        addr = DRIVER_STS + sizeof(st_DRIVER_STS)*tmp;
        size = (9 - tmp)*sizeof(st_DRIVER_STS);
        FRAMMultiRead(GuiTmpData, addr, size);
        FRAMMultiWrite(GuiTmpData, addr - sizeof(st_DRIVER_STS), size);
        
        if(Selected_Driver == tmp - 1 || Selected_Driver >= Drv_Regist_Cnt-1){
          if(Drv_Regist_Cnt > 1) { 
            if(Selected_Driver > 0) Selected_Driver--;
          }
          else
            Selected_Driver = 0;
          FRAMByteWrite(Selected_Driver, SELECTED_DRIVER);
          Selected_Driver_Conversion();
        }
        Drv_Regist_Cnt--;
        FRAMByteWrite(Drv_Regist_Cnt, DRV_REGIST_CNT);   
        for(i = 0; i < Drv_Regist_Cnt; i++) {
          addr = DRIVER_INFO+i*sizeof(st_DRIVER_INFO);
          FRAMMultiRead((u8 *)&Driver_Info[i], addr, sizeof(st_DRIVER_INFO));
          addr = DRIVER_STS+i*sizeof(st_DRIVER_STS);
          FRAMMultiRead((u8 *)&Driver_Sts[i], addr, sizeof(st_DRIVER_STS));
        }
      }          
      break;
      
    default : break;
    
  }          
}

void Key_MenuMode_Cancel(void)
{
  if(Menu_Disp_Mode == 0) {
    Display_Mode = 0;
    Btn_UpDn_Cnt = 0;
  }
  else {
    Menu_Disp_Mode = 0;
    Drv_Regist_Mode = 0;
    Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
  }
  Btn_Right_Cnt = 0;
}

void Key_MenuMode_Left(void)
{
  int tmp;
  
  if(Menu_Disp_Mode == 1 && DownloadByDateFlag) {
    if(Btn_Right_Cnt > 0) {
      Btn_Right_Cnt--;
      if(Btn_Right_Cnt == 0) 
        Btn_UpDn_Cnt = DNLD_DAY_MAX;
      else if(Btn_Right_Cnt == 1) 
        Btn_UpDn_Cnt = (int)DL_StartDay.tm_year -  (int)RTC_Val.tm_year;
      else if(Btn_Right_Cnt == 2) 
        Btn_UpDn_Cnt = (int)DL_StartDay.tm_mon -  (int)RTC_Val.tm_mon;
      else if(Btn_Right_Cnt == 3) 
        Btn_UpDn_Cnt = (int)DL_StartDay.tm_mday -  (int)RTC_Val.tm_mday;
      else if(Btn_Right_Cnt == 4) 
        Btn_UpDn_Cnt = (int)DL_EndDay.tm_year -  (int)RTC_Val.tm_year;
      else if(Btn_Right_Cnt == 5) 
        Btn_UpDn_Cnt = (int)DL_EndDay.tm_mon -  (int)RTC_Val.tm_mon;
      else if(Btn_Right_Cnt == 6) 
        Btn_UpDn_Cnt = (int)DL_EndDay.tm_mday -  (int)RTC_Val.tm_mday;
    }
    else
      Key_MenuMode_Cancel();
  }
  else if(Menu_Disp_Mode == 3 && Drv_Regist_Mode < 3) {
    if(Btn_Right_Cnt > 0) {
      Btn_Right_Cnt--;
    }
    else if(Drv_Regist_Mode > 0) {
      Drv_Regist_Mode--;
      Btn_Right_Cnt = 0;
    }
    else {
      Key_MenuMode_Cancel();
      return;
    }
    switch(Drv_Regist_Mode) {
      case 0:       
        if(Tmp_Buff[0][Btn_Right_Cnt] == 0x20) tmp = 0x40;
        else tmp = Tmp_Buff[0][Btn_Right_Cnt];
        if(Btn_Right_Cnt == 0) 
          Btn_UpDn_Cnt = tmp - 0x41;    // 커서 위치 값 
        else 
          Btn_UpDn_Cnt = tmp - 0x40;    // 커서 위치 값 
        break;
      case 1:       
        Btn_UpDn_Cnt = (int)Tmp_Buff[1][Btn_Right_Cnt] - 0x30;    // 커서 위치 값 
        break;
      case 2:       
        Btn_UpDn_Cnt = (int)Tmp_Buff[2][Btn_Right_Cnt] - 0x30;    // 커서 위치 값 
        break;
    }
  }
  else
    Key_MenuMode_Cancel();
}

void Key_MenuMode_Right(void)
{
  int tmp;
  
  if(Menu_Disp_Mode == 1) { //데이터 다운로드
    if(DownloadByDateFlag) {  //일자별 다운로드 처리
      if(Btn_Right_Cnt == 0) {
        DL_StartDay.tm_year = RTC_Val.tm_year;
        DL_StartDay.tm_mon = RTC_Val.tm_mon;
        DL_StartDay.tm_mday = RTC_Val.tm_mday;
        DL_EndDay.tm_year = RTC_Val.tm_year;
        DL_EndDay.tm_mon = RTC_Val.tm_mon;
        DL_EndDay.tm_mday = RTC_Val.tm_mday;
      }
      Btn_Right_Cnt++; 
      if(Btn_Right_Cnt < 7) {
        if(Btn_Right_Cnt == 1) 
          Btn_UpDn_Cnt = (int)DL_StartDay.tm_year -  (int)RTC_Val.tm_year;
        else if(Btn_Right_Cnt == 2) 
          Btn_UpDn_Cnt = (int)DL_StartDay.tm_mon -  (int)RTC_Val.tm_mon;
        else if(Btn_Right_Cnt == 3) 
          Btn_UpDn_Cnt = (int)DL_StartDay.tm_mday -  (int)RTC_Val.tm_mday;
        else if(Btn_Right_Cnt == 4) 
          Btn_UpDn_Cnt = (int)DL_EndDay.tm_year -  (int)RTC_Val.tm_year;
        else if(Btn_Right_Cnt == 5) 
          Btn_UpDn_Cnt = (int)DL_EndDay.tm_mon -  (int)RTC_Val.tm_mon;
        else if(Btn_Right_Cnt == 6) 
          Btn_UpDn_Cnt = (int)DL_EndDay.tm_mday -  (int)RTC_Val.tm_mday;        
        return;
      }
    }
  }
  else if(Menu_Disp_Mode == 3) { // 운전자 등록
    Btn_Right_Cnt++; 
    if(Drv_Regist_Mode == 0) { //운전자 이름, 운전자 번호 앞부분
      if(Btn_Right_Cnt < 10) {
        if(Tmp_Buff[0][Btn_Right_Cnt] == 0x20) tmp = 0x40;
        else tmp = Tmp_Buff[0][Btn_Right_Cnt];
        if(tmp == 0) Btn_UpDn_Cnt = 0;
        else {
          if(Btn_Right_Cnt == 0) 
            Btn_UpDn_Cnt = tmp - 0x41;    // 커서 위치 값 
          else 
            Btn_UpDn_Cnt = tmp - 0x40;    // 커서 위치 값 
        }
        return;
      }
    }
    else if(Drv_Regist_Mode == 1) {       
      if(Btn_Right_Cnt < 10) {
        if(Tmp_Buff[1][Btn_Right_Cnt] == 0) Btn_UpDn_Cnt = 0;
        else Btn_UpDn_Cnt = (int)Tmp_Buff[1][Btn_Right_Cnt] - 0x30;    // 커서 위치 값 
        return;
      }
    }
    else if(Drv_Regist_Mode == 2) { //운전자 번호 뒷부분
      if(Btn_Right_Cnt < 7) {
        if(Tmp_Buff[1][Btn_Right_Cnt] == 0) Btn_UpDn_Cnt = 0;
        else Btn_UpDn_Cnt = (int)Tmp_Buff[2][Btn_Right_Cnt] - 0x30;    // 커서 위치 값 
        return;
      }
    }
  }
  Key_MenuMode_OK();
}


void Key_MenuMode(void)
{
  
  if(USB_DataDL_Flag)  return;
  if(USB_DataDL_Complete) {
    Menu_Disp_Mode = 0;
    Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
    USB_DataDL_Complete = 0;
    Display_Flag = 1;                 
    return;
  }          
  switch(Key_Val) {  
    case KEY_OK:    
      Key_MenuMode_Right();
      if(!USB_DataDL_Flag) 
        Display_Flag = 1;
      break;
      
    case (KEY_OK|KEY_LONG):      
      Key_MenuMode_OK();
      if(!USB_DataDL_Flag) 
        Display_Flag = 1;
      break; 
      
    case KEY_UP:      
      Btn_UpDn_Cnt++;
      Display_Flag = 1;
      break;
      
    case KEY_DOWN:      
      Btn_UpDn_Cnt--; 
      Display_Flag = 1;
      break;
      
    case KEY_CANCEL:   
      Key_MenuMode_Left(); 
      Display_Flag = 1;
      break;
      
    case (KEY_CANCEL|KEY_LONG): 
      Key_MenuMode_Cancel();
      Display_Flag = 1;
      break;
      
    default : break;
  }  
}

#define MAX_MENU_TITLE 18
const char MenuTitle[MAX_MENU_TITLE][16] =
{
"다운로드        ",
"다운로드 이력   ",
"운전자 등록     ",
"운전자 선택     ",
"운전자 삭제     ",
"프로그램버전    ",
"차량번호        ",
"회사정보        ",
"사업자등록번호  ",
"차대번호        ",
"교정인자        ",
"형식승인번호    ",
"제품일련번호    ",
"모뎀정보        ",
"서버정보        ",
"공회전제한 설정 ",
"공회전제한 위치 ",
"공회전제한 상태 "
};
void Menu_DisplayTitle(u8 x, u8 y, u8 mode, u8 idx)
{
  if(idx < MAX_MENU_TITLE)
    lcd_puts_big_len(x,y,mode,(u8*)MenuTitle[idx],16);
}
/**
  * @brief  Menu Display
  * @param  None
  * @retval None
  */
void Menu_Display(void) 
{
  u8 tmp, tmp2, i, idx;
  int itmp;
//  u16 addr;
//  u8 ch;
  
  switch(Menu_Disp_Mode) {
    case 0:
#ifndef ISG_ENABLE      
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = -14;  //ISG 메뉴 빼고 15개..
      if(Btn_UpDn_Cnt < -14) Btn_UpDn_Cnt = 0;
      if(Btn_UpDn_Cnt == 0)  {
          idx = 0;
          Menu_DisplayTitle(0,0,1,idx);
          Menu_DisplayTitle(0,2,0,idx+1);
          Btn_UpDn_Sel = idx;
      }
      else if(Btn_UpDn_Cnt == -14)  {
          idx = 14;
          Menu_DisplayTitle(0,0,1,idx);
          Btn_UpDn_Sel = idx;// + 1;          
      }
#else   
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 1-MAX_MENU_TITLE;  
      if(Btn_UpDn_Cnt < 1-MAX_MENU_TITLE) Btn_UpDn_Cnt = 0;
      if(Btn_UpDn_Cnt == 0)  {
          idx = 0;
          Menu_DisplayTitle(0,0,1,idx);
          Menu_DisplayTitle(0,2,0,idx+1);
          Btn_UpDn_Sel = idx;
      }
      else if(Btn_UpDn_Cnt == 1-MAX_MENU_TITLE)  {
          idx = MAX_MENU_TITLE-1;
          Menu_DisplayTitle(0,0,1,idx);
          Btn_UpDn_Sel = idx;// + 1;          
      }      
#endif      
      else if(Btn_UpDn_Cnt < Btn_UpDn_Old) {       //down
          idx = abs(Btn_UpDn_Cnt) - 1;
          Menu_DisplayTitle(0,0,0,idx);
          Menu_DisplayTitle(0,2,1,idx+1);
          Btn_UpDn_Sel = idx + 1;
      }
      else { //if(Btn_UpDn_Cnt > Btn_UpDn_Old ) {  //up
          idx = abs(Btn_UpDn_Cnt);
          Menu_DisplayTitle(0,0,1,idx);
          Menu_DisplayTitle(0,2,0,idx+1);
          Btn_UpDn_Sel = idx;
      }
      Btn_UpDn_Old = Btn_UpDn_Cnt;
      break;

    case 1: //다운로드 
      Menu_DisplayTitle(0,0,0,0); //다운로드
      if(Btn_Right_Cnt == 0) {
        if(Btn_UpDn_Cnt < 0 ) Btn_UpDn_Cnt = DNLD_DAY_MAX+1;
        if(Btn_UpDn_Cnt > DNLD_DAY_MAX+1) Btn_UpDn_Cnt = 0;
        
        if(Btn_UpDn_Cnt == DNLD_DAY_MAX) { 
          Dnld_Day_Cnt = 0;
          DownloadByDateFlag = 1;
          lcd_puts_big(48,2,0,"날짜");
        }
        else if(Btn_UpDn_Cnt == DNLD_DAY_MAX+1) {
          Dnld_Day_Cnt = DNLD_DAY_MAX+1;
          DownloadByDateFlag = 0;
          lcd_puts_big(48,2,0,"전체");
        }
        else {
          Dnld_Day_Cnt = Btn_UpDn_Cnt+1;
          DownloadByDateFlag = 0;
          lcd_big_printf(48,2,0,"%2d일",Dnld_Day_Cnt);   
        }
      }

      if(Btn_Right_Cnt > 6) Btn_Right_Cnt = 6;  //6개 입력..
      else if(Btn_Right_Cnt == 1) {
        if(Btn_UpDn_Cnt < -1 ) Btn_UpDn_Cnt = -1;
        else if(Btn_UpDn_Cnt > 0) Btn_UpDn_Cnt = 0;
        if(RTC_Val.tm_year > 0) DL_StartDay.tm_year = RTC_Val.tm_year+Btn_UpDn_Cnt;
      }
      else if(Btn_Right_Cnt == 2) {
        if(Btn_UpDn_Cnt < 1-RTC_Val.tm_mon) Btn_UpDn_Cnt = 12-RTC_Val.tm_mon;
        else if(Btn_UpDn_Cnt > 12-RTC_Val.tm_mon) Btn_UpDn_Cnt = 1-RTC_Val.tm_mon;
        DL_StartDay.tm_mon = RTC_Val.tm_mon+Btn_UpDn_Cnt;
        if(DL_StartDay.tm_year == RTC_Val.tm_year && DL_StartDay.tm_mon > RTC_Val.tm_mon) {
          DL_StartDay.tm_mon = RTC_Val.tm_mon;
          Btn_UpDn_Cnt = 0;
        }
      }
      else if(Btn_Right_Cnt == 3) {
        if(Btn_UpDn_Cnt < 1-RTC_Val.tm_mday) Btn_UpDn_Cnt = rtc_days_in_month[DL_StartDay.tm_mon]-RTC_Val.tm_mday;
        else if(Btn_UpDn_Cnt > rtc_days_in_month[DL_StartDay.tm_mon]-RTC_Val.tm_mday) Btn_UpDn_Cnt = 1-RTC_Val.tm_mday;
        DL_StartDay.tm_mday = RTC_Val.tm_mday+Btn_UpDn_Cnt;
        if(DL_StartDay.tm_year == RTC_Val.tm_year && DL_StartDay.tm_mon == RTC_Val.tm_mon && DL_StartDay.tm_mday > RTC_Val.tm_mday) {
          DL_StartDay.tm_mday = RTC_Val.tm_mday;
          Btn_UpDn_Cnt = 0;
        }
      }
      else if(Btn_Right_Cnt == 4) {
        if(Btn_UpDn_Cnt < -1 ) Btn_UpDn_Cnt = -1;
        else if(Btn_UpDn_Cnt > 0) Btn_UpDn_Cnt = 0;
        if(RTC_Val.tm_year > 0) DL_EndDay.tm_year = RTC_Val.tm_year+Btn_UpDn_Cnt;
        if(DL_EndDay.tm_year < DL_StartDay.tm_year) {
          DL_EndDay.tm_year = DL_StartDay.tm_year;
          Btn_UpDn_Cnt = DL_EndDay.tm_year-RTC_Val.tm_year;
        }
      }
      else if(Btn_Right_Cnt == 5) {
        if(Btn_UpDn_Cnt < 1-RTC_Val.tm_mon) Btn_UpDn_Cnt = 12-RTC_Val.tm_mon;
        else if(Btn_UpDn_Cnt > 12-RTC_Val.tm_mon) Btn_UpDn_Cnt = 1-RTC_Val.tm_mon;
        DL_EndDay.tm_mon = RTC_Val.tm_mon+Btn_UpDn_Cnt;
        if(DL_EndDay.tm_year == RTC_Val.tm_year && DL_EndDay.tm_mon > RTC_Val.tm_mon) {
          DL_EndDay.tm_mon = RTC_Val.tm_mon;
          Btn_UpDn_Cnt = 0;
        }
        if(DL_EndDay.tm_year == DL_StartDay.tm_year && DL_EndDay.tm_mon < DL_StartDay.tm_mon) {
          DL_EndDay.tm_mon = DL_StartDay.tm_mon;
          Btn_UpDn_Cnt = DL_EndDay.tm_mon-RTC_Val.tm_mon;
        }
      }
      else if(Btn_Right_Cnt == 6) {
        if(Btn_UpDn_Cnt < 1-RTC_Val.tm_mday) Btn_UpDn_Cnt = rtc_days_in_month[DL_EndDay.tm_mon]-RTC_Val.tm_mday;
        else if(Btn_UpDn_Cnt > rtc_days_in_month[DL_EndDay.tm_mon]-RTC_Val.tm_mday) Btn_UpDn_Cnt = 1-RTC_Val.tm_mday;
        DL_EndDay.tm_mday = RTC_Val.tm_mday+Btn_UpDn_Cnt;
        if(DL_EndDay.tm_year == RTC_Val.tm_year && DL_EndDay.tm_mon == RTC_Val.tm_mon && DL_EndDay.tm_mday > RTC_Val.tm_mday) {
          DL_EndDay.tm_mday = RTC_Val.tm_mday;
          Btn_UpDn_Cnt = 0;
        }
        if(DL_EndDay.tm_year == DL_StartDay.tm_year && DL_EndDay.tm_mon == DL_StartDay.tm_mon && DL_EndDay.tm_mday < DL_StartDay.tm_mday) {
          DL_EndDay.tm_mday = DL_StartDay.tm_mday;
          Btn_UpDn_Cnt = DL_EndDay.tm_mday-RTC_Val.tm_mday;
        }
      }

      if(Btn_Right_Cnt > 0) {
        Dnld_Day_Cnt = 0;
        lcd_printf(0,2,0, "                    ");
        lcd_printf(0,3,0, " %02d/%02d/%02d ~ %02d/%02d/%02d", DL_StartDay.tm_year, DL_StartDay.tm_mon, DL_StartDay.tm_mday, DL_EndDay.tm_year, DL_EndDay.tm_mon, DL_EndDay.tm_mday);
        if(Btn_Right_Cnt == 1) lcd_printf(6,3,1, "%02d", DL_StartDay.tm_year);
        else if(Btn_Right_Cnt == 2) lcd_printf(24,3,1, "%02d", DL_StartDay.tm_mon);
        else if(Btn_Right_Cnt == 3) lcd_printf(42,3,1, "%02d", DL_StartDay.tm_mday);
        else if(Btn_Right_Cnt == 4) lcd_printf(72,3,1, "%02d", DL_EndDay.tm_year);
        else if(Btn_Right_Cnt == 5) lcd_printf(90,3,1, "%02d", DL_EndDay.tm_mon);
        else if(Btn_Right_Cnt == 6) lcd_printf(108,3,1, "%02d", DL_EndDay.tm_mday);
      }
      
      break;                     

    case 2: //다운로드 이력
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 0;
      if(Btn_UpDn_Cnt < -(Dnld_Hist_Cnt-1)) Btn_UpDn_Cnt = -(Dnld_Hist_Cnt-1);
      Menu_DisplayTitle(0,0,0,1); //다운로드
      if(Dnld_Hist_Cnt == 0)
        lcd_puts_big(0,2,0,"정보없음! ");
      else {
        tmp = abs(Btn_UpDn_Cnt);
        if(tmp < Dnld_Hist_Cnt) {
          tmp2 = (Dnld_Hist_Cnt-1) - tmp;
          lcd_printf(0,2,0, "[%01d] %02d/%02d/%02d %02d:%02d:%02d", tmp+1, Dnld_Hist[tmp2][0], Dnld_Hist[tmp2][1], Dnld_Hist[tmp2][2], Dnld_Hist[tmp2][3], Dnld_Hist[tmp2][4], Dnld_Hist[tmp2][5]);
        }
        tmp = tmp+1;
        if(tmp < Dnld_Hist_Cnt) {
          tmp2 = (Dnld_Hist_Cnt-1) - tmp;
          lcd_printf(0,3,0, "[%01d] %02d/%02d/%02d %02d:%02d:%02d", tmp+1, Dnld_Hist[tmp2][0], Dnld_Hist[tmp2][1], Dnld_Hist[tmp2][2], Dnld_Hist[tmp2][3], Dnld_Hist[tmp2][4], Dnld_Hist[tmp2][5]);
        }
        else {
          lcd_printf(46,3,0, "- END -"); // - E N D - 
        }
      }         
      break;
      
    case 3: //운전자 등록   

      switch(Drv_Regist_Mode) {
        case 0:       
          if(Btn_Right_Cnt == 0) idx = 25; // A  -> Z
          else                   idx = 26; // ' '  -> Z
          if(Btn_UpDn_Cnt < 0) Btn_UpDn_Cnt = idx;       
          if(Btn_UpDn_Cnt > idx) Btn_UpDn_Cnt = 0;       // Z  -> ' '
          if(Btn_Right_Cnt > 9) Btn_Right_Cnt = 9;    //10 자리 
          
          lcd_puts_big(0,0,0,"운전자 이름입력 ");

          if(Btn_Right_Cnt == 0) 
            Tmp_Buff[0][Btn_Right_Cnt] = 0x41+Btn_UpDn_Cnt;    // 커서 위치 값 
          else
            Tmp_Buff[0][Btn_Right_Cnt] = 0x40+Btn_UpDn_Cnt;    // 커서 위치 값 
          if(Tmp_Buff[0][Btn_Right_Cnt] == 0x40) 
              Tmp_Buff[0][Btn_Right_Cnt] = 0x20; 
          
          for(i = 0; i < 10; i++)             // 커서 앞부분 쓰기 
              lcd_putc_big(i*8,2,0,Tmp_Buff[0][i]);
          lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[0][Btn_Right_Cnt]);   // 커서위치 쓰기 
          break;
        case 1:       
          if(Btn_UpDn_Cnt < 0) Btn_UpDn_Cnt = 9;    // 0 -> 9
          if(Btn_UpDn_Cnt > 9) Btn_UpDn_Cnt = 0;    // 9  -> 0
          if(Btn_Right_Cnt > 9) Btn_Right_Cnt = 9;  //10 자리
    
          lcd_puts_big(0,0,0,"운전자 코드입력 ");
    
          Tmp_Buff[1][Btn_Right_Cnt] = 0x30+Btn_UpDn_Cnt;    // 커서 위치 값 
          for(i = 0; i < 10; i++)             // 커서 앞부분 쓰기 
              lcd_putc_big(i*8,2,0,Tmp_Buff[1][i]);
          lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]);   // 커서위치 쓰기 
          break;
        case 2:       
          if(Btn_UpDn_Cnt < 0) Btn_UpDn_Cnt = 9;    // 0 -> 9
          if(Btn_UpDn_Cnt > 9) Btn_UpDn_Cnt = 0;    // 9  -> 0
          if(Btn_Right_Cnt > 6) Btn_Right_Cnt = 6;  //7 자리
    
          lcd_puts_big(0,0,0,"운전자 사번입력 ");
          Tmp_Buff[2][Btn_Right_Cnt] = 0x30+Btn_UpDn_Cnt;    // 커서 위치 값 
          for(i = 0; i < 7; i++)             // 커서 앞부분 쓰기 
            lcd_putc_big(i*8,2,0,Tmp_Buff[2][i]);
          lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[2][Btn_Right_Cnt]);   // 커서위치 쓰기 
          break;
        case 3: 
          {
            st_DRIVER_INFO driver;
            
            if(Drv_Regist_Cnt > 9) Drv_Regist_Cnt = 9;
            if(Drv_Regist_Cnt < 9) {   
              memcpy(driver.Name, Tmp_Buff[0], 10);
              memcpy(driver.Code, Tmp_Buff[1], 10);
              memcpy(driver.Num, Tmp_Buff[2], 7);
              Add_New_Driver(&driver, 0, 0);
//              addr = DRIVER_INFO+(Drv_Regist_Cnt*sizeof(st_DRIVER_INFO));
//              
//              memcpy(Driver_Info[Drv_Regist_Cnt].Name, Tmp_Buff[0], 10);
//              memcpy(Driver_Info[Drv_Regist_Cnt].Code, Tmp_Buff[1], 10);
//              memcpy(Driver_Info[Drv_Regist_Cnt].Num, Tmp_Buff[2], 7);
//              FRAMMultiWrite((u8*)&Driver_Info[Drv_Regist_Cnt], addr, sizeof(st_DRIVER_INFO)); 
//
//              memcpy((u8*)&Driver_Sts[Drv_Regist_Cnt], 0, sizeof(st_DRIVER_STS));
//              addr = DRIVER_STS+(Drv_Regist_Cnt*sizeof(st_DRIVER_STS));
//              FRAMMultiWrite((u8*)&Driver_Sts[Drv_Regist_Cnt], addr, sizeof(st_DRIVER_STS)); 
//              
//              if(Drv_Regist_Cnt == 0) { //first start            
//                Selected_Driver = 0;
//                FRAMByteWrite(Selected_Driver, SELECTED_DRIVER); 
//                Selected_Driver_Conversion();
//              }          
//              Drv_Regist_Cnt++;
//              FRAMByteWrite(Drv_Regist_Cnt, DRV_REGIST_CNT);
              
              lcd_puts_big(0,2,0,"등록완료! ");
              
              if(NotRegist_Flag) { //1.4.0.0
                Display_Flag = 1;            
                Display_Mode = 0;            
              }
            }
            else
              lcd_puts_big(0,2,0,"등록실패! ");
          }
          break;
      }//switch(Drv_Regist_Mode) {        
    
      break; //case 3: //운전자 등록

    case 4: //운전자 선택    
      if(Drv_Regist_Cnt == 0) {
        lcd_puts_big(0,2,0,"데이터 없음!    ");
        break;
      }
      itmp = (-1)*(Drv_Regist_Cnt-1); 

      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 0;
      if(Btn_UpDn_Cnt < itmp) Btn_UpDn_Cnt = itmp;
      
      tmp = abs(Btn_UpDn_Cnt);
      
      if(tmp == Selected_Driver)
        lcd_putc_big(0,0,0,0x3E); // 선택되어 있는 운전자 Index 앞에 화살표  ex)>1.dhkim
      else
        lcd_putc_big(0,0,0,' '); 
      if(tmp == Selected_Driver-1)
        lcd_putc_big(0,2,0,0x3E); // 선택되어 있는 운전자 Index 앞에 화살표  ex)>1.dhkim
      else
        lcd_putc_big(0,2,0,' '); 
      
      lcd_putc_big(8,0,0,tmp+0x31); // 운전자 Index
      lcd_putc_big(16,0,0,'.');
      lcd_puts_big_len(24, 0, 1, Driver_Info[tmp].Name, 10); // 운전자 이름
      tmp = tmp+1;
      if(tmp < Drv_Regist_Cnt) {
        lcd_putc_big(8,2,0,tmp+0x31); // 운전자 Index
        lcd_putc_big(16,2,0,'.');
        lcd_puts_big_len(24, 2, 0, Driver_Info[tmp].Name, 10); // 운전자 이름
      }
      else {
        lcd_puts_big(20,2,0, "- END -"); // - E N D - 
      }
      break;

    case 5: //운전자 삭제    
      if(Drv_Regist_Cnt == 0) {
        lcd_puts_big(0,2,0,"데이터 없음!    ");
        break;
      }
      itmp = (-1)*(Drv_Regist_Cnt-1); 

      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 0;
      if(Btn_UpDn_Cnt < itmp) Btn_UpDn_Cnt = itmp;

      tmp = abs(Btn_UpDn_Cnt);

      if(tmp == Selected_Driver)
        lcd_putc_big(0,0,0,0x3E); // 선택되어 있는 운전자 Index 앞에 화살표  ex)>1.dhkim
      else
        lcd_putc_big(0,0,0,' '); 
      if(tmp == Selected_Driver-1)
        lcd_putc_big(0,2,0,0x3E); // 선택되어 있는 운전자 Index 앞에 화살표  ex)>1.dhkim
      else
        lcd_putc_big(0,2,0,' '); 

      lcd_putc_big(8,0,0,tmp+0x31); // 운전자 Index
      lcd_putc_big(16,0,0,'.');
      
      lcd_puts_big_len(24, 0, 1, Driver_Info[tmp].Name, 10); // 운전자 이름
      tmp = tmp+1;
      if(tmp < Drv_Regist_Cnt) {
        lcd_putc_big(8,2,0,tmp+0x31); // 운전자 Index
        lcd_putc_big(16,2,0,'.');
        lcd_puts_big_len(24, 2, 0, Driver_Info[tmp].Name, 10); // 운전자 이름
      }
      else {
        lcd_puts_big(20,2,0, "- END -"); // - E N D - 
      }    
      break;      

    case 6: //프로그램버전       
      lcd_big_printf(0,0,0,"FW V%1u.%1u.%1u.%1u", FW_Ver_Key[4]-'L', FW_Ver_Key[5]-'O', FW_Ver_Key[6]-'O', FW_Ver_Key[7]-'P');
#ifdef RELEASE_OUT
      lcd_putc_big(24,0,0,'v');
#endif
      if(Setting.DTG_Type == DTG_GREEN)  //1.4.0.0
        lcd_putc_big(96,0,0,'G');//1.4.0.0
      
      if(Setting.UpdateCount) {        
        if(Setting.UpdateCount < 10) 
          lcd_printf(104,1,0,"%d",Setting.UpdateCount);
        else 
          lcd_printf(104,1,0,"?");
      }
      lcd_big_printf(0,2,0,"BL V%1u.%1u T", Setting.FW_Ver[4], Setting.FW_Ver[5]);
      switch(Setting.FW_Type) {
        case MDT_S1 :
          lcd_big_printf(72,2,0,"1 S1");
          break;
        case MDT_LG :
          lcd_big_printf(72,2,0,"4 LG");
          break;
        case WIFI_LOOP :
          lcd_big_printf(72,2,0,"10WF");
          break;
        case EXT_MODEM :
          lcd_big_printf(72,2,0,"11EM");
          break;
        default :  
          lcd_big_printf(72,2,0,"F");
          break;
      } //switch(Setting.FW_Type) {
      break; //case 1    
      
    case 7: //차량번호     
      if(Tmp_Buff[0][0] < MAX_CAR_TYPE) lcd_puts_big_len(0,0,0, (u8*)Car_Type_Name[Tmp_Buff[0][0]],16);
      if(Tmp_Buff[0][1] < MAX_CAR_REGION) lcd_puts_big_len(0,2,0, (u8*)Car_No_Region[Tmp_Buff[0][1]],4);
      lcd_big_printf(40,2,0,"%1u%1u",Tmp_Buff[0][2], Tmp_Buff[0][3]);
      if(Tmp_Buff[0][4] < MAX_CAR_CLASS) lcd_puts_big_len(56,2,0, (u8*)Car_No_Class[Tmp_Buff[0][4]],2);
      lcd_big_printf(80,2,0,"%1u%1u%1u%1u",Tmp_Buff[0][5], Tmp_Buff[0][6], Tmp_Buff[0][7], Tmp_Buff[0][8]);
      break;      
 
    case 8: //회사이름
      Menu_DisplayTitle(0,0,0,7); 
      lcd_puts_big_len(0,2,0,Tmp_Buff[0],12);
      break;         
        
    case 9: //사업자등록번호
      Menu_DisplayTitle(0,0,0,8); 
      for(i = 0; i < 10; i++) {
          lcd_putc_big(i*8,2,0,Tmp_Buff[0][i]);
      }
      break;  

    case 10: //차대번호
      Menu_DisplayTitle(0,0,0,9); 
      for(i = 0; i < 3; i++) {
          lcd_putc_big(i*8+104,0,0,Tmp_Buff[0][i]);
      }
      for(i = 0; i < 14; i++) {
          lcd_putc_big(i*8,2,0,Tmp_Buff[0][i+3]);
      }
      break;       

    case 11: //교정인자
      lcd_big_printf(0,0,0, "SPD : %5uP/Km",Setting.K_Factor); 
      lcd_big_printf(0,2,0, "RPM : %5u", Setting.RPM_Factor); 
      break;  

    case 12: //형식승인번호
      Menu_DisplayTitle(0,0,0,11); 
      for(i = 0; i < 10; i++) {
          lcd_putc_big(i*8,2,0,Tmp_Buff[0][i]);
      }
      break;      
      
    case 13: //제품일련번호
      Menu_DisplayTitle(0,0,0,12); 
      for(i = 0; i < 14; i++) {
          lcd_putc_big(i*8,2,0, Tmp_Buff[0][i]);
      }
      break;
    case 14:
      if(Setting.ExtModemMode == 0)         
        lcd_printf(0,0,0,"Protocol : LOOP");
      else if(Setting.ExtModemMode == 1)         
        lcd_printf(0,0,0,"Protocol : Hanuri");
      else if(Setting.ExtModemMode == 2)         
        lcd_printf(0,0,0,"Protocol : KT_GVN");
      
      if(Setting.ExtModemTelecom == 0)  
        lcd_printf(0,1,0,"Telecom : KT");
      else if(Setting.ExtModemTelecom == 1)  
        lcd_printf(0,1,0,"Telecom : SKT");
      else if(Setting.ExtModemTelecom == 2)  
        lcd_printf(0,1,0,"Telecom : LGU+");
      
      if(Setting.ExtModemModel == 0)  
        lcd_printf(0,2,0,"Modem : Teladin 3G");
      else if(Setting.ExtModemModel == 1)  
        lcd_printf(0,2,0,"Modem : Teladin LTE");
      else if(Setting.ExtModemModel == 2)  
        lcd_printf(0,2,0,"Modem : AM LTE");
        
      lcd_printf(0,3,0,"CNUM : 0%u", ModemInfo.CNUM);
      
      break;
    case 15:
      {
        u8 ip[4];
        if(Setting.ExtModemMode == LOOP_PROTOCOL) {     
          memcpy(ip,&ModemInfo.LpIP,sizeof(ModemInfo.LpIP));
          lcd_big_printf(0,0,0,"%u.%u.%u.%u", ip[0],ip[1],ip[2],ip[3]);      
          lcd_big_printf(0,2,0,"%u", ModemInfo.LpPort); 
        }        
        else {    
          memcpy(ip,&ModemInfo.SrvIP,sizeof(ModemInfo.SrvIP));
          lcd_big_printf(0,0,0,"%u.%u.%u.%u", ip[0],ip[1],ip[2],ip[3]);      
          lcd_big_printf(0,2,0,"%u", ModemInfo.SrvPort);  
        }
      }
      
      break;
      
  }
}


void Read_SettingMode(void)
{
  int i;
  u8  size;
  u16 addr;
  u32 val;

  switch(Setting_Disp_Mode) {//- switch HSDisplay_Mode(1,2,3,4,5,6,7) start-// 
    case 3: 
      RTC_GetDateTime();
      
      Tmp_Buff[0][0] = RTC_Val.tm_year;
      Tmp_Buff[0][1] = RTC_Val.tm_mon;
      Tmp_Buff[0][2] = RTC_Val.tm_mday;
      Tmp_Buff[0][3] = RTC_Val.tm_hour;
      Tmp_Buff[0][4] = RTC_Val.tm_min; 
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 5);
      break;
    case 4: 
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_Reg_No - (u8 *)&Setting; 
      size = sizeof(Setting.Car_Reg_No);
      FRAMMultiRead(Tmp_Buff[0], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[1][i] = Tmp_Buff[0][i];
      break;
    case 5: 
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting;
      size = sizeof(Setting.Company_Name);
      FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
      for(i = 0; i < size; i++) {
        if(Tmp_Buff[0][i] >= 0x30 && Tmp_Buff[0][i] <= 0x39) { 
           Tmp_Buff[0][i] = Tmp_Buff[0][i] - 0x30;     
           Tmp_Buff[1][i] = Tmp_Buff[0][i];     
        }
        else if(Tmp_Buff[0][i] >= 0x41 && Tmp_Buff[0][i] <= 0x5A) {
           Tmp_Buff[0][i] = Tmp_Buff[0][i] - 0x41+10;     
           Tmp_Buff[1][i] = Tmp_Buff[0][i];     
        }
        else {
           Tmp_Buff[0][i] = 36;     //blank 0x20
           Tmp_Buff[1][i] = Tmp_Buff[0][i];     //blank 0x20
        }
      }
      break;
    case 6: 
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Reg_No - (u8 *)&Setting;
      size = sizeof(Setting.Company_Reg_No);
      FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[1][i] = Tmp_Buff[0][i];
      break;
    case 7: 
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_No - (u8 *)&Setting;
      size = sizeof(Setting.Car_No);
      FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
      for(i = 0; i < size; i++) {
        if(Tmp_Buff[0][i] >= 0x30 && Tmp_Buff[0][i] <= 0x39) { 
           Tmp_Buff[0][i] = Tmp_Buff[0][i] - 0x30;     
           Tmp_Buff[1][i] = Tmp_Buff[0][i];     
        }
        else if(Tmp_Buff[0][i] >= 0x41 && Tmp_Buff[0][i] <= 0x5A) {
           Tmp_Buff[0][i] = Tmp_Buff[0][i] - 0x41+10;     
           Tmp_Buff[1][i] = Tmp_Buff[0][i];     
        }
        else {
           Tmp_Buff[0][i] = 36;     //blank 0x20
           Tmp_Buff[1][i] = Tmp_Buff[0][i];     //blank 0x20
        }
      }
      break;
    case 8: {
        pstEasyMenu m = (pstEasyMenu)Tmp_Buff[0];
        m->SignalCarType = Setting.Can_Enable;      
        if(Setting.Can_Type == 1)       m->CanProtocol = 1;
        else if(Setting.Can_Type == 5)  m->CanProtocol = 2;
        else if(Setting.Can_Type == 6)  m->CanProtocol = 3;
        else if(Setting.Can_Type == 7)  m->CanProtocol = 4;
        else                            m->CanProtocol = 0;
        
        if(Setting.CanBaudrate == 250)  m->CanBaudrate = 0;
        else                            m->CanBaudrate = 1;
        
        if(Setting.CanMode)             m->CanMode = 0;
        else                            m->CanMode = 1;
        m->BrakeType = Setting.Brake_Type;
        memcpy(Tmp_Buff[1], m, sizeof(stEasyMenu));                  
      }
      break;
    case 9:
      Tmp_Buff[0][0] = Setting.Set_Direction; //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];          
      Disp_GSen_Val = 1;
      break;
    case 10:
      addr = DRV_SETTING_PARM + (u8 *)&Setting.K_Factor - (u8 *)&Setting;
      size = sizeof(Setting.K_Factor) + sizeof(Setting.RPM_Factor);
      FRAMMultiRead((u8 *)&Setting.K_Factor, addr, size);
      Tmp_Buff[0][0] = Setting.K_Factor/10000;
      Tmp_Buff[0][1] = (Setting.K_Factor%10000)/1000;
      Tmp_Buff[0][2] = (Setting.K_Factor%1000)/100;
      Tmp_Buff[0][3] = (Setting.K_Factor%100)/10;
      Tmp_Buff[0][4] = Setting.K_Factor%10;

      Tmp_Buff[0][5] = Setting.RPM_Factor/10000;
      Tmp_Buff[0][6] = (Setting.RPM_Factor%10000)/1000;
      Tmp_Buff[0][7] = (Setting.RPM_Factor%1000)/100;
      Tmp_Buff[0][8] = (Setting.RPM_Factor%100)/10;
      Tmp_Buff[0][9] = Setting.RPM_Factor%10;
           
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 11);               

      break;
      
    case 11:    //odo_offset
      val = (u32)Setting.ODO_Offeset;
      Tmp_Buff[0][0] =  val/10000000;
      Tmp_Buff[0][1] = (val%10000000)/1000000;
      Tmp_Buff[0][2] = (val%1000000)/100000;
      Tmp_Buff[0][3] = (val%100000)/10000;
      Tmp_Buff[0][4] = (val%10000)/1000;
      Tmp_Buff[0][5] = (val%1000)/100;
      Tmp_Buff[0][6] = (val%100)/10;
      Tmp_Buff[0][7] =  val%10;                    
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 8);                  
      break;
      
    case 12:                    
      FRAMMultiRead((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);  
      Tmp_Buff[0][0] = Calib_F_Factor/100;
      Tmp_Buff[0][1] = (Calib_F_Factor%100)/10;
      Tmp_Buff[0][2] = Calib_F_Factor%10;    
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 3);       
      break;
      
    case 13:                    
      
      addr = DRV_SETTING_PARM + offsetof(st_Setting,Dist_Percent); 
      FRAMMultiRead((u8 *)&Setting.Dist_Percent, addr, sizeof(Setting.Dist_Percent));  
      Tmp_Buff[0][0] =  Setting.Dist_Percent/1000;
      Tmp_Buff[0][1] = (Setting.Dist_Percent%1000)/100;
      Tmp_Buff[0][2] = (Setting.Dist_Percent%100)/10;
      Tmp_Buff[0][3] =  Setting.Dist_Percent%10;    
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 4);       
      break;
      
    case 14:
      addr = DRV_SETTING_PARM + offsetof(st_Setting,FileOutput); 
      FRAMMultiRead((u8 *)&Setting.FileOutput, addr, sizeof(Setting.FileOutput));
      Tmp_Buff[0][0] =  Setting.FileOutput;                  
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 1);    
      break;
      
    case 15:
      addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
      FRAMMultiRead((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));
      Tmp_Buff[0][0] =  Setting.ExtModemMode;                  
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 1);                                      
      break;
    case 16:
      {                  
        u8 ip[4];
        u16 port;
        if(Setting.ExtModemMode == LOOP_PROTOCOL) {
          addr = MODEM_INFO + offsetof(tsModemInfo,LpIP); 
          FRAMMultiRead((u8 *)&ModemInfo.LpIP, addr, 6);  
          memcpy(ip,&ModemInfo.LpIP,4);
          memcpy(&port,&ModemInfo.LpPort,2);
        }
        else {
          addr = MODEM_INFO + offsetof(tsModemInfo,SrvIP);   
          FRAMMultiRead((u8 *)&ModemInfo.SrvIP, addr, 6);     
          memcpy(ip,&ModemInfo.SrvIP,4);
          memcpy(&port,&ModemInfo.SrvPort,2);       
        }                 
        
        Tmp_Buff[0][0] = ip[0]/100;
        Tmp_Buff[0][1] = (ip[0]%100)/10;
        Tmp_Buff[0][2] = ip[0]%10;    
        
        Tmp_Buff[0][3] = ip[1]/100;
        Tmp_Buff[0][4] = (ip[1]%100)/10;
        Tmp_Buff[0][5] = ip[1]%10;    
        
        Tmp_Buff[0][6] = ip[2]/100;
        Tmp_Buff[0][7] = (ip[2]%100)/10;
        Tmp_Buff[0][8] = ip[2]%10;    
        
        Tmp_Buff[0][9] = ip[3]/100;
        Tmp_Buff[0][10] = (ip[3]%100)/10;
        Tmp_Buff[0][11] = ip[3]%10;    
        
        Tmp_Buff[0][12] = port/10000;
        Tmp_Buff[0][13] = (port%10000)/1000;
        Tmp_Buff[0][14] = (port%1000)/100;  
        Tmp_Buff[0][15] = (port%100)/10;
        Tmp_Buff[0][16] = (port%10);
                    
        memcpy(Tmp_Buff[1], Tmp_Buff[0], 17);         
      }
      break;
      
    case 17:                  //Parameter Export //1.4.0.0
      Tmp_Buff[0][0] = 0;
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;
      
    case 18:                  //initialize //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;
      
    case 19:                  //initialize //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;

    case 20: //1.4.0.0
      FRAMMultiRead(Tmp_Buff[0], HS_PASSWORD, 4);
      for(i = 0; i < 4; i++)
        Tmp_Buff[1][i] = Tmp_Buff[0][i];           
      break;                 
      
    default : break;                
  }//- switch HSDisplay_Mode(1,2,3,4,5,6,7) end-//      
}

void Key_SettingMode_OK(void)
{
  int i,j;
  u8  size;
  u16 addr;
  u8 cnt; //1.4.0.0
  u32 val;  
  
  switch(Setting_Disp_Mode) {//- switch HSDisplay_Mode(0,1,2,3,4,5,6,7) start-// 
    case 0: //password setting
      FRAMMultiRead(Tmp_Buff[2], HS_PASSWORD, 4);
      if( ((Tmp_Buff[1][0] == Tmp_Buff[2][0]) && (Tmp_Buff[1][1] == Tmp_Buff[2][1]) && (Tmp_Buff[1][2] == Tmp_Buff[2][2]) && (Tmp_Buff[1][3] == Tmp_Buff[2][3]))
        ||((Tmp_Buff[1][0] == 9) && (Tmp_Buff[1][1] == 8) && (Tmp_Buff[1][2] == 7) && (Tmp_Buff[1][3] == 6)) )
        Setting_Disp_Mode = 2;            
      else
        Setting_Disp_Mode = 1;  
      break;
      
    case 1:       //password failed initialize                 
      for(i = 0; i < 3; i++) 
        for(j = 0; j < 20; j++) 
          Tmp_Buff[i][j] = 0;   
      Setting_Disp_Mode = 0;  
      break;
      
    case 2:         //처음 진입 메뉴
      Setting_Disp_Mode = Btn_UpDn_Sel + 3;       
      memset(Tmp_Buff[0], 0, 20);
      memset(Tmp_Buff[1], 0, 20);
      memset(Tmp_Buff[2], 0, 20);
      Read_SettingMode();
      break;         
      
    case 3: 
      RTC_SetDateTime(Tmp_Buff[1]);
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 6);
      RTC_GetDateTime();
      Tmp_Buff[0][0] = RTC_Val.tm_year;
      Tmp_Buff[0][1] = RTC_Val.tm_mon;
      Tmp_Buff[0][2] = RTC_Val.tm_mday;
      Tmp_Buff[0][3] = RTC_Val.tm_hour;
      Tmp_Buff[0][4] = RTC_Val.tm_min; 
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 5);
      break;
      
    case 4:               
      size = sizeof(Setting.Car_Reg_No);
      memcpy(Setting.Car_Reg_No, Tmp_Buff[1], size); //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_Reg_No - (u8 *)&Setting;
      FRAMMultiWrite((u8 *)Tmp_Buff[1], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];     
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
      cnt = 0;
      for(i = 1; i < sizeof(Setting.Car_Reg_No)-3; i++) 
        if(Setting.Car_Reg_No[i] == 0x00)  cnt++; 
      if(cnt != sizeof(Setting.Car_Reg_No)-4) {
        if(!Drv_Regist_Cnt ) {
          NotRegist_Flag = 1;
          for(i = 0; i < 10; i++) {
            Tmp_Buff[0][i] = 0x20;
            Tmp_Buff[1][i] = 0x30;
            Tmp_Buff[2][i] = 0x30;
          }                
          Display_Mode = 1;
          Menu_Disp_Mode = 3;
          Btn_UpDn_Cnt = 0;
          Btn_Right_Cnt = 0;   
        }
      }                
      break;
      
    case 5:
      size = sizeof(Setting.Company_Name);
      for(i = 0; i < size; i++) {
        Tmp_Buff[2][i] = ACode[Tmp_Buff[1][i]];
        if(Tmp_Buff[2][i] == 0x20) Tmp_Buff[2][i] = 0;
      }
      memcpy(Setting.Company_Name, Tmp_Buff[2], size); //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Name - (u8 *)&Setting;
      FRAMMultiWrite((u8 *)Tmp_Buff[2], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];
      if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {    //KT GV_N
        Modem_ChangeURLInfo();
      }
      break;
      
    case 6: 
      size = sizeof(Setting.Company_Reg_No);
      memcpy(Setting.Company_Reg_No, Tmp_Buff[1], size); //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Company_Reg_No - (u8 *)&Setting;
      FRAMMultiWrite((u8 *)Tmp_Buff[1], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];
      break;
      
    case 7:
      size = sizeof(Setting.Car_No);
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Car_No - (u8 *)&Setting;
      for(i = 0; i < size; i++)
        Tmp_Buff[2][i] = ACode[Tmp_Buff[1][i]];
      memcpy(Setting.Car_No, Tmp_Buff[2], size); //1.4.0.0
      FRAMMultiWrite((u8 *)Tmp_Buff[2], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];
      break;
      
    case 8: {
        pstEasyMenu m = (pstEasyMenu)Tmp_Buff[1];

        if(m->Save) {
          m->Save = 0;
          if(Setting.Can_Type < 4) delay_ms(1000);  ///Can Response 받을 수 있는 시간 기다린 후 진행....
          
          Setting.Can_Enable = m->SignalCarType;
          FRAMMultiWrite((u8 *)&Setting.Can_Enable, DRV_SETTING_PARM + offsetof(st_Setting,Can_Enable), sizeof(Setting.Can_Enable)); 
          
          if(m->SignalCarType) {  
            if(m->CanProtocol == 0) {
              Setting.Can_Type = 4;
              Setting.CanProtocol = CAN_ID_EXT;
            }
            else {
              if(m->CanProtocol == 2) Setting.Can_Type = 5;
              else if(m->CanProtocol == 3) Setting.Can_Type = 6;
              else if(m->CanProtocol == 4) Setting.Can_Type = 7;
              else Setting.Can_Type = 1;
              Setting.CanProtocol = CAN_ID_STD;
            }                    
            FRAMMultiWrite((u8 *)&Setting.CanProtocol, DRV_SETTING_PARM + offsetof(st_Setting,CanProtocol), sizeof(Setting.CanProtocol));                     
            FRAMMultiWrite((u8 *)&Setting.Can_Type, DRV_SETTING_PARM + offsetof(st_Setting,Can_Type), sizeof(Setting.Can_Type)); 

            if(!m->CanBaudrate) Setting.CanBaudrate = 250;
            else                Setting.CanBaudrate = 500;
            FRAMMultiWrite((u8 *)&Setting.CanBaudrate, DRV_SETTING_PARM + offsetof(st_Setting,CanBaudrate), sizeof(Setting.CanBaudrate)); 

            if(m->CanMode == 0 && (Setting.Can_Type == 4 || Setting.Can_Type == 5 ))
              Setting.CanMode = CAN_Mode_Silent;      
            else
              Setting.CanMode = CAN_Mode_Normal;      
            FRAMMultiWrite((u8 *)&Setting.CanMode, DRV_SETTING_PARM + offsetof(st_Setting,CanMode), sizeof(Setting.CanMode)); 
          }
          Setting.Brake_Type = m->BrakeType; 
          FRAMMultiWrite((u8 *)&Setting.Brake_Type, DRV_SETTING_PARM + offsetof(st_Setting,Brake_Type), sizeof(Setting.Brake_Type)); 
          CAN_Configuration();
        }//if(m->Save) {                
        memcpy(Tmp_Buff[0],Tmp_Buff[1],sizeof(stEasyMenu));
      }    
      break;
      
    case 9: 
      Setting.Set_Direction = Tmp_Buff[1][0]; //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.Set_Direction - (u8 *)&Setting; //1.4.0.0
      size = sizeof(Setting.Set_Direction); //1.4.0.0
      FRAMByteWrite(Setting.Set_Direction, addr);   //1.4.0.0           
      memcpy(GS_Offset, GS_Val_Set, sizeof(GS_Offset)); //2.1.3
      FRAMMultiWrite((u8 *)&GS_Offset, OFFSET_GX, sizeof(GS_Offset));   //2.1.3  
      Tmp_Buff[0][0] = Tmp_Buff[1][0];
      break;
      
    case 10:            

#ifndef GUI_CAN_NOT_CHANGE_KFACTOR      
      Calib_K_Factor = (Tmp_Buff[1][0] * 10000) + (Tmp_Buff[1][1] * 1000) + (Tmp_Buff[1][2] * 100) + (Tmp_Buff[1][3] * 10) + Tmp_Buff[1][4];
      Setting.K_Factor = Calib_K_Factor;
      addr = DRV_SETTING_PARM + (u8 *)&Setting.K_Factor - (u8 *)&Setting;
      size = sizeof(Setting.K_Factor);
      if(Setting.K_Factor == 0) Setting.K_Factor = 5096;
      FRAMMultiWrite((u8 *)&Setting.K_Factor, addr, size);

      Setting.RPM_Factor = (Tmp_Buff[1][5] * 1000) + (Tmp_Buff[1][6] * 1000) + (Tmp_Buff[1][7] * 100) + (Tmp_Buff[1][8] * 10) + Tmp_Buff[1][9];
      addr = DRV_SETTING_PARM + (u8 *)&Setting.RPM_Factor - (u8 *)&Setting;//136; // car regist start addr
      size = sizeof(Setting.RPM_Factor);
      FRAMMultiWrite((u8 *)&Setting.RPM_Factor, addr, size);
#endif
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 10);  
      break;

    case 11: //odo_offset              
      val = (Tmp_Buff[1][0] * 10000000) 
          + (Tmp_Buff[1][1] * 1000000) 
          + (Tmp_Buff[1][2] * 100000) 
          + (Tmp_Buff[1][3] * 10000) 
          + (Tmp_Buff[1][4] * 1000) 
          + (Tmp_Buff[1][5] * 100) 
          + (Tmp_Buff[1][6] * 10) 
          +  Tmp_Buff[1][7];              

      Setting.ODO_Offeset = val;
  
      DailyData.Trip = 0;
      DailyData.DTrip = 0;
      DailyData.TTrip = (double)Setting.ODO_Offeset;
      Val.TTrip = DailyData.TTrip;
      Val.TTripOld = Val.TTrip;
      SecData.Trip = 0;
      SecData.DTrip = 0;
      SecData.TTrip = DailyData.TTrip;
                      
      addr = DRV_SETTING_PARM + offsetof(st_Setting, ODO_Offeset);
      size = sizeof(Setting.ODO_Offeset) ;
      FRAMMultiWrite((u8 *)&Setting.ODO_Offeset, addr, size); 

      FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
      
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 8);
      break; 

    case 12: //fuel factor
      Calib_F_Factor = (Tmp_Buff[1][0] * 100) + (Tmp_Buff[1][1] * 10) + Tmp_Buff[1][2];              
      FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);  
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 3);
      break;

    case 13: //distance adjust
      Setting.Dist_Percent = (Tmp_Buff[1][0] * 1000) + (Tmp_Buff[1][1] * 100) + (Tmp_Buff[1][2]*10) + Tmp_Buff[1][3];              
      addr = DRV_SETTING_PARM + offsetof(st_Setting,Dist_Percent); 
      FRAMMultiWrite((u8 *)&Setting.Dist_Percent, addr, sizeof(Setting.Dist_Percent));     
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 4);
      break;
      
    case 14:
      Setting.FileOutput = Tmp_Buff[1][0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting,FileOutput); 
      FRAMMultiWrite((u8 *)&Setting.FileOutput, addr, sizeof(Setting.FileOutput));     
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 1);
      break;
      
    case 15:
      Setting.ExtModemMode = Tmp_Buff[1][0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
      FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));     
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 1);
      break;
      
    case 16:
      {
        u8 ip[4];
        ip[0] = (Tmp_Buff[1][0] * 100) + (Tmp_Buff[1][1] * 10) + Tmp_Buff[1][2]; 
        ip[1] = (Tmp_Buff[1][3] * 100) + (Tmp_Buff[1][4] * 10) + Tmp_Buff[1][5]; 
        ip[2] = (Tmp_Buff[1][6] * 100) + (Tmp_Buff[1][7] * 10) + Tmp_Buff[1][8]; 
        ip[3] = (Tmp_Buff[1][9] * 100) + (Tmp_Buff[1][10] * 10) + Tmp_Buff[1][11]; 
        if(Setting.ExtModemMode == LOOP_PROTOCOL) {
          memcpy(&ModemInfo.LpIP,ip,sizeof(ip)); 
          addr = MODEM_INFO + offsetof(tsModemInfo,LpIP);   
          FRAMMultiWrite((u8 *)&ModemInfo.LpIP, addr, sizeof(ip)); 
          
          ModemInfo.LpPort =  (Tmp_Buff[1][12] * 10000) + (Tmp_Buff[1][13] * 1000) + (Tmp_Buff[1][14] * 100) + (Tmp_Buff[1][15]*10) + Tmp_Buff[1][16];                    
          addr = MODEM_INFO + offsetof(tsModemInfo,LpPort);   
          FRAMMultiWrite((u8 *)&ModemInfo.LpPort, addr, sizeof(ModemInfo.LpPort));  
          memset(HostSvr.SvrName, 0, sizeof(HostSvr.SvrName));
          sprintf(HostSvr.SvrName,"%u.%u.%u.%u",ip[0],ip[1],ip[2],ip[3]);
          HostSvr.SvrPort = ModemInfo.LpPort;
          FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo)); 
          SendSettingFlag = 1;
        }
        else {
          memcpy(&ModemInfo.SrvIP,ip,sizeof(ip));
          addr = MODEM_INFO + offsetof(tsModemInfo,SrvIP);   
          FRAMMultiWrite((u8 *)&ModemInfo.SrvIP, addr, sizeof(ip));  
          
          ModemInfo.SrvPort =  (Tmp_Buff[1][12] * 10000) + (Tmp_Buff[1][13] * 1000) + (Tmp_Buff[1][14] * 100) + (Tmp_Buff[1][15]*10) + Tmp_Buff[1][16];                     
          addr = MODEM_INFO + offsetof(tsModemInfo,SrvPort);   
          FRAMMultiWrite((u8 *)&ModemInfo.SrvPort, addr, sizeof(ModemInfo.SrvPort)); 
        }
        memcpy(Tmp_Buff[0], Tmp_Buff[1], 17);           
      }
      break;
      
    case 17: //Parameter Export /Import
      Tmp_Buff[0][0] = Tmp_Buff[1][0];  
      if(USB_Detect_Flag) {
        lcd_puts_big(0,2,0,"                "); //init
        if(Tmp_Buff[1][0] == 0) 
          USB_ParamDL_Flag = 1;
        else if(Tmp_Buff[1][0] == 1) 
          USB_ParamUL_Flag = 1;
      }
      break; 
          
    case 18: //Firmware Update
      Tmp_Buff[0][0] = Tmp_Buff[1][0];  
      if(USB_Detect_Flag) {
        lcd_puts_big(0,2,0,"                "); //init
        if(Tmp_Buff[1][0] == 1) 
          USB_FirmwareUpdate_Flag = 1;
      }
      break; 
      
    case 19: //initialize //1.4.0.0      
      Tmp_Buff[2][0] = Tmp_Buff[1][0]; //버퍼가 모자라서..
      memset(GuiTmpData,0,sizeof(GuiTmpData));
      if(Tmp_Buff[1][0] > 0 && Setting.Can_Type < 4) delay_ms(1000);  ///Can Response 받을 수 있는 시간 기다린 후 진행....
      if(Tmp_Buff[1][0] == 1) {          

        SecData.Fuel = 0;
        SecData.DFuel = 0;
        SecData.TFuel = 0;
        DailyData.Fuel = 0;
        DailyData.DFuel = 0;
        DailyData.TFuel = 0;
        CAN_Val.FuelRate = 0;
        CAN_Val.FuelRateSum = 0;
        CAN_Val.FuelRateCnt = 0;
        CAN_Val.InstKPL = 0;
        CAN_Val.InstKPLSum = 0;
        CAN_Val.InstKPLCnt = 0;
        CAN_Val.AvgKPL = 0;
        CAN_Val.TFuel = 0;

        FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
        NVIC_SystemReset();                
      }                
      else if(Tmp_Buff[1][0] == 2) {                  
        Setting.ODO_Offeset = 0;
        
        SecData.Trip = 0;
        SecData.DTrip = 0;
        SecData.TTrip = 0;
        DailyData.Trip = 0;
        DailyData.DTrip = 0;
        DailyData.TTrip = 0;
        DailyData.LapTime = 0;
        DailyData.DLapTime = 0;
        DailyData.Drv_Time = 0;
        DailyData.Idle_Time = 0;
        DailyData.IdleStop_Time = 0;
        DailyData.IdleStop_Cnt = 0;
        Val.TTrip = 0;
        Val.TTripOld = 0;
                        
        addr = DRV_SETTING_PARM + offsetof(st_Setting, ODO_Offeset);
        size = sizeof(Setting.ODO_Offeset);
        FRAMMultiWrite((u8*)&Setting.ODO_Offeset, addr, size); 
        
        FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
        NVIC_SystemReset();                
      }                
      else if(Tmp_Buff[1][0] == 3) {
        Modem_Parameter_Reset();
        NVIC_SystemReset();                
      }             
      else if(Tmp_Buff[1][0] == 4) {
        Parameter_Reset();
        NVIC_SystemReset();                
      }             
      Tmp_Buff[0][0] = Tmp_Buff[2][0];    
      break;    
      
    case 20: //1.4.0.0
      FRAMMultiWrite(Tmp_Buff[1], HS_PASSWORD, 4);
      for(i = 0; i < 4; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];            
      break;      
          
  }//- switch HSDisplay_Mode(0,1,2,3,4,5,6,7) end-// 
        
  if(Setting_Disp_Mode != 0) {
    Btn_UpDn_Cnt = 0;
    Btn_Right_Cnt = 0; 
  }
}

void Key_SettingMode_Cancel(void)
{
  if(Setting_Disp_Mode < 3) {       
    memset(Tmp_Buff[0],0,20);
    memset(Tmp_Buff[1],0,20);
    memset(Tmp_Buff[2],0,20);
    Setting_Disp_Mode = 0;
    Btn_Right_Cnt = 0;
    Display_Mode = 0;
    Btn_UpDn_Cnt = 0;
  }  
  else {
    Setting_Disp_Mode = 2;  
    Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
    Disp_GSen_Val = 0;
  }
  Btn_Right_Cnt = 0;
}

#ifdef GUI_CAN_NOT_CHANGE_KFACTOR       
const unsigned char MaxSettingRightCnt[24] = {
  4, 1, 1,  5, 9, 12, 10, 17,  
  6, 1,  1,  8, 3, 4,  1, 1, 
  17,  1,  1, 1, 0, 0, 0, 0};
#else
const unsigned char MaxSettingRightCnt[24] = {
  4, 1, 1,  5, 9, 12, 10, 17,  
  6, 1,  10,  8, 3, 4,  1, 1, 
  17,  1,  1, 1, 0, 0, 0, 0};
#endif

void Key_SettingMode_Left(void) 
{
  if(Setting_Disp_Mode > 19) return;
  
  if(Setting_Disp_Mode == 2)   {
    Key_SettingMode_Cancel();
  }
  else if(MaxSettingRightCnt[Setting_Disp_Mode] > 1) { 
    if(Btn_Right_Cnt > 0) {
      Btn_Right_Cnt--;
      Btn_UpDn_Cnt = (int)Tmp_Buff[1][Btn_Right_Cnt]-(int)Tmp_Buff[0][Btn_Right_Cnt];
    }
    else
      Key_SettingMode_Cancel();
  }
  else
    Key_SettingMode_Cancel();
}

void Key_SettingMode_Right(void)
{
  if(Setting_Disp_Mode > 19) return;
  
  if(Setting_Disp_Mode == 2)   {
    Key_SettingMode_OK();
  }
  else if(MaxSettingRightCnt[Setting_Disp_Mode] > 1) { 
    if(Btn_Right_Cnt < MaxSettingRightCnt[Setting_Disp_Mode]-1) { 
      Btn_Right_Cnt++;
      Btn_UpDn_Cnt = (int)Tmp_Buff[1][Btn_Right_Cnt]-(int)Tmp_Buff[0][Btn_Right_Cnt]; 
    }
    else
      Key_SettingMode_OK();
  }
  else
    Key_SettingMode_OK();
}

void Key_SettingMode(void)
{
  if(USB_ParamDL_Flag || USB_ParamUL_Flag || USB_FirmwareUpdate_Flag) return;
  if(USB_ParamDL_Complete || USB_ParamUL_Complete || USB_FirmwareUpdate_Complete) {
    USB_ParamDL_Complete = 0;
    USB_ParamUL_Complete = 0;
    USB_FirmwareUpdate_Complete = 0;
    Key_SettingMode_Cancel();
    Display_Flag = 1;
    return;
  }
  
  switch(Key_Val) {  
    case KEY_OK:      
      Key_SettingMode_Right();
      if(!USB_ParamDL_Flag && !USB_ParamUL_Flag && !USB_FirmwareUpdate_Flag)
        Display_Flag = 1;
      break;
      
    case (KEY_OK|KEY_LONG):      
      Key_SettingMode_OK();
      if(!USB_ParamDL_Flag && !USB_ParamUL_Flag && !USB_FirmwareUpdate_Flag)
        Display_Flag = 1;
      break; 

    case KEY_UP:      
      Btn_UpDn_Cnt++;
      Display_Flag = 1;
      break;
      
    case KEY_DOWN:      
      Btn_UpDn_Cnt--; 
      Display_Flag = 1;
      break;
      
    case KEY_CANCEL: 
      Key_SettingMode_Left();
      Display_Flag = 1;
      break;
      
    case (KEY_CANCEL|KEY_LONG): 
      Key_SettingMode_Cancel();
      Display_Flag = 1;
      break;
      
    default : break;
  }  
}

/**
  * @brief  UpDnRolling
  * @param  None
  * @retval None
  */
void UpDnRolling(int cnt, u8 data)
{
  cnt--;
  if(Btn_UpDn_Cnt < 0-(int)data) Btn_UpDn_Cnt = cnt-(int)data;
  if(Btn_UpDn_Cnt > cnt-(int)data) Btn_UpDn_Cnt = 0-(int)data;
}

#define MAX_SETTING_TITLE 18
const char SettingTitle[MAX_SETTING_TITLE][16] =
{
"날짜/시간설정   ",
"차량번호        ",
"회사정보        ",
"사업자등록번호  ",
"차대번호        ",
"신호설정        ",
"G센서         ",
"교정인자        ",
"ODO초기값설정   ",
"연료량 설정     ",
"거리 설정       ",
"파일출력        ",
"모뎀정보        ",
"서버정보        ",
"설정값 파일     ",
"펌웨어 업데이트 ",
"제품초기화      ",
"비밀번호 변경   "
};

void Setting_DisplayTitle(u8 x, u8 y, u8 mode, u8 idx)
{
  if(idx < MAX_SETTING_TITLE)
    lcd_puts_big_len(x,y,mode,(u8*)SettingTitle[idx],16);
}

/**
  * @brief  Hidden Set Menu Display
  * @param  None
  * @retval None
  */
void Setting_Display(void) 
{
  unsigned char i, idx;

  switch(Setting_Disp_Mode) {
    case 0:
      lcd_big_printf(0,0,0, "Password        ");         
      if(Btn_Right_Cnt > 3) Btn_Right_Cnt = 3;  //4 자리
      for(i = 0; i < 4; i++) {
        lcd_putc_big(i*8,2,0,Tmp_Buff[1][i]+0x30);
      }
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);       
      break;
    case 1:
      lcd_big_printf(0,2,0, "Password  Failed");         
      break;
    case 2:
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = -16;
      if(Btn_UpDn_Cnt < -16) Btn_UpDn_Cnt = 0; //2.2
      if(Btn_UpDn_Cnt == 0)  {
          idx = 0;
          Setting_DisplayTitle(0,0,1,idx);
          Setting_DisplayTitle(0,2,0,idx+1);
          Btn_UpDn_Sel = idx;
      }
      else if(Btn_UpDn_Cnt == -16)  { //2.2
          idx = 15; //2.2
          Setting_DisplayTitle(0,0,0,idx);
          Setting_DisplayTitle(0,2,1,idx+1);
          Btn_UpDn_Sel = idx + 1;          
      }
      else if(Btn_UpDn_Cnt < Btn_UpDn_Old) {       //down
          idx = abs(Btn_UpDn_Cnt) - 1;
          Setting_DisplayTitle(0,0,0,idx);
          Setting_DisplayTitle(0,2,1,idx+1);
          Btn_UpDn_Sel = idx + 1;
      }
      else { //if(Btn_UpDn_Cnt > Btn_UpDn_Old ) {  //up
          idx = abs(Btn_UpDn_Cnt);
          Setting_DisplayTitle(0,0,1,idx);
          Setting_DisplayTitle(0,2,0,idx+1);
          Btn_UpDn_Sel = idx;
      }
      Btn_UpDn_Old = Btn_UpDn_Cnt;
      break;

    case 3: //날짜/시간설정 
      if(Btn_Right_Cnt > 4) Btn_Right_Cnt = 4;  //10 자리
      
      Setting_DisplayTitle(0,0,0,0); 
      lcd_big_printf(0,2,0, "%02u-%02u-%02u %02u:%02u  ", Tmp_Buff[1][0], Tmp_Buff[1][1], Tmp_Buff[1][2], Tmp_Buff[1][3], Tmp_Buff[1][4]);         

      if(Btn_Right_Cnt == 0) {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
        if(Btn_UpDn_Cnt > 99-Tmp_Buff[0][0]) Btn_UpDn_Cnt = 99-Tmp_Buff[0][0];
        Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
        lcd_big_printf(0,2,1, "%02u", Tmp_Buff[1][0]);         
      }
      else if(Btn_Right_Cnt == 1) {
        if(Btn_UpDn_Cnt < 1-(int)Tmp_Buff[0][1]) Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][1];
        if(Btn_UpDn_Cnt > 12-Tmp_Buff[0][1]) Btn_UpDn_Cnt = 12-Tmp_Buff[0][1];
        Tmp_Buff[1][1] = Tmp_Buff[0][1]+Btn_UpDn_Cnt;
        lcd_big_printf(24,2,1, "%02u", Tmp_Buff[1][1]);         
      }
      else if(Btn_Right_Cnt == 2) {
        if(Btn_UpDn_Cnt < 1-(int)Tmp_Buff[0][2]) Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][2];
        if(Btn_UpDn_Cnt > 31-Tmp_Buff[0][2]) Btn_UpDn_Cnt = 31-Tmp_Buff[0][2];
        Tmp_Buff[1][2] = Tmp_Buff[0][2]+Btn_UpDn_Cnt;
        lcd_big_printf(48,2,1, "%02u", Tmp_Buff[1][2]);         
      }
      else if(Btn_Right_Cnt == 3) {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][3]) Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][3];
        if(Btn_UpDn_Cnt > 23-Tmp_Buff[0][3]) Btn_UpDn_Cnt = 23-Tmp_Buff[0][3];
        Tmp_Buff[1][3] = Tmp_Buff[0][3]+Btn_UpDn_Cnt;
        lcd_big_printf(72,2,1, "%02u", Tmp_Buff[1][3]);         
      }
      else if(Btn_Right_Cnt == 4) {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][4]) Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][4];
        if(Btn_UpDn_Cnt > 59-Tmp_Buff[0][4]) Btn_UpDn_Cnt = 59-Tmp_Buff[0][4];
        Tmp_Buff[1][4] = Tmp_Buff[0][4]+Btn_UpDn_Cnt;
        lcd_big_printf(96,2,1, "%02u", Tmp_Buff[1][4]);         
      }
      break;     
      
    case 4: //차량번호     

      if(Btn_Right_Cnt > 8) Btn_Right_Cnt = 8;  //10 자리
      if(Tmp_Buff[1][0] < MAX_CAR_TYPE) lcd_puts_big_len(0,0,0, (u8*)Car_Type_Name[Tmp_Buff[1][0]],16);
      if(Tmp_Buff[1][1] < MAX_CAR_REGION) lcd_puts_big_len(0,2,0, (u8*)Car_No_Region[Tmp_Buff[1][1]],4);
      lcd_putc_big(32,2,0,' ');
      lcd_big_printf(40,2,0, "%1u%1u", Tmp_Buff[1][2], Tmp_Buff[1][3]);         
      if(Tmp_Buff[1][4] < MAX_CAR_CLASS) lcd_puts_big_len(56,2,0, (u8*)Car_No_Class[Tmp_Buff[1][4]],2);
      lcd_putc_big(72,2,0,' ');
      lcd_big_printf(80,2,0, "%1u%1u%1u%1u", Tmp_Buff[1][5], Tmp_Buff[1][6], Tmp_Buff[1][7], Tmp_Buff[1][8]);         

      if(Btn_Right_Cnt == 0) {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) Btn_UpDn_Cnt = 11-(int)Tmp_Buff[0][0];
        if(Btn_UpDn_Cnt > 11-(int)Tmp_Buff[0][0]) Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
        Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
        if(Tmp_Buff[1][0] < MAX_CAR_TYPE) lcd_puts_big_len(0,0,1, (u8*)Car_Type_Name[Tmp_Buff[1][0]],16);
      }
      else if(Btn_Right_Cnt == 1) {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][1]) Btn_UpDn_Cnt = 17-(int)Tmp_Buff[0][1]; //1.4.0.0
        if(Btn_UpDn_Cnt > 17-(int)Tmp_Buff[0][1]) Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][1]; //1.4.0.0
        Tmp_Buff[1][1] = Tmp_Buff[0][1]+Btn_UpDn_Cnt;
        if(Tmp_Buff[1][1] < MAX_CAR_REGION) lcd_puts_big_len(0,2,1, (u8*)Car_No_Region[Tmp_Buff[1][1]],4);
      }
      else if(Btn_Right_Cnt == 4) {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][4]) Btn_UpDn_Cnt = 50-(int)Tmp_Buff[0][4];
        if(Btn_UpDn_Cnt > 50-(int)Tmp_Buff[0][4]) Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][4];
        Tmp_Buff[1][4] = Tmp_Buff[0][4]+Btn_UpDn_Cnt;
        if(Tmp_Buff[1][4] < MAX_CAR_CLASS) lcd_puts_big_len(56,2,1, (u8*)Car_No_Class[Tmp_Buff[1][4]],2);
      }
      else {
        if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
          Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
        if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
          Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
        Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
        if(Btn_Right_Cnt < 4)
          lcd_putc_big(32+(Btn_Right_Cnt-1)*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
        else
          lcd_putc_big(48+(Btn_Right_Cnt-1)*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
      }
      break;      
 
    case 5: //회사이름    
//      if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {   //KT GV_N
//        if(Btn_Right_Cnt > 9) Btn_Right_Cnt = 9;  //10 자리
//      }
//      else {
        if(Btn_Right_Cnt > 11) Btn_Right_Cnt = 11;  //12 자리
      //}
      Setting_DisplayTitle(0,0,0,2); 
      for(i = 0; i < 12; i++) {
        if(Tmp_Buff[1][i] > 36)
          lcd_putc_big(i*8,2,0,0x20);
        else
          lcd_putc_big(i*8,2,0,ACode[Tmp_Buff[1][i]]);
      }
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 36-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 36-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8,2,1,ACode[Tmp_Buff[1][Btn_Right_Cnt]]);
      
      break;         
        
    case 6: //사업자등록번호     
      if(Btn_Right_Cnt > 9) Btn_Right_Cnt = 9;  //10 자리
      Setting_DisplayTitle(0,0,0,3); 
      
      for(i = 0; i < 10; i++) {
        if(Tmp_Buff[1][i] < 0x30 && Tmp_Buff[1][i] > 0x39)
          lcd_putc_big(i*8,2,0,0x20);
        else
          lcd_putc_big(i*8,2,0,Tmp_Buff[1][i]);
      }
      if(Btn_UpDn_Cnt < 0x30-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0x39-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 0x39-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0x30-(int)Tmp_Buff[0][Btn_Right_Cnt];
      
      if(Tmp_Buff[0][Btn_Right_Cnt] == 0x20 || Tmp_Buff[0][Btn_Right_Cnt] == 0) {       
        Tmp_Buff[0][Btn_Right_Cnt] = 0x30;
        Btn_UpDn_Cnt = 0;
      }      
      
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]);
      break;  

    case 7: //차대번호
      if(Btn_Right_Cnt > 16) Btn_Right_Cnt = 16;  //17 자리
      Setting_DisplayTitle(0,0,0,4); 
      for(i = 0; i < 3; i++) {
        if(Tmp_Buff[1][i] > 36)
          lcd_putc_big(i*8+104,0,0,0x20);
        else
          lcd_putc_big(i*8+104,0,0,ACode[Tmp_Buff[1][i]]);
      }
      for(i = 0; i < 14; i++) {
        if(Tmp_Buff[1][i+3] > 36)
          lcd_putc_big(i*8,2,0,0x20);
        else
          lcd_putc_big(i*8,2,0,ACode[Tmp_Buff[1][i+3]]);
      }
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 36-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 36-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      if(Btn_Right_Cnt < 3)
        lcd_putc_big(Btn_Right_Cnt*8+104,0,1,ACode[Tmp_Buff[1][Btn_Right_Cnt]]);
      else
        lcd_putc_big((Btn_Right_Cnt-3)*8,2,1,ACode[Tmp_Buff[1][Btn_Right_Cnt]]);
      break;     
      
    case 8: {
        if(Btn_Right_Cnt > sizeof(stEasyMenu)-1) Btn_Right_Cnt = sizeof(stEasyMenu)-1;
        pstEasyMenu m = (pstEasyMenu)Tmp_Buff[1];
        if(Btn_Right_Cnt == 1)
          UpDnRolling(5,Tmp_Buff[0][Btn_Right_Cnt]);
        else if(Btn_Right_Cnt == 4)
          UpDnRolling(3,Tmp_Buff[0][Btn_Right_Cnt]);
        else
          UpDnRolling(2,Tmp_Buff[0][Btn_Right_Cnt]);
        Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
        u8 v;
        
        if(m->SignalCarType == SIG_LINE) {
          if(Btn_Right_Cnt == 1) Btn_Right_Cnt = 4;
          else if(Btn_Right_Cnt == 3) Btn_Right_Cnt = 0;
        }
        switch(Btn_Right_Cnt) {
          case 0://offsetof(stEasyMenu,CanEnable):  // Car Type
            lcd_puts_big(0,0,0,"수신신호설정    ");
            v = Tmp_Buff[1][Btn_Right_Cnt];
            if(v == SIG_LINE)     lcd_puts_big(0,2,1,"Line"); 
            else if(v == SIG_CAN) lcd_puts_big(0,2,1,"CAN"); 
            break;
          case 1:   // CAN-Type
            if(m->SignalCarType == SIG_CAN) {
              lcd_puts_big(0,0,0,"차종별 CAN설정  ");
              v = Tmp_Buff[1][Btn_Right_Cnt];
              if(v == 0) lcd_puts_big(0,2,1,"상용    ");
              else if(v == 1)   lcd_puts_big(0,2,1,"승용    ");
              else if(v == 2)   lcd_puts_big(0,2,1,"현기승용");
              else if(v == 3)   lcd_puts_big(0,2,1,"쌍용승용");
              else if(v == 4)   lcd_puts_big(0,2,1,"르노승용");
              break;
            }
          case 2:  //CAN Baud        
            if(m->SignalCarType == SIG_CAN) {
              lcd_puts_big(0,0,0,"CAN수신속도     ");
              v = Tmp_Buff[1][Btn_Right_Cnt];
              if(!v) lcd_puts_big(0,2,1,"250 kbps");
              else   lcd_puts_big(0,2,1,"500 kbps");
              break;
            }
          case 3:  //CAN Mode       
            if(m->SignalCarType == SIG_CAN) {
              lcd_puts_big(0,0,0,"CAN모드설정     ");
              v = Tmp_Buff[1][Btn_Right_Cnt];
              if(!v) lcd_puts_big(0,2,1,"Silent"); 
              else   lcd_puts_big(0,2,1,"Normal");
              break;
            }
          case 4:  //Brake Type       
            lcd_puts_big(0,0,0,"브레이크 신호   ");
            v = Tmp_Buff[1][Btn_Right_Cnt];
            if(m->SignalCarType == SIG_LINE && v == 2) {
              Tmp_Buff[1][Btn_Right_Cnt] = 0;
              v = 0;
            }
            if(!v) lcd_puts_big(0,2,1,"Normal "); 
            else if(v == 1)  lcd_puts_big(0,2,1,"Inverse");
            else if(v == 2)  lcd_puts_big(0,2,1,"CAN    ");
            break;
          case 5:   // Save          
            lcd_puts_big(0,0,0,"설정값 저장     ");
            v = Tmp_Buff[1][Btn_Right_Cnt];
            if(!v) lcd_puts_big(0,2,1,"저장안함    ");
            else   lcd_puts_big(0,2,1,"적용 및 저장");
            break;
        }
      }
      break;

    case 9: //G센서
      Tmp_Buff[1][0] = abs(Btn_UpDn_Cnt+Tmp_Buff[0][0])%6; 
      lcd_bitmap(0,0,64,32,0,(unsigned char *)bitmap_Install_DTG[Tmp_Buff[1][0]]);
      lcd_printf(64,1,0, "X: 0.00G"); 
      lcd_printf(64,2,0, "Y: 0.00G"); 
      lcd_printf(64,3,0, "Z: 0.00G"); 
      break;      

    case 10: //교정인자 
      if(Btn_Right_Cnt > 10) Btn_Right_Cnt = 10;  //11 자리
      
      lcd_puts_big(0,0,0, "SPD :       P/Km"); 
      lcd_puts_big(0,2,0, "RPM :           ");          

      for(i = 0; i < 10; i++) {
        if(i < 5)
          lcd_putc_big(i*8+48,0,0,Tmp_Buff[1][i]+0x30);
        else
          lcd_putc_big((i-5)*8+48,2,0,Tmp_Buff[1][i]+0x30);     
      }  
#ifndef GUI_CAN_NOT_CHANGE_KFACTOR       
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
            
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      if(Btn_Right_Cnt < 5) //0~4
        lcd_putc_big(Btn_Right_Cnt*8+48,0,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      else if(Btn_Right_Cnt > 4 && Btn_Right_Cnt < 10) //5~9
        lcd_putc_big((Btn_Right_Cnt-5)*8+48,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
      else { //10
        Tmp_Buff[1][Btn_Right_Cnt] = (Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt)%2;
        if(Tmp_Buff[1][Btn_Right_Cnt]) 
          lcd_putc_big(48+48,2,1, 'M');
        else
          lcd_putc_big(48+48,2,1, 'A');
      }
#endif      
      break;    
      
    case 11: //ODO offset
      Setting_DisplayTitle(0,0,0,8); 

      if(Btn_Right_Cnt > 7) Btn_Right_Cnt = 7;  //8 자리
      
      lcd_puts_big(0,2,0, "         Km     ");          

      for(i = 0; i < 8; i++) 
          lcd_putc_big(i*8,2,0,Tmp_Buff[1][i]+0x30);     
      
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
            
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
      break;  

    case 12: //fuel factor
      if(Btn_Right_Cnt > 2) Btn_Right_Cnt = 2;  //3 자리
      Setting_DisplayTitle(0,0,0,9); 
      
      for(i = 0; i < 3; i++) {
        if(Tmp_Buff[1][i] > 9)
          lcd_putc_big(i*8,2,0,0x20);
        else
          lcd_putc_big(i*8,2,0,Tmp_Buff[1][i]+0x30);
      }
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[1][Btn_Right_Cnt] + 0x30);
      lcd_putc_big(32,2,0, '%'); 
      break;  
      
    case 13: //dist factor
      if(Btn_Right_Cnt > 3) Btn_Right_Cnt = 3;  //4 자리
      Setting_DisplayTitle(0,0,0,10); 

      lcd_putc_big(0*8,2,0,Tmp_Buff[1][0]+0x30);
      lcd_putc_big(1*8,2,0,Tmp_Buff[1][1]+0x30);
      lcd_putc_big(2*8,2,0,Tmp_Buff[1][2]+0x30);
      lcd_putc_big(3*8,2,0,'.');
      lcd_putc_big(4*8,2,0,Tmp_Buff[1][3]+0x30);
      lcd_putc_big(6*8,2,0,'%'); 

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      idx = Btn_Right_Cnt;
      if(Btn_Right_Cnt == 3) idx++; 
      lcd_putc_big(idx*8,2,1,Tmp_Buff[1][Btn_Right_Cnt] + 0x30);
      break;  
      
    case 14: 
      Setting_DisplayTitle(0,0,0,11); 
      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "TXT(KS File)    ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "DTX(LOOP File)  ");    
      
      break;
      
    case 15: 
      Setting_DisplayTitle(0,0,0,12); 
      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 2-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 2-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "LOOP            ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Hanuri          ");       
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "KT_GVN          ");   
      
      break;
    case 16:
      if(Setting.ExtModemMode == KT_GVN_PROTOCOL) {
        Btn_Right_Cnt = 0;
        Btn_UpDn_Cnt = 0;
      }
      if(Btn_Right_Cnt > 16) Btn_Right_Cnt = 16;  //3 자리
      idx=0;
      for(i = 0; i < 12; i++) {        
        if((i%3)==0 && i>0) {
          lcd_putc_big(i*8+idx,0,0,'.');
          idx += 8;
        }
        lcd_putc_big(i*8+idx,0,0,Tmp_Buff[1][i]+'0');
      }
      for(i = 0; i < 5; i++) {
        lcd_putc_big(i*8,2,0,Tmp_Buff[1][i+12]+'0');
      }
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      if(Btn_Right_Cnt < 12) {
        idx = (Btn_Right_Cnt/3)*8;
        lcd_putc_big(Btn_Right_Cnt*8+idx,0,1,Tmp_Buff[1][Btn_Right_Cnt] + 0x30);
      }
      else {
        lcd_putc_big((Btn_Right_Cnt-12)*8,2,1,Tmp_Buff[1][Btn_Right_Cnt] + 0x30);
      }
      break;
      
    case 17: //Parameter Export //1.4.0.0
      Setting_DisplayTitle(0,0,0,14); 
      
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(USB_Detect_Flag) {
        if(Tmp_Buff[1][0] == 0)
          lcd_puts_big(0,2,1,"설정값 내보내기 ");
        else if(Tmp_Buff[1][0] == 1)
          lcd_puts_big(0,2,1,"설정값 읽어오기 ");
      }
      else {
        lcd_puts_big(0,2,0, "USB Not Found   ");   
      }
      break;   
      
    case 18: //Firmware update
      Setting_DisplayTitle(0,0,0,15); 
      
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(USB_Detect_Flag) {
        if(Tmp_Buff[1][0] == 0)
          lcd_puts_big(0,2,1, "No ");   
        else if(Tmp_Buff[1][0] == 1)
          lcd_puts_big(0,2,1, "YES");   
      }
      else {
        lcd_puts_big(0,2,0, "USB Not Found   ");   
      }
      break;   
      
    case 19: //Initialize //1.4.0.0
      Setting_DisplayTitle(0,0,0,16); 
      
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 4-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 4-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "No");   
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Fuel");   
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "Distance");   
      else if(Tmp_Buff[1][0] == 3)
        lcd_puts_big(0,2,1, "Modem Reset");   
      else if(Tmp_Buff[1][0] == 4)
        lcd_puts_big(0,2,1, "Factory Reset");   
                
      break;      
      
    case 20: //set password //1.4.0.0
      Setting_DisplayTitle(0,0,0,17); 
      if(Btn_Right_Cnt > 3) Btn_Right_Cnt = 3;  //4 자리
      for(i = 0; i < 4; i++) {
        lcd_putc_big(i*8,2,0,Tmp_Buff[1][i]+0x30);
      }
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);     
      break;
      
    default : break;
  }//- switch HSDisplay_Mode
}


void Read_IsgMode(void)
{
  switch(Isg_Disp_Mode) {
    case 0: 
      Tmp_Buff[0][0] = TmpIsgSetting.IsgMode;
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
    case 1: 
      Tmp_Buff[0][0] = TmpIsgSetting.IdleTimeLimit/100;
      Tmp_Buff[0][1] = (TmpIsgSetting.IdleTimeLimit%100)/10;
      Tmp_Buff[0][2] = TmpIsgSetting.IdleTimeLimit%10;
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 3);               
      break;
    case 2: 
      Tmp_Buff[0][0] = TmpIsgSetting.BatVoltLimit/100;
      Tmp_Buff[0][1] = (TmpIsgSetting.BatVoltLimit%100)/10;
      Tmp_Buff[0][2] = TmpIsgSetting.BatVoltLimit%10;
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 3);               
      break;
    case 3: 
      Tmp_Buff[0][0] = TmpIsgSetting.CoolTempLimit/10;
      Tmp_Buff[0][1] = TmpIsgSetting.CoolTempLimit%10;
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 2);               
      break;
    case 4: 
      Tmp_Buff[0][0] = TmpIsgSetting.InclineLimit/10;
      Tmp_Buff[0][1] = TmpIsgSetting.InclineLimit%10;
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 2);               
      break;
    case 5: 
      Tmp_Buff[0][0] = TmpIsgSetting.PBrakeMode;
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
    case 6: 
      Tmp_Buff[0][0] = TmpIsgSetting.AirPressMode;
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
    case 7: 
      Tmp_Buff[0][0] = TmpIsgSetting.NGearMode;
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
    case 8: 
      Tmp_Buff[0][0] = TmpIsgSetting.GpsMode;
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
    case 9: 
      if(TmpIsgSetting.GeoFenceCnt > 0 && TmpIsgSetting.GeoFenceCnt <= 10) {
        Tmp_Buff[0][0] = TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt-1].Radius/100;
        Tmp_Buff[0][1] = (TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt-1].Radius%100)/10;
        Tmp_Buff[0][2] = TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt-1].Radius%10;
      }
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 3);               
      break;
    case 10: 
      Tmp_Buff[0][0] = 0;  
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
      
    case 11: 
      Tmp_Buff[0][0] = 0;  
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
      
    case 12: 
      Tmp_Buff[0][0] = 0;Tmp_Buff[0][1] = 0;Tmp_Buff[0][2] = 0;
      if(IsgGpsMenuMode == 0 || IsgGeoFenceIdx == 10) {
        if(TmpIsgSetting.GeoFenceCnt > 0 && TmpIsgSetting.GeoFenceCnt <= 10) {
          Tmp_Buff[0][0] = TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt-1].Radius/100;
          Tmp_Buff[0][1] = (TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt-1].Radius%100)/10;
          Tmp_Buff[0][2] = TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt-1].Radius%10;
        }
      }
      else {
        Tmp_Buff[0][0] = TmpIsgSetting.GeoFence[IsgGeoFenceIdx].Radius/100;
        Tmp_Buff[0][1] = (TmpIsgSetting.GeoFence[IsgGeoFenceIdx].Radius%100)/10;
        Tmp_Buff[0][2] = TmpIsgSetting.GeoFence[IsgGeoFenceIdx].Radius%10;
      }
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 3);               
      break;
      
    case 13: 
      Tmp_Buff[0][0] = 0;  
      Tmp_Buff[1][0] = Tmp_Buff[0][0];
      break;
      
    default : break;                
  }
  Btn_UpDn_Cnt = 0;
  Btn_Right_Cnt = 0;   
}

void Key_IsgMode_OK(void)
{
  switch(Isg_Disp_Mode) {
    case 0: 
      TmpIsgSetting.IsgMode = Tmp_Buff[1][0];
      if(Tmp_Buff[1][0])  Isg_Disp_Mode++;
      else Isg_Disp_Mode = 10;
      Read_IsgMode();
      break;
    case 1: 
      TmpIsgSetting.IdleTimeLimit = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 2: 
      TmpIsgSetting.BatVoltLimit = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 3: 
      TmpIsgSetting.CoolTempLimit = Tmp_Buff[1][0]*10+Tmp_Buff[1][1];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 4: 
      TmpIsgSetting.InclineLimit = Tmp_Buff[1][0]*10+Tmp_Buff[1][1];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 5: 
      TmpIsgSetting.PBrakeMode = Tmp_Buff[1][0];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 6: 
      TmpIsgSetting.AirPressMode = Tmp_Buff[1][0];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 7: 
      TmpIsgSetting.NGearMode = Tmp_Buff[1][0];
      Isg_Disp_Mode++;
      Read_IsgMode();
      break;
    case 8: 
      TmpIsgSetting.GpsMode = Tmp_Buff[1][0];
      if(Tmp_Buff[1][0])  Isg_Disp_Mode++;
      else Isg_Disp_Mode = 10;
      Read_IsgMode();
      break;
    case 9: 
      {
        u8 i, point = 0;
        double tmp_dist,x_dist,y_dist,gps_dist;
        if(GPS_Valid && TmpIsgSetting.GeoFenceCnt < 10) {
          for(i = 0; i < TmpIsgSetting.GeoFenceCnt; i++) {
            tmp_dist = abs(SecData.Lon - TmpIsgSetting.GeoFence[i].Lon);
            x_dist = (double)tmp_dist*0.088;
            tmp_dist = abs(SecData.Lat - TmpIsgSetting.GeoFence[i].Lat);
            y_dist = (double)tmp_dist*0.111;
            gps_dist = sqrt((x_dist*x_dist)+(y_dist*y_dist));
            if(gps_dist < TmpIsgSetting.GeoFence[i].Radius) {
              TmpIsgSetting.GeoFence[i].Radius = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
              point = 1;
              TmpIsgSetting.GeoFence[i].Radius = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
              break;
            }
          }
          if(point == 0) {
            TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt].Lat = SecData.Lat;
            TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt].Lon = SecData.Lon;
            TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt].Radius = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
            TmpIsgSetting.GeoFenceCnt++;
          }
        }
        Isg_Disp_Mode++;
        Read_IsgMode();
      }
      break;
    case 10: 
      if(Tmp_Buff[1][0]) {
        memcpy((u8*)&IsgSetting, (u8*)&TmpIsgSetting, sizeof(stIsgSetting));
        FRAMMultiWrite((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
      }
      Display_Mode = 1;
      Menu_Disp_Mode = 0;
      Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
      Btn_Right_Cnt = 0;
      Isg_Disp_Mode = 0;
      break;    

    case 11: 
      {
        u8 i;
        double tmp_dist,x_dist,y_dist,gps_dist;
        IsgGpsMenuMode = Tmp_Buff[1][0];
        IsgGeoFenceIdx = 10;
        if(GPS_Valid && TmpIsgSetting.GeoFenceCnt < 10) {
          for(i = 0; i < TmpIsgSetting.GeoFenceCnt; i++) {
            tmp_dist = abs(SecData.Lon - TmpIsgSetting.GeoFence[i].Lon);
            x_dist = (double)tmp_dist*0.088;
            tmp_dist = abs(SecData.Lat - TmpIsgSetting.GeoFence[i].Lat);
            y_dist = (double)tmp_dist*0.111;
            gps_dist = sqrt((x_dist*x_dist)+(y_dist*y_dist));
            if(gps_dist < TmpIsgSetting.GeoFence[i].Radius) {
              IsgGeoFenceIdx = i;
              break;
            }
          }
        }
        if(!GPS_Valid) Isg_Disp_Mode = 13;
        else if(IsgGpsMenuMode == 0 && (TmpIsgSetting.GeoFenceCnt > 9 || IsgGeoFenceIdx < 10))  Isg_Disp_Mode = 13;
        else if(IsgGpsMenuMode > 0 && (TmpIsgSetting.GeoFenceCnt == 0 || IsgGeoFenceIdx == 10))  Isg_Disp_Mode = 13;
        else if(IsgGpsMenuMode == 2 && IsgGeoFenceIdx < 10) {
          memset((u8*)&TmpIsgSetting.GeoFence[IsgGeoFenceIdx], 0, sizeof(stGeoFence));
          for(i = IsgGeoFenceIdx+1; i < TmpIsgSetting.GeoFenceCnt; i++) {
            memcpy((u8*)&TmpIsgSetting.GeoFence[i-1], (u8*)&TmpIsgSetting.GeoFence[i], sizeof(stGeoFence));
            memset((u8*)&TmpIsgSetting.GeoFence[i], 0, sizeof(stGeoFence));
          }
          TmpIsgSetting.GeoFenceCnt--;
          Isg_Disp_Mode = 13;
        }
        else
          Isg_Disp_Mode++;
        
        Read_IsgMode();
      }
      break;
      
    case 12: 
      if(IsgGpsMenuMode == 0 && TmpIsgSetting.GeoFenceCnt < 10) {
        TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt].Lat = SecData.Lat;
        TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt].Lon = SecData.Lon;
        TmpIsgSetting.GeoFence[TmpIsgSetting.GeoFenceCnt].Radius = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
        TmpIsgSetting.GeoFenceCnt++;
      }
      else if(IsgGpsMenuMode == 1 && IsgGeoFenceIdx < 10) {
        TmpIsgSetting.GeoFence[IsgGeoFenceIdx].Lat = SecData.Lat;
        TmpIsgSetting.GeoFence[IsgGeoFenceIdx].Lon = SecData.Lon;
        TmpIsgSetting.GeoFence[IsgGeoFenceIdx].Radius = Tmp_Buff[1][0]*100+Tmp_Buff[1][1]*10+Tmp_Buff[1][2];
      }
      Isg_Disp_Mode++;
      Read_IsgMode();
      break; 
      
    case 13: 
      if(Tmp_Buff[1][0]) {
        memcpy((u8*)&IsgSetting, (u8*)&TmpIsgSetting, sizeof(stIsgSetting));
        FRAMMultiWrite((u8*)&IsgSetting, ISG_SETTING_PARM, sizeof(stIsgSetting));
      }
      Display_Mode = 1;
      Menu_Disp_Mode = 0;
      Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
      Btn_Right_Cnt = 0;
      Isg_Disp_Mode = 0;
      break;    
      
    case 14: 
      Display_Mode = 1;
      Menu_Disp_Mode = 0;
      Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
      Btn_Right_Cnt = 0;
      Isg_Disp_Mode = 0;
      break;    
  }
}

void Key_IsgMode_Cancel(void)
{
  if(Isg_Disp_Mode == 0 || Isg_Disp_Mode == 10  || Isg_Disp_Mode == 11|| Isg_Disp_Mode >= 13) {
    Display_Mode = 1;
    Menu_Disp_Mode = 0;
    Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
    Btn_Right_Cnt = 0;
  }  
  else {
    Btn_UpDn_Cnt = 0;
    Btn_Right_Cnt = 0;
    Isg_Disp_Mode--;
    Read_IsgMode();
  }
}

const unsigned char MaxIsgRightCnt[15] = {1, 3, 3, 2, 2, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1};

void Key_IsgMode_Left(void) 
{
  if(MaxIsgRightCnt[Isg_Disp_Mode] == 1)   {
    Key_IsgMode_Cancel();
  }
  else { 
    if(Btn_Right_Cnt > 0) {
      Btn_Right_Cnt--;
      Btn_UpDn_Cnt = (int)Tmp_Buff[1][Btn_Right_Cnt]-(int)Tmp_Buff[0][Btn_Right_Cnt];
    }
    else
      Key_IsgMode_Cancel();
  }
}

void Key_IsgMode_Right(void)
{
  if(MaxIsgRightCnt[Isg_Disp_Mode] == 1)   {
    Key_IsgMode_OK();
  }
  else { 
    if(Btn_Right_Cnt < MaxIsgRightCnt[Isg_Disp_Mode]-1) { 
      Btn_Right_Cnt++;
      Btn_UpDn_Cnt = (int)Tmp_Buff[1][Btn_Right_Cnt]-(int)Tmp_Buff[0][Btn_Right_Cnt]; 
    }
    else
      Key_IsgMode_OK();
  }
}

void Key_IsgMode(void)
{
  
  switch(Key_Val) {  
    case KEY_OK:      
      Key_IsgMode_Right();
      Display_Flag = 1;
      break;
      
    case (KEY_OK|KEY_LONG):      
      Key_IsgMode_OK();
      Display_Flag = 1;
      break; 

    case KEY_UP:      
      Btn_UpDn_Cnt++;
      Display_Flag = 1;
      break;
      
    case KEY_DOWN:      
      Btn_UpDn_Cnt--; 
      Display_Flag = 1;
      break;
      
    case KEY_CANCEL: 
      Key_IsgMode_Left();
      Display_Flag = 1;
      break;
      
    case (KEY_CANCEL|KEY_LONG): 
      Key_IsgMode_Cancel();
      Display_Flag = 1;
      break;
      
    default : break;
  }  
}

/**
  * @brief  Hidden Set Menu Display
  * @param  None
  * @retval None
  */
void Isg_Display(void) 
{
  if(TmpIsgSetting.IsgMode == 0) {
    if(Isg_Disp_Mode > 0 && Isg_Disp_Mode < 10) Isg_Disp_Mode = 0;
  }
  else {
    if(TmpIsgSetting.GpsMode == 0) {
      if(Isg_Disp_Mode == 9) Isg_Disp_Mode = 8;
    }
  }
  switch(Isg_Disp_Mode) {
    case 0:
      lcd_puts_big(0,0,0, "공회전 제한     ");         
      Btn_Right_Cnt = 0;

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 1) lcd_puts_big(0,2,1,"적용    "); 
      else    lcd_puts_big(0,2,1,"적용안함"); 
      break;
      
    case 1: 
      if(Btn_Right_Cnt > 2) Btn_Right_Cnt = 2;  //3 자리
      lcd_puts_big(0,0,0, "IDLE 허용시간   "); 
      lcd_big_printf(0,2,0, "  %1u%1u%1u 초 ", Tmp_Buff[1][0], Tmp_Buff[1][1], Tmp_Buff[1][2]);         

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8+16,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      break;    

    case 2: 
      if(Btn_Right_Cnt > 2) Btn_Right_Cnt = 2;  //3 자리
      lcd_puts_big(0,0,0, "배터리 저전압   "); 
      lcd_big_printf(0,2,0, "  %1u%1u.%1u V", Tmp_Buff[1][0], Tmp_Buff[1][1], Tmp_Buff[1][2]);         

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      if(Btn_Right_Cnt != 2)
        lcd_putc_big(Btn_Right_Cnt*8+16,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      else
        lcd_putc_big(Btn_Right_Cnt*8+24,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      break;    
      
    case 3: 
      if(Btn_Right_Cnt > 1) Btn_Right_Cnt = 1;  //2 자리
      lcd_puts_big(0,0,0, "냉각수 온도     "); 
      lcd_big_printf(0,2,0, "  %1u%1u 도 ", Tmp_Buff[1][0], Tmp_Buff[1][1]);         

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8+16,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      break;    

    case 4: 
      if(Btn_Right_Cnt > 1) Btn_Right_Cnt = 1;  //2 자리
      lcd_puts_big(0,0,0, "경사각 설정     "); 
      lcd_big_printf(0,2,0, "  %1u%1u 도 ", Tmp_Buff[1][0], Tmp_Buff[1][1]);         

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8+16,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      break;    

    case 5:
      lcd_puts_big(0,0,0, "주차 브레이크     ");         
      Btn_Right_Cnt = 0;

      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 2-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 2-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "Normal ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Inverse");       
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "CAN Use");   
      else if(Tmp_Buff[1][0] > 2) {
        lcd_puts_big(0,2,1, "Normal "); 
        Tmp_Buff[1][0] = 0;
//        lcd_puts_big(0,2,1, "Not Use");   //안전차원에서 파킹 브레이크는 꼭 있기..
      }
      break;
      
    case 6:
      lcd_puts_big(0,0,0, "압축공기 신호   ");         
      Btn_Right_Cnt = 0;

      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 3-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 3-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "Normal ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Inverse");       
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "CAN Use");   
      else if(Tmp_Buff[1][0] == 3)
        lcd_puts_big(0,2,1, "Not Use");   
      break;
      
    case 7:
      lcd_puts_big(0,0,0, "중립기어 신호   ");         
      Btn_Right_Cnt = 0;

      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 3-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 3-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "Normal ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Inverse");       
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "CAN Use");   
      else if(Tmp_Buff[1][0] == 3)
        lcd_puts_big(0,2,1, "Not Use");   
      break;

    case 8:
      lcd_puts_big(0,0,0, "GPS 위치설정    ");         
      Btn_Right_Cnt = 0;

      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "적용안함");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "적용    ");   
      break;
      
    case 9: 
      if(Btn_Right_Cnt > 2) Btn_Right_Cnt = 2;  //3 자리
      lcd_puts_big(0,0,0, "GPS 적용반경    "); 
      lcd_big_printf(0,2,0, "  %1u%1u%1u 미터 ", Tmp_Buff[1][0], Tmp_Buff[1][1], Tmp_Buff[1][2]);         

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8+16,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      break;    
      
    case 10:
      lcd_puts_big(0,0,0,"설정값 저장     ");
      Btn_Right_Cnt = 0;

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 1) lcd_puts_big(0,2,1,"적용 및 저장");
      else    lcd_puts_big(0,2,1,"저장안함    ");
      break;

    case 11:
      lcd_big_printf(0,0,0,"적용위치: %02u개소",TmpIsgSetting.GeoFenceCnt);
      Btn_Right_Cnt = 0;

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 2-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 2-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0) lcd_puts_big(0,2,1,"현재 위치추가");
      else if(Tmp_Buff[1][0] == 1) lcd_puts_big(0,2,1,"현재 위치변경");
      else    lcd_puts_big(0,2,1,"현재 위치삭제");
      break;
      
    case 12: 
      if(Btn_Right_Cnt > 2) Btn_Right_Cnt = 2;  //3 자리
      lcd_puts_big(0,0,0, "GPS 적용반경    "); 
      lcd_big_printf(0,2,0, "  %1u%1u%1u 미터 ", Tmp_Buff[1][0], Tmp_Buff[1][1], Tmp_Buff[1][2]);         

      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_putc_big(Btn_Right_Cnt*8+16,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);        
      break;    
      
    case 13: //IsgGpsMenuMode, IsgGeoFenceIdx
      {
        u8 resp = 0;
        if(!GPS_Valid) {
          lcd_puts_big(0,0,0,"GPS 수신안됨!   ");
        }
        else if(IsgGpsMenuMode == 0) {
          if(IsgSetting.GeoFenceCnt > 9) 
            lcd_puts_big(0,0,0,"등록 수 초과!   ");
          else if(IsgGeoFenceIdx < 10) 
            lcd_puts_big(0,0,0,"중복 적용지역!  ");
          else {
            lcd_puts_big(0,0,0,"현재 위치추가   ");
            resp = 1;
          }
        }
        else {
          if(IsgSetting.GeoFenceCnt == 0) 
            lcd_puts_big(0,0,0,"등록 지점 없음! ");
          else if(IsgGeoFenceIdx == 10) 
            lcd_puts_big(0,0,0,"미적용지역!     ");
          else { 
            if(IsgGpsMenuMode == 1)
              lcd_puts_big(0,0,0,"현재 위치변경   ");
            else
              lcd_puts_big(0,0,0,"현재 위치삭제   ");
            resp = 1;
          }
        }
        Btn_Right_Cnt = 0;
        if(resp == 1) {
          if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
            Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
          if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
            Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
          Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
          if(Tmp_Buff[1][0] == 1) lcd_puts_big(0,2,1,"적용 및 저장");
          else    lcd_puts_big(0,2,1,"저장안함    ");
        }
        else {
          Tmp_Buff[1][0] = 0;
          lcd_puts_big(0,2,1,"저장안함    ");
        }
      }
      break;
      
    case 14: //IsgGpsMenuMode, IsgGeoFenceIdx
      break;

    default : break;
  }
}

/**
  * @brief  Check_KeyCancel
  * @param  none
  * @retval key value
  */  	

u8 Check_KeyCancel(void)
{
  u8 keyval = 0;

  keyval = (KEY_PORT->IDR>>8);
  keyval &= KEY_MASK;
  
  if(keyval == KEY_CANCEL) 
    return 1;
  else return 0;
}
/**
  * @brief  Key_Scan
  * @param  none
  * @retval key value
  */  	

void Key_Scan(void)
{
  u8 keyval = 0;
  u8 ret = 0;

  keyval = (KEY_PORT->IDR>>8);
  keyval &= KEY_MASK;

  if((keyval^Old_Key)) {
    Long_Key_Cnt = 0;
    if(!Long_Key_Flag && keyval == 0)   {ret = Old_Key; Key_Flag = 1;}
    Long_Key_Flag = 0;
  }
  else {
    ret = 0;
    if(!Long_Key_Flag && keyval != 0 && Long_Key_Cnt < 0xff) Long_Key_Cnt++;
  }
  if(!Long_Key_Flag && keyval != 0 && Long_Key_Cnt > LONG_KEY_LIMIT)   {
    Long_Key_Flag = 1;
    ret = keyval|0x80;
    Key_Flag = 1;
  }

  Old_Key = keyval;
    
  Key_Val = ret;
}

/**
  * @brief  Key_Process
  *         by timer3 int (200ms)
  * @param  None
  * @retval None
  */
void Key_Process(void)
{
  if(!Key_Flag) return;
  Key_Flag = 0;

  if(!Backligth_Sel) {                   //백라이트를 끄고 다닐때 버튼을 누르면 백라이트 켜지고 버튼 동작이 10초동안 없으면 꺼진다.
    Backlight_Cnt = 0;
    lcd_backlight(30);
  }
  
  switch(Display_Mode) { //- switch Display_Mode(1,2,3,4) start-// 
    case 0:
      Key_MainMode();
      break;
      
    case 1:
      Key_MenuMode();
      break;
      
    case 2:
      Key_SettingMode();
      break;

#ifdef ISG_ENABLE      
    case 3:
      Key_IsgMode();
      break;
#endif      
    default : break;          
  }
}

/**
  * @brief  Disp_Init
  * @param  None
  * @retval None
  */
void Disp_Init(void) 
{
  lcd_init();
  lcd_backlight(Backligth_Sel*10);

  Display_Mode = 0;  
  Display_Flag = 1;
#ifdef GUI_PERIODIC_REFRESH 
  Main_Disp_Mode = 1;
#endif
  Display();
}

/**
  * @brief  Display
  * @param  None
  * @retval None
  */
int LCD_ReflashCnt = 0;
int IsgParmStateCnt;

u8 DriverNameDisplay(void)
{
  u8 len;

  if(VehMaintenanceFlag) {
   if((DriverNameDispCnt%6) == 0) {
      lcd_clear();
      lcd_puts_big(0,1,0," [차량 정비중]");
   }
   else if((DriverNameDispCnt%6) == 3) {
     Display_Flag = 1;
   }
   DriverNameDispCnt++;
   Gui_Count = 0;
   return 1;
  }
  if(DriverNameDispFlag) {   
   if(DriverNameDispCnt == 0) {
     len = StringLen((char*)Driver_Info[Selected_Driver].Name, 10);
     if(len > 0) {
        lcd_clear();
        lcd_puts_big(0,0,0," [운전자 인식]");
        lcd_puts_big_len(16,2,0,Driver_Info[Selected_Driver].Name,len);
     }
     else DriverNameDispFlag = 0;
   }
   DriverNameDispCnt++;
   if(DriverNameDispCnt > 5) {
     DriverNameDispFlag = 0;
     Display_Flag = 1;
   }
  }  
  if(DriverNameDispFlag) {
   Gui_Count = 0;
   return 1;
  }
  else  return 0;
}

void Display(void)
{
  s16 gval[3], i;

  Gui_Count++;
  if(Gui_Count > DISPLAY_FREQ) {//1sec   
    
#ifdef GUI_PERIODIC_REFRESH      
  LCD_ReflashCnt++;
  if(LCD_ReflashCnt > 5) {
    lcd_refresh();
    LCD_ReflashCnt = 0;
    Display_Flag = 1;
  }
#endif      
   if(DriverNameDisplay()) return;   

    //- switch Display_Mode end -// 
    switch(Display_Mode) {  
      case 0: //MainDisp
        if(!Display_Flag) {
          Main_Display();
        }
        break;
      case 1: //Menu
        CAN_Key_Off_Time = 0;
        IsgParmStateCnt = 0;
        break;
      case 2: //Setting
        CAN_Key_Off_Time = 0;
        if(Disp_GSen_Val && Setting_Disp_Mode == 9) {
          switch(Tmp_Buff[1][0]) {
            case 0:
              gval[0] = GS_Val_Dir[0];
              gval[1] = GS_Val_Dir[1];
              gval[2] = GS_Val_Dir[2];//GS_Offset[2];
              break;
            case 1:
              gval[0] = GS_Val_Dir[0];
              gval[1] = GS_Val_Dir[2];
              gval[2] = GS_Val_Dir[1];// + GS_Offset[1];
              break;
            case 2:
              gval[0] = GS_Val_Dir[1];
              gval[1] = GS_Val_Dir[0];
              gval[2] = GS_Val_Dir[2];// + GS_Offset[2];
              break;
            case 3:
              gval[0] = GS_Val_Dir[1];
              gval[1] = GS_Val_Dir[2];
              gval[2] = GS_Val_Dir[0];// + GS_Offset[0];
              break;
            case 4:
              gval[0] = GS_Val_Dir[2];
              gval[1] = GS_Val_Dir[0];
              gval[2] = GS_Val_Dir[1];// + GS_Offset[1];
              break;
            case 5:
              gval[0] = GS_Val_Dir[2];
              gval[1] = GS_Val_Dir[1];
              gval[2] = GS_Val_Dir[0];// + GS_Offset[0];
              break;
          }//- switch(Tmp_Buff[1][0]) -//
          for(i = 0; i < 3; i++) {
            GS_Val_Set[i] = GS_Val_Dir[i]+GS_Offset[i]; 
            gval[i] = gval[i]*1000/GSENSOR_1G_VAL;
            if(i == 2) gval[i] = gval[i] + 1000;
            gval[i] = gval[i]/10;
          }
          if(gval[0] < 0)  lcd_printf(64,1,0, "X:-%01u.%02uG", -1*gval[0]/100, (-1*gval[0])%100); 
          else    lcd_printf(64,1,0, "X: %01u.%02uG", gval[0]/100, gval[0]%100); 
          if(gval[1] < 0)  lcd_printf(64,2,0, "Y:-%01u.%02uG", -1*gval[1]/100, (-1*gval[1])%100); 
          else    lcd_printf(64,2,0, "Y: %01u.%02uG", gval[1]/100, gval[1]%100); 
          if(gval[2] < 0)  lcd_printf(64,3,0, "Z:-%01u.%02uG", -1*gval[2]/100, (-1*gval[2])%100); 
          else    lcd_printf(64,3,0, "Z: %01u.%02uG", gval[2]/100, gval[2]%100); 
        }
        break;
      case 3: //Menu
        CAN_Key_Off_Time = 0;
        if(Isg_Disp_Mode == 14) {
          if((Btn_UpDn_Cnt%2) == 0) { //(IsgParmStateCnt%6) < 3) {
            if(IsgParmStateCnt > 0 && (IsgParmStateCnt%6) == 0) lcd_clear();
            lcd_printf(0,0,0,"ISG: %u,%2u", IsgSetting.IsgMode, SecData.IsgStatus);
            lcd_printf(64,0,0,"I-T: %4u", IdleTimeCount);
            lcd_printf(0,1,0,"B-V: %2u.%01u", SecData.BatVolt/10, SecData.BatVolt%10);
            lcd_printf(65,1,0,"C-T: %d.%u  ", SecData.Temp1/10, abs(SecData.Temp1)%10);
            lcd_printf(0,2,0,"INC: %d.%u  ", SecData.Temp2/10, abs(SecData.Temp2)%10);
            lcd_printf(64,2,0,"P-B:  %u", SecData.Signal.PBrake);
            lcd_printf(0,3,0,"N-G:  %u", SecData.Signal.NGear);
            lcd_printf(64,3,0,"H-A:  %u", SecData.Signal.HpAirAlarm);     
          }
          else if((Btn_UpDn_Cnt%2)) { //(IsgParmStateCnt%6) == 3) { 
            lcd_clear();
            if(IsgSetting.IsgMode == 0) lcd_puts_big(0,1,0,"공회전 적용안함 "); 
            else if(SecData.IsgStatus == 1 && SecData.Speed[0] > 0) lcd_puts_big(0,1,0," 정지상태 아님  ");
            else if(SecData.IsgStatus == 1 && DpfFlag) lcd_puts_big(0,1,0,"  DPF 작동상태  ");
            else if(SecData.IsgStatus == 2) {lcd_puts_big(0,0,0," IDLE상태 아님  "); lcd_big_printf(0,2,0,"   RPM = %04u", SecData.RPM);}
            else if(SecData.IsgStatus == 3) {lcd_puts_big(0,0,0," 배터리 저전압  "); lcd_big_printf(0,2,0,"   B-V = %02u.%01u", SecData.BatVolt/10, SecData.BatVolt%10);}
            else if(SecData.IsgStatus == 4) {lcd_puts_big(0,0,0,"  냉각수 저온   "); lcd_big_printf(0,2,0,"   C-T = %d.%u", SecData.Temp1/10, abs(SecData.Temp1)%10);}
            else if(SecData.IsgStatus == 5) {lcd_puts_big(0,0,0,"  경사진 상태   "); lcd_big_printf(0,2,0,"   INC = %d.%u", SecData.Temp2/10, abs(SecData.Temp2)%10);}
            else if(SecData.IsgStatus == 6) {lcd_puts_big(0,0,0," 파킹 브레이크  "); lcd_puts_big(0,2,0,"   신호 없음    ");}
            else if(SecData.IsgStatus == 7) {lcd_puts_big(0,0,0,"   중립 기어    "); lcd_puts_big(0,2,0,"   신호 없음    ");}
            else if(SecData.IsgStatus == 8) {lcd_puts_big(0,0,0,"   압축 공기    "); lcd_puts_big(0,2,0,"   저압 알람    ");}
            else if(SecData.IsgStatus == 9) lcd_puts_big(0,1,0," GPS 미적용지역 ");
            else if(SecData.IsgStatus == 10) {lcd_puts_big(0,0,0,"공회전제한 대기 "); lcd_big_printf(0,2,0,"   %03u < %03u", IdleTimeCount, IsgSetting.IdleTimeLimit);}
            else if(SecData.IsgStatus == 20 || SecData.IsgStatus == 30) lcd_puts_big(0,1,0,"공회전제한 작동 ");
            else if(SecData.IsgStatus == 40) {lcd_puts_big(0,0,0,"공회전 제한성공 "); lcd_big_printf(0,2,0,"    %02:%02:%02    ", IdleStop_Time/3600, (IdleStop_Time%3600)/60, (IdleStop_Time%60));}
            else if(SecData.IsgStatus == 50) lcd_puts_big(0,1,0,"공회전 제한실패 ");
            else if(SecData.IsgStatus == 90) lcd_puts_big(0,1,0,"처음시동 공회전 ");
          }
          IsgParmStateCnt++;
        }
        break;
    }//- switch Display_Mode end -//   
    Gui_Count = 0;     
  }
  if(!Display_Flag) return;
  
  if((Display_Mode < 2 && (Key_Val == KEY_CANCEL || Key_Val == KEY_OK)) || (Key_Val&KEY_LONG) == KEY_LONG) {
    lcd_refresh();
  }

  lcd_clear();
  if(Display_Mode == 0) {
    Main_Display();
  }
  else if(Display_Mode == 1) {
    Menu_Display();
  }
  else if(Display_Mode == 2) {
    Setting_Display();
  }
#ifdef ISG_ENABLE   
  else if(Display_Mode == 3) {
    Isg_Display();
  }
#endif  
  Display_Flag = 0; 
}


/* Task_GUI function */
void Task_GUI(void const * argument)
{
  /* USER CODE BEGIN Task_GUI */
  int gui_count = 0;
//  int reflashcnt = 0;
  
  COM_Command_Char = ' ';
  
  /* Infinite loop */
  for(;;)
  {
    if((gui_count%100) == 0) {
      Key_Scan();
//      LED_DO_TOGGLE();
    }
    Key_Process();
    if(!Power_Off_Flag && !USB_Download_Flag) {
//      lcd_refresh();
      Display(); 
    }      
    gui_count++;
    if(gui_count > 999) {
      gui_count = 0;
      if(!Backligth_Sel) {                   //백라이트를 끄고 다닐때 버튼을 누르면 백라이트 켜지고 버튼 동작이 10초동안 없으면 꺼진다.
        if(Backlight_Cnt < 10) Backlight_Cnt++; 
        else lcd_backlight(0);
      }
//      reflashcnt++;
//      printk("%d\n",RTC_LVal);
//      if(reflashcnt > 5) {
//        LCD_Refresh_Flag = 1;      
//        reflashcnt = 0;
//      }
    }
    
//   if(USB_DataDL_Flag)
//     USB_Download();

    osDelay(1);
  }
  /* USER CODE END Task_GUI */
}

/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/
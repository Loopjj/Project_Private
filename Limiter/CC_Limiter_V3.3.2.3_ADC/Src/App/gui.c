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

#define LONG_KEY_LIMIT      7
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
u8  Btn_Right_Cnt, Btn_Right_Old;
u8  *Bitmap_MenuMode_Title[20], *Bitmap_SettingMode_Title[22]; 
u8  Tmp_Buff[3][20];
u8  Disp_GSen_Val;
u8  Disp_Alert;

u8 Key_Val;
u8 Key_ESC_Flag;
u8 Key_Flag;

u8 Old_Key = 0, Long_Key_Cnt = 0, Long_Key_Flag = 0;

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
      if(Power_Off_Mode) 
        KeyOKLongFlag = 1;
      else{
        Display_Mode = 1;
        Menu_Disp_Mode = 0;
        Btn_UpDn_Cnt = 0;
        Btn_Right_Cnt = 0;
        Display_Flag = 1;
      }
      break;

    case KEY_UP:      
      if(Main_Disp_Mode > 0) 
        Main_Disp_Mode--;
      else
        Main_Disp_Mode = 7;
      Display_Flag = 1;
      break;
      
    case KEY_DOWN:      
      Main_Disp_Mode++;
      if(Main_Disp_Mode > 7) Main_Disp_Mode = 0; 
      Display_Flag = 1;
      break;
      
    case KEY_CANCEL:      
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
      Backligth_Sel = (Backligth_Sel+1)%4; //1.4.0.0
      lcd_backlight(Backligth_Sel*10);
      break;
      
    case (KEY_OK|KEY_LONG):      
      Display_Flag = 1;  
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

    case (KEY_CANCEL|KEY_DOWN|KEY_LONG):
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
}

/**
  * @brief  Main_Display
  * @param  None
  * @retval None
  */
rtc_time Gui_RTC_Val;
u16 Gui_BatVolt;
u8 Gui_UreaLevel;
u8 Gui_Concentration;
u16 Gui_NOxIn, Gui_NOxOut;
u16 Gui_TempIn, Gui_TempOut;
u16 Gui_DpfTemp, Gui_DpfPress;
u16 Gui_MAF, Gui_State, Gui_SError, Gui_DError;
u32 Gui_DosingRate;
u32 Gui_LN;
u32 Gui_LT;
u16 Gui_Pedal[2];
u8 Gui_Speed;
u32 Gui_UreaTime;
#define UNIT_C_DEGREE 0x01

void UreaAlarmDisplay(void) 
{
  u8 urea_alarm_type = 0;
  u32 urea_acc_time;
  
  if(UreaAlarm.Status || UreaAlarm.Quality || UreaAlarm.Level || UreaAlarm.SysErr) {
    if(UreaAlarm.Status) urea_alarm_type = 1;
    else if(UreaAlarm.Quality) urea_alarm_type = 2;
    else if(UreaAlarm.Level) urea_alarm_type = 3;
    else if(UreaAlarm.SysErr) urea_alarm_type = 4;
    if(SecData.UreaAccFlag == 2) {
      lcd_puts_big(0,2,0,"2단계 출력제한중");
      if(UreaAccControl.Status == 2) urea_alarm_type = 1;
      else if(UreaAccControl.Quality== 2) urea_alarm_type = 2;
      else if(UreaAccControl.Level  == 2) urea_alarm_type = 3;
      else if(UreaAccControl.SysErr == 2) urea_alarm_type = 4;
    }
    else if(SecData.UreaAccFlag == 1) {
      lcd_puts_big(0,2,0,"1단계 출력제한중");
      if(UreaAccControl.Status == 1) urea_alarm_type = 1;
      else if(UreaAccControl.Quality== 1) urea_alarm_type = 2;
      else if(UreaAccControl.Level  == 1) urea_alarm_type = 3;
      else if(UreaAccControl.SysErr == 1) urea_alarm_type = 4;
    }
    else if(UreaAccControl.Status || UreaAccControl.Quality || UreaAccControl.Level || UreaAccControl.SysErr) {
      lcd_puts_big(0,2,0,"출력제한 예정   ");
      if(UreaAccControl.Status) urea_alarm_type = 1;
      else if(UreaAccControl.Quality) urea_alarm_type = 2;
      else if(UreaAccControl.Level) urea_alarm_type = 3;
      else if(UreaAccControl.SysErr) urea_alarm_type = 4;
    }
    else {
#ifdef TIME_HOUR
      urea_acc_time = Setting.UreaAlHr1*3600 - UreaAlarmLapTime;
#else
      urea_acc_time = Setting.UreaAlHr1*60 - UreaAlarmLapTime;
#endif
      if(UreaAlarm.Status) {
//        lcd_puts_big(0,2,0,"  점검바랍니다. ");
#ifdef TIME_HOUR
        lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", urea_acc_time/3600, (urea_acc_time%3600)/60);
#else
        lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", urea_acc_time/60, urea_acc_time%60);//(urea_acc_time%3600)/60);
#endif
        urea_alarm_type = 1;
      }
      else if(UreaAlarm.Quality) {
//        lcd_puts_big(0,2,0,"  교체바랍니다. ");
#ifdef TIME_HOUR
        lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", urea_acc_time/3600, (urea_acc_time%3600)/60);
#else
        lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", urea_acc_time/60, urea_acc_time%60);//(urea_acc_time%3600)/60);
#endif
        urea_alarm_type = 2;
      }
      else if(UreaAlarm.Level) {
        urea_alarm_type = 3;
        lcd_puts_big(0,2,0,"  보충바랍니다. ");
      }
      else if(UreaAlarm.SysErr) {
        urea_alarm_type = 4;
//        lcd_puts_big(0,2,0,"  점검바랍니다. ");
#ifdef TIME_HOUR
        lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", urea_acc_time/3600, (urea_acc_time%3600)/60);
#else
        lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", urea_acc_time/60, urea_acc_time%60);//(urea_acc_time%3600)/60);
#endif
      }
    }
    if(urea_alarm_type == 1)
      lcd_puts_big(0,0,0,"시스템 점검/탈거");
    else if(urea_alarm_type == 2)
      lcd_puts_big(0,0,0,"요소수 품질불량!");
    else if(urea_alarm_type == 3) {
      if(UreaAlarm.Level == 3)
        lcd_big_printf(0,0,0,"요소수Lv %2u%%미만", Setting.UreaLvlAL);
      else if(UreaAlarm.Level == 2)
        lcd_big_printf(0,0,0,"요소수Lv %2u%%미만", Setting.UreaLvlLo);
      else if(UreaAlarm.Level == 1)
        lcd_big_printf(0,0,0,"요소수Lv %3u%%   ", SecData.UreaLevel);
    }
    else if(urea_alarm_type == 4) 
      lcd_big_printf(0,0,0,"시스템에러 %2X,%2X", Gui_SError, Gui_DError);
  }  
}

void Main_Display(void)
{
  //u32 temp;
  u32 UreaAccTime;
  
  switch(Main_Disp_Mode)  {
    case 0: //요소수 레벨, 배터리 전압, 시간... 
      Display_CheckItem();
      if(UreaAlarm.Status || UreaAlarm.Quality || UreaAlarm.Level || UreaAlarm.SysErr) {
        UreaAlarmDisplay();
      }
      else {
        if(Display_Flag || RTC_Val.tm_mon != Gui_RTC_Val.tm_mon || RTC_Val.tm_mday != Gui_RTC_Val.tm_mday || RTC_Val.tm_hour != Gui_RTC_Val.tm_hour || RTC_Val.tm_min != Gui_RTC_Val.tm_min) {
          lcd_printf(62,0,0,"%02u-%02u %02u:%02u", RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min);
          Gui_RTC_Val = RTC_Val;
        }
        if(Display_Flag) {
          lcd_printf(0,1,0,"                          ");
          lcd_puts_big(0,2,0,"요소수 Lv: ");
        }
        if(Display_Flag || SecData.UreaLevel != Gui_UreaLevel) {
            lcd_big_printf(88,2,0,"%3u %%", SecData.UreaLevel);
            Gui_UreaLevel = SecData.UreaLevel;
        }
  //      if(Display_Flag || abs(SecData.Vbat-Gui_BatVolt) > 1) {
  //        temp = SecData.Vbat;
  //        lcd_printf(88,3,0,"%2u.%01u V       ", temp/10, temp%10);
  //        Gui_BatVolt = SecData.Vbat;
  //      }
      }
      break;
    case 1: //요소수 레벨, 농도. 
      if(Display_Flag) {
        lcd_puts_big(0,1,0,"요소수 ");
        lcd_puts_big(56,0,0,"Lv: ");
        lcd_puts_big(56,2,0,"Co: ");
      }
      if(Display_Flag || SecData.UreaLevel != Gui_UreaLevel) {
        lcd_big_printf(88,0,0,"%3u %%", SecData.UreaLevel);
        Gui_UreaLevel = SecData.UreaLevel;
      }
      if(Display_Flag || SecData.Concentration != Gui_Concentration) {
        lcd_big_printf(88,2,0,"%3u %%", SecData.Concentration);
        Gui_Concentration = SecData.Concentration;
      }
      break;
    case 2: //Nox In Out
      if(Display_Flag) {
        lcd_puts_big(0,1,0,"NOx ");
        lcd_puts_big(32,0,0,"In : ");
        lcd_puts_big(32,2,0,"Out: ");
      }
      if(Display_Flag || SecData.NOxIn != Gui_NOxIn) {
        lcd_big_printf(64,0,0,"%4u ppm", SecData.NOxIn);
        Gui_NOxIn = SecData.NOxIn;
      }
      if(Display_Flag || SecData.NOxOut != Gui_NOxOut) {
        lcd_big_printf(64,2,0,"%4u ppm", SecData.NOxOut);
        Gui_NOxOut = SecData.NOxOut;
      }
      break;
    case 3: //Temp In Out 
      if(Display_Flag) {
        lcd_puts_big(0,1,0,"TEMP ");
        lcd_puts_big(40,0,0,"In : ");
        lcd_puts_big(40,2,0,"Out: ");
      }
      if(Display_Flag || SecData.TempIn != Gui_TempIn) {
        lcd_big_printf(80,0,0,"%4u %c", SecData.TempIn, UNIT_C_DEGREE);
        Gui_TempIn = SecData.TempIn;
      }
      if(Display_Flag || SecData.TempOut != Gui_TempOut) {
        lcd_big_printf(80,2,0,"%4u %c", SecData.TempOut, UNIT_C_DEGREE);
        Gui_TempOut = SecData.TempOut;
      }
      break;
//    case 4: //DPF Temp, Pressure  
//      if(Display_Flag) {
//        lcd_puts_big(0,1,0,"DPF ");
//        lcd_puts_big(40,0,0,"T: ");
//        lcd_puts_big(40,2,0,"P: ");
//      }
//      if(Display_Flag || SecData.DpfTemp != Gui_DpfTemp) {
//        lcd_big_printf(56,0,0,"%4u    %c", SecData.DpfTemp, UNIT_C_DEGREE);
//        Gui_DpfTemp = SecData.DpfTemp;
//      }
//      if(Display_Flag || SecData.DpfPress != Gui_DpfPress) {
//        lcd_big_printf(56,2,0,"%4u mbar", SecData.DpfPress);
//        Gui_DpfPress = SecData.DpfPress;
//      }      
//      break;
    case 4: //Dosing Rate, MAF
      if(Display_Flag) {
        lcd_puts_big(0,0,0,"D.RATE: ");
        lcd_puts_big(0,2,0,"MAF   : ");
      }
      if(Display_Flag || SecData.DosingRate != Gui_DosingRate) {
        lcd_big_printf(56,0,0,"%5u g/h", SecData.DosingRate);
        Gui_DosingRate = SecData.DosingRate;
      }
      if(Display_Flag || SecData.MAF != Gui_MAF) {
        lcd_big_printf(56,2,0,"%4u kg/h", SecData.MAF);
        Gui_MAF = SecData.MAF;
      }      
      break;
    case 5: //Limiter In V, Out V
      if(Display_Flag) {
        lcd_puts_big(0,0,0," IN. V : ");
        lcd_puts_big(0,2,0,"OUT. V : ");
      }
      if(Display_Flag || Pedal_ADC[0] != Gui_Pedal[0]) {
        lcd_big_printf(72,0,0,"%1u.%03u V  ", Pedal_ADC[0]/1000, Pedal_ADC[0]%1000);
        Gui_Pedal[0] = Pedal_ADC[0];
      }
      if(Display_Flag || Pedal_ADC[1] != Gui_Pedal[1]) {
        lcd_big_printf(72,2,0,"%1u.%03u V  ", Pedal_ADC[1]/1000, Pedal_ADC[1]%1000);
        Gui_Pedal[1] = Pedal_ADC[1];
      }      
      break;
//    case 6: //State
//      if(Display_Flag) {
//        lcd_puts_big(0,0,0,"State: ");
//        lcd_puts_big(0,2,0,"Error: ");
//        lcd_big_printf(56,0,0,"0x%2X     ", Gui_State);
//        lcd_big_printf(56,0,0,"0x%2X,0x%2X", Gui_SError, Gui_DError);
//      }
//      break;
//      
//    case 7: //배터리 전압, 시간... 
//      Display_CheckItem();
//      if(Display_Flag || RTC_Val.tm_mon != Gui_RTC_Val.tm_mon || RTC_Val.tm_mday != Gui_RTC_Val.tm_mday || RTC_Val.tm_hour != Gui_RTC_Val.tm_hour || RTC_Val.tm_min != Gui_RTC_Val.tm_min) {
//        lcd_printf(62,0,0,"%02u-%02u %02u:%02u", RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min);
//        Gui_RTC_Val = RTC_Val;
//      }
//      if(Display_Flag || abs(SecData.Vbat-Gui_BatVolt) > 1) {
//        temp = SecData.Vbat;
//        lcd_printf(0,1,0,"                          ");
//        lcd_puts_big(0,2,0,"배터리 V: ");
//        lcd_big_printf(80,2,0,"%2u.%01u V", temp/10, temp%10);
//        Gui_BatVolt = SecData.Vbat;
//      }
//      break;
      
//     case 6: //GPS  
//      if(Display_Flag || SecData.LT != Gui_LT) {
//        lcd_printf(0,1,0,"LAT");
//        lcd_big_printf(32,0,0,"%3u.%06u", SecData.LT/1000000, SecData.LT%1000000);
//        Gui_LT = SecData.LT;
//      }
//      if(Display_Flag || SecData.LN != Gui_LN) {
//        lcd_printf(0,3,0,"LON");
//        lcd_big_printf(32,2,0,"%3u.%06u", SecData.LN/1000000, SecData.LN%1000000);
//        Gui_LN = SecData.LN;
//      }
//      break;
    case 6: //속도
      if(Display_Flag || SecData.Speed != Gui_Speed) {
        lcd_big_printf(0,2,0,"Speed");
        lcd_big_printf(50,2,0,"%3u", SecData.Speed);
        lcd_big_printf(75,2,0,"km/h");
        Gui_Speed = SecData.Speed;
      }
      break;
    case 7: //속도
      
#ifdef TIME_HOUR
      UreaAccTime = Setting.UreaAlHr2*3600 - UreaAlarmLapTime;
#else
      UreaAccTime = Setting.UreaAlHr2*60 - UreaAlarmLapTime;
#endif
      if(Display_Flag || UreaAccTime != Gui_UreaTime) {
        if(SecData.UreaAccFlag == 1) {
#ifdef TIME_HOUR
          if(Setting.UreaAlHr2*3600 >= UreaAlarmLapTime) {
            lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", UreaAccTime/3600, (UreaAccTime%3600)/60);
          }
#else
          if(Setting.UreaAlHr2*60 >= UreaAlarmLapTime) {
            lcd_big_printf(0,2,0,"%2u:%2u후 출력제한", UreaAccTime/60, UreaAccTime%60);//(urea_acc_time%3600)/60);
          }
#endif
          else {
            lcd_puts_big(0,2,0,"2단계 예정!     ");
          }
        }
        else if(SecData.UreaAccFlag == 2) {
          lcd_puts_big(0,2,0,"2단계 출력제한중");
        }
        else {
          lcd_puts_big(0,2,0,"정상운행중");
        }
        Gui_UreaTime = UreaAccTime;
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
  * @brief  Key_MenuMode_OK
  * @param  None
  * @retval None
  */
void Key_MenuMode_OK(void)
{
  u8  size;
  u16 addr;
  
  switch(Menu_Disp_Mode) {//- switch SDisplay_Mode(0,3,4,5) start-// 
    case 0: //sub menu index
      Btn_UpDn_Cnt = 0;
      Btn_Right_Cnt = 0;
      Menu_Disp_Mode = Btn_UpDn_Sel + 1; 

      if(Menu_Disp_Mode == 4) {  
        memset(Tmp_Buff[0], 0 ,20);
        addr = DRV_SETTING_PARM + (u8 *)&Setting.Form_Approval_No - (u8 *)&Setting;
        size = sizeof(Setting.Form_Approval_No);
        FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
      }
      else if(Menu_Disp_Mode == 5) {  
        memset(Tmp_Buff[0], 0 ,20);
        addr = DRV_SETTING_PARM + (u8 *)&Setting.Serial_No - (u8 *)&Setting;
        size = sizeof(Setting.Serial_No);
        FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
      }
      else if(Menu_Disp_Mode == 6) {  
        addr = DRV_SETTING_PARM + (u8 *)&Setting.PinCode - (u8 *)&Setting;
        size = sizeof(Setting.PinCode);
        FRAMMultiRead((u8 *)Tmp_Buff[0], addr, size);
      }        
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
      
#ifdef UREA_ACC_TEST_MENU_ENABLE
    case 5: //출력감소 테스트
      PrevUreaAlarm = UreaAlarm;
      UreaTestFlag = abs(Btn_UpDn_Cnt);
      AccTestFlag = 1;
      break;
#endif      
    default : break;
  }          
}

void Key_MenuMode_Cancel(void)
{
  if(Menu_Disp_Mode == 0) {
    Display_Mode = 0;
    Btn_UpDn_Cnt = 0;
  }
  else if(Menu_Disp_Mode == 5) {
    UreaAlarm = PrevUreaAlarm;
    AccTestFlag = 0;
    Display_Mode = 0;
    Btn_UpDn_Cnt = 0;
    Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel;
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
  else
    Key_MenuMode_Cancel();
}

void Key_MenuMode_Right(void)
{
  
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

#ifdef UREA_ACC_TEST_MENU_ENABLE
#define MAX_MENU_TITLE 5
const char MenuTitle[MAX_MENU_TITLE][16] =
{
"다운로드        ",
"다운로드 이력   ",
"프로그램버전    ",
//"형식승인번호    ",
"제품일련번호    ",
//"PIN CODE        ",
//"모뎀정보        ",
//"서버정보        ",
"출력감소 테스트 "
};
#else
#define MAX_MENU_TITLE 8
const char MenuTitle[MAX_MENU_TITLE][16] =
{
"다운로드        ",
"다운로드 이력   ",
"프로그램버전    ",
"형식승인번호    ",
"제품일련번호    ",
"PIN CODE        ",
"모뎀정보        ",
"서버정보        "
};
#endif
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
  
  switch(Menu_Disp_Mode) {
    case 0:
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 1-MAX_MENU_TITLE;  //ISG 메뉴 빼고 15개..
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
      
    case 3: //프로그램버전       
      lcd_big_printf(0,0,0,"FW V%1u.%1u.%1u.%1u", FW_Ver_Key[4]-'L', FW_Ver_Key[5]-'O', FW_Ver_Key[6]-'O', FW_Ver_Key[7]-'P');
#ifdef RELEASE_OUT
      lcd_putc_big(24,0,0,'v');
#endif
      
      if(Setting.UpdateCount) {        
        if(Setting.UpdateCount < 10) 
          lcd_printf(104,1,0,"%d",Setting.UpdateCount);
        else 
          lcd_printf(104,1,0,"?");
      }
      lcd_big_printf(0,2,0,"BL V%1u.%1u T", Setting.FW_Ver[4], Setting.FW_Ver[5]);
      switch(Setting.FW_Type) {
        case EXT_MODEM :
          lcd_big_printf(72,2,0,"11EM");
          break;
        default :  
          lcd_big_printf(72,2,0,"F");
          break;
      } //switch(Setting.FW_Type) {
      break; //case 1    
      
#ifdef UREA_ACC_TEST_MENU_ENABLE
    case 4: //제품일련번호
      Menu_DisplayTitle(0,0,0,12); 
      for(i = 0; i < 14; i++) {
          lcd_putc_big(i*8,2,0, Tmp_Buff[0][i]);
      }
      break;
    case 5:  //알람 출력 감소 테스트
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 0;
      if(Btn_UpDn_Cnt < -2) Btn_UpDn_Cnt = -2;
      lcd_big_printf(0,0,0,"현재알람단계: %1u ", UreaTestFlag);
      if(Btn_UpDn_Cnt == 0)
        lcd_puts_big(0,2,1,"정상 동작       ");
      else if(Btn_UpDn_Cnt == -1)
        lcd_puts_big(0,2,1,"1단계 알람 작동 ");
      else if(Btn_UpDn_Cnt == -2)
        lcd_puts_big(0,2,1,"2단계 알람 작동 ");
      break;
#else
    case 4: //형식승인번호
      Menu_DisplayTitle(0,0,0,11); 
      for(i = 0; i < 10; i++) {
          lcd_putc_big(i*8,2,0,Tmp_Buff[0][i]);
      }
      break;      
      
    case 5: //제품일련번호
      Menu_DisplayTitle(0,0,0,12); 
      for(i = 0; i < 14; i++) {
          lcd_putc_big(i*8,2,0, Tmp_Buff[0][i]);
      }
      break;

    case 6: //PinCode
      Menu_DisplayTitle(0,0,0,13); 
      for(i = 0; i < 6; i++) {
          lcd_putc_big(i*8,2,0,Tmp_Buff[0][i]);
      }      
      break;
      
    case 7: //모뎀정보
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
    case 8:  //서버정보
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
#endif
  }
}


void Read_SettingMode(void)
{
  int i;
  u8  size;
  u16 addr;

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
      addr = DRV_SETTING_PARM + offsetof(st_Setting,UserNumber); 
      FRAMMultiRead((u8 *)&Setting.UserNumber, addr, sizeof(Setting.UserNumber));
      Tmp_Buff[0][0] =  Setting.UserNumber;                  
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 1);                                      
      break;
      
    case 5:
      addr = DRV_SETTING_PARM + offsetof(st_Setting,CanBPS); 
      FRAMMultiRead((u8 *)&Setting.CanBPS, addr, sizeof(Setting.CanBPS));
      Tmp_Buff[0][0] =  Setting.CanBPS&0x0F;                  
      Tmp_Buff[0][1] =  (Setting.CanBPS>>4)&0x0F;                  
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 2);                                      
      break;

    case 6:
      addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaLvlHi - (u8 *)&Setting;
      size = sizeof(Setting.UreaLvlHi) + sizeof(Setting.UreaLvlLo) + sizeof(Setting.UreaLvlAL);
      FRAMMultiRead((u8 *)&Setting.UreaLvlHi, addr, size);
      Tmp_Buff[0][0] = (Setting.UreaLvlHi%100)/10;
      Tmp_Buff[0][1] = Setting.UreaLvlHi%10;

      Tmp_Buff[0][2] = (Setting.UreaLvlLo%100)/10;
      Tmp_Buff[0][3] = Setting.UreaLvlLo%10;

      Tmp_Buff[0][4] = (Setting.UreaLvlAL%100)/10;
      Tmp_Buff[0][5] = Setting.UreaLvlAL%10;

      memcpy(Tmp_Buff[1], Tmp_Buff[0], 6);  
      break;

    case 7:
      addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaConHi - (u8 *)&Setting;
      size = sizeof(Setting.UreaConHi) + sizeof(Setting.UreaConLo);
      FRAMMultiRead((u8 *)&Setting.UreaConHi, addr, size);
      Tmp_Buff[0][0] = (Setting.UreaConHi%100)/10;
      Tmp_Buff[0][1] = Setting.UreaConHi%10;

      Tmp_Buff[0][2] = (Setting.UreaConLo%100)/10;
      Tmp_Buff[0][3] = Setting.UreaConLo%10;

      memcpy(Tmp_Buff[1], Tmp_Buff[0], 4);  
      break;
      
    case 8:
      addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaAlHr1 - (u8 *)&Setting;
      size = sizeof(Setting.UreaAlHr1) + sizeof(Setting.UreaAlHr2) + sizeof(Setting.UreaBuzInt);
      FRAMMultiRead((u8 *)&Setting.UreaAlHr1, addr, size);
      Tmp_Buff[0][0] = (Setting.UreaAlHr1%100)/10;
      Tmp_Buff[0][1] = Setting.UreaAlHr1%10;

      Tmp_Buff[0][2] = (Setting.UreaAlHr2%100)/10;
      Tmp_Buff[0][3] = Setting.UreaAlHr2%10;

      Tmp_Buff[0][4] = (Setting.UreaBuzInt%100)/10;
      Tmp_Buff[0][5] = Setting.UreaBuzInt%10;
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 6);  
      break;
      
    case 9:
      addr = DRV_SETTING_PARM + offsetof(st_Setting,FileOutput); 
      FRAMMultiRead((u8 *)&Setting.FileOutput, addr, sizeof(Setting.FileOutput));
      Tmp_Buff[0][0] =  Setting.FileOutput;                  
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 1);    
      break;
#ifdef UREA_ACC_TEST_MENU_ENABLE
    case 10:                  //initialize //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;
      
    case 11:                  //initialize //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;
      
    case 12: //1.4.0.0
      FRAMMultiRead(Tmp_Buff[0], HS_PASSWORD, 4);
      for(i = 0; i < 4; i++)
        Tmp_Buff[1][i] = Tmp_Buff[0][i];           
      break;
#else
    case 10:
      addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
      FRAMMultiRead((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));
      Tmp_Buff[0][0] =  Setting.ExtModemMode;                  
      memcpy(Tmp_Buff[1], Tmp_Buff[0], 1);                                      
      break;
      
    case 11:
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
      
    case 12:                  //initialize //1.4.0.0
      BLE_PinCode_Generation();
      for(i = 0; i < 6; i++) {
        Tmp_Buff[0][i] = BLE_PinCode[i] + 0x30;
        Tmp_Buff[1][i] = Tmp_Buff[0][i];
      }
      break;
      
    case 13:                  //initialize //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;
      
    case 14:                  //initialize //1.4.0.0
      Tmp_Buff[1][0] = Tmp_Buff[0][0];    
      break;
      
    case 15: //1.4.0.0
      FRAMMultiRead(Tmp_Buff[0], HS_PASSWORD, 4);
      for(i = 0; i < 4; i++)
        Tmp_Buff[1][i] = Tmp_Buff[0][i];           
      break;
#endif
      
    default : break;                
  }//- switch HSDisplay_Mode(1,2,3,4,5,6,7) end-//      
}

void Key_SettingMode_OK(void)
{
  int i,j;
  u8  size;
  u16 addr;
  
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
        for(j = 0; j < 21; j++) 
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
      Setting.UserNumber = Tmp_Buff[1][0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting,UserNumber); 
      FRAMMultiWrite((u8 *)&Setting.UserNumber, addr, sizeof(Setting.UserNumber));
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 1);                                      
      break;
      
    case 5:
      Setting.CanBPS = (Tmp_Buff[1][1]<<4)+Tmp_Buff[1][0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting,CanBPS); 
      FRAMMultiWrite((u8 *)&Setting.CanBPS, addr, sizeof(Setting.CanBPS));
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 2);       
      CAN_Configuration();
      break;

    case 6:
      Setting.UreaLvlHi = Tmp_Buff[1][0]*10+Tmp_Buff[1][1];
      Setting.UreaLvlLo = Tmp_Buff[1][2]*10+Tmp_Buff[1][3];
      Setting.UreaLvlAL = Tmp_Buff[1][4]*10+Tmp_Buff[1][5];
      addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaLvlHi - (u8 *)&Setting;
      size = sizeof(Setting.UreaLvlHi) + sizeof(Setting.UreaLvlLo) + sizeof(Setting.UreaLvlAL);
      FRAMMultiWrite((u8 *)&Setting.UreaLvlHi, addr, size);
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 6);  
      break;

    case 7:
      Setting.UreaConHi = Tmp_Buff[1][0]*10+Tmp_Buff[1][1];
      Setting.UreaConLo = Tmp_Buff[1][2]*10+Tmp_Buff[1][3];
      addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaConHi - (u8 *)&Setting;
      size = sizeof(Setting.UreaConHi) + sizeof(Setting.UreaConLo);
      FRAMMultiWrite((u8 *)&Setting.UreaConHi, addr, size);
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 4);  
      break;
      
    case 8:
      Setting.UreaAlHr1 = Tmp_Buff[1][0]*10+Tmp_Buff[1][1];
      Setting.UreaAlHr2 = Tmp_Buff[1][2]*10+Tmp_Buff[1][3];
      Setting.UreaBuzInt = Tmp_Buff[1][4]*10+Tmp_Buff[1][5];
      addr = DRV_SETTING_PARM + (u8 *)&Setting.UreaAlHr1 - (u8 *)&Setting;
      size = sizeof(Setting.UreaAlHr1) + sizeof(Setting.UreaAlHr2) + sizeof(Setting.UreaBuzInt);
      FRAMMultiWrite((u8 *)&Setting.UreaAlHr1, addr, size);
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 6);        
      break;

    case 9:
      Setting.FileOutput = Tmp_Buff[1][0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting,FileOutput); 
      FRAMMultiWrite((u8 *)&Setting.FileOutput, addr, sizeof(Setting.FileOutput));     
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 1);
      break;
#ifdef UREA_ACC_TEST_MENU_ENABLE
    case 10: //Firmware Update
      Tmp_Buff[0][0] = Tmp_Buff[1][0];  
      if(USB_Detect_Flag) {
        lcd_puts_big(0,2,0,"                "); //init
        if(Tmp_Buff[1][0] == 1) 
          USB_FirmwareUpdate_Flag = 1;
      }
      break; 
      
    case 11: //initialize //1.4.0.0      
      if(Tmp_Buff[1][0] == 1) {
        Modem_Parameter_Reset();
        NVIC_SystemReset();                
      }             
      else if(Tmp_Buff[1][0] == 2) {
        Parameter_Reset();
        NVIC_SystemReset();                
      }             
      Tmp_Buff[0][0] = Tmp_Buff[1][0];    
      break;    

    case 12: //1.4.0.0
      FRAMMultiWrite(Tmp_Buff[1], HS_PASSWORD, 4);
      for(i = 0; i < 4; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];            
      break;
#else
    case 10:
      Setting.ExtModemMode = Tmp_Buff[1][0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode); 
      FRAMMultiWrite((u8 *)&Setting.ExtModemMode, addr, sizeof(Setting.ExtModemMode));     
      memcpy(Tmp_Buff[0], Tmp_Buff[1], 1);
      break;
      
    case 11:
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
          
    case 12: 
      size = sizeof(Setting.PinCode);
      memcpy(Setting.PinCode, Tmp_Buff[1], size); //1.4.0.0
      addr = DRV_SETTING_PARM + (u8 *)&Setting.PinCode - (u8 *)&Setting;
      FRAMMultiWrite((u8 *)Tmp_Buff[1], addr, size);
      for(i = 0; i < size; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];
      break;
      
    case 13: //Firmware Update
      Tmp_Buff[0][0] = Tmp_Buff[1][0];  
      if(USB_Detect_Flag) {
        lcd_puts_big(0,2,0,"                "); //init
        if(Tmp_Buff[1][0] == 1) 
          USB_FirmwareUpdate_Flag = 1;
      }
      break; 
      
    case 14: //initialize //1.4.0.0      
      if(Tmp_Buff[1][0] == 1) {
        Modem_Parameter_Reset();
        NVIC_SystemReset();                
      }             
      else if(Tmp_Buff[1][0] == 2) {
        Parameter_Reset();
        NVIC_SystemReset();                
      }             
      Tmp_Buff[0][0] = Tmp_Buff[1][0];    
      break;    

    case 15: //1.4.0.0
      FRAMMultiWrite(Tmp_Buff[1], HS_PASSWORD, 4);
      for(i = 0; i < 4; i++)
        Tmp_Buff[0][i] = Tmp_Buff[1][i];            
      break;      
#endif
          
  }//- switch HSDisplay_Mode(0,1,2,3,4,5,6,7) end-// 
        
  if(Setting_Disp_Mode != 0) {
    Btn_UpDn_Cnt = 0;
    Btn_Right_Cnt = 0; 
  }
}

void Key_SettingMode_Cancel(void)
{
  if(Setting_Disp_Mode < 3) {       
    if(Setting_Disp_Mode == 6) {
      Display_Mode = 2;
      Setting_Disp_Mode = 4; 
      Read_SettingMode();       
    }
    else {
      memset(Tmp_Buff[0],0,20);
      memset(Tmp_Buff[1],0,20);
      memset(Tmp_Buff[2],0,20);
      Display_Mode = 0;
      Setting_Disp_Mode = 0;
    }
    Btn_Right_Cnt = 0;
    Btn_UpDn_Cnt = 0;
  }  
  else {
    Setting_Disp_Mode = 2;  
    Btn_UpDn_Cnt = (-1)*Btn_UpDn_Sel; 
    Disp_GSen_Val = 0;
  }
  Btn_Right_Cnt = 0;
}

#ifdef UREA_ACC_TEST_MENU_ENABLE
const unsigned char MaxSettingRightCnt[24] = {
  4, 1, 1, 5, 1, 2, 6, 4,  
  6, 1, 1, 1, 4, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0};
#else
const unsigned char MaxSettingRightCnt[24] = {
  4, 1, 1, 5, 1, 2, 6, 4,  
  6, 1, 1, 17, 6, 1, 1, 0, 
  0, 0, 0, 0, 0, 0, 0, 0};
#endif

void Key_SettingMode_Left(void) 
{
#ifdef UREA_ACC_TEST_MENU_ENABLE
  if(Setting_Disp_Mode > 12) return;
#else
  if(Setting_Disp_Mode > 14) return;
#endif
  
  if(Setting_Disp_Mode == 2)   {
    Key_SettingMode_Cancel();
  }
  else if(MaxSettingRightCnt[Setting_Disp_Mode] > 1) { 
    if(Btn_Right_Cnt > 0) {
      Btn_Right_Old = Btn_Right_Cnt;
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
#ifdef UREA_ACC_TEST_MENU_ENABLE
  if(Setting_Disp_Mode > 12) return;
#else
  if(Setting_Disp_Mode > 14) return;
#endif
  
  if(Setting_Disp_Mode == 2)   {
    Key_SettingMode_OK();
  }
  else if(MaxSettingRightCnt[Setting_Disp_Mode] > 1) { 
    if(Btn_Right_Cnt < MaxSettingRightCnt[Setting_Disp_Mode]-1) { 
      Btn_Right_Old = Btn_Right_Cnt;
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
  if(USB_FirmwareUpdate_Flag) return;
  if(USB_FirmwareUpdate_Complete) {
    USB_FirmwareUpdate_Complete = 0;
    Key_SettingMode_Cancel();
    Display_Flag = 1;
    return;
  }
  
  switch(Key_Val) {  
    case KEY_OK:      
      Key_SettingMode_Right();
      if(!USB_FirmwareUpdate_Flag)
        Display_Flag = 1;
      break;
      
    case (KEY_OK|KEY_LONG):      
      Key_SettingMode_OK();
      if(!USB_FirmwareUpdate_Flag)
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
#ifdef UREA_ACC_TEST_MENU_ENABLE
#define MAX_SETTING_TITLE 10
const char SettingTitle[MAX_SETTING_TITLE][16] =
{
"날짜/시간설정   ",
"사용자설정      ",
"CAN수신속도     ",
"UREA레벨알람    ",
"UREA농도알람    ",
"UREA알람시간    ",
"파일출력        ",
"펌웨어 업데이트 ",
"제품초기화      ",
"비밀번호 변경   "
};
#else
#define MAX_SETTING_TITLE 13
const char SettingTitle[MAX_SETTING_TITLE][16] =
{
"날짜/시간설정   ",
"사용자설정      ",
"CAN수신속도     ",
"UREA레벨알람    ",
"UREA농도알람    ",
"UREA알람시간    ",
"파일출력        ",
"모뎀정보        ",
"서버정보        ",
"PIN CODE 설정   ",
"펌웨어 업데이트 ",
"제품초기화      ",
"비밀번호 변경   "
};
#endif

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
      if(Btn_UpDn_Cnt > 0 ) Btn_UpDn_Cnt = 1-MAX_SETTING_TITLE;
      if(Btn_UpDn_Cnt < 1-MAX_SETTING_TITLE) Btn_UpDn_Cnt = 0; //2.2
      if(Btn_UpDn_Cnt == 0)  {
          idx = 0;
          Setting_DisplayTitle(0,0,1,idx);
          Setting_DisplayTitle(0,2,0,idx+1);
          Btn_UpDn_Sel = idx;
      }
      else if(Btn_UpDn_Cnt == MAX_SETTING_TITLE)  { //2.2
          idx = MAX_SETTING_TITLE-1; //2.2
          Setting_DisplayTitle(0,0,1,idx);
//          Setting_DisplayTitle(0,2,1,idx+1);
          Btn_UpDn_Sel = idx;// + 1;          
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
      
    case 4: 
      Setting_DisplayTitle(0,0,0,1); 
      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 7-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 7-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "NONE            ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "일진 35L        ");        
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "일진 50L        ");    
      else if(Tmp_Buff[1][0] == 3)
        lcd_puts_big(0,2,1, "이엔드디        ");    
      else if(Tmp_Buff[1][0] == 4)
        lcd_puts_big(0,2,1, "이엔드디 GEN3   ");    
      else if(Tmp_Buff[1][0] == 5)
        lcd_puts_big(0,2,1, "세라컴          ");    
      else if(Tmp_Buff[1][0] == 6)
        lcd_puts_big(0,2,1, "클린어스        ");    
      else if(Tmp_Buff[1][0] == 7)
        lcd_puts_big(0,2,1, "클린어스 SDS    ");    
      break;

    case 5: 
      if(Btn_Right_Cnt > 1) Btn_Right_Cnt = 1;
      UpDnRolling(2,Tmp_Buff[0][Btn_Right_Cnt]);
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_puts_big(0,0,0," CAN1    CAN2   ");
      lcd_puts_big(0,2,0,"                ");
      if(Btn_Right_Cnt == 0) {
        if(!Tmp_Buff[1][0]) lcd_puts_big(0,2,1,"250kbps");
        else   lcd_puts_big(0,2,1,"500kbps");
        if(!Tmp_Buff[1][1]) lcd_puts_big(64,2,0,"500kbps");
        else   lcd_puts_big(64,2,0,"250kbps");
      }
      else {
        if(!Tmp_Buff[1][0]) lcd_puts_big(0,2,0,"250kbps");
        else   lcd_puts_big(0,2,0,"500kbps");
        if(!Tmp_Buff[1][1]) lcd_puts_big(64,2,1,"500kbps");
        else   lcd_puts_big(64,2,1,"250kbps");
      }
      break;      
      
    case 6: 
      if(Btn_Right_Cnt > 5) Btn_Right_Cnt = 5;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_puts_big(0,0,0," LV1  LV2  LV3  ");
      lcd_puts_big(0,2,0,"   %    %    %  ");
      for(i = 0; i < 6; i++)
        lcd_putc_big((i/2)*40+(i%2)*8+8,2,0,Tmp_Buff[1][i]+0x30);
      lcd_putc_big((Btn_Right_Cnt/2)*40+(Btn_Right_Cnt%2)*8+8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
      break;      
      
    case 7: 
      if(Btn_Right_Cnt > 3) Btn_Right_Cnt = 3;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_puts_big(0,0,0," HIGH    LOW    ");
      lcd_puts_big(0,2,0,"   %       %    ");
      for(i = 0; i < 4; i++)
        lcd_putc_big((i/2)*64+(i%2)*8+8,2,0,Tmp_Buff[1][i]+0x30);
      lcd_putc_big((Btn_Right_Cnt/2)*64+(Btn_Right_Cnt%2)*8+8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
      break;      

    case 8: 
      if(Btn_Right_Cnt > 5) Btn_Right_Cnt = 5;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 9-(int)Tmp_Buff[0][Btn_Right_Cnt];
      if(Btn_UpDn_Cnt > 9-(int)Tmp_Buff[0][Btn_Right_Cnt]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][Btn_Right_Cnt];
      Tmp_Buff[1][Btn_Right_Cnt] = Tmp_Buff[0][Btn_Right_Cnt]+Btn_UpDn_Cnt;
      lcd_puts_big(0,0,0," 1차A 2차A 부저 ");
#ifdef TIME_HOUR
      lcd_puts_big(0,2,0,"   h    h    m  ");
#else
      lcd_puts_big(0,2,0,"   m    m    m  ");
#endif
      for(i = 0; i < 6; i++)
        lcd_putc_big((i/2)*40+(i%2)*8+8,2,0,Tmp_Buff[1][i]+0x30);
      lcd_putc_big((Btn_Right_Cnt/2)*40+(Btn_Right_Cnt%2)*8+8,2,1,Tmp_Buff[1][Btn_Right_Cnt]+0x30);
      break;      
     
    case 9: 
      Setting_DisplayTitle(0,0,0,6); 
      Btn_Right_Cnt = 0;
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 1-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 1-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "XLS(Text File)  ");        
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "DTE(Binary File)");    
      
      break;

#ifdef UREA_ACC_TEST_MENU_ENABLE
    case 10: //Firmware update
      Setting_DisplayTitle(0,0,0,7); 
      
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
      
    case 11: //Initialize //1.4.0.0
      Setting_DisplayTitle(0,0,0,8); 
      
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 2-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 2-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "No");   
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Modem Reset");   
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "Factory Reset");   
                
      break;      
      
    case 12: //set password //1.4.0.0
      Setting_DisplayTitle(0,0,0,9); 
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
#else
    case 10: 
      Setting_DisplayTitle(0,0,0,7); 
      Btn_Right_Cnt = 0;
      Tmp_Buff[1][0] = 0;
//      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
//        Btn_UpDn_Cnt = 2-(int)Tmp_Buff[0][0];
//      if(Btn_UpDn_Cnt > 2-(int)Tmp_Buff[0][0]) 
//        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
//      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "LOOP            ");        
//      else if(Tmp_Buff[1][0] == 1)
//        lcd_puts_big(0,2,1, "Hanuri          ");       
//      else if(Tmp_Buff[1][0] == 2)
//        lcd_puts_big(0,2,1, "KT_GVN          ");   
      
      break;
    case 11:
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

    case 12: //PINCODE  설정
      if(Btn_Right_Cnt > 6) Btn_Right_Cnt = 6;  //6 자리
      Setting_DisplayTitle(0,0,0,9); 
      
      for(i = 0; i < 6; i++) {
        if((Tmp_Buff[1][i] < 0x30 && Tmp_Buff[1][i] > 0x39) || Tmp_Buff[1][i] == 0)
          lcd_putc_big(i*8,2,0,0x30);
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
      
    case 13: //Firmware update
      Setting_DisplayTitle(0,0,0,10); 
      
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
      
    case 14: //Initialize //1.4.0.0
      Setting_DisplayTitle(0,0,0,11); 
      
      if(Btn_UpDn_Cnt < 0-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 2-(int)Tmp_Buff[0][0];
      if(Btn_UpDn_Cnt > 2-(int)Tmp_Buff[0][0]) 
        Btn_UpDn_Cnt = 0-(int)Tmp_Buff[0][0];
      Tmp_Buff[1][0] = Tmp_Buff[0][0]+Btn_UpDn_Cnt;
      if(Tmp_Buff[1][0] == 0)
        lcd_puts_big(0,2,1, "No");   
      else if(Tmp_Buff[1][0] == 1)
        lcd_puts_big(0,2,1, "Modem Reset");   
      else if(Tmp_Buff[1][0] == 2)
        lcd_puts_big(0,2,1, "Factory Reset");   
                
      break;      
      
    case 15: //set password //1.4.0.0
      Setting_DisplayTitle(0,0,0,12); 
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
#endif
      
    default : break;
  }//- switch HSDisplay_Mode
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

void Display(void)
{

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
  
    //- switch Display_Mode end -// 
    switch(Display_Mode) {  
      case 0: //MainDisp
        if(!Display_Flag) {
            Main_Display();
        }
        break;
      case 1: //Menu
        break;
      case 2: //Setting
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
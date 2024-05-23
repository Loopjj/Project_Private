/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : usbuser.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/05/2017
*
* Description        : user program for usb 
*
*********************************************************************************************************
*/
#define USBUSER_GLOBALS

#include "includes.h"

extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

extern USBH_HandleTypeDef hUsbHostFS;
extern ApplicationTypeDef Appli_state;
extern void MX_USB_HOST_Process(void); 

/* Flags */
u8 USB_Error_Flag;
u8 USB_Cancel_Flag;

u8 TmpBuff[256];
char ParamFileName[20];
/**
  * @brief  USBH_USR_MSC_Application
  * @param  None
  * @retval Staus
  */
//extern static BYTE check_fs (FATFS *fs,WORD sect);
void USB_DriverFileUpdate(void)
{
  st_DRIVER_INFO driver;
  u32 rcnt;
  int i;
  u8 item = 0, dflag;
  u8 rdata[10], idx;
  
  if(f_findfirst(&Ddir, &Finfo, "", "*.dif") == FR_OK) {  /* Start to search for JPEG files with the name started by "dsc" */
    if(Finfo.fsize >= sizeof(st_DRIVER_INFO) && Finfo.fsize < 256) {
      if (f_open(&USBHFile, Finfo.fname, FA_READ) == FR_OK) {
        f_read (&USBHFile, TmpBuff, Finfo.fsize, (void *)&rcnt); 
        f_close(&USBHFile);
        memset((u8*)&driver, 0, sizeof(st_DRIVER_INFO));
        if(rcnt > 0) {
          for(i = 0; i < rcnt; i++) {
            if(IsSameString("#이름:",(char*)&TmpBuff[i],6)) item = 1;
            else if(IsSameString("#코드:",(char*)&TmpBuff[i],6)) item = 2;
            else if(IsSameString("#사번:",(char*)&TmpBuff[i],6)) item = 3;
            if(TmpBuff[i] == '[') {
              dflag = 1; 
              idx = 0;
              memset(rdata, 0,10);
            }
            else if(dflag) {
              if(TmpBuff[i] == ']') {
                if(item == 1 && idx > 0 && idx <= 10) 
                  memcpy(driver.Name,rdata,idx);
                else if(item == 2 && idx > 0 && idx <= 10) 
                  memcpy(driver.Code,rdata,idx);
                else if(item == 3 && idx == 7) { 
                  memcpy(driver.Num,rdata,idx);
                  if(IsSameString("9999999", (char*)driver.Num, 7)) {
                    VehMaintenanceFlag = 1;
                    DriverNameDispCnt = 0;
                    DriverNameDispFlag = 0;
                  }
                  else {
                    Add_New_Driver(&driver, 1, 1);
                    DriverNameDispFlag = 1;
                    DriverNameDispCnt = 0;
                  }
                  break;
                }
                dflag = 0;
              }
              else if(TmpBuff[i] >= '0'){
                if(idx < 10) rdata[idx++] = TmpBuff[i];
              }
              else dflag = 0;
            }
          }
        }
      }
    }
  }
  f_closedir(&Ddir);     
}

char DrvVehFileName[32];
void USB_DrvVehFileUpdate(void)
{
  u8 len, idx;
  u32 bytesWritten;
  rtc_time dtime;
  
  if(Selected_Driver < 9) {
    len = StringLen((char*)Driver_Info[Selected_Driver].Num,7); 
    if(len > 0 && len < 8) {
      memset(DrvVehFileName, 0, 32);
      memcpy(DrvVehFileName, (char*)Driver_Info[Selected_Driver].Num, len);
      if(TripStartTime > 0)
        dtime = gmtime_conv(TripStartTime);
      else
        dtime = RTC_Val;
      sprintf(&DrvVehFileName[len],"_%02u%02u.dvf",dtime.tm_year, dtime.tm_mon);
      if(f_open(&USBHFile, DrvVehFileName, FA_WRITE|FA_OPEN_APPEND) == FR_OK) {
        memset(TmpBuff, 0, sizeof(TmpBuff)); 
        if(USB_DrvVeh_Mode == 0) {
          sprintf((char*)TmpBuff,"START,");
          idx = 6;
        }
        else {
          sprintf((char*)TmpBuff,"END,");
          idx = 4;
        }
        len = StringLen((char*)VRN,12);
        memcpy(&TmpBuff[idx], VRN, len);
        idx += len;
        sprintf((char*)&TmpBuff[idx],",%02u/%02u/%02u,%02u:%02u:%02u\r\n",RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec);
        idx = StringLen((char*)TmpBuff,200);
        f_write(&USBHFile, TmpBuff, idx, &bytesWritten);
        /* Close file */
        f_close(&USBHFile);
      }  
    }
  }
  if(Power_Off_Flag)   Power_Off();  
}

#define USER_PROGRAM_MAX_SIZE (0x80000-0x8000)
#define USER_PROGRAM_MIN_SIZE 0x100

void Check_FirmwareUpdate(void)
{
  u32 val0, jump = 0;
  
  //- Upgrade File Detected -//
  if(f_stat(DOWNLOAD_FILENAME, &Finfo) == FR_OK) {
    jump = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);
    val0 = ((u32)Finfo.fdate*0x10000) + (u32)Finfo.ftime;
    if(jump > 0 && jump < 10) {
      FRAMMultiWrite((u8 *)&val0, FIRMWARE_DATE, 4);  
      HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0);
    }
  }
}

void USB_FirmwareUpdate(void)
{
  u32 val0, val1, jump = 0;
  
  //- Upgrade File Detected -//
  if(f_stat(DOWNLOAD_FILENAME, &Finfo) == FR_OK) {
    jump = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);
    val0 = ((u32)Finfo.fdate*0x10000) + (u32)Finfo.ftime;

    if(jump > 0 && jump < 10) {
      FRAMMultiWrite((u8 *)&val0, FIRMWARE_DATE, 4);  
      HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0);
      lcd_puts_big(0,2,0,"Update OK       "); 
    }
    else {
      FRAMMultiRead((u8 *)&val1, FIRMWARE_DATE, 4);  
      if(val0 != val1) { //1.4.0.0
//        IT_Configuration(DISABLE);
        Parameter_Save();
        //HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 1);  //0x50524F47);   //PROG    0x4A554D50); // JUMP
        lcd_clear();
        lcd_printf(4,1,0,"Found New Ver");
        lcd_printf(4,2,0,"System Reset");   
#if 1        
        u32 startCode, flen = 0;
        u16 bytesRead, file_crc;//, read_crc;
        u32 i, n, mod, nand_addr;
        u8 error = 0;
  
        nand_addr = NAND_PROGRAM_ADDR;
        memset(NandTmpBuffer, 0, NAND_PAGE_SIZE);
        if (f_open(&USBHFile, DOWNLOAD_FILENAME, FA_READ) == FR_OK) {
          flen = f_size(&USBHFile);  //TmpFile.fsize;
          if(flen > USER_PROGRAM_MIN_SIZE &&  flen < USER_PROGRAM_MAX_SIZE) {
            n = flen/NAND_PAGE_SIZE;
            mod = flen%NAND_PAGE_SIZE;
            file_crc = 0;
            for(i = 0; i < n; i++) {
              IWDG_ReloadCounter(); 
              if(USBH_MSC_IsReady(&hUsbHostFS) == 0) return;
              if(f_read (&USBHFile, NandTmpBuffer, NAND_PAGE_SIZE, (void *)&bytesRead) == FR_OK) {
                if(bytesRead != NAND_PAGE_SIZE) { 
                  error = 1;
                  break;
                }
                if(i == 0) {
                  memcpy((u8*)&startCode, NandTmpBuffer, 4);
                  if ((startCode & 0x2FFE0000 ) != 0x20000000) {
                    error = 1;
                    break;
                  }
                }
                IWDG_ReloadCounter();
                Nand_Write_Buffer(NandTmpBuffer, nand_addr, NAND_PAGE_SIZE); 
                file_crc = getcrc16(file_crc, NandTmpBuffer, NAND_PAGE_SIZE);
                nand_addr += NAND_PAGE_SIZE;
                memset(NandTmpBuffer, 0, NAND_PAGE_SIZE);
              }
              else {
                error = 1;
                break;
              }
            }
            IWDG_ReloadCounter();
            if(mod > 0 && error == 0) {
              if(f_read (&USBHFile, NandTmpBuffer, mod, (void *)&bytesRead) == FR_OK) {
                if(bytesRead != mod) { 
                  error = 1;
                }
                else {
                  Nand_Write_Buffer(NandTmpBuffer, nand_addr, mod); 
                  file_crc = getcrc16(file_crc, NandTmpBuffer, mod);
                  nand_addr += mod;
                }
              }
            }
            /* nand inspection
            if(nand_addr - NAND_PROGRAM_ADDR == flen) {
              nand_addr = NAND_PROGRAM_ADDR;
              read_crc = 0;
              for(i = 0; i < n; i++) {
                IWDG_ReloadCounter(); 
                memset(NandTmpBuffer, 0, NAND_PAGE_SIZE);
                Nand_Read_Buffer(NandTmpBuffer, nand_addr, NAND_PAGE_SIZE); 
                read_crc = getcrc16(read_crc, NandTmpBuffer, NAND_PAGE_SIZE);
                nand_addr += NAND_PAGE_SIZE;
              }
              if(mod > 0) {
                IWDG_ReloadCounter(); 
                memset(NandTmpBuffer, 0, NAND_PAGE_SIZE);
                Nand_Read_Buffer(NandTmpBuffer, nand_addr, mod); 
                read_crc = getcrc16(read_crc, NandTmpBuffer, mod);
              }
              if(read_crc != file_crc)
                error = 1;
            }
            else 
              error = 1;
            */
          }
          else
            error = 1;
          f_close (&USBHFile);
        }     
        if(error == 0 && flen > USER_PROGRAM_MIN_SIZE) {
          //flen = flen+(4-(flen%4));
          HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 10);  //0x50524F47);   //PROG    0x4A554D50); // JUMP
          HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, flen);  //0x50524F47);   //PROG    0x4A554D50); // JUMP
          FRAMMultiWrite((u8 *)&val0, FIRMWARE_DATE, 4);  
        }         
#endif        
        IWDG_ReloadCounter();
        delay_ms(2000);
        NVIC_SystemReset();
      }
      else {
        lcd_puts_big(0,2,0,"Same Firmware   ");
      }
    }
  }
  else {
    lcd_puts_big(0,2,0,"File Not Found  ");
  }
}

void USB_ParamUpload(void)
{
  u8 reset_flag = 0, param_flag = 0, file_ok = 0;
  u16 rcnt, diff_cnt = 0;
  u16 i;

  st_Setting *hd;
  if (f_open(&USBHFile, ParamFileName, FA_READ) == FR_OK) file_ok = 1;
  else if (f_open(&USBHFile, PARAMETER_FILENAME, FA_READ) == FR_OK) file_ok = 1;
  
  if (file_ok) {
    if (f_size(&USBHFile) == 256) {  
      f_read (&USBHFile, TmpBuff, 256, (void *)&rcnt);   
      f_close(&USBHFile);
      hd = (st_Setting*)TmpBuff;
      if(hd->Drv_Name[0] != 0xff) {
        diff_cnt = 0;
        for(i = 0; i < sizeof(Setting.Drv_Name); i++)
          if(Setting.Drv_Name[i] != hd->Drv_Name[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(Setting.Drv_Name,hd->Drv_Name,sizeof(Setting.Drv_Name));
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Drv_Code[0] != 0xff) {
        diff_cnt = 0;
        for(i = 0; i < sizeof(Setting.Drv_Code); i++)
          if(Setting.Drv_Code[i] != hd->Drv_Code[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(Setting.Drv_Code,hd->Drv_Code,sizeof(Setting.Drv_Code));
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Car_Reg_No[0] != 0xff) {
        if(Setting.Car_Reg_No[0] != hd->Car_Reg_No[0]) {
          Setting.Car_Reg_No[0] = hd->Car_Reg_No[0];
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Car_Reg_No[1] != 0xff) {
        diff_cnt = 0;
        for(i = 1; i < sizeof(Setting.Car_Reg_No); i++)
          if(Setting.Car_Reg_No[i] != hd->Car_Reg_No[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(&Setting.Car_Reg_No[1],&hd->Car_Reg_No[1],sizeof(Setting.Car_Reg_No)-1);
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Car_No[0] != 0xff) {
        diff_cnt = 0;
        for(i = 0; i < sizeof(Setting.Car_No); i++)
          if(Setting.Car_No[i] != hd->Car_No[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(Setting.Car_No,hd->Car_No,sizeof(Setting.Car_No));
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Company_Name[0] != 0xff) {
        diff_cnt = 0;
        for(i = 0; i < sizeof(Setting.Company_Name); i++)
          if(Setting.Company_Name[i] != hd->Company_Name[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(Setting.Company_Name,hd->Company_Name,sizeof(Setting.Company_Name));
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Company_Reg_No[0] != 0xff) {
        diff_cnt = 0;
        for(i = 0; i < sizeof(Setting.Company_Reg_No); i++)
          if(Setting.Company_Reg_No[i] != hd->Company_Reg_No[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(Setting.Company_Reg_No,hd->Company_Reg_No,sizeof(Setting.Company_Reg_No));
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Form_Approval_No[0] != 0xff) {
        diff_cnt = 0;
        for(i = 0; i < sizeof(Setting.Form_Approval_No); i++)
          if(Setting.Form_Approval_No[i] != hd->Form_Approval_No[i]) diff_cnt++;
        if(diff_cnt) {
          memcpy(Setting.Form_Approval_No,hd->Form_Approval_No,sizeof(Setting.Form_Approval_No));
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->K_Factor != 0xffff) {
        if(Setting.K_Factor != hd->K_Factor) {
          Setting.K_Factor = hd->K_Factor;
          param_flag = 1;
        }
      }
      if(hd->RPM_Factor != 0xffff) {
        if(Setting.RPM_Factor != hd->RPM_Factor) {
          Setting.RPM_Factor = hd->RPM_Factor;
          param_flag = 1;
        }
      }
      if(hd->FW_Type != 0xff && hd->FW_Type > 0) {
        if(Setting.FW_Type != hd->FW_Type) {
          Setting.FW_Type = hd->FW_Type;
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Set_Direction != 0xff) {
        if(Setting.Set_Direction != hd->Set_Direction) {
          Setting.Set_Direction = hd->Set_Direction;
          param_flag = 1;
        }
      }
      if(hd->Can_Enable != 0xff) {
        if(Setting.Can_Enable != hd->Can_Enable) {
          Setting.Can_Enable = hd->Can_Enable;
          param_flag = 1;
        }
      }
      if(hd->Can_Type != 0xff) {
        if(Setting.Can_Type != hd->Can_Type) {
          Setting.Can_Type = hd->Can_Type;
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->Brake_Type != 0xff) {
        if(Setting.Brake_Type != hd->Brake_Type) {
          Setting.Brake_Type = hd->Brake_Type;
          param_flag = 1; 
        }
      }
      if(hd->CanProtocol != 0xff) {
        if(Setting.CanProtocol != hd->CanProtocol) {
          Setting.CanProtocol = hd->CanProtocol;
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->CanBaudrate != 0xff) {
        if(Setting.CanBaudrate != hd->CanBaudrate) {
          Setting.CanBaudrate = hd->CanBaudrate;
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->CanMode != 0xff) {
        if(Setting.CanMode != hd->CanMode) {
          Setting.CanMode = hd->CanMode;
          param_flag = 1; reset_flag = 1;
        }
      }
      if(hd->ODO_Offeset != 0xffffffff) {
        if(Setting.ODO_Offeset != hd->ODO_Offeset) {
          Setting.ODO_Offeset = hd->ODO_Offeset;
          DailyData.TTrip = (double)hd->ODO_Offeset;
          FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
          Val.TTrip = DailyData.TTrip;
          Val.TTripOld = Val.TTrip;
          param_flag = 1;
        }
      }
      if(hd->Fuel_Factor > 0 && hd->Fuel_Factor < 1000) {
        if(Setting.Fuel_Factor != hd->Fuel_Factor) {
          Setting.Fuel_Factor = hd->Fuel_Factor;
          Calib_F_Factor = hd->Fuel_Factor;
          param_flag = 1;
          FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);                
        }
      }              
      if(hd->K_Factor_Percent > 0 && hd->K_Factor_Percent < 0xff) {
        if(Setting.K_Factor_Percent != hd->K_Factor_Percent) {
          Setting.K_Factor_Percent = hd->K_Factor_Percent;
          Setting.K_Factor = Setting.K_Factor*100/hd->K_Factor_Percent;                
          param_flag = 1;
        }
      }
      if(hd->RPM_Factor_Percent > 0 && hd->RPM_Factor_Percent < 0xff) {
        if(Setting.RPM_Factor_Percent != hd->RPM_Factor_Percent) {
          Setting.RPM_Factor_Percent = hd->RPM_Factor_Percent;
          Setting.RPM_Factor = Setting.RPM_Factor*hd->RPM_Factor_Percent/100;           
          param_flag = 1;
        }
      }
      if(hd->Fuel_Factor_Percent > 0 && hd->Fuel_Factor_Percent < 0xff) {
        if(Setting.Fuel_Factor_Percent != hd->Fuel_Factor_Percent) {
          Setting.Fuel_Factor_Percent = hd->Fuel_Factor_Percent;
          Calib_F_Factor = Calib_F_Factor*100/hd->Fuel_Factor_Percent;
          Setting.Fuel_Factor = Calib_F_Factor;
          param_flag = 1;
          FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);                
        }
      }
      if(hd->ExtModemMode > 0 && hd->ExtModemMode < 0xff) {
        if(Setting.ExtModemMode != hd->ExtModemMode) {
          Setting.ExtModemMode = hd->ExtModemMode;
          param_flag = 1; reset_flag = 1;
        }
      }
//      if(hd->ExtModemTelecom > 0 && hd->ExtModemTelecom < 0xff) {
//        if(Setting.ExtModemTelecom != hd->ExtModemTelecom) {
//          Setting.ExtModemTelecom = hd->ExtModemTelecom;
//          param_flag = 1;
//        }
//      }
//      if(hd->ExtModemModel > 0 && hd->ExtModemModel < 0xff) {
//        if(Setting.ExtModemModel != hd->ExtModemModel) {
//          Setting.ExtModemModel = hd->ExtModemModel;
//          param_flag = 1;
//        }
//      }
      if(hd->FileOutput > 0 && hd->FileOutput < 0xff) {
        if(Setting.FileOutput != hd->FileOutput) {
          Setting.FileOutput = hd->FileOutput;
          param_flag = 1;
        }
      }
      if(hd->ExtModemVDIS > 0 && hd->ExtModemVDIS < 0xff) {
        if(Setting.ExtModemVDIS != hd->ExtModemVDIS) {
          Setting.ExtModemVDIS = hd->ExtModemVDIS;
          param_flag = 1; reset_flag = 1;
        }
      }
      if(param_flag) 
        FRAMMultiWrite((u8 *)&Setting, DRV_SETTING_PARM, sizeof(Setting));
      if(reset_flag) {
        Parameter_Save();
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0);  
        lcd_clear();
        lcd_puts_big(4,0,0,"Found Parameter");
        lcd_puts_big(4,2,0,"System Reset");   
        vTaskDelay(2000);
        NVIC_SystemReset();
      }
      else {
        if(param_flag) lcd_puts_big(0,2,0,"Upload OK       "); 
        else lcd_puts_big(0,2,0,"Same Parameter  ");
      }
    }
    else {
      f_close(&USBHFile);
      lcd_puts_big(0,2,0,"Upload File Fail"); 
    }
  }
  else {
    lcd_puts_big(0,2,0,"File Not Found  "); 
  }
#if 0  //volume label add driver    
  else {
    size = sizeof(Setting.Drv_Name);
    for(i = 0; i < size; i++) 
      drv_buff[i] = VolLabel[i];
    if(drv_buff[0] != 0 ) {
      for(n = 0; n < Drv_Regist_Cnt; n++) {    //compare 
        pt = (u8 *)&Driver_Info[n].Name;
        diff_cnt = 0;
        for(i = 0; i < size; i++) {
          if(*(pt++) != drv_buff[i]) 
            diff_cnt++;
        }
        if(diff_cnt == 0) {
          if(Selected_Driver != n) {
            Selected_Driver = n;
            FRAMByteWrite(Selected_Driver, SELECTED_DRIVER); 
            NVIC_SystemReset();
          }
          break;    // Same data --> break;
        }
      }
    }
  }
#endif        
}

/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */

char str_car_reg[13], str_car_reg_fname[13];
char str_drv_code[19];
st_Setting TmpDailySetting;
st_Setting FistDailySetting;
st_DRIVER_INFO TmpDailyDriver_Info[9];
char save_fdir[20], save_fname[60], encrypt_bytes[40];
int ErrorDataCnt;
u32 NandResp;

void Data_Download(void)
{  
  UINT bytesWritten;  
  u32 i, j, k, addr;
  u8 *tmp;
  u16 day_cnt = 0;
  rtc_time dtime, stime;
  u8 trip_cnt = 0;
  u8 file_save_flag = 0;
  u8 drv_index = 10;
  time_t start_date, end_date;
  u16 tmp_dtrip;
  u32 tmp_ttrip;
  u8  tmp_brake;
  char gx_sign, gy_sign;
  s16 gx, gy;
  u8 scode;
  u32 total_cnt = 0, down_cnt = 0;
  u16 progress = 0, old_progress = 0;

  u8 daily_setting_flag = 0;
  time_t prevSecDataTime = 0;
  
  u8 encrypt_flag = 0, encrypt_byte, encrypt_len;
  u16 encrypt_sum = 0;  
  u32 mod_page_addr= 0;
  
  encrypt_flag = SECURE_TXT_FILE_SAVE;
  
  IWDG_ReloadCounter(); 
  
//- get start day -//
  if(Dnld_Day_Cnt == 0) Dnld_Day_Cnt = 1;
  
  
  if(DownloadByDateFlag) {
    start_date = RTCValTotime(DL_StartDay);
    day_cnt = CalDayCnt(start_date, RTC_LVal);
    if(day_cnt >= DAY_SAVE_MAX)
      day_cnt = DAY_SAVE_MAX;
    if(day_cnt >= DailyData.Index)
      day_cnt = DailyData.Index;
    start_date = RTC_LVal-(RTC_LVal%86400)-day_cnt*86400;
    end_date = RTCValTotime(DL_EndDay)+86400;
  }
  else {
    if(Dnld_Day_Cnt == DNLD_DAY_MAX+1) {
      if(DailyData.Index >= DAY_SAVE_MAX)
        day_cnt = DAY_SAVE_MAX;
      else 
        day_cnt = DailyData.Index;
    }
    else {
      if(DailyData.Index >= Dnld_Day_Cnt) 
        day_cnt = Dnld_Day_Cnt-1;
      else
        day_cnt = DailyData.Index;
    }
    start_date = RTC_LVal-(RTC_LVal%86400)-day_cnt*86400;
    end_date = RTC_LVal-(RTC_LVal%86400)+86400;
  }
  
  IWDG_ReloadCounter();
  if(USBH_MSC_IsReady(&hUsbHostFS) == 0) {
    lcd_clear();
    lcd_printf(0,2,0,"Device Not Connected");
    return;
  }

  IWDG_ReloadCounter(); 
 
  if(f_opendir(&Ddir, "DTES") != FR_OK) {
    if(f_mkdir("DTES") != FR_OK) {
      lcd_clear();
      lcd_printf(0,2,0,"Create Directory Failed");
      return;      
    }
    else {
      if(f_opendir(&Ddir, "DTES") != FR_OK) {
        lcd_clear();
        lcd_printf(0,2,0,"Open Directory Failed");
        return; 
      }
    }
  }      

  memset((u8*)&save_fdir, 0, 20); 
  sprintf(save_fdir, "DTES/%d%d%d%d", Setting.Car_Reg_No[5], Setting.Car_Reg_No[6], Setting.Car_Reg_No[7], Setting.Car_Reg_No[8]);  
  if(f_opendir(&Ddir, save_fdir) != FR_OK) {
    if(f_mkdir(save_fdir) != FR_OK) {
      lcd_clear();
      lcd_printf(0,2,0,"Create Directory Failed");
      return;      
    }
    else {
      if(f_opendir(&Ddir, save_fdir) != FR_OK) {
        lcd_clear();
        lcd_printf(0,2,0,"Open Directory Failed");
        return;        
      }
    }
  }    

  memcpy((u8*)&FistDailySetting, (u8*)&Setting, sizeof(st_Setting)); 
  for(i = 0; i <= day_cnt; i++) {
    IWDG_ReloadCounter();  
    memset((u8*)&TmpDailyData, 0, sizeof(stDailyData)); 
    Nand_Read_DailyData(day_cnt-i, (u8*)&TmpDailyData);
    if(TmpDailyData.Time >= start_date && TmpDailyData.Time < end_date) 
      total_cnt += TmpDailyData.NoOfSecData;
    if(!daily_setting_flag) {
      memset((u8*)&TmpDailySetting, 0, sizeof(st_Setting)); 
      Nand_Read_DailySetting(day_cnt-i, (u8*)&TmpDailySetting);
      if(TmpDailySetting.Identity == 0xDDDDDDDD) {
        memcpy((u8*)&FistDailySetting, (u8*)&TmpDailySetting, sizeof(st_Setting));
        daily_setting_flag = 1;
      }
    }
  }
  if(total_cnt == 0) total_cnt = 1;
  ErrorDataCnt = 0;
  
  memcpy((u8*)&TmpDailyDriver_Info[0], (u8*)&Driver_Info[0], sizeof(st_DRIVER_INFO)*9);
  for(i = 0; i <= day_cnt; i++) {
    IWDG_ReloadCounter();  
    memset((u8*)&TmpDailyData, 0, sizeof(stDailyData)); 
    trip_cnt = 0;
    drv_index = 10;
    Nand_Read_DailyData(day_cnt-i, (u8*)&TmpDailyData);
    memset((u8*)&TmpDailySetting, 0, sizeof(st_Setting)); 
    Nand_Read_DailySetting(day_cnt-i, (u8*)&TmpDailySetting);
    if(TmpDailySetting.Identity != 0xDDDDDDDD) 
      memcpy((u8*)&TmpDailySetting, (u8*)&FistDailySetting, sizeof(st_Setting));
    if(DailyData.DailyDataType == 1) {
      Nand_Read_DailyDriverInfo(day_cnt-i, (u8*)&TmpDailyDriver_Info[0]);
      if(TmpDailyDriver_Info[0].Num[0] < '0' || TmpDailyDriver_Info[0].Num[0] > '9') 
        memcpy((u8*)&TmpDailyDriver_Info[0], (u8*)&Driver_Info[0], sizeof(st_DRIVER_INFO)*9);
    }
    else {
      memcpy((u8*)&TmpDailyDriver_Info[0], (u8*)&Driver_Info[0], sizeof(st_DRIVER_INFO)*9);
    }
    if(TmpDailyData.Time >= start_date && TmpDailyData.Time < end_date) {
      dtime = gmtime_conv(TmpDailyData.Time);
      lcd_printf(80,1,0,"%02u/%02u/%02u", dtime.tm_year, dtime.tm_mon, dtime.tm_mday);
      if(TmpDailyData.NoOfSecData > 0) {
        memset((u8*)&str_car_reg, 0, 13); 
        memset((u8*)&str_car_reg_fname, 0, 13); 
        if(TmpDailySetting.Car_Reg_No[1]) {
          str_car_reg[0] = Car_No_Region[TmpDailySetting.Car_Reg_No[1]][0];
          str_car_reg[1] = Car_No_Region[TmpDailySetting.Car_Reg_No[1]][1];
          str_car_reg[2] = Car_No_Region[TmpDailySetting.Car_Reg_No[1]][2];
          str_car_reg[3] = Car_No_Region[TmpDailySetting.Car_Reg_No[1]][3];
          str_car_reg[4] = TmpDailySetting.Car_Reg_No[2]+0x30;
          str_car_reg[5] = TmpDailySetting.Car_Reg_No[3]+0x30;
          str_car_reg[6] = Car_No_Class[TmpDailySetting.Car_Reg_No[4]][0];
          str_car_reg[7] = Car_No_Class[TmpDailySetting.Car_Reg_No[4]][1];
          str_car_reg[8] = TmpDailySetting.Car_Reg_No[5]+0x30;
          str_car_reg[9] = TmpDailySetting.Car_Reg_No[6]+0x30;
          str_car_reg[10] = TmpDailySetting.Car_Reg_No[7]+0x30;
          str_car_reg[11] = TmpDailySetting.Car_Reg_No[8]+0x30;   
          memcpy(str_car_reg_fname, str_car_reg,12);
        }
        else {
          str_car_reg[0] = '#';
          str_car_reg[1] = '#';
          str_car_reg[2] = '#';
          str_car_reg[3] = '#';
          str_car_reg[4] = TmpDailySetting.Car_Reg_No[2]+0x30;
          str_car_reg[5] = TmpDailySetting.Car_Reg_No[3]+0x30;
          str_car_reg[6] = Car_No_Class[TmpDailySetting.Car_Reg_No[4]][0];
          str_car_reg[7] = Car_No_Class[TmpDailySetting.Car_Reg_No[4]][1];
          str_car_reg[8] = TmpDailySetting.Car_Reg_No[5]+0x30;
          str_car_reg[9] = TmpDailySetting.Car_Reg_No[6]+0x30;
          str_car_reg[10] = TmpDailySetting.Car_Reg_No[7]+0x30;
          str_car_reg[11] = TmpDailySetting.Car_Reg_No[8]+0x30;          
          memcpy(str_car_reg_fname, &str_car_reg[4],8);
        }
        for(j = 0; j < TmpDailyData.NoOfSecData; j++) {
          IWDG_ReloadCounter(); 
          addr = (TmpDailyData.NandStartAddress+j*sizeof(tsSecData))%NAND_SIZE_DATA;
          if(addr < DataSaveAddr && addr >= DataSaveAddr - (DataSaveAddr%NAND_PAGE_SIZE) ) {
            memcpy((u8*)&TmpSecData, (u8*)(Nand_Save_Buff+(addr%NAND_PAGE_SIZE)),sizeof(tsSecData));
          }
          else {
            mod_page_addr = (addr%NAND_PAGE_SIZE);
            if(mod_page_addr == 0 || j == 0)
              NandResp = Nand_Read_Buffer(NandTmpBuffer, addr-mod_page_addr, NAND_PAGE_SIZE);
            memcpy((u8*)&TmpSecData, (u8*)&NandTmpBuffer[mod_page_addr],sizeof(tsSecData));
          }
//          Nand_Read_SecData((u8*)&TmpSecData, addr, sizeof(tsSecData)); 
          if(IsSameDay(TmpSecData.Time, TmpDailyData.Time)) {
            
#ifndef CONTINUOUS_SECDATA_TRIP            
            if((TmpSecData.DriverIndex != drv_index || TmpSecData.TripCount != trip_cnt) && TmpSecData.TripCount < 100) {
#else              
            if((TmpSecData.DriverIndex != drv_index || TmpSecData.Time > prevSecDataTime+1) && trip_cnt < 99) {
#endif              
              if(Check_KeyCancel()) {
                if(file_save_flag && &USBHFile != NULL) {
                  if(f_close (&USBHFile) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  file_save_flag = 0;
                }
                USB_Cancel_Flag = 1;
                return;
              }
              if(TmpSecData.DriverIndex != drv_index) {
                if(TmpSecData.DriverIndex > 8) TmpSecData.DriverIndex = 0;
                memcpy(str_drv_code, TmpDailyDriver_Info[TmpSecData.DriverIndex].Code, 10);
                str_drv_code[10] = TmpSecData.DriverIndex+'1';
                memcpy(&str_drv_code[11], TmpDailyDriver_Info[TmpSecData.DriverIndex].Num, 7);                
                for(k = 0; k < 18; k++)
                  if(str_drv_code[k] < '0' || str_drv_code[k] > '9')
                    str_drv_code[k] = '0';
                str_drv_code[18] = 0;
              }
              drv_index = TmpSecData.DriverIndex;
#ifndef CONTINUOUS_SECDATA_TRIP               
              trip_cnt = TmpSecData.TripCount;
#else              
              trip_cnt++;
#endif              
              if(!USB_DataDL_LastTrip || (USB_DataDL_LastTrip && TmpSecData.Time >= TripStartTime)) {
                if(file_save_flag && &USBHFile != NULL) {
                  if(f_close (&USBHFile) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  file_save_flag = 0;
  //                vTaskDelay(1);
                }
                memset(save_fname, 0, sizeof(save_fname));
                if(Setting.FileOutput) {
                  sprintf(save_fname,"%s/%04u%02u%02u-%s-S-%02u-%s.DTE",save_fdir,dtime.tm_year+2000,dtime.tm_mon,dtime.tm_mday,str_car_reg_fname
                                                             ,trip_cnt, &str_drv_code[11]);
                }
                else {
                  if(encrypt_flag) {
                    memset(encrypt_bytes, 0, sizeof(encrypt_bytes));
                    sprintf(encrypt_bytes,"%04u%02u%02u-%s-X-%02u-%s.TXT",dtime.tm_year+2000,dtime.tm_mon,dtime.tm_mday,str_car_reg_fname
                                                               ,trip_cnt, &str_drv_code[11]);
                    sprintf(save_fname,"%s/%s",save_fdir,encrypt_bytes);
                    encrypt_len = StringLen(encrypt_bytes,40);
                    encrypt_sum = 0;
                    for(k = 0; k < encrypt_len; k++) encrypt_sum += encrypt_bytes[k];
                    encrypt_byte = (encrypt_sum&0xff);
                    if(encrypt_byte == 0) encrypt_byte = 1;

                  }
                  else {
                    sprintf(save_fname,"%s/%04u%02u%02u-%s-S-%02u-%s.TXT",save_fdir,dtime.tm_year+2000,dtime.tm_mon,dtime.tm_mday,str_car_reg_fname
                                                             ,trip_cnt, &str_drv_code[11]);
                    encrypt_byte = 0;
                  }
                }
                f_unlink(save_fname);
                
                IWDG_ReloadCounter();
               
                // Open the file to write on it 
                if(f_open(&USBHFile, save_fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
                  lcd_clear();
                  lcd_printf(0,2,0,"Create File Failed");
                  return;
                }    
                file_save_flag = 1;
                if(Setting.FileOutput) {
                  if(f_write (&USBHFile, (u8*)&TmpDailySetting, 256, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                }
                else {
                  sprintf((char*)TmpBuff,"###########LDT-300BS");
                  if(encrypt_flag) {
                    for(k = 0; k < 20; k++) TmpBuff[k] = TmpBuff[k]^encrypt_byte;
                  }
                  if(f_write (&USBHFile, TmpBuff, 20, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  
                  memcpy(TmpBuff, TmpDailySetting.Car_No, 17);
                  if(encrypt_flag) {
                    for(k = 0; k < 17; k++) TmpBuff[k] = TmpBuff[k]^encrypt_byte;
                  }
                  if(f_write (&USBHFile, TmpBuff, 17, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  
                  sprintf((char*)TmpBuff, "%02u%s", Car_Type_Code[TmpDailySetting.Car_Reg_No[0]], str_car_reg); 
                  if(encrypt_flag) {
                    for(k = 0; k < 14; k++) TmpBuff[k] = TmpBuff[k]^encrypt_byte;
                  }
                  if(f_write (&USBHFile, TmpBuff, 14, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  
                  memcpy(TmpBuff, TmpDailySetting.Company_Reg_No, 10);
                  if(encrypt_flag) {
                    for(k = 0; k < 10; k++) TmpBuff[k] = TmpBuff[k]^encrypt_byte;
                  }
                  if(f_write (&USBHFile, TmpBuff, 10, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  
                  memcpy(TmpBuff, str_drv_code, 18);
                  if(encrypt_flag) {
                    for(k = 0; k < 18; k++) TmpBuff[k] = TmpBuff[k]^encrypt_byte;
                  }
                  if(f_write (&USBHFile, TmpBuff, 18, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                }
              }
            }
            if(file_save_flag && prevSecDataTime != TmpSecData.Time) {
              if(Setting.FileOutput) {
                if(f_write (&USBHFile, (u8*)&TmpSecData, sizeof(tsSecData), &bytesWritten) != FR_OK) {
                  USB_Error_Flag = 1;
                  return;
                }
              }
              else {                
                tmp_dtrip = (u16)(TmpSecData.DTrip);
                tmp_ttrip = (u32)(TmpSecData.TTrip);
                stime = gmtime_conv(TmpSecData.Time);
                tmp_brake = TmpSecData.Signal.Brake;
                gx = TmpSecData.AccX;
                gy = TmpSecData.AccY;
                if(gx < 0) {
                  gx = -gx;
                  gx_sign = '-';
                }
                else
                  gx_sign = '+';
                if(gy < 0) {
                  gy = -gy;
                  gy_sign = '-';
                }
                else
                  gy_sign = '+';
                
                scode = 0;
                if(TmpSecData.Status.GPS_Fault)         scode = 11;
                else if(TmpSecData.Status.Speed_Fault)  scode = 12;
                else if(TmpSecData.Status.RPM_Fault)    scode = 13;
                else if(TmpSecData.Status.Brake_Fault)  scode = 14;
                else if(TmpSecData.Status.Input_Fault)  scode = 21;
                else if(TmpSecData.Status.Output_Fault) scode = 22;
                else if(TmpSecData.Status.Save_Fault)   scode = 31;
                else if(TmpSecData.Status.Comm_Fault)   scode = 32;
                else if(TmpSecData.Status.Dist_Fault)   scode = 41;
                else if(TmpSecData.Status.Power_Fault)  scode = 99;
                     
                sprintf((char*)TmpBuff, "%04u%07u%02u%02u%02u%02u%02u%02u00%03u%04u%01u%09u%09u%03u%c%03u.%01u%c%03u.%01u%02u"
                        ,tmp_dtrip, tmp_ttrip, stime.tm_year, stime.tm_mon, stime.tm_mday, stime.tm_hour, stime.tm_min, stime.tm_sec
                        ,TmpSecData.Speed[0], TmpSecData.RPM, tmp_brake, TmpSecData.Lon, TmpSecData.Lat, TmpSecData.Azi
                        ,gx_sign, gx/10, (gx%10),gy_sign, gy/10, (gy%10), scode); 
                if(encrypt_flag) {
                  for(k = 0; k < 68; k++) TmpBuff[k] = TmpBuff[k]^encrypt_byte;
                }
                if(f_write (&USBHFile, TmpBuff, 68, &bytesWritten) != FR_OK) {
                  USB_Error_Flag = 1;
                  return;
                }
              }
            }
            prevSecDataTime = TmpSecData.Time;
            down_cnt++;
            progress = 100*down_cnt/total_cnt;
            if(progress > 99) progress = 99;
            if(progress != old_progress) {
              lcd_fill_box(0, (80*progress/100), 3, 4);
              lcd_big_printf(88,2,0,"%3u", progress);
              old_progress = progress;
            }
          }
          else {
            ErrorDataCnt++;
          }
          if(!Power_Off_Mode && (j%3600) == 0) {
            if(Start_Flag > 1) {
              Nand_Write_SaveData();
              Day_Change();
            }
          }
        }
      }
    }
                 
    if(file_save_flag && &USBHFile != NULL) {
      if(f_close (&USBHFile) != FR_OK) {
        USB_Error_Flag = 1;
        return;
      }
      file_save_flag = 0;
    }
//    vTaskDelay(1);
  }

  
  IWDG_ReloadCounter();  

#ifdef EVENT_DATA_SAVE_ENABLE  
//- event data save -//
  u32 n, e_addr, mod;
  long rday;
  u8 ev_idx, e_cnt = 0;
  st_Setting *ev_head;

  memset(save_fname, 0, sizeof(save_fname));
  sprintf(save_fname, "%s/Event_%02d%02d%02d%s", save_fdir ,RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, ".DTG");
  f_unlink(save_fname);
  // Open the file to write on it 
  if(f_open(&USBHFile, save_fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
    lcd_clear();
    lcd_printf(0,2,0,"Create Event File Failed");
    return;
  }    
  
  memset(NandTmpBuffer, 0, NAND_PAGE_SIZE);
  n = sizeof(st_Setting);
  if(Evt.Idx > 0)
    ev_idx = Evt.Idx - 1;
  else
    ev_idx = 9;
  for(i = 0; i < 10; i++) {
    if(ev_idx >= i)
      e_addr = (ev_idx-i)*NAND_BLOCK_SIZE + NAND_EVENT_DATA_ADDR; 
    else
      e_addr = (ev_idx-i+10)*NAND_BLOCK_SIZE + NAND_EVENT_DATA_ADDR; 
    Nand_Read_Buffer(NandTmpBuffer, e_addr, n);
    ev_head = (st_Setting *)NandTmpBuffer;
    rday = ((u32)ev_head->Time[0]<<16)|((u32)ev_head->Time[1]<<8)|ev_head->Time[2]; 
    if(ev_head->Identity == 0xEEEEEEEE && rday > 0 && e_cnt < 10) e_cnt++;
    else break;
  }     
  
  n = (2000*sizeof(st_DATA) + sizeof(st_Setting))/NAND_PAGE_SIZE;
  mod = (2000*sizeof(st_DATA) + sizeof(st_Setting))%NAND_PAGE_SIZE;
  if(e_cnt > 0) {           //ev_sidx == 0  No Event 
    for(i = 0; i < e_cnt; i++) {
      if(ev_idx >= i)
        e_addr = (ev_idx-i)*NAND_BLOCK_SIZE + NAND_EVENT_DATA_ADDR; 
      else
        e_addr = (ev_idx-i+10)*NAND_BLOCK_SIZE + NAND_EVENT_DATA_ADDR; 
      for(j = 0; j < n; j++) { //j =32에서 멈춤..
        addr = j*NAND_PAGE_SIZE+e_addr;
        Nand_Read_Buffer(NandTmpBuffer, addr, NAND_PAGE_SIZE);
        if(f_write (&USBHFile, NandTmpBuffer, NAND_PAGE_SIZE, &bytesWritten) != FR_OK) {
          USB_Error_Flag = 1;
          return;
        }
//        delay_ms(1);
        IWDG_ReloadCounter();  
      }
      addr = j*NAND_PAGE_SIZE+e_addr;
      Nand_Read_Buffer(NandTmpBuffer, addr, mod);
      if(f_write (&USBHFile, NandTmpBuffer, mod, &bytesWritten) != FR_OK) {
        USB_Error_Flag = 1;
        return;
      }
//      delay_ms(1);
      IWDG_ReloadCounter();  
    }
  }  

// Close file and filesystem 
  
  if(f_close (&USBHFile) != FR_OK) {
    USB_Error_Flag = 1;
    return;
  }  
#endif
  lcd_puts_big(0,0,0,"다운로드완료!   ");//
  lcd_fill_box(0, 80, 3, 4);
  lcd_puts_big(88,2,0,"100");

  if(Dnld_Hist_Cnt < 5) {
    tmp = (u8 *)&RTC_Val;
    for(i = 0; i < 6; i++)
      Dnld_Hist[Dnld_Hist_Cnt][i] = *tmp++;    
    Dnld_Hist_Cnt++;
  }
  else {
    for(i = 0; i < 4; i++)
      for(j = 0; j < 6; j++)
        Dnld_Hist[i][j] = Dnld_Hist[i+1][j];    
    tmp = (u8 *)&RTC_Val;
    for(i = 0; i < 6; i++)
      Dnld_Hist[4][i] = *tmp++;    
  }
  FRAMMultiWrite((u8 *)&Dnld_Hist_Cnt, DNLD_HIST_CNT, 1);
  for(i = 0; i < 5; i++)
    FRAMMultiWrite((u8 *)&Dnld_Hist[i], DNLD_HIST + i*6, 6);  
}


/**
  * @brief  Main routine for Mass Storage Class.
  * @param  None
  * @retval None
  */
void MSC_Application(void)
{			

  Check_FirmwareUpdate();  
  USB_DriverFileUpdate();
//  USB_FirmwareUpdate();
//  USB_ParamUpload();
}

u8 USB_ParamDownload(void) // 파일 두개 생성
{
  u8 resp = 0;
  u32 bytesWritten;
  
  if(f_open(&USBHFile, ParamFileName, FA_CREATE_ALWAYS|FA_WRITE) == FR_OK) {
    if(f_write(&USBHFile, (u8 *)&Setting, sizeof(st_Setting), &bytesWritten) == FR_OK) {    
      resp = 1;
      f_close(&USBHFile);
    } 
    else resp = 0;
  }
  if(f_open(&USBHFile, PARAMETER_FILENAME, FA_CREATE_ALWAYS|FA_WRITE) == FR_OK) {
    if(f_write(&USBHFile, (u8 *)&Setting, sizeof(st_Setting), &bytesWritten) == FR_OK) {    
      f_close(&USBHFile);
    } 
  }
  if(resp == 0)
    lcd_puts_big(0,2,0,"Download Fail   ");
  else
    lcd_puts_big(0,2,0,"Download OK     ");
  
  return resp;
}

void USB_Download(void)
{
  USB_Download_Flag = 1;
  USB_Error_Flag = 0;
  USB_Cancel_Flag = 0;

  if(Power_Off_Flag) 
    Parameter_Save();

  if(USB_DataDL_LastTrip) 
    Dnld_Day_Cnt = CalDayCnt(RTC_LVal,TripStartTime)+1;
  Data_Download();
  USB_DataDL_LastTrip = 0;
  
  if(Power_Off_Flag && !USB_DrvVeh_Flag)
    Power_Off();

  if(USB_Error_Flag) {
    lcd_clear();
    lcd_printf(0,2,0,"File Writing Fail");
    USBH_ReEnumerate(&hUsbHostFS);
  }
  else if(USB_Cancel_Flag) {
    lcd_clear();
    lcd_printf(0,2,0,"File Writing Cancel");
    USB_Cancel_Flag = 0;
  }
  
  USB_Download_Flag = 0;
}

u8 UsbFaultCnt;
int UsbFaultTimeCnt;
void CheckUsbFault(void)
{
  if(!USB_Detect_Flag && hUsbHostFS.gState != HOST_IDLE) { 
    UsbFaultTimeCnt++;
    if(UsbFaultTimeCnt > 1000) {
      USBH_LL_Disconnect(&hUsbHostFS);
      USBH_ReEnumerate(&hUsbHostFS);
      UsbFaultTimeCnt = 0;
      UsbFaultCnt++;
    }
  }
  else UsbFaultTimeCnt = 0;
}

void InitParamFileName(void)
{
  int i, idx = 0;
  
  memset(ParamFileName, 0, 20);
  for(i = 0; i < sizeof(Setting.Serial_No); i++) {
    if((Setting.Serial_No[i] >= '0' && Setting.Serial_No[i] <= '9') || (Setting.Serial_No[i] >= 'A' && Setting.Serial_No[i] <= 'z'))
      ParamFileName[idx++] = Setting.Serial_No[i];
    else
      break;
  }
  if(idx == 0)
    sprintf(ParamFileName,"%s",PARAMETER_FILENAME);
  else {
    sprintf(&ParamFileName[idx],".PRM");
  }
}

/**
  * @brief  USB_JigTestWrite
  * @param  None
  * @retval None
  */
char writeArray[] = "Test file in USB";

u8 USB_JigTestWrite(void)
{			
  u8 res = 0;
  u32 bytesWritten;
  
  if(f_open(&USBHFile, "usb_test.txt", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
    f_lseek(&USBHFile, f_size(&USBHFile));
    
    if((Fresp = f_write(&USBHFile, writeArray, strlen(writeArray), &bytesWritten)) == FR_OK) {
      printk("File written OK!\n");
      res = 1;

    } else {
      printk("File writing error!\n");
      res = 0;      
    }
    
    /* Close file */
    f_close(&USBHFile);
  } else {
    /* File not opened */
    printk("Failed to open file!\n");
    res = 0; 
  }
  
  return res;
}

/**
  * @brief USB_JigTestCompare
  * @param  None
  * @retval None
  */
u8 USB_JigTestCompare(void) 
{
 
  u8 wr, rm, res = 0;
  u32 flen = 0;
  u32 bytesRead;
  
  if(f_open(&USBHFile, "usb_test.txt", FA_READ) == FR_OK) {
    flen = f_size(&USBHFile);
    f_read (&USBHFile, TmpBuff, flen, (void *)&bytesRead);  
    
    if(TmpBuff[0] == 'T' && TmpBuff[1] == 'e' && TmpBuff[2] == 's' && TmpBuff[3] == 't') {
      wr = 1; 
    }
    else wr = 0;

    f_close (&USBHFile);

    if(f_unlink("usb_test.txt") == FR_OK)
      rm = 1; 
    else rm = 0;
    
    if(wr && rm) 
      res = 1;
    else res = 0;
  }
  
  return res;
}


/* Task_USB function */
void Task_USB(void const * argument)
{
  u8 res = 0;
  /* USER CODE BEGIN Task_USB */
  /* init code for FATFS */
  MX_FATFS_Init();
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  
  InitParamFileName();
  
  USB_POWER_ON();     // USB Power On
  /* Infinite loop */
  for(;;) 
  { 
    /* USB Host Background task */
    MX_USB_HOST_Process();
    
    /* Mass Storage Application State Machine */
    switch(Appli_state) 
    {
    case APPLICATION_READY:
      /* If not mounted already */
        /* Try to mount and write using FATFS */
      if ((Fresp = f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 0)) == FR_OK) {
        /* Mounted OK */
        printk("USB FS: Mounted OK!\n");
          
        USB_Download_Flag = 1;
        MSC_Application();
        USB_Download_Flag = 0;
        USB_Detect_Flag = 1;
        
      } else {
        /* Mounting error! */
        printk("USB FS: Failed to mount! FRES: %d\n", Fresp);
      }        
      Appli_state = APPLICATION_IDLE;
      break;
      
    case APPLICATION_APP:
#ifdef EVENT_DATA_SAVE_ENABLE
     if(USB_DataDL_Flag && !Evt.Save_Flag) {
#else        
     if(USB_DataDL_Flag) {
#endif       
        USB_Download();
        USB_DataDL_Flag = 0;
        USB_DataDL_Complete = 1;
      }
      else if(USB_ParamDL_Flag) {
        USB_ParamDownload();
        USB_ParamDL_Flag = 0;
        USB_ParamDL_Complete = 1;
      }
      else if(USB_ParamUL_Flag) {
        USB_ParamUpload();
        USB_ParamUL_Flag = 0;
        USB_ParamUL_Complete = 1;
      }
      else if(USB_FirmwareUpdate_Flag) {
        USB_FirmwareUpdate();
        USB_FirmwareUpdate_Flag = 0;
        USB_FirmwareUpdate_Complete = 1;
      }
      else if(USB_JigTest_Flag) {
        res = USB_JigTestWrite();
        TestJIgTxData(1, 0x80, (u8*)&res, 1);        
        USB_JigTest_Flag = 0;
      }
      else if(USB_JigCompare_Flag) {
        res = USB_JigTestCompare();
        TestJIgTxData(1, 0x90, (u8*)&res, 1);  
        USB_JigCompare_Flag = 0;
      }
      else if(USB_DrvVeh_Flag) {
        if(!VehMaintenanceFlag)
          USB_DrvVehFileUpdate();
        USB_DrvVeh_Flag = 0;
      }
      
      if(USB_DataDL_Flag == 0 && USB_ParamDL_Flag == 0 && USB_ParamUL_Flag == 0 && USB_FirmwareUpdate_Flag == 0 \
        && USB_JigTest_Flag == 0 && USB_JigCompare_Flag == 0 && USB_DrvVeh_Flag == 0)
        Appli_state = APPLICATION_IDLE;
      break;      
      
    case APPLICATION_DISCONNECT:
     /* Force MSC Device to re-enumerate */
//      f_mount(NULL, (TCHAR const*)"", 0);
      USB_Detect_Flag = 0;
      Appli_state = APPLICATION_IDLE; 
      if(VehMaintenanceFlag) {
        VehMaintenanceFlag = 0;
        Display_Flag = 1;
      }
      break;
      
    case APPLICATION_IDLE:
      if(USB_DataDL_Flag || USB_ParamDL_Flag || USB_ParamUL_Flag || USB_FirmwareUpdate_Flag || USB_JigTest_Flag \
        || USB_JigCompare_Flag || USB_DrvVeh_Flag)
        Appli_state = APPLICATION_APP;      
      break;
      
    default:
      break;      
    }
    CheckUsbFault();
    osDelay(1); 
  }  
  /* USER CODE END Task_USB */
}

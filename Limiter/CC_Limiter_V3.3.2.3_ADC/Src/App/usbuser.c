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


/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */

st_Setting TmpDailySetting;
st_Setting FistDailySetting;
char save_fdir[20], save_fname[60];
int ErrorDataCnt;
u32 NandResp;

void Data_Download(void)
{  
  UINT bytesWritten;  
  u32 i, j, addr;
  u8 *tmp;
  u16 day_cnt = 0;
  rtc_time dtime, stime;
  u8 trip_cnt = 0;
  u8 file_save_flag = 0;
  time_t start_date, end_date;
  u32 total_cnt = 0, down_cnt = 0;
  u16 progress = 0, old_progress = 0;

  u8 daily_setting_flag = 0;
  time_t prevSecDataTime = 0;
  
  u32 mod_page_addr= 0;
  
  u16 ver;
  u16 canbps;
  UINT dlen;
  
  ver = (MAJOR*1000) + (MINOR*100) + (RELEASE*10) + BUILD;
  if((Setting.CanBPS&0x0F))
    canbps = 500;
  else
    canbps = 250;
  
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
  sprintf(save_fdir, "DTES/SCR");  
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
  
  for(i = 0; i <= day_cnt; i++) {
    IWDG_ReloadCounter();  
    memset((u8*)&TmpDailyData, 0, sizeof(stDailyData)); 
    trip_cnt = 0;
    Nand_Read_DailyData(day_cnt-i, (u8*)&TmpDailyData);
    if(TmpDailyData.Time >= start_date && TmpDailyData.Time < end_date) {
      dtime = gmtime_conv(TmpDailyData.Time);
      lcd_printf(80,1,0,"%02u/%02u/%02u", dtime.tm_year, dtime.tm_mon, dtime.tm_mday);
      if(TmpDailyData.NoOfSecData > 0) {
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
          if(IsSameDay(TmpSecData.TM, TmpDailyData.Time)) {
            if(TmpSecData.TM > prevSecDataTime+10 && trip_cnt < 99) {
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
              trip_cnt++;
              if(!USB_DataDL_LastTrip || (USB_DataDL_LastTrip && TmpSecData.TM >= TripStartTime)) {
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
                  sprintf(save_fname,"%s/%02u%02u%02u_%02u.DTE",save_fdir,dtime.tm_year,dtime.tm_mon,dtime.tm_mday,trip_cnt);
                }
                else {
                  sprintf(save_fname,"%s/%02u%02u%02u_%02u.xls",save_fdir,dtime.tm_year,dtime.tm_mon,dtime.tm_mday,trip_cnt);
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
                  memset(TmpBuff, 0, 256);
                  sprintf((char*)TmpBuff, "Date\tTime\tNOxIn\tNOxOut\tTempIn\tTempOut\tDosingRate\tTankLevel\t");
                  dlen = StringLen((char*)TmpBuff, 256);
                  if(f_write (&USBHFile, TmpBuff, dlen, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                  memset(TmpBuff, 0, 256);
                  sprintf((char*)TmpBuff, "MAF\tState\tConcentration\tVbat\tLimitSts\tSpeed\tErrorCode\tCheckCode\tInV\tOutV\tOBD V%01d.%01d\tUser %01d\tCAN %01d kbps", ver/100, ver%100, TmpSecData.User, canbps);
                  dlen = StringLen((char*)TmpBuff, 256);
                  if(f_write (&USBHFile, TmpBuff, dlen, &bytesWritten) != FR_OK) {
                    USB_Error_Flag = 1;
                    return;
                  }
                }
              }
            }
            if(file_save_flag && prevSecDataTime != TmpSecData.TM) {
              if(Setting.FileOutput) {
                if(f_write (&USBHFile, (u8*)&TmpSecData, sizeof(tsSecData), &bytesWritten) != FR_OK) {
                  USB_Error_Flag = 1;
                  return;
                }
              }
              else {           
                stime = gmtime_conv(TmpSecData.TM);
                memset(TmpBuff, 0, 256);
                sprintf((char*)TmpBuff, "\n%02u.%02u.%02u\t%02u:%02u:%02u\t%01d\t%01d\t%01d\t%01d\t%01d\t%01d\t%01d\t%01d\t%01d\t%01d.%01d\t%02d\t%01d\t%01d\t%01d\t%01d.%03d\t%01d.%03d"
                        ,stime.tm_year, stime.tm_mon, stime.tm_mday, stime.tm_hour, stime.tm_min, stime.tm_sec
                        , TmpSecData.NOxIn, TmpSecData.NOxOut
                        ,TmpSecData.TempIn, TmpSecData.TempOut, TmpSecData.DosingRate, TmpSecData.UreaLevel
                        ,TmpSecData.MAF, TmpSecData.State, TmpSecData.Concentration, TmpSecData.Vbat/10, TmpSecData.Vbat%10, TmpSecData.UreaAccFlag,TmpSecData.Speed,TmpSecData.Err.B,TmpSecData.Chk.W
                        ,TmpSecData.basePress/1000, TmpSecData.basePress%1000, TmpSecData.DpfTemp/1000, TmpSecData.DpfTemp%1000);
                dlen = StringLen((char*)TmpBuff, 256);
                if(f_write (&USBHFile, TmpBuff, dlen, &bytesWritten) != FR_OK) {
                  USB_Error_Flag = 1;
                  return;
                }
              }
            }
            prevSecDataTime = TmpSecData.TM;
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
  
  if(Power_Off_Flag)
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
     if(USB_DataDL_Flag) {
        USB_Download();
        USB_DataDL_Flag = 0;
        USB_DataDL_Complete = 1;
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
      
      if(USB_DataDL_Flag == 0 && USB_FirmwareUpdate_Flag == 0 \
        && USB_JigTest_Flag == 0 && USB_JigCompare_Flag == 0)
        Appli_state = APPLICATION_IDLE;
      break;      
      
    case APPLICATION_DISCONNECT:
     /* Force MSC Device to re-enumerate */
//      f_mount(NULL, (TCHAR const*)"", 0);
      USB_Detect_Flag = 0;
      Appli_state = APPLICATION_IDLE; 
      break;
      
    case APPLICATION_IDLE:
      if(USB_DataDL_Flag || USB_FirmwareUpdate_Flag || USB_JigTest_Flag \
        || USB_JigCompare_Flag)
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

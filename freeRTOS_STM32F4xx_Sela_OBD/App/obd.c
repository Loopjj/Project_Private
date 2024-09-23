/*
*************************************** (C) COPYRIGHT 2015 Loop ****************
*
* File Name          : obd.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 06/19/2015
*
* Description        : obd source 
*
********************************************************************************
*                       REVISION HISTORY
********************************************************************************

------------------ V1.0.0 ---------------------------
2018.11.29 50EA ���

------------------ V1.1.0 ---------------------------
2018.12.07 ���� ��ũ ���� ��� ����

------------------ V1.0.0 ---------------------------
2020.05.27 ��ȸ SCR ���� 100Set ���� CAN 250k

------------------ V1.0.1 ---------------------------
2020.07.06 ������ ���� ���� 1�� -> 4��
           RTC �ð� �ѱ��ð� ����

------------------ V1.0.2 ---------------------------
2020.07.08 ũ��� Idel��� 5�� ���� ���� ����

------------------ V1.0.3 ---------------------------
2020.07.08 ũ��� ��ũ ����
           ���� ��Ŭ ����

------------------ V1.0.4 ---------------------------
2020.07.31 ���� ������ ���� �� Flash_Read_SecData -> SPI_FLASH_BufferRead ����

------------------ V1.0.4.1 -------------------------
2020.08.07 Base��� ���� ���� ����

------------------ V1.0.4.2 -------------------------
2020.08.20 Setting�� �ʱ�ȭ ����

*/
#define OBD_GLOBALS
#include "includes.h"

static FATFS FatFs;

#define SaveSize   16
#define SyncSize   8
#define SyncByte   0xc6

#define LOW_LIMT_OF_TIME  0x58000000  //2017�� ���İ�.

__packed typedef struct {
  INT8U sync[8];      
  Time_t rtc;
}header_t;

header_t Header;
INT8U PrevSpeed;
unsigned long PrevLat, PrevLon;
unsigned int GPSNotValidCount;

INT8U GPS_Time_Sync;
INT8U GPS_Time_NoSyncCnt;

time_t PrevSecTime;

const INT8U rotate[14] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x20 };
const INT8U rport[14]  = { 0, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 0, 0, 0 }; 


/*
********************************************************************************
* Description : UpdateTimeToGPSTime
* Arguments   : None
* Return      : 
* Note        :  UpdateTimeToGPSTime
******************************************************************************** 
*/       
void UpdateTimeToGPSTime(INT32U rtime)
{
	struct tm tm_time;
  Time_t t;

  t = gmtime_conv(rtime);
  if(GPS_Time_Sync)  {
    tm_time.tm_sec = t.tm_sec;
    tm_time.tm_min = t.tm_min;
    tm_time.tm_hour = t.tm_hour;
    tm_time.tm_mday = t.tm_mday;
    tm_time.tm_mon = t.tm_mon;
    tm_time.tm_year = t.tm_year;
    tm_time.tm_isdst = 0;
    SetRTCTime(&tm_time);    
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
  Time_t gps_time;
  u32 diff = 0;
  
  if(GPS_Valid && rmc.time != Prev_rmctime) {
    gps_time.tm_hour = rmc.time/10000;
    gps_time.tm_min  = (rmc.time/100)%100; 
    gps_time.tm_sec  = rmc.time%100;
    gps_time.tm_year = rmc.date%100;
    gps_time.tm_mon  = (rmc.date/100)%100;
    gps_time.tm_mday = rmc.date/10000;
    RTC_GPS_Val = mktime_conv(gps_time);
    diff = abs(RTC_GPS_Val-(RTC_LVal-TIMESHIFT_SEC));
    if(RTC_GPS_Val > LOW_LIMT_OF_TIME) {
      if(diff > 3) {
        GPS_Time_Sync = 1; 
        if(diff > 86400 && GPS_SyncOKTime < 60)   UpdateTimeToGPSTime(RTC_GPS_Val+TIMESHIFT_SEC);   //�Ϸ��̻� �ð����϶� �ٷ� ������Ʈ �ϰ� �׷��� ������ Ű������ ������Ʈ..
      }
      else GPS_SyncOKTime++; //GPS �� �ð��� 3���̳� ������ ��� ī��Ʈ 
    }
    Prev_rmctime = rmc.time;
    GPS_Time_NoSyncCnt = 0;
  }  
  else {
    if(GPS_Time_Sync) {
      GPS_Time_NoSyncCnt++;
      if(GPS_Time_NoSyncCnt > 30) GPS_Time_Sync = 0;  //�� 3�� 
    }
  }
}  

/*******************************************************************************
* Function Name  : DrvDataUpdate
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
stREAD_VALUE PrevReadValue;
void DrvDataUpdate(void)
{
  unsigned long lat, lon;
  
  ReadValue.TM = RTC_LVal;
  
  if(GPS_Valid) {
    GPS_Speed_Val = (u32)(rmc.speed  * 1.852);
    // GPS ������ 
    lat = (unsigned long)(gga.latitude  * 1000000.0);
    lon = (unsigned long)(gga.longitude * 1000000.0);
    if((ReadValue.TM - PrevSecTime) < 5) {
      if(abs(PrevLat-lat) < 50000) {
        ReadValue.LT = lat;
      }
      else ReadValue.LN = 0;
      if(abs(PrevLon-lon) < 50000) {
        ReadValue.LN = lon;
      }
      else ReadValue.LN = 0;
    }
    else {
      ReadValue.LT = 0;
      ReadValue.LN = 0;
    }
 
    ReadValue.CO  = (unsigned short)rmc.course;
    PrevSecTime = ReadValue.TM;
    PrevLat = lat; 
    PrevLon = lon;
    GPSNotValidCount = 0;
  }
  else {
    if(GPSNotValidCount < 60)   GPSNotValidCount++;
    if(!SecData.Sig.KeyOn || GPSNotValidCount >= 60) {
      ReadValue.LT = 0;
      ReadValue.LN = 0;
//      ReadValue.AT = 0;
      GPS_Speed_Val = 0;
    }
  }
  ReadValue.Speed = GPS_Speed_Val;

// ���� ������ �����Ҷ� �Ʒ� �����͸� ����� ������.
//  ReadValue.State; // End, CleanEarth
//  ReadValue.Concentration;  //CleanEarth
//  ReadValue.SystemError[8]; //EnD
//  ReadValue.DoserError[8];  //Iljin
  
  if(Save_SecData_Cnt < 300) {
    memcpy((u8*)&ReadValue_Buff[Save_SecData_Cnt++], (u8*)&ReadValue, sizeof(stREAD_VALUE));
  }
  if(!Save_SecData_Flag) {
      Save_SecData_Flag = 1;
  }
  
  PrevReadValue = ReadValue;
}

/*******************************************************************************
* Function Name  : Flash_Write_SaveData
* Description    : Data write
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_Write_SaveData(void)
{
  u16 size;
  u16 n;

  if(!Save_SecData_Flag) return; 

  if(DataSaveAddr%64)
    DataSaveAddr = (DataSaveAddr+(64-DataSaveAddr%64));
  if((DataSaveAddr % SPI_FLASH_PAGE) == 0) {   // every 4K erase
    SPI_FLASH_PageErase(DataSaveAddr);
  }
  
  size = sizeof(stREAD_VALUE);
  for(n = 0; n < Save_SecData_Cnt; n++) {
    SPI_FLASH_BufferWrite((u8*)&ReadValue_Buff[n], DataSaveAddr, size);
    DataSaveAddr = (DataSaveAddr+size)%FLASH_DATA_SIZE;
    memset((u8*)&ReadValue_Buff[n], 0, size);
  }
  
  FRAMMultiWrite((u8 *)&DataSaveAddr,DRV_SAVE_ADDR,4); 

  Save_SecData_Cnt = 0;
  Save_SecData_Flag = 0;
}

/*
********************************************************************************
* Description : Flash_Read_SecData
* Arguments   : *buffer, address, length
* Return      : 
* Note        : Read Flash sec data
******************************************************************************** 
*/
void Flash_Read_SecData(u8 *pBuffer, u32 addr, u16 len)
{
  if(addr < DataSaveAddr && addr >= DataSaveAddr - (DataSaveAddr % SPI_FLASH_PAGE) ) {
    memcpy(pBuffer, (u8*)(Flash_Save_Buff+(addr % SPI_FLASH_PAGE)),len);
  }
  else {
    SPI_FLASH_BufferRead(pBuffer, addr, len);
  }
}

/*
********************************************************************************
* Description : Default Set
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void DefaultSet(void)
{
  u32 i, len;
  u16 addr, SetSiz = 0;

	DATA_LED(ON);	  // LED ON
  memset(Flash_Save_Buff, 0, SPI_FLASH_PAGE);
  FRAMMultiRead((u8 *)&Setting, DRV_SETTING_PARM, sizeof(st_Setting)); 
  
  for(i = 0; i < 256; i++)  {            //Fram ��� �ʱ�ȭ  8Kbyte
    FRAMMultiWrite(Flash_Save_Buff, i*32, 32);
    IWDG_ReloadCounter(); 
    delay_ms(100);
  }
  
  Setting.FW_Type = USE_MODEM;
  addr = DRV_SETTING_PARM + offsetof(st_Setting, FW_Type);
  FRAMByteWrite(Setting.FW_Type, addr);
  
  memset(Setting.Model_No, ' ', sizeof(Setting.Model_No));
  len = StringLen((char*)Model_Name, 15);
  memcpy(Setting.Model_No, (u8*)Model_Name, len);
  addr = DRV_SETTING_PARM + (u8 *)&Setting.Model_No - (u8 *)&Setting;
  FRAMMultiWrite((u8 *)&Setting.Model_No, addr, sizeof(Setting.Model_No));
  IWDG_ReloadCounter(); 
  
  Setting.t_DelayP = PDELAYTIME;
  SetSiz += sizeof(Setting.t_DelayP);
  
  Setting.t_Buzzer = 1;
  SetSiz += sizeof(Setting.t_Buzzer);
  
  if(Setting.UserNumber == USER_CERACOMB) {
    Setting.P1L_Set = 350;
    SetSiz += sizeof(Setting.P1L_Set);
    
    Setting.P1H_Set = 400;
    SetSiz += sizeof(Setting.P1H_Set);
  }
  else {
    Setting.P1L_Set = 500;
    SetSiz += sizeof(Setting.P1L_Set);
    
    Setting.P1H_Set = 700;
    SetSiz += sizeof(Setting.P1H_Set);
  }
  
  Setting.Prangehigh = PRANGEHIGH;
  SetSiz += sizeof(Setting.Prangehigh);
  
  Setting.Prangelow = PRANGELOW;
  SetSiz += sizeof(Setting.Prangelow);
  
  Setting.P1CalHigh = PCalHigh;
  SetSiz += sizeof(Setting.P1CalHigh);
  
  Setting.P1CalLow = PCalLow;
  SetSiz += sizeof(Setting.P1CalLow);
  
  Setting.T1Offset = T_Offset;
  SetSiz+= sizeof(Setting.T1Offset);
  
  if(Setting.UserNumber == USER_CERACOMB) {
    Setting.TLL_Set = 10;
    SetSiz+= sizeof(Setting.TLL_Set);
    
    Setting.TLH_Set = 15;
    SetSiz+= sizeof(Setting.TLH_Set);
  }
  else if(Setting.UserNumber == USER_ILJIN35L || Setting.UserNumber == USER_ILJIN50L) {
    Setting.TLL_Set = 0;
    SetSiz+= sizeof(Setting.TLL_Set);
    
    Setting.TLH_Set = 30;
    SetSiz+= sizeof(Setting.TLH_Set);
  }
  else {
    Setting.TLL_Set = 0;
    SetSiz+= sizeof(Setting.TLL_Set);
    
    Setting.TLH_Set = 15;
    SetSiz+= sizeof(Setting.TLH_Set);
  }
  IWDG_ReloadCounter();
  
  Setting.t_DelayTL = TLDELAYTIME;
  SetSiz+= sizeof(Setting.t_DelayTL);
  
  Setting.LED = 0x1000;
  SetSiz+= sizeof(Setting.LED);
  
  Setting.IdlePress = 0;
  SetSiz+= sizeof(Setting.IdlePress);
  ReadValue.basePress = Setting.IdlePress;
  memset(P_BUF, 0, 12*2);
  
  Setting.Prangehigh = PRANGEHIGH;
  SetSiz+= sizeof(Setting.Prangehigh);
  
  Setting.Prangelow = PRANGELOW;
  SetSiz+= sizeof(Setting.Prangelow);
  
	Setting.PAlarm.B = 0;
  SetSiz+= sizeof(Setting.PAlarm);
  
  if(Setting.UserNumber >= USER_MAX) Setting.UserNumber = 0;
  SetSiz+= sizeof(Setting.UserNumber);
  
	Setting.CanBPS = 1;
  SetSiz+= sizeof(Setting.CanBPS);
  
  addr = DRV_SETTING_PARM + offsetof(st_Setting, t_DelayP); 
  FRAMMultiWrite((u8 *)&Setting.t_DelayP, addr, SetSiz);
  IWDG_ReloadCounter();
  
  Modem_Parameter_Reset();
	DATA_LED(OFF);
}

/*
********************************************************************************
* Description : Parameter Initialize
******************************************************************************** 
*/
void InitPara(void) 
{
  u8 ver[6];
  u16 i, addr, size;    
  u32 saddr = 0;
  
  ReadValue.FW_Ver = (MAJOR*1000) + (MINOR*100) + (RELEASE*10) + BUILD;
  DMS_ver = (MAJOR*1000) + (MINOR*100) + (RELEASE*10) + BUILD;
  Flash_Save_Buff =  (u8*)BKPSRAM_BASE;
    
  FRAMMultiRead((u8 *)&ver, FIRMWARE_VER, 6);
  if(!(ver[0] == MAJOR && ver[1] == MINOR && ver[2] == RELEASE && ver[3] == BUILD)) {
    ver[0] = MAJOR; ver[1] = MINOR; ver[2] = RELEASE; ver[3] = BUILD;
    ver[4] = 0; ver[5] = 0;
    FRAMMultiWrite((u8 *)&ver, FIRMWARE_VER, 6);
    
    addr = DRV_SETTING_PARM + (u8 *)&Setting.FW_Ver - (u8 *)&Setting;
    size = sizeof(Setting.FW_Ver)-2; //1.4.0.0
    FRAMMultiWrite((u8 *)&ver, addr, size); 
  }

  FRAMMultiRead((u8 *)&Setting, DRV_SETTING_PARM, sizeof(st_Setting));
  
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
  if(Setting.PAlarm.PHAlarmFlag) LPRESS_LED(Setting.PAlarm.PHAlarmFlag);
  else if(Setting.PAlarm.PLAlarmFlag) LPRESS_LED(Setting.PAlarm.PLAlarmFlag);
  IWDG_ReloadCounter();
  
  if(Setting.IdlePress > 1000) {
    Setting.IdlePress = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, IdlePress);
    FRAMMultiWrite((u8 *)&Setting.IdlePress, addr, sizeof(Setting.IdlePress));
  }
  for(i=0; i<12; i++) P_BUF[i] = Setting.IdlePress;
  ReadValue.basePress = Setting.IdlePress;
  if(Setting.UserNumber > USER_MAX) {
    Setting.UserNumber = 0;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, UserNumber);
    FRAMMultiWrite((u8 *)&Setting.UserNumber, addr, sizeof(Setting.UserNumber));
  }
  if(Setting.CanBPS > 1) {
    Setting.CanBPS = 1;
    addr = DRV_SETTING_PARM + offsetof(st_Setting, CanBPS);
    FRAMMultiWrite((u8 *)&Setting.CanBPS, addr, sizeof(Setting.CanBPS));
  }
  IWDG_ReloadCounter();
  
  GPIO_Write(GPIOE, (GPIO_ReadOutputData(GPIOE) | 0xf000));
  GPIO_Write(GPIOE, (GPIO_ReadOutputData(GPIOE) & (~(Setting.LED &  0xf000))));

  FRAMMultiRead((u8 *)&saddr, DRV_SAVE_ADDR, 4);   
  if(saddr%sizeof(stREAD_VALUE) > 0 || saddr > FLASH_DATA_SIZE) 
    saddr = DataSaveAddr; //RTC_BK_Addr->Sent_Addr;
  DataSaveAddr = saddr;

  FRAMMultiRead((u8 *)&saddr, DRV_SENT_ADDR, 4);   
  if(saddr%sizeof(stREAD_VALUE) > 0 || saddr > FLASH_DATA_SIZE) 
    saddr = DataSaveAddr; //RTC_BK_Addr->Sent_Addr;
  DataSentAddr = saddr;
  
  FRAMMultiRead((u8 *)&saddr, VDIS_SENT_ADDR, 4);   
  if(saddr%sizeof(stREAD_VALUE) > 0 || saddr > FLASH_DATA_SIZE) 
    saddr = DataSaveAddr;
  VdisSentAddr = saddr;
  IWDG_ReloadCounter();
  

	if(Setting.t_Buzzer > 10) 
		Setting.t_Buzzer = 10;
}

/*
********************************************************************************
* Description : SaveData
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void MMCSaveData(void)
{
 	sysinfo_t *s = &_sysinfo;   
  
 	if(open_append(&fp, F_Name)==FR_OK) {
    f_printf(&fp, "\n");
#if 0	
	                   // Date		
		f_printf(&fp, "%01d%01d.%01d%01d.%01d%01d\t",    
                 		s->tm_ptr.tm_year/10, s->tm_ptr.tm_year%10, s->tm_ptr.tm_mon/10, s->tm_ptr.tm_mon%10, s->tm_ptr.tm_mday/10, s->tm_ptr.tm_mday%10);
                   // Time
		f_printf(&fp, "%01d%01d:%01d%01d:%01d%01d\t",    
                 		s->tm_ptr.tm_hour/10, s->tm_ptr.tm_hour%10, s->tm_ptr.tm_min/10, s->tm_ptr.tm_min%10, s->tm_ptr.tm_sec/10, s->tm_ptr.tm_sec%10);
                   // DpfTemp, DpfPress, NOxIn, NOxOut
		f_printf(&fp, "%01d\t%01d\t%01d\t%01d\t",ReadValue.DpfTemp, ReadValue.DpfPress, ReadValue.NOxIn, ReadValue.NOxOut);
                   // TempIn, TempOut, DosingRate(lu), UreaLevel
		f_printf(&fp, "%01d\t%01d\t%01d\t%01d\t", ReadValue.TempIn, ReadValue.TempOut, ReadValue.DosingRate, ReadValue.UreaLevel);
                   // MAF, State, Concentration, Vbat
		f_printf(&fp, "%01d\t%01d\t%01d\t%01d", ReadValue.MAF, ReadValue.State, ReadValue.Concentration, ReadValue.Vbat);
#else
    f_printf(&fp, "%01d%01d.%01d%01d.%01d%01d\t",    
                 		s->tm_ptr.tm_year/10, s->tm_ptr.tm_year%10, s->tm_ptr.tm_mon/10, s->tm_ptr.tm_mon%10, s->tm_ptr.tm_mday/10, s->tm_ptr.tm_mday%10);
                   // Time
		f_printf(&fp, "%01d%01d:%01d%01d:%01d%01d\t",    
                 		s->tm_ptr.tm_hour/10, s->tm_ptr.tm_hour%10, s->tm_ptr.tm_min/10, s->tm_ptr.tm_min%10, s->tm_ptr.tm_sec/10, s->tm_ptr.tm_sec%10);
                   // NOxIn NOxOut MAF T1 Inject
		f_printf(&fp, "%01d\t%01d\t%01d\t%01d\t%01d\t%01d\t%02d.%02d\t%02d.%02d\t",
                    SecData.NOxIn, SecData.NOxOut, SecData.MAF, SecData.T1, SecData.T2, SecData.Tavg, SecData.DosingRatehouer/100, SecData.DosingRatehouer%100,SecData.TotalDosingRate/100, SecData.TotalDosingRate%100);
                   // Battery Level P1 Factor 
		f_printf(&fp, "%02d.%02d\t %01d\t %01d\t %01d.%02d\t", 
                    SecData.BV/100, SecData.BV%100, SecData.TankLevelP, SecData.P1, SecData.StatusAlpha/100, SecData.StatusAlpha%100);
                   // Error Check Signal
		f_printf(&fp, "%01d\t%01d\t%01d\t%01d", SecData.Err.B,SecData.Chk.W,SecData.Sig.W,SecData.Gps.Speed);
#endif    
		f_close(&fp);    
  	Flags.SaveOkFlag = 1;
	}
  else {
  	Flags.SaveOkFlag = 0;  
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
 	sysinfo_t *s = &_sysinfo;
  int canbps = 0;
 
  memset((INT8U *)F_Name, 0x00, 12);
  
	F_Name[0] = (s->tm_ptr.tm_year/10) + 0x30;	// convert to ascii code
	F_Name[1] = (s->tm_ptr.tm_year%10) + 0x30;
	F_Name[2] = (s->tm_ptr.tm_mon/10) + 0x30;	// convert to ascii code
	F_Name[3] = (s->tm_ptr.tm_mon%10) + 0x30;
	F_Name[4] = (s->tm_ptr.tm_mday/10) + 0x30;	// convert to ascii code
	F_Name[5] = (s->tm_ptr.tm_mday%10) + 0x30;
	F_Name[6] = '.';	
	F_Name[7] = 'x';
	F_Name[8] = 'l';	
	F_Name[9] = 's';
  
  if(Setting.CanBPS)
    canbps = 500;
  else
    canbps = 250;
  
  if(f_open(&fp,F_Name,FA_OPEN_EXISTING) != FR_OK) { //file MUST exist for reading
   	if(f_open(&fp,F_Name,FA_CREATE_ALWAYS| FA_WRITE) == FR_OK)
 	  {
#if 0      
      f_printf(&fp, "Date\tTime\tDpfTemp\tDpfPress\tNOxIn\tNOxOut\t");
      f_printf(&fp, "TempIn\tTempOut\tDosingRate\tTankLevel\t");
      f_printf(&fp, "MAF\tState\tConcentration\tVbat\t");
      f_printf(&fp, "OBD V%01d.%01d\t", DMS_ver/100, DMS_ver%100);
      f_printf(&fp, "User %01d\tCAN %01d kbps", ReadValue.User, canbps);
#else
      f_printf(&fp, "Date\tTime\tNOxIn\tNOxOut\tMAF\tT1\tT2\tTavg\tInject\tTotalInject\t");
  		f_printf(&fp, "Battery\tLevel\tP1\tfactor\tError\tCheck\tSignal\tSpeed");      
#endif      
      f_close(&fp);
		  Flags.SaveOkFlag = 1;     	
		  return 1;
	  }
	  else{
 	    Flags.SaveOkFlag = 0;    	
	    return 0;
	  }	  
  }
  else {
	  f_close(&fp);
  	MMCSaveData();
  	return 1; 
  }       		
}

/*
********************************************************************************
* Description : Disk Detect
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
int Disk_Dtect(void)
{
	if(!MMC_DETECT() && !DetectFlag) 
		return 1;
	else {
		Flags.SaveOkFlag = 0; 
  	return 0;
	}
}

/*
********************************************************************************
* Description : System Check
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void MMC_Check(void) 
{
  int res;
  
  if(!Flags.CheckFlag) {
    if(Disk_Dtect()) {
      if(disk_initialize(0) == FR_OK)	{   // 0: ATA, 1: USB
        f_mount(&FatFs, "", 0);          
				res = scan_files("");	
				if(res == FR_DISK_FULL)	
					Disk_Full = 1;
        else 
          Disk_Full = 0;
        DetectFlag = 0;
      }
      else DetectFlag = 1; 
      if(Disk_Dtect()) FileCreate();
    }
      Flags.CheckFlag = 1;
  }
}

/*
********************************************************************************
* Description : KeyOn_Process
* Arguments   : none
* Return      : 
* Note        : KeyOn check process
******************************************************************************** 
*/
void KeyOn_Process(void)
{
	sysinfo_t *s = &_sysinfo 	;

	/*Key Off�� Power Off*/

  if(POWER_SIG())	{
    s->key.keyon_st = ON;
    s->key.nStatus = 0;
  }
	else {
    s->key.keyon_st = OFF;
  }
		
	{
		switch(s->key.nStatus)
		{
		case 0:
			if(s->key.keyon_st == ON)//���࿡ on�̸�...�ٽ� ���ư�....����
			{
				s->key.nStatus= 0;
				s->key.OffCount = 0;
			}
			else
			{
        vTaskDelay(100);
        CHARGE(OFF);
        system_power_off();
			}
			break;
		default:
			break;
		}
	}
}

/*
********************************************************************************
* Description : SecDataSave
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
void SecDataSave(void)  
{
	sysinfo_t *s = &_sysinfo;
  
  if(Flags.SaveDataFlag && s->key.nStatus < 2) {
    CheckGPSTimeSyc();
    DrvDataUpdate();
    Flash_Write_SaveData();
    if(Setting.FW_Type == USE_MODEM) {
      Modem_SecIntProcess();
    }
//    memset(&ReadValue.SysCode,0,sizeof(stSysCode));
    Flags.SaveDataFlag = 0;
  }
}

/*
********************************************************************************
* Description : Analysis Process
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Analysis(void)
{
  //Tank Level �˶�
	if ((ReadValue.UreaLevel <= Setting.TLL_Set) && (!Flags1.TLvLAlarmFlag)) { //Low Tank Level Alarm	
		Flags1.HTLvLCntFlag = 1;
		if(HTLvLCnt >= Setting.t_DelayTL){			           //HTLvLCntNum = Delay count
      Flags1.TLvHAlarmFlag = 1;
      Flags1.TLvLAlarmFlag = 1;
      Flags1.HTLvLCntFlag = 0;
			TLv_Beep = 3;
			HTLvLCnt = 0;
      NORMAL_LED(OFF);
//      HPRESS_LED(Flags1.TLvLAlarmFlag);
    }
	}
 	else if ((ReadValue.UreaLevel <= Setting.TLH_Set) && (!Flags1.TLvHAlarmFlag)) {			//Low Tank Level Alarm	
		Flags1.HTLvHCntFlag = 1;
		if(HTLvHCnt >= Setting.t_DelayTL){			             //HTLvHCntNum = Delay count Time 15S
      Flags1.TLvHAlarmFlag = 1;
      Flags1.HTLvHCntFlag = 0;
			HTLvHCnt = 0;
      NORMAL_LED(OFF);
//      HPRESS_LED(Flags1.TLvHAlarmFlag);
		}
	}
	else if ((ReadValue.UreaLevel > Setting.TLH_Set) && (Flags1.TLvHAlarmFlag)) {
		Flags1.HTLvHCntFlag = 1;
		if(HTLvHCnt >= Setting.t_DelayTL){			           //HTLvLCntNum = Delay count
      Flags1.TLvHAlarmFlag = 0;
      Flags1.TLvLAlarmFlag = 0;
      Flags1.HTLvHCntFlag = 0;
			HTLvLCnt = 0;
      NORMAL_LED(ON);
//      HPRESS_LED(Flags1.TLvHAlarmFlag);
    }
	}
 	else if ((ReadValue.UreaLevel > Setting.TLL_Set) && (Flags1.TLvLAlarmFlag)) {
		Flags1.HTLvLCntFlag = 1;
		if(HTLvLCnt >= Setting.t_DelayTL){			             //HTLvHCntNum = Delay count
      Flags1.TLvLAlarmFlag = 0;
      Flags1.HTLvLCntFlag = 0;
			HTLvHCnt = 0;
		}
	}
  
  if(Flags1.BuzzerFlag) {
    BuzzerOn(Setting.t_Buzzer);
    Flags1.BuzzerFlag = 0;
  }
}

/*
********************************************************************************
* Description : Idle Pressure Check
******************************************************************************** 
*/
void Idle_Pressure_Check(void) 
{
  u16 i, addr;
  
  DeltaP1 = Pre_dP1-ReadValue.DpfPress;
  Pre_dP1 = ReadValue.DpfPress;    

  // Idle Pressure
  if(SecData.Sig.KeyOn && ReadValue.DpfPress && !ReadValue.Speed && (ReadValue.MAF > 15) && !DeltaP1) {
    IdleP1Cnt++;
    if(IdleP1Cnt >= 5) { // 5s Check                                                                               
      P_BUF[P_Cnt] = ReadValue.DpfPress;
      P_Cnt++;
      if(P_Cnt >= 12) { // 5 * 12 = 60s Check
        if(Setting.IdlePress != ReadValue.basePress) {
          Setting.IdlePress = ReadValue.basePress;
          addr = DRV_SETTING_PARM + offsetof(st_Setting, IdlePress);
          FRAMMultiWrite((u8 *)&Setting.IdlePress, addr, sizeof(Setting.IdlePress));
        }
        P_Cnt = 0;
      }
      for(i=0;i<12;i++)
        SumP1 += P_BUF[i];
          
      ReadValue.basePress = (u16)(SumP1 / 12);
      SumP1 = 0;
      IdleP1Cnt = 0;
    }
  } 
  else IdleP1Cnt = 0;
}

/*
********************************************************************************
* Description : File Name
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
INT32U File_Name(int cnt, char n_val)
{
  INT32U temp_val = 0;

  n_val -= 0x30;
  switch(cnt) {
    case 0:
      temp_val = n_val * 100000;
      break;
    case 1:
      temp_val = n_val * 10000;
      break;
    case 2:
      temp_val = n_val * 1000;
      break;
    case 3:
      temp_val = n_val * 100;
      break;
    case 4:
      temp_val = n_val * 10;
      break;
    case 5:
      temp_val = n_val;
      break;
  }

  return temp_val; 
}

/*
********************************************************************************
* Description : remove files
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void remove_files (
	char* path		/* Pointer to the working buffer with start path */
)
{
	DIR dirs;
  TCHAR	dfname[10];
	FRESULT res;
  FILINFO DFinfo;
	INT32U ctime = 0, mtime = 0, i;
  
	res = f_opendir(&dirs, path);
	if (res == FR_OK) {
		while (((res = f_readdir(&dirs, &DFinfo)) == FR_OK) && DFinfo.fname[0]) {
      for(i = 0; i < 6; i++)
        mtime += File_Name(i, DFinfo.fname[i]);
      if(!ctime || (ctime > mtime)) {
        memcpy(dfname, &DFinfo.fname, 10);
        ctime = mtime;
      }
      mtime = 0;
		}
    f_unlink(dfname);
    if(DelFiles > 0)DelFiles--;
    else {
      Disk_Full = 0;
      FileCreate();
    }
    f_closedir(&dirs);
	}
}


/* 
*********************************************************************************
*                           Task MMC                                            *
*********************************************************************************
*/
void Task_OBD(void *p_arg)
{
  sysinfo_t *s = (sysinfo_t *)p_arg;
  
  DATA_LED(ON);
  LPRESS_LED(ON);
  HPRESS_LED(ON);
  TEMP_LED(ON);
  NORMAL_LED(ON);
  vTaskDelay(1000);
  DATA_LED(OFF);
  LPRESS_LED(OFF);
  HPRESS_LED(OFF);
  TEMP_LED(OFF);
  NORMAL_LED(OFF);
  InitPara();
  Flags.StartFlag = 0;
  vTaskDelay(1000);
  ModemInit();
  Setting.FW_Type = USE_MODEM;
  ReadValue.User = Setting.UserNumber;
  BSP_CAN_Init();
  vTaskDelay(3000);
  CHARGE(ON);
  MMC_Check();
  Save_SecData_Cnt = 0;
  PreDay = s->tm_ptr.tm_mday;
  vTaskResume(xHandle_can); // CAN Task start
  
  for(;;)
	{
		s->twatchObdFlag = 1;
    if(Flags.CheckFlag && SecData.Sig.KeyOn) {
      if(TmrChk(TMR_DLINTERVAL) == 0)	{	
        TmrSetT(TMR_DLINTERVAL, 10);
        TmrStart(TMR_DLINTERVAL);
        if(!BackUpFlag && Flags.SecIntFlag) {
            Analysis();
          if(Setting.ECUFW_flag && Flags.DownloadFlag)
            CanFotaFile();
          else if(Setting.ECUFW_flag && !Flags.DownloadFlag) {
            keyflag.hidden=3;
            keyflag.setcnt=2;
          }            
          if(Disk_Dtect()) {
            if(Disk_Full) {
              remove_files("");
            }
            else if(Flags.DownloadFlag) 
              CanTxFile();
            else if(Flags.MMCSaveFlag) {
              if(PreDay != s->tm_ptr.tm_mday) {
                FileCreate();
                PreDay = s->tm_ptr.tm_mday;
              }
              MMCSaveData();
              Flags.MMCSaveFlag = 0;
            }
          }
          SecDataSave();
          DATA_LED(Flags.SaveOkFlag);
        }
      }
    }
    if(Flags.KeyOnFlag) 
    {
      Start_Flag = 1;
//      Idle_Pressure_Check();
      KeyOn_Process();
      Flags.KeyOnFlag = 0;
    }
    s->twatchObdFlag = 0;
    vTaskDelay(1);
	}
}

/*
********************************************************************************
* Description : dbg_factory_init
* Arguments   : none
* Return      : 
* Note        : factory init(Sys memory)
******************************************************************************** 
*/
INT16U dbg_factory_init(void)
{
	int pass;

	if(CLI_GetArgCnt() < 1)	return INV_PRM;
	
	pass = comparison_num((char *)P_ARG(1));

  if(pass == 1234) {  // password�� ���Ŀ� ���Ұ�..
    DefaultSet();
    printk("\nSystem factory init!!\n");
  }
  else 
    printk("\nPassword mismatch!!\n");
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_factory_init
* Arguments   : none
* Return      : 
* Note        : factory init(Sys memory)
******************************************************************************** 
*/
INT16U dbg_scrclib_init(void)
{
	int pass;

	if(CLI_GetArgCnt() < 1)	return INV_PRM;
	
	pass = comparison_num((char *)P_ARG(1));

  if(pass == 1234) {  // password�� ���Ŀ� ���Ұ�..
    DefaultSet();
    printk("\nCalib default init!!\n");
  }
  else 
    printk("\nPassword mismatch!!\n");
	return NO_DISPLAY;
}


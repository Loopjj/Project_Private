/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : bsp_util.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 09/25/2014
*
* Description        : SCR board configuration 
*
*********************************************************************************************************
*/

#define BUTIL_GLOBALS
#include "includes.h"


void BSRAMSemaphore_Init(void)
{
	BSP_BKSramSem = xSemaphoreCreateMutex();
	BSP_BKRtcSem  = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_BKSramSem);
	vSemaphoreCreateBinary(BSP_BKRtcSem );
}

/** 
* Returns the system uptime, in milliseconds. 
*/ 
INT32U systick_uptime(void) { 
  return systick_uptime_millis; 
} 


INT8U write_to_backup_sram( INT8U *data, INT16U size, INT16U offset )
{
	const INT16U backup_size = 0x1000;
	INT8U* base_addr = (INT8U *) BKPSRAM_BASE;
	INT16U i;

	if(BSP_BKSramSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_BKSramSem, ( portTickType ) 0 )  == pdTRUE)
	{

		if( size + offset >= backup_size ) 
		{
			/* ERROR : the last byte is outside the backup SRAM region */
			xSemaphoreGive(BSP_BKSramSem);
			return FALSE;
		}
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
			/* disable backup domain write protection */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   // set RCC->APB1ENR.pwren
		PWR_BackupAccessCmd(ENABLE);                          // set PWR->CR.dbp = 1;
		/** enable the backup regulator (used to maintain the backup SRAM content in
		* standby and Vbat modes).  NOTE : this bit is not reset when the device
		* wakes up from standby, system reset or power reset. You can check that
		* the backup regulator is ready on PWR->CSR.brr, see rm p144 */
		PWR_BackupRegulatorCmd(ENABLE);     // set PWR->CSR.bre = 1;
		for( i = 0; i < size; i++ )
		{
			*(base_addr + offset + i) = *(data + i);
		}
		PWR_BackupAccessCmd(DISABLE);                     // reset PWR->CR.dbp = 0;
		xSemaphoreGive(BSP_BKSramSem);
		return TRUE;
	
	}
	return FALSE;
}

INT8U read_from_backup_sram( INT8U *data, INT16U size, INT16U offset )
{
	const INT16U backup_size = 0x1000;
	INT8U* base_addr = (INT8U *) BKPSRAM_BASE;
	INT16U i;

	if(BSP_BKSramSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_BKSramSem, ( portTickType ) 0 )  == pdTRUE)
	{
		if( size + offset >= backup_size ) 
		{
			/* ERROR : the last byte is outside the backup SRAM region */
			xSemaphoreGive(BSP_BKSramSem);
			return FALSE;
		}
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
		for( i = 0; i < size; i++ )
		{
			*(data + i) = *(base_addr + offset + i);
		}
		xSemaphoreGive(BSP_BKSramSem);
		return TRUE;
	}
	return FALSE;
}

INT8U write_to_backup_rtc( INT32U *data, INT16U size, INT16U offset ) 
{
	const INT16U backup_size = 80;
	volatile INT32U* base_addr = &(RTC->BKP0R);
	INT16U i;

	if(BSP_BKRtcSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_BKRtcSem, ( portTickType ) 0 )  == pdTRUE)
	{

		if( size + offset >= backup_size ) {
			/* ERROR : the last byte is outside the backup SRAM region */
			xSemaphoreGive(BSP_BKRtcSem);
			return FALSE;
		} 
		else if( offset % 4 || size % 4 )
		{
			/* ERROR: data start or num bytes are not word aligned */
			xSemaphoreGive(BSP_BKRtcSem);
			return FALSE;
		} 
		else 
		{
			size >>= 2;      /* divide by 4 because writing words */
		}
		/* disable backup domain write protection */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   // set RCC->APB1ENR.pwren
		PWR_BackupAccessCmd(ENABLE);                          // set PWR->CR.dbp = 1;
		for( i = 0; i < size; i++ ) 
		{
			*(base_addr + offset + i) = *(data + i);
		}
		PWR_BackupAccessCmd(DISABLE);                     // reset PWR->CR.dbp = 0;
		// consider also disabling the power peripherial?
		xSemaphoreGive(BSP_BKRtcSem);
		return TRUE;
	}
	return FALSE;
}

INT8U read_from_backup_rtc( INT32U *data, INT16U size, INT16U offset )
{
	const uint16_t backup_size = 80;
	volatile INT32U* base_addr = &(RTC->BKP0R);
	INT16U i;

	if(BSP_BKRtcSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_BKRtcSem, ( portTickType ) 0 )  == pdTRUE)
	{

		if( size + offset >= backup_size )
		{
			/* ERROR : the last byte is outside the backup SRAM region */
			xSemaphoreGive(BSP_BKRtcSem);
			return FALSE;
		} 
		else if( offset % 4 || size % 4 )
		{
			/* ERROR: data start or num bytes are not word aligned */
			xSemaphoreGive(BSP_BKRtcSem);
			return FALSE;
		} 
		else 
		{
			size >>= 2;      /* divide by 4 because writing words */
		}
		/* read should be 32 bit aligned */
		for( i = 0; i < size; i++ ) {
			*(data + i) = *(base_addr + offset + i);
		}
		xSemaphoreGive(BSP_BKRtcSem);
		return TRUE;
	}
	return FALSE;
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	if(RTC_TimeStructure.RTC_Hours > 24) RTC_TimeStructure.RTC_Hours -= 24;
	printk(" :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	/* Get the current Alarm */
	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	printk("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}
void RTC_DateShow(void)
{
	RTC_DateTypeDef RTC_DateStructure;
	/* Get the current Date */
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	printk("\n\r  The current date:  %0.2d-%0.2d-%0.2d", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date);

}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeSet(INT8U hours, INT8U Min, INT8U sec)
{
	RTC_TimeTypeDef RTC_TimeStructure;

	printk("\n\r==============Time Settings=====================================\n\r");
	if( hours > 12)
		RTC_TimeStructure.RTC_H12     = RTC_H12_PM;//RTC_H12_PM
	else
		RTC_TimeStructure.RTC_H12     = RTC_H12_AM;//RTC_H12_PM

    RTC_TimeStructure.RTC_Hours = (uint8_t)(hours);

    RTC_TimeStructure.RTC_Minutes = (uint8_t)Min;
  
    RTC_TimeStructure.RTC_Seconds = (uint8_t)sec;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		printk("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		/* Indicator for the RTC configuration */
		RTC_TimeShow();
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
}
void RTC_TimeRegulate(struct tm *tm_ptr)
{
	INT8U ret=0;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	//printk("\n\r==============Time Settings=====================================\n\r");
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;//RTC_H12_PM

    RTC_TimeStructure.RTC_Hours = (uint8_t)(tm_ptr->tm_hour);

    RTC_TimeStructure.RTC_Minutes = (uint8_t)tm_ptr->tm_min;
  
    RTC_TimeStructure.RTC_Seconds = (uint8_t)tm_ptr->tm_sec;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		printk("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
	tm_ptr->tm_wday = 0;
	RTC_DateStructure.RTC_WeekDay = (uint8_t) tm_ptr->tm_wday;

	RTC_DateStructure.RTC_Month   = (uint8_t) tm_ptr->tm_mon;

	RTC_DateStructure.RTC_Date    = (uint8_t) tm_ptr->tm_mday;

	RTC_DateStructure.RTC_Year    = (uint8_t) tm_ptr->tm_year;// - 1900;

	ret =RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
	//if( RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
	if(ret == ERROR)
	{
		printk("\n>> !! RTC Set Date failed. !! <<\n");
	}
	else
	{
		//printk("\n>> !! RTC Set Date success. !! <<\n");
		//RTC_DateShow();
		//RTC_TimeShow();
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
}

	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
void GetRTCTime(struct tm *tm_ptr)
{
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	tm_ptr->tm_year = RTC_DateStructure.RTC_Year;// + 1900   ;
	tm_ptr->tm_mon  = RTC_DateStructure.RTC_Month  ;
	tm_ptr->tm_mday = RTC_DateStructure.RTC_Date   ;
	tm_ptr->tm_hour = RTC_TimeStructure.RTC_Hours  ;
	tm_ptr->tm_min  = RTC_TimeStructure.RTC_Minutes;
	tm_ptr->tm_sec  = RTC_TimeStructure.RTC_Seconds;
  
  RTC_Time.tm_sec  = tm_ptr->tm_sec;
  RTC_Time.tm_min  = tm_ptr->tm_min;
  RTC_Time.tm_hour = tm_ptr->tm_hour;
  RTC_Time.tm_mday = tm_ptr->tm_mday;
  RTC_Time.tm_mon  = tm_ptr->tm_mon;
  RTC_Time.tm_year = tm_ptr->tm_year;
  RTC_LVal    = mktime_conv(RTC_Time);  
}
void MakeTimePtr(struct tm *tm_ptr,INT32U year, INT8U mon, INT8U mday, INT8U hours, INT8U mins, INT8U sec, INT8U wday)
{
	tm_ptr->tm_year = year ;
	tm_ptr->tm_mon  = mon  ;
	tm_ptr->tm_mday = mday ;
	tm_ptr->tm_hour = hours;
	tm_ptr->tm_min  = mins ;
	tm_ptr->tm_sec  = sec  ;
	tm_ptr->tm_wday = wday;

}
void SetRTCTime(struct tm *tm_ptr)
{
	RTC_TimeRegulate(tm_ptr);
}

INT8U TimeParsorSet(char *data)
{
	struct tm tm_ptr;
	if(data[0] != 0) 	tm_ptr.tm_year = data[0];// + 2000 ;
	else				return 0;
	if(data[1] != 0 && data[1] <= 12)	tm_ptr.tm_mon  = data[1]  ;
	else				return 0;
	if(data[2] != 0 && data[2] <= 31)	tm_ptr.tm_mday = data[2] ;
	else				return 0;
	if(data[3] <= 24)	tm_ptr.tm_hour = data[3];
	else				return 0;
	if(data[4] <= 60)	tm_ptr.tm_min  = data[4] ;
	else				return 0;
	if(data[5] <= 60)	tm_ptr.tm_sec  = data[5]  ;
	else				return 0;
	SetRTCTime(&tm_ptr);
	return 1;
}
void ResponseTime(char *data)
{
	struct tm tm_ptr;

	GetRTCTime(&tm_ptr);

	data[0] = (INT8U)(tm_ptr.tm_year);//-2000);
	data[1] = (INT8U)tm_ptr.tm_mon  ;
	data[2] = (INT8U)tm_ptr.tm_mday ;
	data[3] = (INT8U)tm_ptr.tm_hour ;
	data[4] = (INT8U)tm_ptr.tm_min  ;
	data[5] = (INT8U)tm_ptr.tm_sec  ;
}

#define	DAY_STRING_MAX	60
static char dday[ DAY_STRING_MAX ] 	;
static char wday[7][4] = {"Sun","Mon", "Tue", "Wed", "Thu", "Fri", "Sat" } ;


char * btime2str(struct tm *tm_ptr, char *format)  // like strftime/*{{{*/
{ 
	int    index = 0  ; 
	int    i          ; 

	memset(dday, 0x00, DAY_STRING_MAX ) ;

	for (i = 0; i < strlen(format); i++) 
	{ 
		switch(format[i]) 
		{ 
			case 'Y': // ??
				sprintf(dday+index, "%04d", tm_ptr->tm_year); 
				index += 4; 
				break; 
			case 'y': // ??
				sprintf(dday+index, "%02d", (tm_ptr->tm_year % 100)); 
				index += 2; 
				break; 
			case 'm': // ??1-12), 遺?0-59)  
				if (format[i+1] == 'i') 
				{ 
					sprintf(dday+index, "%02d", tm_ptr->tm_min); 
					index += 2; 
					i++; 
					break; 
				} 
				else 
				{ 
					sprintf(dday+index, "%02d", tm_ptr->tm_mon); 
					index += 2; 
					break; 
				} 
			case 'd': // ??(1-31) 
				sprintf(dday+index, "%02d", tm_ptr->tm_mday); 
				index += 2; 
				break; 
			case 'D': // ?붿씪(??~ ?? 
				sprintf(dday+index, "%s", wday[tm_ptr->tm_wday]); 
				index += 3; 
				break; 
			case 'h': // ??00-23) 
				sprintf(dday+index, "%02d", tm_ptr->tm_hour ); 
				index += 2; 
				break; 
			case 's': // 珥?00-59) 
				sprintf(dday+index, "%02d", tm_ptr->tm_sec); 
				index += 2; 
				break; 

			default: 
				dday[index] = format[i]; 
				index++; 
		} 
	} 
	return dday; 
} /*}}}*/


void MemoryDisplay(INT32U offset,INT32U size)
{
	INT32U i,j;
	INT8U temp,buf[0x10];
	INT8U *pMem = (INT8U *)offset;

	while(size%16)size++;

	printk("\n");

	for(i=0; i<size; i++)
	{
		temp = pMem[i];
		
		if((i&0xF) == 0){
			if(i){
				for(j=0; j<0x10; j++){
					if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
						printk("%c",buf[j]);
					}
					else{
						printk(".");
					}
				}
			}
			printk("\n%05X : ",offset+i);
		}
		
		buf[i&0xF] = temp;
		
		printk("%02X ",temp);
	}

	for(j=0; j<0x10; j++){
		if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
			printk("%c",buf[j]);
		}
		else{
			printk(".");
		}
	}
	
	printk("\n");	
}


INT16U dbg_rtc_set(void)
{
	INT32U yy,mon,day,hh,mm,ss,wday;
	struct tm tm_ptr;
  
	if(CLI_GetArgCnt() < 7) return INV_PRM;
	
	yy  = comparison_num((char *)P_ARG(1));
	mon = comparison_num((char *)P_ARG(2));
	day= comparison_num((char *)P_ARG(3));
	hh  = comparison_num((char *)P_ARG(4));
	mm  = comparison_num((char *)P_ARG(5));
	ss  = comparison_num((char *)P_ARG(6));
	wday= comparison_num((char *)P_ARG(7));

	MakeTimePtr(&tm_ptr,yy, mon, day, hh, mm, ss,wday);
	SetRTCTime(&tm_ptr);

	return NO_DISPLAY;
}


INT16U dbg_rtc_gettime(void)
{

	sysinfo_t *s = &_sysinfo 	;

	printk("Read RTC Sec = %s\n",btime2str(&s->tm_ptr, "y-m-d h:mi:s"));

    return NO_DISPLAY;

}


INT16U dbg_backupsramwr_test(void)
{
	INT16U size, addr;
  static INT8U BS_RxB[100];
  
	if(CLI_GetArgCnt() < 2) return INV_PRM;
	
	addr = comparison_num((char *)P_ARG(1));
	size = comparison_num((char *)P_ARG(2));

  if(size > 100) size = 100;
	read_from_backup_sram( BS_RxB, size, addr);
	MemoryDisplay((INT32U)BS_RxB, size);

	return NO_DISPLAY;
}

INT16U dbg_backupsramrd_test(void)
{
	INT8U BS_RxB[16];
	INT8U i;

	for(i=0;i<sizeof(BS_RxB);i++)
	{
		BS_RxB[i] = 0x00;
	}
	read_from_backup_sram( BS_RxB, 16, 0);
	MemoryDisplay((INT32U)BS_RxB, 16);

	return NO_DISPLAY;
}


struct tm tm_time;
/*
********************************************************************************
* Description :   mktime conversion
******************************************************************************** 
*/
time_t mktime_conv(Time_t t) 
{
  tm_time.tm_sec = t.tm_sec;
  tm_time.tm_min = t.tm_min;
  tm_time.tm_hour = t.tm_hour;
  tm_time.tm_mday = t.tm_mday;
  tm_time.tm_mon = t.tm_mon-1;
  tm_time.tm_year = 2000+t.tm_year-1900;
  tm_time.tm_isdst = 0;
  return  mktime(&tm_time);
}  
  
/*
********************************************************************************
* Description :   gmtime conversion
******************************************************************************** 
*/
Time_t gmtime_conv(INT32U rtime)
{  
  Time_t local;
  struct tm *t;
  INT32U ltime;  
  
  ltime =  rtime;
  t = gmtime((time_t*)&ltime);
  local.tm_wday = (u8)(t->tm_wday);
  if(t->tm_year+1900 > 2000)
    local.tm_year = (u8)(t->tm_year+1900-2000);
  else local.tm_year = 0;
  local.tm_mon = (INT8U)(t->tm_mon+1);
  local.tm_mday = (INT8U)(t->tm_mday);
  local.tm_hour = (INT8U)(t->tm_hour);
  local.tm_min = (INT8U)(t->tm_min);
  local.tm_sec = (INT8U)(t->tm_sec);
  return local;
}

/*
********************************************************************************
* Description :   IsSameDate
******************************************************************************** 
*/
u8 IsSameDate(long time1, long time2) //두 날이 같으면 1 틀리면 0    [JCH.2014.03.14]
{
  long daycnt1, daycnt2;
  
  daycnt1 = time1/3600/24;
  daycnt2 = time2/3600/24;
  if(daycnt1 == daycnt2) return 1;
  else return 0;
}
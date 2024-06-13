/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : bsp.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 01/01/2014
*
* Description        : board configuration
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef  UTIL_H
#define  UTIL_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BUTIL_GLOBALS
#define  UTIL_EXT
#else
#define  UTIL_EXT  extern
#endif

/** System elapsed time, in milliseconds */ 
UTIL_EXT volatile INT32U systick_uptime_millis; 

UTIL_EXT char * btime2str(struct tm *tm_ptr, char *format);

#define	STIME(ptr)		btime2str(ptr, "y-m-d h:mi:s")
#define	PTIME(ptr)		btime2str(ptr, "ymdhmis")
#define	PHOUR_MIN(ptr)	btime2str(ptr, "hmi")
#define	WUITIME(ptr)	btime2str(ptr, "y.m.d-h:mi:s")
#define MKTIME(ptr)		btime2str(ptr, "h:mi:s")


UTIL_EXT void BSRAMSemaphore_Init(void);
UTIL_EXT INT32U systick_uptime(void);
UTIL_EXT INT8U write_to_backup_sram( INT8U *data, INT16U size, INT16U offset  );
UTIL_EXT INT8U read_from_backup_sram( INT8U *data, INT16U size, INT16U offset );
UTIL_EXT INT8U write_to_backup_rtc( INT32U *data, INT16U size, INT16U offset  );
UTIL_EXT INT8U read_from_backup_rtc( INT32U *data, INT16U size, INT16U offset );
UTIL_EXT void MemoryDisplay(INT32U offset,INT32U size);

UTIL_EXT void RTC_TimeShow(void);
UTIL_EXT void RTC_AlarmShow(void);
UTIL_EXT void RTC_DateShow(void);
UTIL_EXT void RTC_TimeSet(INT8U hours, INT8U Min, INT8U sec);
UTIL_EXT void RTC_TimeRegulate(struct tm *tm_ptr);
UTIL_EXT void GetRtcTime(struct tm *tm_ptr);
UTIL_EXT void MakeTimePtr(struct tm *tm_ptr,INT32U year, INT8U mon, INT8U mday, INT8U hours, INT8U mins, INT8U sec, INT8U wday);
UTIL_EXT void SetRTCTime(struct tm *tm_ptr);
UTIL_EXT INT8U TimeParsorSet(char *data);
UTIL_EXT void ResponseTime(char *data);






UTIL_EXT INT16U dbg_rtc_set(void);
UTIL_EXT INT16U dbg_rtc_gettime(void);
UTIL_EXT INT16U dbg_backupsramwr_test(void);



#define	BSPU_COMMANDS	\
	{"tset",    dbg_rtc_set, ALL_DIR,"tset 14 12 15 17 53 00, time set"},\
	{"tget",    dbg_rtc_gettime, ALL_DIR,"tget time get"},\
	{"bswr",    dbg_backupsramwr_test, ALL_DIR,"bswr 0xff"},\

#endif
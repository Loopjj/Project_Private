/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : rtc.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/07/2017
*
* Description        : rtc
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __RTC_H
#define __RTC_H

#ifdef RTC_GLOBALS
#define RTC_EXT
#else
#define RTC_EXT	extern
#endif 

#include <time.h>

__packed typedef struct {
  unsigned char tm_year;
  unsigned char tm_mon;
  unsigned char tm_mday;
  unsigned char tm_hour;
  unsigned char tm_min;
  unsigned char tm_sec;
  unsigned char tm_wday;
  unsigned char tm_yday;
}rtc_time ;
/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
RTC_EXT RTC_HandleTypeDef hrtc;
RTC_EXT RTC_TimeTypeDef sTime;
RTC_EXT RTC_DateTypeDef sDate;

RTC_EXT void MX_RTC_Init(void);

RTC_EXT rtc_time RTC_Val;
RTC_EXT time_t RTC_LVal;
RTC_EXT rtc_time gmtime_conv(time_t rtime);
RTC_EXT time_t RTCValTotime(rtc_time rtc_val);

RTC_EXT void RTC_SetDateTime(uint8_t *dt);
RTC_EXT void RTC_GetDateTime(void);
RTC_EXT void RTC_BKRam_Init(void);
RTC_EXT void RTC_WriteBackupRegister(uint32_t backupRegister, uint32_t data);
RTC_EXT uint32_t RTC_ReadBackupRegister(uint32_t backupRegister);


RTC_EXT INT16U dbg_backupsramwr_test(void);

#define RTC_COMMANDS \
		{"bkramr",    dbg_backupsramwr_test, ALL_DIR, "bkramr address size"},\

#endif
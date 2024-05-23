/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : alarmbell.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 19/11/2018
*
* Description        : alarm bell process
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/
#ifndef __CHILDALARM_H
#define __CHILDALARM_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef CHILDALARM_GLOBALS
#define CHILDALARM_EXT
#else
#define CHILDALARM_EXT	extern
#endif 

typedef enum {
  CHILD_KEY_CHECK,
  CHILD_WAIT_ALARM,
  CHILD_ALARM_ON,
  CHILD_ALARM_IDLE,
  CHILD_ALARM_OFF,
  CHILD_POWER_OFF  
}teChild_Step;

__packed typedef struct {
  INT32U  Time; 
  INT8U   Alarm;
  INT32U  Lon;      
  INT32U  Lat;      
  INT16U  Azi;         
  INT8U   Speed;  
}tsChildAlarm_Data;

__packed typedef struct {
  INT8U  PreChildAlarm; 
  tsChildAlarm_Data ChildAlarm_Data;
}tsChildAlarm_Event;

CHILDALARM_EXT INT8U ChildAlarmStep;
CHILDALARM_EXT INT8U ChildAlarmEnable, ChildAlarmOut;
CHILDALARM_EXT INT8U ChildServerFlag, VdisChildServerFlag;
CHILDALARM_EXT INT8U VdisChildEndFlag;
CHILDALARM_EXT int VdisChildRetryCount;
CHILDALARM_EXT INT8U ChildAlarmOffFlag;

CHILDALARM_EXT tsChildAlarm_Event ChildAlarm_Event;
CHILDALARM_EXT tsChildAlarm_Data  TxChildAlarm_Data;

CHILDALARM_EXT void CheckSleepingChildAlarm(void);
CHILDALARM_EXT void CheckSleepingChild(void);
CHILDALARM_EXT void CheckDigitalInput(void);
#endif
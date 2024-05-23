/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : alarmbell.c
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
/* Includes ------------------------------------------------------------------*/
#define CHILDALARM_GLOBALS
#include "includes.h"
  
/*
********************************************************************************
* Description : CheckChildAlarm
* Arguments   : 
* Return      : 
* Note        : Check Child Alarm Bell
******************************************************************************** 
*/
#define CHILD_ALARM_WAIT_LIMIT    110000  // 117*1000 milisec   1분50초..
#define CHILD_ALARM_WAIT1_LIMIT   140000  // 2*1000 milisec     30초..

void CheckSleepingChildAlarm(void)
{
#ifndef ISG_ENABLE  
#ifdef CHECK_SLEEPING_CHILD_ENABLE
  if((Setting.Can_Enable && CAN_Key_Off_Time > CAN_KEY_OFF_WAIT_LIMIT) || (KEY_OFF() && SecData.RPM < KEY_OFF_RPM_LOW_LIMIT)) {  
    if(ChildAlarmEnable == 0) ChildAlarmEnable = 1;
  }
  else {
//    if(ChildAlarmOffFlag) {
//      ChildAlarmStep = CHILD_ALARM_OFF;
//      TripStartTime = 0;
//    }      
//    else {  
      if(ChildAlarmEnable) TripStartTime = 0;
      ChildAlarmEnable = 0;
      ChildAlarmStep = CHILD_KEY_CHECK;
//    }
  }
  
  switch(ChildAlarmStep) {
    case CHILD_KEY_CHECK:
      if(ChildAlarmEnable == 1) {
        SecData.Signal.CheckChild = 1;        
        ChildAlarmStep = CHILD_WAIT_ALARM;
      }
      break;
    case CHILD_WAIT_ALARM:
      if(Key_Off_Time > CHILD_ALARM_WAIT_LIMIT)       // 1분50초 기다림
        ChildAlarmStep = CHILD_ALARM_ON;
      break;
    case CHILD_ALARM_ON:
      if(Key_Off_Time > CHILD_ALARM_WAIT1_LIMIT)      // 30초 안에 Alarm 없으면 Power Off
        ChildAlarmStep = CHILD_POWER_OFF;
      else {
        if(!VdisChildServerFlag && !DI1_SIG()) {      // Alarm  On 기다림
          SecData.Signal.ChildAlarm = 1;
          TxChildAlarm_Data.Alarm = 0;                // Alarm On : 0
          TxChildAlarm_Data.Time = SecData.Time;
          TxChildAlarm_Data.Lon = SecData.Lon;
          TxChildAlarm_Data.Lat = SecData.Lat;
          TxChildAlarm_Data.Azi = SecData.Azi;
          TxChildAlarm_Data.Speed = SecData.Speed[0];
          VdisChildServerFlag = 1;        
          ChildAlarmOffFlag = 1;
//          ChildAlarmStep = CHILD_ALARM_IDLE;
          ChildAlarmStep = CHILD_POWER_OFF;
        }
      }
      break;
    case CHILD_ALARM_IDLE:
      if(Key_Off_Time > BLE_SCAN_KEY_OFF_WAIT_LIMIT)  // 30분 안에 Alarm Off 없으면 Power Off
        ChildAlarmStep = CHILD_POWER_OFF;
      break;
    case CHILD_ALARM_OFF:                           
      if(!VdisChildServerFlag && DI1_SIG()) {         // Alarm Off 기다림
        SecData.Signal.ChildAlarm = 0;
        TxChildAlarm_Data.Alarm = 1;                  // Alarm Off : 1
        TxChildAlarm_Data.Time = SecData.Time;
        TxChildAlarm_Data.Lon = SecData.Lon;
        TxChildAlarm_Data.Lat = SecData.Lat;
        TxChildAlarm_Data.Azi = SecData.Azi;
        TxChildAlarm_Data.Speed = SecData.Speed[0];          
        VdisChildServerFlag = 1;
        ChildAlarmOffFlag = 0;
        ChildAlarmStep = CHILD_POWER_OFF;          
      }
      break;
    case CHILD_POWER_OFF:
      if(!VdisChildServerFlag) {
        SecData.Signal.CheckChild = 0;
//        TakeSendFlag = 1;                         // Event 보내고 종료 할거면 살려
        ChildAlarmEnable = 2;
      }
      break;
  }
#else
  ChildAlarmEnable = 0;
  SecData.Signal.CheckChild = 0;
  SecData.Signal.ChildAlarm = 0;
  ChildAlarmStep = CHILD_KEY_CHECK;
#endif    
#endif  
}

/*
********************************************************************************
* Description : CheckSleepingChild
* Arguments   : 
* Return      : 
* Note        : Check Sleep Child
******************************************************************************** 
*/
#define SLEEPING_CHILD_CHECK_WAIT_LIMIT  10000 // 40*1000 milisec   // 데모용 5초..
void CheckSleepingChild(void)
{
#ifndef ISG_ENABLE  
  #ifdef CHECK_SLEEPING_CHILD_ENABLE  
    if((Setting.Can_Enable && CAN_Key_Off_Time > CAN_KEY_OFF_WAIT_LIMIT) || (KEY_OFF() && SecData.RPM < KEY_OFF_RPM_LOW_LIMIT)) { 
      if(ChildAlarmEnable == 0) ChildAlarmEnable = 1;
      if(!DI1_SIG()) {
        SecData.Signal.CheckChild = 1;
        SecData.Signal.ChildAlarm = 0;
        if(ChildAlarmEnable == 1) {
          ChildAlarmEnable = 2;
          ChildAlarmOut = 0;      
          Key_Off_Time = 0;
          DO1_RESET();
        }
      }
      if(ChildAlarmEnable == 1 && Key_Off_Time > SLEEPING_CHILD_CHECK_WAIT_LIMIT) {
        SecData.Signal.ChildAlarm = 1;
        if(!ChildAlarmOut) {
          ChildAlarmOut = 1;
          DO1_SET();
        }
      }
    }
    else {
        ChildAlarmEnable = 0;
        SecData.Signal.CheckChild = 0;
        SecData.Signal.ChildAlarm = 0;
        if(ChildAlarmOut) {
          ChildAlarmOut = 0;          
          DO1_RESET();
        }
    }
  #else
    ChildAlarmEnable = 0;
    SecData.Signal.CheckChild = 0;
    SecData.Signal.ChildAlarm = 0;
  #endif    
#endif  
}

#ifdef DIGITAL_CHECK_ENABLE
/*
********************************************************************************
* Description : CheckSleepingChild
* Arguments   : 
* Return      : 
* Note        : Check Digital Input
******************************************************************************** 
*/
void CheckDigitalInput(void)
{
  if(DI2_SIG())
    SecData.Signal.Door = 0; 
  else 
    SecData.Signal.Door = 1; 
    
  if(DI1_SIG())
    SecData.Signal.CheckChild = 0; 
  else 
    SecData.Signal.CheckChild = 1;
}
#endif
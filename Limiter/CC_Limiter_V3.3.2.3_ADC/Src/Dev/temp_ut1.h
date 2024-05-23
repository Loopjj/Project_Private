/*
*************************************** (C) COPYRIGHT 2016 LOOP *************************************
*
* File Name          : temp_ut1.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 08/04/2016
*
* Description        : temperature sensor protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef UT1PROTOCOL_H
#define UT1PROTOCOL_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef UT1PROTOCOL_GLOBALS
#define UT1PROTOCOL_EXT
#else
#define UT1PROTOCOL_EXT	extern
#endif 

__packed typedef struct {
  u8 TempEnable       :1;
  u8 Ch1Enable         :1;
  u8 Ch1Short           :1;
  u8 Ch1Open           :1;
  u8 Ch2Enable         :1;
  u8 Ch2Short           :1;
  u8 Ch2Open           :1;
  u8 res                   :1;
}stTempStatus;

UT1PROTOCOL_EXT short Temp_dVal[2];
UT1PROTOCOL_EXT stTempStatus TempStatus;
UT1PROTOCOL_EXT void Temp_parse_proc(void);
UT1PROTOCOL_EXT void UT1_parse_proc(unsigned char r);
UT1PROTOCOL_EXT void CheckTemperatureEnable(void); 

#endif

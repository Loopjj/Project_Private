/*
*************************************** (C) COPYRIGHT 2019 Loop *************************************
*
* File Name          : modem_ADAS.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 02/19/2019
*
* Description        : ADAS transmit header
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef MODEM_ADAS_H
#define MODEM_ADAS_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef MODEM_ADAS_GLOBALS
#define MODEM_ADAS_EXT
#else
#define MODEM_ADAS_EXT	extern
#endif 

MODEM_ADAS_EXT u32 SendPolTimeCnt;

MODEM_ADAS_EXT u8 OnAdasFlag;
MODEM_ADAS_EXT u8 OnAdasStep;
MODEM_ADAS_EXT int OnAdasWaitCount;
MODEM_ADAS_EXT u8 OnAdasRespFlag;
MODEM_ADAS_EXT int OnAdasRetryCount;

MODEM_ADAS_EXT u8 SendAdasFlag;
MODEM_ADAS_EXT u8 SendAdasStep;
MODEM_ADAS_EXT int SendAdasWaitCount;
MODEM_ADAS_EXT u8 SendAdasRespFlag;
MODEM_ADAS_EXT int SendAdasRetryCount;

MODEM_ADAS_EXT u8 EventAdasFlag, EventTxComplete;
MODEM_ADAS_EXT u8 EventAdasStep;
MODEM_ADAS_EXT int EventAdasWaitCount;
MODEM_ADAS_EXT u8 EventAdasRespFlag;
MODEM_ADAS_EXT int EventAdasRetryCount;

MODEM_ADAS_EXT u8 OffAdasFlag;
MODEM_ADAS_EXT u8 OffAdasStep;
MODEM_ADAS_EXT int OffAdasWaitCount;
MODEM_ADAS_EXT u8 OffAdasRespFlag;
MODEM_ADAS_EXT int OffAdasRetryCount;

MODEM_ADAS_EXT u8 TripAdasFlag;
MODEM_ADAS_EXT u32 TripLapTimeCnt;
MODEM_ADAS_EXT u32 TripSpeedSum;

MODEM_ADAS_EXT tsSvrInfo AdasSvr;
MODEM_ADAS_EXT tsADASEventCount ADASEventCount, PreADASEventCount;
MODEM_ADAS_EXT tsADASPckNo ADASPckNo;
MODEM_ADAS_EXT tsADASPckData ADASPckData;

MODEM_ADAS_EXT void Modem_Init_ADAS(void);
MODEM_ADAS_EXT void Modem_SecInt_ADAS(void);
MODEM_ADAS_EXT void ADAS_Rx_Process(u8 r);
MODEM_ADAS_EXT void Modem_Parameter_Reset_ADAS(void);
MODEM_ADAS_EXT void Modem_DataIdleProcess_ADAS(void);
MODEM_ADAS_EXT void Modem_OnAdasProcess(void);
MODEM_ADAS_EXT void Modem_SendAdasProcess(void);
MODEM_ADAS_EXT void Modem_EventAdasProcess(void);
MODEM_ADAS_EXT void Modem_OffAdasProcess(void);
MODEM_ADAS_EXT void ADAS_Trip_Count(void);
#endif
/*
*************************************** (C) COPYRIGHT 2015 Loop *************************************
*
* File Name          : modem_TMX.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 06/28/2016
*
* Description        : modem header
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef MODEM_DMS_H
#define MODEM_DMS_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef MODEM_DMS_GLOBALS
#define MODEM_DMS_EXT
#else
#define MODEM_DMS_EXT	extern
#endif 

MODEM_DMS_EXT u32 FotaPolTimeCnt;
MODEM_DMS_EXT u32 QualPolTimeCnt;

MODEM_DMS_EXT tsSvrInfo DmSvr;
MODEM_DMS_EXT tsSvrInfo QualSvr;

MODEM_DMS_EXT u8 QualNotiFlag;
MODEM_DMS_EXT u8 OffNotiFlag;
MODEM_DMS_EXT u8 PushResetFlag;
MODEM_DMS_EXT u8 PushResetStart;

MODEM_DMS_EXT int QualNotiRetryCount;
MODEM_DMS_EXT int OffNotiRetryCount;
MODEM_DMS_EXT int FotaRetryCount;

MODEM_DMS_EXT void DMS_Rx_Process(u8  r);
MODEM_DMS_EXT void Modem_OnNotiProcess_DMS(void);
MODEM_DMS_EXT void Modem_QualNotiProcess_DMS(void);
MODEM_DMS_EXT void Modem_OffNotiProcess_DMS(void);
MODEM_DMS_EXT void Modem_FotaProcess_DMS(void);
MODEM_DMS_EXT void Modem_SecInt_DMS(void);
MODEM_DMS_EXT void Modem_DataIdleProcess_DMS(void);
MODEM_DMS_EXT void Modem_Parameter_Reset_DMS(void);
MODEM_DMS_EXT void Modem_Init_DMS(void);
#endif
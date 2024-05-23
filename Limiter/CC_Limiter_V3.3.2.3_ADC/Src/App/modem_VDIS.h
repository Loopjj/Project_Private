/*
*************************************** (C) COPYRIGHT 2015 Loop *************************************
*
* File Name          : modem_GVN.h
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

#ifndef MODEM_VDIS_H
#define MODEM_VDIS_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef MODEM_VDIS_GLOBALS
#define MODEM_VDIS_EXT
#else
#define MODEM_VDIS_EXT	extern
#endif 
MODEM_VDIS_EXT u8 Vdis_NeworkErrorFlag, Vdis_TxTerminateFlag;

MODEM_VDIS_EXT void Modem_RxCharGet_Vdis(unsigned char r);
MODEM_VDIS_EXT void Modem_RxChar_Vdis(u8 urc_mode, unsigned char r);
MODEM_VDIS_EXT void Modem_GetSvrProcess_Vdis(void);
MODEM_VDIS_EXT void Modem_VdisSvrProcess(void);
MODEM_VDIS_EXT void Modem_BleSvrProcess(void);
MODEM_VDIS_EXT void Modem_ChildAlarmSvrProcess(void);
MODEM_VDIS_EXT void Modem_SecInt_Vdis(void);
MODEM_VDIS_EXT void Modem_DataIdleProcess_Vdis(void);
MODEM_VDIS_EXT void Modem_Parameter_Reset_Vdis(void);
MODEM_VDIS_EXT void Modem_Init_Vdis(void);
#endif
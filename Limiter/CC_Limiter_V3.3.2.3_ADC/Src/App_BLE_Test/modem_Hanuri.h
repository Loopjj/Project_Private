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

#ifndef MODEM_HANURI_H
#define MODEM_HANURI_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef MODEM_HANURI_GLOBALS
#define MODEM_HANURI_EXT
#else
#define MODEM_HANURI_EXT	extern
#endif 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
MODEM_HANURI_EXT int Modem_RxChar_Hanuri(unsigned char r);
MODEM_HANURI_EXT void Modem_RxDecode_Hanuri(void);
MODEM_HANURI_EXT int Modem_TxData_Hanuri(unsigned char msg_id, unsigned char marker);
MODEM_HANURI_EXT void Modem_DataSvrProcess_Hanuri(void);
MODEM_HANURI_EXT void Modem_OnNotiProcess_Hanuri(void);
MODEM_HANURI_EXT void Modem_SecInt_Hanuri(void);
MODEM_HANURI_EXT void Modem_DataIdleProcess_Hanuri(void);
MODEM_HANURI_EXT void Modem_Init_Hanuri(void);
MODEM_HANURI_EXT void Modem_Parameter_Reset_Hanuri(void);

#endif
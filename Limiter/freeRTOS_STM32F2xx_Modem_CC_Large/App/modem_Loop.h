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

#ifndef MODEM_LOOP_H
#define MODEM_LOOP_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef MODEM_LOOP_GLOBALS
#define MODEM_LOOP_EXT
#else
#define MODEM_LOOP_EXT	extern
#endif 

MODEM_LOOP_EXT int Modem_RxChar_Loop(unsigned char r);
MODEM_LOOP_EXT void Modem_TxData_Loop(teCommand com, unsigned short idx, unsigned short cnt, unsigned int len, u8* data);
MODEM_LOOP_EXT void Modem_RxDecode_Loop(void);
MODEM_LOOP_EXT void Modem_SendData_Loop(u8 mode);
MODEM_LOOP_EXT void Modem_DataSvrProcess_Loop(void);
MODEM_LOOP_EXT void Modem_Init_Loop(void);
MODEM_LOOP_EXT void Modem_SecInt_Loop(void);
MODEM_LOOP_EXT void Modem_DataIdleProcess_Loop(void);
MODEM_LOOP_EXT void Modem_LoopFotaProcess(void);
#endif
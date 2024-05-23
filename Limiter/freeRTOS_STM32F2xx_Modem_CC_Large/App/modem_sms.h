/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : modem_sms.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : modem SMS
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/#ifndef __MODEM_SMS_H
#define __MODEM_SMS_H

#ifdef MODEM_SMS_GLOBALS
#define MODEM_SMS_EXT
#else
#define MODEM_SMS_EXT	extern
#endif 

MODEM_SMS_EXT void Modem_SMSSend(char* snum, char* msg, u8 len);
MODEM_SMS_EXT unsigned char  Modem_SMS_DeleteAll(void);
MODEM_SMS_EXT unsigned char  Modem_SMS_Delete(u8 idx);
MODEM_SMS_EXT void SMS_Process(u8  r);
MODEM_SMS_EXT void Modem_SMS_ReadAll(void);
MODEM_SMS_EXT void DecodeSMS(u8 msg_delete);

#endif
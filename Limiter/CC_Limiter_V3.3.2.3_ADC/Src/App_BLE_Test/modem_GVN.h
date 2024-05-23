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

#ifndef MODEM_GVN_H
#define MODEM_GVN_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef MODEM_GVN_GLOBALS
#define MODEM_GVN_EXT
#else
#define MODEM_GVN_EXT	extern
#endif 

MODEM_GVN_EXT u8 CLI_APN_ChangeFlag;
MODEM_GVN_EXT tsURLInfo URLInfo;
MODEM_GVN_EXT tsSDR_Policy SDRPolicy;
MODEM_GVN_EXT tsCLI_Msg CLI_Msg;
MODEM_GVN_EXT tsURLInfo BkURLInfo;

MODEM_GVN_EXT u8 SDR_Policy_State;
MODEM_GVN_EXT u8 CLI_State, CLI_Cmd;
MODEM_GVN_EXT u16 CLI_No;
MODEM_GVN_EXT u8 TxTerminateFlag, NeworkErrorFlag;
MODEM_GVN_EXT u8 SDRPolicyRetryDelayFlag, GV_DiscardFlag, GV_Discard_Cnt, GV_Term_Cnt;
MODEM_GVN_EXT u16 SDRPolicyRetryDelayCnt;

MODEM_GVN_EXT void Modem_RxChar_GV(unsigned char r);
MODEM_GVN_EXT u8 ServerDomain_Process_GVN(char *serverDamain, u8 len, u8 mode);
MODEM_GVN_EXT void CLI_Process_GVN(void);
MODEM_GVN_EXT void Modem_DataSvrProcess_GVN(void);
MODEM_GVN_EXT void Modem_SecInt_GVN(void);
MODEM_GVN_EXT void Modem_DataIdleProcess_GVN(void);
MODEM_GVN_EXT void Modem_URLChangeReset(void);
MODEM_GVN_EXT void Modem_ChangeURLInfo(void);
MODEM_GVN_EXT void Modem_Parameter_Reset_GVN(void);
MODEM_GVN_EXT void Modem_Init_GVN(void);
#endif
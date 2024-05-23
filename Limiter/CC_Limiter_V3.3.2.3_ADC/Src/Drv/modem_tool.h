/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : modem_tool.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : modem test & setting tool
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef MODEM_TOOL_H
#define MODEM_TOOL_H

#ifdef MODEM_TOOL_GLOBALS
#define MODEM_TOOL_EXT
#else
#define MODEM_TOOL_EXT	extern
#endif 

#define BLESCANNER        0
#define RFIDCARD          1
/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
MODEM_TOOL_EXT u8 ReceiveMediaFlag;
MODEM_TOOL_EXT u8 DTG_SendFlag, DTG_IntFlag;
MODEM_TOOL_EXT u8 MtTxAPNFlag;
MODEM_TOOL_EXT u8 MtReadAPNFlag,MtChangeAPNFlag;
MODEM_TOOL_EXT char MtTxBuff[256];
MODEM_TOOL_EXT void DTG_SecData_Send(void);
MODEM_TOOL_EXT void ModemTool_Process(void);
MODEM_TOOL_EXT void MtSendPacket(char cmd, char type,	char systype, char *psource, int dataLen);
#endif
/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : guiproto.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : gui protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef GUIPROTOCOL_H
#define GUIPROTOCOL_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef GUIPROTOCOL_GLOBALS
#define GUIPROTOCOL_EXT
#else
#define GUIPROTOCOL_EXT	extern
#endif

GUIPROTOCOL_EXT void Init_GUIcommRxProc(int port);
GUIPROTOCOL_EXT void RTC_ValSet(INT8U *data);
GUIPROTOCOL_EXT int RTC_ValGet(INT8U *pbuf);
GUIPROTOCOL_EXT int GUI_parse_proc(void);

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/

//---------------------------------------------------------------------- command
#define TYPE_HREPORT				0x10
#define TYPE_HSET					  0x20
#define TYPE_LREQUEST				0x01
#define TYPE_LRESPONSE			0x02

//--------------------------------------------------------- packet 예외처리 부분
#define START_PACKET				0x7E
#define END_PACKET					0x7F

#define CHKSUM_ERR					0x01
#define SYS_TYPE_ERR				0x02

#define GUI_BUFFSZ          128

/*
*********************************************************************************************************
*                                               TYPE DEFINE
*********************************************************************************************************
*/

typedef struct{
	char cmd;
	char type;
	char systype;
	char msg[GUI_BUFFSZ];
}protocol_t;

#endif
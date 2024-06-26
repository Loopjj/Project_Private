/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : jigproto.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : jig protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef JIGPROTOCOL_H
#define JIGPROTOCOL_H

#ifdef JIGPROTOCOL_GLOBALS
#define JIGPROTO_EXT
#else
#define JIGPROTO_EXT	extern
#endif 

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

JIGPROTO_EXT void JIG_task(void);

#endif
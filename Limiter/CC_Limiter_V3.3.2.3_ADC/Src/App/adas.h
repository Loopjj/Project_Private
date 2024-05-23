/*
*************************************** (C) COPYRIGHT 2016 LOOP *************************************
*
* File Name          : adas.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 30/01/2019
*
* Description        : adas protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef ADAS_H
#define ADAS_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef ADAS_GLOBALS
#define ADAS_EXT
#else
#define ADAS_EXT	extern
#endif 

ADAS_EXT u8 UartADAS_Enable, UartADAS_EnableCnt;
ADAS_EXT u8 UartADAS_LDWS, UartADAS_FCWS;

ADAS_EXT void CheckUartADASEnable(void);
ADAS_EXT void RxDataADAS(unsigned char r);

#endif
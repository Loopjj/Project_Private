/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : dtg.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : dtg process
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __DTG_H
#define __DTG_H

#ifdef DTG_GLOBALS
#define DTG_EXT
#else
#define DTG_EXT	extern
#endif 

DTG_EXT void Task_DTG(void const * argument);
#endif
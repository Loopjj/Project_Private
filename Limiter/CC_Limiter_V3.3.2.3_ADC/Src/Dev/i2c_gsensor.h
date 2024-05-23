/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : gsensor.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/08/2017
*
* Description        : Gsensor header 
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	GSENSOR_H
#define	GSENSOR_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef GSENSOR_GLOBALS
#define SEN_EXT
#else
#define SEN_EXT extern
#endif

#define LIS2DH12_RANGE_2GA	0x08
#define LIS2DH12_RANGE_4GA	0x18
#define LIS2DH12_RANGE_8GA	0x28
#define LIS2DH12_RANGE_16GA	0x38

SEN_EXT s16 GS_Val[3],GS_Buff[3][10];
SEN_EXT s16 GS_Val_Conv[3], GS_Val_Max[3], GS_Val_Dir[3];
SEN_EXT s16 GS_Offset[3], GS_Val_Set[3]; //2.1.3


SEN_EXT void I2C1_ACC_Init(void);
SEN_EXT void I2C1_ACC_Read(void);

#endif
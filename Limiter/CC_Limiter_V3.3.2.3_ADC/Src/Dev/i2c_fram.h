/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : i2c_fram.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/13/2017
*
* Description        : F-Ram header 
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	__FRAM_H
#define	__FRAM_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef FRAM_GLOBALS
#define FRAM_EXT
#else
#define FRAM_EXT extern
#endif

FRAM_EXT void I2C2_FRAM_Init(void);
FRAM_EXT void FRAMByteWrite(u8 data, u16 addr);
FRAM_EXT void FRAMMultiWrite(u8 *data, u16 addr, u16 len);
FRAM_EXT void FRAMByteRead(u8* data, u16 addr);
FRAM_EXT void FRAMMultiRead(u8 *data, u16 addr, u16 len);
FRAM_EXT INT16U dbg_fram_read(void);
FRAM_EXT INT16U dbg_fram_write(void);

#define I2C_COMMANDS \
		{"fr",  dbg_fram_read,  ALL_DIR, "fr [addr] [size] fram read"},\
		{"fw",  dbg_fram_write, ALL_DIR, "fw [addr] [data] fram write 1 byte"},\

#endif
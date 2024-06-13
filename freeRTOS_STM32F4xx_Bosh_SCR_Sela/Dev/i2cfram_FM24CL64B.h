/*
*************************************** (C) COPYRIGHT 2013 loop *************************************
*
* File Name          : i2cfram_FM24CL64B.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/14/2014
*
* Description        : I2C1 line interface configuration
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	FRAMI2C_H
#define	FRAMI2C_H


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef I2CFRAM_GLOBALS
#define FRAM_EXT
#else
#define FRAM_EXT extern
#endif

#define I2C_DEV					I2C2

#define WAIT_TIME 5
#define SLAVE_ADDRESS			0xA0

#define I2C_TIMEOUT				((uint32_t)0x1000)
#define I2C_LONG_TIMEOUT		((uint32_t)(10 * I2C_TIMEOUT))

typedef enum {
	FRAM_SCR_SYSTEM_SAVE = 0,
  FRAM_SCR_MAP_SAVE,
  FRAM_SCR_FACTOR_SAVE,
  FRAM_SCR_CALIB_SAVE,
  FRAM_SCR_MAF_SAVE,
	FRAM_SCRSAVE_MAX
}fram_num_t;

typedef enum {TX = 0, RX = 1, TXREG = 2} I2C_Dir;

FRAM_EXT void framSemaphore_Init(void);
FRAM_EXT int WriteArray_i2c(int dest, char *src,int sz);
FRAM_EXT int ReadArray_i2c(char *target,int src,int sz);
FRAM_EXT void FRAM_page_readwrite(INT8U add_offset, INT8U save);

FRAM_EXT INT16U dbg_fram_read(void);
FRAM_EXT INT16U dbg_fram_write(void);


#define I2C_COMMANDS \
		{"fr",  dbg_fram_read,  ALL_DIR, "fr [addr] [size] fram read"},\
		{"fw",  dbg_fram_write, ALL_DIR, "fw [addr] [data] fram write 1 byte"},\


#endif
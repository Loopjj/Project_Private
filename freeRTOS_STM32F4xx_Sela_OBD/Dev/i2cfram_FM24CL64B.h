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

#define I2C_DEV					    I2C1

#define SLAVE_ADDRESS			  0xA0


typedef enum {
	RECORD_ADDR_SAVE = 0,
	DATA_ADDR_SAVE,
  SENT_ADDR_SAVE,
  SYSINFO_SAVE,
  CARINFO_SAVE,
  MODEMINFO_SAVE,
  LASTINFO_SAVE,
  APNNAME_SAVE,
  SCRCALIB_SAVE,
  PALARM_SAVE
}fram_num_t;

typedef enum {TX = 0, RX = 1, TXREG = 2} I2C_Dir;

FRAM_EXT void framSemaphore_Init(void);
FRAM_EXT int WriteArray_i2c(int dest, INT8U *src,int sz);
FRAM_EXT int ReadArray_i2c(INT8U *target,int src,int sz);
FRAM_EXT int WriteArray_i2c_multi(int dest, INT8U *src,int sz);
FRAM_EXT int ReadArray_i2c_multi(INT8U *target,int src,int sz);
FRAM_EXT int FRAMMultiWrite(INT8U *src, int dest, int sz);
FRAM_EXT int FRAMMultiRead(INT8U *target, int src, int sz);
FRAM_EXT int FRAMByteWrite(char data, int address);
FRAM_EXT int FRAMByteRead(char *data, int address);

FRAM_EXT INT16U dbg_fram_read(void);
FRAM_EXT INT16U dbg_fram_write(void);


#define I2C_COMMANDS \
		{"fr",  dbg_fram_read,  ALL_DIR, "fr [addr] [size] fram read"},\
		{"fw",  dbg_fram_write, ALL_DIR, "fw [addr] [data] fram write 1 byte"},\


#endif
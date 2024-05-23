/*
*************************************** (C) COPYRIGHT 2013 XNIL *************************************
*
* File Name          : nv.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 01/28/2013
*
* Description        :  
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	NV_H
#define	NV_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef NV_GLOBALS
#define NV_EXT
#else
#define NV_EXT extern
#endif



/*
*********************************************************************************************************
*                                              TYPE DEFINE 
*********************************************************************************************************
*/

// spi flash save point map
typedef enum {
	PROJECT_SAVE = 0,
	SYSTEM_SAVE,
	SPI_SAVE_MAX
}spi_flash_num_t;



/*
*********************************************************************************************************
*                                           GLOBAL Variable
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                           GLOBAL FUNCTIONS
*********************************************************************************************************
*/
NV_EXT void nvxSemaphore_init(void);
NV_EXT void SPI_FLASH_Member_Write(u8* pBuffer, u32 flashWaddr, u16 NumByteToWrite);
NV_EXT int SPI_FLASH_LoadSave(int itemNum, int rw);
NV_EXT INT16U GetFlashAreaCRC(int faddr, int size);
//NV_EXT int ExcelParam_LoadSave(char *pdata, int dataSz, int rw);
NV_EXT INT32S FlashBackupWriteByte(INT32U addr,INT8U Data);
NV_EXT INT8U FlashBackupReadByte(INT32U addr);


/*-------- dbg function --------*/
NV_EXT INT16U dbg_sflash_read_id(void);
NV_EXT INT16U dbg_sflash_dispMap(void);
NV_EXT INT16U dbg_sflash_smd(void);
NV_EXT INT16U dbg_sflash_modify(void);
NV_EXT INT16U dbg_sflash_flush(void);
NV_EXT INT16U dbg_sflash_flush_all(void);
NV_EXT INT16U dbg_sflash_flash_chip(void);
NV_EXT INT16U dbg_sflash_write(void);
NV_EXT INT16U dbg_sflash_read(void);
NV_EXT INT16U dbg_sflash_select(void);
NV_EXT INT16U dbg_inittest_select(void);
NV_EXT INT16U dbg_sflash_crc(void);


#define	SPIFLASH_DRV_COMMANDS	\
	{"sfmap", dbg_sflash_dispMap,   ALL_DIR, "flash map display"},\
	{"sfmd",  dbg_sflash_smd,       ALL_DIR, "sfmd [addr] memory read(4K)"},\
	{"sfmod", dbg_sflash_modify,    ALL_DIR, "sfmod [addr][data] data modify"},\
	{"sfse",  dbg_sflash_flush,     ALL_DIR, "sfse [addr] sector erase(4K)"},\
	{"sfsa",  dbg_sflash_flush_all, ALL_DIR, "erase all(MCU Area)"},\
	{"sfce",  dbg_sflash_flash_chip,ALL_DIR, "bulk erase all(64M)"},\
	{"sfid",  dbg_sflash_read_id,   ALL_DIR, "flash id read"},\
	{"sfw",   dbg_sflash_write,     ALL_DIR, "sfw [addr] [data] flash write 1 byte"},\
	{"sfr",   dbg_sflash_read,      ALL_DIR, "sfr [addr] flash read 1 byte"},\
	{"sfcrc", dbg_sflash_crc,		    ALL_DIR, "sfcrc [0:MCU] flash crc cal"},\
	{"fsel",  dbg_sflash_select,	  ALL_DIR, "fsel [0:MCU] -> flash control select"},\
	{"itest", dbg_inittest_select,	ALL_DIR, "itest -> Init test"},\



#endif
/*
********************* (C) COPYRIGHT 2011 XNIL ***************END OF FILE*****************************
*/

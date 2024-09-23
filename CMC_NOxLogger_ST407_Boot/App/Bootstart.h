/******************** (C) COPYRIGHT 2014  ******************************
* File Name          : includes.h
* Author             : Digital Development Team
* Version            : V1.0.0
* Date               : 08/24/2014
* Description        : Bootloader sequence header 
* Programer			 : MKH
*******************************************************************************/



/* Module --------------------------------------------------------------------*/
#ifndef	BOOTSTART_H
#define	BOOTSTART_H



/* Extern --------------------------------------------------------------------*/
#ifdef BOOT_GLOBALS
#define BOOT_EXT
#else
#define BOOT_EXT extern
#endif

#include "ff.h"

/* Define --------------------------------------------------------------------*/
#define PRINTK_BUFF_SIZE		  512



#define CRC_16_POLYNOMIAL		  0x00001021

// STM32 Internal Flash
#define INT_FLASH_PAGESIZE  	0x00000800
#define APPLICATION_ADDR		  0x08010000
#define APPLICATION_SIZE		  0x00040000

// Serial Flash Map (16Mbyte) : 0x000000 ~ 0xFFFFFF
//------------------------------ 0x000000
//|    Data  康开   |
//------------------------------ 0xD00000
//|    User  康开   |
//------------------------------ 0xE00000
//|   Download 康开  |
//------------------------------ 0xE80000
//|    Backup 康开   | 
//------------------------------ 0xFFFFFF

#define SPI_FLASH_PAGESIZE		0x00000100

#define PROJECT_INFO_BASE		  0x00D00000
#define SPIFLASH_DOWNMEM		  0x00E00000
#define SPIFLASH_BACKUPBASE 	0x00E80000

#define USER_FLASH_LAST_PAGE_ADDRESS  0x08050000

/* Typedef -------------------------------------------------------------------*/
typedef __packed struct{
	INT8U project_code[10];			
	INT8U boot_seq[10];
	INT16U download_code;				
	INT32U file_size;
	INT16U file_crc;
}Prjcode_t;

BOOT_EXT Prjcode_t tPrjcode;

/* Global functions ----------------------------------------------------------*/
BOOT_EXT void printd(char *fmt, ...);
BOOT_EXT void ApplicationJump(void);
BOOT_EXT void System_reset(void);
BOOT_EXT void percent_display(int cur,int max);

BOOT_EXT INT8U BackupIntToSpiflash(void);
BOOT_EXT INT8U SpiFlashFileCheck(Prjcode_t *p);
BOOT_EXT INT8U SpiFlashToIntFlash(Prjcode_t *p, INT32U SpiflashAddr);
BOOT_EXT INT16U GenerateCRC_gui(char *pszBuffer, INT32U nLength, INT16U last_CRC16);

BOOT_EXT INT8U AppFileCheck(Prjcode_t *p, FIL* fp);
BOOT_EXT INT8U SdFlashToIntFlash(Prjcode_t *p, FIL* fp);

#endif


/******************* (C) COPYRIGHT 2014  ***************END OF FILE****/

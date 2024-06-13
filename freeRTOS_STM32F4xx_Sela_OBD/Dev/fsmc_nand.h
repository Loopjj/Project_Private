/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               fsmc_nand.h
** Descriptions:            The FSMC NAND application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-16
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef __FSMC_NAND_H
#define __FSMC_NAND_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/

#ifdef NAND_GLOBALS
#define NAND_EXT
#else
#define NAND_EXT extern
#endif


#define NAND_DEBUG	0

typedef struct  
{ 
  INT16U Zone; 
  INT16U Block; 
  INT16U Page; 
} NAND_ADDRESS; 

__packed typedef struct {
	u8 BadBlockCount;
	u16 BadBlockAddr[50];
	u8 BadBlockCheckSum;
}st_BadBlockData;




/* Private define ------------------------------------------------------------*/

/* NAND Area definition   */
#define CMD_AREA                   (uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<17)  /* A17 = ALE high */

#define DATA_AREA                  ((uint32_t)0x00000000) 

/* FSMC NAND memory command */
#define	NAND_CMD_READ_1             ((uint8_t)0x00)
#define	NAND_CMD_READ_TRUE          ((uint8_t)0x30)

#define	NAND_CMD_RDCOPYBACK         ((uint8_t)0x00)
#define	NAND_CMD_RDCOPYBACK_TRUE    ((uint8_t)0x35)

#define NAND_CMD_PAGEPROGRAM        ((uint8_t)0x80)
#define NAND_CMD_PAGEPROGRAM_TRUE   ((uint8_t)0x10)

#define NAND_CMD_COPYBACKPGM        ((uint8_t)0x85)
#define NAND_CMD_COPYBACKPGM_TRUE   ((uint8_t)0x10)
	
#define NAND_CMD_ERASE0             ((uint8_t)0x60)
#define NAND_CMD_ERASE1             ((uint8_t)0xD0)  

#define NAND_CMD_READID             ((uint8_t)0x90)	
#define NAND_CMD_STATUS             ((uint8_t)0x70)
#define NAND_CMD_RESET              ((uint8_t)0xFF)

#define NAND_CMD_CACHEPGM           ((uint8_t)0x80)
#define NAND_CMD_CACHEPGM_TRUE      ((uint8_t)0x15)

#define NAND_CMD_RANDOMIN           ((uint8_t)0x85)
#define NAND_CMD_RANDOMOUT          ((uint8_t)0x05)
#define NAND_CMD_RANDOMOUT_TRUE     ((uint8_t)0xE0)

#define NAND_CMD_CACHERD_START      ((uint8_t)0x00)
#define NAND_CMD_CACHERD_START2     ((uint8_t)0x31)
#define NAND_CMD_CACHERD_EXIT       ((uint8_t)0x34)

/* NAND memory status */
#define NAND_VALID_ADDRESS         ((uint32_t)0x00000100)
#define NAND_INVALID_ADDRESS       ((uint32_t)0x00000200)
#define NAND_TIMEOUT_ERROR         ((uint32_t)0x00000400)
#define NAND_BUSY                  ((uint32_t)0x00000000)
#define NAND_ERROR                 ((uint32_t)0x00000001)
#define NAND_READY                 ((uint32_t)0x00000040)

/* FSMC NAND memory parameters */
/* for MT29F4G08ABAEAWP*/

#define NAND_PAGE_SIZE             ((u16)0x1000) /* 4096 bytes per page w/o Spare Area */
#define NAND_BLOCK_COUNT           ((u16)0x0040) /* 64x4096 bytes pages per block */
#define NAND_BLOCK_SIZE            (u32)((u32)NAND_PAGE_SIZE*(u32)NAND_BLOCK_COUNT) //
#define NAND_ZONE_SIZE             ((u16)0x0400) /* 8192 Block per zone */
#define NAND_SPARE_AREA_SIZE       ((u16)0x0040) /* last 64 bytes as spare area */
#define NAND_MAX_ZONE              ((u16)0x0004) /* 4 zones of 1024 block */

#define NAND_SIZE                   (u32)0x20000000 //512M BYTE = 0X20000000
#define NAND_SIZE_RESERVE           (u32)0x8000000  //512 Blocks
#define NAND_SIZE_DATA              (u32)(NAND_SIZE - NAND_SIZE_RESERVE)
#define NAND_BLOCK_NUM				      2048
#if 0
#define NAND_MEMORY_TEST_ADDR       (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*1)  //설정 영역 Addr  뒷단 12블럭
#define NAND_BADBLOCK_DATA_ADDR     (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*5)  //BAD BLOCK DATA  뒷단 50블럭 
#define NAND_BADBLOCK_BASE          (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*56)
#else

#define NAND_MEMORY_TEST_ADDR       (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*456)  //설정 영역 Addr  뒷단 12블럭
#define NAND_BADBLOCK_DATA_ADDR     (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*460)  //BAD BLOCK DATA  뒷단 50블럭 
#define NAND_BADBLOCK_BASE          (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*511)
#endif


/* FSMC NAND memory address computation */
#define ADDR_1st_CYCLE(ADDR)       (uint8_t)( (ADDR) & 0xFF)              /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (uint8_t)( ((ADDR)& 0x0F00) >> 8)      /* 2nd addressing cycle */
#define ADDR_3rd_CYCLE(ADDR)       (uint8_t)( ((ADDR)& 0xFF0000) >> 16)   /* 3rd addressing cycle */
#define ADDR_4th_CYCLE(ADDR)       (uint8_t)( ((ADDR)& 0xFF000000) >> 24) /* 4th addressing cycle */

/* Private function prototypes -----------------------------------------------*/

NAND_EXT st_BadBlockData BadBlockData;

NAND_EXT void FSMC_NAND_Init(void);
NAND_EXT void FSMC_NAND_Test(void);
NAND_EXT uint32_t FSMC_NAND_WriteSmallPage(uint8_t *pBuffer, uint32_t Address, uint32_t size);
NAND_EXT uint32_t FSMC_NAND_ReadSmallPage (uint8_t *pBuffer, uint32_t Address, uint32_t size);
NAND_EXT uint32_t FSMC_NAND_WriteSpareArea(uint8_t *pBuffer, uint32_t Address, uint32_t size);
NAND_EXT uint32_t FSMC_NAND_ReadSpareArea(uint8_t *pBuffer, uint32_t Address, uint32_t size);
NAND_EXT uint32_t FSMC_NAND_EraseBlock(uint32_t Address);
NAND_EXT uint32_t FSMC_NAND_Reset(void);
NAND_EXT uint32_t FSMC_NAND_GetStatus(void);
NAND_EXT uint32_t FSMC_NAND_ReadStatus(void);
NAND_EXT uint32_t FSMC_NAND_AddressIncrement(uint32_t* Address);
NAND_EXT void NandSemaphore_Init(void);

NAND_EXT INT32U Nand_Write_Buffer(INT8U *pBuffer, INT32U addr, INT16U len);
NAND_EXT INT32U Nand_Read_Buffer(INT8U *pBuffer, INT32U addr, INT16U len);
NAND_EXT void Nand_Read_BadblockData(void);
NAND_EXT INT32U Nand_Check_Badblock(INT32U addr);
NAND_EXT void Nand_Make_Badblock_Table(void);

NAND_EXT INT16U dbg_nandread_test(void);
NAND_EXT INT16U CMD_NAND_checkbb(void);
NAND_EXT INT16U dbg_nandErase_test(void);
NAND_EXT INT16U CMD_NAND_id(void);

#if NAND_DEBUG >0
NAND_EXT INT16U dbg_nandWriteRead_test(void);
NAND_EXT INT16U dbg_nandWrite_test(void);
NAND_EXT INT16U dbg_nandblockread_test(void);
NAND_EXT INT16U dbg_nandspareread_test(void);
NAND_EXT INT16U dbg_nandsparewrite_test(void);
#endif


#define NAND_COMMANDS \
		{"nandr",   dbg_nandread_test, ALL_DIR, "nandr addr size (max 4k)"},\
		{"nandb",   CMD_NAND_checkbb, ALL_DIR, "nandb bad block check"},\
		{"nandid",   CMD_NAND_id, ALL_DIR, "nandid nand identifier"},\
		{"nande",   dbg_nandErase_test, ALL_DIR, "nande address "},\


/*
		
		{"nandr",   dbg_nandblockread_test, ALL_DIR, "nandr address"},\
		{"nandsr",   dbg_nandspareread_test, ALL_DIR, "nandsr address"},\
		{"nandwr",   dbg_nandWriteRead_test, ALL_DIR, "nandwr address data"},\
		{"nandw",   dbg_nandWrite_test, ALL_DIR, "nandw address data "},\
		{"nandsw",   dbg_nandsparewrite_test, ALL_DIR, "nandsw address 0xff"},\
*/



#endif /* __FSMC_NAND_H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

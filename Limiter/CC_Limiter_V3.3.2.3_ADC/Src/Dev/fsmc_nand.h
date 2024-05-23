/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : fsmc_nand.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/11/2017
*
* Description        : nand flash for fsmc
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __NAND_H
#define __NAND_H

#ifdef NAND_GLOBALS
#define NAND_EXT
#else
#define NAND_EXT	extern
#endif 
/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
__packed typedef struct {
	uint8_t   BadBlockCount;
	uint16_t  BadBlockAddr[50];
	uint8_t   BadBlockCheckSum;
}st_BadBlockData;

/* Private define ------------------------------------------------------------*/
#define NAND_PAGE_SIZE              ((uint16_t)0x1000) /* 4096 bytes per page w/o Spare Area */
#define NAND_BLOCK_COUNT            ((uint16_t)0x0080) /* 128x4096 bytes pages per block */
#define NAND_BLOCK_SIZE             (uint32_t)((uint32_t)NAND_PAGE_SIZE*(uint32_t)NAND_BLOCK_COUNT) //
#define NAND_SPARE_AREA_SIZE        ((uint16_t)0x00e0) /* last 224 bytes as spare area */
#define NAND_BLOCK_NUM				      2048

#define NAND_SIZE                   (uint32_t)0x40000000 //1G BYTE
#define NAND_SIZE_RESERVE           (uint32_t)0x4000000  //128 Blocks
#define NAND_SIZE_DATA              (uint32_t)(NAND_SIZE - NAND_SIZE_RESERVE)
#define NAND_EVENT_DATA_ADDR        (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*0)  //10Block
#define NAND_DAILY_DATA_ADDR        (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*10)  //10Block  64*9~64*10 일 Index
#define NAND_DAILY_DATA_PAGE_SIZE   (10*NAND_BLOCK_COUNT) 
#define NAND_RESERVED_ADDR          (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*20)  // 50 Block
#define NAND_BADBLOCK_DATA_ADDR     (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*70)   //BAD BLOCK DATA  Max 49 Block 
#define NAND_BADBLOCK_BASE          (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*119)  //BAD BLOCK Index  1 Block
#define NAND_FW_BACK_ADDR           (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*120)  //(u32)(NAND_SIZE - NAND_BLOCK_SIZE*8)   
#define NAND_PROGRAM_ADDR           (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*122)  //(u32)(NAND_SIZE - NAND_BLOCK_SIZE*6)  
#define NAND_TRIP_BASE              (u32)(NAND_SIZE_DATA + NAND_BLOCK_SIZE*124)  //Not Use   
#define NAND_TRIP_MAX_COUNT         (u16)(NAND_BLOCK_SIZE * 4 / 16)
#define NAND_BAD_CHECK_BLOCK_NUM    2048 - 50 - 8  //2048 Block에서 Badblock 대체 블록과 뒷단 8Block 기존 Bootloader와 호환을 위해 제외 (프로그램 영역)) 
                                                   //--> 제품 제작시에 프로그램로딩 과정에서 프로그램  영역 체크 
#define NAND_BAD_BLOCK_NUM          49

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
NAND_EXT NAND_HandleTypeDef hnand1;
NAND_EXT NAND_IDTypeDef NAND_ID;
NAND_EXT st_BadBlockData BadBlockData;
NAND_EXT u8 NandTmpBuffer[NAND_PAGE_SIZE];
NAND_EXT u8  *Nand_Save_Buff;
NAND_EXT u16  BK_Sram_Addr; //0~4095

NAND_EXT u16 Nand_Block_Count;
NAND_EXT u32 Nand_Block_Size;
NAND_EXT u16 Nand_Block_Num;
NAND_EXT u16 Nand_Bad_Check_Block_Num;

NAND_EXT void MX_FSMC_Init(void);
NAND_EXT void FSMC_Nand_Init(void);
NAND_EXT uint32_t FSMC_NAND_Reset(void);

NAND_EXT DSTATUS USER_initialize (BYTE pdrv);    /* Physical drive nmuber to identify the drive */
NAND_EXT INT32U Nand_Read_Buffer(INT8U *pBuffer, INT32U addr, INT16U len);
NAND_EXT INT32U Nand_Write_Buffer(INT8U *pBuffer, INT32U addr, INT16U len);
NAND_EXT INT16U CMD_NAND_id(void);
NAND_EXT void Check_Nand_Id(void);

NAND_EXT void Nand_Write_SaveData(void);
NAND_EXT void Nand_Read_SecData(u8 *pBuffer, u32 addr, u16 len);
NAND_EXT void Nand_Write_DailyData(void);
NAND_EXT u8 Nand_Read_DailyData(u16 prev_cnt, u8* dest);
NAND_EXT u8 Nand_Read_DailyData_LG(u8* day_data);
NAND_EXT u8 Nand_Read_DailySetting(u16 prev_cnt, u8* dest);
NAND_EXT u8 Nand_Read_DailyDriverInfo(u16 prev_cnt, u8* dest);

NAND_EXT void MemoryDisplay(INT32U offset,INT32U size);
NAND_EXT INT16U dbg_nandread_test(void);
NAND_EXT INT16U dbg_nandblockread_test(void);
NAND_EXT INT16U CMD_NAND_checkbb(void);
NAND_EXT INT16U dbg_nandspareread_test(void);
NAND_EXT INT16U dbg_nandsparewrite_test(void);
NAND_EXT INT16U CMD_NAND_id(void);
NAND_EXT INT16U dbg_nandWriteRead_test(void);
NAND_EXT INT16U dbg_nandErase_test(void);
NAND_EXT INT16U dbg_nandWrite_test(void);

#define NAND_COMMANDS \
		{"nandid",  CMD_NAND_id, ALL_DIR, "nandid"},
#ifdef NAND_DBG_ENABLE
    {"nand",    dbg_nandread_test, ALL_DIR, "nand cmd addr size"},\
		{"nandr",   dbg_nandblockread_test, ALL_DIR, "nandr address"},\
		{"nandb",   CMD_NAND_checkbb, ALL_DIR, "nandb"},\
		{"nandsr",  dbg_nandspareread_test, ALL_DIR, "nandsr address"},\
		{"nandsw",  dbg_nandsparewrite_test, ALL_DIR, "nandsw address 0xff"},\
		{"nandwr",  dbg_nandWriteRead_test, ALL_DIR, "nandwr address data"},\
		{"nande",   dbg_nandErase_test, ALL_DIR, "nande address "},\
		{"nandw",   dbg_nandWrite_test, ALL_DIR, "nandw address data "},
#endif
#endif
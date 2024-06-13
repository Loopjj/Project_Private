/**
  ******************************************************************************
  * @file    include.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

#define DEF_TRUE			  1

#define GUI_PORT			  1	//UART2
#define DBG_PORT			  3	//UART4
#define CAN1_PORT			  2	//CAN1
#define CAN2_PORT			  4	//CAN2


#define OFF					    0
#define ON					    1
#define NWAIT				    2

#define FAIL				    0
#define OK					    1

#define FALSE				    0
#define TRUE				    1

#define NO_SAVE				  0
#define SAVE				    1

#define INT_ANT         0
#define EXT_ANT         1

#define INACTIVE        0
#define ACTIVE          1

#define USER_SET_OFF		0
#define USER_SET_ON			1

#define NOT_DETECTED    0
#define DETECTED        1

#define INLET				    0
#define OUTLET				  1

#define ALARMCLEAR			0
#define ALARMSET			  1

#define AUTO_MODE			  0
#define MANUAL_MODE			1   
#define ALPHA_MODE			2
#define DOSING_MODE			3   
   
#define START				    1
#define STOP				    0

#define __READ				  0
#define __SAVE				  1

#define B_FULL				  1
#define B_EMPTY				  0

/*
*********************************************************************************************************
*                                              SYSTEM DEVICE DEFINE
*********************************************************************************************************
*/


typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point   
												 */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */


/*
*********************************************************************************************************
*                                            STRUCTURE BASE
*********************************************************************************************************
*/

//---------------------------------------------------------------------//
//----------------------- Internal Flash Memory -----------------------//
// LookUp 테이블 저장 섹터 
#define LUP_TABLE_BASE			0x08050000
// Excel 데이터 Save
#define EXCEL_SAVE_BASE			0x08076000
// FSMC CS1 시작 번지 
#define FSMC_CS1_BASEADDR		0x60000000

#define nINTBLKSZ(A)			((((A) & (CODE_MEM_PAGESZ - 1)) < (CODE_MEM_PAGESZ - CRCSZ)) ? (((A) / CODE_MEM_PAGESZ) + 1) : (((A) / CODE_MEM_PAGESZ) + 2))
//---------------------------------------------------------------------//



//---------------------------------------------------------------------//
//----------------------- External Flash Memory -----------------------//

#define SPI_FLASH_PAGE			0x1000		// 4 KB
#define SPI_FLASH_BLOCK			0x10000		// 64 KB
#define FLASH_DOWN_BLOCKCOUNT	8


#define PAGESZ					SPI_FLASH_PAGE
#define CRCSZ					2


// 페이지의 끝 2Byte는 CRC를 저장한다
// 구조체의 SIZE를 FLASH SECTOR SIZE로 나눈 나머지가 FLASH SECTOR SIZE에서 2Byte(CRC 저장 공간)를 뺀 값보다 작으면 
// FLASH SECTOR 갯수는 구조체의 SIZE를 FLASH SECTOR SIZE로 나눈 몫 + 1 이고
// 그렇지 않으면 구조체의 SIZE를 FLASH SECTOR SIZE로 나눈 몫 + 2 의 값을 갖는다 
#define FLASHOFFSET(A)			(((sizeof(A) & (PAGESZ - 1)) < (PAGESZ - CRCSZ)) ? ((sizeof(A) / PAGESZ) + 1) : ((sizeof(A) / PAGESZ) + 2))
#define nEXTBLKSZ(A)			((((A) & (PAGESZ - 1)) < (PAGESZ - CRCSZ)) ? (((A) / PAGESZ) + 1) : (((A) / PAGESZ) + 2))


// External Flash Memory Address
// 각 구조체의 BASE 어드레스는 FLASH OFFSET에 FLASH SIZE를 곱한 값을 갖는다 
// 이전 구조체의 BASE 어드레스 값을 OFFSET으로 더해준다
#define PROJECT_BASE			0x00000000
#define SYSTEM_BASE				(PAGESZ * FLASHOFFSET(Prjcode_t)       + PROJECT_BASE)
//#define TABLE_PARAM_SAVE_BASE	(PAGESZ * FLASHOFFSET(SysControl_t)    + SYSTEM_BASE)
//#define DSP_DB_SAVE_BASE		(PAGESZ * FLASHOFFSET(table_nv_t)      + TABLE_PARAM_SAVE_BASE)
//#define DSP_PARAM_SAVE_BASE		(PAGESZ * FLASHOFFSET(total_table_t)   + DSP_DB_SAVE_BASE)
//#define RPT_DOWNLINK_BASE		(PAGESZ * FLASHOFFSET(dsp_t)           + DSP_PARAM_SAVE_BASE)
//#define RPT_UPLINK_BASE			(PAGESZ * FLASHOFFSET(RfControl_t)     + RPT_DOWNLINK_BASE)
//#define SELF_CONDITION_BASE		(PAGESZ * FLASHOFFSET(RfControl_t)     + RPT_UPLINK_BASE)
//#define MODEM_CONTROL_SAVE_BASE	(PAGESZ * FLASHOFFSET(Self_Condition_t)+ SELF_CONDITION_BASE)

#define FRAM_SAVE_BASE			0x0000
#define FRAM_PARMSAVE_BASE	(FRAM_SAVE_BASE     + 0x100)	  //256 Byte
#define FRAM_MAPSAVE_BASE		(FRAM_PARMSAVE_BASE + 0x100)	  //768 Byte
#define FRAM_FACTOR_BASE    (FRAM_MAPSAVE_BASE  + 0x300)    //128 Byte
#define FRAM_CALIB_BASE     (FRAM_FACTOR_BASE   + 0x80)     //128 Byte
#define FRAM_MAF_BASE       (FRAM_CALIB_BASE   + 0x80)     //128 Byte
#define FRAM_SIZE_MAX			0x2000	//8KByte

// GUI 다운로드 베이스 (External Flash Memory)
#define DOWNLOAD_SECTOR			0x00200000



//---------------------------------------------------------------------//

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/

#ifdef DEBUG
#define DBGPRINTK(VAL)	printk("\n### %s %d(0x%X) ###\n",__func__,VAL,VAL)
#define DPRINTK(VAL)    printk(VAL)
#define DK(VAL)         printk(VAL);
#else
#define DBGPRINTK(VAL)
#define DPRINTK(VAL)
#endif

#define STR_DLUL(X)		(X?"UPLINK":"DOWNLINK")

#define FLOATSTR(X)		((int)((float)X*100))

#define FLOATdp(X)		((int)((float)(X)))
#define FLOATlp(X)		(abs(((int)(((float)(X))*10.0f ) % 10)))		

//#define SCR_USE_NAND	1



/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdint.h> 

#include "arch/cc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Rtimers.h"

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "core_cm4.h"



/* Includes ------------------------------------------------------------------*/
#include "system_cfg.h"
#include "bsp.h"
#include "cli.h"
#include "nv.h"
#include "tmr.h"
#include "commrtos.h"
#include "terminaldrv.h"
#include "debug.h"
#include "i2cfram_FM24CL64B.h"

#include "crc16.h"
#include "guiprotocol.h"

#include "spiflash_W25Q64BV.h"

#include "scr.h"
#include "sys_struct.h"
#include "OBDProtocol.h"
#include "ECUProtocol.h"
#include "Average.h"
#include "lwip/arch.h"
#include "ff.h"
#include "fs.h"
#include "lwip/sys.h"
#include "fsmc_nand.h"
#include "bsp_util.h"
#include "ieee754.h"
#include "pid.h"

#define GUITASK_NAME		"GUI"
#define CLITASK_NAME		"CLI"
#define TMRTASK_NAME		"TMR"
#define OBDTASK_NAME		"OBD"
#define ECUTASK_NAME		"ECU"
#define ADCTASK_NAME		"ADC"
#define SCRTASK_NAME		"SCR"

/* Task Priority init define */

#define CLI_TASK_PRIO	 ( tskIDLE_PRIORITY + 1 )
#define TMR_TASK_PRIO  ( tskIDLE_PRIORITY + 2 ) 
#define LMS_TASK_PRIO	 ( tskIDLE_PRIORITY + 3 )
#define OBD_TASK_PRIO	 ( tskIDLE_PRIORITY + 4 )
#define ECU_TASK_PRIO	 ( tskIDLE_PRIORITY + 5 )
#define ADC_TASK_PRIO	 ( tskIDLE_PRIORITY + 6 )
#define SCR_TASK_PRIO  ( tskIDLE_PRIORITY + 7 )

/* Software timer related definitions. */
//#if configUSE_TIMERS == 1
//#define configTIMER_TASK_PRIORITY               10
//#define configTIMER_QUEUE_LENGTH                10
//#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE
//#endif

extern xSemaphoreHandle SpiFlashDevSem;
extern xSemaphoreHandle BSP_SaveSem;
extern xSemaphoreHandle BSP_I2C1Sem;
extern xSemaphoreHandle BSP_NANDSem;
extern xSemaphoreHandle BSP_BKSramSem;
extern xSemaphoreHandle BSP_BKRtcSem;
extern xSemaphoreHandle BSP_Can2Trap;


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  
void Time_Update(void);
void Delay(uint32_t nCount);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


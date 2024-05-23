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

#define DEF_TRUE			1

#define GPS_PORT			0	//UART1   
#define MOD_PORT			1	//UART2
#define CAN1_PORT			2	//CAN1
   
#define OFF					  0
#define ON					  1
#define NWAIT				  2

#define FAIL				  0
#define OK					  1

#define FALSE				  0
#define TRUE				  1

#define NO_SAVE			  0
#define SAVE				  1

#define INT_ANT       0
#define EXT_ANT       1

#define INACTIVE      0
#define ACTIVE        1

#define USER_SET_OFF  0
#define USER_SET_ON		1

#define NOT_DETECTED  0
#define DETECTED      1

#define INLET				  0
#define OUTLET				1

#define ALARMCLEAR		0
#define ALARMSET			1

#define AUTO_MODE			0
#define MANUAL_MODE		1
#define NORMAL_MODE		2

#define START				  1
#define STOP				  0

#define __READ				0
#define __SAVE				1

#define B_FULL				1
#define B_EMPTY				0

#define EXT_RTCGET		0
#define	EXT_RTCSET		1
#define EXT_RTCWAT		2
#define EXT_CMDWAT		3

#define SAVE_SD				0
#define SAVE_USB			1
   
#define HEXTOBIN(ch) ((ch <= '9') ? ch - '0' : ch - ('A' - 10))

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

__packed typedef struct {
  INT8U tm_sec;
  INT8U tm_min;
  INT8U tm_hour;
  INT8U tm_mday;
  INT8U tm_mon;
  INT8U tm_year;
  INT8U tm_wday;
  INT8U tm_yday;
} Time_t;


/*
*********************************************************************************************************
*                                            STRUCTURE BASE
*********************************************************************************************************
*/

//---------------------------------------------------------------------//
//----------------------- Internal Flash Memory -----------------------//
// LookUp ���̺� ���� ���� 
#define LUP_TABLE_BASE			0x08050000
// Excel ������ Save
#define EXCEL_SAVE_BASE			0x08076000
// FSMC CS1 ���� ���� 
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


// �������� �� 2Byte�� CRC�� �����Ѵ�
// ����ü�� SIZE�� FLASH SECTOR SIZE�� ���� �������� FLASH SECTOR SIZE���� 2Byte(CRC ���� ����)�� �� ������ ������ 
// FLASH SECTOR ������ ����ü�� SIZE�� FLASH SECTOR SIZE�� ���� �� + 1 �̰�
// �׷��� ������ ����ü�� SIZE�� FLASH SECTOR SIZE�� ���� �� + 2 �� ���� ���´� 
#define FLASHOFFSET(A)			(((sizeof(A) & (PAGESZ - 1)) < (PAGESZ - CRCSZ)) ? ((sizeof(A) / PAGESZ) + 1) : ((sizeof(A) / PAGESZ) + 2))
#define nEXTBLKSZ(A)			((((A) & (PAGESZ - 1)) < (PAGESZ - CRCSZ)) ? (((A) / PAGESZ) + 1) : (((A) / PAGESZ) + 2))


// External Flash Memory Address
// �� ����ü�� BASE ��巹���� FLASH OFFSET�� FLASH SIZE�� ���� ���� ���´� 
// ���� ����ü�� BASE ��巹�� ���� OFFSET���� �����ش�

// Serial Flash Map (16Mbyte) : 0x000000 ~ 0xFFFFFF
//------------------------------ 0x000000
//|    Data  ����   |
//------------------------------ 0xD00000
//|    User  ����   |
//------------------------------ 0xE00000
//|   Download ����  |
//------------------------------ 0xE80000
//|    Backup ����   | 
//------------------------------ 0xFFFFFF

#define FLASH_DATA_BASE	    0x00000000    // (0x000000 ~ 0x00CFFFFF) 13M byte
#define PROJECT_BASE			  0x00D00000    // 0xD00000 ~ 0xD00FFF
#define SYSTEM_BASE				  (PAGESZ * FLASHOFFSET(Prjcode_t) + PROJECT_BASE)  // 0xD01000 ~ 0xDFFFFF
#define ECUDATA_SECTOR			0x00D80000    // GUI �ٿ�ε� ���̽� (External Flash Memory)
#define DOWNLOAD_SECTOR			0x00E00000    // GUI �ٿ�ε� ���̽� (External Flash Memory)
#define BACKUP_SECTOR			  0x00E80000    // GUI �ٿ�ε� ��� ���̽� (External Flash Memory)

#define FLASH_DATA_MAX      0x00400000    //4�� �����͸� �Ϸ� 12�ð� ���� �� 24�� ������ ���� ����
#define FLASH_DATA_SIZE     0x00D00000

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
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Rtimers.h"

#include "stm32f2xx_conf.h"
#include "stm32f2xx.h"
#include "core_cm3.h"

/* Includes ------------------------------------------------------------------*/
#include "system_cfg.h"
#include "bsp.h"

#include "cli.h"
#include "tmr.h"
#include "commrtos.h"
#include "terminaldrv.h"
#include "debug.h"
#include "nv.h"

#include "i2cfram_FM24CL64B.h"
#include "spiflash_W25Q64BV.h"
#include "spi_max6675.h"

#include "usb_defines.h"
#include "usb_hcd_int.h"
#include "usbh_usr.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"

#include "sys_struct.h"
#include "crc16.h"
#include "fnd.h"
#include "bsp_util.h"

#include "diskio.h"
#include "ff.h"
#include "ffconf.h"
#include "integer.h"
#include "fatfs_sd_sdio.h"
#include "fatfs_usb.h"
#include "usb_msc_app.h"
#include "fsmc_nand.h"
#include "can.h"
#include "key.h"
#include "extern.h"
#include "obd.h"
#include "drivefree.h"
#include "delay.h"
#include "modem.h"
#include "modem_Loop.h"
#include "modem_sms.h"
#include "avg.h"
#include "nmeap.h"
#include "nmeap_def.h"
#include "guiproto.h"

#define CLITASK_NAME		"CLI"
#define TMRTASK_NAME		"TMR"
#define CANTASK_NAME		"CAN"
#define MODTASK_NAME		"MOD"
#define OBDTASK_NAME		"OBD"
#define GPSTASK_NAME		"GPS"
#define KEYTASK_NAME		"KEY"

/* Task Priority init define */

#define TMR_TASK_PRIO  ( tskIDLE_PRIORITY + 1 )
#define MOD_TASK_PRIO	 ( tskIDLE_PRIORITY + 2 ) 
#define CAN_TASK_PRIO	 ( tskIDLE_PRIORITY + 3 )
#define OBD_TASK_PRIO	 ( tskIDLE_PRIORITY + 4 )
#define GPS_TASK_PRIO	 ( tskIDLE_PRIORITY + 5 )
#define KEY_TASK_PRIO	 ( tskIDLE_PRIORITY + 6 )
#define CLI_TASK_PRIO	 ( tskIDLE_PRIORITY + 7 )

extern xSemaphoreHandle SpiFlashDevSem;
extern xSemaphoreHandle BSP_SaveSem;
extern xSemaphoreHandle BSP_I2C1Sem;
extern xSemaphoreHandle BSP_NANDSem;
extern xSemaphoreHandle BSP_BKSramSem;
extern xSemaphoreHandle BSP_BKRtcSem;
extern xSemaphoreHandle BSP_CanSem;
extern xSemaphoreHandle BSP_KeySem;

extern FATFS USB_Fs,  SD_Fs; 
extern FIL   USB_Fil, SD_Fil;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


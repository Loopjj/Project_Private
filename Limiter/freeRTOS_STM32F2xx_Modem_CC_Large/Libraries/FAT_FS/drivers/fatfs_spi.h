/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2013          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED_SD
#define _DISKIO_DEFINED_SD

//#define _USE_WRITE	1	/* 1: Enable disk_write function */
//#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

#include "diskio.h"
#include "integer.h"

#include "stm32f2xx.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_gpio.h"
#include "misc.h"


#ifndef FATFS_SPI
#define FATFS_SPI							SPI1
#define FATFS_SPI_PINSPACK					TM_SPI_PinsPack_1
#endif

#ifndef FATFS_CS_PIN		
#define FATFS_CS_PORT						GPIOA
#define FATFS_CS_PIN						GPIO_Pin_4
#endif

#ifndef FATFS_USE_DETECT_PIN
#define FATFS_USE_DETECT_PIN				1
#endif

#ifndef FATFS_USE_WRITEPROTECT_PIN
#define FATFS_USE_WRITEPROTECT_PIN			0
#endif

#if FATFS_USE_DETECT_PIN > 0
#ifndef FATFS_USE_DETECT_PIN_PIN			
#define FATFS_USE_DETECT_PIN_PORT			GPIOE
#define FATFS_USE_DETECT_PIN_PIN			GPIO_Pin_3
#endif
#endif

#if FATFS_USE_WRITEPROTECT_PIN > 0
#ifndef FATFS_USE_WRITEPROTECT_PIN_PIN
#define FATFS_USE_WRITEPROTECT_PIN_RCC		RCC_AHB1Periph_GPIOB			
#define FATFS_USE_WRITEPROTECT_PIN_PORT		GPIOB
#define FATFS_USE_WRITEPROTECT_PIN_PIN		GPIO_Pin_7
#endif
#endif


#define FATFS_CS_LOW						GPIO_WriteBit(FATFS_CS_PORT,  FATFS_CS_PIN, Bit_RESET)
#define FATFS_CS_HIGH						GPIO_WriteBit(FATFS_CS_PORT,  FATFS_CS_PIN, Bit_SET)
#define SDCARD_DETECT()						GPIO_ReadInputDataBit(FATFS_USE_DETECT_PIN_PORT, FATFS_USE_DETECT_PIN_PIN)


/*---------------------------------------*/
/* Prototypes for disk control functions */
DSTATUS TM_FATFS_SD_disk_initialize(void);
DSTATUS TM_FATFS_SD_disk_status(void);
DRESULT TM_FATFS_SD_disk_read(BYTE* buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_disk_write(const BYTE* buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_disk_ioctl(BYTE cmd, void* buff);

#endif


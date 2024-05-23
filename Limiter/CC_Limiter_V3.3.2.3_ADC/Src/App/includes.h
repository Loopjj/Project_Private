/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : includes.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/05/2017
*
* Description        : includes 
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __INCLUDES_H
#define __INCLUDES_H

/*
*********************************************************************************************************
*                                            STRUCTURE BASE
*********************************************************************************************************
*/
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
typedef double         FP64;                     /* Double precision floating point   								 */
   
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
#include "typedef.h"

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "usb_host.h"  

#include "systemcfg.h"
#include "rtc.h"
#include "nmeap.h"
#include "cli.h"
#include "terminal.h"
#include "uart.h"
#include "usbuser.h"
#include "debug.h"

#include "sys_struct.h"
#include "extern.h"
#include "i2c_gsensor.h"
#include "fsmc_nand.h"
#include "spi_flash.h"
#include "i2c_fram.h"
#include "i2c_lcd.h"
#include "gui.h"
//#include "temp_ut1.h"
#include "adc.h"
#include "can.h"
#include "dtg.h"
#include "timer.h"
#include "bsp_int.h"
#include "delay.h"
#include "ble.h"
#include "modem.h"
#include "modem_sms.h"
//#include "mdt.h"
//#include "adas.h"
#include "test_jig.h"
#include "modem_tool.h"
//#include "blescan.h"
//#include "rfidcard.h"
//#include "childalarm.h"
//#include "modem_ADAS.h"

/* Private typedef -----------------------------------------------------------*/
#define OFF					  0
#define ON					  1
#define NWAIT				  2

#define FAIL				  0
#define OK					  1

#define FALSE				  0
#define TRUE				  1

#define NO_SAVE			  0
#define SAVE				  1

#define INACTIVE      0
#define ACTIVE        1

#define NOT_DETECTED  0
#define DETECTED      1

#define LIMITER_TEST
/* Private define ------------------------------------------------------------*/
// Fram Memory Address
#define FRAM_DATA_BASE	          0x0000                           // 16 Byte

#define FRAM_RES_BASE             (0x1000    + 64)   // 64Byte
#define FRAM_SIZE_MAX			        0x2000	//8KByte

/* Private macro -------------------------------------------------------------*/
  
#define USB_POWER_ON()			HAL_GPIO_WritePin(USB_PWR_CNTL_GPIO_Port, USB_PWR_CNTL_Pin, GPIO_PIN_SET)
#define USB_POWER_OFF()			HAL_GPIO_WritePin(USB_PWR_CNTL_GPIO_Port, USB_PWR_CNTL_Pin, GPIO_PIN_RESET)
#define GPS_POWER_ON()			HAL_GPIO_WritePin(GPS_PWR_CNTL_GPIO_Port, GPS_PWR_CNTL_Pin, GPIO_PIN_SET)
#define GPS_POWER_OFF()			HAL_GPIO_WritePin(GPS_PWR_CNTL_GPIO_Port, GPS_PWR_CNTL_Pin, GPIO_PIN_RESET)
#define POWER_ENABLE_ON()		HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_SET)
#define POWER_ENABLE_OFF()	HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_RESET)
#define CDMA_POWER_ON()			HAL_GPIO_WritePin(CDMA_PWR_CNTL_GPIO_Port, CDMA_PWR_CNTL_Pin, GPIO_PIN_SET)
#define CDMA_POWER_OFF()		HAL_GPIO_WritePin(CDMA_PWR_CNTL_GPIO_Port, CDMA_PWR_CNTL_Pin, GPIO_PIN_RESET)
#define BLE_POWER_ON()			HAL_GPIO_WritePin(BLE_RST_GPIO_Port, BLE_RST_Pin, GPIO_PIN_SET)
#define BLE_POWER_OFF()			HAL_GPIO_WritePin(BLE_RST_GPIO_Port, BLE_RST_Pin, GPIO_PIN_RESET)
#define BUZZER_ON()			    HAL_GPIO_WritePin(GPS_PWR_CNTL_GPIO_Port, GPS_PWR_CNTL_Pin, GPIO_PIN_SET)
#define BUZZER_OFF()			  HAL_GPIO_WritePin(GPS_PWR_CNTL_GPIO_Port, GPS_PWR_CNTL_Pin, GPIO_PIN_RESET)

#define WATCHDOG_TOGGLE()   HAL_GPIO_TogglePin(HW_WATCHDOG_GPIO_Port, HW_WATCHDOG_Pin)
#define LED_ALIVE_TOGGLE()  HAL_GPIO_TogglePin(LED_ALIVE_GPIO_Port, LED_ALIVE_Pin)
#define LED_DO_TOGGLE()     HAL_GPIO_TogglePin(DO1_GPIO_Port, DO1_Pin)
#define SPEED_SIG()         HAL_GPIO_ReadPin(SPEED_GPIO_Port, SPEED_Pin)
#define RPM_SIG()           HAL_GPIO_ReadPin(RPM_GPIO_Port, RPM_Pin)
#define BRAKE_SIG()         HAL_GPIO_ReadPin(BRAKE_GPIO_Port, BRAKE_Pin)
#define KEY_OFF()           HAL_GPIO_ReadPin(DI_KEY_GPIO_Port, DI_KEY_Pin)

#define DI1_SIG()           HAL_GPIO_ReadPin(DI_1_GPIO_Port, DI_1_Pin)
#define DI2_SIG()           HAL_GPIO_ReadPin(DI_2_GPIO_Port, DI_2_Pin)
#define DO1_SET()	      		HAL_GPIO_WritePin(DO1_GPIO_Port, DO1_Pin, GPIO_PIN_SET)
#define DO1_RESET()   			HAL_GPIO_WritePin(DO1_GPIO_Port, DO1_Pin, GPIO_PIN_RESET)
#define DO1_CHECK()   			HAL_GPIO_ReadPin(DO1_GPIO_Port, DO1_Pin)
#define DO2_SET()	      		HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_SET)
#define DO2_RESET()   			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_RESET)
#define DO2_CHECK()   			HAL_GPIO_ReadPin(DO2_GPIO_Port, DO2_Pin)

/* Private variables ---------------------------------------------------------*/

#endif /* __MAIN_H */   
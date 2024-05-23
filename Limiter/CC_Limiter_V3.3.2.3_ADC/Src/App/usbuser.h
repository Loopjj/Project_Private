/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : usbuser.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/05/2017
*
* Description        : user program for usb 
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __USBUSER_H
#define __USBUSER_H

#ifdef USBUSER_GLOBALS
#define USBUSER_EXT
#else
#define USBUSER_EXT	extern
#endif 

/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
USBUSER_EXT u8 USB_DataDL_Flag, USB_DataDL_Complete, USB_DataDL_LastTrip;
USBUSER_EXT u8 USB_FirmwareUpdate_Flag, USB_FirmwareUpdate_Complete;
USBUSER_EXT u8 USB_JigTest_Flag, USB_JigCompare_Flag;
USBUSER_EXT u8 USB_Detect_Flag;
USBUSER_EXT u8 USB_Download_Flag;
USBUSER_EXT stDailyData TmpDailyData;
USBUSER_EXT tsSecData TmpSecData;
USBUSER_EXT rtc_time DL_StartDay, DL_EndDay;
USBUSER_EXT u8 DownloadByDateFlag; 
USBUSER_EXT FRESULT Fresp;
USBUSER_EXT DIR Ddir;
USBUSER_EXT FILINFO Finfo;

USBUSER_EXT void Task_USB(void const * argument);
USBUSER_EXT void MSC_Application(void);
USBUSER_EXT void MX_USB_HOST_Process(void);
USBUSER_EXT void USB_Download(void);
USBUSER_EXT u8 USB_ParamDownload(void);
USBUSER_EXT void InitParamFileName(void);
#endif
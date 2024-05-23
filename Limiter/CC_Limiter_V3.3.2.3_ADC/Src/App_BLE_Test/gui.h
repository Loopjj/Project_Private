/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : gui.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : gui
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __GUI_H
#define __GUI_H

#ifdef GUI_GLOBALS
#define GUI_EXT
#else
#define GUI_EXT	extern
#endif 
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


extern const unsigned char bitmap_Download[];
extern const unsigned char bitmap_Down_end[];
extern const unsigned char bitmap_Down_Failed[];

GUI_EXT u8 Main_Disp_Mode, Display_Mode, Display_Flag;
GUI_EXT u8 Backlight_Cnt, Backligth_Sel;
GUI_EXT st_DRIVER_INFO Driver_Info[9];
GUI_EXT st_DRIVER_STS  Driver_Sts[9];
GUI_EXT u8 Dnld_Day_Cnt, Dnld_Hist_Cnt, Dnld_Hist[5][6];
GUI_EXT u8 Drv_Regist_Mode, Drv_Regist_Cnt;
GUI_EXT u8 Selected_Driver;
GUI_EXT u8 KeyOKLongFlag;
GUI_EXT u8 COM_Command_Char;
GUI_EXT u8 DriverNameDispFlag, DriverNameDispCnt;

GUI_EXT void Task_GUI(void const * argument);
GUI_EXT void Key_Scan(void);
GUI_EXT void Key_Process(void);
GUI_EXT void Disp_Init(void);
GUI_EXT void Display(void);
GUI_EXT u8 Check_KeyCancel(void);
GUI_EXT void DownloadDisplayInit(void);
GUI_EXT void Read_IsgMode(void);
GUI_EXT void Selected_Driver_Conversion(void);
GUI_EXT void Menu_DisplayTitle(u8 x, u8 y, u8 mode, u8 idx);
GUI_EXT void Setting_DisplayTitle(u8 x, u8 y, u8 mode, u8 idx);
GUI_EXT void Add_New_Driver(st_DRIVER_INFO *driver, u8 selset, u8 mode);
/* Private functions ---------------------------------------------------------*/

#endif
/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/
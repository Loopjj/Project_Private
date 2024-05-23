/*
*************************************** (C) COPYRIGHT 2015 Loop *************************************
*
* File Name          : obd.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 06/19/2015
*
* Description        : obd 
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef OBDSAVE_H
#define OBDSAVE_H



#ifdef OBD_GLOBALS
#define OBD_EXT
#else
#define OBD_EXT	extern
#endif

OBD_EXT void Task_OBD(void *p_arg);
OBD_EXT void DrvDataUpdate(void);
OBD_EXT void UpdateTimeToGPSTime(INT32U rtime);
OBD_EXT int Disk_Dtect(void);

OBD_EXT void Flash_Write_SaveData(void);
OBD_EXT void Flash_Read_SecData(u8 *pBuffer, u32 addr, u16 len);
OBD_EXT void DefaultSet(void);
OBD_EXT void InitPara(void);
OBD_EXT INT16U dbg_factory_init(void);
OBD_EXT INT16U dbg_scrclib_init(void);

#define	INIT_COMMANDS	\
{"finit",  dbg_factory_init,  ALL_DIR, "finit [pass] factory init"},\
{"cinit",  dbg_scrclib_init,  ALL_DIR, "cinit [pass] calib default init"}, \
  
#endif


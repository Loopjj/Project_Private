/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : can.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : CAN1 
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef CANPROTOOBD_H
#define CANPROTOOBD_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef CAN_GLOBALS
#define CAN_EXT
#else
#define CAN_EXT	extern
#endif 

#define CAN_STAN	CAN_ID_STD
#define CAN_EXTN	CAN_ID_EXT

#define MAX_CAN_MON_NO        10
#define MAX_CAN_DATA_LENGTH   8 


CAN_EXT int obd_debug;

__packed typedef struct {
  INT8U B;
  struct{
    INT8U State_0       :1;
    INT8U State_1       :1;
    INT8U State_2       :1;
    INT8U State_3       :1;
    INT8U State_4       :1;
    INT8U State_5       :1;
    INT8U State_6       :1;
    INT8U State_7       :1;
  };
}State_t;


/*
*********************************************************************************************************
*                                           GLOBAL variables
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           Functions
*********************************************************************************************************
*/
CAN_EXT void Task_CAN(void *p_arg);
CAN_EXT INT8U CanTxFile(void);
CAN_EXT INT8U CanFotaFile(void);
CAN_EXT void CAN_Decode(void);
CAN_EXT void MakeCanMassage(CanTxMsg *ptarget, INT32U id, INT8U type, INT8U rtr, INT8U *src, INT8U len);
CAN_EXT void SendSaveFlag(void);
CAN_EXT void SendResetFlag(void);

CAN_EXT INT16U dbg_ob_debug_test(void);

#define CAN_COMMANDS \
		{"nox",   dbg_ob_debug_test, ALL_DIR, "nox can data display"},\



#endif
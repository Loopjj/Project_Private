/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : can.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : can process
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __CAN_H
#define __CAN_H

#ifdef CAN_GLOBALS
#define CAN_EXT
#else
#define CAN_EXT	extern
#endif 
#define CAN_STAN	CAN_ID_STD
#define MAX_CAN_MON_NO        10
#define MAX_CAN_DATA_LENGTH   8

__packed typedef struct {
  INT32U NOxIn            :1;
  INT32U NOxOut           :1;
  INT32U TempIn           :1;
  INT32U TempOut          :1;
  INT32U UreaLevel        :1;
  INT32U DosingRate       :1;
  INT32U MAF              :1;
  INT32U Concentration    :1;
  INT32U SystemError      :1;
  INT32U DoserError       :1;
  INT32U Vbat             :1;
  INT32U State            :1;
  INT32U DpfPress         :1;
  INT32U res1             :19;  
}stCanEnable;

__packed typedef struct {
  INT16U NOxIn;
  INT16U NOxOut;
  INT16U TempIn;
  INT16U TempOut;
  INT16U UreaLevel;
  INT16U DosingRate;
  INT16U MAF;
  INT16U Concentration;
  INT16U SystemError;
  INT16U DoserError;
  INT16U Vbat;
  INT16U State;
  INT16U DpfPress;
}stCanEnableCnt;

#define CAN_Mode_Normal             ((uint8_t)0x00)  /*!< normal mode */
#define CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< loopback mode */
#define CAN_Mode_Silent             ((uint8_t)0x02)  /*!< silent mode */
#define CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< loopback combined with silent mode */

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

CAN_EXT CAN_HandleTypeDef hcan1;
CAN_EXT CAN_HandleTypeDef hcan2;

CAN_EXT stCanEnable CAN_Enable;
CAN_EXT u8 CAN_ErrorFlag, CAN_ErrorCnt;
CAN_EXT u8 EV_KeyOn;

CAN_EXT void MX_CAN1_Init(void);
CAN_EXT void MX_CAN2_Init(void);
CAN_EXT void CAN_Std_Decode(CAN_HandleTypeDef* hcan);
CAN_EXT void CAN_Ext_Decode(CAN_HandleTypeDef* hcan);
CAN_EXT void Task_CAN(void const * argument);
CAN_EXT void CheckCanDisable(void);
CAN_EXT void StepSendTrap(void);
CAN_EXT void Send_LimitStep(void);
#endif
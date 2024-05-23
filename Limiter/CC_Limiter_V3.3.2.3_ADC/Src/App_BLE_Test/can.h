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

typedef enum {
  SEND_PID_00  = 0, 
  CHECK_PID_00, 
  SEND_PID_40, 
  CHECK_PID_40, 
  SEND_PID_RPM, 
  CHECK_PID_RPM, 
  SEND_PID_SPEED, 
  CHECK_PID_SPEED, 
  SEND_PID_MAF, 
  CHECK_PID_MAF, 
  SEND_PID_FUELRATE, 
  CHECK_PID_FUELRATE, 
  J1979_IDLE
}en_J1979Prcess;

__packed typedef struct {
  INT32U Speed      :1;
  INT32U RPM        :1;
  INT32U Brake      :1;
  INT32U FuelRate   :1;
  INT32U KPL        :1;
  INT32U ODO        :1;
  INT32U Speed2     :1;
  INT32U RPM2       :1;
  INT32U Brake2     :1;
  INT32U TFuel      :1;
  INT32U KPL2       :1;
  INT32U ODO2       :1;
  INT32U MAF        :1;
  INT32U AEBS       :1;
  INT32U LDWS       :1;
  INT32U res0       :1;  
  INT32U PBrake      :1;
  INT32U LPAlarm     :1;
  INT32U CoolantTemp :1;
  INT32U DPF         :1;
  INT32U NGear       :1;  
  INT32U res1        :11;  
}stCanEnable;

__packed typedef struct {
  INT16U Speed;
  INT16U RPM;
  INT16U Brake;
  INT16U FuelRate;
  INT16U KPL;
  INT16U ODO;
  INT16U Speed2;
  INT16U RPM2;
  INT16U Brake2;
  INT16U TFuel;
  INT16U KPL2;
  INT16U ODO2;
  INT16U MAF;
  INT16U AEBS;
  INT16U LDWS;
  INT16U PBrake;
  INT16U LPAlarm;
  INT16U CoolantTemp;
  INT16U DPF;
  INT16U NGear;
}stCanEnableCnt;

__packed typedef struct {
  INT16U RPM;
  INT16U Speed;
  INT16U MAF;
  INT16U FuelRate;
  INT16U res[4];
}stJ1979EcuID;

__packed typedef struct {
  unsigned char   Speed;
  unsigned short  RPM;
  unsigned char   Brake;
  double          FuelRate;
  double          FuelRateSum;
  unsigned short  FuelRateCnt;
  double          InstKPL;
  double          InstKPLSum;
  unsigned short  InstKPLCnt;
  double          AvgKPL;
  double          TFuel;
  double          ODO;
  double          PODO;
  double          MAF;
  unsigned char   Time[6]; //sec, min, hour, mon, day, year  
  unsigned char   AEBS;
  unsigned char   LDWS1;
  unsigned char   LDWS2;  
  unsigned char   PBrake;  
  unsigned char   LPAlarm;  
  short           CoolantTemp;  
  unsigned char   NGear;  
  unsigned char   DPF;  
}stCanVal;

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
CAN_EXT stCanVal CAN_Val;
CAN_EXT u8 CAN_ErrorFlag, CAN_ErrorCnt;

CAN_EXT void MX_CAN1_Init(void);
CAN_EXT void MX_CAN2_Init(void);
CAN_EXT u8 CAN2_ADAS_TxData(void);
CAN_EXT void CAN_Std_Decode(CAN_HandleTypeDef* hcan);
CAN_EXT void CAN_Ext_Decode(CAN_HandleTypeDef* hcan);
CAN_EXT void Task_CAN(void const * argument);
CAN_EXT void CheckCanDisable(void);
#endif
/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : ble.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 02/01/2018
*
* Description        : ble
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __BLE_H
#define __BLE_H

#ifdef BLE_GLOBALS
#define BLE_EXT
#else
#define BLE_EXT	extern
#endif 

#define BLE_RX_BUFFER_SIZE  256 
#define BLE_MESSAGE_SIZE    256

#define BLE_RX_BUFFER_MASK (BLE_RX_BUFFER_SIZE - 1)
#if (BLE_RX_BUFFER_SIZE & BLE_RX_BUFFER_MASK)
	#error BLE RX buffer size is not a power of 2
#endif

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
__packed typedef struct {
  u8  Company_Name[12];     // company name :           회사 이름    
  u8  Company_Reg_No[10];   // company regist no :      사업자등록번호
  u8  Car_Type_No;          // car type no :            자동차 유형
  u8  VRN[12];              // car regist no :          자동차 등록번호 
  u8  Car_No[17];           // car no :                 차대번호        
  u8  Set_Direction;        // G Sensor
  u8  Can_Type;             // Can Type
  u16 CanBaudrate;          // Can Baudrate
  u8  CanMode;              // Can Mode
  u16 K_Factor;             // speed factor                        
  u16 RPM_Factor;           // rpm factor
  u16 Calib_F_Factor;
  u16 Dist_Percent;  
  s32 ODO_Offeset;     
}st_BufPara;

__packed typedef struct {
  u8  Driver_Name[10];     // driver name :           운전자 이름
  u8  Driver_Code[10];     // driver code :           운전자 코드
  u8  Driver_Number[7];    // driver number :         운전자 번호
}st_BufDriver;

__packed typedef struct {
  u8   IsgMode; 
  u16  IdleTimeLimit; 
  u16  BatVoltLimit;
  u8   CoolTempLimit; 
  u8   InclineLimit; 
  u8   PBrakeMode;  
  u8   AirPressMode;  
  u8   NGearMode; 
  u8   GpsMode; 
  u16  Radius;
}stBleIsgSetting;

BLE_EXT uint32_t __millisCounter;
BLE_EXT INT8U BLE_InitFLag;
BLE_EXT INT8U BLE_SmartInfoFlag;
BLE_EXT INT8U BLE_SendFlag, BLE_IntFlag, BLE_SmartFlag, BLE_SmartWaitFlag, BLE_SendHeaderFlag;
BLE_EXT INT8U	BLE_RxBuf[BLE_RX_BUFFER_SIZE];
BLE_EXT INT16U BLE_RxHead;
BLE_EXT INT16U BLE_RxTail;
BLE_EXT INT8U BLE_LastFLag;
BLE_EXT INT16U Ble_Wait_Cnt; 
BLE_EXT INT8U BLE_MSG_RxBuf[BLE_MESSAGE_SIZE];
BLE_EXT INT8U BLE_PinCode[6];
BLE_EXT void BLE_PinCode_Generation(void);
BLE_EXT void Task_BLE(void const * argument);

#endif
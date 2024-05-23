/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : blescan.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 03/30/2018
*
* Description        : ble secan data receive
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef BLESCAN_H
#define BLESCAN_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef BLESCAN_GLOBALS
#define BLESCAN_EXT
#else
#define BLESCAN_EXT	extern
#endif 

#define BLE_GAP_ADDR_LEN            6
#define SCAN_MAXNUM                 56 

__packed typedef struct {
  u8  addr[BLE_GAP_ADDR_LEN];
  s8  rssi;  
  u8  cnt;  
}tsBLE_Data;

__packed typedef struct {
  INT32S  RssiSum;
  INT16U  RssiCnt;
  INT8S   RssiAvg;
}tsBLE_Rssi;

__packed typedef struct {
  INT32U Time; 
  INT8U  CurTake;
  tsBLE_Data ScanData;
  INT32U  Lon;      
  INT32U  Lat;      
  INT16U  Azi;         
  INT8U   Speed;  
}tsBLE_Body;

__packed typedef struct {
  INT16U CheckCount;
  INT8U  PreTake; 
  tsBLE_Body BleBody;
  tsBLE_Rssi AddRssi;
  tsBLE_Rssi SubRssi;  
}tsBLE_Event;

BLESCAN_EXT INT8U BLE_TestSpeed;
BLESCAN_EXT INT8U BleEndFlag,VdisBleEndFlag;
BLESCAN_EXT INT8U BleServerFlag,VdisBleServerFlag;
BLESCAN_EXT int VdisBleRetryCount;
BLESCAN_EXT double BLE_SpeedCheckDist;

BLESCAN_EXT INT8U BLE_CheckVehicleStop;
BLESCAN_EXT INT16U BLE_SpeedCheckCount;
BLESCAN_EXT INT8U BLE_AddEnable;
BLESCAN_EXT INT16U BLE_ZeroCheckCount;
BLESCAN_EXT INT8U BLE_SubEnable;

BLESCAN_EXT INT8U TxEvent_Count, RxEvent_Count;
BLESCAN_EXT INT8U TxEvent_Index;
BLESCAN_EXT INT8U TxBLE_Index;
BLESCAN_EXT tsBLE_Data ScanData[SCAN_MAXNUM];
BLESCAN_EXT tsBLE_Event TxEvent[SCAN_MAXNUM];
BLESCAN_EXT tsBLE_Body  TxBLE_Body[SCAN_MAXNUM];
BLESCAN_EXT tsBLE_Body BLE_StopPoint;

BLESCAN_EXT void BLE_time_check(void);
BLESCAN_EXT void BLE_data_process(INT8U scancount);
BLESCAN_EXT void BLE_speed_check(INT8U speed);
BLESCAN_EXT void BLE_take_send(void);
BLESCAN_EXT void BLE_current_take(void);
#endif
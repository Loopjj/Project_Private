/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : rfidcard.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 13/07/2018
*
* Description        : rf id card reader
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/
#ifndef __RFIDCARD_H
#define __RFIDCARD_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
#ifdef RFIDCARD_GLOBALS
#define RFIDCARD_EXT
#else
#define RFIDCARD_EXT	extern
#endif 

#define RF_ID_LEN           10
#define CONTACT_MAXNUM      100

__packed typedef struct {
  INT32U  Time; 
  INT8U   CurTake;
  INT8U   ID[RF_ID_LEN];
  INT32U  Lon;      
  INT32U  Lat;      
  INT16U  Azi;         
  INT8U   Speed;  
}tsRFID_Data;

__packed typedef struct {
  INT8U  PreTake; 
  tsRFID_Data RFID_Data;
}tsRFID_Event;

RFIDCARD_EXT INT8U Contact_Index;
RFIDCARD_EXT INT8U Contact_Count;
RFIDCARD_EXT INT8U TxRFID_Index;;
RFIDCARD_EXT INT8U RxCardID[RF_ID_LEN];
RFIDCARD_EXT tsRFID_Event RFID_Event[CONTACT_MAXNUM];
RFIDCARD_EXT tsRFID_Data  TxRFID_Data[CONTACT_MAXNUM];

RFIDCARD_EXT void RFID_data_process(void);
RFIDCARD_EXT void RFID_tx_data_process(void);
RFIDCARD_EXT void RFID_time_check(void);
RFIDCARD_EXT void RFID_take_send(void);
RFIDCARD_EXT void RFID_current_take(void);
#endif

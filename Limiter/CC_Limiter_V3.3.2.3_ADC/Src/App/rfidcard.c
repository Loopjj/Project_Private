/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : rfidcard.c
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
/* Includes ------------------------------------------------------------------*/
#define RFIDCARD_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
#define RFID_CHECK_TIME     10
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
u8 ContactCheckCount;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*
********************************************************************************
* Description : RFID_data_process
* Arguments   : none
* Return      : none
* Note        : RFID receive data process
******************************************************************************** 
*/
void RFID_data_process(void) 
{
  int idx;
  INT8U cmp_flag;
  
  cmp_flag = 1;
  for(idx = 0; idx < Contact_Index; idx++) {
    if(strncmp((char *)&RFID_Event[idx].RFID_Data.ID, (char *)&RxCardID, RF_ID_LEN) == 0) {
      if(RFID_Event[idx].RFID_Data.CurTake) {
        Contact_Count--;
        RFID_Event[idx].RFID_Data.CurTake = 0;                // 하차        
      }
      else {
        Contact_Count++;
        RFID_Event[idx].RFID_Data.CurTake = 1;                // 승차        
      }
      RFID_Event[idx].RFID_Data.Time = SecData.Time;          // 승/하차 시간
      RFID_Event[idx].RFID_Data.Lon = SecData.Lon; 
      RFID_Event[idx].RFID_Data.Lat = SecData.Lat; 
      RFID_Event[idx].RFID_Data.Azi = SecData.Azi; 
      RFID_Event[idx].RFID_Data.Speed = SecData.Speed[0];
      MtSendPacket(0x61, 0x21, 0x01, (char*)&RFID_Event[idx].RFID_Data, sizeof(tsRFID_Data));  // RFID 리더로 응답        
      cmp_flag = 0;
      break;
    }    
  }
  
  if(cmp_flag) {
    memcpy((uint8_t *)&RFID_Event[idx].RFID_Data.ID, (uint8_t *)&RxCardID, RF_ID_LEN);
    if(!RFID_Event[idx].RFID_Data.CurTake) {
      Contact_Count++;
      RFID_Event[idx].RFID_Data.CurTake = 1;             // 승차
      RFID_Event[idx].RFID_Data.Time = SecData.Time;     // 승차 시간
      RFID_Event[idx].RFID_Data.Lat = SecData.Lat; 
      RFID_Event[idx].RFID_Data.Lon = SecData.Lon; 
      RFID_Event[idx].RFID_Data.Azi = SecData.Azi; 
      RFID_Event[idx].RFID_Data.Speed = SecData.Speed[0];
      MtSendPacket(0x61, 0x21, 0x01, (char*)&RFID_Event[idx].RFID_Data, sizeof(tsRFID_Data));  // RFID 리더로 응답      
    }
    Contact_Index++;
    if(Contact_Index > CONTACT_MAXNUM)
      Contact_Index = CONTACT_MAXNUM;    
  }
}

/*
********************************************************************************
* Description : RFID_tx_process
* Arguments   : none
* Return      : none
* Note        : RFID tx data process
******************************************************************************** 
*/
void RFID_tx_data_process(void) 
{
  int i, idx;
  INT8U index = 0;
  INT8U  cmpcnt;
  
  // 승하차 변경 사항 체크
  if(!BleServerFlag && !VdisBleServerFlag && !TakeSendFlag) {
    cmpcnt = 0;
    TxRFID_Index = 0;
    memset((uint8_t *)&TxRFID_Data, 0, sizeof(tsRFID_Data) * CONTACT_MAXNUM);
    for(idx = 0; idx < Contact_Index; idx++) {
      if(RFID_Event[idx].RFID_Data.CurTake != RFID_Event[idx].PreTake) {
        memcpy((uint8_t *)&TxRFID_Data[TxRFID_Index], (uint8_t *)&RFID_Event[idx].RFID_Data, sizeof(tsRFID_Data));
        TxRFID_Index++;
        cmpcnt++;
      }
    }      
    if(cmpcnt) {                                                      // 승하차 변동 사항 있으면 전송
      if(Setting.ExtModemMode == LOOP_PROTOCOL)
        BleServerFlag = 1;                                            // Event 데이터 전송
      if(Setting.ExtModemVDIS == VDIS_BLE_MODE) {
        VdisBleServerFlag = 1;
        VdisBleRetryCount = 0;
      }
      for(idx = 0; idx < Contact_Index; idx++)                       // 승하차 정보 백업
        RFID_Event[idx].PreTake = RFID_Event[idx].RFID_Data.CurTake;
      
      for(i=0; i < Contact_Index; i++) {
        if(RFID_Event[i].RFID_Data.CurTake == 1) {
          if(i > index) {
            memcpy((u8*)&RFID_Event[index], (u8*)&RFID_Event[i], sizeof(tsRFID_Event));
          }
          index++;
        }
      }
      if(Contact_Index != index) {
        for(i=index; i < Contact_Index; i++) {
          memset((u8*)&RFID_Event[i], 0, sizeof(tsRFID_Event));
        }
        Contact_Index = index;
      }      
    }
  }
}

/*
********************************************************************************
* Description : RFID_time_check
* Arguments   : none
* Return      : none
* Note        : RFID time check
******************************************************************************** 
*/
void RFID_time_check(void) 
{
  ContactCheckCount++;
  if(ContactCheckCount > RFID_CHECK_TIME) {
    RFID_tx_data_process();
    ContactCheckCount = 0;
  }
}

/*
********************************************************************************
* Description : RFID_take_send
* Arguments   : none
* Return      : none
* Note        : RFID send
******************************************************************************** 
*/ 
void RFID_take_send(void)
{
  int idx;
  INT8U  cmpcnt;
  if(Contact_Index) {
	  if(!TakeSendFlag) {
      if(!BleServerFlag && !VdisBleServerFlag) {
        cmpcnt = 0;
        TxRFID_Index = 0;
        for(idx = 0; idx < Contact_Index; idx++) {
          if(RFID_Event[idx].RFID_Data.CurTake) {
            memcpy((uint8_t *)&TxRFID_Data[idx], (uint8_t *)&RFID_Event[idx].RFID_Data, sizeof(tsRFID_Data));
            TxRFID_Data[idx].CurTake = 2;
            TxRFID_Index++;
            cmpcnt++;
          }
        }
        if(cmpcnt) {
          if(Setting.ExtModemMode == LOOP_PROTOCOL)
            BleServerFlag = 1;
        }
        TakeSendFlag = 1;        
      }
    }
  }
  else TakeSendFlag = 1;
}

/*
********************************************************************************
* Description : RFID_current_take
* Arguments   : none
* Return      : none
* Note        : RFID send
******************************************************************************** 
*/ 
void RFID_current_take(void)
{
  int idx;

  TxRFID_Index = 0;
  for(idx = 0; idx < Contact_Index; idx++) {
    if(RFID_Event[idx].RFID_Data.CurTake) {
      memcpy((uint8_t *)&TxRFID_Data[idx], (uint8_t *)&RFID_Event[idx].RFID_Data, sizeof(tsRFID_Data));
      TxRFID_Index++;
    }
  }

}

/*
********************* (C) COPYRIGHT 2018 LOOP ***************END OF FILE*****************************
*/
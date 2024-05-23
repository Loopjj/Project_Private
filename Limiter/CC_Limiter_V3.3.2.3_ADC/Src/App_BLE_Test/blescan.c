/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : blescan.c
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
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define BLESCAN_GLOBALS
#include "includes.h"

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
#define BLE_PACKET_BUFFSZ                 640
#define MAX_CHECK_TIME                    30

#define DETCT_ENABLE_COUNT                3
#define ZERO_ENABLE_COUNT                 90
#define BLE_SCAN_SPEED_LIMIT_COUNT        60    //60sec
#define BLE_SCAN_SPEED_LIMIT_DISTANCE     50    //50m
#define BLE_SCAN_ADD_ENABLE_COUNT_LIMIT   60    //60sec
#define BLE_SCAN_ZERO_LIMIT_COUNT         10    //10sec
#define BLE_SCAN_RSSI_DIFF                10    //-10dB

/*
********************************************************************************
* Description : BLE_time_check
* Arguments   : none
* Return      : none
* Note        : BLE time check
******************************************************************************** 
*/
void BLE_time_check(void) 
{
  int txidx;
    
  for(txidx = 0; txidx < TxEvent_Index; txidx++) {
    if(TxEvent[txidx].CheckCount > 0 ) {
     if(BLE_SubEnable)
        TxEvent[txidx].CheckCount--;
    }
    else {
      if(TxEvent[txidx].BleBody.CurTake) {
        TxEvent_Count--;        
        TxEvent[txidx].BleBody.CurTake = 0;                 // 하차 
        TxEvent[txidx].BleBody.Time = BLE_StopPoint.Time;   // 하차 시간 
        TxEvent[txidx].BleBody.Lat = BLE_StopPoint.Lat;
        TxEvent[txidx].BleBody.Lon = BLE_StopPoint.Lon;
        TxEvent[txidx].BleBody.Azi = BLE_StopPoint.Azi;
        TxEvent[txidx].BleBody.Speed = BLE_StopPoint.Speed;
      }
    }
  }
}
/*
********************************************************************************
* Description : BLE_speed_check
* Arguments   : speed
* Return      : none
* Note        : BLE speed check
******************************************************************************** 
*/
u16 BLE_SubEnable_Count, BLE_AddEnable_Count;

void BLE_speed_check(INT8U speed) 
{
  
  if(BLE_TestSpeed > 0 && speed == 0) speed = BLE_TestSpeed;

  //차량 정차 조건
  if(speed == 0) {
    BLE_ZeroCheckCount++;
    if(BLE_ZeroCheckCount > BLE_SCAN_ZERO_LIMIT_COUNT) {
      BLE_CheckVehicleStop = 1;
      if(!BLE_AddEnable) {
        BLE_StopPoint.Time = SecData.Time;     // 승차 시간
        BLE_StopPoint.Lat = SecData.Lat; 
        BLE_StopPoint.Lon = SecData.Lon; 
        BLE_StopPoint.Azi = SecData.Azi; 
        BLE_StopPoint.Speed = SecData.Speed[0]; 
      }
    }
    BLE_SpeedCheckCount = 0;
  }
  else {
    BLE_SpeedCheckCount++;
    if(BLE_SpeedCheckCount > BLE_SCAN_SPEED_LIMIT_COUNT) {
      BLE_CheckVehicleStop = 0;
    }
    BLE_ZeroCheckCount = 0;
  }
  // 승차시 속도 조건
  if(speed > 0) {
    if(!BLE_AddEnable && BLE_CheckVehicleStop) {
      BLE_SpeedCheckDist += (double)speed / 3.6;
      if(BLE_SpeedCheckDist > BLE_SCAN_SPEED_LIMIT_DISTANCE) {
        BLE_AddEnable = 1;
        BLE_CheckVehicleStop = 0;
        BLE_AddEnable_Count = 0;
      }
    }
  }
  // 승차 조건 해제  
  if(BLE_AddEnable) {
    BLE_AddEnable_Count++;  
    if(BLE_AddEnable_Count > BLE_SCAN_ADD_ENABLE_COUNT_LIMIT) {   //-- TimeOut
      BLE_AddEnable = 0;
      BLE_SpeedCheckDist = 0;
    }
    else if(TxEvent_Index == RxEvent_Count) {   // 스캔개수 == 탑승인원
      BLE_AddEnable = 0;
      BLE_SpeedCheckDist = 0;
    }
    else if((TxEvent_Index < RxEvent_Count) && BLE_CheckVehicleStop) {   // 스캔개수  > 탑승인원  && 10초이상 정지 상태  ==> 탑승장소...
      BLE_AddEnable = 0;
      BLE_SpeedCheckDist = 0;
    }
  }
  // 하차시 속도 조건
  if(speed == 0) {
    if(BLE_CheckVehicleStop) BLE_SubEnable = 1;
    BLE_SubEnable_Count = 0;
  }
  else {
    if(BLE_SubEnable_Count > ZERO_ENABLE_COUNT)
      BLE_SubEnable = 0;
    BLE_SubEnable_Count++;
  }
}

/*
********************************************************************************
* Description : BLE_data_process
* Arguments   : scan count
* Return      : none
* Note        : BLE receive data process
******************************************************************************** 
*/
INT8U DetectIndex, DetectAddr[SCAN_MAXNUM][BLE_GAP_ADDR_LEN];
INT16U DetectCount[SCAN_MAXNUM], PrevDetectCount[SCAN_MAXNUM];
INT8U SubRssiFlag;
void BLE_data_process(INT8U scancount)
{
  int i, rxidx, txidx, dtecidx, rssi_diff;
  INT8U  cmpflag, cmpcnt;
  INT8U index = 0;
  
  if(scancount > SCAN_MAXNUM) scancount = SCAN_MAXNUM;
  
  for(dtecidx = 0; dtecidx < DetectIndex; dtecidx++) {
    if(BLE_AddEnable) PrevDetectCount[dtecidx] = DetectCount[dtecidx];
    else PrevDetectCount[dtecidx] = 0;
    DetectCount[dtecidx] = 0;
  }
  
  for(rxidx = 0; rxidx < scancount; rxidx++) {
    if(BLE_AddEnable) { // && ScanData[rxidx].rssi > -80) {
      for(dtecidx = 0; dtecidx < DetectIndex; dtecidx++) {
        if(strncmp((char *)DetectAddr[dtecidx], (char *)&ScanData[rxidx].addr, BLE_GAP_ADDR_LEN) == 0) {
          DetectCount[rxidx] = PrevDetectCount[dtecidx]+1;
          break;
        }
      }
      if(dtecidx == DetectIndex) DetectCount[rxidx] = 1;
    }

    cmpflag = 1;
    for(txidx = 0; txidx < TxEvent_Index; txidx++) {          // 등록 데이터와 비교.
      if(strncmp((char *)&TxEvent[txidx].BleBody.ScanData.addr, (char *)&ScanData[rxidx].addr, BLE_GAP_ADDR_LEN) == 0) {
        if(SecData.Signal.KeyOn) {
          if(SecData.Speed[0] > 0) {
            TxEvent[txidx].AddRssi.RssiSum += ScanData[rxidx].rssi * ScanData[rxidx].cnt;
            TxEvent[txidx].AddRssi.RssiCnt += ScanData[rxidx].cnt;
            TxEvent[txidx].AddRssi.RssiAvg  = TxEvent[txidx].AddRssi.RssiSum / TxEvent[txidx].AddRssi.RssiCnt;
          }
          SubRssiFlag = 0;
        }
        else {
          if(!SubRssiFlag) {
            TxEvent[txidx].SubRssi.RssiSum = ScanData[rxidx].rssi * ScanData[rxidx].cnt;
            TxEvent[txidx].SubRssi.RssiCnt = ScanData[rxidx].cnt;
            TxEvent[txidx].SubRssi.RssiAvg = TxEvent[txidx].SubRssi.RssiSum / TxEvent[txidx].SubRssi.RssiCnt;            
            SubRssiFlag = 1;
          }
          else {
            TxEvent[txidx].SubRssi.RssiSum += ScanData[rxidx].rssi * ScanData[rxidx].cnt;
            TxEvent[txidx].SubRssi.RssiCnt += ScanData[rxidx].cnt;
            TxEvent[txidx].SubRssi.RssiAvg  = TxEvent[txidx].SubRssi.RssiSum / TxEvent[txidx].SubRssi.RssiCnt;
          }
          rssi_diff = TxEvent[txidx].AddRssi.RssiAvg - TxEvent[txidx].SubRssi.RssiAvg;
        }
        if(!SubRssiFlag || (SubRssiFlag && rssi_diff < BLE_SCAN_RSSI_DIFF))
          TxEvent[txidx].CheckCount = MAX_CHECK_TIME;       // 체크시간 초기화
        cmpflag = 0;
        break;
      }
    }
    
    if(cmpflag) {
      if(DetectCount[rxidx] >= DETCT_ENABLE_COUNT && BLE_AddEnable) {
        memcpy((uint8_t *)&TxEvent[txidx].BleBody.ScanData, (uint8_t *)&ScanData[rxidx], sizeof(tsBLE_Data));
        TxEvent[txidx].CheckCount = MAX_CHECK_TIME;         // 체크시간 초기화
        if(!TxEvent[txidx].BleBody.CurTake) {
          TxEvent_Count++;
          TxEvent[txidx].BleBody.CurTake = 1;               // 승차
          TxEvent[txidx].BleBody.Time = BLE_StopPoint.Time; // 승차 시간
          TxEvent[txidx].BleBody.Lat = BLE_StopPoint.Lat;
          TxEvent[txidx].BleBody.Lon = BLE_StopPoint.Lon;
          TxEvent[txidx].BleBody.Azi = BLE_StopPoint.Azi;
          TxEvent[txidx].BleBody.Speed = BLE_StopPoint.Speed;
          TxEvent[txidx].AddRssi.RssiSum = ScanData[rxidx].rssi * ScanData[rxidx].cnt;
          TxEvent[txidx].AddRssi.RssiCnt = ScanData[rxidx].cnt;
          TxEvent[txidx].AddRssi.RssiAvg = TxEvent[txidx].AddRssi.RssiSum / TxEvent[txidx].AddRssi.RssiCnt;
        }
        TxEvent_Index++;
        if(TxEvent_Index > SCAN_MAXNUM)
          TxEvent_Index = SCAN_MAXNUM;
      }
    }
  }
  
  memset((INT8U*)DetectAddr, 0, sizeof(DetectAddr));
  DetectIndex = 0;
  for(rxidx = 0; rxidx < scancount; rxidx++) {
    memcpy(DetectAddr[DetectIndex], (uint8_t *)&ScanData[rxidx].addr, BLE_GAP_ADDR_LEN);
    DetectIndex++;
  }
  
  // 승하차 변경 사항 체크
  if(!BleServerFlag && !VdisBleServerFlag && !TakeSendFlag) {
    cmpcnt = 0;
    TxBLE_Index = 0;
    memset((INT8U*)TxBLE_Body, 0, SCAN_MAXNUM * sizeof(tsBLE_Body));
    for(i=0; i < TxEvent_Index; i++) {
      if(TxEvent[i].BleBody.CurTake != TxEvent[i].PreTake) {
        memcpy((uint8_t *)&TxBLE_Body[TxBLE_Index], (uint8_t *)&TxEvent[i].BleBody, sizeof(tsBLE_Body));
        TxBLE_Index++;
        cmpcnt++;
      }
    }      
    if(cmpcnt) {                                          // 승하차 변동 사항 있으면 전송
      if(Setting.ExtModemMode == LOOP_PROTOCOL)
        BleServerFlag = 1;                                // Event 데이터 전송
      if(Setting.ExtModemVDIS == VDIS_BLE_MODE) {
        VdisBleServerFlag = 1;
        VdisBleRetryCount = 0;
      }
      for(i = 0; i < TxEvent_Index; i++)                  // 승하차 정보 백업
        TxEvent[i].PreTake = TxEvent[i].BleBody.CurTake;
      
      for(i = 0; i < TxEvent_Index; i++) {
        if(TxEvent[i].BleBody.CurTake == 1) {
          if(i > index) {
            memcpy((u8*)&TxEvent[index], (u8*)&TxEvent[i], sizeof(tsBLE_Event));
          }
          index++;
        } 
      }
      if(TxEvent_Index != index) {
        for(i=index; i < TxEvent_Index; i++) {
          memset((u8*)&TxEvent[i], 0, sizeof(tsBLE_Event));
        }
        TxEvent_Index = index;
      }
    }
  }
}

/*
********************************************************************************
* Description : BLE_take_send
* Arguments   : none
* Return      : none
* Note        : BLE send
******************************************************************************** 
*/ 
void BLE_take_send(void) 
{
  int idx;
  INT8U  cmpcnt;
  
	if(TxEvent_Index) {
	  if(!TakeSendFlag) {
      if(!BleServerFlag && !VdisBleServerFlag) {
        cmpcnt = 0;
        TxBLE_Index = 0; 
        for(idx = 0; idx < TxEvent_Index; idx++) {
          if(TxEvent[idx].BleBody.CurTake) {
            memcpy((uint8_t *)&TxBLE_Body[idx], (uint8_t *)&TxEvent[idx].BleBody, sizeof(tsBLE_Body));
            TxBLE_Body[idx].CurTake = 2;
            TxBLE_Index++;
            cmpcnt++;
          }
        }
        if(cmpcnt) BleServerFlag = 1;
        TakeSendFlag = 1;
      }
    }
  }
  else TakeSendFlag = 1;
}

/*
********************************************************************************
* Description : BLE_current_take
* Arguments   : none
* Return      : none
* Note        : BLE send
******************************************************************************** 
*/ 
void BLE_current_take(void)
{
  int idx;
  
  TxBLE_Index = 0; 
  for(idx = 0; idx < TxEvent_Index; idx++) {
    if(TxEvent[idx].BleBody.CurTake) {
      memcpy((uint8_t *)&TxBLE_Body[idx], (uint8_t *)&TxEvent[idx].BleBody, sizeof(tsBLE_Body));
      TxBLE_Index++;
    }
  }
}

/*
********************* (C) COPYRIGHT 2018 LOOP ***************END OF FILE*****************************
*/

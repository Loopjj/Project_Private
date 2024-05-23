/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : extern.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.1.0
*
* Date               : 07/29/2015
*
* Description        : extern 
*
*********************************************************************************************************
*/
#include "includes.h"

unsigned short DMS_ver;

const char Model_Name[15] = "DL-300-SCR";
const char PCB_HW_Ver[10] = "REV 1.00";
char Fota_FW_Ver[10];
char IP[4];

stScrValue ScrValue;
st_Setting Setting;

stSysControl SysCtl;
stScrMap ScrMap;
INT8U ServerRW = 0;

INT32U DataSaveAddr, DataSentAddr;
INT8U Start_Flag, SaveCnt = 0;
INT8U GPS_Speed_Val;

//- data save -//
stREAD_VALUE ReadValue, RD_Sec_Data;
stREAD_VALUE ReadValue_Buff[300];
INT8U *Flash_Save_Buff = 0;

INT16U Save_SecData_Cnt;
INT8U Save_SecData_Flag;      //0~9

// CAN Variables
xTaskHandle xHandle_can = NULL;
CanRxMsg RxMessage;
canrxmsg_t Can_RxMsg;

INT8U RSSI;
stSTATUS Sts;
Flags_t  Flags;
Flags_t1  Flags1;
//stPM1_Error PM1_Error;
//stPM2_Error PM2_Error;

FIL fp, fp1;
INT8U PreDay = 0;
char F_Name[12];
INT32U AccSize;
INT16U AccFiles, AccDirs, DelFiles;
INT8U Disk_Full;

INT8U nDlStatus = 0;  //ECU Download status
INT32U SendPacket = 0;
INT32U TotalPacket = 0;

KEYCHK_t key;
KEYFLAG_t keyflag;

INT8U DetectFlag = 0;
INT8U	BackUpFlag = 0;
INT8U SaveCount = 0;
INT8U SD_SaveCount = 0;

INT8U TerrorFlag = 0;
INT32U GPSTime;
INT16U SaveTimeCount;
INT8U  SaveTimeCheck;
Time_t RTC_Time;
INT32U RTC_LVal, RTC_GPS_Val;
INT16U HPLCnt = 0;
INT16U HPHCnt = 0;
INT16U HTLvLCnt = 0;
INT16U HTLvHCnt = 0;
INT16U QualityCnt = 0;
INT16U TLv_Beep = 0;
INT16U CANDataCnt = 0;
INT8U  buzerflag = 0;

INT16U DeltaP1 = 0, Pre_dP1 = 0, SumP1 = 0, IdleP1Cnt = 0;
INT16U P_BUF[12] = {0};
INT8U  P_Cnt = 0;

INT16U PLCal, PHCal;
INT16S TOffset, TSum;

INT16S NoxO2[2];
INT8U NoxFail[2], NoxErr[2], NoxData[2][3];

INT8U Par_Buf[64], bPar_Buf[64], Par_Flag = 0, Par_Cnt = 0;
INT16U Par_Time = 0;
INT8U Map_Buf[376], bMap_Buf[376], Map_Flag = 0, Map_Cnt = 0;
INT16U Map_Time = 0;

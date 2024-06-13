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
const char Model_Name_KT[15] = "LDT-300BS-ADAS";
const char PCB_HW_Ver[10] = "REV 1.00";
const char Approval_No[12] = "20-0001-00";
char Fota_FW_Ver[10];
char IP[4];

const char FW_Ver_Key[] = {'L','O','O','P',MAJOR+'L', MINOR+'O', RELEASE+'O', BUILD+'P', USE_MODEM};

stSCRData SecData;
st_Setting Setting;
stScrMap ScrMap;

INT32U DataSaveAddr, DataSentAddr, VdisSentAddr;
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
unsigned char datarx [8];
unsigned char RxCAN0Header;
unsigned char RxCAN0Flag[MAX_CAN_MON_NO];
unsigned char RxCAN0Buffer[MAX_CAN_MON_NO][MAX_CAN_DATA_LENGTH]; // ID, data buffer
unsigned int RxCAN0Data[MAX_CAN_MON_NO][MAX_CAN_DATA_LENGTH]; // ID, data buffer

INT8U RSSI;
Flags_t  Flags;
Flags_t1  Flags1;

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
st_Setting Setting;
INT16U HPLCnt = 0;
INT16U HPHCnt = 0;
INT16U HTLvLCnt = 0;
INT16U HTLvHCnt = 0;
INT16U TLv_Beep = 0;
INT16U CANDataCnt = 0;
INT16U UserSetCnt = 0;

INT16U DeltaP1 = 0, Pre_dP1 = 0, SumP1 = 0, IdleP1Cnt = 0;
INT16U P_BUF[12] = {0};
INT8U  P_Cnt = 0;

INT16U PLCal, PHCal;
INT16S TOffset, TSum;

INT8U  ECUFW_flag;
INT32U ECUFW_Size;
INT16U ECUFW_DVSize;
INT16U ECUFW_CRC16;

INT8U Par_Buf[64], bPar_Buf[64], Par_Flag = 0, Par_Cnt = 0;
INT16U Par_Time = 0;
INT8U Map_Buf[376], bMap_Buf[376], Map_Flag = 0, Map_Cnt = 0;
INT16U Map_Time = 0;

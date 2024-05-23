/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : dtg.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : dtg process
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __DTG_H
#define __DTG_H

#define CAN_KEY_OFF_WAIT_LIMIT       60       // 60  sec
#define KEY_OFF_RPM_LOW_LIMIT        100
#define DEFAULT_KEY_OFF_WAIT_LIMIT   3000    // 3*1000 milisec
#define BLE_SCAN_TAKE_WAIT_LIMIT     1740000 // 29*60*1000 milisec   //BLE SCAN적용시 29분뒤 잔류인원 보냄
#define BLE_SCAN_KEY_OFF_WAIT_LIMIT  1800000 // 30*60*1000 milisec   //BLE SCAN적용시 30분뒤에 무조건 꺼짐....
#define MAX_CAR_REGION  18
#define MAX_CAR_CLASS   51
#define MAX_CAR_TYPE    12

#ifdef DTG_GLOBALS
#define DTG_EXT

const u8 Car_No_Region[MAX_CAR_REGION][4] = 
{
  {0x00 , 0x00 , 0x00 , 0x00}, //없음
  {0xBC , 0xAD , 0xBF , 0xEF}, //서울
  {0xBA , 0xCE , 0xBB , 0xEA}, //부산
  {0xB4 , 0xEB , 0xB1 , 0xB8}, //대구
  {0xC0 , 0xCE , 0xC3 , 0xB5}, //인천
  {0xB1 , 0xA4 , 0xC1 , 0xD6}, //광주
  {0xB4 , 0xEB , 0xC0 , 0xFC}, //대전
  {0xBF , 0xEF , 0xBB , 0xEA}, //울산
  {0xB0 , 0xE6 , 0xB1 , 0xE2}, //경기
  {0xB0 , 0xAD , 0xBF , 0xF8}, //강원
  {0xC3 , 0xE6 , 0xBA , 0xCF}, //충북
  {0xC3 , 0xE6 , 0xB3 , 0xB2}, //충남
  {0xC0 , 0xFC , 0xBA , 0xCF}, //전북
  {0xC0 , 0xFC , 0xB3 , 0xB2}, //전남
  {0xB0 , 0xE6 , 0xBA , 0xCF}, //경북
  {0xB0 , 0xE6 , 0xB3 , 0xB2}, //경남
  {0xC1 , 0xA6 , 0xC1 , 0xD6}, //제주
  {0xBC , 0xBC , 0xC1 , 0xBE}  //세종
};

const u8 Car_No_Region_UTF8[MAX_CAR_REGION][4] = 
{
  {0x00, 0x00, 0x00, 0x00}, //없음
  {0xC1, 0x1C, 0xC6, 0xB8},
  {0xBD, 0x80, 0xC0, 0xB0},
  {0xB3, 0x00, 0xAD, 0x6C},
  {0xC7, 0x78, 0xCC, 0x9C},
  {0xAD, 0x11, 0xC8, 0xFC},
  {0xB3, 0x00, 0xC8, 0x04},
  {0xC6, 0xB8, 0xC0, 0xB0},
  {0xAC, 0xBD, 0xAE, 0x30},
  {0xAC, 0x15, 0xC6, 0xD0},
  {0xCD, 0xA9, 0xBD, 0x81},
  {0xCD, 0xA9, 0xB0, 0xA8},
  {0xC8, 0x04, 0xBD, 0x81},
  {0xC8, 0x04, 0xB0, 0xA8},
  {0xAC, 0xBD, 0xBD, 0x81},
  {0xAC, 0xBD, 0xB0, 0xA8},
  {0xC8, 0x1C, 0xC8, 0xFC},
  {0xC1, 0x38, 0xC8, 0x85} 
};

const u8 Car_No_Class[MAX_CAR_CLASS][2] = 
{
  {0xB0 , 0xA1} , //가    
  {0xB3 , 0xAA} , //나   
  {0xB4 , 0xD9} , //다    
  {0xB6 , 0xF3} , //라    
  {0xB8 , 0xB6} , //마
  {0xB0 , 0xC5} , //거
  {0xB3 , 0xCA} , //너
  {0xB4 , 0xF5} , //더
  {0xB7 , 0xAF} , //러
  {0xB8 , 0xD3} , //머
  {0xB9 , 0xF6} , //버
  {0xBC , 0xAD} , //서
  {0xBE , 0xEE} , //어
  {0xC0 , 0xFA} , //저
  {0xB0 , 0xED} , //고
  {0xB3 , 0xEB} , //노
  {0xB5 , 0xB5} , //도
  {0xB7 , 0xCE} , //로
  {0xB8 , 0xF0} , //모
  {0xBA , 0xB8} , //보
  {0xBC , 0xD2} , //소
  {0xBF , 0xC0} , //오
  {0xC1 , 0xB6} , //조
  {0xB1 , 0xB8} , //구
  {0xB4 , 0xA9} , //누
  {0xB5 , 0xCE} , //두
  {0xB7 , 0xE7} , //루
  {0xB9 , 0xAB} , //무
  {0xBA , 0xCE} , //부
  {0xBC , 0xF6} , //수
  {0xBF , 0xEC} , //우
  {0xC1 , 0xD6} , //주
  {0xB9 , 0xD9} , //바
  {0xBB , 0xE7} , //사
  {0xBE , 0xC6} , //아
  {0xC0 , 0xDA} , //자
  {0xC7 , 0xE3} , //허
  {0xBF , 0xDC} , //외교
  {0xBF , 0xB5} , //영사
  {0xC1 , 0xD8} , //준외
  {0xC1 , 0xD8} , //준영
  {0xB1 , 0xB9} , //국기
  {0xC7 , 0xF9} , //협정
  {0xB4 , 0xEB} , //대표
  {0xC0 , 0xB0} , //육
  {0xB0 , 0xF8} , //공
  {0xC7 , 0xD8} , //해
  {0xBE , 0xEE} , //어
  {0xB9 , 0xE8} , //배
  {0xC7 , 0xCF} , //하
  {0xC8 , 0xA3}   //호  
};

const u8 Car_No_Class_UTF8[MAX_CAR_CLASS][2] = 
{
  {0xAC, 0x00},
  {0xB0, 0x98},
  {0xB2, 0xE4},
  {0xB7, 0x7C},
  {0xB9, 0xC8},
  {0xAC, 0x70},
  {0xB1, 0x08},
  {0xB3, 0x54},
  {0xB7, 0xEC},
  {0xBA, 0x38},
  {0xBC, 0x84},
  {0xC1, 0x1C},
  {0xC5, 0xB4},
  {0xC8, 0x00},
  {0xAC, 0xE0},
  {0xB1, 0x78},
  {0xB3, 0xC4},
  {0xB8, 0x5C},
  {0xBA, 0xA8},
  {0xBC, 0xF4},
  {0xC1, 0x8C},
  {0xC6, 0x24},
  {0xC8, 0x70},
  {0xAD, 0x6C},
  {0xB2, 0x04},
  {0xB4, 0x50},
  {0xB8, 0xE8},
  {0xBB, 0x34},
  {0xBD, 0x80},
  {0xC2, 0x18},
  {0xC6, 0xB0},
  {0xC8, 0xFC},
  {0xBC, 0x14},
  {0xC0, 0xAC},
  {0xC5, 0x44},
  {0xC7, 0x90},
  {0xD5, 0xC8},
  {0xC6, 0x78},
  {0xC6, 0x01},
  {0xC9, 0x00},
  {0xC9, 0x00},
  {0xAD, 0x6D},
  {0xD6, 0x11},
  {0xB3, 0x00},
  {0xC7, 0x21},
  {0xAC, 0xF5},
  {0xD5, 0x74},
  {0xC5, 0xB4},
  {0xBC, 0x30},
  {0xD5, 0x58},
  {0xD6, 0x38}  
};

const u8 Car_Type_Code[MAX_CAR_TYPE] = 
{11, 12, 13, 14, 15, 16, 17, 21, 22, 31, 32, 41};

const char Car_Type_Name[MAX_CAR_TYPE][16] = 
{
"시내버스        ",
"농어촌버스      ",
"마을버스        ",
"시외버스        ",
"고속버스        ",
"전세버스        ",
"특수여객자동차  ",
"일반택시        ",
"개인택시        ",
"일반화물자동차  ",
"개별화물자동차  ",
"비사업용자동차  "
};

#pragma location = "EXTCCRAM"
tsSecData Save_SecData_Buff[300]; // nand 1sec * 10  

#else
#define DTG_EXT	extern

DTG_EXT const u8 Car_No_Region[MAX_CAR_REGION][4];
DTG_EXT const u8 Car_No_Region_UTF8[MAX_CAR_REGION][4];
DTG_EXT const u8 Car_No_Class[MAX_CAR_CLASS][2];
DTG_EXT const u8 Car_No_Class_UTF8[MAX_CAR_CLASS][2];
DTG_EXT const u8 Car_Type_Code[MAX_CAR_TYPE];
DTG_EXT const char Car_Type_Name[MAX_CAR_TYPE][16];
DTG_EXT tsSecData Save_SecData_Buff[300]; // nand 1sec * 10  
#endif 

DTG_EXT stRTC_Backup* RTC_BK_Addr;
DTG_EXT u8 Nand_Header_Flag;
DTG_EXT u8 Day_Change_Flag;
DTG_EXT u16 Idle_RPM;
DTG_EXT u8 Speed_Calib_Flag;
DTG_EXT u16 Calib_K_Factor;
DTG_EXT u16 Calib_F_Factor;
DTG_EXT u32 Calib_Speed_Sum1, Calib_Speed_Sum2;
DTG_EXT u8  Line_Speed;
DTG_EXT u32 Speed_Set_Cnt;
DTG_EXT u8  GPS_Speed_Val;
DTG_EXT u8  Power_Off_Flag;
DTG_EXT u8 Start_Flag;
DTG_EXT u8 NotRegist_Flag;
DTG_EXT u16  CAN_Key_Off, CAN_Key_Off_Time;
DTG_EXT u8 Power_Off_Mode;
DTG_EXT tsSecData SecData;
DTG_EXT u16 Save_SecData_Cnt;
DTG_EXT u8 Save_SecData_Flag;      //0~9
DTG_EXT stDailyData DailyData;
DTG_EXT st_Setting Setting;
DTG_EXT u32 DataSaveAddr, DataSentAddr, VdisSentAddr;
DTG_EXT float SecFuelRate,SecMTrip;
DTG_EXT u8 VRN[12];

DTG_EXT u16 IdleTimeCount, IdleStop_Time;
DTG_EXT u8 IsgAccOff;
DTG_EXT u8 IsgFuelSaveFlag;
DTG_EXT u32 TripStartTime;

DTG_EXT u32 KeyOffWaitTimeLimit;
DTG_EXT char Adas_Char, Prev_Adas_Char;

DTG_EXT u32 Key_Off_Time;

DTG_EXT u16 DeltaP1, Pre_dP1, SumP1, IdleP1Cnt;
DTG_EXT u16 P_BUF[12];
DTG_EXT u8  P_Cnt;

DTG_EXT u8  UreaSysErrFlag;
DTG_EXT u8  NOxValidFlag[2];

DTG_EXT stUreaAlarm UreaAlarm, PrevUreaAlarm, UreaAccControl;
DTG_EXT u32 UreaAlarmLapTime;
DTG_EXT u8 AccTestFlag;
DTG_EXT u8 UreaBuzzerFlag, BuzzerOnFlag, BuzzerOnCnt, BuzzerTimeCnt;
DTG_EXT u32 UreaBuzzerTime;
DTG_EXT u32 UreaTestFlag;

DTG_EXT u8 USB_IntFlag, SaveOkFlag;
DTG_EXT u8 PreDay;
DTG_EXT char F_Name[12];
DTG_EXT FIL fp;
DTG_EXT u16 Pedal_ADC[3], perPedal_ADC[2];
DTG_EXT stSIGNAL Sig;
DTG_EXT u8 F_Parameter;
DTG_EXT void System_PowerOff(void) ;
DTG_EXT void Parameter_Reset(void);
DTG_EXT void Signal_Process(void);
DTG_EXT void DrvDataUpdate(void);
DTG_EXT void DailyData_Change(void);
DTG_EXT void Day_Change(void);
DTG_EXT void Parameter_Init(void);
DTG_EXT void Parameter_Save(void);
DTG_EXT void Power_Off(void);
DTG_EXT void Power_Mode(u8 mode);
DTG_EXT void UpdateTimeToGPSTime(void);
#endif
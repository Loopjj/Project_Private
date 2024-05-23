/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : extern.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/05/2017
*
* Description        : extern 
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __EXTERN_H
#define __EXTERN_H

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

/* Public typedef ------------------------------------------------------------*/
// BUILD 0 => LOOP
// BUILD 1 => LOOP + VDIS(BLE)
// BUILD 2 => LOOP + VDIS(DTG)
// BUILD 3 => LOOP + ISG
// BUILD 4 => LOOP + ISG + VDIS(DTG)
// BUILD 5 => KT GVN + DMS
// BUILD 6 => KT GVN + DMS + VDIS(DTG)
// BUILD 7 => S1
// BUILD 8 => LG
// BUILD 9 => VDIS(BLE)..etc

// Field, MANO, KT Version
//typedef enum {
//  MAJOR    = 1,
//  MINOR    = 9,
//  RELEASE  = 1,
//  BUILD    = 4,
//  FACTORY1 = 0,
//  FACTORY2 = 0
//}en_Ver;

//S1 Version
typedef enum {
  MAJOR    = 3,
  MINOR    = 3,
  RELEASE  = 2,
  BUILD    = 3,
  FACTORY1 = 0,
  FACTORY2 = 0
}en_Ver;

/* Public define -------------------------------------------------------------*/
//#define __DEBUG_USB_MSG

#define MDT_S1                  0 
#define MDT_LG                  3
#define WIFI_LOOP               9
#define EXT_MODEM               10

#define CDMA_TYPE               MDT_S1
#if (CDMA_TYPE == MDT_S1)
  //#define __MANO_COMPANY_MODE__
  #define __FIELD_MODE__
#endif

#define LOOP_PROTOCOL           0
#define HANURI_PROTOCOL         1
#define KT_GVN_PROTOCOL         2
#define VDIS_PROTOCOL_ONLY      3
#define MAX_EXT_MODEM_PROTOCOL  3                    

#define EXT_MODEM_PROTOCOL_TYPE LOOP_PROTOCOL

#define NOT_USE_VDIS            0
#define VDIS_BLE_MODE           1
#define VDIS_DTG_MODE           2
#define MAX_EXT_VDIS_MODE       2                    

#define EXT_MODEM_VDIS_MODE     NOT_USE_VDIS

#define NOT_USE_ADAS            0
#define ADAS_DTG_MODE           1
#define MAX_EXT_ADAS_MODE       1                        

#define EXT_MODEM_ADAS_MODE     NOT_USE_ADAS

#define LOOP_SERVER             0
#define WELLTIZEN_SERVER        1
#define MAX_EXT_SERVER          1                    

#define EXT_MODEM_SERVER        LOOP_SERVER
 

#define LOOP_OTA_SERVER_ENABLE


#define EXT_MODEM_LOG_ENABLE
#ifdef EXT_MODEM_LOG_ENABLE
  #define EXT_MODEM_TX_LOG_ENABLE 
  #define EXT_MODEM_RX_LOG_ENABLE 
#endif

//#define BLE_LOG_ENABLE 

//#define GUI_PERIODIC_REFRESH        ///주기적으로 LCD Refresh
#define SECURE_TXT_FILE_SAVE    1     //0 :국토부파일 평문저장 1: 국토부 파일 암호화 저장
#define CONTINUOUS_SECDATA_TRIP       //국토부 파일 저장시  한트립에서 초데이터가 모두 연결되게... 그렇지 않으면 다른트립으로.... 

//#define SENT_ADDR_INIT              //통신 테스트때 DataSentAddr = DataSaveAddr 하기 위한것... 양산 때 빼야함..

#define WATCH_DOG_ENABLE

#define DTG_NORMAL 5
#define DTG_GREEN  7 
#define DTG_TYPE_GREEN  


//#define USE_CAN2_ADAS                 // CAN ADAS입력
//#define USE_UART2_ADAS                // UART ADAS입력

#define DAY_SAVE_MAX 200
#define DNLD_DAY_MAX 90               //31*3

#define DOWNLOAD_FILENAME           "LDT300BS.BIN"
#define PARAMETER_FILENAME          "LDT_PARM.PRM"

#define USE_GPS_TO_BUZZER
#define DOWNLOAD_TRIP_DATA_TO_USB

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

#define SCAN_MAXNUM       56
#define BLE_GAP_ADDR_LEN  6

#define GSENSOR_1G_VAL 256   //256 : +/-8G 512 : +/-4G  1024:  +/-2G
//#define GSENSOR_AVERAGING 

//------ DPF Factor ------
#define P1LS        350
#define P1HS        400
#define TLLS        10
#define TLHS        15	
#define PDELAYTIME  30
#define TLDELAYTIME 30	
#define PRANGEHIGH  1000	
#define PRANGELOW   0
#define PCalLow     450
#define PCalHigh    3740
#define TCal        500
#define T_Offset    10
//-----------------------

#define DATA_SAVE_INTERVAL    4

#define UREA_ACC_TEST_MENU_ENABLE

#define RTC_DELAY 10

#define TIME_HOUR

typedef enum {
  USER_NONE,
  USER_ILJIN35L,
  USER_ILJIN50L,
  USER_EnD,
  USER_EnDGENIII,
  USER_CERACOMB,
  USER_CLEANEARTH,
  USER_CLEANASDS,
  USER_MAX
}teUser_Number;

extern const char Model_Name[15];
extern const char Model_Name_KT[15];
extern const char PCB_HW_Ver[10];
extern const char Approval_No[12];
extern char Fota_FW_Ver[10];
extern const char FW_Ver_Key[];
extern char IP[4];

extern uint32_t _millisCounter;
extern int LimitDelay;

extern const unsigned short crc16tab[256];

extern void Mem_Copy(u8 *dest, u8 *source, u16 len);
extern void Mem_Copy2(u8 *dest, u8 *source, u16 len);
extern void Val_Copy(u8 *dest, u32 source, u8 len);
extern void DAC_Write(u8 mode, u8 data); //mode 0: spd, 1: rpm
unsigned short int  getcrc16(u16 crc, unsigned char *buf, unsigned int len);
unsigned short int  getcrc16b(unsigned char *buf, unsigned int len);
unsigned short getcrc16c(unsigned short crc, unsigned char data);
u8  IsSameDay(u32 time1, u32 time2);
u32  CalDayCnt(u32 time1, u32 time2);
long SwapEndian32 (u32 n);
u16 SwapEndian16 (u16 n);

int StringLen(char* str, int len);
u8 IsSameString(char* str1, char* str2, u8 len);
u8 IsSameString2(char* str1, char* str2, u8 mlen, u8 len, u8* start);
uint32_t millis(void);
void init_millis(void);
long AsciiToVal(char* str);
char IpToInt (const char * ip);
void CarRegNoToVrn(void);
u8 VrnToCarRegNo(u8 *rx_reg_no, u8 len);


/* Public functions ----------------------------------------------------------*/

#endif
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

typedef enum {
  MAJOR    = 1,
  MINOR    = 9,
  RELEASE  = 1,
  BUILD    = 0,
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
 
//#define ISG_ENABLE                    //ISG_ENABLE과 CHECK_SLEEPING_CHILD_ENABLE 둘중 하나만 해야됨.. 둘다는 안됨..

#ifndef ISG_ENABLE
//  #define CHECK_SLEEPING_CHILD_ENABLE
#endif

#define TAKE_SEND_EVENT 

#define LOOP_OTA_SERVER_ENABLE

#if (EXT_MODEM_PROTOCOL_TYPE == KT_GVN_PROTOCOL)
  #define __USE_DMS_SERVER__
  #define __USE_DMS_TEST_SERVER__
  //#define __USE_GVN_TEST_SERVER__
  //#define __USE_LTE_ONLY_MODE__ 
#endif

#if (EXT_MODEM_VDIS_MODE != NOT_USE_VDIS)
  #define __USE_VDIS_TEST_SERVER__ 
#endif

#if (EXT_MODEM_ADAS_MODE != NOT_USE_ADAS)
  //#define __USE_ADAS_TEST_SERVER__ 
#endif

#define EXT_MODEM_LOG_ENABLE
#ifdef EXT_MODEM_LOG_ENABLE
  #define EXT_MODEM_TX_LOG_ENABLE 
  #define EXT_MODEM_RX_LOG_ENABLE 
#endif

#define BLE_LOG_ENABLE 

//#define GUI_PERIODIC_REFRESH        ///주기적으로 LCD Refresh
//#define EVENT_DATA_SAVE_ENABLE
#define GUI_CAN_NOT_CHANGE_KFACTOR    //K-Factor 변경 못하게... 인증때만 필요함..
#define SECURE_TXT_FILE_SAVE    1     //0 :국토부파일 평문저장 1: 국토부 파일 암호화 저장
#define CONTINUOUS_SECDATA_TRIP       //국토부 파일 저장시  한트립에서 초데이터가 모두 연결되게... 그렇지 않으면 다른트립으로.... 

//#define SENT_ADDR_INIT              //통신 테스트때 DataSentAddr = DataSaveAddr 하기 위한것... 양산 때 빼야함..

#define WATCH_DOG_ENABLE

#define DTG_NORMAL 5
#define DTG_GREEN  7 
#define DTG_TYPE_GREEN  


#define USE_CAN2_ADAS                 // CAN ADAS입력
//#define USE_UART2_ADAS                // UART ADAS입력

#define DAY_SAVE_MAX 200
#define DNLD_DAY_MAX 90               //31*3

#define DOWNLOAD_FILENAME           "LDT300BS.BIN"
#define PARAMETER_FILENAME          "LDT_PARM.PRM"

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

#define SCAN_MAXNUM       56
#define BLE_GAP_ADDR_LEN  6

#define GSENSOR_1G_VAL 256   //256 : +/-8G 512 : +/-4G  1024:  +/-2G
//#define GSENSOR_AVERAGING 

extern const char Model_Name[15];
extern const char Model_Name_KT[15];
extern const char PCB_HW_Ver[10];
extern const char Approval_No[12];
extern char Fota_FW_Ver[10];
extern const char FW_Ver_Key[];
extern char IP[4];

extern uint32_t _millisCounter;

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
/* Public functions ----------------------------------------------------------*/

#endif
/**
  ******************************************************************************
  * @file    typedef.h 
  * @author  LOOP
  * @version V1.0
  * @date    04/19/2012
  * @brief   
  ******************************************************************************
  * @copy
  *
  * <h2><center>&copy; COPYRIGHT 2012 LOOP</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
#include <stdint.h>

/* Includes ------------------------------------------------------------------*/
//#include "release.h"
/* Private typedef -----------------------------------------------------------*/
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

__packed typedef struct {
  u8 Level;
  u8 Quality;
  u8 Status;
  u8 SysErr;
}stUreaAlarm;

__packed typedef struct {
  u32 Jump; //0
  u32 FW_Date; //1
  
  u32 Save_Addr; //2
  
  u32 Sent_Addr;  //3
  time_t LastOnTime;  //4
  
  stUreaAlarm UreaAlarm; //5
  stUreaAlarm UreaAccControl; //6
  u32 UreaAlarmLapTime; //7
  
  u32 Reserved[11]; //8
  u32 ChkSum; //19
}stRTC_Backup;

__packed typedef struct tstEasyMenu{
  u8  SignalCarType;
  u8  CanProtocol; //3.4
  u8  CanBaudrate;  //3.4
  u8  CanMode; 
  u8  BrakeType; 
  u8  Save;
}stEasyMenu,*pstEasyMenu;


typedef enum {  
//Name            | addr    | byte
  FIRMWARE_VER    = 0X0000, //6
  FIRMWARE_DATE   = 0X0010, //4
  SET_CONTRAST    = 0X0020, //1
  MAIN_DISPLAY    = 0X0030, //1
  BACKLIGHT_SEL   = 0X0031, //1 //1.4.0.0
  
  OFFSET_GX       = 0X0032, //2.1.3
  OFFSET_GY       = 0X0034, //2.1.3
  OFFSET_GZ       = 0X0036, //2.1.3

  SCR_ALARM_FLAG  = 0X0040, //1 //1.4.0.0
//  CALIB_F_FACTOR  = 0X0082, //2 Calib_F_Factor
//  SPEED_AUTO_TUNE = 0X0084, //1 //2.2.7
//  IDLE_RPM        = 0X0090, //2
  
  HS_PASSWORD     = 0X0100, //4
//  SET_DIRECTION   = 0X0110, //1
//  TRIP_COUNT      = 0X0140, //4

//  ISG_SETTING_PARM = 0x0150,  //32byte
  
  DRV_SETTING_PARM = 0X0200, //256

//  DRV_REGIST_CNT  = 0X0300, //1
//  SELECTED_DRIVER = 0X0310, //1
//  DRIVER_INFO     = 0X0320, //243 = 27*9
//  DRIVER_STS      = 0X0440, //16*9 = 144
//
  DNLD_HIST_CNT   = 0X0500, //1  
  DNLD_HIST       = 0X0510, //30  = 5*6 : Dnld_Hist[5][6]

  DRV_SAVE_ADDR   = 0X0600, //4
  DRV_SENT_ADDR   = 0X0608, //4
//  VDIS_SENT_ADDR  = 0X060C, //4
//  DRV_DAY_IDX     = 0X0610, //1  
  LAST_SENT_DAY   = 0X0614, //4
  
//  EVT_IDX         = 0X0620, //1 
//  LAST_DRV_DAY    = 0X0630, //3   
  
  MODEM_INFO      = 0X0640, //64
  APN_NAME_INFO   = 0X0680, //32
  DATA_SERVER_INFO= 0X06A0, //64
 
//  DRV_DATA_FOOTER = 0X0700, //128
//  WIFI_NAND_ADDR  = 0X0780, //4
  DRV_DAILY_DATA  = 0X0800, //128
  
//  VDIS_GET_SVR    = 0xB00,  //64
//  VDIS_DATA_SVR   = 0xB40,  //64
//  VDIS_VDR_POLICY = 0xB80,  //32
//  DMS_SVR_INFO    = 0xC00,  //64
//  QUAL_SVR_INFO   = 0xC40,  //64
//  GVN_URL_INFO    = 0xC80,  //128
//  GVN_SDR_HEADER  = 0xD00,  //128
//  GVN_SDR_POLICY  = 0xD80,  //64
//  GVN_CLI_MSG     = 0xDC0,  //16
  OTA_FWDN_SVR    = 0xE00,  //64
//  GVN_BK_URL_INFO = 0xE40,  //128
//  ADAS_SVR_INFO   = 0xF00,  //64
//  ADAS_HEADER     = 0xF40,  //64   
//  DRV_DAY         = 0X1000,
  FRAM_MAX_ADDR   = 0X2000  
}en_FRAM_ADDR;


typedef enum {
  DATA_HEADER = 0, 
  SEC_DATA    = 1, 
  DATA_FLUSH  = 2
}en_NAND_SAVE_TYPE;

typedef enum {
  NORMAL_BRAKE  = 0, 
  INVERSE_BRAKE = 1, 
  CAN_BRAKE     = 2
}en_BRAKE_TYPE;

#define INDEX_MAX 4096
#define COUNT_MAX 4096
#define LENGTH_MAX 4096
#define TX_BUFF_MAX LENGTH_MAX+100 //5000
#define MODEM_RX_DELAY 10
#define KEY_OFF_DELAY 180

typedef enum {
    CM_INFO = ('I'<<24)+('N'<<16)+('F'<<8)+'O',
    CM_FWIF = ('F'<<24)+('W'<<16)+('I'<<8)+'F',
    CM_FWSD = ('F'<<24)+('W'<<16)+('S'<<8)+'D',
    CM_FWAK = ('F'<<24)+('W'<<16)+('A'<<8)+'K',
    CM_DTSC = ('D'<<24)+('T'<<16)+('S'<<8)+'C',
    CM_DTSS = ('D'<<24)+('T'<<16)+('S'<<8)+'S',
    CM_FWER = ('F'<<24)+('W'<<16)+('E'<<8)+'R',
    CM_PRMW = ('P'<<24)+('R'<<16)+('M'<<8)+'W',
    CM_NOTI = ('N'<<24)+('O'<<16)+('T'<<8)+'I',
    CM_EVSS = ('E'<<24)+('V'<<16)+('S'<<8)+'S',
    CM_CLSE = ('C'<<24)+('L'<<16)+('S'<<8)+'E',
    CM_DYSS = ('D'<<24)+('Y'<<16)+('S'<<8)+'S',
    CM_MDIR = ('M'<<24)+('D'<<16)+('I'<<8)+'R',
    CM_MDIW = ('M'<<24)+('D'<<16)+('I'<<8)+'W',
    CM_ISIR = ('I'<<24)+('S'<<16)+('I'<<8)+'R',
    CM_ISIW = ('I'<<24)+('S'<<16)+('I'<<8)+'W'
}teCommand;

typedef enum {
  DV_SERVER,          //Server ack
  DV_LOOPBACK,     //Test Only
  DV_SCR2,              //SCR AEA
  DV_DTG,                //DTG
  DV_SCR,                //Not Use, Old Data structure
  DV_GPSTR,            //GPS Tracker for Philippines
  DV_MOBILE,           // For Mobile Phone
  DV_MDT_EL,           //MDT Elex
  DV_DPF,                 // DPF
  DV_COLOMBIA,       // Not Use For Gasan Testing
  DV_TRK_BC,           //GPS Tracker with Beacon
  DV_TRK_RF,            //GPS Tracker with RFID (For Malaysia)
  DV_ISG,                  //Idling Stop
  DV_MAX
}teDevice;

typedef enum {
  ST_SPK_ST,
  ST_SPK_CF,
  ST_DEVICE_ID,
  ST_IDENTITY_NO,
  ST_COMMAND,
  ST_INDEX,
  ST_COUNT,
  ST_LENGTH,
  ST_DATA,
  ST_CRC16,
  ST_EPK_ST,
  ST_EPK_CF,
  ST_PROCESSING
}tePckStep;

__packed typedef struct {
  unsigned short SPK;
  unsigned short DeviceID;
  unsigned int   IdentityNo;
  unsigned int   Command;
  unsigned short Index;
  unsigned short Count;
  unsigned int   Length;
}tsPckHeader;

__packed typedef struct {
  unsigned short CRC16;
  unsigned short EPK;
}tsPckFooter;

__packed typedef struct {
  unsigned int   Step;
  unsigned int   Cnt;
  unsigned short CalcCRC16;
  unsigned int   Delay;
  tsPckHeader    Header;
  unsigned char  Data[LENGTH_MAX];   //if c++ : TMemoryStream
  tsPckFooter    Footer;
}tsRpck;


__packed typedef struct {
    unsigned int   Size;
    unsigned short DivisionSize;
    unsigned short CRC16;
}tsFWIF;

__packed typedef struct {
  INT16U Warn            :1;
  INT16U Idle            :1;
  INT16U Gps             :1;
  INT16U ModemReset      :1;
  INT16U FRAM_Write      :1;
  INT16U FRAM_Read       :1;
  INT16U SentAddr        :1;
  INT16U SystemReset     :1;
  INT16U TimeSync        :1;
  INT16U Temp            :1;//여기서 부터 사용안함
  INT16U Injection       :1;
  INT16U Dist            :1;
  INT16U Sensor          :1;
  INT16U Press	         :1;
  INT16U Comm            :1;
  INT16U Urea            :1;
}stSysCode;

__packed typedef union {
  INT8U B;
  struct {
    INT8U T1            :1;   // 온도1 동작 에러
    INT8U Supply        :1;   // 에어압력 불량
    INT8U Noxin         :1;   // Nox UP 센서 동작 에러
    INT8U Noxout        :1;   // Nox DN 센서 동작 에러
    INT8U ULevel        :1;   // 요소수 레벨
    INT8U MAF           :1;   // MAF 센서 동작 에러
    INT8U UTemp         :1;   // 요소수 온도
    INT8U UQuality      :1;   // 요소수 품질 에러
  };
}stERROR;

__packed typedef union{
  INT16U W;
  struct {
    INT16U Noxin      :1;
    INT16U Noxout     :1;
    INT16U BatAlarm   :1;
    INT16U MAF        :1;
    INT16U MAFTemp    :1;
    INT16U Press      :1;
    INT16U T1         :1;
    INT16U T2         :1;
    INT16U T3         :1;
    INT16U P1         :1;
    INT16U P2         :1;
    INT16U Pump       :1;
    INT16U UTemp      :1;
    INT16U IGNFail    :1;
    INT16U res        :2;
  };
}stCHECK;

__packed typedef union{
  INT16U W;
  struct {
    INT16U Noxin        :1;
    INT16U Noxout       :1;    
    INT16U KeyOn        :1;    
    INT16U Operation    :1;
    INT16U Purge        :1;
    INT16U Flushing     :1;
    INT16U Motor        :1;
    INT16U Dosing       :1;
    INT16U Supply       :1;
    INT16U Noxact       :1;
    INT16U Heater       :1;
    INT16U res          :5;
  };
}stSIGNAL;

__packed typedef struct {
    time_t        TM;             //4
  unsigned int    LT;             //8
  unsigned int    LN;             //12
  INT16U          basePress;      //14
  unsigned short  CO;             //16
  unsigned short  FW_Ver;         //18
  unsigned char   Speed;          //19
  stERROR         Err ;           //20
  stSysCode       SysCode;        //22  
  INT8U           User;           //23
  INT16U          DpfTemp;        //25
  stCHECK         Chk;            //27 
  INT16S          NOxIn;          //29
  INT16S          NOxOut;         //31
  INT16S          TempIn;         //33
  INT16S          TempOut;        //35
  INT32U          DosingRate;     //39
  INT16U          UreaLevel;      //41
  INT16U          MAF;            //43
  INT8U           State;          //44
  INT8U           Concentration;  //45
  INT8U           SystemError[8]; //53
  INT8U           DoserError[8];  //61
  INT16U          Vbat;           //63
  INT8U           UreaAccFlag;    //64  출력제한 단계 
  
}tsSecData;  // 64 byte



__packed typedef struct {
  tsSecData Data[64];
}tsBkSramPtr,*ptsBkSramPtr;


__packed typedef struct {
  time_t    Time;                       

  u32        NandStartAddress;   
  u32        NoOfSecData;          
  u16        Index;                  
  u16        TripCount;       

  float  Trip;				                 
  float  DTrip;				                
  double  TTrip;				            
  float	 Fuel;				                 
  float	 DFuel;				                 
  double	 TFuel;				             

  u32 LapTime;                        
  u32 DLapTime;                      
  u32 Drv_Time;                       

  u8  Evt_Count;                      
  
  u32 Lat;
  u32 Lon;
  u16 Azi;
  u8   reserved0[10]; 
//- Idle-Stop Data -//
  u32 Idle_Time;                      
  u32 IdleStop_Time;                
  u16 IdleStop_Cnt;                  
  
//- EMS Data -//  
  u8   reserved1[34];                 //128
  u8   DailyDataType;         //128
  u16 CRC16;
}stDailyData;

__packed typedef struct {
  INT16U B;
  struct {
    INT8U PLAlarmFlag     :1;
    INT8U PHAlarmFlag     :1;
    INT8U Reserved        :6;
  };
}stPAlarm;

__packed typedef struct {
  u32 Identity;             //0~3 DDDDDDDD | EEEEEEEE  
  u8  Time[7];              //4~10 ss nn hh dd mm yy msec   
//- ch 1 rw
  u8  Drv_Name[10];         //11~21 driver name :            운전자 이름     
  u8  Drv_Code[18];         // driver code :            운전자 코드     
  u8  Car_Reg_No[12];       // car regist no :          자동차 등록번호 
  u8  Car_No[17];           // car no :                 차대번호        
  u8  Company_Name[12];     // company name :           회사 이름      
  u8  Company_Reg_No[10];   // company regist no :      사업자등록번호
  u8  Form_Approval_No[10]; // form approval no :       형식승인번호
  u8  Serial_No[14];        // serial no :              제품일련번호
  u8  Model_No[20];         // model no :  
  u16 K_Factor;             // speed factor                        
  u16 RPM_Factor;   
//- ch2 r
  u8  FW_Type;     
  u8  FW_Ver[6];
  u32 FW_Date;
//- ch3 rw
  INT16U  t_DelayP;
  INT16U  t_Buzzer;
  INT16U  P1L_Set;
  INT16U  P1H_Set;
  INT16U  Prange;
  INT16U  Prangehigh;
  INT16U  Prangelow;
  INT16U  P1CalHigh;
  INT16U  P1CalLow;
  INT16U  T1Offset;
  INT16U  TLL_Set;
  INT16U  TLH_Set;
  INT16U  t_DelayTL;
  INT16U  LED;
  INT16U  IdlePress;
  
  INT8U   UserNumber;
  INT8U   CanBPS;
  stPAlarm PAlarm;
  
  INT8U  UreaLvlHi;
  INT8U  UreaLvlLo;
  INT8U  UreaLvlAL;
  INT8U  UreaConHi;
  INT8U  UreaConLo;
  INT8U  UreaAlHr1;
  INT8U  UreaAlHr2;
  INT8U  UreaBuzInt;
  INT8U  UreaAlEn;
  //INT8U  reserved1[33];
//- ch4 r
//- ch5 rw 
//- ch7 r 
//- ch8 rw 
//- ch9 r 
  u8  FileOutput;
  u8  PinCode[6];
  u8  ExtModemMode;         // 모뎀 Protocol  LOOP = 0, Hanuri = 1, KT_GVN = 2
  u8  ExtModemTelecom;      // 모뎀 통신사  KT = 0 , SKT = 1, LGU+ = 2
  u8  ExtModemModel;      // 모뎀 모델 Teladin 3G = 0, Teladin LTE = 1, AM  LTE= 2
  //u8  reserved2[6];   
//- ch10 rw  
  u32 Lat;
  u32 Lon;
  u16 Azi;
  u32 UpdateCount;
  u8  FactoryNumber;
  u8  NandType;
  u16 CRC16;
}st_Setting;                //256byte

__packed typedef struct {  
  u32  SrvIP;
  u16  SrvPort;
  u32   CNUM;
  char  MSISDN[16];
  u16  DataServerInterval;
  u16  VehStateInterval;
  u16  VehStateInterval_Idle;
  u32  LpIP;
  u16  LpPort;
  u16  FotaPollingIntrval;
  u16  QualPollingIntrval;
  u8   FactoryInit;
  u8   ServerChange;
  u8   ServerConnected;
  u16  DataInterval;
  u8   AdasInterval;
  u8   res2[14];
  u16 CRC16;
}tsModemInfo; //64

__packed typedef struct {  
  char SvrName[60];
  u32  SvrPort;
}tsSvrInfo; //32

__packed typedef struct {  
  char URLName[60];
  char SubDir[20];
  char CompanyID[20];
  char DataDir[20];
  unsigned short Port;
  unsigned char BackupFlag;
  unsigned char reserved[5];
}tsURLInfo; //64

__packed typedef struct {  
  char Tel[16];
  char Msg[80];
}tsNOTI_Msg; 

#define URC_BUFF_MAX 100
typedef struct {
  int    Step;
  char   Buff[URC_BUFF_MAX];  
  int    Cnt;
  int    Len;
  int    RespFlag;
  int    Mode;  
}tsUrc;

typedef struct {
  int Step;
  u8  RCnt;
  u8  MCnt;
  u8  DCnt;
  char RcvNum [20];
  char Date[30];
  char Msg[80];
}tsSMSData;
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#endif
/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/

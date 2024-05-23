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
  u16 Brake     :1;
  u16 res0      :1;
  u16 StartTrip :1;
  u16 KeyOn     :1;
  u16 SOS       :1;
  u16 Accident  :1;
  u16 Event     :1;  
  u16 Temp1     :1;     //온도1
  u16 Temp2     :1;     //온도2
  u16 PBrake    :1;     // ISG Parking Brake
  u16 HpAirAlarm  :1;    // ISG High Pressure Air Alarm
  u16 NGear     :1;     // ISG Neutral Gear Position
  u16 Door        :1;    // Door Open/Close
  u16 CheckChild  :1;  // Sleeping child Check Button
  u16 ChildAlarm   :1;  // Sleeping child Check Alarm Out 
  u16 res15     :1;   
}st_SIGNAL;

__packed typedef struct {
  u16 GPS_Fault     :1;
  u16 Speed_Fault     :1;
  u16 RPM_Fault     :1;
  u16 Brake_Fault     :1;
  u16 Input_Fault     :1;
  u16 Output_Fault     :1;
  u16 Save_Fault     :1;
  u16 Comm_Fault     :1;
  u16 Dist_Fault     :1;
  u16 Power_Fault     :1;
  u16 Short_Fault   :1;
  u16 Open_Fault    :1;
  u16 res          :3;  
  u16 Event :1;
}st_STATUS;

__packed typedef struct { 
  u8  Time[7]; //7  0
  u8  Speed; //1  7
  u16 RPM;      //2  8
  u32 Lat;  //4  10
  u32 Lon;  //4  14
  u16 Azi;  //2  18
  s16 G_X;      //2  20
  s16 G_Y;      //2  22
  st_SIGNAL Signal; //2  24
  st_STATUS  Status;       //2  26
  float Fuel_Consump; //4  28
}st_DATA;  //32byte
 
//10sec data struct
__packed typedef struct { 
  u8  Speed;        //1
  u16 RPM;          //2
  s16 G_X;          //2
  s16 G_Y;          //2 
  u8 Brake;
}st_EVT_DATA;  //8byte

//Driver Info
__packed typedef struct {
  u8 Name[10];
  u8 Code[10];
  u8 Num[7];
}st_DRIVER_INFO;

//Driver Status
__packed typedef struct {
  u32 LastRegTime;  
  u16 LastLapTime;
  u16 RegCount;
  u8 res[8];
}st_DRIVER_STS;

//10sec data struct
__packed typedef struct { 
  st_EVT_DATA         Data[1000];
  u16                 Data_Idx;        //0~1000
  st_EVT_DATA         Save_Data[2000];  
  u16                 Save_Data_Idx;   //0~2000 
  u16                 Nand_Data_Idx;   //0~2000
  u32                 Nand_Addr;
  u8                  Save_Flag;
  u8                  Idx;              //0~9
  u8                  Count;
  u32                 Lat[10], Lon[10];
  u16                 Azi[10];   
  u32                 Save_Lat[20], Save_Lon[20];
  u16                 Save_Azi[20];     
  st_STATUS      Status[10], Save_Status[20];//2
}st_EVT; 

//1sec footer struct
__packed typedef struct { 
  u32 Identity;             // CCCCCCCC
  u8  Time[7];              // ss nn hh dd mm yy        
  u32 ODO_Pulse_Cnt;
  u32 Trip_Pulse_Cnt; 
  u32 DTrip_Pulse_Cnt;
  u32 LapTime; 
  u32 DLapTime; 
  u32 Drv_Time;

//- EMS Data -//  
  u32 Drv_RPM_Time; 
  u32 Acc_Fast_Cnt;
  u32 Acc_Slow_Cnt;
  u32 Hi_RPM_Time;
  u32 Brake_Cnt;
  
//- Idle-Stop Data -//
  u32 Idle_Time;
  u32 IdleStop_Time;
  u32 IdleStop_Cnt;
  
  float Fuel_Consump;
  float DFuel_Consump;
  double TFuel_Consump;
  
  u8  Evt_Count;
//  u32 KeyOn_Time;
//  u32 KeyOff_Time;
  u8  res[42]; 
  u16 CRC16;
}st_DRV_FOOTER;  //128byte


__packed typedef struct { //200ea
  u8  Time[6];
  u32 Nand_Addr;
//  u32 sDrvDist;        //section drive distance       구간주행거리
  u32 Drive_Time;        //section drive time           구간주행시간
//  u32 sFuelConsum;     //section fuel consumption     구간연료소모량
//  u8  eventCnt;        //event count                  이벤트데이터 개수
  u8  res[2];           //reserved data
}st_DRV_DAY_DATA;  //16byte


__packed typedef struct { 
  u8  DateTime[6];
  u8  Speed;
  u16 RPM;
  u16 Trip;
  u32 ODO;
  u32 Lat;
  u32 Lon;
  u16 Azi;
  st_STATUS  Status;
}st_DVR_Data;

__packed typedef struct { 
  u8  STX;
  u8  DeviceID;
  u8  Len;
  st_DVR_Data Data;
  u8  Chksum;
  u8  ETX;
}st_DVR_Packet;

__packed typedef struct { 
  u8  Speed;          //1
  u16 RPM;            //2
  u8  AUX;            //1
  u16 Status;  //2
  u32 Lat;            //4
  u32 Lon;            //4
  u16 Azi;            //2
  u32 DFuel;          //4
  u32 TFuel;          //4
}st_MDT_DATA;  //24byte

__packed typedef struct { 
  u8  Speed;          //1
  u16 RPM;            //2
  u8  AUX;            //1
  st_STATUS  Status;  //2
  u32 Lat;            //4
  u32 Lon;            //4
  u16 Azi;            //2
}st_MDT_DATA_LG;  //16byte

__packed typedef struct { 
  u8  DateTime[7];
  u32 Trip;
  u32 ODO;
  u32 DTrip;
  u32 LabTime;
  u32 Fuel;
  u8  Speed;
  u16 RPM;
  u8  GPS_Status;
  u8  GPS_DateTime[6];
  u32 Lat;
  u32 Lon;
  u16 Azi;
  u8  GPS_Speed;
  s16 AccX;
  s16 AccY;
  u8  Aux;
  u8  Status;
  u32 DFuel;
  u32 TFuel;
  u8  res[2];  
}st_U_Plus_Data;



__packed typedef struct {
//  unsigned char STX;
//  unsigned char DeviceID;
//  unsigned short Length;
//  unsigned short Command;
  unsigned char CarType;
  unsigned char CarRegNo[10];
  unsigned char CarNo[17];
  unsigned char CompanyNo[10];
  unsigned short SpeedFactor;
  unsigned short RPM_Factor;
  unsigned char CarIdx;
  unsigned char SpeedAutoTune;
  unsigned short DistTune;
  unsigned short RPM_Tune;
  unsigned short FuelTune;
//  unsigned char ChkSum;
//  unsigned char ETX;
}stSMS;

__packed typedef struct {
//	unsigned char STX;
//	unsigned char DeviceID;
//	unsigned short Length;
//	unsigned short Command;
	unsigned char DrvCode[10];
	unsigned char DrvIdx;
	unsigned char DrvNum[7];
        unsigned char ODO_Init;
        unsigned char FuelInit;
        unsigned char ODO_Type;
	unsigned char res[47];
//	unsigned char ChkSum;
//	unsigned char ETX;
}stSMS_Ex;

__packed typedef struct {
  u32 Jump; //0
  u32 FW_Date; //1
  
  u32 Save_Addr; //2
  
  u32 Sent_Addr;  //3
  time_t LastOnTime;  //4
  
  float Trip; //5
  float DTrip; //6
  double TTrip; //7
  
  float Fuel; //9
  float DFuel; //10
  double TFuel; //11  

  u32 Reserved[6]; //14
  u32 ChkSum; //19
}stRTC_Backup;

__packed typedef struct tstSystem{
  u8 Contrast;
  u8 Display;
  u8 BackLight;
  u8 Password[4];
}stSystem,*pstSystem;

__packed typedef struct tstEasyMenu{
  u8  SignalCarType;
  u8  CanProtocol; //3.4
  u8  CanBaudrate;  //3.4
  u8  CanMode; 
  u8  BrakeType; 
  u8  Save;
}stEasyMenu,*pstEasyMenu;

__packed typedef struct {
  u32   Lat; 
  u32   Lon;
  u16   Radius;
}stGeoFence;

__packed typedef struct {
  u8   IsgMode; 
  u16  IdleTimeLimit; 
  u16  BatVoltLimit;
  u8    CoolTempLimit; 
  u8    InclineLimit; 
  u8    PBrakeMode;  
  u8    AirPressMode;  
  u8    NGearMode; 
  u8    GpsMode; 
  u8    GeoFenceCnt;
  u8    res0[8];
  stGeoFence GeoFence[10];
  u16   IdleFuelRate;
  u8  FirstOnCheckTime;  //0~24시간
  u8  IsgSkipTime;          //0~60분
  u8    res[4]; 
//  u8    res[6]; 
}stIsgSetting;


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

  CALIB_F_FACTOR  = 0X0082, //2 Calib_F_Factor
  SPEED_AUTO_TUNE = 0X0084, //1 //2.2.7
  IDLE_RPM        = 0X0090, //2
  
  HS_PASSWORD     = 0X0100, //4
  SET_DIRECTION   = 0X0110, //1
  TRIP_COUNT      = 0X0140, //4

  ISG_SETTING_PARM = 0x0150,  //32byte
  
  DRV_SETTING_PARM = 0X0200, //256

  DRV_REGIST_CNT  = 0X0300, //1
  SELECTED_DRIVER = 0X0310, //1
  DRIVER_INFO     = 0X0320, //243 = 27*9
  DRIVER_STS      = 0X0440, //16*9 = 144

  DNLD_HIST_CNT   = 0X0500, //1  
  DNLD_HIST       = 0X0510, //30  = 5*6 : Dnld_Hist[5][6]

  DRV_SAVE_ADDR   = 0X0600, //4
  DRV_SENT_ADDR   = 0X0608, //4
  VDIS_SENT_ADDR  = 0X060C, //4
  DRV_DAY_IDX     = 0X0610, //1  
  LAST_SENT_DAY   = 0X0614, //4
  
  EVT_IDX         = 0X0620, //1 
  LAST_DRV_DAY    = 0X0630, //3   
  
  MODEM_INFO      = 0X0640, //64
  APN_NAME_INFO   = 0X0680, //32
  DATA_SERVER_INFO= 0X06A0, //64
 
//  DRV_DATA_FOOTER = 0X0700, //128
  WIFI_NAND_ADDR  = 0X0780, //4
  DRV_DAILY_DATA  = 0X0800, //128
  
  VDIS_GET_SVR    = 0xB00,  //64
  VDIS_DATA_SVR   = 0xB40,  //64
  VDIS_VDR_POLICY = 0xB80,  //32
  DMS_SVR_INFO    = 0xC00,  //64
  QUAL_SVR_INFO   = 0xC40,  //64
  GVN_URL_INFO    = 0xC80,  //128
  GVN_SDR_HEADER  = 0xD00,  //128
  GVN_SDR_POLICY  = 0xD80,  //64
  GVN_CLI_MSG     = 0xDC0,  //16
  OTA_FWDN_SVR    = 0xE00,  //64
  GVN_BK_URL_INFO = 0xE40,  //128
  ADAS_SVR_INFO   = 0xF00,  //64
  ADAS_HEADER     = 0xF40,  //64   
  DRV_DAY         = 0X1000,
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

typedef enum {
  SIG_LINE,
  SIG_CAN,  
  SIG_HK_PERSONAL,
  SIG_HK_TRUCK_2013,
  SIG_SUNLONG_BUS,
  SIG_DAEWOO_PRIMA,
  SIG_DAEWOO_NOBUS,
  SIG_BENZ_TRUCK,
  SIG_MAN_TRUCK,
  SIG_HK_TRUCK_2014
}en_SIG_TYPE;

typedef enum {
  SPEED_AUTO_TUNE_ENABLE,
  SPEED_AUTO_TUNE_DISABLE  
}en_SPEED_AUTO_TUNE_TYPE;



#define WIFI_TX_SIZE 1024
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

typedef enum {
  ST_STX,
  ST_DUMMY,
  ST_DIRECTION,
  ST_SPEED,
  ST_LDW,
  ST_FCW,
  ST_RESERVED,
  ST_CRC,
  ST_CR,
  ST_LF,
}teAdasPckStep;

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
  time_t Time;                           //4

  unsigned char  Speed[2];
  unsigned short RPM;            //8

  st_SIGNAL Signal;
  st_STATUS Status;                  //12

  unsigned int   Lat;                //16
  unsigned int   Lon;               //20

  unsigned short Azi;
  unsigned char  GPS_Status;
  unsigned char  GPS_Speed;  //24

  short AccX;
  short AccY;                          //28

  unsigned char  TripCount;	  //29
  unsigned char  DriverIndex;	//30
  unsigned char  RSSI;
  unsigned char  IsgStatus;			  //32 //Idle Stop일때만 씀...

  float		Trip;				  //36
  float		DTrip;			  //40
  float   TTrip;			  //44
  float		Fuel;         //48
  float		DFuel;        //52
  float	  TFuel;		    //56
  unsigned short  BatVolt;   //58
  unsigned char AEBS;
  unsigned char LDWS;
  short Temp1;                       // Idle Stop일때 냉각수 온도.CoolantTemp
  short Temp2;                      // Idle Stop일때 경사각 . InclineAngle
}tsSecData;

__packed typedef struct {
  tsSecData Data[64];
}tsBkSramPtr,*ptsBkSramPtr;

__packed typedef struct {
  unsigned int IndexAddr;
  unsigned int DataAddr;
  unsigned int TripCount;
  unsigned int SentAddr;
  char AP_NM[16];
  char AP_PW[16];
  char TargetIP[16];
  char TargetPort[6];
  unsigned short CRC16;
}tsSaveCtrl;

__packed typedef struct {
  tsSecData SaveData[10];
  u8 SaveCnt;
}tsNand;


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
  double   TTrip;
  double   TTripOld;
  double   FuelRate;
  double   FuelDist;
}stVal;

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
  u8  Set_Direction; 
  u8  Can_Enable; 
  u8  Can_Type;                                             
  u8  Car_Idx;          
  u8  Car_SetFactor; 
  u8  Brake_Type;
  u8  DAC_Speed;
  u8  DAC_RPM;  
  u8  DTG_Type;
  u32 CNUM;
  u8  Car_Type;
  u8  reserved1[5];         //1.4.0.0  
  u8  CanProtocol;          //3.4
  u16 CanBaudrate;          //3.4
  u8  CanMode;              //3.4
  u8  SpeedType;            //3.4
  u8  RPM_Type;             //3.4
  u8  ODO_Type;             //3.4
  u8  FuelType;             //3.4  
  u8  UseDVR;               //1.4.0.0       
  u8  reserved11[10];       //1.4.0.0       
//- ch4 r
  u8  Day_Change;               
  u32 ODO_Pulse_Cnt;
  u32 DTrip_Pulse_Cnt;
  u32 Drv_Time;
//- ch5 rw 
  s32 ODO_Offeset;                                            
//- ch7 r 
  float Fuel_Consump;
  float DFuel_Consump;
  double TFuel_Consump;
//- ch8 rw 
  u16 Fuel_Factor; 
  u8  SpeedAutoTune;
  u8  K_Factor_Percent;
  u8  RPM_Factor_Percent;
  u8  Fuel_Factor_Percent;
  u16 Dist_Percent;
//- ch9 r 
  u8  TripCount;
  u8  ExtModemMode;         // 모뎀 Protocol  LOOP = 0, Hanuri = 1, KT_GVN = 2
  u8  ExtModemTelecom;      // 모뎀 통신사  KT = 0 , SKT = 1, LGU+ = 2
  u8  ExtModemModel;      // 모뎀 모델 Teladin 3G = 0, Teladin LTE = 1, AM  LTE= 2
  u8  FileOutput;
  u8  ExtModemVDIS;
  u8  ExtModemADAS;
  u8  reserved2[3];   
//- ch10 rw  
  u32 Lat;
  u32 Lon;
  u16 Azi;
  u32 UpdateCount;
  u8  FactoryNumber;
  u8  Ver;
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
  u16 PrtocolId;
  u8 MsgId;
  u8 SvcId;
  u8 MsgLen[3];
}tsSvrMsgHeader; //7
  
__packed typedef struct {  
  u32 Tid;
  u8 TripCount;
  u8 Option[3];
  u8 Model[20];
  u8 VIN[17];
  u8 VehicleType;
  u8 VRN[12];
  u8 BRN[10];
  u8 DCode[18];
  u16 DataCount;
}tsSvrDTGHeader; //88

__packed typedef struct {  
  time_t Time;  //4
  u8 MSec;      //5
  u32 DTrip;    //9
  u32 TTrip;    //13
  u32 Trip;     //17
  u8 Speed;     //18
  u16 RPM;      //20
  u8 Brake;     //21
  u32 Lon;      //25
  u32 Lat;      //29
  u16 Azi;      //31
  u16 AccX;     //33 
  u16 AccY;     //35
  u8 Status;    //36
  u32 DFuel;    //40
  u32 TFuel;    //44
  u32 Fuel;     //48
}tsSvrDTGData;  //48
  
__packed typedef struct {  
  u8 VRN[12];
  u32 Tid;
  u8 Option[3];
  time_t Time;  //4
  u8 MSec;   //5
  u16 EventCode;  //7
}tsSvrVehState; //48

__packed typedef struct {
  char Serial_No[20];
  char Model_Name[20];
  char FW_Version[10];
  char Biz_No[13];
  char VIN[17];
  char Veh_No[12];
  char Driver_ID[20];
  unsigned char reserved[16];
}tsSDR_Header;  //128

__packed typedef struct {  
  char URLName[60];
  char SubDir[20];
  char CompanyID[20];
  char DataDir[20];
  unsigned short Port;
  unsigned char BackupFlag;
  unsigned char reserved[5];
}tsURLInfo; //64

#define MAX_FACTOR_ID_CNT  53
__packed typedef struct {  
  unsigned short PolicyNum;
  unsigned short TxInterval;
  unsigned char DataInterval;
  unsigned char RetryDelay;
  unsigned char RetryCnt;
  unsigned char DiscardCnt;
  unsigned char TermCnt;
  unsigned char reserve[2];
  unsigned char FactorIDEnable[MAX_FACTOR_ID_CNT];
}tsSDR_Policy; //64

__packed typedef struct {  
  unsigned short CLI_No;
  unsigned char CLI_Cmd;
  unsigned char CLI_State;
  unsigned char res[12];
}tsCLI_Msg; //16

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
  
  
__packed typedef struct {  
  u8 Step;
  u16 Cnt;
  u16 SendLen;
  u16 SendCnt;
  u16 SendIdx;
  u16 WaitCnt;//, EchoRxCnt;
}tsWifiTx;  
//__packed typedef struct {
//}st_Flag;        //256byte

__packed typedef struct {
  u32 Time;
  u8   Type;
  u8   MAC_Addr[6];
  s8   RSSI;
  s8   Power;
  u8   IO_Val;      //14
  u32  Lat;      
  u32  Lon;      
  u16  Azi;           //24
  u8   Speed;
  u8   res[7];
}stLpEventData;

__packed typedef struct {  
  u16 DataServerInterval;
  u8 DataInterval;
  u8 RetryDelay;
  u8 RetryCnt;
  u8 DiscardCnt;
  u8 TermCnt;
  u8 reserve[25];
}tsVDRPolicy; //32
  
__packed typedef struct {  
  u16 FCWOn;
  u16 RightLDWOn;
  u16 LeftLDWOn;
  u16 PedesFDWOn;
  u16 HWL;
  u16 U_FCW;
  u16 SLI;
  u16 FailSafeOn;
  u16 FailSafeOff;
}tsADASEventCount; 

__packed typedef struct {  
  u16 AdasSerialNo;
  u16 AdasTripNo;
}tsADASPckNo; 

__packed typedef struct { 
  u8  EventCode;  
  u8  TripAvgSpeed;
  u8  TripMaxSpeed;
  u32 Speed40TIme;
  u32 Speed4060TIme;
  u32 Speed6080TIme;
  u32 Speed80100TIme;
  u32 Speed100TIme;
  u32 OverSpeedTIme;
  u16 OverSpeedCnt;
  u16 UnderSpeedCnt;
  u32 AdasGPSLon;
  u32 AdasGPSLat;
}tsADASPckData; 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#endif
/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/

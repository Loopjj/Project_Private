/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : extern.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
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

#ifndef EXTERN_H
#define EXTERN_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
typedef enum {
  MAJOR    = 1,
  MINOR    = 0,
  RELEASE  = 0,
  BUILD    = 0,
}en_Ver;

#define MODEM

#define TIMESHIFT_SEC 9*3600   //한국시간 계산을 위한 9시간 SHIFT 초단위

#define _P1         0
#define _T1         1

#define MAX_FILE    1000
#define DEL_FILE    50
#define DELETE_FACTOR  100

#define MAX_SENSOR  2

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

#define SENDTIME    4

typedef enum {
  USER_NONE,
  USER_ILJIN35L,
  USER_ILJIN50L,
  USER_EnD,
  USER_EnDGENIII,
  USER_CERACOMB,
  USER_CLEANEARTH,
  USER_CLEANASDS,
  USER_LOOP,
  USER_MAX
}teUser_Number;

typedef struct
{
  uint32_t id;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint8_t ide;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  uint8_t rtr;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t dlc;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  uint8_t fmi;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} canrxmsg_t;

__packed typedef struct{
	INT8U	  kflag;
	INT8U	  kiflag;
	INT16U	kcount;
	INT16U	oldkey;
	INT16U	newkey;
	INT16U  keycount;
}KEYCHK_t;

__packed typedef struct{
  INT8U mode;       // mode 0 : Menu
  INT8U hidden;
  INT8U user;
  INT8U hmode;
  INT8U cmode;
	INT8U	setcnt;
	INT8U longflag;
  INT8U twinkflag;
}KEYFLAG_t;

__packed typedef struct {
  INT16U IntFlag         :1;
  INT16U Power           :1;
  INT16U Blink           :1; 
  INT16U OneEx           :1;
  INT16U PasswdFlag      :1;
  INT16U MMCSaveFlag     :1;
  INT16U DownloadFlag	   :1;
  INT16U CheckFlag       :1; 
  INT16U SaveOkFlag      :1; 
  INT16U StartFlag       :1;
  INT16U EndFlag         :1;  
  INT16U KeyOnFlag       :1;
  INT16U SaveDataFlag    :1;
  INT16U GUIDataFlag     :1;
  INT16U GUIADCFlag      :1;
  INT16U SecIntFlag      :1;  
}Flags_t;
 
__packed typedef struct {
  INT16U HPLCntFlag      :1;
  INT16U HPHCntFlag      :1;
  INT16U HTLvHCntFlag    :1;
  INT16U HTLvLCntFlag    :1;
  INT16U TLvLAlarmFlag   :1;
  INT16U TLvHAlarmFlag   :1;
  INT16U BuzzerFlag      :1;
  INT16U CANDataFlag     :1;
  INT16U DispFlag        :1;  
  INT16U Reserved        :7;  
}Flags_t1;
 
__packed typedef struct {
  INT8U B;
  struct {
    INT8U PLAlarmFlag     :1;
    INT8U PHAlarmFlag     :1;
    INT8U Reserved        :6;
  };
}stPAlarm;

__packed typedef union {
  INT8U B;
  struct {
    INT8U Noxin         :1;   // Nox UP 센서 동작 에러
    INT8U Noxout        :1;   // Nox DN 센서 동작 에러
    INT8U T1            :1;   // 온도1 동작 에러
    INT8U T2            :1;   // 온도2 동작 에러
    INT8U ULevel        :1;   //
    INT8U Nozzle        :1;   // 노즉 막힘
    INT8U UTemp         :1;   // 요소수 온도
    INT8U Supply        :1;   // 에어압력 불량
  };
}stERROR;

__packed typedef union{
  INT16U W;
  struct {
    INT16U Noxin        :1;
    INT16U Noxout       :1;
    INT16U BatAlarm     :1;
    INT16U MAF          :1;
    INT16U MAFTemp      :1;
    INT16U res          :11;
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
    INT16U res          :7;
  };
}stSIGNAL;

__packed typedef union {
  INT8U B;
  struct {
    INT8U REGStart  :1;
    INT8U Mode      :1;
    INT8U Test      :1;
    INT8U Reserved  :5;
  };
}stSTATUS;

__packed typedef struct {
  INT32U  Lat;
  INT32U  Lon;
  INT16U  Azi;
  INT8U   Speed;
}stGPSData;

__packed typedef struct {
	time_t TM;            //4
  INT16U Ver;           //6
  INT16S NOxIn;         //8
  INT16S NOxOut;        //10
  INT16U MAF;           //12
  INT16S T1;            //14
  INT16S T2;            //16
  INT16S Tavg;          //18
  INT16U P1;            //20
  INT32U DosingRatehouer;//24
  INT32U TotalDosingRate;//28
  stERROR Err;          //29
  stSTATUS Sts;         //30
  stCHECK Chk;          //32
  stSIGNAL Sig;         //34
  stGPSData Gps;        //45
  INT16U Maf_temp;      //47
  INT32S curXk;         //51
  INT32S Xc;            //55
  INT32S H1k;           //59
  INT32S H2k;           //63
  INT8U ChkSum;         //64
  INT16U TankLevelP;    //66
	INT16S TankTemp;      //68
  INT16U DosingDuty;    //70
  INT16U BV;            //72 
  INT32S curXh;         //76
  INT16U StatusAlpha;   //78
	INT32S Kl;            //82
	INT32S Kp;            //86
	INT32S Vk;            //90
	INT32S Gamma;         //94
  INT32S Formula1;      //98
  INT32S Formula2;      //102
  INT32S Formula3;      //106
  INT16U Reduction;     //108
  INT8U Reserved[20];          
}stSCRData;  // 128 byte

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

__packed typedef struct {
  time_t          TM;             //4
  unsigned int    LT;             //8
  unsigned int    LN;             //12
  INT16U          basePress;      //14
  unsigned short  CO;             //16
  unsigned short  FW_Ver;         //18
  unsigned char   Speed;          //19
  unsigned char   CSQ;            //20
  stSysCode       SysCode;        //22
  INT8U           User;           //23
  INT16U          DpfTemp;        //25
  INT16U          DpfPress;       //27
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
  INT8U           GPS_hdop;       //64
}stREAD_VALUE;    // 64 byte

#define MESSAGE_SIZE        256

typedef struct {
	INT8U			sync[2];
  INT8U  		stx;
  INT8U  		length;
  INT8U  		opcode;
  INT8U			no;
}HEADER;

typedef struct {
  HEADER  	header;
  INT8U    	msg[MESSAGE_SIZE];
}MSG_FRAME;

__packed typedef struct {
  INT8S VRN[14];
}stCarInfo; //14

__packed typedef struct {
  u32  Identity;             //0~3 DDDDDDDD | EEEEEEEE  
  u8   Time[7];              //4~10 ss nn hh dd mm yy msec   
//- ch 1 rw (127byte)
  u8   Drv_Name[10];         //11~21 driver name :       운전자 이름     
  u8   Drv_Code[18];         // driver code :            운전자 코드     
  u8   Car_Reg_No[12];       // car regist no :          자동차 등록번호 
  u8   Car_No[17];           // car no :                 차대번호        
  u8   Company_Name[12];     // company name :           회사 이름      
  u8   Company_Reg_No[10];   // company regist no :      사업자등록번호
  u8   Form_Approval_No[10]; // form approval no :       형식승인번호
  u8   Serial_No[14];        // serial no :              제품일련번호
  u8   Model_No[20];         // model no :    
  u16  K_Factor;             // speed factor                        
  u16  RPM_Factor; 
//- ch2 r (11byte)
  u8   FW_Type;     
  u8   FW_Ver[6];
  u32  FW_Date;
//- ch3 rw (33byte)
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
//- ch4 r 
//- ch5 rw                                            
//- ch7 r 
//- ch8 rw 
//- ch9 r (61byte) 
  u8   ExtModemMode;        // 모뎀 Protocol  LOOP = 0, Hanuri = 1, KT_GVN = 2
  u8   ExtModemTelecom;     // 모뎀 통신사  KT = 0 , SKT = 1, LGU+ = 2
  u8   ExtModemModel;       // 모뎀 모델 Teladin 3G = 0, Teladin LTE = 1, AM  LTE= 2
  u8   ExtModemVDIS;
  u8   ExtModemADAS;
  u16  ECUFW_Size;
  u8   ECUFW_flag;
  u8   reserved1[53];   
//- ch10 rw (13byte)  
  u32  Lat;
  u32  Lon;
  u16  Azi;
  u8   Ver;
  u16  CRC16;
}st_Setting;                //256byte

__packed typedef struct{
	INT8U col[8];
}map_t;

__packed typedef struct{
	map_t row[72];
}stMap;

__packed typedef struct {	
	INT16U BaseTemp[24]   ; // 48
	INT16U BaseMaf[24]    ; // 48
	stMap  AlpaMap		    ; // 576
}stScrMap;

typedef enum {  
//Name            | addr    | byte
  FIRMWARE_VER    = 0X0000, //6
  
  DRV_SETTING_PARM= 0X0100, //256

  DRV_SAVE_ADDR   = 0X0200, //4
  DRV_SENT_ADDR   = 0X0204, //4  
  VDIS_SENT_ADDR  = 0X0208, //4
  LAST_SENT_DAY   = 0X020C, //4 
  FRAM_ADDR       = 0X0210, //8
  
  MODEM_INFO      = 0X0240, //64
  APN_NAME_INFO   = 0X0280, //32
  DATA_SERVER_INFO= 0X02A0, //64
  
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

extern unsigned short DMS_ver;

extern const char Model_Name[15];
extern const char Model_Name_KT[15];
extern const char PCB_HW_Ver[10];
extern const char Approval_No[12];
extern char Fota_FW_Ver[10];
extern char IP[4];

extern const char FW_Ver_Key[];

extern stSCRData SecData;
extern st_Setting Setting;
extern stScrMap  ScrMap;
extern INT32U DataSaveAddr, DataSentAddr, VdisSentAddr;
extern INT8U Start_Flag, SaveCnt;
extern INT8U GPS_Speed_Val;

//- data save -//
extern stREAD_VALUE ReadValue, RD_Sec_Data;
extern stREAD_VALUE ReadValue_Buff[300];

extern INT8U *Flash_Save_Buff;

extern INT16U Save_SecData_Cnt;
extern INT8U Save_SecData_Flag;      //0~9

extern INT8U RSSI;
extern Flags_t  Flags;
extern Flags_t1  Flags1;

// CAN Variables
extern xTaskHandle xHandle_can;
extern CanRxMsg RxMessage;
extern canrxmsg_t Can_RxMsg;
extern unsigned char datarx[8];
extern unsigned char RxCAN0Buffer[MAX_CAN_MON_NO][MAX_CAN_DATA_LENGTH]; // ID, data buffer
extern unsigned int RxCAN0Data[MAX_CAN_MON_NO][MAX_CAN_DATA_LENGTH];
extern unsigned char RxCAN0Flag[MAX_CAN_MON_NO];
extern unsigned char RxCAN0Header;

extern FIL fp, fp1;
extern INT8U PreDay;
extern char F_Name[12];
extern INT32U AccSize;
extern INT16U AccFiles, AccDirs, DelFiles;
extern INT8U Disk_Full;

extern INT8U nDlStatus;
extern INT32U SendPacket;
extern INT32U TotalPacket;

extern KEYCHK_t	key;
extern KEYFLAG_t keyflag;

extern INT8U DetectFlag;
extern INT8U BackUpFlag;
extern INT8U SaveCount;
extern INT8U SD_SaveCount;

extern INT8U TerrorFlag;
extern INT32U GPSTime;
extern INT16U SaveTimeCount;
extern INT8U  SaveTimeCheck;
extern Time_t RTC_Time;
extern INT32U RTC_LVal, RTC_GPS_Val;
extern INT16U HPLCnt;
extern INT16U HPHCnt;
extern INT16U HTLvLCnt;
extern INT16U HTLvHCnt;
extern INT16U TLv_Beep;
extern INT16U CANDataCnt;
extern INT16U UserSetCnt;

extern INT16U DeltaP1, Pre_dP1, SumP1, IdleP1Cnt;
extern INT16U P_BUF[12];
extern INT8U  P_Cnt;

extern INT16U PLCal, PHCal;
extern INT16S TOffset, TSum;

extern INT8U  ECUFW_flag;

extern INT8U  ECUFW_flag;
extern INT32U ECUFW_Size;
extern INT16U ECUFW_DVSize;
extern INT16U ECUFW_CRC16;

extern INT8U Par_Buf[64], bPar_Buf[64], Par_Flag, Par_Cnt;
extern INT16U Par_Time;
extern INT8U Map_Buf[376], bMap_Buf[376], Map_Flag, Map_Cnt;
extern INT16U Map_Time;
#endif
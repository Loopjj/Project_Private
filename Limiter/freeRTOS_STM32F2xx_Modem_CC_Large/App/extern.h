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
  MAJOR    = 2,   //10.84
  MINOR    = 0,
  RELEASE  = 0,
  BUILD    = 0,
}en_Ver;

#define MODEM

#define TIMESHIFT_SEC 9*3600   //한국시간 계산을 위한 9시간 SHIFT 초단위

#define _P1         0
#define _T1         1

#define MAX_FILE    400
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
  INT16U CANWaitFlag     :1;
}Flags_t;
 
__packed typedef struct {
  INT16U HPLCntFlag      :1;
  INT16U HPHCntFlag      :1;
  INT16U QualityCntFlag  :1; //for Quality
  INT16U HTLvHCntFlag    :1;
  INT16U HTLvLCntFlag    :1;
  INT16U TLvLAlarmFlag   :1;
  INT16U TLvHAlarmFlag   :1;
  INT16U QualityAlarmFlag:1; //for Quality
  INT16U BuzzerFlag      :1;
  INT16U CANDataFlag     :1;
  INT16U DispFlag        :1;  
  INT16U RegenOBD        :1;
  INT16U RegenOffOBD     :1;
  INT16U GpsDataFlag     :1;
  INT16U Reserved        :2;  
}Flags_t1;
 
__packed typedef union {
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
    INT16U res          :7;
  };
}stSIGNAL;

__packed typedef union {
  INT8U B;
  struct {
    INT8U Mode       :1;
    INT8U Dist       :1;
    INT8U SentAddr   :1;
    INT8U SystemReset:1;
    INT8U Gps        :1;
    INT8U Fram       :1;
    INT8U Comm       :1; 
    INT8U TimeSync   :1; 
  };
}stSTATUS;

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
//- ch3 rw (32byte)
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
  
  INT8U   CanBPS;
  stPAlarm PAlarm;
//- ch4 r 
//- ch5 rw                                            
//- ch7 r 
//- ch8 rw 
//- ch9 r (62byte) 
  u8   ExtModemMode;        // 모뎀 Protocol  LOOP = 0, Hanuri = 1, KT_GVN = 2
  u8   ExtModemTelecom;     // 모뎀 통신사  KT = 0 , SKT = 1, LGU+ = 2
  u8   ExtModemModel;       // 모뎀 모델 Teladin 3G = 0, Teladin LTE = 1, AM  LTE= 2
  u8   reserved1[3];
  u8   ECU_Ver[4];
  u32  ECUFW_Size;
  u16  ECUFW_CRC16;
  u8   ECUFW_flag;
  u8   reserved2[45];  
//- ch10 rw (13byte)  
  u32  Lat;
  u32  Lon;
  u16  Azi;
  u8   Ver;
  u16  CRC16;
}st_Setting;                //256byte

__packed typedef union {
  u8 B;
  struct {
    u8 T1        :1;
    u8 T2        :1;
    u8 T3        :1;
    u8 T4        :1; 
    u8 P1        :1;
    u8 Perror    :1;
    u8 IGN_L     :1;
    u8 IGN_H     :1;
  }Sensor;
}stError;


__packed typedef union{
  u8 B;
  struct {
    u8 Over2     :1;
    u8 Over3     :1;
    u8 Over4     :1;    
    u8 Extinguish:1;    
    u8 BV        :1;
    u8 CF        :1;
    u8 Misfire   :1;
    u8 Retry     :1;
  }Func;
}stCheck;

__packed typedef union{
  u8 B;
  struct {
    u8 Ready      : 1;
    u8 Igniter    : 1;
    u8 Motor      : 1;
    u8 KeyOn      : 1;
    u8 Gsensor    : 1;
    u8 RegenSts1  : 1;
    u8 RegenSts2  : 1;
    u8 Test       : 1;
  };
}stSignal;

__packed typedef union{
  u8 B;
  struct {
    u8 REGStart  : 1;
    u8 SCRStart  : 1;
    u8 Mode      : 1;
    u8 scrReady  : 1;
    u8 Reserved  : 4;
  };
}stStatus;

__packed typedef union{
  INT16U K;
  struct {
    INT16U CIRC_DIAG  : 1;
    INT16U SELF_DIAG  : 1;
    INT16U HEATR_DIAG : 1;
    INT16U POS_DIAG   : 1;
    INT16U HEATR_ON   : 1;
    INT16U TEMP_MEAS  : 1;
    INT16U HEATR_OFF  : 1;
    INT16U NOT_RDY    : 1;
    INT16U BYTT       : 1;
    INT16U CAN        : 1;
    INT16U Reserved   : 6;  
  };
}stPM1_Error;

__packed typedef union{
  INT16U K;
  struct {
    INT16U CIRC_DIAG  : 1;
    INT16U SELF_DIAG  : 1;
    INT16U HEATR_DIAG : 1;
    INT16U POS_DIAG   : 1;
    INT16U HEATR_ON   : 1;
    INT16U TEMP_MEAS  : 1;
    INT16U HEATR_OFF  : 1;
    INT16U NOT_RDY    : 1;
    INT16U BYTT       : 1;
    INT16U CAN        : 1;
   /* ONLY PM2 ERROR */  
    INT16U DPF_CRACK  : 1;
    INT16U MEM        : 1;
    INT16U SD         : 1;
    INT16U DPF_COM    : 1;
    INT16U Reserved   : 2;
  };
}stPM2_Error;

__packed typedef struct {
  INT32S curXk;         //94
  INT32S Xc;            //98
  INT32S H1k;           //102
  INT32S H2k;           //106
  INT32S curXh;         //118
	INT32S Vk;            //90
	INT32S Gamma;         //94  
	INT32S Kl;            //124
	INT32S Kp;            //128
}stScrValue;

__packed typedef struct {
  u16 T[4];         //2 8
  u16 P;            //2 10
  u16 Rate;         //2 12
  u16 IHC;          //2 14
  u16 f_REG;        //2 16
  u8 CarType;       //1 17
  u8 FPD;           //1 18
  stError Error;    //1 19
  stCheck Check;    //1 20
  u16 t_REG;        //2 22
  u16 n_REG;        //2 24
  stSignal Sig;     //1 25
  stStatus Sts;     //1 26
  u8 Lock;          //1 27
  u32 DrvTime;      //4 31
  u16 baseP1;       //2 33
  u16 f_SCR;        //2 35
  u16 t_SCR;        //2 37
  u16 n_SCR;        //2 39
  u8 RegenStartKey; //1 40
  u16 t_OffSCR;     //2 42
}stReadValue;

__packed typedef struct {
  time_t TM;           //4
  INT32U LT;           //8
  INT32U LN;           //12
  INT8U  Speed;        //13   
  INT8U  CSQ;          //14 
  INT16U CO;           //16
  INT16U FW_Ver;       //18
  
  stReadValue DpfValue;//60
  
  INT16U Ver;          //62
  INT16S NOxIn;        //64
  INT16S NOxOut;       //66
  INT16U MAF;          //68
  INT16S TempIn;       //70
  INT16S TempOut;      //72
  INT16S Tavg;         //74
  INT16S TempSpa;      //76  
  INT16U SupplyP;      //78
  INT32U DosingRatehouer;//82
  INT32U TotalDosingRate;//86
  stERROR Err;          //87
  stSTATUS Sts;         //88
  stCHECK Chk;          //90
  stSIGNAL Sig;         //92
  INT16S Maf_temp;      //94
  INT8U UreaLevel;      //95
  INT8U LimitStep;      //96
	INT16S TankTemp;      //98
  INT16U DosingDuty;    //100
  INT16U BV;            //102
  INT16U StatusAlpha;   //104
  INT32S Formula1;      //108
  INT32S Formula2;      //112
  INT32S Formula3;      //116
  INT16U Reduction;     //118
  INT16U UreaQuality;   //120
  INT16U DPF_Crack;       //PM_Senser1;    //122
  stPM1_Error PM1_Error ;    //124
  stPM2_Error PM2_Error ;    //126
  INT16U PM_Senser4;    //128
}stREAD_VALUE;    // 128 byte

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

__packed typedef struct {
	INT16U	CutMaf					  ; // PAR 1
	INT16U  LowTankLevel		  ;
  INT16S	CutTankTemp				;	
	INT16U	TankType          ;
  
	INT16U  LowT1   				  ; // PAR 2
	INT16U	HighT1  				  ;
	INT16U	LowP1				      ;
	INT16U	HighP1			      ;

	INT16U	BatVoltageLow			; // PAR 3
  INT8U	  Temp_Ratio     		;
  INT8U	  Scr_Mode       		;
  INT8U	  Scr_Period     		;
  INT8U   Reserved0     		;    
  INT16U	Reserved1				  ;

	INT16U  ScrWaitTime				; // PAR 4
  INT8U   ScrPurge          ;
  INT8U   Reserved2     		;  
  INT16U  ScrPurgeTime      ;
  INT8U   ScrMafMode        ;
 	INT8U 	Reserved3			    ;
  
	INT16U	MafOffset         ;	// PAR 5
  INT16U  MafDiameter       ;	
	INT16U  MapOffset         ;	
  INT8U	  Nozzle        		;      
  INT8U	  Reserved4     		;    
  
	INT16U  LowT2   				  ; // PAR 6  
  INT16U  MafRfactor        ;	  
  INT16U  scrLowT3          ;
  INT16U  scrLowT4          ;
}stSysControl;

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
extern const char PCB_HW_Ver[10];
extern char Fota_FW_Ver[10];
extern char IP[4];

extern const char FW_Ver_Key[];

extern stScrValue ScrValue;
extern st_Setting Setting;

extern stSysControl SysCtl;
extern stScrMap ScrMap;
extern INT8U ServerRW;

extern INT32U DataSaveAddr, DataSentAddr;
extern INT8U Start_Flag, SaveCnt;
extern INT8U GPS_Speed_Val;

//- data save -//
extern stREAD_VALUE ReadValue, RD_Sec_Data;
extern stREAD_VALUE ReadValue_Buff[300];
extern INT8U *Flash_Save_Buff;

extern INT16U Save_SecData_Cnt;
extern INT8U Save_SecData_Flag;      //0~9

extern INT8U RSSI;
extern stSTATUS Sts;
extern Flags_t  Flags;
extern Flags_t1  Flags1;
//extern stPM1_Error PM1_Error;
extern stPM2_Error PM2_Error;

// CAN Variables
extern xTaskHandle xHandle_can;
extern CanRxMsg RxMessage;
extern canrxmsg_t Can_RxMsg;
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
extern INT16U QualityCnt;
extern INT16U TLv_Beep;
extern INT16U CANDataCnt;
extern INT8U  buzerflag;

extern INT16U DeltaP1, Pre_dP1, SumP1, IdleP1Cnt;
extern INT16U P_BUF[12];
extern INT8U  P_Cnt;

extern INT16U PLCal, PHCal;
extern INT16S TOffset, TSum;

extern INT16S NoxO2[2];
extern INT8U NoxFail[2], NoxErr[2], NoxData[2][3];

extern INT8U Par_Buf[64], bPar_Buf[64], Par_Flag, Par_Cnt;
extern INT16U Par_Time;
extern INT8U Map_Buf[376], bMap_Buf[376], Map_Flag, Map_Cnt;
extern INT16U Map_Time;
#endif
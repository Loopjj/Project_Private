/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : scr.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 09/05/2014
*
* Description        : 
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef	SCR_H
#define	SCR_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef SCR_GLOBALS
#define SCR_EXT
#else
#define SCR_EXT extern
#endif



SCR_EXT int scr_debug;

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/
#define SET_BOOT_SYS				0x01

#define SET_BOOT_SYS_STR			"01"
#define SET_BOOT_PAR_STR			"02"
#define SET_BOOT_MAP_STR			"03"
#define SET_BOOT_FAC_STR			"04"
#define SET_BOOT_CAL_STR			"05"

#define EXHAUST			    75.5		//mm
#define SCR_DIAMETER	  304.8		//mm(12inch)
#define SCR_FILTER		  0.7			//70% 면적

#define OFFSET_X1000		1000.0
#define OFFSET_X24400		24400.0

#define MWNH3	          17.0
#define NH3BASE	        0.184167
#define AIR_DENSITY     1.28

#define SCR_CHECKMODE		  0
#define SCR_SENSORON		  1
#define SCR_SENSORONWAIT	2
#define SCR_SENSORCK		  3
#define SCR_CALCULATE		  4
#define SCR_SOLCONTL		  5
#define SCR_MOTORCONTL    6
#define SCR_DOSING  		  7
#define SCR_SUPPLYMODULE  8
#define SCR_P1CK          9
#define SCR_PRESSCK2		  10
#define SCR_ALARMCK			  11
#define SCR_NWAIT			    12
#define	SCR_ENVCK_WAIT		13

#define SUPPLY_MANUAL		  20
#define SUPPLY_SOLCON		  21
#define SUPPLY_SOLCK		  22
#define SUPPLY_SOLCK2		  23
#define SUPPLY_MOTORCON   24
#define SUPPLY_DOSINGCON  25
#define SUPPLY_START		  26
#define SUPPLY_PERIOD		  27
#define SUPPLY_WAIT			  28

#define ALARM_MAXCOUNT		20
#define ALARM_CHECKCOUNT	10

#define LEN_SYSVALUE      156

/*
*********************************************************************************************************
*                                              TYPE DEFINE
*********************************************************************************************************
*/
//아래 map 범위을 수정할경우 scr.c에 INT16U FactorCall(void) 함수위에 Bast Point를 수정해야 한다.
//static INT16U BaseTemp[8]={150,200,250,300,350,400,450,500};
//static INT32U BaseSv[8]  ={10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000};
__packed typedef struct{
	INT8U col[8];
}map_t;

__packed typedef struct{
	map_t row[72];
}stMap;

__packed typedef struct{
	INT16U AdcLevel;
	INT8S  Offset;
}stCalMaf;

__packed typedef struct{
	INT32U Ka                 ;
	INT32U Kd                 ;	
	INT32U Kr                 ;
  INT16U OFA                ;
  INT32U S1                 ;
  INT16U S2                 ;
	INT16U L                  ;	    //SCR Length(m)
	INT32U ScrArea            ;     //SCR Filter의 면적
	INT8U	 Xsp				        ;
	INT8U	 Coeff              ;
  INT32U Kl                 ;
  INT32U Kp                 ;
}stFactor;

__packed typedef struct{
	INT8U	ContrlSet	          ;
	INT8U	Supply_use          ;
	INT16U Gpio_Period        ;	    // 100Hz,100msec
	INT16U Duty_Cycle         ;	    // 50%
	INT8U Supply_debug        ;
	INT16U Pwm_Count          ;
}stSupply;

__packed typedef struct{
	INT8U	ContrlSet	          ;
	INT8U	Dosing_use          ;
	INT16U Gpio_Period        ;	    // 100Hz,100msec
	INT16U Duty_Cycle         ;	    // 50%
	
	INT8U Dosing_debug        ;
	INT16U Dosing_Count       ;
}stDosing;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif

__packed typedef struct {	
	INT8U	sys_firstboot			    ;
	INT8U	Valid_Str[10]			    ;
  
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

	INT16U  ScrWaitTime				;  // PAR 4
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
  
	INT8S	  SystemTemperature   ;		// + 온도 상한( temperature를 감시하여 이 값이상이면 alarm발생으로 끝)
  INT8U   ControlMode		      ;
  INT8U 	ScrTestMode		    ;		// SCR 동력계 테스트 모드로 사용....추후 결정할것...  
	INT8U	  Reserve[16]				  ;		// 추후 변수 추가 할려면 Reserve영역을 사용해라
}stSysControl;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif

__packed typedef struct {	
	INT8U	scr_firstboot		; // 1
	INT8U	Valid_Str[10]		; // 10
	INT16U BaseTemp[24]   ; // 48
	INT16U BaseMaf[24]    ; // 48
	stMap  AlpaMap		    ; // 576
}stScrMap;

__packed typedef struct {	
	INT8U	scr_firstboot		;
	INT8U	Valid_Str[10]		;
	stFactor Catalyst			;
}stScrFactor;

__packed typedef struct {
	INT8U	 enable		      ;
	INT16U RangeLow	      ;
	INT16U RangeHigh      ;
	INT16U AdcLow		      ;
	INT16U AdcHigh	      ;
}stCalib;

__packed typedef struct {
	INT8U	scr_firstboot		;
	INT8U	Valid_Str[10]	  ;
  stCalib Temp[2]       ;
  stCalib Supply        ;
	stCalMaf CalMaf[5]		;
  INT8U NOx_enable[2]   ;
}stScrCalib;

__packed typedef struct {
	INT8U	scr_firstboot		        ;
	INT8U	Valid_Str[10]	      	  ;	
	INT16U Adc[10]                ;
	INT16U Value[10]              ; 
}stScrMaf;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif
__packed typedef union {
  INT16U W;
  struct {
    INT16U Beta         :1;
    INT16U Reduction    :1;
    INT16U ScrFlowStart :1;
    INT16U SupplyStart  :1;
    INT16U DosingStart  :1;
    INT16U TimeRequest  :1;
    INT16U WriteScrMap  :1;
    INT16U SupplyT1Cut  :1;
    INT16U ModuleT1Cut  :1;
    INT16U P1Cut        :1;
    INT16U MAFCut       :1;
    INT16U Flushing     :1;    
    INT16U res          :4;
  };
}stFLAG;

__packed typedef union {
  INT8U B;
  struct {
    INT8U Noxin         :1;   // Nox UP 센서 동작 에러
    INT8U Noxout        :1;   // Nox DN 센서 동작 에러
    INT8U T1            :1;   // 온도1 동작 에러
    INT8U T2            :1;   // 온도2 동작 에러
    INT8U ULevel        :1;   // 요소수 레벨
    INT8U MAF           :1;   // MAF 센서 동작 에러
    INT8U UTemp         :1;   // 
    INT8U Supply        :1;   // 에어압력 불량
  };
}stERROR;

__packed typedef union{
  INT16U W;
  struct {
    INT16U Noxin        :1;
    INT16U Noxout       :1;
    INT16U BatAlarm     :1;
    INT16U MAFTemp      :1;
    INT16U res          :12;
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
    INT16U res          :6;
  };
}stSIGNAL;

__packed typedef struct {
  INT8U REGStart  : 1;
  INT8U Mode      : 1;
  INT8U Test      : 1;
  INT8U Reserved  : 5;
}stSTATUS;

__packed typedef struct {			    // 보고에 사용할수있는 프로토콜과 위치를 맞춘 정보.
	INT16U Version		      ;//2
	INT16S T1_temp			    ;//4
	INT16S T2_temp			    ;//6
	INT16S Tavg_temp		    ;//8
	INT16S AMB_temp			    ;//10
	INT16U P1_bar			      ;//12
	INT16S Noxppm[2]		    ;//16
	INT16U DosingDuty  	    ;//18
	INT16U MafKg_H			    ;//20
	INT16S Maf_temp 	      ;//22
	INT16S TankTemp		      ;//24
  INT16U TankLevelP		    ;//26
	INT16U BattVoltage		  ;//28
  INT8U Map_X             ;//29
  INT8U Map_Y             ;//30
	INT8U	StatusAlpha		    ;//31
	INT8U	SCRMode			      ;//32
	INT16U NoxReduction	    ;//34
	stERROR	SystemError		  ;//35
	stCHECK SystemCheck		  ;//37
	stSIGNAL SystemSignal	  ;//39
	INT8U	MDosingStart	    ;//40
  
	double DosingRatehouer	;//48
	double TotalDosingRate  ;//56
  double curXk            ;//64
	double Xc               ;//72
  double H1k              ;//80
	double H2k              ;//88
  double Yk               ;//96
	double Kl               ;//104
	double Kp               ;//112
  double curXh            ;//120
	double Vk		            ;//128
	double Gamma			      ;//136
  double Formula1         ;//144
  double Formula2         ;//152
  double Formula3         ;//160
  double NH3Up            ;//168
  double NH3Dn            ;//176 
  
	INT16U NoxRxCount[2]	  ;//4
	INT8U	NoxSensorFlag[2]  ;//6
	INT8U	NoxErr[2]		      ;//8
	INT8U	NoxLinkSt[2]	    ;//10
	INT8U	AlarmCriticalSet  ;//11
	INT32U reducCount		    ;//15
	double MDosingRatehouer ;//23
  double MDosingDuty      ;//31
	double BackupMaf		    ;//39
  double Xsp              ;//47
  double Omega            ;//55
  double preXk            ;//63
  double preXh            ;//71
  double preUk            ;//79
  double curUk            ;//87

	INT8U	AlarmNoxinCount	  ;//88
	INT8U	AlarmNoxOutCount  ;//89
	INT8U	AlarmPressCount	  ;//90
	INT8U Maker			        ;//91
	INT8U	Supplier		      ;//92
	INT8U	Type[2]		        ;//94
	INT8S	Temperature		    ;//95
    
	INT16S mNoxppm[2]		    ;//99
	INT16S mTavg    		    ;//101
	INT16U mMaf   			    ;//103
  
	// PSU 
 	INT16U BatStatus		    ;//105
	stDosing dosing			    ;//114
	INT8U	AlphaSet		      ;//115
	INT8U	SolControlOnOff	  ;//116
	INT8U	SolOffCount		    ;//117
	INT8U	PressuarTotalCount;//118
	INT8U	TimeRequestFlag	  ;//119
	INT32U TmrDosingPeriod  ;//123
	INT32U sumUreaCount	    ;//127
	//INT8U	ScrFlowStartFlag  ;
	stSupply supply			    ;//136
  
  // DPF
  stSTATUS Sts            ;//137
}stSysStatus;							        // Proto status response


/*
*********************************************************************************************************
*                                           GLOBAL variables
*********************************************************************************************************
*/
SCR_EXT stSysStatus   SysSts;		
SCR_EXT stSysControl  SysSet;	
SCR_EXT stScrMap      ScrMap;
SCR_EXT stScrFactor   ScrFactor;
SCR_EXT stScrCalib    ScrCalib;
SCR_EXT stScrMaf      ScrMaf;
SCR_EXT stFLAG        ScrFlag;
SCR_EXT INT8S         source_version[150];
SCR_EXT INT8S         map_delay;
SCR_EXT INT16S        DosingTimer;
SCR_EXT INT16S        NoxSlip;
SCR_EXT INT8U	        OldMode;
SCR_EXT INT8U	        FirstFlag;
SCR_EXT INT8U	        nStatus;
SCR_EXT INT8U	        PurgeFlag;
SCR_EXT INT8U	        PurgeSolFlag;
SCR_EXT INT16U        FlushingCount;
SCR_EXT INT8U         Inject_On, Inject_Duty;
SCR_EXT INT16U        SCR_PERIOD;
/*
*********************************************************************************************************
*                                           GLOBAL FUNCTION
*********************************************************************************************************
*/
SCR_EXT void SYS_Control_Init(void);
SCR_EXT double UreaSolMolperM3ToKgperHoure(double u_molperm3);
SCR_EXT void SolPressureCtrl(INT8U onoff);
SCR_EXT void MotorCtrl(INT8U onoff);
SCR_EXT void SupplyPower(INT8U onoff);
SCR_EXT void HeaterCtrl(INT8U onoff);

SCR_EXT void SCR_Control_Off(void);
SCR_EXT void Task_Scr(void *p_arg);
SCR_EXT void AlarmClear(void);
SCR_EXT void AlarmTask(void);
SCR_EXT void ModuleTask(void);
SCR_EXT INT8U NoxCheck(void);
SCR_EXT void AlarmCheckPressure(void);
SCR_EXT void ScrNoxDownpersent(void);
SCR_EXT int ScrCalculate(void);
SCR_EXT INT8U FactorCall(void);
SCR_EXT INT16U GramPerHourToDM(double Gram, INT16U ScrPeriod);
SCR_EXT void SCR_Reduction_Buffer(void);

/*-------- dbg function --------*/
SCR_EXT INT16U dbg_tSET_reset_test(void);
SCR_EXT INT16U dbg_fwver_view(void);
SCR_EXT INT16U dbg_zeoliteCatalyst_view(void);
SCR_EXT INT16U dbg_scrTask_Ctrol(void);
SCR_EXT INT16U dbg_scr_test(void);
SCR_EXT INT16U dbg_Factor_Save(void);
SCR_EXT INT16U dbg_ScrCalib_Save(void);
SCR_EXT INT16U dbg_Scr_ManualTest(void);
SCR_EXT INT16U dbg_Scr_DosingTest(void);
SCR_EXT INT16U dbg_Scr_AutoMode(void);
//SCR_EXT INT16U dbg_SetdFactor_Ctrol(void);


#define SCRCONTROL_COMMANDS	\
	{"setrst", 	 dbg_tSET_reset_test,           ALL_DIR, "set reset"},\
	{"fwver",	   dbg_fwver_view,                ALL_DIR, "firmware ver"},\
	{"fasave",   dbg_Factor_Save,               ALL_DIR, "factor save"},\
	{"casave",   dbg_ScrCalib_Save,             ALL_DIR, "scr calibartion save"},\
	{"manual",   dbg_Scr_ManualTest,            ALL_DIR, "scr manual test"},\
	{"dosing",   dbg_Scr_DosingTest,            ALL_DIR, "scr dosing test"},\
	{"auto",     dbg_Scr_AutoMode,              ALL_DIR, "scr auto mode"},\
	{"scr",	     dbg_scrTask_Ctrol,             ALL_DIR, "scr 1"},\
	{"sss",	     dbg_scr_test,                  ALL_DIR, "scr debug on/off"},\

#endif
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/


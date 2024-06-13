/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : scr.c
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
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define SCR_GLOBALS
#include "includes.h"
/*
*********************************************************************************************************
*                                               FUNCTIONS
*********************************************************************************************************
*/
#define REDUCTION_MAXSIZE	600
#define STEPNOX_MAXSIZE   16
#define ERRKP_MAXSIZE     256

typedef struct{
	INT16U noxin;
	INT16U noxout;
	INT16U maf;
}Reduction_t;

#pragma location = "EXTCCRAM"
static Reduction_t	 preduc[REDUCTION_MAXSIZE];

/*
********************************************************************************
* Description : Operator_version
* Arguments   : none
* Return      : 
* Note        : version
******************************************************************************** 
*/
void Operator_version(void)		
{
	
	memset(source_version,0,sizeof(source_version));
	sprintf((char*)source_version,"\n\nLoop %s V%x.%x %s,SYSTYPE:0x%08x,\nBuild : %s %s (SourceVer %s)\n\n",
		PRODUCT_NAME,VERSION_NUM/0x10,VERSION_NUM & 0x0f,HW_NAME,0x43,__DATE__, __TIME__,SRC_VERSION_NUM);
}

/*
********************************************************************************
* Description : SYS_Control_Factory_Init
* Arguments   : none
* Return      : 
* Note        : 기능 동작시 필요한 각종 구조체 데이터의 초기화.
******************************************************************************** 
*/
static void SYS_Control_Factory_Init(void)
{
	memset((char *)&SysSet,0xFF,sizeof(stSysControl));			// MEMORY 영역이 최초에는 0xFF 이기 때문에 0xFF로 초기화

	SysSet.sys_firstboot = SET_BOOT_SYS;
	
	SysSet.CutMaf								  = 279;
  SysSet.LowTankLevel           = 10;
	SysSet.CutTankTemp						= -10;  //추후 설정할것.
  SysSet.TankType               = 1;
	SysSet.LowT1  								= 200;
	SysSet.HighT1 								= 450;
	SysSet.LowP1								  = 7000; //mbar
	SysSet.HighP1								  = 10700;//mbar
	SysSet.BatVoltageLow					= 2000;	// 10V: 2000/100
  SysSet.Temp_Ratio             = 80;
  SysSet.Scr_Mode               = 1;
  SysSet.Scr_Period             = 1;    // 1Hz
  SysSet.ScrWaitTime            = 300;
  SysSet.ScrPurge               = 0;
  SysSet.ScrPurgeTime           = 60;  
  SysSet.MafOffset              = 750;
  SysSet.MafDiameter            = 82;	  //82mm 
  SysSet.MapOffset              = 1000;	
  SysSet.Nozzle        		      = 1;    // 1: Air Type, 2 : Water Type  
  
	SysSet.SystemTemperature			= 80;		// 온도 상한( NMS board 의 temperature를 감시하여 이 값이상이면 alarm발생으로 끝
}

/*
********************************************************************************
* Description : SCR_Map_Factory_Init
* Arguments   : none
* Return      : 
* Note        : SCR MaP value factroy initialize
******************************************************************************** 
*/
static void SCR_Map_Factory_Init(void)
{
  int i, k;
  
  memset((char *)&ScrMap,0xFF,sizeof(stScrMap));		// MEMORY 영역이 최초에는 0xFF 이기 때문에 0xFF로 초기화

  ScrMap.scr_firstboot = SET_BOOT_SYS;
  for(i=0; i<24; i++) {
    for(k=0; k<24; k++) {
      ScrMap.AlpaMap.row[i].col[k]		= 100;
    }
    ScrMap.BaseMaf[i] = (i * 50) + 300;
    ScrMap.BaseTemp[i] = i * 20;
  }
}

/*
********************************************************************************
* Description : SCR_Reduction_Buffer
* Arguments   : none
* Return      : 
* Note        : SCR Reduction Buffer
******************************************************************************** 
*/
void SCR_Reduction_Buffer(void) 
{
  if(ScrFlag.Reduction == B_EMPTY) {
    preduc[SysSts.reducCount].noxin  = SysSts.Noxppm[0];
    preduc[SysSts.reducCount].noxout = SysSts.Noxppm[1];
    preduc[SysSts.reducCount].maf    = (INT16U)SysSts.BackupMaf;
    SysSts.reducCount++;
    if(SysSts.reducCount >= REDUCTION_MAXSIZE) {
      ScrFlag.Reduction = B_FULL;
    }
  }
}

/*
********************************************************************************
* Description : SCR_Control_Off
* Arguments   : none
* Return      : 
* Note        : SCR Control Off
******************************************************************************** 
*/
void SCR_Control_Off(void)
{
  SysSts.SystemSignal.Dosing = OFF;
  SysSts.SystemSignal.Supply = OFF;
  SysSts.SystemSignal.Flushing = OFF;	
  SolPressureCtrl(OFF);
//  Inject_On = 0;
//  OUTPUT_DOSING(OFF);
  PWM_SetInject(0);
  MotorCtrl(OFF);
  SupplyPower(OFF);  
  EndDosing();
  EndPwm();  
  SysSts.supply.Pwm_Count = 0;
  SysSts.supply.Duty_Cycle = 0;
  FlushingCount = 0;
  PurgeSolFlag = 0;
}
/*
********************************************************************************
* Description : SCR_Calib_Factory_Init
* Arguments   : none
* Return      : 
* Note        : SCR factor value factroy initialize
******************************************************************************** 
*/
#define FE_KA			      80.00
#define FE_KD		  	    5
#define FE_KR			      150
#define FE_OFA  		    1.00
#define FE_XSP          0.10
#define FE_S1			      60.0
#define FE_S2			      0.000
#define FE_L			      0.152		//m
#define FE_A            0.05586	//m2
#define FE_COEFF        0.77
#define FE_KL           0.002000
#define FE_KP           0.00050

static void SCR_Factor_Factory_Init(void)
{
  memset((char *)&ScrFactor,0xFF,sizeof(stScrFactor));			  // MEMORY 영역이 최초에는 0xFF 이기 때문에 0xFF로 초기화

  ScrFactor.scr_firstboot                 = SET_BOOT_SYS;
  ScrFactor.Catalyst.Ka						        = FE_KA	        ;
  ScrFactor.Catalyst.Kd							      = FE_KD		  	  ;	
  ScrFactor.Catalyst.Kr							      = FE_KR		  	  ;
  ScrFactor.Catalyst.OFA    					    = FE_OFA * 100  ;
  ScrFactor.Catalyst.S1   			          = FE_S1  * 10   ;
  ScrFactor.Catalyst.S2     		          = FE_S2  * 1000 ;
  ScrFactor.Catalyst.L      		  			  = FE_L   * 1000	;
  ScrFactor.Catalyst.ScrArea        		  = FE_A   * 100000;
	ScrFactor.Catalyst.Xsp				          = FE_XSP * 100  ;
	ScrFactor.Catalyst.Coeff                = FE_COEFF * 100;  
  ScrFactor.Catalyst.Kl       	  			  = FE_KL  * 1000000;  
  ScrFactor.Catalyst.Kp       	  			  = FE_KP  * 100000;    
}

/*
********************************************************************************
* Description : SCR_Calib_Factory_Init
* Arguments   : none
* Return      : 
* Note        : SCR calibration value factroy initialize
******************************************************************************** 
*/
static void SCR_Calib_Factory_Init(void)
{
  
  memset((char *)&ScrCalib,0xFF,sizeof(stScrCalib));			// MEMORY 영역이 최초에는 0xFF 이기 때문에 0xFF로 초기화

  ScrCalib.scr_firstboot = SET_BOOT_SYS;
  
	ScrCalib.Temp[0].enable				  = 1;
	ScrCalib.Temp[1].enable				  = 1;
	ScrCalib.Temp[0].AdcHigh				= 3858;			//4.5V ADC Value
	ScrCalib.Temp[1].AdcHigh				= 3858;			//4.5V ADC Value
	ScrCalib.Temp[0].AdcLow					= 296;			//0.5V ADC Value
	ScrCalib.Temp[1].AdcLow					= 296;			//0.5V ADC Value
	ScrCalib.Temp[0].RangeHigh			= 720;
	ScrCalib.Temp[1].RangeHigh			= 720;
	ScrCalib.Temp[0].RangeLow				= 0;
	ScrCalib.Temp[1].RangeLow				= 0;
  
	ScrCalib.Supply.enable				  = 1;
	ScrCalib.Supply.AdcHigh				  = 3568;			//4.5V ADC Value
	ScrCalib.Supply.AdcLow  			  = 724;			//0.5V ADC Value
	ScrCalib.Supply.RangeHigh			  = 12000;
	ScrCalib.Supply.RangeLow			  = 0;
	   
	ScrCalib.CalMaf[0].AdcLevel			= 839;
	ScrCalib.CalMaf[1].AdcLevel			= 1639;
	ScrCalib.CalMaf[2].AdcLevel			= 2434;
	ScrCalib.CalMaf[3].AdcLevel			= 3235;
	ScrCalib.CalMaf[4].AdcLevel			= 3637;
	ScrCalib.CalMaf[0].Offset			  = -2;	      // -0.02
	ScrCalib.CalMaf[1].Offset			  = 0;	      // 0.00
	ScrCalib.CalMaf[2].Offset			  = 3;	      // 0.03
	ScrCalib.CalMaf[3].Offset			  = 2;	      // 0.02
	ScrCalib.CalMaf[4].Offset			  = 7;	      // 0.07 
}

/*
********************************************************************************
* Description : SCR_Maf_Factory_Init
* Arguments   : none
* Return      : 
* Note        : SCR calibration value factroy initialize
******************************************************************************** 
*/
static void SCR_Maf_Factory_Init(void)
{
  
  memset((char *)&ScrMaf,0xFF,sizeof(stScrMaf));			// MEMORY 영역이 최초에는 0xFF 이기 때문에 0xFF로 초기화

  ScrCalib.scr_firstboot = SET_BOOT_SYS;
  
	ScrMaf.Adc[0]				                  = 926;
	ScrMaf.Adc[1]				                  = 1236;
	ScrMaf.Adc[2]				                  = 1545;
	ScrMaf.Adc[3]				                  = 1854;
	ScrMaf.Adc[4]				                  = 2163;
	ScrMaf.Adc[5]				                  = 2472;
	ScrMaf.Adc[6]				                  = 2781;
	ScrMaf.Adc[7]				                  = 3090;
	ScrMaf.Adc[8]				                  = 3399;
	ScrMaf.Adc[9]				                  = 3708;
  
	ScrMaf.Value[0]			                  = 30;
	ScrMaf.Value[1]			                  = 92;
	ScrMaf.Value[2]			                  = 182;
	ScrMaf.Value[3]			                  = 315;
	ScrMaf.Value[4]			                  = 509;
	ScrMaf.Value[5]			                  = 787;
	ScrMaf.Value[6]			                  = 1181;
	ScrMaf.Value[7]			                  = 1728;
	ScrMaf.Value[8]			                  = 2478;  
	ScrMaf.Value[8]			                  = 3488;   
}

/*
********************************************************************************
* Description : SYS_Control_Init
* Arguments   : none
* Return      : 
* Note        : SCR initialize
******************************************************************************** 
*/
void SYS_Control_Init(void)
{
	SPI_FLASH_LoadSave(PROJECT_SAVE, NO_SAVE);
	FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, NO_SAVE);
	FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, NO_SAVE);
	FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, NO_SAVE);
	FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, NO_SAVE);
  FRAM_page_readwrite(FRAM_SCR_MAF_SAVE, NO_SAVE);
	
	//FRAM Check.....
	if(memcmp((char*)&SysSet.Valid_Str[0],"STANDARD",8) == 0)				// STANDARD가 쓰여져 있는가  
	{
		if(memcmp((char*)&SysSet.Valid_Str[8],SET_BOOT_SYS_STR,2) != 0){
			SYS_Control_Factory_Init();
			memcpy((char*)&SysSet.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&SysSet.Valid_Str[8],SET_BOOT_SYS_STR,2);
			SPI_FLASH_LoadSave(SYSTEM_SAVE,SAVE);
			printd("Initialzing..\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
	else
	{
		SPI_FLASH_LoadSave(SYSTEM_SAVE,NO_SAVE);						// SysControl_t 값을 Serial Flash에서 읽어왔다
		if(SysSet.sys_firstboot != SET_BOOT_SYS)
		{
			SYS_Control_Factory_Init();
			memcpy((char*)&SysSet.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&SysSet.Valid_Str[8],SET_BOOT_SYS_STR,2);
			SPI_FLASH_LoadSave(SYSTEM_SAVE,SAVE);
			printd("Factory init to FRAM\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}

	//FRAM Check.....
	printd("FRAM SCR Map data ");
	if(memcmp((char*)&ScrMap.Valid_Str[0],"STANDARD",8) == 0)				// STANDARD가 쓰여져 있는가  
	{
		if(memcmp((char*)&ScrMap.Valid_Str[8],SET_BOOT_MAP_STR,2) != 0){
			SCR_Map_Factory_Init();
			memcpy((char*)&ScrMap.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrMap.Valid_Str[8],SET_BOOT_MAP_STR,2);
			FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);
			printd("Initialzing..\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
	else
	{
		if(ScrMap.scr_firstboot != SET_BOOT_SYS)
		{
			SCR_Map_Factory_Init();
			memcpy((char*)&ScrMap.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrMap.Valid_Str[8],SET_BOOT_MAP_STR,2);
			FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);
			printd("Factory init to FRAM\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
    
	//FRAM Check.....
	printd("FRAM SCR Factor data ");
	if(memcmp((char*)&ScrFactor.Valid_Str[0],"STANDARD",8) == 0)				// STANDARD가 쓰여져 있는가  
	{
		if(memcmp((char*)&ScrFactor.Valid_Str[8],SET_BOOT_FAC_STR,2) != 0){
			SCR_Factor_Factory_Init();
			memcpy((char*)&ScrFactor.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrFactor.Valid_Str[8],SET_BOOT_FAC_STR,2);
			FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);
			printd("Initialzing..\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
	else
	{
		if(ScrFactor.scr_firstboot != SET_BOOT_SYS)
		{
			SCR_Factor_Factory_Init();
			memcpy((char*)&ScrFactor.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrFactor.Valid_Str[8],SET_BOOT_FAC_STR,2);
			FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);
			printd("Factory init to FRAM\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
    
	//FRAM Check.....
	printd("FRAM SCR Calibration data ");
	if(memcmp((char*)&ScrCalib.Valid_Str[0],"STANDARD",8) == 0)				// STANDARD가 쓰여져 있는가  
	{
		if(memcmp((char*)&ScrCalib.Valid_Str[8],SET_BOOT_CAL_STR,2) != 0){
			SCR_Calib_Factory_Init();
			memcpy((char*)&ScrCalib.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrCalib.Valid_Str[8],SET_BOOT_CAL_STR,2);
			FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
			printd("Initialzing..\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
	else
	{
		if(ScrCalib.scr_firstboot != SET_BOOT_SYS)
		{
			SCR_Calib_Factory_Init();
			memcpy((char*)&ScrCalib.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrCalib.Valid_Str[8],SET_BOOT_CAL_STR,2);
			FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
			printd("Factory init to FRAM\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
  
	//FRAM Check.....
	printd("FRAM SCR Maf data ");
	if(memcmp((char*)&ScrMaf.Valid_Str[0],"STANDARD",8) == 0)				// STANDARD가 쓰여져 있는가  
	{
		if(memcmp((char*)&ScrMaf.Valid_Str[8],SET_BOOT_CAL_STR,2) != 0){
			SCR_Maf_Factory_Init();
			memcpy((char*)&ScrMaf.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrMaf.Valid_Str[8],SET_BOOT_CAL_STR,2);
			FRAM_page_readwrite(FRAM_SCR_MAF_SAVE, SAVE);
			printd("Initialzing..\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
	else
	{
		if(ScrMaf.scr_firstboot != SET_BOOT_SYS)
		{
			SCR_Maf_Factory_Init();
			memcpy((char*)&ScrMaf.Valid_Str[0],"STANDARD",8);
			memcpy((char*)&ScrMaf.Valid_Str[8],SET_BOOT_CAL_STR,2);
			FRAM_page_readwrite(FRAM_SCR_MAF_SAVE, SAVE);
			printd("Factory init to FRAM\n");
		}
		else
		{
			printd("Loading..(FRAM)\n");
		}
	}
  
	memset((INT8U *)&SysSts,0x00,sizeof(stSysStatus));	// 변수들 0으로 초기화
	Operator_version();
	dl.mode  = CMD_DLMODE_NORMAL;
	Cdo.mode = DL_NONE;
	Cdo.WriteOnePacketSize		  = 256;
	Cdo.WritePacketCount		    = 0;
	SysSts.Version              = 0x0100;

	SysSts.supply.Supply_use    = 0;
	SysSts.supply.ContrlSet	    = STOP;
	SysSts.supply.Gpio_Period   = 500;          //500msec
	SysSts.supply.Duty_Cycle    = 85;           //20%
	SysSts.supply.Pwm_Count     = 0;

	SysSts.NoxSensorFlag[0]	    = 0;            //NoxSensor on/off flag
	SysSts.NoxSensorFlag[1]	    = 0;            //NoxSensor on/off flag
	SysSts.Noxppm[0]		        = 0;            //default
	SysSts.Noxppm[1]		        = 0;            //default

	SysSts.supply.Supply_debug  = 0;
	SysSts.TotalDosingRate      = 0.0;	        //누적 분사량 초기화
	SysSts.SCRMode			        = AUTO_MODE;
	SysSts.NoxErr[0]		        = 0;
	SysSts.NoxErr[1]		        = 0;

	SysSts.SystemError.B	      = 0;
	SysSts.SystemCheck.W	      = 0;
	SysSts.SystemSignal.W		    = 0;
	SysSts.NoxRxCount[0]	      = 0;
	SysSts.NoxLinkSt[0]         = FAIL;
	SysSts.NoxRxCount[1]	      = 0;
	SysSts.NoxLinkSt[1]         = FAIL;

	SysSts.AlarmCriticalSet	    = ALARMSET;

	SysSts.MDosingStart		      = 0;
	SysSts.preUk                = 0.0;
  SysSts.preXh                = 0.0;
  SysSts.preXk                = 0.0;
  
	SysSts.reducCount			      = 0;
	SysSts.NoxReduction			    = 0;          //저감율 0%

	ScrFlag.TimeRequest         = 0;
	ScrFlag.ScrFlowStart        = 0;
	ScrFlag.Reduction           = 0;

  if(SysSet.ScrTestMode == 1)
    SysSts.SystemSignal.Operation = ON;	
  else
    SysSts.SystemSignal.Operation = OFF;

  if(SysSet.ScrPurge == 1)
    SysSts.SystemSignal.Purge = ON;	
  else
    SysSts.SystemSignal.Purge = OFF;

	if(SysSet.ScrPurgeTime > 180) 
    SysSet.ScrPurgeTime = 180;

	if(SysSet.Scr_Period == 2) 
    SCR_PERIOD = 250;
  else
    SCR_PERIOD = 500;
  
  SysSet.ScrTestMode = 0;   //Auto Mode 문제로 초기화
}

/*
********************************************************************************
* Description : SolPressureCtrl
* Arguments   : none
* Return      : 
* Note        : supply module sol contorl
******************************************************************************** 
*/
void SolPressureCtrl(INT8U onoff)
{
	OUTPUT_SOL(onoff & 0x1);
}

/*
********************************************************************************
* Description : MotorCtrl
* Arguments   : none
* Return      : 
* Note        : Supply module motor contorl
******************************************************************************** 
*/
void MotorCtrl(INT8U onoff)
{
	OUTPUT_MOTOR(onoff & 0x1);
  if(onoff) StartPwm();
  else EndPwm();
}

/*
********************************************************************************
* Description : SupplyPower
* Arguments   : none
* Return      : 
* Note        : Supply power control
******************************************************************************** 
*/
void SupplyPower(INT8U onoff)
{
	OUTPUT_POWER(onoff & 0x1);
}

/*
********************************************************************************
* Description : HeaterCtrl
* Arguments   : none
* Return      : 
* Note        : Supply module hearter contorl
******************************************************************************** 
*/
void HeaterCtrl(INT8U onoff)
{
	OUTPUT_DO(onoff & 0x1);
}


/*
********************************************************************************
* Description : GramPerHourToDM
* Arguments   : status
* Return      : 
* Note        : 1회 분사후 나머지 분사해야 되는 양을 다음 분사량에 더한다.
               다음 분사량에 더하는 변수는 Count 값
******************************************************************************** 
*/
INT16U GramPerHourToDM(double Gram, INT16U ScrPeriod)
{ 
	double DosingDuty = 0, DosingRate = 0;

  if(SysSet.Nozzle == 2) {    // Water Type Nozzle
    DosingDuty = (Gram - 21.16) / 71.1748;
  }
  else {                      // Air Type Nozzle
    if(Gram > 314.0)
      DosingDuty = ((Gram - 100.94) / 42.616) * 1.08; // 1.08는 분사량을 조정하기 위해 곱함.
    else
      DosingDuty = (5 * Gram) / 314.0;                // detec
  }
  if(SCR_PERIOD == 500) 
    DosingRate = (Gram / 3600.0);       // 1Hz
  else   
    DosingRate = (Gram / 3600.0) / 2.0; // 2Hz
 
  if(DosingDuty < 0.0) DosingDuty = 0.0; // 1% 이하는 분사 안함.
  else if(DosingDuty > 100.0) DosingDuty = 100.0;
  
  SysSts.DosingDuty = (INT16U)DosingDuty;
  
  if(DosingDuty) {
    DosingDuty = ((double)ScrPeriod / 10.0)  * DosingDuty; 
    SysSts.TotalDosingRate += DosingRate;
  }
	return (INT16U)DosingDuty;
}

/*
********************************************************************************
*                                            NoxSensorToMolperM3()
*
* Description : NOx Sensor에서 읽은 PPM 값을 mol/m3로 변환하여 리턴한다..
*
* Argument(s) : INT8U channel(ch 0: Nox Sensor Inlet, ch 1: Nox Sensor Outlet
*
* Return(s)   : mol/m3.
*
* Caller(s)   : 
*
* Note(s)     : Doc 폴더에 SCR 수식변환.xlsx, Sheet "단위환산" 참고할것.
********************************************************************************
*/

double NoxSensorToMolperM3(INT8U ch)
{
	double MolPerM3 = 0.0;

	if( SysSts.Noxppm[ch] > 0 )
		MolPerM3 = (double)SysSts.Noxppm[ch] / 22414.0;
	else
		MolPerM3 = 0.0;

	return MolPerM3;
}

/*
********************************************************************************
* Gamma()
*
* Description : 단면적과 유량을 이용한 유속을 구하고 감마R의 값을 구한다.
*
* Argument(s) : none
*
* Return(s)   : 
*
* Caller(s)   : 
*
* Note(s)     : Doc 폴더에 SCR 수식변환.xlsx, Sheet "Sheet1" 참고할것.
* 유속(m/s)    = MAF/(1.29*3600*A*OFA)*(273/(273+Tmaf);
* 단면적 A(m2) = SCR 면적 / 100000
* 감마r        = 유속/Ohm*len
********************************************************************************
*/
double Gamma(void)
{
  double scrarea, gamma;
  double ofa;
  double maf_temp, kelvin_temp;
  double scr_lenth;
  double omega_s1, omega_s2;
	struct tm tm_ptr;

  scrarea   = (double)ScrFactor.Catalyst.ScrArea / 100000.0;
  ofa       = (double)ScrFactor.Catalyst.OFA / 100.0;
  maf_temp  = (double)SysSts.Maf_temp;
  kelvin_temp  =  273.0 / (273.0 + maf_temp); 
  scr_lenth = (double)ScrFactor.Catalyst.L / 1000.0;
  omega_s1  = (double)ScrFactor.Catalyst.S1 / 10.0;
  omega_s2  = (double)ScrFactor.Catalyst.S2 / 100.0;
	SysSts.BackupMaf    = (double)SysSts.MafKg_H;
  SysSts.Omega        = omega_s1 * exp((-1.0 * omega_s2) * SysSts.Tavg_temp);
  SysSts.Vk           = SysSts.BackupMaf / ((1.29 * 3600.0 * scrarea * ofa) * kelvin_temp);
  gamma               = SysSts.Vk / (scr_lenth * SysSts.Omega);
	      
	if ( scr_debug ) 
	{
#if 0
		printk("MAF = [%.5f], MAF_temp = [%d], \n",
			SysSts.BackupMaf ,
			SysSts.MAF_temp,
			);
#endif
		GetRtcTime(&tm_ptr);
		printk("\n\n");
		printk("[%s]  ",WUITIME(&tm_ptr));

		printk(" SysSts.Vk = [%.10f] ,SysSts.Gammar = [%.10f]\n",SysSts.Vk, gamma);
	}
  return gamma;
}

/*
********************************************************************************
* XcCal()
*
* Description : Xc 값을 구합.
*
* Argument(s) : Yk, NoxUP, NoxDN
*
* Return(s)   : 
*
* Caller(s)   : 
*
* Note(s)     : Doc 폴더에 SCR 수식변환.xlsx, Sheet "Sheet1" 참고할것.
********************************************************************************
*/
double XcCal(double noxdn, double NoxUP, double NoxDN)
{
  double kr, xc;
  
  kr = (double)ScrFactor.Catalyst.Kr;
  if(noxdn == 0.0) {
    xc = ((NoxUP / 1.0) - 1.0) * SysSts.Gamma / kr;
  }
  else {
    xc = ((NoxUP / NoxDN) - 1.0) * SysSts.Gamma / kr; 
  }
  return xc;    
}

/*
********************************************************************************
* H1Cal()
*
* Description : h1 값을 구합.
*
* Argument(s) : noxup
*
* Return(s)   : 
*
* Caller(s)   : 
*
* Note(s)     : Doc 폴더에 SCR 수식변환.xlsx, Sheet "Sheet1" 참고할것.
********************************************************************************
*/
double H1Cal(double noxup)
{
  double h1, kr;
  
  kr = (double)ScrFactor.Catalyst.Kr;
  h1 = (noxup * SysSts.Gamma) / (SysSts.Gamma +(kr * SysSts.preXk));
  if(h1 < 0.0) h1 = 0.0;
  
  SysSts.H1k = h1 * 22414.0;
  
  return h1;
}
     
/*
********************************************************************************
* H2Cal()
*
* Description : h2 값을 구합.
*
* Argument(s) : none
*
* Return(s)   : 
*
* Caller(s)   : 
*
* Note(s)     : Doc 폴더에 SCR 수식변환.xlsx, Sheet "Sheet1" 참고할것.
********************************************************************************
*/
double H2Cal(void)
{
  double h2, ka, kd;
  
  kd = (double)ScrFactor.Catalyst.Kd;
  ka = (double)ScrFactor.Catalyst.Ka / 100.0;
  h2 = ((kd * SysSts.preXk) + (SysSts.Gamma * SysSts.preUk)) / (SysSts.Gamma + (ka * (1.0-SysSts.preXk)));
  if(h2 < 0.0) h2 = 0.0;
 
  SysSts.H2k = h2 * 22414.0;
  
  return h2;
}

/*****************************************************************************
*                                            UreaSolMolperM3ToKgperHoure()
*
* Description : mol/m3 값을 g/h로 변환한다.
*
* Argument(s) : Uk+1
*
* Return(s)   : gperhoure(g/h)
* 분사량 구하는 식 : 71.55622 * nh3_in(k+1) * MAF
******************************************************************************
*/
double UreaSolMolperM3ToKgperHoure(double u_molperm3)
{
	double gperhoure;
  gperhoure = 71.55622 * u_molperm3 * SysSts.BackupMaf;

	return gperhoure;
}

/*
********************************************************************************
* Description : FactorCall
* Arguments   : value
* Return      : 
* Note        : Table data read
******************************************************************************** 
*/
INT8U FactorCall(void)
{
	INT8U ret=0;
	INT16U maf_val, temp_val, i, k;
	INT8U  x = 0, y = 0, dev = 0, mod = 0;

	maf_val = SysSts.MafKg_H;
	temp_val = SysSts.Tavg_temp;

	if(	temp_val >= ScrMap.BaseTemp[0] && maf_val >= ScrMap.BaseMaf[0] ) {
		for(i=0; i<24; i++) {
			if(temp_val >= ScrMap.BaseTemp[23]) { x = 23; break;}
			else if( temp_val < ScrMap.BaseTemp[0])  { x = 0; break; }
			else if( temp_val >= ScrMap.BaseTemp[i] && temp_val < ScrMap.BaseTemp[i+1]) {
				x=i;
				break;
			}
		}
		for(k=0; k<24; k++) {
			if(maf_val >= ScrMap.BaseMaf[23]) {	y = 23; break;}
			else if(maf_val < ScrMap.BaseMaf[0]) {	y = 0; break;}
			else if(maf_val >= ScrMap.BaseMaf[k] && maf_val < ScrMap.BaseMaf[k+1]) {
				y=k;
				break;
			}
		}
    dev = y / 8;
    mod = y % 8;
		ret = ScrMap.AlpaMap.row[(x*3)+dev].col[mod];
	}
	else {
		ret = 0;
	}
  
  SysSts.Map_X = x;
  SysSts.Map_Y = y;
	if ( scr_debug )printk("FactorCall = %d, x= %d, y = %d\n",ret, y,x);
	return ret;

}

/*
********************************************************************************
* ScrCalculate()
*
* Description : 분사량 계산
*
* Argument(s) : None
*
* Return(s)   : 
*
* Caller(s)   : 
*
* Note(s)     : Doc 폴더에 SCR 수식변환.xlsx, Sheet "Sheet1" 참고할것.
********************************************************************************
*/
int ScrCalculate(void)
{
	double nox_up, nox_dn;  // mol/m3
  double NOX_UP, NOX_DN;  // PPM
  double yk, dt, h1, h2, cross_sen, mapoffset;
	double alpha, formula1, formula2, formula3;
//  double min_dosing;
  
	SysSts.Kl   = (double)ScrFactor.Catalyst.Kl / 1000000.0;  
	SysSts.Kp   = (double)ScrFactor.Catalyst.Kp / 100000.0;
  SysSts.Xsp  = (double)ScrFactor.Catalyst.Xsp / 100.0;
  cross_sen   = (double)ScrFactor.Catalyst.Coeff / 100.0;
	dt = (double)SCR_PERIOD / 1000.0;   //Dosing Period

	SysSts.Gamma = Gamma();							//유속과 감마을 계산한다.
	nox_up = NoxSensorToMolperM3(0);		//Inlit(입력 NOx Sensor)
	nox_dn = NoxSensorToMolperM3(1);		//Outlit(출력 NOx Sensor)
  NOX_UP = (double)SysSts.Noxppm[0];
  NOX_DN = (double)SysSts.Noxppm[1];
  
  h1 = H1Cal(nox_up);
  h2 = H2Cal();
  
	////////////////////////////////////
	if ( scr_debug )
	{
		printk("kl = [%.10f]\n",SysSts.Kl);
	}
	///////////////////////////////////

  SysSts.Xc = XcCal(nox_dn, NOX_UP, NOX_DN);
  if(SysSts.Xc > 0.0) {
    if(SysSts.Xc > 1.0) 
      SysSts.Xc = 1.0;
  }
  else SysSts.Xc = 0.0;

  yk = h1 + (cross_sen * h2);  
  SysSts.Yk = yk * 22414.0;
  
  SysSts.curXk = SysSts.preXk + (dt * (SysSts.Gamma * (SysSts.preUk + h1 - h2 - nox_up)));
  if(SysSts.curXk > 0.0) {
    if(SysSts.curXk > 1.0)
      SysSts.curXk = 1.0;
  }
  else SysSts.curXk = 0.0;

  SysSts.curXh = SysSts.preXh + (dt * ((SysSts.Gamma *(SysSts.preUk + h1 - h2 - nox_up)) - (SysSts.Kl * (NOX_UP / 200.0) * (SysSts.preXh -  SysSts.Xc))));
  if(SysSts.curXh > 0.0) {
    if(SysSts.curXh > 1.0)
      SysSts.curXh = 1.0;
  }
  else SysSts.curXh = 0.0;

  if(SysSts.SCRMode == ALPHA_MODE) { // ALPHA MODE
    SysSts.StatusAlpha = SysSts.AlphaSet;
    alpha = (double)SysSts.StatusAlpha / 100.0;
  }
  else {                            // AUTO MODE
    SysSts.StatusAlpha = FactorCall();
    mapoffset = (double)SysSet.MapOffset / 1000.0;
    alpha = ((double)SysSts.StatusAlpha / 100.0) * mapoffset; // Map 전체를 Offset 만틈 이동
  }
  
  formula1 = alpha * nox_up;
  SysSts.Formula1 = formula1 * 22414.0;
  
  formula2 = (SysSts.Kl * ((NOX_UP / 200.0) * (SysSts.curXh -  SysSts.Xc))) / SysSts.Gamma;
  SysSts.Formula2 = formula2 * 22414.0;
  
  formula3 = (SysSts.Kp * (SysSts.Xsp - SysSts.curXh)) / SysSts.Gamma; 
  SysSts.Formula3 = formula3 * 22414.0;
  
  if(SysSet.Scr_Mode == 1) {                // Mode 1
    SysSts.curUk = formula1;
  }
  else if(SysSet.Scr_Mode == 2) {           // Mode 2
    SysSts.curUk = formula1 + formula2;
  }
  else if(SysSet.Scr_Mode == 3) {           // Mode 3
    SysSts.curUk = formula1 + formula3;
  }
  else {                                    // Mode 4
    SysSts.curUk = formula1 + formula2 + formula3;
  }
  if(SysSts.curUk > 0.0) {
    if(SysSts.curUk > (nox_up * 1.5))
      SysSts.curUk = (nox_up * 1.5);
  }
  else SysSts.curUk = 0.0;
	if(scr_debug )printk("SysSts.curUk = [%.10f]\n",SysSts.curUk);
  
	if ( scr_debug ) {
		printk("\nUk[%.10f]        = Formula1[%.10f]       + Formula2[%.10f]      + Formula3[%.10f]\n",
           SysSts.curUk, SysSts.Formula1,SysSts.Formula2,SysSts.Formula3);
	}
  SysSts.DosingRatehouer = UreaSolMolperM3ToKgperHoure(SysSts.curUk);
//  SysSts.DosingRatehouer = SysSts.DosingRatehouer * 1.04;  // 동력계 테스트 확인용 !!!!!!!!!!! 
  
// 2021.07.28
//  if(SysSet.Nozzle == 2) 
//    min_dosing = 21.16;   // Wate type
//  else 
//    min_dosing = 100.94;  // Air type
//      
//  if(SysSts.DosingRatehouer < min_dosing ) {  //최소도징량 보다 작으면 분사하지 않음.
//    SysSts.curUk = 0.0;
//    SysSts.DosingRatehouer = 0.0;
//  }
  
//아래 함수에서 실분사량이 이론치 보다 작으면, 이론치를 실분사량으로 치환 
//그리고, Max 분사량으로 부터 curUk을 다시 구해 치환         
  SysSts.dosing.Gpio_Period =  GramPerHourToDM(SysSts.DosingRatehouer, SCR_PERIOD);

	if ( scr_debug ) {
		printk("\nNoxin, Noxout,  Temp,  MAF,     KL,    KP,     Xc,,       cammar,     v(m/s),     Xk+1,     Xk,     Xhk+1,  Xhk,    Uk+1,     Uk,    Dosingrate\n");
		printk("%04d,   %04d,   %04d,  %.1f, %.5f, %.2f, %.10f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f\n",
			SysSts.Noxppm[0],
			SysSts.Noxppm[1],
			SysSts.T1_temp,
			SysSts.BackupMaf,
			SysSts.Kl,
			SysSts.Kp,
			SysSts.Xc,
			SysSts.Gamma,
			SysSts.Vk,
			SysSts.curXk,
			SysSts.preXk,
			SysSts.curXh,
			SysSts.preXh,
      SysSts.curUk,  
			SysSts.preUk,
			SysSts.DosingRatehouer);
	}
	SysSts.preUk         = SysSts.curUk;
  SysSts.preXh         = SysSts.curXh;
  SysSts.preXk         = SysSts.curXk;
	return 0;
}

/*
********************************************************************************
* Description : ScrCheckEnvironment
* Arguments   : status
* Return      : 
* Note        : SCR environment check
******************************************************************************** 
*/
INT8U ScrCheckEnvironment(void)
{
	INT8U st = 0;

  if(SysSts.SCRMode == MANUAL_MODE)
    st = 1;
  else {
    if( (SysSts.NoxErr[0] & 0x10) && (SysSts.NoxErr[1] & 0x10)  \
      &&  !SysSts.SystemError.ULevel && !SysSts.SystemError.UTemp) {
      st = 1;
    }
  }
	return st;
}

/*
********************************************************************************
* Description : ScrTask
* Arguments   : status
* Return      : 
* Note        : Scr Task
******************************************************************************** 
*/
INT8U ScrTask(void)
{
	INT8U	check = 0;
	static INT32U	FirstBootTime = 0;
	static INT16U	nWaitDelay=0;

	switch( nStatus ) {
	case SCR_CHECKMODE:
		if(FirstFlag == 0) {			//First booting 이면 설정한 Time만큼 Delay을 가져가기 위함...
			FirstBootTime++;
			if(FirstBootTime % 0x100 == 0) {
				if ( scr_debug ) printk("FirstBootTime = %d\n",FirstBootTime);
			}
			if(SysSts.SCRMode <= ALPHA_MODE) {
				check = ScrCheckEnvironment();
        if(SysSet.ScrWaitTime < SysSet.ScrPurgeTime)
          SysSet.ScrWaitTime = SysSet.ScrPurgeTime;
				if( FirstBootTime >= (100 * SysSet.ScrWaitTime)) {										//10분 경과 했냐?(50msec 주기 : 12000, 20msec 주기 : 30000, 10msec 주기는 60000)
					FirstBootTime = (100 * SysSet.ScrWaitTime);
					if(check) {         // check = 1 정상
						ScrFlag.ScrFlowStart = 1;							// scr 동작을 start 했다는 flag
						FirstFlag = 1;
						nStatus = SCR_SENSORCK;
						if ( scr_debug ) printk("Environment Check Ok\n");
					}
					else {
						nStatus = SCR_ENVCK_WAIT;
						if ( scr_debug ) printk("Environment Check [%d]\n",check);
					}
          TmrSetT(TMR_SCRPRIODEINTERVAL, SCR_PERIOD);		// 주기 Check Timer 가동
          TmrStart(TMR_SCRPRIODEINTERVAL);
				}
        else {
          if(SysSet.ScrPurge && PurgeFlag == 0) {
            ScrFlag.Flushing = 1;
            ScrFlag.SupplyStart = 1;
            SysSts.SystemSignal.Supply = ON;
            PurgeSolFlag = 0;
            if(SysSts.P1_bar >= SysSet.LowP1) {
//              PWM_SetInject(50);  // 향후 판단
              ScrFlag.SupplyStart = 0;
              ScrFlag.Flushing = 0;
              FlushingCount = 0;
              PurgeFlag = 1;              
            }            
          }
        }
			}
			else {
				FirstFlag = 1;
				nStatus = SUPPLY_MANUAL;
			}
		}
		else {																	//First boot가 아니면......
			if(SysSts.SCRMode <= ALPHA_MODE) {
				check = ScrCheckEnvironment();
				if(check)	{         // check = 1 정상
					ScrFlag.ScrFlowStart = 1;								// scr 동작을 start 했다는 flag
					nStatus = SCR_SENSORCK;
				}
				else {
					nStatus = SCR_ENVCK_WAIT;
				}
        TmrSetT(TMR_SCRPRIODEINTERVAL, SCR_PERIOD);		// 주기 Check Timer 가동
        TmrStart(TMR_SCRPRIODEINTERVAL);
			}
			else {
				nStatus = SUPPLY_MANUAL;
			}
		}
		break;

	case SCR_SENSORON:
		//printk("in [%d]nox sensor on...\n",nStatus);
		if(SysSts.NoxSensorFlag[0] == OFF || SysSts.NoxSensorFlag[0] == NWAIT) {
			SysSts.NoxSensorFlag[0] = ON;
			SysSts.NoxSensorFlag[1] = ON;
			nStatus = SCR_SENSORONWAIT;
		}
		else {
			nStatus = SCR_SENSORCK;
		}
		break;

	case SCR_SENSORONWAIT:		//초기 Nox Sensor을 On할경우 Delay을 가져감...
		nWaitDelay++;
		if(nWaitDelay >= 5) {
			nWaitDelay = 0;
			nStatus = SCR_SENSORCK;
		}
		//printk("out [%d]nox sensor check wait...\n",nStatus);
		break;

	case SCR_SENSORCK:
		if(SysSts.AlarmCriticalSet == ALARMSET)
			nStatus = SCR_ALARMCK;
		else {
      if(SysSet.ScrTestMode == 1) {
        ScrFlag.DosingStart = 0;
        ScrFlag.SupplyStart = 0;
        EndDosing();
        SysSts.preUk = 0;
        SysSts.preXh = 0;
        SysSts.preXk = 0;        
        nStatus = SCR_ALARMCK;        
      }
      else {
        nStatus = SCR_SUPPLYMODULE;
      }
    }
		break;

	case SCR_SUPPLYMODULE:
    if(!ScrFlag.SupplyStart && !ScrFlag.SupplyT1Cut) {
      if ( scr_debug )printk("in [%d]supply start...\n",nStatus);
      ScrFlag.SupplyStart = 1;
      SysSts.SystemSignal.Supply = ON;
      if ( scr_debug )printk("out [%d]supply start...\n",nStatus);
    }
    nStatus = SCR_ALARMCK;
		break;

	case SCR_ALARMCK:
		if( SysSts.SystemError.Noxin || SysSts.SystemError.Noxout ) {
			if ( scr_debug )printk("Nox Sensor Alarm set.....\n");
      if(SysSts.SCRMode == MANUAL_MODE) 
         SysSts.AlarmCriticalSet = ALARMCLEAR;  
      else 
         SysSts.AlarmCriticalSet = ALARMSET;
		}
    else 
      SysSts.AlarmCriticalSet = ALARMCLEAR;
 
		nStatus = SCR_ENVCK_WAIT;
		if ( scr_debug )printk("out [%d]Alarm check start...\n",nStatus);
		break;
    
	case SCR_ENVCK_WAIT:
		if(TmrChk(TMR_SCRPRIODEINTERVAL) == 0) {
			nStatus = SCR_CHECKMODE;
		}
		break;

	//////////////////////////////Dosng Mode////////////////////////////////////////////
	case SUPPLY_MANUAL:
		//if(SysSts.SCRMode == DOSING_MODE) {
			if(SysSts.MDosingStart || ScrFlag.Flushing) {
        ScrFlag.SupplyStart = 1;
        SysSts.SystemSignal.Supply = ON;
      }
      else {
        ScrFlag.DosingStart = 0;
        ScrFlag.SupplyStart = 0;
      }
		//}
    nStatus = SCR_CHECKMODE;          
		break;
	default:
		break;
	}
	//printk("scr task return  = %d\n",nStatus);
	return nStatus;
}

/*
********************************************************************************
* Description : Task_Scr
* Arguments   : none
* Return      : 
* Note        : Scr Task
******************************************************************************** 
*/
#define KEY_SIG_TIMING 2.5
void KeyOffCheck(void)
{
  sysinfo_t *s = &_sysinfo;
  
	/*Key Off시 Power Off*/
  switch(s->key.nStatus) {
  case 0:
		if(s->key.keyon_flag) {
			if(KEYON_SIG() == 0) {
        s->key.keyon_st = ON;
				s->key.keyon_flag = 0;
      }
			else {
				s->key.keyon_flag++;
				if(s->key.keyon_flag > (KEY_SIG_TIMING * 100)) { //2.5sec
					s->key.keyon_st = OFF;
					s->key.keyon_flag = 0;
          s->key.nStatus = 1;
				}
			}
		}
    break;
  case 1:
    if(KEYON_SIG() == 0) {
      s->key.keyon_st = ON;
      s->key.nStatus= 0;
    }
    else {
      SysSts.NoxSensorFlag[0] = OFF;
      SysSts.NoxSensorFlag[1] = OFF;
      //FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);
      if(SysSet.ScrPurge) {
        nStatus = SUPPLY_MANUAL;
        ScrFlag.Flushing = 1;
        PurgeSolFlag = 1;
        if(SysSts.SystemSignal.Dosing) {
          EndDosing();
          ScrFlag.DosingStart = 0;
          SysSts.SystemSignal.Dosing = OFF;
        }
      }
      s->key.nStatus = 2;
    }
    break;
  case 2:
    if(KEYON_SIG() == 0) {  // key on
      s->key.nStatus= 0;
      s->key.keyon_st = ON;
      s->key.keyon_flag = 0;
      SysSts.NoxSensorFlag[0] = ON;
      SysSts.NoxSensorFlag[1] = ON;
      if(SysSet.ScrPurge) {        
        nStatus = SCR_CHECKMODE;
        ScrFlag.Flushing = 0;
        ScrFlag.SupplyStart = 0;
        SCR_Control_Off();
      }
    }
    else {                // key off
      if(SysSet.ScrPurge) {
        s->key.keyon_flag++;
        if(s->key.keyon_flag > ((SysSet.ScrPurgeTime * 100) + 500) || ScrFlag.Flushing == 0) {
          printk("power off\n");
          system_power_off();
        }
     }
      else {
        printk("power off\n");
        system_power_off();
      }
    }
    break;
  default:
    break;
  }
}

/*
********************************************************************************
* Description : Task_Scr
* Arguments   : none
* Return      : 
* Note        : Scr Task
******************************************************************************** 
*/
void Task_Scr(void *p_arg)
{
	sysinfo_t *s = (sysinfo_t *)p_arg 	;
		
	for(;;)
	{
		s->twatchScrFlag = 1;
		if(TmrChk(TMR_SCRINTERVAL) == 0) {
			TmrSetT(TMR_SCRINTERVAL, 10);
			TmrStart(TMR_SCRINTERVAL);
			ScrTask();
      KeyOffCheck();
		}
		s->twatchScrFlag = 0;
		vTaskDelay(1);
	}
}

/*
********************************************************************************
* Description : NoxCheck
* Arguments   : none
* Return      : 
* Note        : NOx sensor check
******************************************************************************** 
*/
INT8U NoxCheck(void)
{
	INT8U ret = 0;

	if( (SysSts.NoxErr[0] & 0x10)) ret += 1;	//Inlet nox Sensor In range
	if( (SysSts.NoxErr[1] & 0x10)) ret += 1; //Outlet nox Sensor In range

	return ret;
}

/*
********************************************************************************
* Description : AlarmCheckNoxn
* Arguments   : none
* Return      : 
* Note        : NOx sensor alarm check
******************************************************************************** 
*/
void AlarmCheckNoxn(INT8U ch)
{
	static INT8U noxlinkCount[2]={0,0};
	static INT8U noxCheckCount[2]={0,0};
	static INT16U NoxLinkCountback[2]={1,1};

	if(NoxLinkCountback[ch] != SysSts.NoxRxCount[ch] ) { //task에서 Receive Count가 증가하면...송신이 잘되고 있다.
		SysSts.NoxLinkSt[ch] = OK;
		NoxLinkCountback[ch] = SysSts.NoxRxCount[ch];
	}
	else {
		SysSts.NoxLinkSt[ch] = FAIL;
	}

	if(SysSts.NoxLinkSt[ch] == FAIL) {
		noxlinkCount[ch]++;
		if(noxlinkCount[ch] >= ALARM_MAXCOUNT) {
      if(!ch)
        SysSts.SystemError.Noxin = 1;
      else
        SysSts.SystemError.Noxout = 1;
		}
		else if(noxlinkCount[ch] >= ALARM_CHECKCOUNT) {
      if(!ch)
        SysSts.SystemCheck.Noxin = 1;
      else
        SysSts.SystemCheck.Noxout = 1;
		}
    if(!ch)
      SysSts.SystemSignal.Noxin = 0;
    else
      SysSts.SystemSignal.Noxout = 0; 
		if ( scr_debug )printk("Nox ch = %d link fail.....\n",ch);
	}
	else {
		noxlinkCount[ch] = 0;
		if(SysSts.NoxSensorFlag[ch] == ON) {
			if( (SysSts.NoxErr[ch] & 0x10) == 0 )	{ //Check//0x55
				noxCheckCount[ch]++;
        if(noxlinkCount[ch] >= ALARM_MAXCOUNT) {
          if(!ch)
            SysSts.SystemError.Noxin = 1;
          else
            SysSts.SystemError.Noxout = 1;
        }        
				else if(noxCheckCount[ch] >= ALARM_CHECKCOUNT) {
          if(!ch)
            SysSts.SystemCheck.Noxin = 1;
          else 
            SysSts.SystemCheck.Noxout = 1;
        }
			}
			else {
				noxCheckCount[ch] = 0;
        if(!ch)
          SysSts.SystemCheck.Noxin = 0;
        else 
          SysSts.SystemCheck.Noxout = 0;
        if(!ch)
          SysSts.SystemError.Noxin = 0;
        else 
          SysSts.SystemError.Noxout = 0;        
			}
      if(!ch)
        SysSts.SystemSignal.Noxin = 1;
      else
        SysSts.SystemSignal.Noxout = 1; 
		}
		else {
			noxCheckCount[ch] = 0;
      if(!ch)
        SysSts.SystemCheck.Noxin = 0;
      else 
        SysSts.SystemCheck.Noxout = 0;
      if(!ch)
        SysSts.SystemError.Noxin = 0;
      else 
        SysSts.SystemError.Noxout = 0; 
		}
	}
}

/*
********************************************************************************
* Description : ModuleTask
* Arguments   : none
* Return      : 
* Note        : Module Task
******************************************************************************** 
*/
void ModuleTask(void)
{
	static INT8U  SupplyT1Count=0;
	static INT8U  DosingT1Count=0;
  static INT8U  SupplyCount=0;
	static INT8U  MAFCount=0;
	static INT8U  SolCloseCount=0;
  
  if(ScrFlag.SupplyStart == 1) {
    if(SysSts.T1_temp < (SysSet.LowT1 - 10) || SysSts.T1_temp > (SysSet.HighT1 + 10)) { //SupplyModule T1 Check
      SupplyT1Count++;
      if(SupplyT1Count >= 30) {   // 3초 체크
        SupplyT1Count = 0;
        ScrFlag.SupplyT1Cut = 1;
      }
    }
    else {
      SupplyT1Count = 0;
      ScrFlag.SupplyT1Cut = 0;
    }
    
    if(SysSts.SCRMode <= ALPHA_MODE) {
      if(SupplyCount >= 30) {   // 3초 체크
        if(SysSet.ScrPurge && (KEYON_SIG() || !FirstFlag)) PurgeFlag = 1;
        else if(ScrFlag.SupplyT1Cut || !SysSts.MafKg_H) ScrFlag.SupplyStart = 0;
        SupplyCount = 0; 
      }
      else SupplyCount++;
    }

    //DosingModule
    if(SysSts.T1_temp < SysSet.LowT1 || SysSts.T1_temp > SysSet.HighT1) { //T1 Check
      DosingT1Count++;
      if(DosingT1Count >= 30) {   // 3초 체크
        DosingT1Count = 0;
        ScrFlag.ModuleT1Cut = 1;
      }
    }
    else {
      DosingT1Count = 0;
      ScrFlag.ModuleT1Cut = 0;
    }
    
    if(SysSts.MafKg_H < SysSet.CutMaf) { //MAF Check
      MAFCount++;
      if(MAFCount >= 30) {
        MAFCount = 0;
        ScrFlag.MAFCut = 1;
      }
    }
    else {
      MAFCount = 0;
      ScrFlag.MAFCut = 0;
    }
    
    if(!SysSts.MDosingStart && ScrFlag.DosingStart && (SysSts.SCRMode <= ALPHA_MODE && ScrFlag.ModuleT1Cut))// || SysSts.ScrFlag.MAFCut 
      ScrFlag.DosingStart = 0;
    else if(!ScrFlag.DosingStart && !ScrFlag.ModuleT1Cut)
      ScrFlag.DosingStart = 1;
  
    if(SysSts.P1_bar > SysSet.HighP1) { //sol OnOff
      SolCloseCount++;
      if(SolCloseCount > 30) {   // 3초 체크
        ScrFlag.SupplyStart = 0;
        SolCloseCount = 0;
      }
    }
    else {
       SolCloseCount = 0;
    }
  }
  else {
    if(SysSts.T1_temp < (SysSet.LowT1 - 10) || SysSts.T1_temp > (SysSet.HighT1 + 10)) { //SupplyModule T1 Check
      ScrFlag.SupplyT1Cut = 1;
    }
    else {
      ScrFlag.SupplyT1Cut = 0;
    }
  }
}

/*
********************************************************************************
* Description : AlarmClear
* Arguments   : none
* Return      : 
* Note        : Alarm clear
******************************************************************************** 
*/
void AlarmClear(void)
{
	SysSts.SystemSignal.W = 0;
	SysSts.SystemCheck.W  = 0;
	SysSts.SystemError.B  = 0;
}

/*
********************************************************************************
* Description : AlarmTask
* Arguments   : none
* Return      : 
* Note        : Alarm Task
******************************************************************************** 
*/
void AlarmTask(void)
{
	sysinfo_t *s = &_sysinfo;
	static INT8U  TankLLevelCount=0;
	static INT8U  TankHLevelCount=0;
  static INT8U  TankTempCount=0;
  static INT8U  PSupplyCount=0;
	static INT8U  FirstBootFlag=0;
	static INT8U  NOxFirstFlag = 0;
	static INT16U OneHoureCount = 0;
	static INT16U FirstNoxOnTime = 0;
  int ch;
  
	OneHoureCount++;							    //1시간 Count 값 (1시간이 되면 Time Request을 요청해라.
	if(FirstBootFlag == 0) {				  //맨처음은 30초 후에 보내라....
		if(OneHoureCount >= 30) {
			OneHoureCount = 0;
			FirstBootFlag = 1;					  // Boot 후에 30초 후에 Time Request을 보내라...
			ScrFlag.TimeRequest = 1;			//ECUProtocol.c에서 동작을 한다.(시간정보을 요청한다)
		}
	}
	else {
		if(OneHoureCount >= 3600) {
			OneHoureCount = 0;
			ScrFlag.TimeRequest = 1;			//ECUProtocol.c에서 동작을 한다.
		}
	}
	if(NOxFirstFlag == 0) {
		FirstNoxOnTime++;
//		if(FirstNoxOnTime >= 60) {      //Boot후 1분이 지나면 Nox을 On해라...
		if(FirstNoxOnTime >= 10) {      //Boot후 10초 지나면 Nox을 On해라...      
			SysSts.NoxSensorFlag[0] = ON;
			SysSts.NoxSensorFlag[1] = ON;
			if(scr_debug) printk("nox sensor on..........[%d]\n",SysSts.NoxSensorFlag[0]);
			FirstNoxOnTime = 0;
			NOxFirstFlag = 1;
		}
	}

	if(s->key.keyon_st == ON)
		SysSts.SystemSignal.KeyOn = 1;
	else
		SysSts.SystemSignal.KeyOn = 0;

  for(ch = 0; ch < 2; ch++) {
    if((ScrCalib.NOx_enable[ch] == 1) && (SysSts.SystemSignal.Noxact))
      AlarmCheckNoxn(ch);
  }
  
  if(!SysSet.LowTankLevel) {
    TankLLevelCount = 0;
		TankHLevelCount = 0;
		SysSts.SystemError.ULevel = 0;
  }
	else if(SysSts.TankLevelP < SysSet.LowTankLevel) { //TankLevel Check
		TankLLevelCount++;
		if(TankLLevelCount >= 10)
		{
			TankLLevelCount = 0;
			TankHLevelCount = 0;
			SysSts.SystemError.ULevel = 1;
		}
	}
	else if(SysSts.TankLevelP >= SysSet.LowTankLevel + 5) {
		TankHLevelCount++;
		if(TankHLevelCount >= 10)
		{
			TankLLevelCount = 0;
			TankHLevelCount = 0;
			SysSts.SystemError.ULevel = 0;
		}
	}
	else {
    TankLLevelCount = 0;
    TankHLevelCount = 0;
	}
  
  if( SysSts.TankTemp > SysSet.CutTankTemp ) {  // Tank 온도 체크
    TankTempCount++;
    if(TankTempCount >= 10) {
      TankTempCount = 0;
      SysSts.SystemError.UTemp = 0;
    } 
  }  
  else  {
    TankTempCount = 0;
    SysSts.SystemError.UTemp = 1;    
  }
  
  if( (SysSts.SystemSignal.Supply == ON) && (SysSts.P1_bar < SysSet.LowP1) ) { // Supply 압력 체크     
      PSupplyCount++;
      if(PSupplyCount >= 90) {
        PSupplyCount = 0;
        SysSts.SystemError.Supply = 1;
      }
  }   
    else {
      PSupplyCount = 0;
      SysSts.SystemError.Supply = 0;
    }
  }
/*
********************************************************************************
* Description : ScrNoxDownpersent
* Arguments   : none
* Return      : 
* Note        : SCR NOx persent calculate
******************************************************************************** 
*/
void ScrNoxDownpersent(void)
{
	static INT8U flag = 0;
	int i = 0;
	INT32U sigmaNoxinout = 0;
	INT32U sigmaNoxin	 = 0;
	INT32U Noxinout      = 0;
	INT32U TmrInterval = 0;
	double x=0.0;
  
  if(!ScrFlag.ScrFlowStart) return;
  
	TmrInterval = 1000 * 5;//5sec 주기로 제거율을 보내줘라.

	if(flag != ScrFlag.ScrFlowStart) {			// SCR 동장이 정상 가동 되었나.....확인하고 tmr interval을 설정한다.
		flag = ScrFlag.ScrFlowStart;
		TmrSetT(TMR_NOXDOWNINTERVAL, TmrInterval );
		TmrStart(TMR_NOXDOWNINTERVAL);
	}
	if(ScrFlag.ScrFlowStart) {
		if(TmrChk(TMR_NOXDOWNINTERVAL) == 0 || ScrFlag.Reduction == B_FULL) {
			ScrFlag.Reduction = B_FULL;			//설정 시간 동안 시간이 지났으면....버퍼 저장을 잠시 중단
			sigmaNoxin       = 0;
			sigmaNoxinout    = 0;
			Noxinout		     = 0;

			for(i=0; i<SysSts.reducCount; i++)
			{
				sigmaNoxin    += preduc[i].noxin;
				if(preduc[i].noxin > preduc[i].noxout)
				{
					Noxinout = preduc[i].noxin - preduc[i].noxout;
					sigmaNoxinout += Noxinout;

				}
			}
			x = (double)sigmaNoxinout/(double)sigmaNoxin;
			SysSts.NoxReduction = (INT16U)(x*100);
			if ( scr_debug )printk("reduction  = %d\n",SysSts.NoxReduction);

			ScrFlag.Reduction = B_EMPTY;
			SysSts.reducCount = 0;	//버퍼 카운트 클리어

			TmrSetT(TMR_NOXDOWNINTERVAL, TmrInterval);
			TmrStart(TMR_NOXDOWNINTERVAL);
		}
	}
}

/*
*********************************************************************************************************
*                                              DEBUG FUNCTIONS
*********************************************************************************************************
*/

/*
********************************************************************************
* Description : dbg_Factor_Save
* Arguments   : status
* Return      : 
* Note        : debug factor save
******************************************************************************** 
*/
INT16U dbg_Factor_Save(void)
{
  FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);
  printd("\n Factor Save to FRAM\n");
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_ScrCalib_Save
* Arguments   : status
* Return      : 
* Note        : debug scr calibration save
******************************************************************************** 
*/
INT16U dbg_ScrCalib_Save(void)
{
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  printd("\n Factor Save to FRAM\n");
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_Scr_ManualTest
* Arguments   : status
* Return      : 
* Note        : debug scr manual test
******************************************************************************** 
*/
INT16U dbg_Scr_ManualTest(void)
{
  if(SysSts.MDosingStart == 0) {
    SysSts.SCRMode = DOSING_MODE;
    SysSts.MDosingStart = 1;
    printd("\n SCR Manual Test Start\n");
  }
  else {
    SysSts.MDosingStart = 0;
    printd("\n SCR Manual Test Stop\n");
  }
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_Scr_DosingTest
* Arguments   : status
* Return      : 
* Note        : debug scr dosing test
******************************************************************************** 
*/
INT16U dbg_Scr_DosingTest(void)
{
  if(SysSts.SCRMode == DOSING_MODE) {
    SysSts.SCRMode = AUTO_MODE;
    SysSts.SystemSignal.Dosing = OFF;
    ScrFlag.DosingStart = 0;
    OUTPUT_DOSING(OFF);
    printd("\n SCR Dosing Test Stop\n");
  }
  else {
    SysSts.SCRMode = DOSING_MODE;
    SysSts.SystemSignal.Dosing = ON;
    ScrFlag.DosingStart = 1;
    DosingTimer = SCR_PERIOD * 10;
    printd("\n SCR Dosing Test Start\n");
  }
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_Scr_AutoMode
* Arguments   : status
* Return      : 
* Note        : debug scr auto test
******************************************************************************** 
*/
INT16U dbg_Scr_AutoMode(void)
{
  if(SysSts.SCRMode != AUTO_MODE) {
    SysSts.SCRMode = AUTO_MODE;
    printd("\n SCR Auto Mode\n");
  }
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_scrTask_Ctrol
* Arguments   : status
* Return      : 
* Note        : SCR task control
******************************************************************************** 
*/
INT16U dbg_scrTask_Ctrol(void)
{
	sysinfo_t *s = &_sysinfo 	;
	INT8U val;

	if(CLI_GetArgCnt() < 1){
		return NO_DISPLAY;
	}
	val = comparison_num((char *)P_ARG(1));
	
	s->ScrControlFlag = val;
	s->ScrDebugPrintFlag = 0;
	printk("\n Start SCR Task....\n");

	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_tSET_reset_test
* Arguments   : status
* Return      : 
* Note        : 
******************************************************************************** 
*/
INT16U dbg_tSET_reset_test(void)
{		
	memcpy((char*)&SysSet.Valid_Str[0],0x00,10);

	SPI_FLASH_LoadSave(SYSTEM_SAVE,SAVE);

	SysSet.sys_firstboot = 0x00;			

	SPI_FLASH_LoadSave(SYSTEM_SAVE,SAVE);

	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_fwver_view
* Arguments   : status
* Return      : 
* Note        : 
******************************************************************************** 
*/
INT16U dbg_fwver_view(void)
{
	printk("%s\n",source_version);
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_scr_test
* Arguments   : status
* Return      : 
* Note        : 
******************************************************************************** 
*/
INT16U dbg_scr_test(void)
{
	if ( scr_debug )
		scr_debug = 0;
	else
		scr_debug = 1;

	return NO_DISPLAY;
}



/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

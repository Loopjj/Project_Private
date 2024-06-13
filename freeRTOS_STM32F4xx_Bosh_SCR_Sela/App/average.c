/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : ADCAverage.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/22/2014
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


#define AVR_GLOBALS
#include "includes.h"

#define TANK_FORMULA_USE
#define MAF_TNUM  10
//3.3K Thermistor Output Table (-40 ~ 85)
#define TEMPCNT 126 
const INT16U TempTable[TEMPCNT] = {
      3823,3819,3815,3810,3806,3803,3800,3795,3789,3783,3776,3769,3762,3754,3746,3742,3738,3728,3719,3709,
      3698,3687,3675,3663,3649,3643,3636,3622,3607,3591,3574,3557,3539,3521,3501,3491,3481,3460,3438,3415,
      3391,3367,3342,3316,3302,3289,3261,3232,3203,3172,3141,3109,3076,3042,3025,3008,2972,2936,2900,2862,
      2824,2785,2746,2706,2686,2666,2625,2584,2542,2500,2458,2415,2372,2329,2308,2286,2243,2200,2157,2114,
      2070,2028,1985,1942,1921,1900,1858,1817,1775,1735,1694,1654,1615,1575,1556,1537,1499,1462,1425,1389,
      1354,1319,1285,1251,1235,1219,1186,1154,1123,1094,1063,1034,1006,979 ,966 ,953 ,926 ,900 ,876 ,851 ,
      827 ,804 ,782 ,760 ,750 ,740 } ; 

typedef struct {
	INT16U	buffer[100];
	INT16U	headIndex;
	INT16U	rearIndex;
	INT16U	Max;// 멕스값은 최소값으로 초기화하고 
	INT16U	Min;// 민 값은 최대값으로 초기화하면 다음측정시에 바로 업데이트 될텐데 초기화 할 곳이 마땅치 않다. 일단보류.
	INT32U	sum;
}runavr_t;

#pragma location = "EXTCCRAM"
static runavr_t runavr[AVR_BUFF_MAX]={0};

double  g_xPredict[AVR_BUFF_MAX];
double  g_x[AVR_BUFF_MAX]={0};
double  g_PPredict[AVR_BUFF_MAX];
double  g_P[AVR_BUFF_MAX];
double  g_K[AVR_BUFF_MAX];
double  g_z[AVR_BUFF_MAX];

/*
********************************************************************************
* Description : init_runavr_buff
* Arguments   : none
* Return      : none
* Note        : buffer init
******************************************************************************** 
*/
void init_runavr_buff(void)
{
	int i=0;
	for(i=0;i<AVR_BUFF_MAX;i++)
	{
		memset((char *)runavr[i].buffer,0x00,sizeof(runavr[i].buffer));
		runavr[i].headIndex = 0;
		runavr[i].rearIndex = 0;
		runavr[i].Max		= 0;
		runavr[i].Min		= 0xffff;
		runavr[i].sum		= 0;

	}
}

/*
********************************************************************************
* Description : insert_sort
* Arguments   : data, length, channel
* Return      : none
* Note        : sort
******************************************************************************** 
*/
__INLINE void insert_sort(INT16U *buff, INT16U len,int ch)
{
	INT16U i,j,t;
	runavr_t *pAVR;
	pAVR = &runavr[ch];

	for(i=0;i<len-1;i++)
	{
		for(j=1;j < len-i;j++){
			if(pAVR->buffer[j-1] > pAVR->buffer[j])
			{
				t = pAVR->buffer[j-1];
				pAVR->buffer[j-1] = pAVR->buffer[j];
				pAVR->buffer[j] = t;
			}
		}
	}
}

/*
********************************************************************************
* Description : Kalman Filter
* Arguments   : Channel, ADC Value
* Return      : correct value
* Note        : 
******************************************************************************** 
*/
double calcKalman(INT16U ch, double a_MeasuredValue)
{
  double l_z ;
  
  // Predict..
  g_xPredict[ch] = g_x[ch];
  g_PPredict[ch] = g_P[ch] + 0.001;
  g_K[ch] = g_PPredict[ch]/(g_PPredict[ch] + 10.0);
  l_z = a_MeasuredValue;
  g_x[ch] = g_xPredict[ch]+g_K[ch]*(l_z-g_xPredict[ch]);
  g_P[ch] = (1 - g_K[ch])*g_PPredict[ch];

  return g_x[ch];
}

/*
********************************************************************************
* Description : runningAverageADC
* Arguments   : channel, value
* Return      : average ADC
* Note        : average ADC value
******************************************************************************** 
*/
INT16U runningAverageADC(int ch,INT16U val)
{
	INT8U	AverageCount= 0;
	runavr_t *pAVR;
	pAVR = &runavr[ch];										// 원하는 adc체널의 포인터를 얻어온다.

	AverageCount = ADC_MAXAVERAGE_IN;

	if ( pAVR->headIndex == AverageCount )
	{
		pAVR->headIndex = 0;
		pAVR->rearIndex = AverageCount-1;
	}
	pAVR->sum -= pAVR->buffer[pAVR->headIndex];
	pAVR->buffer[pAVR->headIndex] = val;
	pAVR->sum += pAVR->buffer[pAVR->headIndex];
	pAVR->headIndex++;
	return (INT16U) (pAVR->sum/AverageCount);
	
}

/*
********************************************************************************
* Description : ext_insert_sort
* Arguments   : data, channel
* Return      : none
* Note        : sort
******************************************************************************** 
*/
__INLINE void ext_insert_sort(INT16U *buff, INT16U len,int ch)
{
	INT16U i,j,t;
	runavr_t *pAVR;
	pAVR = &runavr[ch];

	for(i=0;i<len-1;i++)
	{
		for(j=1;j < len-i;j++){
			if(pAVR->buffer[j-1] > pAVR->buffer[j])
			{
				t = pAVR->buffer[j-1];
				pAVR->buffer[j-1] = pAVR->buffer[j];
				pAVR->buffer[j] = t;
			}
		}
	}
}

/*
********************************************************************************
* Description : ext_runningAverageADC
* Arguments   : channel, value
* Return      : average ADC
* Note        : average ADC value
******************************************************************************** 
*/
INT16U ext_runningAverageADC(int ch,INT16U val)
{
	INT8U	i=0,N=0;
	INT16U	retunad=0;
	runavr_t *pAVR;
	pAVR = &runavr[ch];										// 원하는 adc체널의 포인터를 얻어온다.
	
	N = ADC_MAXAVERAGE/2;
	pAVR->buffer[pAVR->headIndex] = val;

	if ( pAVR->headIndex == ADC_MAXAVERAGE ){				// 버퍼 경계 처리 
		pAVR->headIndex = 0;								// 다시 0
		pAVR->sum = 0;
		pAVR->rearIndex = ADC_MAXAVERAGE-1;					// 이전번호 인덱스 
		ext_insert_sort(pAVR->buffer,ADC_MAXAVERAGE,ch);
		for(i=0;i < N;i++)
		{
			if(ADC_MAXAVERAGE == 20)
				pAVR->sum += pAVR->buffer[5 + i];// 큰 값을 sum한다.
			else if(ADC_MAXAVERAGE == 30)
				pAVR->sum += pAVR->buffer[7 + i];// 큰 값을 sum한다.
			else if(ADC_MAXAVERAGE == 40)
				pAVR->sum += pAVR->buffer[9 + i];// 큰 값을 sum한다.
			else if(ADC_MAXAVERAGE == 10)
				pAVR->sum += pAVR->buffer[3 + i];// 큰 값을 sum한다.
		}
		retunad = (INT16U)(pAVR->sum / N);
		pAVR->sum = retunad;
	}
	pAVR->headIndex++;										// 인덱스 증가 
	return (INT16U)pAVR->sum;
}

/*
********************************************************************************
* Description : ReadEnvTempTable
* Arguments   : none
* Return      : ECU Temp
* Note        : ECU temperature process
******************************************************************************** 
*/
INT8S ReadEnvTempTable ( void )
{
  double ecutemp;
  
  ecutemp = adcdata[ADC_AVR_TMP36_TEMP]*2.5/4096L;  // 2.5 ref, 12bit
  ecutemp = ((ecutemp-0.75)*100)+25;                // 75mV at 25
  return (INT8S)ecutemp;
  //return( (INT8S)((long)adcdata[ADC_AVR_TMP36_TEMP]*25*10/4096L - 50) );  
}


#define BAT_VOLTAGE_DIVIDER 13.25			/*회로 전압분배율 적용 */
/*
********************************************************************************
* Description : BatteryVoltage
* Arguments   : none
* Return      : value
* Note        : Battery process
******************************************************************************** 
*/
INT16U BatteryVoltage(void)
{
	INT16U volt=0;
	double X=0,V=0;
    
	X = (double)(25 * 10)/4096L;
	V = (X * (double)adcdata[ADC_AVR_BAT]) * (double)BAT_VOLTAGE_DIVIDER ;

	volt = (INT16U)V;
  
  if(volt < SysSet.BatVoltageLow)
    SysSts.SystemCheck.BatAlarm = 1;
  else 
    SysSts.SystemCheck.BatAlarm = 0;
	return volt;
}

/*
********************************************************************************
* Description : Pressure_Process
* Arguments   : channel
* Return      : bar
* Note        : 0.5V의 Reference, 4.5V의 Reference의 측정후 아래 함수에 적용,Reference의 
*               초기값은 SYS_Control_Factory_Init() Define.
******************************************************************************** 
*/
INT16U Pressure_Process(void)
{
  double bar1 = 0;
	//double value;
	double gain = 0;
	if(ScrCalib.Supply.enable)
	{
		if( adcdata[ADC_AVR_P1] < ScrCalib.Supply.AdcLow ) bar1 = 0;
		else if( adcdata[ADC_AVR_P1] > ScrCalib.Supply.AdcHigh ) bar1 = ScrCalib.Supply.AdcHigh;
		else
		{
			gain =(double)(ScrCalib.Supply.RangeHigh - ScrCalib.Supply.RangeLow)/(double)( ScrCalib.Supply.AdcHigh - ScrCalib.Supply.AdcLow);
			bar1 = (double)( ScrCalib.Supply.RangeLow + gain*(double)(adcdata[ADC_AVR_P1] - ScrCalib.Supply.AdcLow));
		}
	}
	else
	{
		bar1 = 0;
	}
	return (INT16U) bar1;
}

/*
********************************************************************************
* Description : MafvoltageOffset
* Arguments   : ADC
* Return      : Kg/h.
* Note        : Maf volatage offset
******************************************************************************** 
*/
double MafvoltageOffset(INT16U adc)
{
	double V, offset;
  
	if     (adc <= ScrCalib.CalMaf[0].AdcLevel ) {
		offset = 0.00;
    V = ((1.0 - 0.0)/((double)ScrCalib.CalMaf[0].AdcLevel - 0.0)) * ((double)adc - 0.0) + 0.0;
  }
	else if( adc > ScrCalib.CalMaf[0].AdcLevel && adc <=ScrCalib.CalMaf[1].AdcLevel ) {
		offset = (double)ScrCalib.CalMaf[0].Offset/100.0;
    V = ((2.0 - 1.0)/((double)ScrCalib.CalMaf[1].AdcLevel - (double)ScrCalib.CalMaf[0].AdcLevel)) * ((double)adc - (double)ScrCalib.CalMaf[0].AdcLevel) + 1.0;
  }
	else if( adc > ScrCalib.CalMaf[1].AdcLevel && adc <= ScrCalib.CalMaf[2].AdcLevel ) {
		offset = (double)ScrCalib.CalMaf[1].Offset/100.0;
    V = ((3.0 - 2.0)/((double)ScrCalib.CalMaf[2].AdcLevel - (double)ScrCalib.CalMaf[1].AdcLevel)) * ((double)adc - (double)ScrCalib.CalMaf[1].AdcLevel) + 2.0;    
  }
	else if( adc > ScrCalib.CalMaf[2].AdcLevel && adc <= ScrCalib.CalMaf[3].AdcLevel ) {
		offset = (double)ScrCalib.CalMaf[2].Offset/100.0;
    V = ((4.0 - 3.0)/((double)ScrCalib.CalMaf[3].AdcLevel - (double)ScrCalib.CalMaf[2].AdcLevel)) * ((double)adc - (double)ScrCalib.CalMaf[2].AdcLevel) + 3.0;       
  }
	else if( adc > ScrCalib.CalMaf[3].AdcLevel && adc <=ScrCalib.CalMaf[4].AdcLevel ) {
		offset = (double)ScrCalib.CalMaf[3].Offset/100.0;
    V = ((4.5 - 4.0)/((double)ScrCalib.CalMaf[4].AdcLevel - (double)ScrCalib.CalMaf[3].AdcLevel)) * ((double)adc - (double)ScrCalib.CalMaf[3].AdcLevel) + 4.0;           
  }
	else {
		offset = (double)ScrCalib.CalMaf[4].Offset/100.0;
    V = ((5.0 - 4.5)/(4096.0 - (double)ScrCalib.CalMaf[4].AdcLevel)) * ((double)adc - (double)ScrCalib.CalMaf[4].AdcLevel) + 4.5;               
  }

  V = (V + offset);
  
	return V;
}

/*
********************************************************************************
* Description : MassAirFlowSensor
* Arguments   : none
* Return      : Kg/h.
* Note        : mass air flow sensor 
******************************************************************************** 
*/
INT16U MassAirFlowSensor(void)
{
	INT16U adc;
	double V, Q, R;
	double  mafoffset, diameter;
  int i, idx = 0;
  
  mafoffset = SysSet.MafOffset / 1000.0;
  diameter = (double)SysSet.MafDiameter/82.0;
  
	adc = adcdata[ADC_AVR_MAF];
  
  if(adc < 200) SysSts.SystemError.MAF = 1; //약 0.3v 이하
  else SysSts.SystemError.MAF = 0;

	V = MafvoltageOffset(adc);

  if(V >= 0.5) {
    if(SysSet.ScrMafMode == 0) {
    //   Q = (30.984* pow(V, 3)) - (135.9 * pow(V,2)) + (280.11 * V) - 184.53;// 세라컴 K엔진 0.715 82pi
    //   Q = (-(12.967* pow(V, 3))) + (207.62 * pow(V,2)) - (636.13 * V) + 635.32;// 세라컴 G엔진
    //   Q = (48.546*pow(V, 3)) - (320.32*pow(V,2)) + 856.65*V - 754.94; //세라컴 F엔진_210930 1.000, 82Pi
    //   Q = (317.6 * pow(V,5)) - (4619.3 * pow(V,4)) + (26575 * pow(V,3)) - (75548 * pow(V,2)) + (106308 * V) - 59270; //세라컴 F엔진 1.000, 82Pi
    //   Q = (229.97 * pow(V,5)) - (3288.5 * pow(V,4)) + (18712 * pow(V,3)) - (52629 * pow(V,2)) + (73370 * V) - 40503; // 세라컴 대형 0.820 153pi
    //   Q = (32.132 * pow(V,3)) - (50.17 * pow(V,2)) + (14.391 * V) + 88.925; // 세라컴 대형 0.820 211012 GS
    //   Q = (152.4 * pow(V,3)) - (1044.1 * pow(V,2)) + (2708.6 * V) - 2302.4; // 세라컴 대형 0.820 211012 GS2
       Q = pow(diameter, 2) * ((0.4119 * pow(V,5)) - (0.8825 * pow(V,4)) + (7.6538 * pow(V,3)) - (0.9055 * pow(V,2)) + (24.154 * V) - 26.849);
    }
    else {
      for(i=0; i < MAF_TNUM; i++) {
        if(adc < ScrMaf.Adc[i]) {
          idx = i; break;
        }
      }
      if(i == MAF_TNUM) idx = MAF_TNUM;
      
      if(idx < 1) 
        Q = (ScrMaf.Value[idx]*adc)/(ScrMaf.Adc[idx]);
      else {
        if(idx == MAF_TNUM)
          Q = ScrMaf.Value[MAF_TNUM-1]+(ScrMaf.Value[MAF_TNUM-1]+500-ScrMaf.Value[MAF_TNUM-1])*(adc-ScrMaf.Adc[MAF_TNUM-1])/(4096-ScrMaf.Adc[MAF_TNUM-1]);
        else   
          Q = ScrMaf.Value[idx-1]+(ScrMaf.Value[idx]-ScrMaf.Value[idx-1])*(adc-ScrMaf.Adc[idx-1])/(ScrMaf.Adc[idx]-ScrMaf.Adc[idx-1]);
      }
    }
  }
  else Q = 0;

  if(SysSet.ScrMafMode == 0)
    R = Q * mafoffset;
  else 
    R = Q;
  
	return (INT16U)R;
}

/*
********************************************************************************
* Description : MassAirFlowSensor Temp
* Arguments   : none
* Return      : Kg/h.
* Note        : mass air flow sensor 
******************************************************************************** 
*/
INT16S MassAirFlowSensor_Temp(void)
{
	INT16U adc;
	double V, T;

	adc = adcdata[ADC_AVR_MAF_TEMP];

	V = ((2.5 /4096.0) * (double)adc) + 0.25;

  if(V < 2.379)  // -40 이하는 버림
    T = (14.569 * pow(V,4)) - (102.93 * pow(V,3)) + (244.77 * pow(V,2)) - (276.71 * V) + 155.04;
  else T = -40.0;
  
	return (INT16S)T;
}

/*
********************************************************************************
* Description : Tank Temperature Check
* Arguments   : none
* Return      : Tank Temperature.
* Note        :
******************************************************************************** 
*/
//#define TANK_TEMP_OFFSET  2.5
#define TANK_TEMP_OFFSET  0.80
INT16S TankTempCheck(void)
{
#ifdef TANK_FORMULA_USE
    INT16S tankTemp;
    double rthm, vo , t = 0;
    
    vo = (double)adcdata[ADC_AVR_UREAT]*2.5/4096;
    rthm  = (3.046 * vo) / (1.0 - (0.3046 * vo));
    if(rthm  > 0)
      t =  (1.0/((1.0 / 298.0) + ((1.0 / 3977.0) * log(rthm / 3.3))) - 273.0)*TANK_TEMP_OFFSET;
    tankTemp = (INT16S)t;
#else
  int i;
  INT16S tankTemp = -40;
  double adcval;
  adcval = (double)adcdata[ADC_AVR_UREAT] * TANK_TEMP_OFFSET;
  
  for(i = 0; i < TEMPCNT; i++) {
    if((INT16U)adcval > TempTable[i]) break;
  }
   tankTemp += i;
#endif
  
  return tankTemp;
}

/*
********************************************************************************
* Description : Tank Level Check
* Arguments   : none
* Return      : TankLevel.
* Note        : 
******************************************************************************** 
*/
#define TankAdcHigh     2365
#define TankAdcLow      4095
#define TankRangeHigh   100
#define TankRangeLow    0
INT16S TankLevelCheck(void)
{
  double tank = 0;
  
#ifdef TANK_FORMULA_USE
  double r, vo;
  
  vo = (double)adcdata[ADC_AVR_UREAL]*2.5/4096;
  if(SysSet.TankType == 1) {
//    r = (0.4569 * vo) / (1.0-(0.3046 * vo));        // 10L
//    r = ((-0.1026 * pow(20,vo)) + 105.15);        // 10L
    r = ((-38 * pow(vo,5)) + (209 * pow(vo,4)) - (459.8 * pow(vo,3)) + (505.78 * pow(vo,2)) - (278.18 * vo) + 161.2);        // 10L
    if(r < 0) 
      tank = 0;
    else if(r > 100)
      tank = 100;
    else
      tank = r;
  }
  else {
    r = ((vo * 24.9) / 3.28299) / (1.0 - (vo / 3.28299));  // 35L
    if(r < 0) 
      tank = 0;
    else 
      tank = (140.73 - r) / 1.157;    
  }
#else  
  double gain = 0;
  
  if( adcdata[ADC_AVR_UREAL] > TankAdcLow ) tank = 0;
  else {
    gain =(double)(TankRangeHigh - TankRangeLow)/(double)( TankAdcHigh - TankAdcLow);
    tank = (double)(TankRangeLow + gain * (double)(adcdata[ADC_AVR_UREAL] - TankAdcLow));
  }
#endif
	return (INT16S) tank; 
}

/*
********************************************************************************
* Description : Temp Sensor(PT200)
* Arguments   : channel
* Return      : temperature.
* Note        : DOC폴더에 온도센서(PT200)_Table.xlsx 참고할것
******************************************************************************** 
*/
INT16S TempSensorPt200(INT8U ch)
{
  double temp[2]={0, 0};

	double gain = 0;
	if(ScrCalib.Temp[ch].enable)
	{
		if( adcdata[ADC_AVR_T1 + ch] < ScrCalib.Temp[ch].AdcLow ) temp[ch] = 0;
		//else if( adcdata[ADC_AVR_T1 +ch] > ScrCalib.TemperatureAdcHigh[ch] ) temp[ch] = ScrCalib.TemperatureRangeHigh[ch];
		else {
			gain =(double)(ScrCalib.Temp[ch].RangeHigh - ScrCalib.Temp[ch].RangeLow)/(double)(ScrCalib.Temp[ch].AdcHigh - ScrCalib.Temp[ch].AdcLow); 
			temp[ch] = (double)( ScrCalib.Temp[ch].RangeLow + gain*(double)( adcdata[ADC_AVR_T1 + ch] - ScrCalib.Temp[ch].AdcLow) );
		}
    if(temp[ch] > ScrCalib.Temp[ch].RangeHigh) {
      if(!ch) {
       SysSts.SystemError.T1 = 1;
       temp[ch] = 0.0;
      }
      else {
       SysSts.SystemError.T2 = 1;
       temp[ch] = 0.0;
      }
    }
    else {
      if(!ch)
        SysSts.SystemError.T1 = 0;
      else
        SysSts.SystemError.T2 = 0;
    }
	}
	else
	{
		temp[ch] = 0;
    if(!ch)
      SysSts.SystemError.T1 = 0;
    else
      SysSts.SystemError.T2 = 0;    
	}

	return (INT16U) temp[ch]; 
}

/*
********************************************************************************
* Description : ACD Task Process
* Arguments   : p_arg.
* Return      : none.
* Note        : ADC
******************************************************************************** 
*/
void Task_ADC(void *p_arg)
{
	sysinfo_t *s = (sysinfo_t *)p_arg 	;
	init_runavr_buff();
	BSP_ADC1ConStart();
		
	for(;;)
	{
		s->twatchAdcFlag = 1;
#if 1
		adcdata[ADC_AVR_MAF]	= runningAverageADC(ADC_MAF,adcdata[ADC_MAF]);
		adcdata[ADC_AVR_AMB_TEMP]		= runningAverageADC(ADC_AMB_TEMP,adcdata[ADC_AMB_TEMP]);
		adcdata[ADC_AVR_T1]		= runningAverageADC(ADC_T1,adcdata[ADC_T1]);
		adcdata[ADC_AVR_T2]		= runningAverageADC(ADC_T2,adcdata[ADC_T2]);
		adcdata[ADC_AVR_MAF_TEMP]		= runningAverageADC(ADC_MAF_TEMP,adcdata[ADC_MAF_TEMP]);
		adcdata[ADC_AVR_P1]		= runningAverageADC(ADC_P1,adcdata[ADC_P1]);
		adcdata[ADC_AVR_UREAT]	= runningAverageADC(ADC_UREAT,adcdata[ADC_UREAT]);
		adcdata[ADC_AVR_UREAL]	= runningAverageADC(ADC_UREAL,adcdata[ADC_UREAL]);
		adcdata[ADC_AVR_BAT]	= runningAverageADC(ADC_BAT,adcdata[ADC_BAT]);
		adcdata[ADC_AVR_TMP36_TEMP] = runningAverageADC(ADC_TMP36_TEMP,adcdata[ADC_TMP36_TEMP]);
#else
		adcdata[ADC_AVR_MAF]	= ext_runningAverageADC(ADC_MAF,adcdata[ADC_MAF]);
		adcdata[ADC_AVR_AMB_TEMP]		= ext_runningAverageADC(ADC_AMB_TEMP,adcdata[ADC_AMB_TEMP]);
		adcdata[ADC_AVR_T1]		= ext_runningAverageADC(ADC_T1,adcdata[ADC_T1]);
		adcdata[ADC_AVR_T2]		= ext_runningAverageADC(ADC_T2,adcdata[ADC_T2]);
		adcdata[ADC_AVR_MAF_TEMP]		= ext_runningAverageADC(ADC_MAF_TEMP,adcdata[ADC_MAF_TEMP]);
		adcdata[ADC_AVR_P1]		= ext_runningAverageADC(ADC_P1,adcdata[ADC_P1]);
		adcdata[ADC_AVR_UREAT]	= ext_runningAverageADC(ADC_UREAT,adcdata[ADC_UREAT]);
		adcdata[ADC_AVR_UREAL]	= ext_runningAverageADC(ADC_UREAL,adcdata[ADC_UREAL]);
		adcdata[ADC_AVR_BAT]	= ext_runningAverageADC(ADC_BAT,adcdata[ADC_BAT]);
		adcdata[ADC_AVR_TMP36_TEMP] = ext_runningAverageADC(ADC_TMP36_TEMP,adcdata[ADC_TMP36_TEMP]);
#endif

		vTaskDelay(1);
		s->twatchAdcFlag = 0;
	}
}

#if 0
INT16U pkAverageADC(int ch, INT16U val)
{
	INT8U	i=0,N=0;
	INT16U	retunad=0;
	int dl_ul=0;
	static INT16U MaxADCV[2]={0,0};
	runavr_t *pAVR;
	pAVR = &runavr[ch];										// 원하는 adc체널의 포인터를 얻어온다.
	

	if( ch == AVR_PKIN_DL ) dl_ul = 0;
	else					dl_ul = 1;

	N = 4;
	pAVR->buffer[pAVR->headIndex] = val;

	if ( pAVR->headIndex == ADC_MAXAVERAGE_PK ){				// 200버퍼 경계 처리 
		pAVR->headIndex = 0;								// 다시 0
		pAVR->sum = 0;
		pAVR->rearIndex = ADC_MAXAVERAGE_PK-1;					// 이전번호 인덱스 
		retunad = MaxADCV[dl_ul];
		pAVR->sum = retunad;
	}
	//if(pAVR->headIndex == 0)	MaxADCV[dl_ul] = val;
	if(pAVR->headIndex == 0)	MaxADCV[dl_ul] = (val + MaxADCV[dl_ul])/2;
	else
		if( MaxADCV[dl_ul] < val) MaxADCV[dl_ul] = val;
	pAVR->headIndex++;										// 인덱스 증가 

	return (INT16U)pAVR->sum;

}

INT16U dbg_inputADCCbuffer_test(void)
{
	int dl_ul, val;
	runavr_t *pAVR;
	pAVR = &runavr[AVR_PKIN_DL];	

	if(CLI_GetArgCnt() < 1) return INV_PRM;
	val = comparison_num((char *)P_ARG(1));
	if(val)
	{
		memset((char *)runavr[AVR_PKIN_DL].buffer, 0x00, sizeof(runavr[AVR_PKIN_DL].buffer));
		pAVR->headIndex = 0;
		flagsampling = val;
	}
	else
		printk("input adc debug flag = %d\n",flagsampling);
		
	return NO_DISPLAY;
}

INT16U dbg_buffer_test(void)
{
	int dl_ul, i;
	runavr_t *pAVR;
	pAVR = &runavr[AVR_PKIN_DL];	

	//if(CLI_GetArgCnt() < 1) return INV_PRM;
	//dl_ul = comparison_num((char *)P_ARG(1));

	for(i=0;i<ADC_MAXAVERAGE_PK;i++)
	{
		printk("[%03d] = %d\n",i,pAVR->buffer[i]);
	}
			
	return NO_DISPLAY;
}
#endif

/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : avg.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : ADC average
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

typedef struct {
	INT16U	buffer[100];
	INT16U	headIndex;
	INT16U	rearIndex;
	INT16U	Max;// 멕스값은 최소값으로 초기화하고 
	INT16U	Min;// 민 값은 최대값으로 초기화하면 다음측정시에 바로 업데이트 될텐데 초기화 할 곳이 마땅치 않다. 일단보류.
	INT32U	sum;
}runavr_t;

static runavr_t runavr[AVR_BUFF_MAX]={0};


/*
********************************************************************************
* Description : init_runavr_buff
* Arguments   : none
* Return      : 
* Note        : digital output test
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
* Arguments   : *buff, length, channel
* Return      : 
* Note        : 
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
* Description : runningAverageADC
* Arguments   : channel, val
* Return      : 
* Note        : 
******************************************************************************** 
*/

INT16U runningAverageADC(int ch,INT16U val)
{
	INT8U	AverageCount= 0;
	runavr_t *pAVR;
	pAVR = &runavr[ch];

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
* Arguments   : *buff, length, channel
* Return      : 
* Note        : 
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
* Arguments   : channel
* Return      : 
* Note        : 
******************************************************************************** 
*/

INT16U ext_runningAverageADC(int ch,INT16U val)
{
	INT8U	i=0,N=0;
	INT16U	retunad=0;
	runavr_t *pAVR;
	pAVR = &runavr[ch];										          // 원하는 adc체널의 포인터를 얻어온다.
	
	N = ADC_MAXAVERAGE/2;
	pAVR->buffer[pAVR->headIndex] = val;

	if ( pAVR->headIndex == ADC_MAXAVERAGE ){				// 버퍼 경계 처리 
		pAVR->headIndex = 0;								          // 다시 0
		pAVR->sum = 0;
		pAVR->rearIndex = ADC_MAXAVERAGE-1;					  // 이전번호 인덱스 
		ext_insert_sort(pAVR->buffer,ADC_MAXAVERAGE,ch);
		for(i=0;i < N;i++)
		{
			if(ADC_MAXAVERAGE == 20)
				pAVR->sum += pAVR->buffer[5 + i];         // 큰 값을 sum한다.
			else if(ADC_MAXAVERAGE == 30)
				pAVR->sum += pAVR->buffer[7 + i];         // 큰 값을 sum한다.
			else if(ADC_MAXAVERAGE == 40)
				pAVR->sum += pAVR->buffer[9 + i];         // 큰 값을 sum한다.
			else if(ADC_MAXAVERAGE == 10)
				pAVR->sum += pAVR->buffer[3 + i];         // 큰 값을 sum한다.
		}
		retunad = (INT16U)(pAVR->sum / N);
		pAVR->sum = retunad;
	}
	pAVR->headIndex++;										          // 인덱스 증가 
	return (INT16U)pAVR->sum;
}


/*
********************************************************************************
* Description : Pressure_Process
* Arguments   : channel
* Return      : 
* Note        : 0.5V의 Reference, 4.5V의 Reference의 측정후 아래 함수에 적용,
*             : Reference의 초기값은 SYS_Control_Factory_Init() Define.
******************************************************************************** 
*/

INT16U Pressure_Process(void)
{
  double bar = 0;
	double gain = 0;

  adcdata[ADC_AVR_P0] = runningAverageADC(ADC_P0, adcdata[ADC_P0]);
  if( adcdata[ADC_AVR_P0] < Setting.P1CalLow) bar = 0;
  else if( adcdata[ADC_AVR_P0] > Setting.P1CalHigh) bar = Setting.Prangehigh;
  else {
    gain =(double)(Setting.Prangehigh - Setting.Prangelow )/(double)(Setting.P1CalHigh - Setting.P1CalLow); 
    bar = (double)(Setting.Prangelow + gain*(double)(adcdata[ADC_AVR_P0] - Setting.P1CalLow));
  }
	return (INT16U)bar;
}


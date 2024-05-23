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
	INT16U	Max;// �߽����� �ּҰ����� �ʱ�ȭ�ϰ� 
	INT16U	Min;// �� ���� �ִ밪���� �ʱ�ȭ�ϸ� ���������ÿ� �ٷ� ������Ʈ ���ٵ� �ʱ�ȭ �� ���� ����ġ �ʴ�. �ϴܺ���.
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
	pAVR = &runavr[ch];										          // ���ϴ� adcü���� �����͸� ���´�.
	
	N = ADC_MAXAVERAGE/2;
	pAVR->buffer[pAVR->headIndex] = val;

	if ( pAVR->headIndex == ADC_MAXAVERAGE ){				// ���� ��� ó�� 
		pAVR->headIndex = 0;								          // �ٽ� 0
		pAVR->sum = 0;
		pAVR->rearIndex = ADC_MAXAVERAGE-1;					  // ������ȣ �ε��� 
		ext_insert_sort(pAVR->buffer,ADC_MAXAVERAGE,ch);
		for(i=0;i < N;i++)
		{
			if(ADC_MAXAVERAGE == 20)
				pAVR->sum += pAVR->buffer[5 + i];         // ū ���� sum�Ѵ�.
			else if(ADC_MAXAVERAGE == 30)
				pAVR->sum += pAVR->buffer[7 + i];         // ū ���� sum�Ѵ�.
			else if(ADC_MAXAVERAGE == 40)
				pAVR->sum += pAVR->buffer[9 + i];         // ū ���� sum�Ѵ�.
			else if(ADC_MAXAVERAGE == 10)
				pAVR->sum += pAVR->buffer[3 + i];         // ū ���� sum�Ѵ�.
		}
		retunad = (INT16U)(pAVR->sum / N);
		pAVR->sum = retunad;
	}
	pAVR->headIndex++;										          // �ε��� ���� 
	return (INT16U)pAVR->sum;
}


/*
********************************************************************************
* Description : Pressure_Process
* Arguments   : channel
* Return      : 
* Note        : 0.5V�� Reference, 4.5V�� Reference�� ������ �Ʒ� �Լ��� ����,
*             : Reference�� �ʱⰪ�� SYS_Control_Factory_Init() Define.
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


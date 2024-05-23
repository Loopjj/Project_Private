/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : adc.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/07/2017
*
* Description        : adc 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define ADC_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint16_t ADC1ConvertedValue; 

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*
********************************************************************************
* Description : MX_ADC1_Init
* Arguments   : none
* Return      : none
* Note        : ADC1 init function
******************************************************************************** 
*/
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /* Start the conversion process */  
  if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC1ConvertedValue, 1) != HAL_OK) //start the DMA collecting the data  
  {
    /* Start Conversation Error */
    Error_Handler();
  }
}

/*
********************************************************************************
* Description : MX_DMA_Init
* Arguments   : none
* Return      : none
* Note        : Enable DMA controller clock
******************************************************************************** 
*/
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
}

/*
********************************************************************************
* Description : BatteryVoltage
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
#define BAT_VOLTAGE_DIVIDER 15.0			/*회로 전압분배율 적용 */

uint16_t BatteryVoltage(void)
{
	uint16_t volt=0;
	double X=0,V=0;
    
	X = (double)25/4096L;
	V = (X * (double)ADC1ConvertedValue) * (double)BAT_VOLTAGE_DIVIDER ;

	volt = (INT16U)V;
	return volt;
}


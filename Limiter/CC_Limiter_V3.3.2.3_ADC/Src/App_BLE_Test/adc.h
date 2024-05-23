/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : adc.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/07/2017
*
* Description        : adc
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __ADC_H
#define __ADC_H

#ifdef ADC_GLOBALS
#define ADC_EXT
#else
#define ADC_EXT	extern
#endif 

ADC_EXT ADC_HandleTypeDef hadc1;
ADC_EXT DMA_HandleTypeDef hdma_adc1;

ADC_EXT void MX_ADC1_Init(void);
ADC_EXT void MX_DMA_Init(void);
ADC_EXT uint16_t MX_ADC_Read(void) ;
ADC_EXT uint16_t BatteryVoltage(void);
#endif


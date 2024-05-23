/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : timer.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : timer configure
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __TIMER_H
#define __TIMER_H

#ifdef TIM_GLOBALS
#define TIM_EXT
#else
#define TIM_EXT	extern
#endif 
TIM_EXT TIM_HandleTypeDef htim2;
TIM_EXT TIM_HandleTypeDef htim3;
TIM_EXT TIM_HandleTypeDef htim4;

TIM_EXT void MX_TIM2_Init(void);
TIM_EXT void MX_TIM3_Init(void);
TIM_EXT void MX_TIM4_Init(void);
TIM_EXT void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);
#endif
/*
*************************************** (C) COPYRIGHT 2017,2018 LOOP *************************************
*
* File Name          : delay.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 01/09/2018
*
* Description        : delay header 
*
*********************************************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H__
#define __DELAY_H__

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void delay_us(uint32_t nCount);
void delay_ms(uint32_t nCount);
void delay_nop(uint32_t nCount);

#endif
/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/
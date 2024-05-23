/*
*************************************** (C) COPYRIGHT 2017,2018 LOOP *************************************
*
* File Name          : delay.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 01/09/2018
*
* Description        : delay routine 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
	
/**
  * @brief  Microsecond delay 
  * @param  Microsecond conunt
  * @retval None
  */  	
void delay_nop(uint32_t nCount)
{
//    nCount *= 5; //cortex m3
//  nCount *= 15; //cortex m4
  for(; nCount!=0; nCount--) {
//      asm(" NOP ");          
  }
}

/**
  * @brief  Microsecond delay 
  * @param  Microsecond conunt
  * @retval None
  */  	
void delay_us(uint32_t nCount)
{
//    nCount *= 5; //cortex m3
  nCount *= 15; //cortex m4
  for(; nCount!=0; nCount--) {
//      asm(" NOP ");          
  }
}
	
/**
  * @brief  msec delay 
  * @param  msec conunt
  * @retval None
  */  	
void delay_ms(uint32_t nCount)
{
  for(; nCount!=0; nCount--) delay_us(1000);
}


/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/
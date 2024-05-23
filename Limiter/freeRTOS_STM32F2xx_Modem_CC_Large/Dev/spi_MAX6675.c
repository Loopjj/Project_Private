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

#define SPI_MAX6675_GLOBALS
#include "includes.h"

#define BIT_RESOLUTION  0.25
#define BIT_CONNECTION  0x04

/*
********************************************************************************
* Description : Read_max6675
* Arguments   : none
* Return      : 
* Note        : read max6675 
******************************************************************************** 
*/

INT16U Read_max6675(void) 
{ 
  INT16U activeValue = 0;
  double realValue = 0;
  INT8U highData = 0, lowData = 0;
  
  GPIO_ResetBits(GPIOB, GPIO_Pin_12);         // CS low
  asm("NOP");
  /* Wait for SPI2 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);   //wait finish sending
  SPI_I2S_SendData(SPI2, 0);
  /* Wait for SPI2 data reception */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) ;   // wait data received
  highData = SPI_I2S_ReceiveData(SPI2);
  
  /* Wait for SPI2 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);   //wait finish sending
  SPI_I2S_SendData(SPI2, 0);
  /* Wait for SPI2 data reception */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) ;   // wait data received
  lowData = SPI_I2S_ReceiveData(SPI2);
  asm("NOP");
  GPIO_SetBits(GPIOB, GPIO_Pin_12);           // CS high

  if((lowData & BIT_CONNECTION) != BIT_CONNECTION) {
    activeValue = (highData << 8); 
    activeValue |= lowData;
    realValue = ((double)(activeValue >> 3) * BIT_RESOLUTION);
    TerrorFlag = 1;
  } 
  else {
    realValue = 0;
    TerrorFlag = 0;
  }
  realValue -= Setting.T1Offset;
  if(realValue < 0) realValue = 0;
  return (INT16U)realValue;
} 




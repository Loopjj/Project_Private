/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : uart.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/05/2017
*
* Description        : uart
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __UART_H
#define __UART_H

#ifdef UART_GLOBALS
#define UART_EXT
#else
#define UART_EXT	extern
#endif 

/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/
UART_EXT uint8_t RxUSART1, RxUSART2, RxUSART3, RxUART4, RxUSART5, RxUSART6;

UART_EXT UART_HandleTypeDef huart1;
UART_EXT UART_HandleTypeDef huart2;
UART_EXT UART_HandleTypeDef huart3;
UART_EXT UART_HandleTypeDef huart4;
UART_EXT UART_HandleTypeDef huart5;
UART_EXT UART_HandleTypeDef huart6;

UART_EXT u8 DVR_Send_Flag;

UART_EXT void MX_USART1_UART_Init(void);
UART_EXT void MX_USART2_UART_Init(void);
UART_EXT void MX_USART3_UART_Init(void);
UART_EXT void MX_UART4_Init(void);
UART_EXT void MX_UART5_Init(void);
UART_EXT void MX_USART6_UART_Init(void);

UART_EXT void Uart_RXDEnble(void);
UART_EXT void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
UART_EXT void UART_SendData(UART_HandleTypeDef *huart, uint8_t data);
UART_EXT int UART_SendDataStr(UART_HandleTypeDef *huart, uint8_t *data, int len);

UART_EXT void Task_COM(void const * argument);
#endif
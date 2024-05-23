/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : uart.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/05/2017
*
* Description        : uart 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define UART_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
********************************************************************************
* Description : MX_USART1_UART_Init
* Arguments   : none
* Return      : 
* Note        : USART1 init function
******************************************************************************** 
*/
void MX_USART1_UART_Init(void)    //BLE Device
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/*
********************************************************************************
* Description : MX_USART2_UART_Init
* Arguments   : none
* Return      : 
* Note        : USART2 init function
******************************************************************************** 
*/
void MX_USART2_UART_Init(void)   //Temp Controller, DVR, ADAS
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/*
********************************************************************************
* Description : MX_USART3_UART_Init
* Arguments   : none
* Return      : 
* Note        : USART3 init function
******************************************************************************** 
*/
void MX_USART3_UART_Init(void)  //Ext Modem, MDT, JIG , PC GUI
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/*
********************************************************************************
* Description : MX_UART4_Init
* Arguments   : none
* Return      : 
* Note        : UART4 init function
******************************************************************************** 
*/
void MX_UART4_Init(void)    //For GPS 
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/*
********************************************************************************
* Description : MX_UART5_Init
* Arguments   : none
* Return      : 
* Note        : UART5 init function
******************************************************************************** 
*/
void MX_UART5_Init(void)  //CLI, AUX
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  	    
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/*
********************************************************************************
* Description : MX_USART6_UART_Init
* Arguments   : none
* Return      : 
* Note        : USART6 init function
******************************************************************************** 
*/
void MX_USART6_UART_Init(void)   // For ADAS protocol
{

  huart6.Instance = USART6;
#ifdef LIMITER_TEST
  huart6.Init.BaudRate = 9600;
#else
  huart6.Init.BaudRate = 115200;
#endif
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/*
********************************************************************************
* Description : RXDEnble
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
void Uart_RXDEnble(void)
{
  if(HAL_UART_Receive_IT(&huart1, &RxUSART1, 1) != HAL_OK)
  {
    Error_Handler(); 
  }  
  if(HAL_UART_Receive_IT(&huart2, &RxUSART2, 1) != HAL_OK)
  {
    Error_Handler(); 
  }
  if(HAL_UART_Receive_IT(&huart3, &RxUSART3, 1) != HAL_OK)
  {
    Error_Handler(); 
  }
  if(HAL_UART_Receive_IT(&huart4, &RxUART4, 1) != HAL_OK)
  {
    Error_Handler(); 
  }
  if(HAL_UART_Receive_IT(&huart5, &RxUSART5, 1) != HAL_OK)
  {
    Error_Handler(); 
  }
  if(HAL_UART_Receive_IT(&huart6, &RxUSART6, 1) != HAL_OK)
  {
    Error_Handler(); 
  }

}


/*
********************************************************************************
* Description : UART_SendData
* Arguments   : handlie , data
* Return      : 
* Note        : Send data
******************************************************************************** 
*/
void UART_SendData(UART_HandleTypeDef *huart, uint8_t data)
{
	uint8_t buf[2];
  
	buf[0] = data;
	buf[1] = '\0';

  if (HAL_UART_Transmit(huart, (uint8_t *)buf, 1, 300)!= HAL_OK)
  {
    // NOTE: !! IMPORTANT!! Not sent message
    Error_Handler();
  }
}

/*
********************************************************************************
* Description : UART_SendDataStr
* Arguments   : handlie , data, len
* Return      : 
* Note        : Send data
******************************************************************************** 
*/
int UART_SendDataStr(UART_HandleTypeDef *huart, uint8_t *data, int len)
{
  u32 timeout;
  
  timeout = len * 5;
  if(timeout < 300) timeout = 300;
  if (HAL_UART_Transmit(huart, (uint8_t *)data, len, timeout)!= HAL_OK)
  {
    // NOTE: !! IMPORTANT!! Not sent message
    return 0;
  }
  else
  {
    // Successful sending on USART
    return 1;
  }  
}


/*
********************************************************************************
* Description : Task_COM
* Arguments   : argument
* Return      : 
* Note        : Task_COM function
******************************************************************************** 
*/
void Task_COM(void const * argument)
{
  /* USER CODE BEGIN Task_COM */

  /* Infinite loop */
  for(;;)
  {
    if(!Power_Off_Mode) {  
      if(Start_Flag > 1) {
      }

    }
    else {
    }
    osDelay(1);
  }
  /* USER CODE END Task_COM */
}

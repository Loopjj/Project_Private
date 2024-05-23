/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : bsp_int.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : bsp intterupt 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define BSP_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*
********************************************************************************
* Description : HAL_RTCEx_WakeUpTimerEventCallback
* Arguments   : RTC_HandleTypeDef *hrtc
* Return      : None
* Note        : RTC_HandleTypeDef *hrtc
******************************************************************************** 
*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{

  BLE_IntFlag = 1;  
  DTG_IntFlag = 1;
  USB_IntFlag = 1;  
  if(Start_Flag) {
    RTC_GetDateTime();   
    if(!Power_Off_Flag && !(FotaRespFlag && LoopFotaFlag == 2) && !(FotaRespFlag && FotaEnable)) // && !FotaRespFlag)
      DrvDataUpdate(); 
    if(!IsSameDay(RTC_LVal+1, DailyData.Time)) 
      Day_Change_Flag = 1;
    Start_Flag = 2;
  }
  
  if(Setting.FW_Type == EXT_MODEM) {
    Modem_SecIntProcess();
  }
  CheckCanDisable();
}

/*
********************************************************************************
* Description : HAL_UART_RxCpltCallback
* Arguments   : UART_HandleTypeDef *huart
* Return      : 
* Note        : 
******************************************************************************** 
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	INT16U uart4_head, uart5_head;;

  if (huart->Instance == USART1)
  {
    HAL_UART_Receive_IT(&huart1, BLE_RxBuf, BLE_RX_BUFFER_SIZE);
  }
  
  if (huart->Instance == USART2)
  {
//#ifdef USE_UART2_ADAS    
//    RxDataADAS(RxUSART2);     
//#else
//    UT1_parse_proc(RxUSART2);
//#endif    
    HAL_UART_Receive_IT(&huart2, &RxUSART2, 1);
  }

  if (huart->Instance == USART3)
  {
    //if(Setting.FW_Type == EXT_MODEM) 테스트 
      //ModemCommPutRxChar(RxUSART3);
//    else if(Setting.FW_Type == MDT_S1 || Setting.FW_Type == MDT_LG) 
//      MDT_RxData_S1(RxUSART3);    
//    else if(Setting.FW_Type == WIFI_LOOP) 
//      WifiRxHeaderParse(RxUSART3);
#ifdef LIMITER_TEST
    DecodeRxData(RxUSART3); 
#endif
    HAL_UART_Receive_IT(&huart3, &RxUSART3, 1);
  }

  if (huart->Instance == UART4)
  {
    // Calculate buffer index
    uart4_head = (GPS_RxHead + 1) & GPS_RX_BUFFER_MASK;
    GPS_RxHead = uart4_head;      // Store new index

    if (uart4_head != GPS_RxTail )
      GPS_RxBuf[uart4_head] = RxUART4; // Store received data in buffer
    
    HAL_UART_Receive_IT(&huart4, &RxUART4, 1);
  }

  if (huart->Instance == UART5)
  {
    // Calculate buffer index
    uart5_head = (CLI_RxHead + 1) & CLI_RX_BUFFER_MASK;
    CLI_RxHead = uart5_head;      // Store new index

    if (uart5_head != CLI_RxTail )
      CLI_RxBuf[uart5_head] = RxUSART5; // Store received data in buffer 
  
    HAL_UART_Receive_IT(&huart5, &RxUSART5, 1);
  }
#ifdef LIMITER_TEST
  if (huart->Instance == USART6)
#else
  if (huart->Instance == USART3)
#endif
  {

//    RxDataADAS(RxUSART6);
#ifdef LIMITER_TEST
    DecodeRxData(RxUSART6);
    HAL_UART_Receive_IT(&huart6, &RxUSART6, 1);  //테스트 
#else
    DecodeRxData(RxUSART3);    
    HAL_UART_Receive_IT(&huart3, &RxUSART3, 1);  //테스트 
#endif
  }  
}

/*
********************************************************************************
* Description : Tx Transfer completed callback
* Arguments   : RTC_HandleTypeDef *hrtc
* Return      : None
* Note        : This example shows a simple way to report end of IT Tx transfer, and 
                you can add your own implementation.
******************************************************************************** 
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Set transmission flag: trasfer complete*/
}

/*
********************************************************************************
* Description : UART error callbacks
* Arguments   : UartHandle: UART handle
* Return      : None
* Note        : This example shows a simple way to report transfer error, and you can
                add your own implementation.
******************************************************************************** 
*/
 void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
      __HAL_UART_CLEAR_OREFLAG(huart);
      __HAL_UART_CLEAR_NEFLAG(huart);
      __HAL_UART_CLEAR_FEFLAG(huart);
}

/*
********************************************************************************
* Description : Period elapsed callback in non blocking mode
* Arguments   : TIM handle
* Return      : None
* Note        : This function is called  when TIM1 interrupt took place, inside
                HAL_TIM_IRQHandler().
******************************************************************************** 
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  
  if (htim->Instance == TIM2) {
    Signal_Process();
  }  
           
  if (htim->Instance == TIM3) {
//
  }  
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/*
********************************************************************************
* Description : EXTI line detection callbacks
* Arguments   : GPIO_Pin: Specifies the pins connected EXTI line
* Return      : None
* Note        : 
******************************************************************************** 
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	sysinfo_t *s = &_sysinfo 	;
  
  if(GPIO_Pin == SPEED_Pin)
  {
		s->pSpeed.pulse_flag = 1;

  }
  
  if(GPIO_Pin == RPM_Pin)
  {
		s->pRpm.pulse_flag = 1;
  }
}

/*
********************************************************************************
* Description : Transmission complete callback in non blocking mode 
* Arguments   : CanHandle: pointer to a CAN_HandleTypeDef structure that contains
*               the configuration information for the specified CAN.
* Return      : None
* Note        : 
******************************************************************************** 
*/

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
  
  if(hcan->pRxMsg->IDE == CAN_ID_STD)
    CAN_Std_Decode(hcan);      
  else 
    CAN_Ext_Decode(hcan);    

  /* Receive */
  if(HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    CAN_ErrorFlag = 1;
    if(CAN_ErrorCnt > 3)
      Error_Handler();
  }
  else {
    CAN_ErrorFlag = 0;
    CAN_ErrorCnt = 0;
  }
}


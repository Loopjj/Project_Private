/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define USB_PWR_CNTL_Pin GPIO_PIN_2
#define USB_PWR_CNTL_GPIO_Port GPIOE
#define CDMA_PWR_CNTL_Pin GPIO_PIN_3
#define CDMA_PWR_CNTL_GPIO_Port GPIOE
#define GPS_PWR_CNTL_Pin GPIO_PIN_4
#define GPS_PWR_CNTL_GPIO_Port GPIOE
#define POWER_ENABLE_Pin GPIO_PIN_5
#define POWER_ENABLE_GPIO_Port GPIOE
#define LCD_SCL_Pin GPIO_PIN_0
#define LCD_SCL_GPIO_Port GPIOE
#define LCD_SDA_Pin GPIO_PIN_1
#define LCD_SDA_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_2
#define LCD_RST_GPIO_Port GPIOF
#define LCD_A0_Pin GPIO_PIN_3
#define LCD_A0_GPIO_Port GPIOF
#define ADC_BAT_Pin GPIO_PIN_0
#define ADC_BAT_GPIO_Port GPIOA
#define ADC_AUX_Pin GPIO_PIN_4
#define ADC_AUX_GPIO_Port GPIOC
#define U4_GPS_RX_Pin GPIO_PIN_1
#define U4_GPS_RX_GPIO_Port GPIOA
#define U2_SENSOR_TX_Pin GPIO_PIN_2
#define U2_SENSOR_TX_GPIO_Port GPIOA
#define U2_SENSOR_RX_Pin GPIO_PIN_3
#define U2_SENSOR_RX_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define U3_CDMA_TX_Pin GPIO_PIN_10
#define U3_CDMA_TX_GPIO_Port GPIOB
#define U3_CDMA_RX_Pin GPIO_PIN_11
#define U3_CDMA_RX_GPIO_Port GPIOB
#define BACKLIGHT_Pin GPIO_PIN_13
#define BACKLIGHT_GPIO_Port GPIOD
#define DI_1_Pin GPIO_PIN_2
#define DI_1_GPIO_Port GPIOG
#define DI_2_Pin GPIO_PIN_3
#define DI_2_GPIO_Port GPIOG
#define SPEED_Pin GPIO_PIN_4
#define SPEED_GPIO_Port GPIOG
#define RPM_Pin GPIO_PIN_5
#define RPM_GPIO_Port GPIOG
#define BRAKE_Pin GPIO_PIN_6
#define BRAKE_GPIO_Port GPIOG
#define LED_ALIVE_Pin GPIO_PIN_7
#define LED_ALIVE_GPIO_Port GPIOG
#define BLE_RST_Pin GPIO_PIN_8
#define BLE_RST_GPIO_Port GPIOG    
#define U6_ADAS_TX_Pin GPIO_PIN_6
#define U6_ADAS_TX_GPIO_Port GPIOC
#define U6_ADAS_RX_Pin GPIO_PIN_7
#define U6_ADAS_RX_GPIO_Port GPIOC
#define G_INT_Pin GPIO_PIN_8
#define G_INT_GPIO_Port GPIOC
#define G_SDA_Pin GPIO_PIN_9
#define G_SDA_GPIO_Port GPIOC
#define G_SCL_Pin GPIO_PIN_8
#define G_SCL_GPIO_Port GPIOA
#define U4_GPS_TX_Pin GPIO_PIN_10
#define U4_GPS_TX_GPIO_Port GPIOC
#define U5_DVR_TX_Pin GPIO_PIN_12
#define U5_DVR_TX_GPIO_Port GPIOC
#define U5_DVR_TXD2_Pin GPIO_PIN_2
#define U5_DVR_TXD2_GPIO_Port GPIOD
#define KEY_1_Pin GPIO_PIN_11
#define KEY_1_GPIO_Port GPIOG
#define KEY_2_Pin GPIO_PIN_12
#define KEY_2_GPIO_Port GPIOG
#define KEY_3_Pin GPIO_PIN_13
#define KEY_3_GPIO_Port GPIOG
#define KEY_4_Pin GPIO_PIN_14
#define KEY_4_GPIO_Port GPIOG
#define DI_KEY_Pin GPIO_PIN_15
#define DI_KEY_GPIO_Port GPIOG
#define DO1_Pin GPIO_PIN_5
#define DO1_GPIO_Port GPIOB
#define DO2_Pin GPIO_PIN_6
#define DO2_GPIO_Port GPIOE
#define EEPROM_SDA_Pin GPIO_PIN_0
#define EEPROM_SDA_GPIO_Port GPIOF
#define EEPROM_SCL_Pin GPIO_PIN_1
#define EEPROM_SCL_GPIO_Port GPIOF
#define HW_WATCHDOG_Pin GPIO_PIN_10
#define HW_WATCHDOG_GPIO_Port GPIOF

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);
void IWDG_ReloadCounter(void);
void CAN_Configuration(void);
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

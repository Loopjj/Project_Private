/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : bsp.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 01/01/2014
*
* Description        : board configuration
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef  BSP_H
#define  BSP_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_GLOBAL
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/


#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "core_cm4.h"

#include  <bsp.h>
//#include  <usb_core.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "includes.h"

/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/
#define WATCHDOG_ENABLE  	1
#define PDV_ENABLE				0
#define RTC_ENABLE				1
/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/


#if PDV_ENABLE > 0
BSP_EXT void PVD_IRQHandler(void);
#endif
BSP_EXT void RTC_IRQHandler(void);

BSP_EXT void I2C1_EV_IRQHandler(void);
BSP_EXT void I2C1_ER_IRQHandler(void);

BSP_EXT void I2C2_EV_IRQHandler(void);
BSP_EXT void I2C2_ER_IRQHandler(void);

BSP_EXT void CAN1_RX0_IRQHandler(void);

BSP_EXT void CAN2_RX0_IRQHandler(void);

BSP_EXT void TIM1_CC_IRQHandler(void);
BSP_EXT void TIM2_IRQHandler(void);
BSP_EXT void TIM3_IRQHandler(void);
BSP_EXT void TIM5_IRQHandler(void);

BSP_EXT void EXTI3_IRQHandler(void);
BSP_EXT void EXTI15_10_IRQHandler(void);
BSP_EXT void EXTI9_5_IRQHandler(void);

BSP_EXT void ADC_IRQHandler(void);

BSP_EXT void DMA2_Stream4_IRQHandler(void);

BSP_EXT void USART1_IRQHandler(void);

BSP_EXT void USART2_IRQHandler(void);

BSP_EXT void USART3_IRQHandler(void);

BSP_EXT void UART4_IRQHandler(void);

BSP_EXT void OTG_FS_IRQHandler(void);
BSP_EXT void OTG_HS_IRQHandler(void);
BSP_EXT void ADC_IRQHandler(void);




/*
*********************************************************************************************************
*                                         SEMAPHORE CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/



#define USART_TX_DMA_ENABLE		0
#define USART_RX_DMA_ENABLE		0


/*
*********************************************************************************************************
*                                             CONSTANTS
*********************************************************************************************************
*/
#define KEYON_SIG()       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define POWER_SIG()       GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)

#define KEY_ENTER()       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)
#define KEY_MODE()        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)
#define KEY_UP()          GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define KEY_RESET()       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
  
#define MMC_DETECT()      GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)

#define DATA_LED(val)		  GPIO_WriteBit(GPIOE,  GPIO_Pin_11, val ? Bit_RESET : Bit_SET)
#define NORMAL_LED(val)	  GPIO_WriteBit(GPIOE,  GPIO_Pin_12, val ? Bit_RESET : Bit_SET)
#define TEMP_LED(val)		  GPIO_WriteBit(GPIOE,  GPIO_Pin_13, val ? Bit_RESET : Bit_SET)
#define HPRESS_LED(val)	  GPIO_WriteBit(GPIOE,  GPIO_Pin_14, val ? Bit_RESET : Bit_SET)
#define LPRESS_LED(val)	  GPIO_WriteBit(GPIOE,  GPIO_Pin_15, val ? Bit_RESET : Bit_SET)

#define CHARGE(val)	      GPIO_WriteBit(GPIOA,  GPIO_Pin_1, val ? Bit_SET : Bit_RESET)
#define BUZZER(val)	      GPIO_WriteBit(GPIOB,  GPIO_Pin_0, val ? Bit_SET : Bit_RESET)
#define DOUTPUT(val)		  GPIO_WriteBit(GPIOB,  GPIO_Pin_1, val ? Bit_SET : Bit_RESET)


#define KeyOn_Check()     { if(POWER_SIG()) SecData.Sig.KeyOn = 1; else SecData.Sig.KeyOn = 0;}

/*
*********************************************************************************************************
*                                              TYPE DEFINE
*********************************************************************************************************
*/

typedef enum {
//--------------------- DMA 스캔 순서에 맞출 것 
	ADC_P0 = 0,
	ADC_AVR_P0,
	ADC_READ_MAX
}adc_num_t;


BSP_EXT INT16U adcdata[ADC_READ_MAX];
/*
*********************************************************************************************************
*                                            INLINE FUNCTIONS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/



BSP_EXT void BSP_Lowlevel_Init(void);
BSP_EXT void BSP_CAN_Init(void);
//BSP_EXT void _Delay(INT32U nCount);
BSP_EXT void system_power_off(void);
BSP_EXT void StartPwm(void);
BSP_EXT void EndPwm(void);

/*------- api 함수 -------*/
BSP_EXT void BSP_ADC1ConStart(void);
BSP_EXT int BSP_GetTempSensor(void);
BSP_EXT void BSP_Amp_Control(int dl_ul, int onoff);
BSP_EXT void BSP_PLL_Control(int dl_ul, int onoff);

BSP_EXT void BSP_LED_Toggle(INT8U led);
BSP_EXT void BSP_USART_ReInit(void);		

BSP_EXT void BSP_UART1RX_IntEnable(FunctionalState state);
BSP_EXT void BSP_UART2RX_IntEnable(FunctionalState state);

/*-------- dbg function --------*/
BSP_EXT INT16U dbg_system_reset(void);
BSP_EXT INT16U dbg_tasklist(void);
BSP_EXT INT16U dbg_ver_rollback(void);
BSP_EXT INT16U dbg_ver_rollup(void);


#define	BSP_COMMANDS	\
	{"reset",   dbg_system_reset, ALL_DIR, "reset"},\
	{"rolback", dbg_ver_rollback, ALL_DIR, "rolback"},\
	{"rolup",   dbg_ver_rollup,   ALL_DIR, "최종 펌웨어 로드"},\
	{"tlist",   dbg_tasklist, ALL_DIR,     "tlist"},\

#define RTC_COMMANDS \
	{"rset",    dbg_rtc_set,  ALL_DIR,  "rset [yy mm dd hh mm ss wd] rtc set"},\
	{"rget",    dbg_rtc_gettime, ALL_DIR, "rget rtc read"},\
    
#endif                                                         
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

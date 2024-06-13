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

#define    UUID_DEVICE           (const BYTE*)0x1FFF7A10
/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/
#define WATCHDOG_ENABLE  	1
#define PDV_ENABLE				0
#define RTC_ENABLE				0
#define PWM_ENABLE				1
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

BSP_EXT void ADC_IRQHandler(void);

BSP_EXT void DMA2_Stream4_IRQHandler(void);

BSP_EXT void USART1_IRQHandler(void);

BSP_EXT void USART2_IRQHandler(void);

BSP_EXT void USART3_IRQHandler(void);

BSP_EXT void UART4_IRQHandler(void);

BSP_EXT void OTG_FS_IRQHandler(void);
//BSP_EXT void OTG_HS_IRQHandler(void);
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
#define KEYON_SIG()         GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3)

#define INPUTDI0()		      GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)
#define INPUTDI1()		      GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9)

#define POWER_ENABLE()	    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);	                  //POWER ENABLE
#define POWER_DISABLE()	    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);	                //POWER DISABLE
  
#define OUTPUT_DO(val)		  GPIO_WriteBit(GPIOG,  GPIO_Pin_8, val ? Bit_SET : Bit_RESET)  //Supply Heater
#define OUTPUT_POWER(val)	  GPIO_WriteBit(GPIOG,  GPIO_Pin_7, val ? Bit_SET : Bit_RESET)  //Supply Power
#define OUTPUT_MOTOR(val)	  GPIO_WriteBit(GPIOG,  GPIO_Pin_6, val ? Bit_SET : Bit_RESET)  //Supply Motor
#define OUTPUT_SOL(val)		  GPIO_WriteBit(GPIOG,  GPIO_Pin_5, val ? Bit_SET : Bit_RESET)  //Supply Bypass Sol
#define OUTPUT_DOSING(val)  GPIO_WriteBit(GPIOG,  GPIO_Pin_4, val ? Bit_SET : Bit_RESET)  //Dosing
#define PWM_IOC(val)	      GPIO_WriteBit(GPIOA,  GPIO_Pin_1, val ? Bit_SET : Bit_RESET)  

#define CAN_OBD_STBY(val)	  GPIO_WriteBit(GPIOG,  GPIO_Pin_2, val ? Bit_SET : Bit_RESET)
#define CAN_ECU_STBY(val)	  GPIO_WriteBit(GPIOC,  GPIO_Pin_6, val ? Bit_SET : Bit_RESET)

#define USART1_DR_Base		  (u32)(&(((USART_TypeDef *)USART1)->DR));
#define USART2_DR_Base		  (u32)(&(((USART_TypeDef *)USART2)->DR));
#define USART3_DR_Base		  (u32)(&(((USART_TypeDef *)USART3)->DR));


#define DMATX_BUFSIZE       512
#define DMARX_BUFSIZE       1



/*
*********************************************************************************************************
*                                              TYPE DEFINE
*********************************************************************************************************
*/

typedef enum {
//--------------------- DMA 스캔 순서에 맞출 것 
	ADC_MAF=0,	
	ADC_AMB_TEMP,
	ADC_T1,
	ADC_T2,
	ADC_MAF_TEMP,
	ADC_P1,
	ADC_UREAT,
	ADC_UREAL,
	ADC_BAT,
	ADC_TMP36_TEMP,

	ADC_AVR_MAF,  //10	
	ADC_AVR_AMB_TEMP,
	ADC_AVR_T1,
	ADC_AVR_T2,
	ADC_AVR_MAF_TEMP,
	ADC_AVR_P1,
	ADC_AVR_UREAT,
	ADC_AVR_UREAL,
	ADC_AVR_BAT,
	ADC_AVR_TMP36_TEMP,

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
BSP_EXT void _Delay(INT32U nCount);
BSP_EXT void system_power_off(void);
BSP_EXT void StartPwm(void);
BSP_EXT void EndPwm(void);
BSP_EXT void StartDosing(void);
BSP_EXT void EndDosing(void);

BSP_EXT void BSP_LED_Toggle(INT8U led);
//BSP_EXT void Speed_Update(u32 Speed);

/*------- api 함수 -------*/
BSP_EXT void BSP_ADC1ConStart(void);
BSP_EXT int BSP_GetTempSensor(void);
BSP_EXT void BSP_Amp_Control(int dl_ul, int onoff);
BSP_EXT void BSP_PLL_Control(int dl_ul, int onoff);

BSP_EXT void BSP_LED_Toggle(INT8U led);
BSP_EXT void BSP_USART_ReInit(void);		

BSP_EXT void BSP_UART2RX_IntEnable(FunctionalState state);
BSP_EXT void BSP_UART4RX_IntEnable(FunctionalState state);

BSP_EXT  int CAN_Receive_mk(CAN_TypeDef* CANx, uint8_t FIFONumber, uint8_t* ptarget);
BSP_EXT  void PWM_SetInject(u8 rate);
//BSP_EXT unsigned int GetRtcTime(void);
//BSP_EXT void RTC_TimeRegulate(struct tm *tm_ptr);


/*-------- dbg function --------*/
BSP_EXT INT16U dbg_system_reset(void);
BSP_EXT INT16U dbg_tasklist(void);
BSP_EXT INT16U dbg_ver_rollback(void);
BSP_EXT INT16U dbg_ver_rollup(void);
BSP_EXT INT16U dbg_supply_Ctrol(void);
BSP_EXT INT16U dbg_line_Ctrol(void);
BSP_EXT INT16U dbg_motor_Ctrol(void);
BSP_EXT INT16U dbg_sol_Ctrol(void);
BSP_EXT INT16U dbg_tank_Ctrol(void);
BSP_EXT INT16U dbg_inputpin_Read(void);


#define	BSP_COMMANDS	\
	{"reset",   dbg_system_reset, ALL_DIR, "reset"},\
	{"rolback", dbg_ver_rollback, ALL_DIR, "rolback"},\
	{"rolup",   dbg_ver_rollup,   ALL_DIR, "최종 펌웨어 로드"},\
	{"tlist",   dbg_tasklist, ALL_DIR,     "tlist"},\
	{"sypply",  dbg_supply_Ctrol, ALL_DIR, "supply heater 0"},\
	{"linec",   dbg_line_Ctrol, ALL_DIR,   "linec 0"},\
	{"motorc",  dbg_motor_Ctrol, ALL_DIR,  "motorc 0"},\
	{"solc",    dbg_sol_Ctrol, ALL_DIR,    "solc 0"},\
	{"tankc",   dbg_tank_Ctrol, ALL_DIR,   "tankc 0"},\
	{"di",      dbg_inputpin_Read, ALL_DIR,"di input pin status"},\

#endif                                                         
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

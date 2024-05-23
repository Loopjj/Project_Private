/*
*************************************** (C) COPYRIGHT 2015 Loop *************************************
*
* File Name          : fnd.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 04/05/2015
*
* Description        : fnd display
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef FND_H
#define FND_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef FND_GLOBALS
#define FND_EXT
#else
#define FND_EXT	extern
#endif 


/*
*********************************************************************************************************
*                                               FND DEFINE
*********************************************************************************************************
*/
#define IO_SET_BIT(gpio, pin, state)	GPIO_WriteBit(gpio, pin, state)

#define FNDCS_1_GPIOn		GPIOD
#define FNDCS_1_PIN			GPIO_Pin_13

#define FNDCS_2_GPIOn		GPIOD
#define FNDCS_2_PIN			GPIO_Pin_3

#define FNDCS_3_GPIOn		GPIOD
#define FNDCS_3_PIN			GPIO_Pin_8

#define FNDCS_4_GPIOn		GPIOD
#define FNDCS_4_PIN			GPIO_Pin_9

#define FNDCS_5_GPIOn		GPIOD
#define FNDCS_5_PIN			GPIO_Pin_10



#define FND_CS( order, state )	        IO_SET_BIT( FNDCS_##order##_GPIOn, FNDCS_##order##_PIN, state ? Bit_SET : Bit_RESET )


#define FNDA_GPIOn			GPIOC
#define FNDA_PIN			  GPIO_Pin_1

#define FNDB_GPIOn			GPIOC
#define FNDB_PIN			  GPIO_Pin_2

#define FNDC_GPIOn			GPIOC
#define FNDC_PIN			  GPIO_Pin_3

#define FNDD_GPIOn			GPIOC
#define FNDD_PIN			  GPIO_Pin_4

#define FNDE_GPIOn			GPIOC
#define FNDE_PIN			  GPIO_Pin_5

#define FNDF_GPIOn			GPIOC
#define FNDF_PIN			  GPIO_Pin_6

#define FNDG_GPIOn			GPIOC
#define FNDG_PIN			  GPIO_Pin_7

#define FNDP_GPIOn			GPIOC
#define FNDP_PIN			  GPIO_Pin_13


#define FND_SET( order, state )	        IO_SET_BIT( FND##order##_GPIOn, FND##order##_PIN, state ? Bit_SET : Bit_RESET )


/*
*********************************************************************************************************
*                                               LED DEFINE
*********************************************************************************************************
*/
#define LED_SD_GPIOn		GPIOE
#define LED_SD_PIN			GPIO_Pin_11		//LED SD Card status

#define LED_NO_GPIOn		GPIOE
#define LED_NO_PIN			GPIO_Pin_12		//LED Normal

#define LED_TE_GPIOn		GPIOE
#define LED_TE_PIN			GPIO_Pin_13		//LED Temp

#define LED_PL_GPIOn		GPIOE
#define LED_PL_PIN			GPIO_Pin_14		//LED Press Low

#define LED_PH_GPIOn		GPIOE
#define LED_PH_PIN			GPIO_Pin_15		//LED Press High


#define LED_SET( order, state )	        IO_SET_BIT( LED_##order##_GPIOn, LED_##order##_PIN, state ? Bit_SET : Bit_RESET )


/*
*********************************************************************************************************
*                                               I/O DEFINE(BUZZER, DO_CTL)
*********************************************************************************************************
*/
#define IO_BU_GPIOn			GPIOB
#define IO_BU_PIN			  GPIO_Pin_0		//BUZZER

#define IO_DO_GPIOn			GPIOB
#define IO_DO_PIN			  GPIO_Pin_1		//DO Control : I/O


#define IO_SET( order, state )	        IO_SET_BIT( IO_##order##_GPIOn, IO_##order##_PIN, state ? Bit_SET : Bit_RESET )
/*
*********************************************************************************************************
*                                           GLOBAL FUNCTION
*********************************************************************************************************
*/


FND_EXT INT8U FND_DATA[5];
FND_EXT void SetFnd(INT8U cs);

FND_EXT void fnd_callback_fun(void);
FND_EXT INT8U InitFirstLedFndCheck(void);

FND_EXT INT16U dbg_buzzer_test(void);
FND_EXT INT16U dbg_doc_test(void);

FND_EXT void Disp2(void);

#define	BSP_IOCCOMMAND	\
{"buzzer",  dbg_buzzer_test,  ALL_DIR, "buzzer [ctrl] 0: off 1: on"},\
	{"doc",		dbg_doc_test,     ALL_DIR, "doc ctrl] 0: off 1: on"},\


#endif 
/*
********************* (C) COPYRIGHT 2015 Loop ***************END OF FILE*****************************
*/

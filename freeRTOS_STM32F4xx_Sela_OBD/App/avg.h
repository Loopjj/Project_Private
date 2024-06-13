/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : adc.c
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
*                                                MODULE
*********************************************************************************************************
*/

#ifndef AVR_H
#define AVR_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef AVR_GLOBALS
#define AVR_EXT
#else
#define AVR_EXT	extern
#endif 

#define AVR_BUFF_MAX		        2
#define ADC_MAXAVERAGE_IN	      20
#define ADC_MAXAVERAGE		      20
#define ADC_MAXAVERAGE_PK	      100
#define ADC_MAXLOOP			        10	/* ADC_MAXAVERAGE * ADC_MAXLOOP */
#define ADC_MAXAVERAGE_INPUT    80

#define SET_BOOT_SYS				0x01

AVR_EXT int flagsampling;

AVR_EXT INT16U MassAirFlowSensor(void);
AVR_EXT INT16S TempSensorPt200(INT8U ch);

AVR_EXT INT8S ReadEnvTempTable ( void );
AVR_EXT void init_runavr_buff(void);
AVR_EXT INT16U runningAverageADC(int ch,INT16U val);
AVR_EXT INT16U ext_runningAverageADC(int ch,INT16U val);
AVR_EXT INT16U Pressure_Process(void);

AVR_EXT void Task_ADC(void *p_arg);


#endif

/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/

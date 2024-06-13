/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : ADCAverage.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/22/2014
*
* Description        :   
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

#define AVR_BUFF_MAX		      11
#define ADC_MAXAVERAGE_IN	    100
#define ADC_MAXAVERAGE		    20
#define ADC_MAXAVERAGE_PK	    100
#define ADC_MAXLOOP			      10	/* ADC_MAXAVERAGE * ADC_MAXLOOP */
#define ADC_MAXAVERAGE_INPUT  80

AVR_EXT int flagsampling;


AVR_EXT INT16U MassAirFlowSensor(void);
AVR_EXT INT16S MassAirFlowSensor_Temp(void);
AVR_EXT INT16S TempSensorPt200(INT8U ch);
AVR_EXT INT16S TankLevelCheck(void);
AVR_EXT INT16S TankTempCheck(void);
AVR_EXT INT8S ReadEnvTempTable (void );
AVR_EXT void init_runavr_buff(void);
AVR_EXT INT16U runningAverageADC(int ch,INT16U val);
AVR_EXT INT16U ext_runningAverageADC(int ch,INT16U val);
AVR_EXT INT16U Pressure_Process(void);
AVR_EXT double MafvoltageOffset(INT16U adc);
AVR_EXT double calcKalman(INT16U ch, double a_MeasuredValue);
AVR_EXT INT16U BatteryVoltage(void);
AVR_EXT void Task_ADC(void *p_arg);


#endif
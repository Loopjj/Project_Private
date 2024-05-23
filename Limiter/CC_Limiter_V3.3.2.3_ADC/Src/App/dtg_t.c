/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : dtg.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : dtg process 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define DTG_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*
********************************************************************************
* Description : Drv_Data_Update
* Arguments   : none
* Return      : 
* Note        : Event Data Update
******************************************************************************** 
*/
char IntFlag = 0;
void Drv_Data_Update(void)
{ 
	sysinfo_t *s = &_sysinfo 	;
  u32 speed = 0, rpm = 0;
//  u8 brake = 0;
//  double fuel_rate = 0, lpk = 0;
  
  if(s->pSpeed.pulse_period > 10000 && s->pSpeed.pulse_count > 0)
    speed = (u32)(((double)s->pSpeed.pulse_count-1)/((double)s->pSpeed.pulse_period*0.00005)*3600.0/6000.0+0.5);
  else speed = 0;
  s->pSpeed.pulse_count = 0;

  if(s->pRpm.pulse_period > 10000 && s->pRpm.pulse_count > 0)
   rpm   = (u32)(((double)s->pRpm.pulse_count-1)/((double)s->pRpm.pulse_period*0.00005)*60.0/(300/100.0)+0.5);
  else rpm = 0;
  s->pRpm.pulse_count = 0; 

  if(speed > 255) speed = 255;
  if(rpm < 100) rpm = 0;
  else if(rpm > 9999) rpm = 9999;
  
  IntFlag = 1;
//  SecData.Speed[0] = (u8)speed;
//  SecData.RPM = (u16)rpm;
 
}

/*
********************************************************************************
* Description : Signal Proess
* Arguments   : none
* Return      : 
* Note        : RPM, Speed
******************************************************************************** 
*/
void Signal_Process(void)  
{ 
	sysinfo_t *s = &_sysinfo 	;

  s->pSpeed.nloop_count++;
  s->pRpm.nloop_count++;

  if(s->pSpeed.pulse_flag)
  {
    if( SPEED_SIG() ) 
      s->pSpeed.pulse_flag = 0;   //speed low 
    else
    {
      s->pSpeed.pulse_flag++;
      if(s->pSpeed.pulse_flag > SPEED_SIG_TIMING )
      {
        s->pSpeed.pulse_count++;
        s->pSpeed.pulse_flag = 0;
        if(s->pSpeed.pulse_count == 1)
        {
          s->pSpeed.nloop_count = 0;
          s->pSpeed.pulse_period = 0;
        }
        s->pSpeed.pulse_period = s->pSpeed.nloop_count;
      }
    }
  }
  
  if(s->pRpm.pulse_flag)
  {
    if( RPM_SIG() ) 
    {
      s->pRpm.pulse_flag++;
      if(s->pRpm.pulse_flag > RPM_SIG_TIMING )
      {
        s->pRpm.pulse_count++;
        s->pRpm.pulse_flag = 0;
        if(s->pRpm.pulse_count == 1)
        {
          s->pRpm.nloop_count = 0;
          s->pRpm.pulse_period = 0;
        }
        s->pRpm.pulse_period = s->pRpm.nloop_count;
      }
    }
    else
      s->pRpm.pulse_flag = 0;     //rpm low 
  }    
}

u8 ValueCnt = 0, ResetCnt;
/* Task_DTG function */
void Task_DTG(void const * argument)
{
  /* USER CODE BEGIN Task_DTG */
  I2C1_ACC_Init();  
  /* Infinite loop */
  for(;;)
  {
    I2C1_ACC_Read();
    SecData.VatVolt = BatteryVoltage();    
    
    if(IntFlag) {
      SecData.Speed[0] = ValueCnt;
      SecData.RPM = ValueCnt;
      ValueCnt++;
      ValueCnt %= 255;
      //LCD Reset
      ResetCnt++;
      if(ResetCnt > 5) {
        lcd_init();
        ResetCnt = 0;
      }
      IntFlag = 0;
    }
    osDelay(1);
  }
  /* USER CODE END Task_DTG */
}
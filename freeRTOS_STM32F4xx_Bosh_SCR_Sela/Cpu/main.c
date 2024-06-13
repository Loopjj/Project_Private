/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @version V1.1.0
  * @date    07/10/2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "includes.h"
#define TANK_CAN_SUPPORT

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private semaphore ---------------------------------------------------------*/
xSemaphoreHandle SpiFlashDevSem = NULL;
xSemaphoreHandle BSP_SaveSem	  = NULL;
xSemaphoreHandle BSP_I2C1Sem	  = NULL;
xSemaphoreHandle BSP_NANDSem	  = NULL;
xSemaphoreHandle BSP_BKSramSem	= NULL;
xSemaphoreHandle BSP_BKRtcSem	  = NULL;
xSemaphoreHandle BSP_Can2Trap   = NULL;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int errno;
/* Private function prototypes -----------------------------------------------*/
//void Event_Proc(void * pvParameters);
static void prvTimerCallback(xTimerHandle xTimer)				;
static void prvwatchdogCallback(xTimerHandle xTimer)			;
static void prvTimerCallbackSpeed(xTimerHandle xTimer)			;
static void prvTimerCallbacksupply(xTimerHandle xTimer)			;
static void prvTimerCallbacksupply_Period(xTimerHandle xTimer)	;

xTimerHandle xTimer01				= NULL;
xTimerHandle xTimer02			  = NULL;
xTimerHandle xTimer03		    = NULL;
xTimerHandle xTimer04			  = NULL;
xTimerHandle xTimer05	      = NULL;

struct PID_DATA pidData;

/*
********************************************************************************
* Description : Throttle Body PID Control
******************************************************************************** 
*/
void InitPID(void)
{
	pid_Init(1, 0, 4, &pidData);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

 int main(void)
{
	INT32U nDelay=0;
	for(nDelay=0;nDelay <0x2ffffff; nDelay++);

 	RCC_ClocksTypeDef rcc_clocks;
    
	BSP_Lowlevel_Init();

	RCC_GetClocksFreq(&rcc_clocks);

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_Config( rcc_clocks.HCLK_Frequency / 100 ); 

	CommInit();
	sys_struct_init();
	CLI_SetConsolPort(DBG_PORT);

	DisplayLogo();
  
	/* Semaphore init */
	flashSemaphore_Init();
	framSemaphore_Init();
	nvxSemaphore_init();
	BSRAMSemaphore_Init();

#if SCR_USE_NAND > 0
	NandSemaphore_Init();
	FSMC_NAND_Reset();
	Nand_Make_Badblock_Table();
	Nand_Read_BadblockData();
#endif

	/*SPI Flash data load*/
	SYS_Control_Init();
  InitPID();
  
#if WATCHDOG_ENABLE > 0
	IWDG_ReloadCounter();  
#endif

	/* Task Init */
	xTaskCreate( Task_CLI, 	(signed portCHAR*)CLITASK_NAME,	configMINIMAL_STACK_SIZE*3,	(void *)&_sysinfo,	CLI_TASK_PRIO,	NULL);
	xTaskCreate( TmrTask, 	(signed portCHAR*)TMRTASK_NAME,	configMINIMAL_STACK_SIZE*2,	(void *)&_sysinfo,	TMR_TASK_PRIO,	NULL);
	xTaskCreate( Task_Gui, 	(signed portCHAR*)GUITASK_NAME,	configMINIMAL_STACK_SIZE*2,	(void *)&_sysinfo,	LMS_TASK_PRIO,	NULL);
	xTaskCreate( Task_Can1, (signed portCHAR*)OBDTASK_NAME,	configMINIMAL_STACK_SIZE*2,	(void *)&_sysinfo,	OBD_TASK_PRIO,	NULL);
	xTaskCreate( Task_Can2, (signed portCHAR*)ECUTASK_NAME,	configMINIMAL_STACK_SIZE*2, (void *)&_sysinfo,	ECU_TASK_PRIO,	NULL);
	xTaskCreate( Task_ADC,  (signed portCHAR*)ADCTASK_NAME,	configMINIMAL_STACK_SIZE,   (void *)&_sysinfo,	ADC_TASK_PRIO,	NULL);
	xTaskCreate( Task_Scr,  (signed portCHAR*)SCRTASK_NAME,	configMINIMAL_STACK_SIZE*4, (void *)&_sysinfo,	SCR_TASK_PRIO,	NULL);
	
	/* Timer Calback */
	xTimer03     = xTimerCreate( ( const signed char * ) "watch", ( 100 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvwatchdogCallback);	
	if(xTimer03 != NULL) xTimerStart(xTimer03,0);
	xTimer01     = xTimerCreate( ( const signed char * ) "disled",( 5 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvTimerCallback);//100
	if(xTimer01 != NULL) xTimerStart(xTimer01,0);
	xTimer02     = xTimerCreate( ( const signed char * ) "Speed", ( 1000 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvTimerCallbackSpeed);
	if(xTimer02 != NULL) xTimerStart(xTimer02,0);
	xTimer04     = xTimerCreate( ( const signed char * ) "supply",( 1 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvTimerCallbacksupply);	
	if(xTimer04 != NULL) xTimerStart(xTimer04,0);
	xTimer05     = xTimerCreate( ( const signed char * ) "period",( 2 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvTimerCallbacksupply_Period);	
	if(xTimer05 != NULL) xTimerStart(xTimer05,0);

	/* Start scheduler */
	vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
    for( ;; )
	{
		vTaskDelay(2000);
	}
}

/* 
** 1mSec 간격으로 Callback Function				        *
** 1mSec 간격으로 Dosing Nozzle을 제어한다.				  *
** 100mSec, 10Hz 기준으로 되어 있음.                *
** Pump을 1회 제어후 제어해야할 Period을 리턴한다.      *
*/
// Variables for PID
int16_t referenceValue, measurementValue;
double inputValue, preDuty;
struct PID_DATA pidData;
static void prvTimerCallbacksupply(xTimerHandle xTimer)
{
  double tmpDuty;
  
  if(SysSts.SystemSignal.Supply == ON) {
    if(!SysSts.supply.Pwm_Count) {          // 500ms ?
      if(SysSts.P1_bar >= SysSet.LowP1 && !ScrFlag.Flushing) {   // 7bar 크냐?
        if(!SysSts.SystemSignal.Dosing && (!ScrFlag.ModuleT1Cut || SysSts.MDosingStart)) {// && !SysSts.ScrFlag.MAFCut) {
          DosingTimer = SCR_PERIOD * 10;
          StartDosing();
          ScrFlag.DosingStart = 1;
          SysSts.SystemSignal.Dosing = ON;
        }
      }
       // PID 제어를 통한 듀티 제어
      referenceValue = 9000;
      measurementValue = 	Pressure_Process();	
      inputValue = pid_Controller(referenceValue, measurementValue, &pidData);
      
      tmpDuty = preDuty + inputValue;
      
      if(tmpDuty < 5) SysSts.supply.Duty_Cycle = 5;
      else if(tmpDuty > 45) SysSts.supply.Duty_Cycle = 45;
      else SysSts.supply.Duty_Cycle = (INT16U)tmpDuty;
      
      preDuty = SysSts.supply.Duty_Cycle;
      
      if(ScrFlag.SupplyStart == 1) {
        SupplyPower(ON);  
        MotorCtrl(ON);
        if(ScrFlag.Flushing) {  // Flushing
          FlushingCount++;
          if((FlushingCount > SysSet.ScrPurgeTime) || (SysSts.P1_bar >= SysSet.LowP1)) {
            ScrFlag.SupplyStart = 0;
            ScrFlag.Flushing = 0;
            FlushingCount = 0;
            PurgeFlag = 1;
          }
          else {
            if(PurgeSolFlag) {
              SolPressureCtrl(ON);
              PWM_SetInject(50);  // 향후 판단
              PurgeFlag = 0;
            }
          }
        }         
      }
      else {
        SCR_Control_Off();
      }
    }
    SysSts.supply.Pwm_Count++;
    SysSts.supply.Pwm_Count %= SCR_PERIOD;
  }
}


/* 
** 30mSec 간격으로 Callback Function				          *
** 알고리즘 500msec 주기는 16번 제어 가능,                 *
** 1000msec 주기는 33번 제어 가능.				                *
** argument : 1m Callback 함수에서 리턴된 Period를 받고   *
** 딜레이 만큰 유지 후 Supply_use Flag을 Set한다.          *
** Supply_use Flag을 Set하면 1m 함수에서 다시 실행하게 한다. *
*/
static void prvTimerCallbacksupply_Period(xTimerHandle xTimer)
{
	sysinfo_t *s = &_sysinfo 	;
  
	if(TmrChk(TMR_DOSINGPERIOD) == 0)
	{
		//TmrSetT(TMR_DOSINGPERIOD, 40);
		//TmrStart(TMR_DOSINGPERIOD);

		if(s->DosingControlPeriod && s->Scrst->supply.ContrlSet)
		{
			if(s->DosingControlPeriod != s->DosingPeriodBackup)
			{
				s->DosingPeriodBackup = s->DosingControlPeriod;
				s->Scrst->supply.Supply_use = 1;
				s->Scrst->supply.Pwm_Count = 0;
			}
		}
		else
			s->DosingPeriodBackup = 0;
  }
}

/* 
**1Sec 간격으로 Callback Function				        *
** 1Sec 간격으로 Speed, Bat Voltage을 검출한다.         *
** Key Off시 Board Power Control                        *
*/
static void prvTimerCallbackSpeed(xTimerHandle xTimer)
{
	/*ADC To Voltage : BAT*/
	SysSts.BattVoltage = BatteryVoltage();
  AlarmTask();
	TaskCheckDownload();
	ScrNoxDownpersent();
}

/* 100mSec 간격으로 Callback Function				            *
** 각 Sensor의 값을 계산                                  *
** LED 100msec, 500msec Control                        *
*/
static void prvTimerCallback(xTimerHandle xTimer)
{
  double temp_ratio, temp_tavg;
  
	BSP_LED_Toggle(1);
  temp_ratio = SysSet.Temp_Ratio/100.0;
	/* 100msec 간격으로 온도를 검출한다. */

	SysSts.T1_temp      = TempSensorPt200(0) ;	//PT200 T1
	SysSts.T2_temp      = TempSensorPt200(1) ;	//PT200 T2
	if(SysSts.SCRMode == MANUAL_MODE)
	{
    SysSts.Tavg_temp  = SysSts.mTavg;
    SysSts.MafKg_H    = SysSts.mMaf;
    SysSts.T1_temp    = SysSts.mTavg;
    SysSts.Noxppm[0]  = SysSts.mNoxppm[0];
    SysSts.Noxppm[1]  = SysSts.mNoxppm[1];
	}
  else {
    if(ScrCalib.Temp[0].enable && ScrCalib.Temp[1].enable) { // T1, T2 온도 2개 다 사용할 경우
      temp_tavg         = (double)SysSts.T1_temp*temp_ratio + (double)SysSts.T2_temp*(1-temp_ratio);
      SysSts.Tavg_temp  = (INT16S)temp_tavg;    //PT200 T1 + T2 avg  
    }
    else 
      SysSts.Tavg_temp  = SysSts.T1_temp;     //T1 온도 하나만 사용할 경우
    SysSts.MafKg_H    = MassAirFlowSensor();	//39320-5200(MAF Sensor)    
#ifndef TANK_CAN_SUPPORT
    SysSts.TankTemp   = TankTempCheck();    
#endif
  }
  if(SysSts.Tavg_temp >= 100) SysSts.SystemSignal.Noxact = 1;

	SysSts.Temperature  = ReadEnvTempTable() ;	//TMP36(Board)
	SysSts.P1_bar       = Pressure_Process();	  //24DPS100-01(압력센서)
  SysSts.Maf_temp     = MassAirFlowSensor_Temp();
#ifndef TANK_CAN_SUPPORT     
  SysSts.TankLevelP   = TankLevelCheck();
#endif  
	if(TmrChk(TMR_LED) == 0)
	{
		TmrSetT(TMR_LED, 500);
		TmrStart(TMR_LED);
		BSP_LED_Toggle(2);
	}
	if(TmrChk(TMR_MODULECK) == 0)
	{
		TmrSetT(TMR_MODULECK, 100);
		TmrStart(TMR_MODULECK);
    ModuleTask();
	}
  map_delay++;
  if(map_delay > 100) map_delay = 0;
}

/* 
** Task 만들경우 아래 함수도 Flag을 추가 해야한다.				                  *
** sys_struct.h에Flag, Counter 변수을 추가, Task에 while문에 추가       *
** Task가 무한 루프에 빠지게 되면 Callback function에서 Watchdog을 시동한다.*
*/
static void prvwatchdogCallback(xTimerHandle xTimer)
{
	sysinfo_t *s = &_sysinfo 	;

	if( s->twatchCliFlag  && (s->twatchCliCount++ >= 30) )
	{
		s->tInfiniteLoop = 1;
		printk("CLI waite 3Sec ......\n");
	}
	else					s->twatchCliCount = 0;

	if( s->twatchGuiFlag && (s->twatchGuiCount++ >= 30) )
	{
		s->tInfiniteLoop = 1;
		printk("GUI waite 3Sec ......\n");
	}
	else					s->twatchGuiCount = 0;

	if( s->twatchObdFlag && (s->twatchObdCount++ >= 30) )
	{
		s->tInfiniteLoop = 1;
		printk("OBD waite 3Sec ......\n");
	}
	else					s->twatchObdCount = 0;

	if( s->twatchEcuFlag && (s->twatchEcuCount++ >= 30) )
	{
		s->tInfiniteLoop = 1;
		printk("ECU waite 3Sec ......\n");
	}
	else					s->twatchEcuCount = 0;

	if( s->twatchAdcFlag && (s->twatchAdcCount++ >= 30) )
	{
		s->tInfiniteLoop = 1;
		printk("ADC waite 3Sec ......\n");
	}
	else					s->twatchAdcCount = 0;
	if( s->twatchScrFlag && (s->twatchScrCount++ >= 30) )
	{
		s->tInfiniteLoop = 1;
		printk("SCR Task waite 3Sec ......\n");
	}
	else					s->twatchScrCount = 0;

	/* Reload IWDG counter */
	if( !s->tInfiniteLoop )
	{
#if WATCHDOG_ENABLE > 0
		IWDG_ReloadCounter();  
#endif
	}

}


void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amout of FreeRTOS heap that 
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}


void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software 
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	printd("vApplicationMallocFailedHook....\n");
	for( ;; );
}

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	printd("StackOverflowHook....%s\n",pcTaskName);
	for( ;; );
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


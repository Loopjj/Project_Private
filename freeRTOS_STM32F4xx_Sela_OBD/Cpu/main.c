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

FATFS USB_Fs,  SD_Fs; 
FIL   USB_Fil, SD_Fil;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private semaphore ---------------------------------------------------------*/
xSemaphoreHandle SpiFlashDevSem = NULL;
xSemaphoreHandle BSP_SaveSem	  = NULL;
xSemaphoreHandle BSP_I2C1Sem	  = NULL;
xSemaphoreHandle BSP_NANDSem	  = NULL;
xSemaphoreHandle BSP_BKSramSem	= NULL;
xSemaphoreHandle BSP_BKRtcSem	  = NULL;
xSemaphoreHandle BSP_CanSem     = NULL;
xSemaphoreHandle BSP_KeySem     = NULL;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int errno;

/* Private function prototypes -----------------------------------------------*/
//void Event_Proc(void * pvParameters);
static void prvTimerCallback(xTimerHandle xTimer);
static void prvTimerCallback1000msec(xTimerHandle xTimer);
static void prvwatchdogCallback(xTimerHandle xTimer);

xTimerHandle xTimer01	= NULL;
xTimerHandle xTimer02	= NULL;
xTimerHandle xTimer03	= NULL;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
   
 int main(void)
{
 	sysinfo_t *s = &_sysinfo;
  
  RCC_ClocksTypeDef rcc_clocks;
    
	BSP_Lowlevel_Init();

	RCC_GetClocksFreq(&rcc_clocks);

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_Config( rcc_clocks.HCLK_Frequency / 100 ); 

	CommInit();                     //rs232 
  KeypadInit();                   //key
	sys_struct_init();
//	CLI_SetConsolPort(GPS_PORT);
//  DisplayLogo();
  
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

#if WATCHDOG_ENABLE > 0
	IWDG_ReloadCounter();  
#endif
  GetRTCTime(&s->tm_ptr);
  PreDay = s->tm_ptr.tm_mday;
  
	/* Task Init */
//	xTaskCreate( Task_CLI, 	(portCHAR*)CLITASK_NAME,	configMINIMAL_STACK_SIZE*2,	(void *)&_sysinfo,	CLI_TASK_PRIO,	NULL);
	xTaskCreate( TmrTask, 	(portCHAR*)TMRTASK_NAME,	configMINIMAL_STACK_SIZE*2,	(void *)&_sysinfo,	TMR_TASK_PRIO,	NULL);
  xTaskCreate( Task_MOD,  (portCHAR*)MODTASK_NAME,	configMINIMAL_STACK_SIZE*4, (void *)&_sysinfo,	MOD_TASK_PRIO,	NULL);
	xTaskCreate( Task_CAN,  (portCHAR*)CANTASK_NAME,	configMINIMAL_STACK_SIZE*2, (void *)&_sysinfo,	CAN_TASK_PRIO,	&xHandle_can);
  xTaskCreate( Task_OBD,  (portCHAR*)OBDTASK_NAME,	configMINIMAL_STACK_SIZE*2, (void *)&_sysinfo,	OBD_TASK_PRIO,	NULL);
  xTaskCreate( Task_GPS,  (portCHAR*)GPSTASK_NAME,	configMINIMAL_STACK_SIZE*2, (void *)&_sysinfo,	GPS_TASK_PRIO,	NULL);
	xTaskCreate( Task_KEY,  (portCHAR*)KEYTASK_NAME,	configMINIMAL_STACK_SIZE*2, (void *)&_sysinfo,	KEY_TASK_PRIO,	NULL);
  
  vTaskSuspend(xHandle_can); // CAN Task stop
  
	/* Timer Calback */
	xTimer01     = xTimerCreate("led",( 100 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvTimerCallback);
	if(xTimer01 != NULL) xTimerStart(xTimer01,0);
	xTimer02     = xTimerCreate("time",( 1000 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvTimerCallback1000msec);
	if(xTimer02 != NULL) xTimerStart(xTimer02,0);
	xTimer03     = xTimerCreate("watch", ( 100 / portTICK_RATE_MS ),pdTRUE,( void * ) 0, prvwatchdogCallback);	
	if(xTimer03 != NULL) xTimerStart(xTimer03,0);


	/* Start scheduler */
	vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
    for( ;; )
	{
		vTaskDelay(2000);
	}
}

static void prvTimerCallback1000msec(xTimerHandle xTimer)
{
  Flags.MMCSaveFlag = 1;
  Flags.KeyOnFlag = 1;
  Flags.IntFlag = 1;
  KeyOn_Check();
  
  //TANK LEVEL 카운터
	if(Flags1.HTLvHCntFlag) HTLvHCnt++;
	else HTLvHCnt = 0;

	if(Flags1.HTLvLCntFlag) HTLvLCnt++;
	else HTLvLCnt = 0;
  
  if(!Flags1.BuzzerFlag && TLv_Beep) {
    if(TLv_Beep > 3) TLv_Beep = 3;
    Flags1.BuzzerFlag = 1;
    TLv_Beep--;
  }
  
  if(key.kiflag)
  {
    key.keycount++;
    if(key.keycount > 3){
      keyflag.longflag = 1;
      key.keycount = 0;
    }                  
  }
  
  Flags1.DispFlag = 1;
}

/* 100mSec 간격으로 Callback Function				                    *
** 각 Sensor의 값을 계산                                        *
** LED 100msec, 500msec Control                                 *
*/
static void prvTimerCallback(xTimerHandle xTimer)
{

	if(TmrChk(TMR_LED) == 0)
	{
		TmrSetT(TMR_LED, 500);
		TmrStart(TMR_LED);
		BSP_LED_Toggle(1);
//    if(!FotaFlag) ReadValue.ObdTemp = Read_max6675();	    //K type
//    ReadValue.ObdPress = Pressure_Process();	//24DPS100-01
  }
}

/* 
** Task 만들경우 아래 함수도 Flag을 추가 해야한다.				                  *
** sys_struct.h에Flag, Counter 변수을 추가, Task에 while문에 추가           *
** Task가 무한 루프에 빠지게 되면 Callback function에서 Watchdog을 시동한다.*
*/
static void prvwatchdogCallback(xTimerHandle xTimer)
{

	sysinfo_t *s = &_sysinfo 	;

	if( s->twatchCliFlag  && (s->twatchCliCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("CLI wait 3Sec ......\n");
	}
	else					s->twatchCliCount = 0;

	if(s->twatchCanFlag && (s->twatchCanCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("CAN OBD wait 3Sec ......\n");
	}
	else					s->twatchCanCount = 0;
	if(s->twatchUSBFlag && (s->twatchUSBCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("USB Host wait 3Sec ......\n");
	}
	else					s->twatchUSBCount = 0;
	if(s->twatchKeyFlag && (s->twatchKeyCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("Key wait 3Sec ......\n");
	}
	else					s->twatchKeyCount = 0;
	if(s->twatchObdFlag && (s->twatchObdCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("OBD wait 3Sec ......\n");
	}
	else					s->twatchObdCount = 0;
	if(s->twatchModFlag && (s->twatchModCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("Modem wait 3Sec ......\n");
	}
	else					s->twatchModCount = 0;  
	if(s->twatchGpsFlag && (s->twatchGpsCount++ >= 20) )
	{
		s->tInfiniteLoop = 1;
		printk("GPS wait 3Sec ......\n");
	}
	else					s->twatchGpsCount = 0;   
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

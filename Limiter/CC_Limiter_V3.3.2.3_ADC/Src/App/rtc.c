/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : rtc.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/07/2017
*
* Description        : rtc 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define RTC_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
#define RTC_STATUS_REG      			RTC_BKP_DR19 /* Status Register */

#define RTC_STATUS_INIT_OK              0x1234       /* RTC initialised */
#define RTC_STATUS_TIME_OK              0x4321       /* RTC time OK */
#define	RTC_STATUS_ZERO                 0x0000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
typedef enum {
	RTC_ClockSource_Internal = 0x00, /*!< Use internal clock source for RTC (LSI oscillator) */
	RTC_ClockSource_External         /*!< Use external clock source for RTC (LSE oscillator) */
} RTC_ClockSource_t;

/* Private functions ---------------------------------------------------------*/

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

rtc_time RTC_Val;
time_t RTC_LVal;

osSemaphoreDef(BSP_BKSramSem);
osSemaphoreId BSP_BKSramSem = NULL;

void RTC_Config(RTC_ClockSource_t source) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	/* We are updating RTC clock */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;

	/* Do not use PLL */
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

	/* LSI is used as RTC clock */
	if (source == RTC_ClockSource_Internal) {
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
		RCC_OscInitStruct.LSIState = RCC_LSI_ON;

		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	} else {
		/* LSE is used */
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
		RCC_OscInitStruct.LSEState = RCC_LSE_ON;

		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	}
	
	/* Config oscillator */
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Select peripheral clock */
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	
	/* Enable RTC Clock */ 
	__HAL_RCC_RTC_ENABLE(); 
}


/* RTC init function */
void MX_RTC_Init(void)
{
	uint32_t status;
  
    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 0x1F;
  hrtc.Init.SynchPrediv = 0x3FF;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  
  /*PWREN : Enable backup domain access  */
  __HAL_RCC_PWR_CLK_ENABLE();

  /*DBP : Enable access to Backup domain */
  HAL_PWR_EnableBkUpAccess();
 
  RTC_Config(RTC_ClockSource_External);    

	/* Get RTC status */
	status = HAL_RTCEx_BKUPRead(&hrtc, RTC_STATUS_REG);

	/* Check if RTC already initialized */
	if (status != RTC_STATUS_TIME_OK) {
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
		/* Save data to backup regiser */
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_STATUS_REG, RTC_STATUS_TIME_OK);    
  }

    /**Enable the WakeUp 
    */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  __HAL_RCC_RTC_ENABLE();
  __HAL_RCC_BKPSRAM_CLK_ENABLE();
	HAL_PWREx_EnableBkUpReg();     // set PWR->CSR.bre = 1;
  HAL_RTCEx_EnableBypassShadow(&hrtc);
}

/**
  * @brief  RTC_BKRam_Init
  * @param  None
  * @retval None
  */
void RTC_BKRam_Init(void)
{
	BSP_BKSramSem = osSemaphoreCreate(osSemaphore(BSP_BKSramSem) , 1);
}


/**
  * @brief  Configure the current time conversion.
  * @param  None
  * @retval None
  */
time_t RTCValTotime(rtc_time rtc_val)
{
  struct tm stime;

  stime.tm_year   = 2000 + rtc_val.tm_year - 1900;
  stime.tm_mon    = rtc_val.tm_mon - 1;
  stime.tm_mday   = rtc_val.tm_mday;
  stime.tm_hour   = rtc_val.tm_hour;
  stime.tm_min    = rtc_val.tm_min;
  stime.tm_sec    = rtc_val.tm_sec;
  stime.tm_isdst  = 0; 
  return mktime(&stime);
}
   
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_SetDateTime(uint8_t *dt)
{
  RTC_DateTypeDef RTC_DateStruct;
  RTC_TimeTypeDef RTC_TimeStruct;

  /** Configure the Time
  */
  RTC_TimeStruct.Hours =   dt[3];
  RTC_TimeStruct.Minutes = dt[4];
  RTC_TimeStruct.Seconds = dt[5];
  RTC_TimeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  RTC_TimeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&hrtc,&RTC_TimeStruct,FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }  
  
  /** Configure the Date
  */
  RTC_DateStruct.Year =   dt[0];
  RTC_DateStruct.Month =  dt[1];
  RTC_DateStruct.Date =   dt[2];
  RTC_DateStruct.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  if(HAL_RTC_SetDate(&hrtc,&RTC_DateStruct,FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
}

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTC_GetDateTime(void)
{
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, FORMAT_BIN);

  RTC_Val.tm_year = RTC_DateStruct.Year;
  RTC_Val.tm_mon = RTC_DateStruct.Month;
  RTC_Val.tm_mday = RTC_DateStruct.Date;
  RTC_Val.tm_hour = RTC_TimeStruct.Hours;
  RTC_Val.tm_min = RTC_TimeStruct.Minutes;
  RTC_Val.tm_sec = RTC_TimeStruct.Seconds;  
  
  RTC_LVal = RTCValTotime(RTC_Val);  
} 

/**
  * @brief  gmtime conversion
  * @param  time_t
  * @retval rtc time
  */
rtc_time gmtime_conv(time_t rtime)
{  
  rtc_time local;
  struct tm *t;
  
  t = gmtime((time_t*)&rtime);
  local.tm_wday = (u8)(t->tm_wday);
  if(t->tm_year+1900 > 2000)
    local.tm_year = (u8)(t->tm_year+1900-2000);
  else local.tm_year = 0;
  local.tm_mon = (u8)(t->tm_mon+1);
  local.tm_mday = (u8)(t->tm_mday);
  local.tm_hour = (u8)(t->tm_hour);
  local.tm_min = (u8)(t->tm_min);
  local.tm_sec = (u8)(t->tm_sec);
  return local;
}

/**
  * @brief  write_to_backup_sram
  * @param  time_t
  * @retval rtc time
  */
INT8U write_to_backup_sram( INT8U *data, INT16U size, INT16U offset )
{
	const INT16U backup_size = 0x1000;
	INT8U* base_addr = (INT8U *) BKPSRAM_BASE;
	INT16U i;

	if(BSP_BKSramSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(osSemaphoreWait(BSP_BKSramSem, 0) == osOK)
	{

		if( size + offset >= backup_size ) 
		{
			/* ERROR : the last byte is outside the backup SRAM region */
			xSemaphoreGive(BSP_BKSramSem);
			return FALSE;
		}
		__HAL_RCC_BKPSRAM_CLK_ENABLE();
			/* disable backup domain write protection */
		__HAL_RCC_PWR_CLK_ENABLE();   // set RCC->APB1ENR.pwren
		HAL_PWR_EnableBkUpAccess();                          // set PWR->CR.dbp = 1;
		/** enable the backup regulator (used to maintain the backup SRAM content in
		* standby and Vbat modes).  NOTE : this bit is not reset when the device
		* wakes up from standby, system reset or power reset. You can check that
		* the backup regulator is ready on PWR->CSR.brr, see rm p144 */
		HAL_PWREx_EnableBkUpReg();     // set PWR->CSR.bre = 1;
		for( i = 0; i < size; i++ )
		{
			*(base_addr + offset + i) = *(data + i);
		}
		HAL_PWR_DisableBkUpAccess();                     // reset PWR->CR.dbp = 0;
		osSemaphoreRelease(BSP_BKSramSem);
		return TRUE;
	
	}
	return FALSE;
}

/**
  * @brief  read_from_backup_sram
  * @param  time_t
  * @retval rtc time
  */
INT8U read_from_backup_sram( INT8U *data, INT16U size, INT16U offset )
{
	const INT16U backup_size = 0x1000;
	INT8U* base_addr = (INT8U *) BKPSRAM_BASE;
	INT16U i;

	if(BSP_BKSramSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(osSemaphoreWait(BSP_BKSramSem, 0) == osOK)
	{
		if( size + offset >= backup_size ) 
		{
			/* ERROR : the last byte is outside the backup SRAM region */
			osSemaphoreRelease(BSP_BKSramSem);
			return FALSE;
		}
    
		__HAL_RCC_BKPSRAM_CLK_ENABLE();

		for( i = 0; i < size; i++ )
		{
			*(data + i) = *(base_addr + offset + i);
		}
		osSemaphoreRelease(BSP_BKSramSem);
		return TRUE;
	}
	return FALSE;
}

/**
  * @brief  dbg_backupsramwr_test
  * @param  time_t
  * @retval rtc time
  */
INT16U dbg_backupsramwr_test(void)
{
	INT16U size, addr;
  static INT8U BS_RxB[100];
  
	if(CLI_GetArgCnt() < 2) return INV_PRM;
	
	addr = comparison_num((char *)P_ARG(1));
	size = comparison_num((char *)P_ARG(2));

  if(size > 100) size = 100;
	read_from_backup_sram( BS_RxB, size, addr);
	MemoryDisplay((INT32U)BS_RxB, size);

	return NO_DISPLAY;
}

void RTC_WriteBackupRegister(uint32_t backupRegister, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&hrtc, backupRegister, data);
}

uint32_t RTC_ReadBackupRegister(uint32_t backupRegister)
{
  return HAL_RTCEx_BKUPRead(&hrtc, backupRegister);
}
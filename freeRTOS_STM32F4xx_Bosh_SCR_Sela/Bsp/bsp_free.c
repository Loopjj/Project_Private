/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : bsp_free.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/18/2014
*
* Description        : SCR board configuration 
*
*********************************************************************************************************
*/

#define BSP_GLOBAL
#include "includes.h"



/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/

#define FLASH_START_BASE	0x10000	// 부트로더 Offset 


/*
*********************************************************************************************************
*                                              TYPE DEFINE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void BSP_IWDG_Init(void);			// Independent Watchdog
static void BSP_NVIC_Init(void);			// Interrupt priority setting
static void BSP_RTC_Init(void);				// 32.768KHz

#if PDV_ENABLE > 0
static void BSP_PVD_Init(void);				// Power Voltage Detect
#endif


static void BSP_GPIO_Init(void);			// etc gpio setting

static void BSP_TIM2_Init(void);			// 0.05ms Timer
static void BSP_TIM3_PWM_Init(void);
static void BSP_TIM4_PWM_Init(void);
static void BSP_TIM5_PWM_Init(void);

static void BSP_EXTI_Init(void);

//static void BSP_I2C1_Init(void);
static void BSP_I2C2_Init(void);

static void BSP_CAN1_Init(void);
static void BSP_CAN2_Init(void);

//static void BSP_SPI1_Init(void);			
static void BSP_SPI2_Init(void);			
//static void BSP_SPI3_Init(void);			

static void BSP_USART2_Init(void);			
//static void BSP_USART3_Init(void);			 
static void BSP_USART4_Init(void);

static void BSP_ADC1_INT_Init(void);
static void BSP_ADC1_TIM1_Init(void);





//------------------------------------------------------------------ Interrupt Service Routine Function

#if PDV_ENABLE > 0
void PVD_IRQHandler(void);
#endif
void RTC_IRQHandler(void);

void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);

void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);

void CAN1_RX0_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);

void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);

void EXTI15_10_IRQHandler(void);
void EXTI3_IRQHandler(void);

void ADC_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void ADC_IRQHandler(void);



extern __IO I2C_Dir I2C_DIR;
extern __IO uint8_t I2C_BLOCKED;
extern __IO uint8_t I2CADDRESS;
extern __IO uint8_t rx_buffer_len;
extern __IO uint8_t tx_buffer_len;
extern __IO uint8_t tx_buffer_ix;
extern __IO uint8_t rx_buffer_ix;
extern __IO uint8_t tx_buffer[];
extern __IO uint8_t *rx_buffer_ptr;

CanRxMsg RxMessage;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         BSP_Lowlevel_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void BSP_Lowlevel_Init(void)
{
	RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);

	SystemInit();	

	BSP_NVIC_Init();

	BSP_GPIO_Init();

	BSP_EXTI_Init();

	BSP_I2C2_Init();

	BSP_CAN1_Init();

	BSP_CAN2_Init();
#if PDV_ENABLE > 0
	BSP_PVD_Init();
#endif

	BSP_SPI2_Init();

	BSP_USART2_Init();

	BSP_USART4_Init();

//	BSP_TIM2_Init();

	
	BSP_ADC1_TIM1_Init();
	BSP_ADC1_INT_Init();
#if PWM_ENABLE > 0
	BSP_TIM3_PWM_Init();
	BSP_TIM4_PWM_Init();  
	BSP_TIM5_PWM_Init();
#endif
#if WATCHDOG_ENABLE > 0
	BSP_IWDG_Init();
#endif

#if RTC_ENABLE > 0
	BSP_RTC_Init();
#endif

#if SCR_USE_NAND > 0
	FSMC_NAND_Init();
#endif

}

/*
*********************************************************************************************************
*                                            BSP_NVIC_Init()
*
* Description : Initialize the board's nested vectered interrupt controller
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void BSP_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Set the Vector Table base location at 0x10000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_START_BASE);					// 내부 Flash 시작 번지 

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel					         = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = I2C2_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the UART4 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the PVD event interrupt */
#if PDV_ENABLE > 0
	NVIC_InitStructure.NVIC_IRQChannel                   = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
#endif
	NVIC_InitStructure.NVIC_IRQChannel					         = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 5;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 6;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = EXTI3_IRQn; //Speed
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 9;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = DMA2_Stream4_IRQn;// ADC3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 10;
	NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = TIM1_CC_IRQn;// ADC1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 11;
	NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#if PWM_ENABLE > 0
	NVIC_InitStructure.NVIC_IRQChannel					         = TIM3_IRQn;// Time 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 13;
	NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = TIM5_IRQn;// Time 5
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 12;
	NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}


/*
*********************************************************************************************************
*                                            BSP_IWDG_Init()
*
* Description : Initialize the board's Independent watch-dog 
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void BSP_IWDG_Init(void)
{
	/* Select HCLK/8 as SysTick clock source */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* IWDGRST flag set */
		/* Clear reset flags */
		RCC_ClearFlag();
	}


	/* IWDG timeout equal to 1 s (the timeout may varies due to LSI frequency dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz(0.8ms) */
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	/* Set counter reload value to 2500(*0.8ms) */
//	IWDG_SetReload(2500);	// 2초 셋팅
	IWDG_SetReload(3750);	// 3초 셋팅

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}


/*
*********************************************************************************************************
*                                            BSP_RTC_Init()
*
* Description : Initialize the board's RTC (32.768KHz)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
#define RTC_CLOCK_SOURCE_LSE

static void BSP_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,     ENABLE);
	/* Enable backup SRAM clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CCMDATARAMEN, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	RCC_BackupResetCmd(ENABLE);
	RCC_BackupResetCmd(DISABLE);

	//RCC_RTCCLKConfig(RTC_CLOCK_SOURCE_LSE);

	#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
	/* LSI used as RTC source clock */
	/* The RTC Clock may varies due to LSI frequency dispersion. */   
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}
	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		
	}
	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	#endif
	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

	/* Calendar Configuration with LSI supposed at 32KHz */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv	=  0xFF; /* (32KHz / 128) - 1 = 0xFF*/
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
}


/*
*********************************************************************************************************
*                                            BSP_PVD_Init()
*
* Description : Initialize the board's Power voltage detect
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
#if PDV_ENABLE > 0
static void BSP_PVD_Init(void)
{

	EXTI_InitTypeDef EXTI_InitStructure;

  /* Enable PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
      
  /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
     falling edges */
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure the PVD Level to 3 (PVD detection level set to 2.5V, refer to the
      electrical characteristics of you device datasheet for more details) */
  PWR_PVDLevelConfig(PWR_PVDLevel_3);

  /* Enable the PVD Output */
  PWR_PVDCmd(ENABLE);
}
#endif


/*
*********************************************************************************************************
*                                            BSP_GPIO_Init()
*
* Description : Initialize GPIO.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void BSP_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
#if (PWM_ENABLE == 0)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
#endif
	
	GPIO_InitStructure.GPIO_Pin   = 
		GPIO_Pin_9 |				//DI1
		GPIO_Pin_10;				//DI0
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*POWER ENABLE*/
	GPIO_InitStructure.GPIO_Pin   = 
		GPIO_Pin_13;				// POWER ENABLE
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = 
		GPIO_Pin_4 |				// TANK_CTL
		GPIO_Pin_5 |				// SOL_CTL
		GPIO_Pin_6 |				// MOTOR_CTL
		GPIO_Pin_7 |				// LINE_CTL
		GPIO_Pin_8 ;				// DO0_CTL
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;//GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
 
  
#if (PWM_ENABLE == 0)
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
#endif
	POWER_ENABLE();                         	/*POWER ENABLE*/

	GPIO_WriteBit(GPIOF, GPIO_Pin_0, Bit_SET);				
	GPIO_WriteBit(GPIOF, GPIO_Pin_1, Bit_SET);	
	GPIO_WriteBit(GPIOF, GPIO_Pin_2, Bit_SET); // MAF ENABLE

}

/*
*********************************************************************************************************
*                                            StartDosing()
*
* Description : Start Dosing.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : prvTimerCallbacksupply().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void StartDosing(void)
{
	TIM3->CCR2 = 200 - 1;
}

/*
*********************************************************************************************************
*                                            EndDosing()
*
* Description : End Dosing.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : prvTimerCallbacksupply().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void EndDosing(void)
{
	TIM3->CCR2 = 0;
}

/*
*********************************************************************************************************
*                                            StartPwm()
*
* Description : Start supply module.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MotorCtrl().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void StartPwm(void)
{
  double duty;
  
  duty = ((double)SysSts.supply.Duty_Cycle / 100.0) * 10000.0 - 1.0;
	TIM5->CCR2 = (INT32U)duty;
}

/*
*********************************************************************************************************
*                                            EndPwm()
*
* Description : End supply module.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MotorCtrl().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void EndPwm(void)
{
	TIM5->CCR2 = 0;
}

/*
*********************************************************************************************************
*                                            TIM3_IRQHandler()
*
* Description : Timer 3 Intterrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//clock HIGH interrupt
	{
    if(SysSts.SystemSignal.Dosing) {
      if(!SysSts.dosing.Dosing_Count) {
        if(SysSts.SCRMode <= ALPHA_MODE) {   // 자동 재생
          ScrCalculate();
        }
        else {                              // 수동 재생
          if(SysSts.MDosingStart == 1) {
            SysSts.DosingRatehouer = SysSts.MDosingRatehouer;          
            SysSts.dosing.Gpio_Period =  GramPerHourToDM(SysSts.MDosingRatehouer, SCR_PERIOD);
          }
          else if(SysSts.MDosingStart == 2) {
            SysSts.dosing.Gpio_Period =  (INT16U)(((double)SCR_PERIOD / 10.0) * SysSts.MDosingDuty);
            SysSts.DosingDuty = (INT16U)SysSts.MDosingDuty;
          }
        }
        if(SysSts.dosing.Gpio_Period && ScrFlag.DosingStart) PWM_SetInject(30);
        else PWM_SetInject(0);
        
        SCR_Reduction_Buffer();
      }
      else if((SysSts.dosing.Gpio_Period == SysSts.dosing.Dosing_Count) || ((SysSts.dosing.Gpio_Period == DosingTimer) && ((SysSts.dosing.Gpio_Period - 1) == SysSts.dosing.Dosing_Count))){
        if(SysSts.dosing.Gpio_Period != DosingTimer) PWM_SetInject(0);
      }
      SysSts.dosing.Dosing_Count++;
      SysSts.dosing.Dosing_Count %= DosingTimer;
    }
    else {
      SysSts.DosingRatehouer = 0;
      SysSts.MDosingDuty = 0;
      SysSts.dosing.Dosing_Count = 0;
    }
    
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

/*
*********************************************************************************************************
*                                            TIM5_IRQHandler()
*
* Description : Timer 5 Intterrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}

/*
*********************************************************************************************************
*                                            BSP_TIM2_Init()
*
* Description : Initialize TIMER.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* ---------------------------------------------------------------
	TIM5 Configuration: Time base:  200KHz (5us)
	- TIM2CLK = 84 MHz, Prescaler = 84-1
	- PrescalerValue = TIM2CLK / (TIM_Period + 1) = 16,800,000
	- Time base =  PrescalerValue / (Prescaler + 1)
                 16,800,000 / 84 = 200000 (5us)
	--------------------------------------------------------------- */
	/* TIM clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // 32bit timer
  
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period        = 5 -1; 
	TIM_TimeBaseStructure.TIM_Prescaler     = 84-1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM Update */
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);

	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);

	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);
  
}

/*
*********************************************************************************************************
*                                            BSP_TIM3_PWM_Init()
*
* Description : Initialize PWM.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_TIM3_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//32bit timer
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 200-1;// 5kHz
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;	  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//40Hz
	//TIM_OCInitStructure.TIM_Pulse = 10500-1;//40Hz
	//TIM_OCInitStructure.TIM_Pulse = 25000-1;//20Hz
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	//TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM3,  TIM_IT_Update, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	//TIM_Cmd(TIM3, DISABLE);
}

/*
*********************************************************************************************************
*                                            BSP_TIM4_PWM_Init()
*
* Description : Initialize PWM.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_TIM4_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//32bit timer
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	
  /* ---------------------------------------------------------------
	TIM4 Configuration: Time base: 2MHz 
	- TIM_Period = 840000 - 1
	- TIM4CLK = 84 MHz, Prescaler = 0
	- PrescalerValue = TIM4CLK / (TIM_Period + 1) = 1000
	- Time base =  PrescalerValue / (Prescaler + 1) = 1000
	--------------------------------------------------------------- */

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 100-1; //20Khz
	TIM_TimeBaseStructure.TIM_Prescaler = 42-1;	  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
  
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
  TIM4->ARR = 100;
  TIM4->CCR2 = 0;  
}

void PWM_SetInject(u8 rate)
{
  if(rate < TIM4->ARR) {
    TIM_SetCompare2(TIM4,rate);
    if(TIM4->CNT > TIM4->ARR)  TIM4->CNT = 0;
  }
  else {
    TIM4->CCR2 = 0;
  }
}  


/*
*********************************************************************************************************
*                                            BSP_TIM5_PWM_Init()
*
* Description : Initialize PWM.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_TIM5_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		//32bit timer
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	
  /* ---------------------------------------------------------------
	TIM5 Configuration: Time base: 100Hz (10msec)
	- TIM_Period = 840000 - 1
	- TIM5CLK = 84 MHz, Prescaler = 0
	- PrescalerValue = TIM5CLK / (TIM_Period + 1) = 1000
	- Time base =  PrescalerValue / (Prescaler + 1) = 1000
	--------------------------------------------------------------- */

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10000-1; //100hz
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;	  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_Pulse = 21000-1;//1Khz
	TIM_OCInitStructure.TIM_Pulse = 0; //500hz
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM5, ENABLE);

	//TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update , ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM5, ENABLE);
	//TIM_Cmd(TIM5, DISABLE);
}

/*
*********************************************************************************************************
*                                            BSP_GPIO_Init()
*
* Description : Initialize GPIO.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void BSP_EXTI_Init(void)
{

  GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
  
	/* Enable SYSCFG clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;					//Key On
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
   
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource3); //Key On

	EXTI_InitStructure.EXTI_Mode	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Line	= EXTI_Line3; //Key On,,EXTI3_IRQHandler
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_Init(&EXTI_InitStructure);

}

#if 0
/*
*********************************************************************************************************
*                                              BSP_I2C1_Init()
*
* Description : Initialize the board's I2C1
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_I2C1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);		//Enable APB1 peripheral clock for I2C1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		//Enable clock for SCL and SDA pins

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;			//Use PB6 for SCL and PB7 for SDA
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                   //Set pins to alternate function
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				//Set GPIO speed
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;                 //Set output to open drain --> the line has to be only pulled low, not driven high
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                       //Enable pull up resistors
  GPIO_Init(GPIOB, &GPIO_InitStruct);                                 //Initialise GPIOB


  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //Connect I2C1 SCL pin to alternate function
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); //Connect I2C1 SDA pin to alternate function

  I2C_StructInit(&I2C_InitStruct);

                                                                   
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                                                                 //I2C mode
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;                                                 //50% duty cycle --> standard
  I2C_InitStruct.I2C_OwnAddress1 = 0x2A;                                                                  //Own address, not relevant in master mode
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                                                                //Enable acknowledge when reading (can be changed later on)
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //Set address length to 7 bit addresses
  I2C_InitStruct.I2C_ClockSpeed = 400000;									//100kHz
  I2C_Init(I2C1, &I2C_InitStruct);                                                                                //Initialise I2C1

  I2C_Cmd(I2C1, ENABLE);  //Enable I2C1
  I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
}
#endif


/*
*********************************************************************************************************
*                                              BSP_I2C2_Init()
*
* Description : Initialize the board's I2C2
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_I2C2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);		//Enable APB1 peripheral clock for I2C1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		//Enable clock for SCL and SDA pins

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;			//Use PB6 for SCL and PB7 for SDA
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                   //Set pins to alternate function
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				//Set GPIO speed
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;                 //Set output to open drain --> the line has to be only pulled low, not driven high
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                       //Enable pull up resistors
  GPIO_Init(GPIOB, &GPIO_InitStruct);                                 //Initialise GPIOB


  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C1); //Connect I2C1 SCL pin to alternate function
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C1); //Connect I2C1 SDA pin to alternate function

  I2C_StructInit(&I2C_InitStruct);

                                                                   
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                                                                 //I2C mode
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;                                                 //50% duty cycle --> standard
  I2C_InitStruct.I2C_OwnAddress1 = 0x2A;                                                                  //Own address, not relevant in master mode
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                                                                //Enable acknowledge when reading (can be changed later on)
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //Set address length to 7 bit addresses
  I2C_InitStruct.I2C_ClockSpeed = 400000;									//100kHz
  I2C_Init(I2C2, &I2C_InitStruct);                                                                                //Initialise I2C1

  I2C_Cmd(I2C2, ENABLE);  //Enable I2C1
  I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
}


/*
*********************************************************************************************************
*                                              BSP_CAN1_Init()
*
* Description : Initialize the board's CAN1
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_CAN1_Init(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;				// CAN1 OBD STBY
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); //Connect CAN1 RX pin to alternate function
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); //Connect CAN1 TX pin to alternate function

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* CAN configuration ********************************************************/  

	/* CAN register init */
	CAN_DeInit(CAN1);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_Silent;//CAN_Mode_Normal;

	// CAN Bitrate
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;    // SJW (1 bis 4 moglich)
	CAN_InitStructure.CAN_BS1=CAN_BS1_14tq;   // Samplepoint 72%
	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq;    // Samplepoint 72%
	CAN_InitStructure.CAN_Prescaler=8;        //  250 kbit/s

#if 0
	CAN_InitStructure.CAN_Prescaler=1;        // 2000 kbit/s
	CAN_InitStructure.CAN_Prescaler=2;        // 1000 kbit/s
	CAN_InitStructure.CAN_Prescaler=4;        //  500 kbit/s
	CAN_InitStructure.CAN_Prescaler=5;        //  400 kbit/s
	CAN_InitStructure.CAN_Prescaler=8;        //  250 kbit/s
	CAN_InitStructure.CAN_Prescaler=10;       //  200 kbit/s
	CAN_InitStructure.CAN_Prescaler=16;       //  125 kbit/s
	CAN_InitStructure.CAN_Prescaler=20;       //  100 kbit/s
	CAN_InitStructure.CAN_Prescaler=40;       //   50 kbit/s
	CAN_InitStructure.CAN_Prescaler=80;       //   40 kbit/s
	CAN_InitStructure.CAN_Prescaler=200;      //   10 kbit/s
	CAN_InitStructure.CAN_Prescaler=1023;      //    ganz langsam
#endif
	
	CAN_Init(CAN1, &CAN_InitStructure);
	/* CAN filter init */
	
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

#if 1
	CAN_FilterInitStructure.CAN_FilterNumber      = 0;						//NOx input
  CAN_FilterInitStructure.CAN_FilterIdHigh		  = (u16)((0x18F00E51<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterIdLow			  = (u16)((0x18F00E51<<3) | 4);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	= (u16)((0x1FF00EAC<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		= (u16)((0x1FF00EAC<<3) | 4);
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber      = 1;						//NOx Output
	CAN_FilterInitStructure.CAN_FilterIdHigh		  = (u16)((0x18F00F52<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterIdLow			  = (u16)((0x18F00F52<<3) | 4);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	= (u16)((0x1FF00FFF<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		= (u16)((0x1FF00FFF<<3) | 4);
	CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FilterInitStructure.CAN_FilterNumber      = 2;						//Urea Tank
	CAN_FilterInitStructure.CAN_FilterIdHigh		  = (u16)((0x18FFFFA3<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterIdLow			  = (u16)((0x18FFFFA3<<3) | 4);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	= (u16)((0x1FFC02FF<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		= (u16)((0x1FFC02FF<<3) | 4);
	CAN_FilterInit(&CAN_FilterInitStructure);
#endif
#if 0
	printd("1 CAN_FilterInitStructure.CAN_FilterIdHigh     = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterIdHigh);
	printd("1 CAN_FilterInitStructure.CAN_FilterIdLow      = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterIdLow);
	printd("1 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterMaskIdHigh);
	printd("1 CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterMaskIdLow);
#endif	
	GPIO_WriteBit(GPIOG, GPIO_Pin_2, Bit_RESET );	// CAN1 OBD STBY HIGH //Bit_RESET

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*
*********************************************************************************************************
*                                              BSP_CAN2_Init()
*
* Description : Initialize the board's CAN2
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void BSP_CAN2_Init(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;				// CAN2 ECU STBY
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2); //Connect CAN1 RX pin to alternate function
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2); //Connect CAN1 TX pin to alternate function

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* CAN configuration ********************************************************/  

	/* CAN register init */
	CAN_DeInit(CAN2);

	CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;// CAN_Mode_Silent; //CAN_Mode_Normal;

	// CAN Bitrate
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;    // SJW (1 bis 4 moglich)
	CAN_InitStructure.CAN_BS1=CAN_BS1_14tq;   // Samplepoint 72%
	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq;    // Samplepoint 72%
	CAN_InitStructure.CAN_Prescaler=4;        //  500 kbit/s

#if 0  
	CAN_InitStructure.CAN_Prescaler=1;        // 2000 kbit/s
	CAN_InitStructure.CAN_Prescaler=2;        // 1000 kbit/s
	CAN_InitStructure.CAN_Prescaler=4;        //  500 kbit/s
	CAN_InitStructure.CAN_Prescaler=5;        //  400 kbit/s
	CAN_InitStructure.CAN_Prescaler=8;        //  250 kbit/s
	CAN_InitStructure.CAN_Prescaler=10;       //  200 kbit/s
	CAN_InitStructure.CAN_Prescaler=16;       //  125 kbit/s
	CAN_InitStructure.CAN_Prescaler=20;       //  100 kbit/s
	CAN_InitStructure.CAN_Prescaler=40;       //   50 kbit/s
	CAN_InitStructure.CAN_Prescaler=80;       //   40 kbit/s
	CAN_InitStructure.CAN_Prescaler=200;      //   10 kbit/s
	CAN_InitStructure.CAN_Prescaler=1023;     //    ganz langsam
#endif
	CAN_Init(CAN2, &CAN_InitStructure);
	CAN_SlaveStartBank(13);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

#if 0
	CAN_FilterInitStructure.CAN_FilterNumber        = 13;						
  CAN_FilterInitStructure.CAN_FilterIdHigh		    = (u16)((0x00000000<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterIdLow			    = (u16)((0x00000000<<3) | 4);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	  = (u16)((0x00000000<<3)>>16);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		  = (u16)((0x00000000<<3) | 4);
	CAN_FilterInit(&CAN_FilterInitStructure);
#else
	CAN_FilterInitStructure.CAN_FilterNumber        = 13;						
  CAN_FilterInitStructure.CAN_FilterIdHigh		    = (u16)(0x0010<<5);  // 0x010 ~ 0x013 수신
  CAN_FilterInitStructure.CAN_FilterIdLow			    = (u16)(0x0098<<5);  // 0x098 ~ 0x099 수신
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	  = (u16)(0x07FC<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		  = (u16)(0x07FE<<5);
	CAN_FilterInit(&CAN_FilterInitStructure);
  
	CAN_FilterInitStructure.CAN_FilterNumber        = 14;						
  CAN_FilterInitStructure.CAN_FilterIdHigh		    = (u16)(0x0100<<5);  // x100 ~ 0x1FF 수신
  CAN_FilterInitStructure.CAN_FilterIdLow			    = (u16)(0x0100<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	  = (u16)(0x0700<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		  = (u16)(0x0700<<5);
	CAN_FilterInit(&CAN_FilterInitStructure);
#endif  
  
#if 0
	printd("CAN_FilterInitStructure.CAN_FilterIdHigh     = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterIdHigh);
	printd("CAN_FilterInitStructure.CAN_FilterIdLow      = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterIdLow);
	printd("CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterMaskIdHigh);
	printd("CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0x%04X\n",CAN_FilterInitStructure.CAN_FilterMaskIdLow);
#endif
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);	// CAN2 ECU STBY HIGH
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN2, CAN_IT_FOV0, ENABLE);
}

/*
*********************************************************************************************************
*                                              BSP_SPI_Init()
*
* Description : Initialize the board's SPI
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Lowlevel_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void BSP_SPI2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	
	/*!< Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*!< Enable the SPI clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* Configure SPI pin: SCK*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure SPI pin: MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure SPI pin: MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable alternate function for SPI pins */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	
	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
 
	/* SPI2 configuration */
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE);

}



static void BSP_USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
														//PORT A USE
	/* Enable USART2, GPIOA clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

	// Connect PXx to USART2_Tx 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	// Connect PXx to USART3_Rx 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	 /* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* USART configured as follow */
	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);

	//BSP_IntVectSet(BSP_INT_ID_USART3,BSP_USART3_IRQHandler);
	
	/* Enable USART2 Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_TC, DISABLE);


	/* Enable USART2 Receive interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	
	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);

}
void BSP_UART2RX_IntEnable(FunctionalState state)
{
	USART_ITConfig(USART2, USART_IT_RXNE, state);
}



static void BSP_USART4_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable UART4, GPIOA, and AFIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 


	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);          
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);  
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure UART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);



	/* USART configured as follow */
	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART4 */
	USART_Init(UART4, &USART_InitStructure);

	
	/* Enable USART3 Transmit interrupts */
	USART_ITConfig(UART4, USART_IT_TC, DISABLE);

	/* Enable USART3 Receive interrupts */
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);

	/* Enable the USART3 */
	USART_Cmd(UART4, ENABLE);
}

void BSP_UART4RX_IntEnable(FunctionalState state)
{
	USART_ITConfig(UART4, USART_IT_RXNE, state);
}



////////////////////////////////////////////////ADC TIME DMA////////////////////////////////////////////////////////////
/*               ADC1, TIM1               */
static void BSP_ADC1_TIM1_Init(void) // TIM1
{
	//uint16_t PrescalerValue = 84;
	//uint16_t CCR1_Val = 98;
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* ---------------------------------------------------------------
	TIM8 Configuration: Time base: 20 KHz (0.1msec)
	- TIM_Period = 8400 - 1
	- TIM8CLK = 84 MHz, Prescaler = 0
	- PrescalerValue = TIM2CLK / (TIM_Period + 1) = 10000
	- Time base =  PrescalerValue / (Prescaler + 1) = 10000
	--------------------------------------------------------------- */


	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 8399;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 8399;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
 
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
 
	/* TIM1 enable counter */
	TIM_Cmd(TIM1, ENABLE);
}

/*
********************************************************************************
* Description : BSP_ADC1_DMA_Init
* Arguments   : none
* Return      : 
* Note        : ADC1 DMA init
******************************************************************************** 
*/
static void BSP_ADC1_DMA_Init(void) 
{
	DMA_InitTypeDef DMA_InitStructure;

	ADC_DeInit();
	DMA_DeInit(DMA2_Stream4);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &adcdata[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream4, ENABLE);

	DMA_ITConfig(DMA2_Stream4, DMA_IT_TC, ENABLE);
}

/*
********************************************************************************
* Description : BSP_ADC1ConStart
* Arguments   : none
* Return      : 
* Note        : ADC Start
******************************************************************************** 
*/
void BSP_ADC1ConStart(void)
{
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConv(ADC1);
}

/*
********************************************************************************
* Description : BSP_ADC1_Init
* Arguments   : none
* Return      : 
* Note        : ADC1 init
******************************************************************************** 
*/

static void BSP_ADC1_INT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	BSP_ADC1_DMA_Init();

	GPIO_InitStructure.GPIO_Pin = 
		GPIO_Pin_4 |			//ADC12_IN4
		GPIO_Pin_5 |			//ADC12_IN5
		GPIO_Pin_6 |			//ADC12_IN6
		GPIO_Pin_7;				//ADC12_IN7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 
		GPIO_Pin_0 ;			//ADC12_IN8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 
		GPIO_Pin_0 |			//ADC123_IN10
		GPIO_Pin_1 |			//ADC123_IN11
		GPIO_Pin_2 |			//ADC123_IN12
		GPIO_Pin_3 |			//ADC123_IN13
		GPIO_Pin_4;				//ADC123_IN14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);



	ADC_CommonInitStructure.ADC_Mode			= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler		= ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay= ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution			= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode			= ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode	= ENABLE;

	ADC_InitStructure.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion		= 10;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_15Cycles);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  5, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,  6, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6,  7, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7,  8, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 9, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 10, ADC_SampleTime_15Cycles);


	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigConv_T1_CC1);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	//ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);


	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
}




/*
*********************************************************************************************************
*********************************************************************************************************
*                                          OS CORTEX-M3 FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
/*
********************************************************************************
* Description : BSP_CPU_ClkFreq
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
CPU_INT32U BSP_CPU_ClkFreq(void)
{
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*
********************************************************************************
* Description : OS_CPU_SysTickClkFreq
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
INT32U OS_CPU_SysTickClkFreq(void)
{
    INT32U freq;

    freq = BSP_CPU_ClkFreq();
    return (freq);
}

/*
********************************************************************************
* Description : CAN_Receive_mk
* Arguments   : none
* Return      : 
* Note        : CAN receive data make packet
******************************************************************************** 
*/
#if 1
void CAN1_Receive(CAN_TypeDef* CANx, uint8_t FIFONumber, canrxmsg_t* RxMessage)
{
  /* Check the parameters */
  assert_param(IS_CAN_ALL_PERIPH(CANx));
  assert_param(IS_CAN_FIFO(FIFONumber));
  /* Get the Id */
  RxMessage->ide = (uint8_t)0x04 & CANx->sFIFOMailBox[FIFONumber].RIR;
  if (RxMessage->ide == CAN_Id_Standard)
  {
    RxMessage->id = (uint32_t)0x000007FF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 21);
  }
  else
  {
    RxMessage->id = (uint32_t)0x1FFFFFFF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 3);
  }
  
  RxMessage->rtr = (uint8_t)0x02 & CANx->sFIFOMailBox[FIFONumber].RIR;
  /* Get the DLC */
  RxMessage->dlc = (uint8_t)0x0F & CANx->sFIFOMailBox[FIFONumber].RDTR;
  /* Get the FMI */
  RxMessage->fmi = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDTR >> 8);
  /* Get the data field */
  RxMessage->data[0] = (uint8_t)0xFF & CANx->sFIFOMailBox[FIFONumber].RDLR;
  RxMessage->data[1] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 8);
  RxMessage->data[2] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 16);
  RxMessage->data[3] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 24);
  RxMessage->data[4] = (uint8_t)0xFF & CANx->sFIFOMailBox[FIFONumber].RDHR;
  RxMessage->data[5] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 8);
  RxMessage->data[6] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 16);
  RxMessage->data[7] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 24);
  /* Release the FIFO */
  /* Release FIFO0 */
  if (FIFONumber == CAN_FIFO0)
  {
    CANx->RF0R |= CAN_RF0R_RFOM0;
  }
  /* Release FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {
    CANx->RF1R |= CAN_RF1R_RFOM1;
  }
  CAN1_OBD_task(RxMessage);
}
#endif
int CAN_Receive_mk(CAN_TypeDef* CANx, uint8_t FIFONumber, uint8_t* ptarget)
{
	int			plen = 0;
	uint8_t		IDE;
	uint32_t	CanID;
	/* Check the parameters */
	assert_param(IS_CAN_ALL_PERIPH(CANx));
	assert_param(IS_CAN_FIFO(FIFONumber));
	/* Get the Id */
	IDE = (uint8_t)0x04 & CANx->sFIFOMailBox[FIFONumber].RIR;
	if (IDE == CAN_Id_Standard)
	{
		CanID = (uint32_t)0x000007FF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 21);
	}
	else
	{
		CanID = (uint32_t)0x1FFFFFFF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 3);
		//printd("%08X\n",CanID);
	}
   
	ptarget[plen++] = 0x7E;															//SOF
	ptarget[plen++] = 0x7E;															//SOF

	ptarget[plen++] = CanID & 0xFF;													//ID
	ptarget[plen++] = (CanID >>  8) & 0xFF;
	ptarget[plen++] = (CanID >> 16) & 0xFF;
	ptarget[plen++] = (CanID >> 24) & 0xFF;
	ptarget[plen++] = IDE;															//IDE
	ptarget[plen++] = (uint8_t)0x02 & CANx->sFIFOMailBox[FIFONumber].RIR;			//RTR
	ptarget[plen++] = (uint8_t)0x0F & CANx->sFIFOMailBox[FIFONumber].RDTR;			//DLC
	ptarget[plen++] = (uint8_t)0xFF & CANx->sFIFOMailBox[FIFONumber].RDLR;			//data 0
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 8);	//data 1
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 16);	//data 2
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 24);	//data 3
	ptarget[plen++] = (uint8_t)0xFF & CANx->sFIFOMailBox[FIFONumber].RDHR;			//data 4
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 8);	//data 5
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 16);	//data 6
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 24);	//data 7
	ptarget[plen++] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDTR >> 8);	//FMI

	ptarget[plen++] = 0x7F;															//EOF
	ptarget[plen++] = 0x7F;															//EOF

  
  /* Release the FIFO */
  /* Release FIFO0 */
  if (FIFONumber == CAN_FIFO0)
  {
    CANx->RF0R |= CAN_RF0R_RFOM0;
  }
  /* Release FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {
    CANx->RF1R |= CAN_RF1R_RFOM1;
  }
  return plen;
}



/*
*********************************************************************************************************
*********************************************************************************************************
*                                         INTERRUPT SERVICE ROUTINE
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_TIM2_IRQHandler()
*
* Description : Timer2 Interrupt Service Routine 
*
* Note(s)     : 1ms 마다 진입
*********************************************************************************************************
*/

//void TIM2_IRQHandler(void)
//{

	//portENTER_CRITICAL();                      
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	//portEXIT_CRITICAL();
//}


/*
*********************************************************************************************************
*                                            BSP_PVD_IRQHandler()
*
* Description : PVD Interrupt Service Routine 
*
* Note(s)     : AVDD가 2.9V 이하로 떨어지면 진입  
*********************************************************************************************************
*/
#if PDV_ENABLE > 0
void PVD_IRQHandler(void)
{
	printd("I'm dying...\n");
}
#endif

/*
*********************************************************************************************************
*                                            BSP_RTC_IRQHandler()
*
* Description : RTC Interrupt Service Routine 
*
* Note(s)     : 1s 마다 진입
*********************************************************************************************************
*/

void RTC_IRQHandler(void)
{
#if 0
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{		
		/* If counter is equal to 86399: one day was elapsed */
		if(RTC_GetCounter() == 86399)
		{
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
			/* Reset counter value */
			RTC_SetCounter(0x0);
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();

			/* Increment the date */
			//DateUpdate();
		}
	  
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
#endif
}


/*
********************************************************************************
* Description : I2C_Serve
* Arguments   : *I2Cx
* Return      : 
* Note        : I2C2 Service Routine 
******************************************************************************** 
*/

void I2C_Serve(I2C_TypeDef *I2Cx)
{
	/*
     * state machine for the whole I2C events
  */
  switch (I2C_GetLastEvent(I2Cx))
	{
		/*
     * Master Mode
    */
    case I2C_EVENT_MASTER_MODE_SELECT:
		/*
     * we want to transmit a single byte
     */
      if(I2C_DIR == TX) {
				I2C_Send7bitAddress(I2Cx, I2CADDRESS, I2C_Direction_Transmitter);
        /*
         * we want to read a register value
         * first step is TRANSMITTING the register address then RECEIVE the data
         */
      }
			else if(I2C_DIR == TXREG) {
				I2C_Send7bitAddress(I2Cx, I2CADDRESS, I2C_Direction_Transmitter);
        /*
         * we want to receive something
         */
      }
			else if(I2C_DIR == RX) {
				I2C_Send7bitAddress(I2Cx, I2CADDRESS, I2C_Direction_Receiver);
      }
      // address sent, if no byte should be sent: request NAK now!
      if( tx_buffer_len == 0 ) {
        // request NAK
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        // request stop condition
        I2C_GenerateSTOP(I2Cx, ENABLE);
      }                        
      break;
      /*
       * we get here after transmitting address + write bit
       */
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
		case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
			if( tx_buffer_ix < tx_buffer_len ) {
				I2C_SendData(I2Cx, tx_buffer[tx_buffer_ix++]);
				//printd("0x[%02X]",tx_buffer[tx_buffer_ix-1]);
      }  
      else {
				if(I2C_DIR == TX) {
				// send stop condition
           I2C_GenerateSTOP(I2Cx, ENABLE);
           I2C_BLOCKED = 0;
         }
         else if(I2C_DIR == TXREG) {
          /*
           * Generate second start and switch to receiver mode
           */
           I2C_DIR = RX;
           I2C_GenerateSTART(I2Cx,ENABLE);
         }
			}
			break;
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
			break;
		case I2C_EVENT_MASTER_MODE_ADDRESS10:
			break;                                  
        /* 
         * we received a byte...
        */
		case I2C_EVENT_MASTER_BYTE_RECEIVED:
      /* Store I2C received data */
      rx_buffer_ptr[rx_buffer_ix++] = I2C_ReceiveData(I2Cx);
      /* Request NACK and send I2C1 STOP condition before receiving the last data */
      if ((rx_buffer_ix == rx_buffer_len - 1) || rx_buffer_len == 1) {
        /* Request ACK and send I2C STOP condition before receiving the last data */
        /* Request NACK */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        /* Send I2C STOP Condition */
        I2C_GenerateSTOP(I2Cx, ENABLE);      
			}
      if (rx_buffer_ix == rx_buffer_len) {
				I2C_BLOCKED = 0;
      }
			break;
			/*
      * we switched the mode
      */
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
      if(rx_buffer_len == 1) {
				/* Request NACK */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        /* Send I2C STOP Condition */
        I2C_GenerateSTOP(I2Cx,ENABLE);
      }
			break;
	}
}


/*
********************************************************************************
* Description : I2C1_EV_IRQHandler
* Arguments   : none
* Return      : 
* Note        : I2C1 Service Routine 
******************************************************************************** 
*/

void I2C1_EV_IRQHandler(void)
{
  I2C_Serve(I2C1);
}


/*
********************************************************************************
* Description : I2C1_ER_IRQHandler
* Arguments   : none
* Return      : 
* Note        : I2C1 Error Service Routine 
******************************************************************************** 
*/

void I2C1_ER_IRQHandler(void)
{
  if (I2C_GetITStatus(I2C1, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
  }
}


/*
*********************************************************************************************************
*                                            I2C2_EV_IRQHandler()
*
* Description : I2C1 Service Routine 
*
* Note(s)     : 
*********************************************************************************************************
*/
void I2C2_EV_IRQHandler(void)
{
        I2C_Serve(I2C2);
}
/*
*********************************************************************************************************
*                                            I2C2_ER_IRQHandler()
*
* Description : I2C1 Error Service Routine 
*
* Note(s)     : 
*********************************************************************************************************
*/
void I2C2_ER_IRQHandler(void)
{
  if (I2C_GetITStatus(I2C2, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
  }
}

/*
*********************************************************************************************************
*                                            CAN1_RX0_IRQHandler()
*
* Description : This function handles CAN1 RX0 request. 
*
* Note(s)     : 
*********************************************************************************************************
*/

void CAN1_RX0_IRQHandler(void)
{
  canrxmsg_t CAN1_RxMessage;
  
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0))
	{
//		__sub_Can_RxInt(CAN1, CAN1_PORT);
    CAN1_Receive(CAN1, CAN_FIFO0, &CAN1_RxMessage);
    
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);    
	}
}

/*
*********************************************************************************************************
*                                            CAN1_RX0_IRQHandler()
*
* Description : This function handles CAN1 RX0 request. 
*
* Note(s)     : 
*********************************************************************************************************
*/

void CAN2_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0))
	{
		__sub_Can_RxInt(CAN2, CAN2_PORT);
    
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);    
	}
}

void TIM1_CC_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
}

/*
*********************************************************************************************************
*                                            TIM2_IRQHandler()
*
* Description : This function handles Time2. 
*
* Note(s)     : 
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)  //0.05msecond
{

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	}
}

/*
*********************************************************************************************************
*                                            EXTI15_10_IRQHandler()
*
* Description : This function handles EXT19_5. 
*
* Note(s)     : 
*********************************************************************************************************
*/
void EXTI3_IRQHandler(void)
{
	sysinfo_t *s = &_sysinfo 	;

	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //SPEED
	{
		s->key.keyon_flag = 1;

		EXTI_ClearITPendingBit(EXTI_Line3);   
	}
}

/*
*********************************************************************************************************
*                                            DMA2_Stream4_IRQHandler()
*
* Description : This function handles ADC3 DMA. 
*
* Note(s)     : 
*********************************************************************************************************
*/
void DMA2_Stream4_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream4, DMA_FLAG_TCIF4);

}

void ADC_IRQHandler(void)
{
	//ADC1ConvertedVoltage= ADC_GetConversionValue(ADC1);
	adcdata[ADC_AVR_MAF_TEMP]		= ext_runningAverageADC(ADC_MAF_TEMP, ADC_GetConversionValue(ADC1) );
	ADC_ClearITPendingBit (ADC1, ADC_IT_EOC);
	printd(".");
}


/*
*********************************************************************************************************
*                                            BSP_USART1_IRQHandler()
*
* Description : USART1 Interrupt Service Routine 
*
* Note(s)     : 
*********************************************************************************************************
*/

void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		__sub_Uart_RxInt(0);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		__sub_Uart_TxInt(0);
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	// Over Run Error Clear
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_ORE);    
		USART_ReceiveData(USART1);
	}
}

/*
*********************************************************************************************************
*                                            BSP_USART2_IRQHandler()
*
* Description : USART2 Interrupt Service Routine 
*
* Note(s)     : 
*********************************************************************************************************
*/

void USART2_IRQHandler(void)
{
  INT16U uart2_head;
  uint8_t rdata;
  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		  /* Receive Data */
    rdata = (uint16_t)(USART2->DR & (uint16_t)0x01FF);
    
    // Calculate buffer index
    uart2_head = ( UART_RxHead + 1 ) & UART_RX_BUFFER_MASK;
    UART_RxHead = uart2_head;      // Store new index

    if ( uart2_head != UART_RxTail )
    {
      UART_RxBuf[uart2_head] = rdata; // Store received data in buffer       
    }
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }

	if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
	{
		__sub_Uart_TxInt(GUI_PORT);
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	// Over Run Error Clear
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(USART2, USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}
}

/*
*********************************************************************************************************
*                                            BSP_USART3_IRQHandler()
*
* Description : USART3 Interrupt Service Routine 
*
* Note(s)     : 
*********************************************************************************************************
*/

void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		__sub_Uart_RxInt(2);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART3, USART_IT_TC) == SET)
	{
		__sub_Uart_TxInt(2);
		USART_ClearITPendingBit(USART3, USART_IT_TC);
	}
	// Over Run Error Clear
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_ORE);    
		USART_ReceiveData(USART3);
	}	
}

/*
*********************************************************************************************************
*                                            BSP_USART3_IRQHandler()
*
* Description : UART4 Interrupt Service Routine 
*
* Note(s)     : 
*********************************************************************************************************
*/

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		__sub_Uart_RxInt(DBG_PORT);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
	if(USART_GetITStatus(UART4, USART_IT_TC) == SET)
	{
		__sub_Uart_TxInt(DBG_PORT);
		USART_ClearITPendingBit(UART4, USART_IT_TC);
	}
	// Over Run Error Clear
	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(UART4, USART_FLAG_ORE);    
		USART_ReceiveData(UART4);
	}	
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            EXTERN API FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

void BSP_LED_Toggle(INT8U led)
{
    CPU_INT32U pins;

    pins = GPIO_ReadOutputData(GPIOF);

    switch(led){
        case 0:
             BSP_LED_Toggle(1);
             BSP_LED_Toggle(2);
             break;

        case 1:
             if ((pins & GPIO_Pin_0) == 0) {
                 GPIO_SetBits(  GPIOF, GPIO_Pin_0);
             } else {
                 GPIO_ResetBits(GPIOF, GPIO_Pin_0);
             }
            break;		

        case 2:
             if ((pins & GPIO_Pin_1) == 0) {
                 GPIO_SetBits(  GPIOF, GPIO_Pin_1);
             } else {
                 GPIO_ResetBits(GPIOF, GPIO_Pin_1);
             }
            break;

        default:
             break;
    }
}


void system_power_off(void)
{
	POWER_DISABLE();
  
  for(;;) {
      __NOP();
    }  
}


/*
*********************************************************************************************************
*                                            DEBUG FUNCTIONS
*********************************************************************************************************
*/


portCHAR TaskListBuf[512];
INT16U dbg_tasklist(void)
{
	memset(TaskListBuf, 0,512);
	vTaskList((signed char *)(TaskListBuf + strlen(TaskListBuf)));
	printk("\nName          State  Priority  Stack   Num\n" );
	printk("---------------------------------------------\n");
	printk("%s\n",TaskListBuf);
	printk("---------------------------------------------\n");
	printk("B : Blocked, R : Ready, D : Deleted, S : Suspended\n");


	return NO_DISPLAY;
}

// 시스템 리셋 
INT16U dbg_system_reset(void)
{
	//USB_Cable_Config(DISABLE);
	vTaskEndScheduler();
	NVIC_SystemReset();
	return NO_DISPLAY;
}


// 마지막 펌웨어 업데이트
INT16U dbg_ver_rollup(void)
{
	memcpy((char*)tPrjcode.boot_seq,"download",8);
	SPI_FLASH_PageErase(PROJECT_BASE);
	SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));	

	NVIC_SystemReset();
	return NO_DISPLAY;	
}


// 다운로드 이전 펌웨어 업데이트 
INT16U dbg_ver_rollback(void)
{
	memcpy((char*)tPrjcode.boot_seq,"rollback",8);
	SPI_FLASH_PageErase(PROJECT_BASE);
	SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));

	NVIC_SystemReset();
	return NO_DISPLAY;
}

/*		PIN INPUT FUNCTIONS		*/

//#define INPUTDI0()		GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)
//#define INPUTDI1()		GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9)
INT16U dbg_inputpin_Read(void)
{
	printk("\nDI 0 = [%s]\n", INPUTDI0() ? "HIGH":"LOW");
	printk("DI 1 = [%s]\n", INPUTDI1() ? "HIGH":"LOW");
	return NO_DISPLAY;
}

/*		PIN OUTPUT FUNCTIONS		*/
//#define OUTPUT_DO(val)		GPIO_WriteBit(GPIOG,  GPIO_Pin_8, val)
//#define OUTPUT_LINE(val)	GPIO_WriteBit(GPIOG,  GPIO_Pin_7, val)
//#define OUTPUT_MOTOR(val)	GPIO_WriteBit(GPIOG,  GPIO_Pin_6, val)
//#define OUTPUT_SOL(val)		GPIO_WriteBit(GPIOG,  GPIO_Pin_5, val)
//#define OUTPUT_TANK(val)	GPIO_WriteBit(GPIOG,  GPIO_Pin_4, val)
INT16U dbg_supply_Ctrol(void)
{
	int val;
	//BitAction val;

	if(CLI_GetArgCnt() < 1){
		return NO_DISPLAY;
	}
	val = comparison_num((char *)P_ARG(1)) &0x1;

	OUTPUT_DO( val );
	printk("\nOutput Ctrl DO0 = %s\n", val ? "HIGH":"LOW");
	return NO_DISPLAY;
}

INT16U dbg_line_Ctrol(void)
{
	int val;
	//BitAction val;

	if(CLI_GetArgCnt() < 1){
		return NO_DISPLAY;
	}
	val = comparison_num((char *)P_ARG(1)) &0x1;
	OUTPUT_POWER(val);
	printk("\nOutput Ctrl Line = %s\n", val ? "HIGH":"LOW");
	return NO_DISPLAY;
}

INT16U dbg_motor_Ctrol(void)
{
	int val;

	if(CLI_GetArgCnt() < 1){
		return NO_DISPLAY;
	}
	val = comparison_num((char *)P_ARG(1)) &0x1;
	OUTPUT_MOTOR(val);
  SysSts.supply.Duty_Cycle = 15;
  if(val) StartPwm();
  else EndPwm();
	printk("\nOutput Ctrl Motor = %s\n", val ? "HIGH":"LOW");
	return NO_DISPLAY;
}

INT16U dbg_sol_Ctrol(void)
{
	int val;

	if(CLI_GetArgCnt() < 1){
		return NO_DISPLAY;
	}
	val = comparison_num((char *)P_ARG(1)) &0x1;
	OUTPUT_SOL(val );
	printk("\nOutput Ctrl SOL = %s\n", val ? "HIGH":"LOW");
	return NO_DISPLAY;
}

INT16U dbg_tank_Ctrol(void)
{
	int val;

	if(CLI_GetArgCnt() < 1){
		return NO_DISPLAY;
	}
	val = comparison_num((char *)P_ARG(1)) &0x1;
	OUTPUT_DOSING(val );
	printk("\nOutput Ctrl TANK = %s\n", val ? "HIGH":"LOW");
	return NO_DISPLAY;
}




/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

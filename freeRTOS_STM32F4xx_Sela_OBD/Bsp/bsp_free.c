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

static void BSP_CAN1_Init(void);      // for car

static void BSP_EXTI_Init(void);      // key on

static void BSP_USART1_Init(void);    // cdma modem

static void BSP_USART2_Init(void);	  // gps

static void BSP_ADC1_Init(void);      // pressure

static void BSP_I2C1_Init(void);      // fram

static void BSP_SPI1_Init(void);			// serial flash

static void BSP_SPI2_Init(void);			// temperature

static void BSP_TIM2_Init(void);			// Fnd Timer

//------------------------------------------------------------------ Interrupt Service Routine Function

#if PDV_ENABLE > 0
void PVD_IRQHandler(void);
#endif
void RTC_WKUP_IRQHandler(void);

void I2C1_EV_IRQHandler(void);

void I2C1_ER_IRQHandler(void);

void CAN1_RX0_IRQHandler(void);

void EXTI9_5_IRQHandler(void);

void USART1_IRQHandler(void);

void USART2_IRQHandler(void);

void TIM2_IRQHandler(void);

extern __IO I2C_Dir I2C_DIR;
extern __IO uint8_t I2C_BLOCKED;
extern __IO uint8_t I2CADDRESS;
extern __IO uint8_t rx_buffer_len;
extern __IO uint8_t tx_buffer_len;
extern __IO uint8_t tx_buffer_ix;
extern __IO uint8_t rx_buffer_ix;
extern __IO uint8_t tx_buffer[];
extern __IO uint8_t *rx_buffer_ptr;

const unsigned long CAN_MON_ID[MAX_CAN_MON_NO] = {0x0000000a, 0x18F00E51, 0x18F00F52, 0x18FFDF00, 0x18FE563D,
                                                  0x18FECA3D,0x18FDB43D, 0x18FDB33D, 0x0CF00A00, 0x18FFDE3D};

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
********************************************************************************
* Description : BSP_Lowlevel_Init
* Arguments   : none
* Return      : 
* Note        : Initialize the Board Support Package (BSP).
******************************************************************************** 
*/

void BSP_Lowlevel_Init(void)
{
	
	RCC_DeInit();

  RCC_HSEConfig(RCC_HSE_ON);

	SystemInit();	

	BSP_NVIC_Init();

	BSP_GPIO_Init();
  
	BSP_RTC_Init();       // interanl rtc
  
  BSP_I2C1_Init();      // fram
    
	BSP_USART1_Init();    // gps

  BSP_USART2_Init();    // modem control
    
	BSP_SPI1_Init();      // serial flash
	
	BSP_SPI2_Init();      // temperature

	BSP_ADC1_Init();      // adc dma set

  BSP_EXTI_Init();      // key on

  BSP_TIM2_Init();      // Time2
  
#if WATCHDOG_ENABLE > 0
	BSP_IWDG_Init();      // watchdog
#endif

#if SCR_USE_NAND > 0
	FSMC_NAND_Init();     // nand
#endif
  
	SD_DeInit();          // sdcard

}

/*
********************************************************************************
* Description : BSP_CAN_Init
* Arguments   : none
* Return      : 
* Note        : Initialize the Board Support Package (BSP).
******************************************************************************** 
*/

void BSP_CAN_Init(void)
{
	BSP_CAN1_Init();      // for car
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

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
  
	/* Enable the CAN1 Interrupt */  
	NVIC_InitStructure.NVIC_IRQChannel					         = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 6;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	/* Enable the PVD event interrupt */
#if PDV_ENABLE > 0
	NVIC_InitStructure.NVIC_IRQChannel                   = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
#endif

	NVIC_InitStructure.NVIC_IRQChannel					         = I2C1_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = I2C1_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					         = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;
  
	NVIC_InitStructure.NVIC_IRQChannel					         = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority		     = 8;
  NVIC_InitStructure.NVIC_IRQChannelCmd				         = ENABLE;  
    
  NVIC_Init(&NVIC_InitStructure);
}


/*
********************************************************************************
* Description : BSP_IWDG_Init
* Arguments   : none
* Return      : 
* Note        : Initialize the board's Independent watch-dog 
******************************************************************************** 
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

	/* IWDG counter clock: 32KHz(LSI) / 32 = 1.00 KHz(1ms) */
	IWDG_SetPrescaler(IWDG_Prescaler_64);

	/* Set counter reload value to 2500(*0.8ms) */
	IWDG_SetReload(3000);	// 6초 셋팅

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}


/*
********************************************************************************
* Description : BSP_RTC_Init
* Arguments   : none
* Return      : 
* Note        : Initialize the board's RTC (32.768KHz)
******************************************************************************** 
*/

#define RTC_CLOCK_SOURCE_LSE
static void BSP_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
  EXTI_InitTypeDef EXTI_InitStructure; 

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,     ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

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

  if((RTC->ISR & RTC_ISR_INITS) == RESET) {
    /* Calendar Configuration with LSI supposed at 32KHz */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv	=  0xFF; /* (32KHz / 128) - 1 = 0xFF*/
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);  
  } 
  
  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line22);
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
 
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
 
  /* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
  RTC_SetWakeUpCounter(0x0);
 
  RTC_ClearITPendingBit(RTC_IT_WUT); 
  EXTI_ClearITPendingBit(EXTI_Line22); 
  
  /* Enable the RTC Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE); 
}


/*
********************************************************************************
* Description : BSP_PVD_Init
* Arguments   : none
* Return      : 
* Note        : Initialize the board's Power voltage detect
******************************************************************************** 
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
********************************************************************************
* Description : BSP_GPIO_Init
* Arguments   : none
* Return      : 
* Note        : General Purpose I/O Initalize
******************************************************************************** 
*/

static void BSP_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

// PORTA Configure
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;    //Charge
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //Default Value Set
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);		//Charge On
  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;      //Power Check
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

// PORTB Configure
	GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_0    //buzzer
									| GPIO_Pin_1;	                  //I/O Out : DO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Default Value Set
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);		//BUZZER Off
  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;     //Button Up
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

// PORTC Configure    
	GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_1    //FND A
									| GPIO_Pin_2                    //FND B
									| GPIO_Pin_3                    //FND C
									| GPIO_Pin_4                    //FND D
									| GPIO_Pin_5                    //FND E
									| GPIO_Pin_6                    //FND F
									| GPIO_Pin_7                    //FND G
									| GPIO_Pin_13;                  //FND DP
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  //Default Value Set 
	GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

// PORTD Configure 
	GPIO_InitStructure.GPIO_Pin   =	  GPIO_Pin_13   //FND 1
		              | GPIO_Pin_3                    //FND 2
									| GPIO_Pin_8                    //FND 3
									| GPIO_Pin_9                    //FND 4
									| GPIO_Pin_10;                  //FND 5
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

// PORTE Configure  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1      //Power LED  
                  | GPIO_Pin_4		                //Main Power Enable pin
									| GPIO_Pin_11		                //LED SD Card status
                  | GPIO_Pin_12		                //LED Normal  
									| GPIO_Pin_13		                //LED Temperature
									| GPIO_Pin_14		                //LED Press Low
                  | GPIO_Pin_15;                  //LED Press High
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  //Default Value Set
  GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);		  //Power LED
	GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_SET);			//Power on Enable
	GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_SET);			//LED SD Card status
	GPIO_WriteBit(GPIOE, GPIO_Pin_13, Bit_SET);			//LED Temp
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);			//LED Press Low
	GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_SET);			//LED Press High
  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0      //Button mode
                  | GPIO_Pin_2                    //Push Key
                  | GPIO_Pin_3                    //SD Card Detection
                  | GPIO_Pin_5                    //Button set
                  | GPIO_Pin_6;			              //Key On Detection
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

}


/*
********************************************************************************
* Description : BSP_EXTI_Init
* Arguments   : none
* Return      : 
* Note        : External Int (for keyon)
******************************************************************************** 
*/
static void BSP_EXTI_Init(void)
{

  GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
  
	/* Enable SYSCFG clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);

	EXTI_InitStructure.EXTI_Mode	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Line	= EXTI_Line6;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_Init(&EXTI_InitStructure);
}


/*
********************************************************************************
* Description : BSP_I2C1_Init
* Arguments   : none
* Return      : 
* Note        : I2C1 for Fram
******************************************************************************** 
*/
static void BSP_I2C1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;

  I2C_DeInit(I2C1);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);		    //Enable APB1 peripheral clock for I2C1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		    //Enable clock for SCL and SDA pins

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;			    //Use PB6 for SCL and PB7 for SDA
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                   //Set pins to alternate function
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				      //Set GPIO speed
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;                 //Set output to open drain --> the line has to be only pulled low, not driven high
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                   //Enable pull up resistors
  GPIO_Init(GPIOB, &GPIO_InitStruct);                         //Initialise GPIOB

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);     //Connect I2C1 SCL pin to alternate function
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);     //Connect I2C1 SDA pin to alternate function
  
  I2C_StructInit(&I2C_InitStruct);
                                                                   
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                     //I2C mode
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;             //50% duty cycle --> standard
  I2C_InitStruct.I2C_OwnAddress1 = 0x2A;                      //Own address, not relevant in master mode
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                    //Enable acknowledge when reading (can be changed later on)
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //Set address length to 7 bit addresses
  I2C_InitStruct.I2C_ClockSpeed = 100000;									    //100kHz
  I2C_Init(I2C1, &I2C_InitStruct);                            //Initialise I2C1

  I2C_Cmd(I2C1, ENABLE);                                      //Enable I2C1
  I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
}


/*
********************************************************************************
* Description : BSP_CAN1_Init
* Arguments   : none
* Return      : 
* Note        : CAN for Automotive
******************************************************************************** 
*/

static void BSP_CAN1_Init(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); //Connect CAN1 RX pin to alternate function
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); //Connect CAN1 TX pin to alternate function

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
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
    
  CAN_InitStructure.CAN_Prescaler=4;        //  250 kbit/s
  
  #if 0
CAN_InitStructure.CAN_Prescaler=1; // 2000 kbit/s
CAN_InitStructure.CAN_Prescaler=2; // 1000 kbit/s
CAN_InitStructure.CAN_Prescaler=4; // 500 kbit/s
CAN_InitStructure.CAN_Prescaler=5; // 400 kbit/s
CAN_InitStructure.CAN_Prescaler=8; // 250 kbit/s
CAN_InitStructure.CAN_Prescaler=10; // 200 kbit/s
CAN_InitStructure.CAN_Prescaler=16; // 125 kbit/s
CAN_InitStructure.CAN_Prescaler=20; // 100 kbit/s
CAN_InitStructure.CAN_Prescaler=40; // 50 kbit/s
CAN_InitStructure.CAN_Prescaler=80; // 40 kbit/s
CAN_InitStructure.CAN_Prescaler=200; // 10 kbit/s
CAN_InitStructure.CAN_Prescaler=1023; // ganz langsam
#endif
	
	CAN_Init(CAN1, &CAN_InitStructure);
	/* CAN filter init */
	
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  
  CAN_FilterInitStructure.CAN_FilterNumber        = 1;						
  CAN_FilterInitStructure.CAN_FilterIdHigh		    = (u16)(0x0010<<5);  // 0x010 ~ 0x013 수신
  CAN_FilterInitStructure.CAN_FilterIdLow			    = (u16)(0x0098<<5);  // 0x098 ~ 0x099 수신
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	  = (u16)(0x07FC<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		  = (u16)(0x07FE<<5);
	CAN_FilterInit(&CAN_FilterInitStructure);
  
	CAN_FilterInitStructure.CAN_FilterNumber        = 2;						
  CAN_FilterInitStructure.CAN_FilterIdHigh		    = (u16)(0x0100<<5);  // x100 ~ 0x1FF 수신
  CAN_FilterInitStructure.CAN_FilterIdLow			    = (u16)(0x0100<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh	  = (u16)(0x0700<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow		  = (u16)(0x0700<<5);
	CAN_FilterInit(&CAN_FilterInitStructure);
  
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


/*
********************************************************************************
* Description : BSP_SPI1_Init
* Arguments   : none
* Return      : 
* Note        : SPI1 for Serial Flash.
******************************************************************************** 
*/

static void BSP_SPI1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	
	/*!< Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/*!< Enable the SPI clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* Configure SPI pin: SCK*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure SPI pin: MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure SPI pin: MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable alternate function for SPI pins */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
 
	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;		//SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;	//SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}


/*
********************************************************************************
* Description : BSP_SPI2_Init
* Arguments   : none
* Return      : 
* Note        : SPI2 for Temperature
******************************************************************************** 
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
	
	/* Configure I/O for Temperature Chip select */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Deselect the Temperature: Chip Select high */
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
 
	/* SPI2 configuration */
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE);
}


/*
********************************************************************************
* Description : BSP_USART1_Init
* Arguments   : none
* Return      : 
* Note        : USART1 for MODEM.
******************************************************************************** 
*/
static void BSP_USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable USART1, GPIOA clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

	// Connect PXx to USART1_Tx 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	// Connect PXx to USART1_Rx 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	 /* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* USART configured as follow */
	USART_InitStructure.USART_BaudRate            = 9600;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);

	//BSP_IntVectSet(BSP_INT_ID_USART3,BSP_USART3_IRQHandler);
	
	/* Enable USART1 Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);

	/* Enable USART1 Receive interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

/*
********************************************************************************
* Description : BSP_UART1RX_IntEnable
* Arguments   : none
* Return      : 
* Note        : USART1 rx intterupt eanble
******************************************************************************** 
*/

void BSP_UART1RX_IntEnable(FunctionalState state)
{
	USART_ITConfig(USART1, USART_IT_RXNE, state);
}

/*
********************************************************************************
* Description : BSP_USART2_Init
* Arguments   : none
* Return      : 
* Note        : USART2 for MODEM.
******************************************************************************** 
*/

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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	 /* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
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

/*
********************************************************************************
* Description : BSP_USART2RX_IntEnable
* Arguments   : none
* Return      : 
* Note        : USART2 rx intterupt eanble
******************************************************************************** 
*/
void BSP_UART2RX_IntEnable(FunctionalState state)
{
	USART_ITConfig(USART2, USART_IT_RXNE, state);
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
	DMA_DeInit(DMA2_Stream0);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &adcdata[ADC_P0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
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
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
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

static void BSP_ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	BSP_ADC1_DMA_Init();

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;			//ADC123_IN10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_Mode			      = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler		    = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode	  = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay= ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution			      = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode			    = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode	  = ENABLE;

	ADC_InitStructure.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign				      = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion		    = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
  
  BSP_ADC1ConStart();  
}

/*
********************************************************************************
* Description : BSP_TIM2_Init
* Arguments   : none
* Return      : 
* Note        : TIMER 2 init
******************************************************************************** 
*/
static void BSP_TIM2_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  /* ---------------------------------------------------------------
  TIM2 Configuration: Time base: 500Hz (2ms)
  - TIM2CLK = 84 MHz, Prescaler = 84-1
  - PrescalerValue = TIM2CLK / (TIM_Period + 1) = 42,000
  - Time base = PrescalerValue / (Prescaler + 1) = 500
  42,000 / 84 = 500 (2ms)
  --------------------------------------------------------------- */
  /* TIM clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 32bit timer

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
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

void CAN_Receive_mk(CAN_TypeDef* CANx, uint8_t FIFONumber, canrxmsg_t* RxMessage)
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
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                         INTERRUPT SERVICE ROUTINE
*********************************************************************************************************
*********************************************************************************************************
*/

/*
********************************************************************************
* Description : PVD_IRQHandler
* Arguments   : none
* Return      : 
* Note        : PVD Interrupt Service Routine (less than 2.9v)
******************************************************************************** 
*/

#if PDV_ENABLE > 0
void PVD_IRQHandler(void)
{
	printd("I'm dying...\n");
}
#endif

/*
********************************************************************************
* Description : RTC_WKUP_IRQHandler
* Arguments   : none
* Return      : 
* Note        : RTC Wakeup global interrupt request.
******************************************************************************** 
*/
void RTC_WKUP_IRQHandler(void)
{
	sysinfo_t *s = &_sysinfo;
  
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET) {
    if(!Flags.StartFlag) {
      GetRTCTime(&s->tm_ptr);
      if(Start_Flag) {
        SaveCnt++;
        if(SaveCnt >= SENDTIME) {
          SaveCnt = 0;
          Flags.SaveDataFlag = 1;
        }
      }
    }
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line22);
  } 
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
*                                            CAN1_RX0_IRQHandler()
*
* Description : This function handles CAN1 RX0 request. 
*
* Note(s)     : 
*********************************************************************************************************
*/
void CAN1_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0))
	{
    CAN_Receive_mk(CAN1, CAN_FIFO0, &Can_RxMsg);
    
    CAN_Decode();
    
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}
}


/*
********************************************************************************
* Description : EXTI9_5_IRQHandler
* Arguments   : none
* Return      : 
* Note        : EXTI9_5 Interrupt Service Routine 
******************************************************************************** 
*/

void EXTI9_5_IRQHandler(void)
{
	//sysinfo_t *s = &_sysinfo 	;

	if(EXTI_GetITStatus(EXTI_Line6) != RESET) //Key
	{
		//s->key.keyon_flag = 1;

		EXTI_ClearITPendingBit(EXTI_Line6);   
	}
}


/*
********************************************************************************
* Description : USART1_IRQHandler
* Arguments   : none
* Return      : 
* Note        : USART1 Interrupt Service Routine 
******************************************************************************** 
*/

void USART1_IRQHandler(void)
{
  uint8_t rdata;
  uint16_t uart1_head;
  
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
	
	  rdata = (uint8_t)(USART1->DR & (uint8_t)0x00FF);

    uart1_head = (GPS_RxHead + 1) & GPS_RX_BUFFER_MASK;
    GPS_RxHead = uart1_head;      // Store new index

    if (uart1_head != GPS_RxTail )
      GPS_RxBuf[uart1_head] = rdata; // Store received data in buffer 
    
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

	if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	// Over Run Error Clear
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(USART1, USART_FLAG_ORE);
		USART_ReceiveData(USART1);
	}

}


/*
********************************************************************************
* Description : USART2_IRQHandler
* Arguments   : none
* Return      : 
* Note        : USART2 Interrupt Service Routine 
******************************************************************************** 
*/

void USART2_IRQHandler(void)
{
  uint8_t rdata;  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
	  rdata = (uint8_t)(USART2->DR & (uint8_t)0x00FF);
    ModemCommPutRxChar(rdata);    
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}

	if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
	{
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
********************************************************************************
* Description : TIM2_IRQHandler
* Arguments   : none
* Return      : 
* Note        : Timer2 Interrupt Service Routine 
******************************************************************************** 
*/
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
  	fnd_callback_fun();
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            EXTERN API FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
********************************************************************************
* Description : BSP_LED_Toggle
* Arguments   : led control
* Return      : 
* Note        : LED Toggle.. system alive check
******************************************************************************** 
*/

void BSP_LED_Toggle(INT8U led)
{
    CPU_INT32U pins;

    pins = GPIO_ReadOutputData(GPIOE);

    switch(led){
        case 0:
             BSP_LED_Toggle(1);
             break;

        case 1:
             if ((pins & GPIO_Pin_1) == 0) {
                 GPIO_SetBits(  GPIOE, GPIO_Pin_1);
             } else {
                 GPIO_ResetBits(GPIOE, GPIO_Pin_1);
             }
            break;		

        default:
             break;
    }
}


/*
********************************************************************************
* Description : system_power_off
* Arguments   : None
* Return      : 
* Note        : system power off
******************************************************************************** 
*/

void system_power_off(void)
{
	GPIO_ResetBits(  GPIOE, GPIO_Pin_4 );
}


/*
*********************************************************************************************************
*                                            DEBUG FUNCTIONS
*********************************************************************************************************
*/

/*
********************************************************************************
* Description : dbg_tasklist
* Arguments   : none
* Return      : 
* Note        : RTOS task list display
******************************************************************************** 
*/
portCHAR TaskListBuf[512];
INT16U dbg_tasklist(void)
{
	memset(TaskListBuf, 0,512);
	vTaskList((char *)(TaskListBuf + strlen(TaskListBuf)));
	printk("\nName          State  Priority  Stack   Num\n" );
	printk("---------------------------------------------\n");
	printk("%s\n",TaskListBuf);
	printk("---------------------------------------------\n");
	printk("B : Blocked, R : Ready, D : Deleted, S : Suspended\n");

	return NO_DISPLAY;
}


/*
********************************************************************************
* Description : dbg_system_reset
* Arguments   : none
* Return      : 
* Note        : 시스템 리셋 
******************************************************************************** 
*/

INT16U dbg_system_reset(void)
{
	vTaskEndScheduler();
	NVIC_SystemReset();
	return NO_DISPLAY;
}


/*
********************************************************************************
* Description : dbg_ver_rollup
* Arguments   : none
* Return      : 
* Note        : 마지막 펌웨어 업데이트
******************************************************************************** 
*/

INT16U dbg_ver_rollup(void)
{
	memcpy((char*)tPrjcode.boot_seq,"download",8);
	SPI_FLASH_PageErase(PROJECT_BASE);
	SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));	

	NVIC_SystemReset();
	return NO_DISPLAY;	
}


/*
********************************************************************************
* Description : dbg_ver_rollback
* Arguments   : none
* Return      : 
* Note        : 다운로드 이전 펌웨어 업데이트 
******************************************************************************** 
*/

INT16U dbg_ver_rollback(void)
{
	memcpy((char*)tPrjcode.boot_seq,"rollback",8);
	SPI_FLASH_PageErase(PROJECT_BASE);
	SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));

	NVIC_SystemReset();
	return NO_DISPLAY;
}


/*
********************************************************************************
* Description : dbg_rtc_setTest
* Arguments   : none
* Return      : 
* Note        : RTC time set
******************************************************************************** 
*/
INT16U dbg_rtc_setTest(void)
{
	sysinfo_t *s = &_sysinfo 	;
	INT32U yy,mon,day,hh,mm,ss,wday;

	if(CLI_GetArgCnt() < 7) return INV_PRM;
	
	yy  = comparison_num((char *)P_ARG(1));
	mon = comparison_num((char *)P_ARG(2));
	day= comparison_num((char *)P_ARG(3));
	hh  = comparison_num((char *)P_ARG(4));
	mm  = comparison_num((char *)P_ARG(5));
	ss  = comparison_num((char *)P_ARG(6));
	wday= comparison_num((char *)P_ARG(7));

	MakeTimePtr(&s->tm_ptr,yy, mon, day, hh, mm, ss,wday);

	SetRTCTime(&s->tm_ptr);

	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_rtc_gettimeTest
* Arguments   : none
* Return      : 
* Note        : RTC time read
******************************************************************************** 
*/
INT16U dbg_rtc_gettimeTest(void)
{

	sysinfo_t *s = &_sysinfo 	;

	printk("Read RTC Sec = %s\n",btime2str(&s->tm_ptr, "y-m-d h:mi:s"));

    return NO_DISPLAY;

}

/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

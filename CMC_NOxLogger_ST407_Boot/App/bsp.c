/******************** (C) COPYRIGHT 2014  ******************************
* File Name          : bsp.c
* Author             : Digital Development Team
* Version            : V1.0.0
* Date               : 08/24/2014
* Description        : board configuration
* Programer			 : MKH
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#define BSP_GLOBALS
#include "includes.h"



/* Private function prototypes -----------------------------------------------*/
static void BSP_USART_Init(void);
static void BSP_SPI_Init(void);
extern FATFS SD_Fs;
extern FIL	 SD_Fil;


/* Private functions ---------------------------------------------------------*/
static void BSP_USART_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable USART1, GPIOA, and AFIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);          
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);



	//////////////////////////////////////////////////////
 
	/* USART configured as follow */
	//USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate            = 9600;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);
		
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

static void BSP_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
		
	/*!< Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/*!< Enable the SPI clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
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
	//SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}

static void BSP_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

// PORTB Configure
	GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_0    //buzzer
									| GPIO_Pin_1;	                  //I/O Out : DO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
// Default Value Set  
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
// Default Value Set 
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
// Default Value Set
  GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);		  //Power LED
	GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_SET);			//Power on Enable
	GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_SET);			//LED SD Card status
	GPIO_WriteBit(GPIOE, GPIO_Pin_13, Bit_SET);			//LED Temp
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);			//LED Press Low
	GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_SET);			//LED Press High
  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0      //Button Set
                  | GPIO_Pin_2                    //Push Key
                  | GPIO_Pin_3                    //SD Card Detection
                  | GPIO_Pin_5                    //Button mode
                  | GPIO_Pin_6;			              //Key On Detection
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/* Global functions ----------------------------------------------------------*/  
void BSP_Init(void)
{
	BSP_GPIO_Init();
	BSP_USART_Init();
	BSP_SPI_Init();
	SD_DeInit();
}

void BSP_Deinit(void)
{
	USART_DeInit(USART1);
	SPI_I2S_DeInit(SPI1);
}

void BSP_md(int addr,int size)
{
	int i;

	printd("\n");

	for(i=0;i<size;i++){
		
		if ( (i & 0xF) == 0 ){
			printd("\n%05X : ",addr+i);
		}
		printd("%02X ",((char *)addr)[i]);
	}
	printd("\n");
	
}

#if 1
FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("0:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}


INT16U dbg_fsmount_test(void)
{
	uint32_t free, total;
    char buffer[50];

	printd("sd card mount and file write.....\n");
	if (f_mount(&SD_Fs, "0:", 1) == FR_OK) 
	{
		/* Mounted ok */
		/* Write only once */
		/* Try to open SD card file */
		if (f_open(&SD_Fil, "0:sd_fileboot.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS) == FR_OK)
		{       
			/* We want to write only once */
			/* Get total and free space on SD card */
			TM_FATFS_DriveSize(&total, &free);

			/* Put data */
			f_puts("This is my first file with SD card and FatFs\n", &SD_Fil);
			f_puts("with SD card library from stm32f4-discovery.com\n", &SD_Fil);
			f_puts("----------------------------------------------------\n", &SD_Fil);
			f_puts("SD card total and free space:\n\n", &SD_Fil);
			/* Total space */
			sprintf(buffer, "Total: %8u kB; %5u MB; %2u GB\n", total, total / 1024, total / 1048576); 
			f_puts(buffer, &SD_Fil);
			/* Free space */
			sprintf(buffer, "Free:  %8u kB; %5u MB; %2u GB\n", free, free / 1024, free / 1048576);
			f_puts(buffer, &SD_Fil);
			f_puts("----------------------------------------------------\n", &SD_Fil);
			/* Close SD card file */
			f_close(&SD_Fil);  
		}
		f_mount(0, "0:", 1);
	}

	return 0;
}
#endif




/******************* (C) COPYRIGHT 2014 ***************END OF FILE****/

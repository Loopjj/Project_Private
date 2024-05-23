/****************************************Copyright (c)****************************************************
**                                      
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               fsmc_nand.c
** Descriptions:            The FSMC Nand application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              MKH
** Created date:            2014-12-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define NAND_GLOBALS
#include "includes.h"

#if 1

/* Private define ------------------------------------------------------------*/
#define NAND_FLASH_START_ADDR     ((uint32_t)0x70000000) 

#define PROGRAM_TIMEOUT	0xF00000

NAND_ADDRESS WriteReadAddr;

//#define ROW_ADDRESS ( (WriteReadAddr.Page*NAND_PAGE_SIZE) + (WriteReadAddr.Block + (WriteReadAddr.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)
//#define ROW_ADDRESS ( (WriteReadAddr.Page) + (WriteReadAddr.Block + (WriteReadAddr.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)
#define ROW_ADDRESS ( (WriteReadAddr.Page) + (WriteReadAddr.Block + (WriteReadAddr.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_COUNT)


/* Private variables ---------------------------------------------------------*/
uint8_t RxBuffer [NAND_PAGE_SIZE];
#if NAND_DEBUG >0
uint8_t TxBuffer [NAND_PAGE_SIZE];
uint8_t RxBuffer [NAND_PAGE_SIZE];
#endif


void NandSemaphore_Init(void)
{
	BSP_NANDSem = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_NANDSem);
}

/*******************************************************************************
* Function Name  : FSMC_NAND_Init
* Description    : Configures the FSMC and GPIOs to interface with the NAND memory.
*                  This function must be called before any write/read operation
*                  on the NAND.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void FSMC_NAND_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
	FSMC_NAND_PCCARDTimingInitTypeDef  p;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);

	/*-- GPIO Configuration ------------------------------------------------------*/
	/* CLE, ALE, D0->D3, NOE, NWE and NCE2  NAND pin configuration  */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);	
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/* CLE, ALE, D0->D3, NOE, NWE and NCE2  NAND pin configuration  */
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 |
                                  GPIO_Pin_7;

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* NWAIT NAND pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                                      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	/* D4->D7 NAND pin configuration  */  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
    /* D4->D7 NAND pin configuration  */ 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);

	//GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FSMC);


	/*-- FSMC Configuration ------------------------------------------------------*/
#if 1
	p.FSMC_SetupTime = 0xf1;
	p.FSMC_WaitSetupTime = 0xf3;
	p.FSMC_HoldSetupTime = 0xf2;
	p.FSMC_HiZSetupTime = 0xf1;
#else
	p.FSMC_SetupTime = 0x1;
	p.FSMC_WaitSetupTime = 0x15;
	p.FSMC_HoldSetupTime = 0x2;
	p.FSMC_HiZSetupTime = 0x1;
#endif


	FSMC_NANDInitStructure.FSMC_Bank						    = FSMC_Bank2_NAND;
	FSMC_NANDInitStructure.FSMC_Waitfeature					= FSMC_Waitfeature_Enable;
	FSMC_NANDInitStructure.FSMC_MemoryDataWidth			= FSMC_MemoryDataWidth_8b;
	FSMC_NANDInitStructure.FSMC_ECC							    = FSMC_ECC_Disable;//FSMC_ECC_Enable;
	FSMC_NANDInitStructure.FSMC_ECCPageSize					= FSMC_ECCPageSize_512Bytes;
#if 1
	FSMC_NANDInitStructure.FSMC_TCLRSetupTime				= 0xF1;//0xA1;
	FSMC_NANDInitStructure.FSMC_TARSetupTime				= 0xF1;//0x15;
#else
	FSMC_NANDInitStructure.FSMC_TCLRSetupTime				= 0x0;//0xA1;
	FSMC_NANDInitStructure.FSMC_TARSetupTime				= 0x0;//0x15;
#endif
	FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct		= &p;
	FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct	= &p;

	FSMC_NANDInit(&FSMC_NANDInitStructure);

	/* FSMC NAND Bank Cmd Test */
	FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);

}

/******************************************************************************
* Function Name  : FSMC_NAND_WriteSmallPage
* Description    : This routine is for writing one or several 4096 Bytes Page size.
* Input          : - pBuffer: pointer on the Buffer containing data to be written   
*                  - Address: First page address
*                  - NumPageToWrite: Number of page to write  
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_WriteSmallPage(uint8_t *pBuffer, uint32_t Address, uint32_t size)
{
	uint32_t index = 0x00, numpagewritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
	uint32_t status = NAND_READY;//, size = 0x00;
	uint16_t caddr,paddr,NumPageToWrite = 5;
	
	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{
		caddr = Address%NAND_PAGE_SIZE;
		paddr = Address/NAND_PAGE_SIZE;

		//while((size != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
		while((NumPageToWrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
		{
			/* Page write command and address */
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF);

			/* Calculate the size */
			//size = NAND_PAGE_SIZE;// + (NAND_PAGE_SIZE * numpagewritten);

			/* Write data */
			for(index=0; index < size; index++)
			{
			  *(__IO uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = pBuffer[index];
			}
		    
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

			/* ¶ÁÃ¦½Å */
		//    while( GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0 );
		    
			/* Check status for successful operation */
			status = FSMC_NAND_GetStatus();
			//printk("FSMC_NAND_WriteSmallPage status = %02X\n",status);
		    
			if(status == NAND_READY)
			{
			  numpagewritten++;

			  NumPageToWrite--;

			  /* Calculate Next small page Address */
			  addressstatus = FSMC_NAND_AddressIncrement(&Address);    
			  //printk("address status FSMC_NAND_WriteSmallPage status = %02X\n",addressstatus);
			} 
		}
		xSemaphoreGive(BSP_NANDSem);
		return (status | addressstatus);
	}
	return NAND_ERROR;
}

/******************************************************************************
* Function Name  : FSMC_NAND_ReadSmallPage
* Description    : This routine is for sequential read from one or several 
*                  4096 Bytes Page size.
* Input          : - pBuffer: pointer on the Buffer to fill  
*                  - Address: First page address
*                  - NumPageToRead: Number of page to read
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_ReadSmallPage(uint8_t *pBuffer, uint32_t Address, uint32_t size)
{
	uint32_t index = 0x00, numpageread = 0x00, addressstatus = NAND_VALID_ADDRESS;
	uint32_t status = NAND_READY;//, size = 0x00;
	uint16_t caddr,paddr,NumPageToRead = 10;
	
	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{
		caddr = Address%NAND_PAGE_SIZE;
		paddr = Address/NAND_PAGE_SIZE;

		while((NumPageToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
		{	   
			/* Page Read command and page address */
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
		   
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF);
		    
			*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
		   
		    
			/* Calculate the size */
			//size = NAND_PAGE_SIZE;// + (NAND_PAGE_SIZE * numpageread);
		    
			for(index=0; index < 0x255; index++);

			/* Get Data into Buffer */    
			for(index=0; index < size; index++)
			{
			  pBuffer[index]= *(__IO uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
			}

			numpageread++;
		    
			NumPageToRead--;

			/* Calculate page address */           			 
			addressstatus = FSMC_NAND_AddressIncrement(&Address);
		}

		status = FSMC_NAND_GetStatus();
		xSemaphoreGive(BSP_NANDSem);
		return (status | addressstatus);
	}
	return NAND_ERROR;
}

/******************************************************************************
* Function Name  : FSMC_NAND_WriteSpareArea
* Description    : This routine write the spare area information for the specified 
*                  pages addresses.
* Input          : - pBuffer: pointer on the Buffer containing data to be written 
*                  - Address: First page address
*                  - NumSpareAreaTowrite: Number of Spare Area to write
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_WriteSpareArea(uint8_t *pBuffer, uint32_t Address, uint32_t size)
{
	INT32U index,status = NAND_ERROR;
	INT16U caddr;
	INT16U paddr;
	//INT8U  ret;
	uint32_t add = Address;

	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{

		add = add & (~(NAND_BLOCK_SIZE-1));
		caddr = NAND_PAGE_SIZE;
		paddr = add/NAND_PAGE_SIZE;
		//printk("\nWR add = 0X%08X, caddr = 0X%08X, paddr = 0X%08X \n",add,caddr,paddr);

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF); 

		//size = NAND_SPARE_AREA_SIZE;
		/* Write the data */ 
		for(index=0; index < size; index++)
		{
		  *(__IO uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = pBuffer[index];
		}
	    
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;
		while( GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0 );

		status = FSMC_NAND_GetStatus();
		xSemaphoreGive(BSP_NANDSem);
		return status;
	}
	return NAND_ERROR;

}

/******************************************************************************
* Function Name  : FSMC_NAND_ReadSpareArea
* Description    : This routine read the spare area information from the specified
*                  pages addresses.
* Input          : - pBuffer: pointer on the Buffer to fill  
*                  - Address: First page address
*                  - NumSpareAreaToRead: Number of Spare Area to read
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_ReadSpareArea(uint8_t *pBuffer, uint32_t Address, uint32_t size)
{
	INT32U index,status;
	INT16U caddr;
	INT16U paddr;
	//INT8U  ret;
	uint32_t add = Address;

	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{

		add = add & (~(NAND_BLOCK_SIZE-1));
		caddr = NAND_PAGE_SIZE;
		paddr = add/NAND_PAGE_SIZE;
		printk("\nRD add = 0X%08X, caddr = 0X%08X, paddr = 0X%08X \n",add,caddr,paddr);

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1;

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF); 

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;

		//for(index=0; index < 0x255; index++);

		/* Data Read */
		//size = NAND_SPARE_AREA_SIZE;
		
		/* Get Data into Buffer */
		for (index=0;index < size; index++)
		{
		  pBuffer[index] = *(__IO uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
		}
		

		status = FSMC_NAND_GetStatus();
		xSemaphoreGive(BSP_NANDSem);
		//printk("  %d[%X]\n",ret,*err);
	  
		return status;
	}
	return NAND_ERROR;

}

/******************************************************************************
* Function Name  : FSMC_NAND_EraseBlock
* Description    : This routine erase complete block from NAND FLASH
* Input          : - Address: Any address into block to be erased
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_EraseBlock( uint32_t Address)
{
	INT32U status = NAND_ERROR;
	uint8_t addr1,addr2,addr3;
	printk("\n Erase block = %04X\n",Address);

	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{
		addr1 = Address;
		addr2 = Address >> 8;
		addr3 = Address >> 16;

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_ERASE0;
	  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = addr1;  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = addr2;
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = addr3;
			
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_ERASE1; 

		status = FSMC_NAND_GetStatus();
		xSemaphoreGive(BSP_NANDSem);
		return status;
	}
	return NAND_ERROR;
}

/******************************************************************************
* Function Name  : FSMC_NAND_Reset
* Description    : This routine reset the NAND FLASH
* Input          : None
* Output         : None
* Return         : NAND_READY
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_Reset(void)
{
	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_RESET;
		xSemaphoreGive(BSP_NANDSem);
		return (NAND_READY);
	}
	return NAND_ERROR;
}

/******************************************************************************
* Function Name  : FSMC_NAND_GetStatus
* Description    : Get the NAND operation status
* Input          : None
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation  
* Attention		 : None  
*******************************************************************************/
uint32_t FSMC_NAND_GetStatus(void)
{
  uint32_t timeout = PROGRAM_TIMEOUT, status = NAND_READY;

  status = FSMC_NAND_ReadStatus(); 

  /* Wait for a NAND operation to complete or a TIMEOUT to occur */
  while ((status != NAND_READY) &&( timeout != 0x00))
  {
     status = FSMC_NAND_ReadStatus();
     timeout --;      
  }

  if(timeout == 0x00)
  {          
    status =  NAND_TIMEOUT_ERROR;      
  } 

  /* Return the operation status */
  return (status);      
}
/******************************************************************************
* Function Name  : FSMC_NAND_ReadStatus
* Description    : Reads the NAND memory status using the Read status command 
* Input          : None
* Output         : None
* Return         : The status of the NAND memory. This parameter can be:
*                   - NAND_BUSY: when memory is busy
*                   - NAND_READY: when memory is ready for the next operation    
*                   - NAND_ERROR: when the previous operation gererates error  
* Attention		 : None 
*******************************************************************************/
uint32_t FSMC_NAND_ReadStatus(void)
{
  uint32_t data = 0x00, status = NAND_BUSY;

  /* Read status operation ------------------------------------ */
  *(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_STATUS; 
  data = *(__IO uint8_t *)(NAND_FLASH_START_ADDR); 

  if((data & NAND_ERROR) == NAND_ERROR)
  {
    status = NAND_ERROR;
  } 
  else if((data & NAND_READY) == NAND_READY)
  {
    status = NAND_READY;
  }
  else
  {
    status = NAND_BUSY; 
  }
  
  return (status);
}

/******************************************************************************
* Function Name  : NAND_AddressIncrement
* Description    : Increment the NAND memory address
* Input          : - Address: address to be incremented.
* Output         : None
* Return         : The new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t FSMC_NAND_AddressIncrement(uint32_t* Address)
{
  uint32_t status = NAND_VALID_ADDRESS;
#if 0

#endif
  
  return (status);
}
#if 0
u32 FSMC_NAND_AddressIncrement(NAND_ADDRESS* Address) 
{ 
  u32 status = NAND_VALID_ADDRESS; 
  
  Address->Page++; 

 
  if(Address->Page == NAND_BLOCK_SIZE) 
  { 
    Address->Page = 0; 
    Address->Block++; 
     
    if(Address->Block == NAND_ZONE_SIZE) 
    { 
      Address->Block = 0; 
      Address->Zone++; 

 
      if(Address->Zone == NAND_MAX_ZONE) 
      { 
        status = NAND_INVALID_ADDRESS; 
      } 
    } 
  }  
   
  return (status); 
} 

#endif
INT8U FSMC_NAND_Badblock(INT32U addr,INT32U *err)
{
	//INT32U index;
	INT16U caddr;
	INT16U paddr;
	INT8U  ret=0;

	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{

		addr = addr & (~(NAND_BLOCK_SIZE-1));
		caddr = NAND_PAGE_SIZE;
		paddr = addr/NAND_PAGE_SIZE;
		//printk("add = 0X%08X, caddr = 0X%08X, paddr = 0X%08X ",addr,caddr,paddr);

		 *(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1;

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( caddr&0xFF      );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( (caddr>>8)&0x0F ); 
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( paddr&0xFF      );  
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( (paddr>>8)&0xFF );
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t) ( (paddr>>16)&0x03);  

		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;

		//for(index=0; index < 0x255; index++);

		/* Get Data into Buffer */    
		ret= *(__IO uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
		

		*err = FSMC_NAND_GetStatus();
		//printk("  %d[%X]\n",ret,*err);
		xSemaphoreGive(BSP_NANDSem);
		return ret;
	}
	return ret;

}

INT8U FSMC_NAND_ReadData(INT8U cmd, INT8U add, INT8U *targer,INT8U size)
{
	//uint32_t data = 0;
	uint32_t i=0;
	//uint8_t pBuffer[20];
	if(BSP_NANDSem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_NANDSem, ( portTickType ) 0 )  == pdTRUE)
	{

		/* Send Command to the command area */ 	
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = cmd;
		*(__IO uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = add;

		for(i=0;i<size;i++)
		{
			targer[i]= *(__IO uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
		}
		xSemaphoreGive(BSP_NANDSem);
	}
	return ERROR;
}

void Nand_Make_Badblock_Table(void)
{
	INT16U i;    
    INT32U addr,err;
	INT32U baseAddr=NAND_BADBLOCK_BASE;
    INT8U status = 0;
	st_BadBlockData badblock;
	INT8U checksum;         

   for(i = 0; i < 10; i++)
   {
	   addr = NAND_BADBLOCK_BASE-i*NAND_BLOCK_SIZE+NAND_PAGE_SIZE;
		//status = Nand_Read_bb(addr);
		status = FSMC_NAND_Badblock(addr, &err);
		if(status != 0)
		{
			baseAddr = NAND_BADBLOCK_BASE-i*NAND_BLOCK_SIZE;
			break;
		}
	}
	if(i == 10) return;
	Nand_Read_Buffer((u8*)&badblock, baseAddr, sizeof(st_BadBlockData));
	if(badblock.BadBlockCount < 50)
	{
		checksum = badblock.BadBlockCount;
		for(i = 0; i < badblock.BadBlockCount; i++)
		{
			checksum += badblock.BadBlockAddr[i]&0xff;
			checksum += (badblock.BadBlockAddr[i]>>8)&0xff;
		}
		if(checksum == badblock.BadBlockCheckSum)
		{ 
			BadBlockData = badblock;
			return;
		}
	}
  
	memset((u8*)&badblock,0,sizeof(st_BadBlockData));
	for(i = 0; i < NAND_BLOCK_NUM; i++)
	{
		addr = i*NAND_BLOCK_SIZE+NAND_PAGE_SIZE;
		status = FSMC_NAND_Badblock(addr, &err);
		if(status == 0)
		{
			if(badblock.BadBlockCount < 40)
				badblock.BadBlockAddr[badblock.BadBlockCount++] = i;   
		}
	}
	badblock.BadBlockCheckSum = badblock.BadBlockCount;
	for(i = 0; i < badblock.BadBlockCount; i++)
	{
		badblock.BadBlockCheckSum += badblock.BadBlockAddr[i]&0xff;
		badblock.BadBlockCheckSum += (badblock.BadBlockAddr[i]>>8)&0xff;
	}
	Nand_Write_Buffer((INT8U*)&badblock, baseAddr, sizeof(st_BadBlockData));
	BadBlockData = badblock;
}


void Nand_Read_BadblockData(void)
{
	INT16U i;
	st_BadBlockData badblock;
	INT8U checksum;
	INT32U baseAddr=NAND_BADBLOCK_BASE,err;  
	INT8U status;
	INT32U addr;

	for(i = 0; i < 10; i++) 
	{
		addr = NAND_BADBLOCK_BASE-i*NAND_BLOCK_SIZE+NAND_PAGE_SIZE;
		status = FSMC_NAND_Badblock(addr, &err);
		if(status != 0) 
		{
			baseAddr = NAND_BADBLOCK_BASE-i*NAND_BLOCK_SIZE;
			break;
		}
	}	

	if(i == 10) {
		printd("\n===Bad block table not found===\n");
		return;
	}
	
	Nand_Read_Buffer((u8*)&badblock, baseAddr, sizeof(st_BadBlockData));
	if(badblock.BadBlockCount < 50) 
	{
		checksum = badblock.BadBlockCount;
		for(i = 0; i < badblock.BadBlockCount; i++) 
		{
			checksum += badblock.BadBlockAddr[i]&0xff;
			checksum += (badblock.BadBlockAddr[i]>>8)&0xff;
		}
		if(checksum == badblock.BadBlockCheckSum)
		{ 
			BadBlockData = badblock;

			printd("\n\n===Bad block table found===\n");
			printd("Cnt:%d\n",BadBlockData.BadBlockCount);
			for(i=0;i<BadBlockData.BadBlockCount;i++)	
			{
				printd("BadBlockindex : %d\n",BadBlockData.BadBlockAddr[i]);
			}
			printd("ChkSUM:0x%02X\n",BadBlockData.BadBlockCheckSum);
			
		}
		else
		{
			printd("===Checksum error. %02X:%02X ===\n",checksum,badblock.BadBlockCheckSum);
		}
	}
	else
	{
		printd("===Bad block not found===\n");
	}
}

INT32U Nand_Check_Badblock(INT32U addr)
{
	INT16U i, block_index = 0;
	INT32U new_addr;
  
	if(BadBlockData.BadBlockCount > 0 &&BadBlockData.BadBlockCount < 50) 
	{
		block_index = addr/NAND_BLOCK_SIZE;
		for(i = 0; i < BadBlockData.BadBlockCount; i++)
		{
			if(BadBlockData.BadBlockAddr[i] == block_index) 
			{
				new_addr = NAND_BADBLOCK_DATA_ADDR + i*NAND_BLOCK_SIZE+(addr%NAND_BLOCK_SIZE);
				addr = new_addr; 
				block_index = addr/NAND_BLOCK_SIZE;
			}	
		}
	}
	return addr;
}


INT32U Nand_Write_Buffer(INT8U *pBuffer, INT32U addr, INT16U len)
{
  //u16 count;
  //u16 caddr;
  INT32U status = NAND_READY, eaddr;
   
  if(len == 0) {
    return 0;
  }
  addr = Nand_Check_Badblock(addr);
  //caddr = addr%NAND_PAGE_SIZE;
 // paddr = addr/NAND_PAGE_SIZE;
  eaddr = addr+len;
 
  if(addr%NAND_BLOCK_SIZE == 0) {//0x40000 = one block size
	  printk("\n 00 Erase block addr = %04X, NAND_PAGE_SIZE = %04X, erase addr = [%04X]\n",addr,NAND_PAGE_SIZE, addr/NAND_PAGE_SIZE);
	  status = FSMC_NAND_EraseBlock(addr/NAND_PAGE_SIZE); 
  }
  else if((addr%NAND_BLOCK_SIZE)+len > NAND_BLOCK_SIZE) {//0x40000 = one block size
	  printk("\n 11 Erase block addr = %04X, NAND_PAGE_SIZE = %04X, erase addr = [%04X]\n",eaddr,NAND_PAGE_SIZE, eaddr/NAND_PAGE_SIZE);
	  status = FSMC_NAND_EraseBlock(eaddr/NAND_PAGE_SIZE); 
  }

  status = FSMC_NAND_WriteSmallPage(pBuffer,addr,len);
  
  return status;
}

INT32U Nand_Read_Buffer(INT8U *pBuffer, INT32U addr, INT16U len)
{
	INT32U status;

	addr = Nand_Check_Badblock(addr);
	status = FSMC_NAND_ReadSmallPage(pBuffer,addr, len);
  
	return status;
}


#if NAND_DEBUG >0
////////////////NAND Memory Test///////////////////////////////////////////////////////////////////////////////////////////
void TestMemory(void)
{
	INT32U rstatus=0,wstatus=0;
	u8 i,index=0;
	INT32U addr;
	
	for(i = 0; i < 10; i++) 
	{
		memset(TxBuffer,0x0,20);
		memset(RxBuffer,0x0,20);
		for (index = 0; index < 10; index++ )
		{
			TxBuffer[index] = i;
		}
		addr = NAND_MEMORY_TEST_ADDR + NAND_PAGE_SIZE * i;
		wstatus = Nand_Write_Buffer(TxBuffer, addr, 10);
		rstatus = Nand_Read_Buffer( RxBuffer, addr, 10);
		MemoryDisplay((u32)RxBuffer,10);
		printk("address = %04X,  write status= %04X, read status = %04X\n", addr,wstatus,rstatus);
	}    
}

/******************************************************************************
* Function Name  : NAND Debug Function
* Attention		 : None
*******************************************************************************/

INT16U dbg_nandWriteRead_test(void)
{
	INT32U addr,index,ret;
	INT8U data;

	if(CLI_GetArgCnt() < 2) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));
	data = comparison_num((char *)P_ARG(2));


	memset(TxBuffer,0x0,sizeof(TxBuffer));
	memset(RxBuffer,0x0,sizeof(RxBuffer));
	for (index = 0; index < NAND_PAGE_SIZE; index++ )
	{
		TxBuffer[index] = data;
	}
	ret = FSMC_NAND_EraseBlock(addr/NAND_PAGE_SIZE);
	printk("\n[%02X]NAND Erase Block 0x%08X\n",ret,addr/NAND_PAGE_SIZE);

	ret = FSMC_NAND_WriteSmallPage(TxBuffer, addr, NAND_PAGE_SIZE);
	printk("[%02X]NAND Written data = 0x%02X\n",ret,data);

	ret = FSMC_NAND_ReadSmallPage (RxBuffer, addr, NAND_PAGE_SIZE);
	MemoryDisplay((u32)RxBuffer,NAND_PAGE_SIZE);
	printk("....NAND Read return = %02X\n",ret);

	return NO_DISPLAY;
}

INT16U dbg_nandWrite_test(void)
{
	INT32U addr,ret,index;
	INT8U data;

	if(CLI_GetArgCnt() < 2) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));
	data = comparison_num((char *)P_ARG(2));

	memset(TxBuffer,0x0,sizeof(TxBuffer));
	for (index = 0; index < NAND_PAGE_SIZE; index++ )
	{
		TxBuffer[index] = data;
	}

	ret = FSMC_NAND_WriteSmallPage(TxBuffer,addr,NAND_PAGE_SIZE);
	printk("\n[%02X]NAND write addr 0x%08X\n",ret,addr);
	return NO_DISPLAY;
}


INT16U dbg_nandblockread_test(void)
{
	INT32U cmd,addr,size;
	//uint8_t RxBuffer [NAND_PAGE_SIZE];


	if(CLI_GetArgCnt() < 1) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));

	FSMC_NAND_ReadSmallPage (RxBuffer, addr, NAND_PAGE_SIZE);
	printk("\nNAND Page Read address = 0x%04X\n",addr);
	MemoryDisplay((u32)RxBuffer,NAND_PAGE_SIZE);

	return NO_DISPLAY;
}

INT16U dbg_nandspareread_test(void)
{
	INT32U cmd,addr,size;

	if(CLI_GetArgCnt() < 1) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));

	FSMC_NAND_ReadSpareArea (RxBuffer, addr, 1);
	printk("\nNAND Spare Read address = 0x%04X\n",addr);
	MemoryDisplay((u32)RxBuffer,NAND_SPARE_AREA_SIZE);

	return NO_DISPLAY;
}
INT16U dbg_nandsparewrite_test(void)
{
	INT32U cmd,addr,size,ret,index;
	INT8U data=0;
	//uint8_t TxBuffer [NAND_PAGE_SIZE];


	if(CLI_GetArgCnt() < 2) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));
	data = comparison_num((char *)P_ARG(2));


	ret = FSMC_NAND_EraseBlock(addr);
	printk("\nNAND Erase block address = [%X]0x%04X\n",ret,addr);
	
	for (index = 0; index < NAND_SPARE_AREA_SIZE; index++ )
	{
		TxBuffer[index] = data;
	}

	FSMC_NAND_WriteSpareArea (TxBuffer, addr, 1);
	printk("\nNAND Spare write address = 0x%04X\n",addr);

	return NO_DISPLAY;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif

INT16U dbg_nandErase_test(void)
{
	INT32U addr,ret;
	if(CLI_GetArgCnt() < 1) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));

	ret = FSMC_NAND_EraseBlock(addr/NAND_PAGE_SIZE);
	printk("\n[%02X]NAND Erase Block 0x%08X\n",ret,addr/NAND_PAGE_SIZE);
	return NO_DISPLAY;

}

INT16U dbg_nandread_test(void)
{
	INT32U addr,size;

	if(CLI_GetArgCnt() < 2) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));
	size = comparison_num((char *)P_ARG(2));
	if( size > NAND_PAGE_SIZE ) size = NAND_PAGE_SIZE;

	//FSMC_NAND_ReadData(cmd,addr,tmp,size);
  Nand_Read_Buffer( RxBuffer, addr, size);
	MemoryDisplay((INT32U)RxBuffer, size);
		
	return NO_DISPLAY;
}


INT16U CMD_NAND_checkbb(void)
{
	INT32U i, bbcnt=0,err;
	INT8U bbstat;

	printk("\n");
	for(i=0;i<NAND_SIZE/NAND_BLOCK_SIZE;i++)
	{
		//printk("[%04d] ",i);
		bbstat = FSMC_NAND_Badblock(NAND_BLOCK_SIZE*i, &err);
		if ( bbstat != 255 ) {
			printk("[%X]ADDR[%04X]:0x%08X : %d",err,i,NAND_BLOCK_SIZE*i,bbstat);
			if ( bbstat == 0 ) {
				printk(" ==>> BAD!!!!\n");
				bbcnt++;
			}
			else{
				printk("\n");
			}
		}
	}

	printk("Bad Block Count : %d\n",bbcnt);
	   
  return NO_DISPLAY;
}

INT16U CMD_NAND_id(void)
{
	u8 rdata[10]={0};

	FSMC_NAND_ReadData(0x90,0,rdata,5);

	printk("\nID 0 read\n");
	MemoryDisplay((u32)rdata,5);

	printk("Manufacturer ID : ");
	if( rdata[0] == 0x2C ){
		 printk("Micron\n");
	}
	else{
		 printk("Unknown\n");
	}

	printk("Device ID [%02X]  : ",rdata[1]);
	switch(rdata[1]){
		case 0xAC:printk("MT29F4G08ABBEA 4Gb, x8, 1.8V\n");
			break;
		case 0xBC:printk("MT29F4G16ABBEA 4Gb, x16, 1.8V\n");
			break;
		case 0xDC:printk("MT29F4G08ABAEA 4Gb, x8, 3.3V\n");
			break;
		case 0xCC:printk("MT29F4G16ABAEA 4Gb, x16, 3.3V\n");
			break;
		default:printk("Unknown device ID\n");
			break;
	}
	

	FSMC_NAND_ReadData(0x90,0x20,rdata,5);
	printk("\nID 0x20 read\n");
	MemoryDisplay((u32)rdata,5);
	
	   
  return NO_DISPLAY;
}



#endif /* #if SCR_USE_NAND > 0*/

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

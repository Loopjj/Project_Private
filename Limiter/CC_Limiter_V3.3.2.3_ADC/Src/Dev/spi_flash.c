/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : i2cfram_FM24CL64B.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : This file provides a set of functions needed to manage the
*                      communication between SPI peripheral and SPI M25P64 FLASH.
*
*********************************************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#define SPI_FLASH_GLOBALS
#include "includes.h"

/* Private define ------------------------------------------------------------*/
#define WRITE       0x02  /* Main Memory Page Program Through Buffer 1 */
#define READ        0x03  /* Read from Memory instruction */
#define RDSR1       0x05  /* Read Status Register instruction  */
#define RDSR2       0x35  /* Read Status Register instruction  */
#define RDID        0x90  /* Read identification */
#define BE32        0x52  /* Page Erase instruction */
#define BE64        0xD8  /* Block Erase instruction */
#define SE          0x20  /* Sector Erase instruction */
#define CE          0xC7  /* Chip Erase instruction */


#define BUSY_Flag   0x01  /* Status Register Busy bit flag */
#define WEL_Flag    0x02  /* Status Register Write Enable Latch(WEL) bit flag */


#define Dummy_Byte  0xA5

osSemaphoreDef(SpiFlashDevSem);
osSemaphoreId SpiFlashDevSem = NULL;



void SPI_Flash_Init(void)
{
  SPI_FLASH_CS_HIGH();  
  SpiFlashDevSem = osSemaphoreCreate(osSemaphore(SpiFlashDevSem) , 1);  
}


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnableLatch
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
	/* Write Enalbe Latch */
	/* Select the FLASH: Chip Select low */
	SPI_FLASH_CS_LOW();
	/* Send Bulk Erase instruction	*/
	SPI_FLASH_SendByte(0x06);
	/* Deselect the FLASH: Chip Select high */
	SPI_FLASH_CS_HIGH();

	/* Wait the end of Flash writing */
	SPI_FLASH_WaitForWEL(SET);
}

void SPI_FLASH_WriteDisable(void)
{
	/* Write Enalbe Latch */
	/* Select the FLASH: Chip Select low */
	SPI_FLASH_CS_LOW();
	/* Send Bulk Erase instruction	*/
	SPI_FLASH_SendByte(0x04);
	/* Deselect the FLASH: Chip Select high */
	SPI_FLASH_CS_HIGH();

	/* Wait the end of Flash writing */
	SPI_FLASH_WaitForWEL(RESET);
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 50) == osOK)
	{
    SPI_FLASH_WriteEnable();

    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    /* Send Sector Erase instruction */
    SPI_FLASH_SendByte(SE);
    /* Send SectorAddr high nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    SPI_FLASH_WriteDisable();

    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();

    osSemaphoreRelease(SpiFlashDevSem);
	}
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageErase
* Description    : Erases the specified FLASH Page.
* Input          : PageAddr: address of the PageA to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageErase(uint32_t PageAddr)
{
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 50) == osOK)
	{
		SPI_FLASH_WriteEnable();
		/* Sector Erase */
		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();
		/* Send Sector Erase instruction */
		SPI_FLASH_SendByte(SE);
		/* Send SectorAddr high nibble address byte */
		SPI_FLASH_SendByte((PageAddr & 0xFF0000) >> 16);
		/* Send SectorAddr medium nibble address byte */
		SPI_FLASH_SendByte((PageAddr & 0xFF00) >> 8);
		/* Send SectorAddr low nibble address byte */
		SPI_FLASH_SendByte(PageAddr & 0xFF);
		/* Deselect the FLASH: Chip Select high */
		SPI_FLASH_CS_HIGH();
		
		SPI_FLASH_WriteDisable();
		
		/* Wait the end of Flash writing */
		SPI_FLASH_WaitForWriteEnd();
    
    osSemaphoreRelease(SpiFlashDevSem);
	}

}

/*******************************************************************************
* Function Name  : SPI_FLASH_BlockErase
* Description    : Erases the specified FLASH Block.
* Input          : BlockAddr: address of the Block to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BlockErase(uint32_t BlockAddr)
{
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 0) == osOK)
	{
		SPI_FLASH_WriteEnable();
	  
		/* 64K Block Erase */
		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();
		/* Send Sector Erase instruction */
		 SPI_FLASH_SendByte(BE64);
		/* Send SectorAddr high nibble address byte */
		SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
		/* Send SectorAddr medium nibble address byte */
		SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
		/* Send SectorAddr low nibble address byte */
		SPI_FLASH_SendByte(BlockAddr & 0xFF);
		/* Deselect the FLASH: Chip Select high */
		SPI_FLASH_CS_HIGH();
	  
		SPI_FLASH_WriteDisable();
	  
		/* Wait the end of Flash writing */
		SPI_FLASH_WaitForWriteEnd();
		osSemaphoreRelease(SpiFlashDevSem);
	}
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ChipErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_ChipErase(void)
{
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 0) == osOK)
	{
		SPI_FLASH_WriteEnable();

		/* Bulk Erase */
		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();
		/* Send Bulk Erase instruction  */
		SPI_FLASH_SendByte(CE);
		/* Deselect the FLASH: Chip Select high */
		SPI_FLASH_CS_HIGH();

		SPI_FLASH_WriteDisable();

		/* Wait the end of Flash writing */
		SPI_FLASH_WaitForWriteEnd();
		osSemaphoreRelease(SpiFlashDevSem);
	}

}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PAGE_SIZE" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  if((WriteAddr%SPI_FLASH_SECTOR) == 0)
    SPI_FLASH_SectorErase(WriteAddr);
  
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 50) == osOK)
	{

		SPI_FLASH_WriteEnable();

		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();
		/* Send "Write to Memory " instruction */
		SPI_FLASH_SendByte(WRITE);
		/* Send WriteAddr high nibble address byte to write to */
		SPI_FLASH_SendByte((WriteAddr >> 16) & 0xFF);
		/* Send WriteAddr medium nibble address byte to write to */
		SPI_FLASH_SendByte((WriteAddr >> 8) & 0xFF);
		/* Send WriteAddr low nibble address byte to write to */
		SPI_FLASH_SendByte(WriteAddr & 0xFF);

		/* while there is data to be written on the FLASH */
		while (NumByteToWrite--)
		{
			/* Send the current byte */
			SPI_FLASH_SendByte(*pBuffer);
			/* Point on the next byte to be written */
			pBuffer++;
		}

		/* Deselect the FLASH: Chip Select high */
		SPI_FLASH_CS_HIGH();

		SPI_FLASH_WriteDisable();

		/* Wait the end of Flash writing */
		SPI_FLASH_WaitForWriteEnd();
		osSemaphoreRelease(SpiFlashDevSem);
	}
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
INT8U SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PAGE_SIZE;
  count = SPI_FLASH_PAGE_SIZE - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PAGE_SIZE;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGE_SIZE */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PAGE_SIZE */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGE_SIZE);
        WriteAddr +=  SPI_FLASH_PAGE_SIZE;
        pBuffer += SPI_FLASH_PAGE_SIZE;
      }

      if(NumOfSingle !=0 ) SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGE_SIZE */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PAGE_SIZE */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PAGE_SIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PAGE_SIZE;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PAGE_SIZE;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGE_SIZE);
        WriteAddr +=  SPI_FLASH_PAGE_SIZE;
        pBuffer += SPI_FLASH_PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  
  return 1;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	//INT8U err;
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 50) == osOK)
	{
		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();

		/* Send "Read from Memory " instruction */
		SPI_FLASH_SendByte(READ);

		/* Send ReadAddr high nibble address byte to read from */
		SPI_FLASH_SendByte((ReadAddr >> 16) & 0xFF);
		/* Send ReadAddr medium nibble address byte to read from */
		SPI_FLASH_SendByte((ReadAddr >> 8) & 0xFF);
		/* Send ReadAddr low nibble address byte to read from */
		SPI_FLASH_SendByte(ReadAddr & 0xFF);


		while (NumByteToRead--) /* while there is data to be read */
		{
			/* Read a byte from the FLASH */
			*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);

			/* Point to the next location where the byte read will be saved */
			pBuffer++;
		}

		/* Deselect the FLASH: Chip Select high */
		SPI_FLASH_CS_HIGH();
		osSemaphoreRelease(SpiFlashDevSem);
	}
}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0;
	//INT8U err;

	
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return 0x0;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 50) == osOK)
	{

		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();

		/* Send "RDID " instruction */
		SPI_FLASH_SendByte(RDID);

		/* Send the 24-bit address of the address to read from -----------------------*/
		/* Send ReadAddr high nibble address byte */
		SPI_FLASH_SendByte(0x00);
		/* Send ReadAddr medium nibble address byte */
		SPI_FLASH_SendByte(0x00);
		/* Send ReadAddr low nibble address byte */
		SPI_FLASH_SendByte(0x00);

		/* Read a byte from the FLASH */
		Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

		/* Read a byte from the FLASH */
		Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

		/* Deselect the FLASH: Chip Select high */
		SPI_FLASH_CS_HIGH();

		Temp = (Temp0 << 8) | Temp1;

		osSemaphoreRelease(SpiFlashDevSem);
	}

	return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
	//INT8U err;

	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(SpiFlashDevSem, 0) == osOK)
	{
		/* Select the FLASH: Chip Select low */
		SPI_FLASH_CS_LOW();

		/* Send "Read from Memory " instruction */
		SPI_FLASH_SendByte(READ);

		/* Send the 24-bit address of the address to read from -----------------------*/
		/* Send ReadAddr high nibble address byte */
		SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
		/* Send ReadAddr medium nibble address byte */
		SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
		/* Send ReadAddr low nibble address byte */
		SPI_FLASH_SendByte(ReadAddr & 0xFF);

		osSemaphoreRelease(SpiFlashDevSem);
	}
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t SPI_FLASH_SendByte(uint8_t data)
{
	uint8_t txbuf[2], rxbuf;
  
	txbuf[0] = data;
	txbuf[1] = '\0';

  if (HAL_SPI_TransmitReceive(&hspi1, txbuf, &rxbuf, 1, 10)!= HAL_OK)
  {
    // NOTE: !! IMPORTANT!! Not sent message
    Error_Handler();
  }
  
  return rxbuf;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(RDSR1);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	//printk("\nRead Status : %X\n", FLASH_Status);
  }
  while ((FLASH_Status & BUSY_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWEL
* Description    : Polls the status of the Write In Progress (WEL) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWEL(uint8_t WEL_Sts)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(RDSR1);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	//printk("\nRead WEL : %X\n", FLASH_Status);
  }
  while (((FLASH_Status & WEL_Flag)>>1) != WEL_Sts); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : dbg_sflash_read_id
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t dbg_sflash_read_id(void)
{
	uint32_t ret = 0;
	uint8_t buf[2];

	ret = SPI_FLASH_ReadID();
	buf[0] = (ret >> 8 ) & 0xFF;
	buf[1] = ret  & 0xFF;

	printd("\nManufacturer ID : ");
	
	if( buf[0] == 0xEF ){
		 printd("Winbond Serial Flash [0x%02X]\n",buf[0]);
	}
	else{
		 printd("Unknown\n");
	}
	printd("Device ID       : ");
	switch(buf[1]){
		case 0x15:printd("W25Q32FVSSIG 32Mb [0x%02X] \n",buf[1]);
			break;
		case 0x16:printd("W25Q64FVSSIG 64Mb [0x%02X]\n",buf[1]);
			break;
		default:printd("Unknown device ID [0x%02X]\n",buf[1]);
			break;
	}

	return NO_DISPLAY;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

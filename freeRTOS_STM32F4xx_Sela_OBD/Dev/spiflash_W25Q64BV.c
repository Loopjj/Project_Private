/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : spiflash.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : This file provides a set of functions needed to manage the
*                      communication between SPI peripheral and SPI M25P64 FLASH.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

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


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void flashSemaphore_Init(void)
{
	SPI_FLASH_CS_HIGH();
	SpiFlashDevSem = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(SpiFlashDevSem);
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
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
#if 0	
  INT8U err;

  OSSemPend(SPI_FLASH_DEV_SEM,0,&err);	  
  if( err != OS_ERR_NONE ){
	  printk("%s err\n",__func__);
	  return;
  }

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

  OSSemPost(SPI_FLASH_DEV_SEM);
#endif  
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageErase
* Description    : Erases the specified FLASH Page.
* Input          : PageAddr: address of the PageA to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageErase(u32 PageAddr)
{
	//INT8U err;

	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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
		
    xSemaphoreGive(SpiFlashDevSem);
	}

}

/*******************************************************************************
* Function Name  : SPI_FLASH_BlockErase
* Description    : Erases the specified FLASH Block.
* Input          : BlockAddr: address of the Block to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BlockErase(u32 BlockAddr)
{

	//INT8U err;

	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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
		xSemaphoreGive(SpiFlashDevSem);
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
	//INT8U err;
	
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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
		xSemaphoreGive(SpiFlashDevSem);
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
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	//INT8U err;

	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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
		xSemaphoreGive(SpiFlashDevSem);
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
INT8U SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  //INT8U retval = 0;


  Addr = WriteAddr % SPI_FLASH_PAGE_SIZE;
  count = SPI_FLASH_PAGE_SIZE - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PAGE_SIZE;

  //printk("\n##### WriteAddr:%X, Addr:%X, NumByteToWrite:%d, count:%d, NumOfPage:%d, NumOfSingle:%d, pBuffer[0]:%X, pBuffer[end]:%X", WriteAddr,Addr,NumByteToWrite,count,NumOfPage,NumOfSingle,pBuffer[0],pBuffer[NumByteToWrite-1]);
//printk("##### WriteAddr:%X, NumByteToWrite:%d\n", WriteAddr,NumByteToWrite);

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
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
	//INT8U err;
	
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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
		xSemaphoreGive(SpiFlashDevSem);
	}
}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0;
	//INT8U err;

	
	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return 0x0;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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

		xSemaphoreGive(SpiFlashDevSem);
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
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
	//INT8U err;

	if(SpiFlashDevSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(SpiFlashDevSem, ( portTickType ) 0 )  == pdTRUE)
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

		xSemaphoreGive(SpiFlashDevSem);
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
u8 SPI_FLASH_ReadByte(void)
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
u8 SPI_FLASH_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_DEV, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI_FLASH_DEV, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_DEV, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI_FLASH_DEV);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_DEV, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  SPI_I2S_SendData(SPI_FLASH_DEV, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_DEV, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI_FLASH_DEV);
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
  u8 FLASH_Status = 0;

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
void SPI_FLASH_WaitForWEL(INT8U WEL_Sts)
{
  u8 FLASH_Status = 0;

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

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

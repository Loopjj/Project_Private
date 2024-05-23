/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : spiflash.h
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : Header for spi_flash.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SPI_FLASH_H
#define SPI_FLASH_H


#define SPI_FLASH_PAGE_SIZE		0x100     //Write Page Size(256 byte)
/*64Mbit(8M byte)*/
#define SPI_FLASH_SECTOR			0x1000
#define SPI_FLASH_32BLOCK			0x8000
#define SPI_FLASH_64BLOCK			0x10000

#define SPI_FLASH_SECTOR_SIZE		SPI_FLASH_SECTOR

#define SPI_FLASH_DEV				SPI1

/* Extern --------------------------------------------------------------------*/
#ifdef SPI_FLASH_GLOBALS
#define SPI_FLASH_EXT
#else
#define SPI_FLASH_EXT extern
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define GPIO_CS                  GPIOA
#define GPIO_Pin_CS              GPIO_Pin_4


/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)



/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
SPI_FLASH_EXT void flashSemaphore_Init(void);
SPI_FLASH_EXT void SPI_FLASH_PageErase(u32 PageAddr);
SPI_FLASH_EXT void SPI_FLASH_BlockErase(u32 BlockAddr);
//SPI_FLASH_EXT void SPI_FLASH_SectorErase(u32 SectorAddr);
SPI_FLASH_EXT void SPI_FLASH_ChipErase(void);
SPI_FLASH_EXT void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
SPI_FLASH_EXT INT8U SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
SPI_FLASH_EXT void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);
SPI_FLASH_EXT u32 SPI_FLASH_ReadID(void);
SPI_FLASH_EXT void SPI_FLASH_StartReadSequence(u32 ReadAddr);
SPI_FLASH_EXT void SPI_FLASH_BinPageSize(void);


SPI_FLASH_EXT void App_SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);
//SPI_FLASH_EXT void GPIO_SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead, u8 ReadStatus);
//SPI_FLASH_EXT void GPIO_SPI_Flash_Init(void);


/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_WaitForWEL(INT8U WEL_Sts);

void Flash_Write_SaveData(void);

#endif /* spiflash_W25Q64BV.H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

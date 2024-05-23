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
SPI_FLASH_EXT SPI_HandleTypeDef hspi1;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,GPIO_PIN_RESET)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,GPIO_PIN_SET)


/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
SPI_FLASH_EXT void MX_SPI1_Init(void);
SPI_FLASH_EXT void SPI_Flash_Init(void);
SPI_FLASH_EXT void SPI_FLASH_PageErase(uint32_t PageAddr);
SPI_FLASH_EXT void SPI_FLASH_BlockErase(uint32_t BlockAddr);
SPI_FLASH_EXT void SPI_FLASH_SectorErase(uint32_t SectorAddr);
SPI_FLASH_EXT void SPI_FLASH_ChipErase(void);
SPI_FLASH_EXT void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
SPI_FLASH_EXT INT8U SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
SPI_FLASH_EXT void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
SPI_FLASH_EXT uint32_t SPI_FLASH_ReadID(void);
SPI_FLASH_EXT void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
SPI_FLASH_EXT void SPI_FLASH_BinPageSize(void);
SPI_FLASH_EXT uint16_t dbg_sflash_read_id(void);

/*----- Low layer function -----*/
uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t data);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_WaitForWEL(uint8_t WEL_Sts);


#endif /* spiflash_W25Q64BV.H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

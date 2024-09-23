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


/* Extern --------------------------------------------------------------------*/
#ifdef SPI_FLASH_GLOBALS
#define SPI_FLASH_EXT
#else
#define SPI_FLASH_EXT extern
#endif


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define GPIO_CS                 GPIOA
#define GPIO_Pin_CS             GPIO_Pin_4 

#define SPIx	SPI1
#define RCC_APBxPeriph_SPIx		RCC_APB2Periph_SPI1


/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)
/* M25P64 SPI FLASH Select Pin Control :  */
//#define SPI_FLASH_CS_MCU()       GPIO_ResetBits(GPIOB, GPIO_Pin_0); GPIO_SetBits(GPIOB, GPIO_Pin_1);
//#define SPI_FLASH_CS_FPGA()      GPIO_SetBits(GPIOB, GPIO_Pin_0); GPIO_ResetBits(GPIOB, GPIO_Pin_1);


/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
SPI_FLASH_EXT void SPI_FLASH_PageErase(u32 PageAddr);
SPI_FLASH_EXT void SPI_FLASH_SectorErase(u32 SectorAddr);
SPI_FLASH_EXT void SPI_FLASH_BlockErase(u32 BlockAddr);
SPI_FLASH_EXT void SPI_FLASH_BulkErase(void);
SPI_FLASH_EXT void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
SPI_FLASH_EXT void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
SPI_FLASH_EXT void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);
SPI_FLASH_EXT u32 SPI_FLASH_ReadID(void);
SPI_FLASH_EXT void SPI_FLASH_StartReadSequence(u32 ReadAddr);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_WaitForWEL(INT8U WEL_Sts);


#endif /* SPI_FLASH_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

/*
********************* (C) COPYRIGHT 2014  ***************END OF FILE*****************************
*/

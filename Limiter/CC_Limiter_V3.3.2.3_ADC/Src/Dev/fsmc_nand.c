/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : fsmc_nand.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/11/2017
*
* Description        : nand falsh for fsmc
*
*********************************************************************************************************
*/
/* USER CODE BEGIN 0 */

/* Includes ------------------------------------------------------------------*/
#define NAND_GLOBALS

#include <string.h>
#include "includes.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DATA_AREA                   ((uint32_t)0x00000000) 
#define PROGRAM_TIMEOUT	            0xF00000

/* Private variables ---------------------------------------------------------*/
#ifdef NAND_DBG_ENABLE
uint8_t TxBuffer [NAND_PAGE_SIZE];
uint8_t RxBuffer [NAND_PAGE_SIZE];
#endif

extern NAND_HandleTypeDef hnand1;
NAND_IDTypeDef NAND_ID;

/* Private function prototypes -----------------------------------------------*/

osSemaphoreDef(FSMC_NandSem);
osSemaphoreId FSMC_NandSem = NULL;
void Nand_Read_BadblockData(void);
u8 RefreshBadblockTable;
/* FSMC initialization function */
/******************************************************************************
* Function Name  : MX_FSMC_Init
* Description    : FSMC initialization function
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void MX_FSMC_Init(void)
{
  FSMC_NAND_PCC_TimingTypeDef ComSpaceTiming;
  FSMC_NAND_PCC_TimingTypeDef AttSpaceTiming;

  /** Perform the NAND1 memory initialization sequence
  */
  hnand1.Instance = FSMC_NAND_DEVICE;
  /* hnand1.Init */
  hnand1.Init.NandBank = FSMC_NAND_BANK2;
  hnand1.Init.Waitfeature = FSMC_NAND_PCC_WAIT_FEATURE_ENABLE;
  hnand1.Init.MemoryDataWidth = FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
  hnand1.Init.EccComputation = FSMC_NAND_ECC_DISABLE;
  hnand1.Init.ECCPageSize = FSMC_NAND_ECC_PAGE_SIZE_256BYTE;
  /* hnand1.Config */
  hnand1.Config.PageSize =      0x1000;   //4096
  hnand1.Config.SpareAreaSize = 0xe0;     //224
  hnand1.Config.BlockSize =     0x80;     //128
  hnand1.Config.BlockNbr =      0x800;    //2048 
  hnand1.Config.PlaneNbr =      0x2;      //2
  hnand1.Config.PlaneSize =     0x400;    //1024
  hnand1.Config.ExtraCommandEnable = DISABLE;
#if 1 
  hnand1.Init.TCLRSetupTime = 0xa1;
  hnand1.Init.TARSetupTime = 0x29;  
  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime =      0xf5;
  ComSpaceTiming.WaitSetupTime =  0xf3;
  ComSpaceTiming.HoldSetupTime =  0xf2;
  ComSpaceTiming.HiZSetupTime =   0xf5;
#else  
  hnand1.Init.TCLRSetupTime = 0x12;
  hnand1.Init.TARSetupTime = 0x18;  
  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime =      7;
  ComSpaceTiming.WaitSetupTime =  15;
  ComSpaceTiming.HoldSetupTime =  8;
  ComSpaceTiming.HiZSetupTime =   9;
#endif  
  /* AttSpaceTiming */
  AttSpaceTiming.SetupTime =      ComSpaceTiming.SetupTime;
  AttSpaceTiming.WaitSetupTime =  ComSpaceTiming.WaitSetupTime;
  AttSpaceTiming.HoldSetupTime =  ComSpaceTiming.HoldSetupTime;
  AttSpaceTiming.HiZSetupTime =   ComSpaceTiming.HiZSetupTime;

  if (HAL_NAND_Init(&hnand1, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  FSMC_NAND_Reset();
}

/******************************************************************************
* Function Name  : FSMC Nand Init
* Description    : This routine semaphore init the NAND FLASH
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void FSMC_Nand_Init(void)
{
	FSMC_NandSem = osSemaphoreCreate(osSemaphore(FSMC_NandSem) , 1);
  Check_Nand_Id();
  Nand_Read_BadblockData();
}

/******************************************************************************
* Function Name  : FSMC_NAND_ReadData
* Description    : This routine read data the NAND FLASH
* Input          : cmd, addr, buf, size
* Output         : None
* Return         : Error
* Attention		   : None
*******************************************************************************/

INT8U FSMC_NAND_ReadData(INT8U cmd, INT8U add, INT8U *targer,INT8U size)
{
	uint32_t i=0;

	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{  

		/* Send Command to the command area */ 	
		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = cmd;
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = add;

		for(i=0;i<size;i++)
		{
			targer[i]= *(__IO uint8_t *)(NAND_DEVICE1 | DATA_AREA);
		}
    osSemaphoreRelease(FSMC_NandSem);
	}
	return ERROR;
}

/******************************************************************************
* Function Name  : MemoryDisplay
* Description    : This routine memory display the NAND FLASH
* Input          : offset, size
* Output         : None
* Return         : Error
* Attention		   : None
*******************************************************************************/
void MemoryDisplay(INT32U offset, INT32U size)
{
	uint32_t i,j;
	uint8_t temp,buf[0x10];
	uint8_t *pMem = (INT8U *)offset;

	while(size%16)size++;

	printk("\n");

	for(i=0; i<size; i++)
	{
		temp = pMem[i];
		
		if((i&0xF) == 0){
			if(i){
				for(j=0; j<0x10; j++){
					if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
						printk("%c",buf[j]);
					}
					else{
						printk(".");
					}
				}
			}
			printk("\n%05X : ",offset+i);
		}
		
		buf[i&0xF] = temp;
		
		printk("%02X ",temp);
	}

	for(j=0; j<0x10; j++){
		if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
			printk("%c",buf[j]);
		}
		else{
			printk(".");
		}
	}
	
	printk("\n");	
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
* Attention		   : None 
*******************************************************************************/
u8 Nand_Status;
uint32_t FSMC_NAND_ReadStatus(void)
{
  uint32_t data = 0x00, status = NAND_BUSY;

  /* Read status operation ------------------------------------ */
  *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_STATUS; 
  data = *(__IO uint8_t *)(NAND_DEVICE1); 
  Nand_Status = data;
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
* Function Name  : FSMC_NAND_GetStatus
* Description    : Get the NAND operation status
* Input          : None
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation  
* Attention		   : None  
*******************************************************************************/
uint32_t FSMC_NAND_GetStatus(void)
{
  uint32_t timeout = PROGRAM_TIMEOUT, status = NAND_READY;

  status = FSMC_NAND_ReadStatus(); 

  /* Wait for a NAND operation to complete or a TIMEOUT to occur */
  while ((status != NAND_READY && status != NAND_ERROR) &&( timeout != 0x00))
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
* Attention		   : None
*******************************************************************************/
uint32_t FSMC_NAND_WriteSmallPage(uint8_t *pBuffer, uint32_t Address, uint32_t size)
{
	uint32_t index = 0x00;
	uint32_t status = NAND_READY;
	uint32_t caddr,paddr;
	
	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{  
    if(Address >= NAND_SIZE)
      return NAND_INVALID_ADDRESS;
    
		caddr = Address%NAND_PAGE_SIZE;
		paddr = Address/NAND_PAGE_SIZE;

    /* Page write command and address */
    *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_A;      
    *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_WRITE0;

    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF);

    /* Write data */
    for(index=0; index < size; index++)
    {
      *(__IO uint8_t *)(NAND_DEVICE1 | DATA_AREA) = pBuffer[index];
    }
      
    *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_WRITE_TRUE1;

    /* Check status for successful operation */
    status = FSMC_NAND_GetStatus();
    //printk("FSMC_NAND_WriteSmallPage status = %02X\n",status);
		    
    osSemaphoreRelease(FSMC_NandSem);
		return status;
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
	uint32_t index = 0x00;
	uint32_t status = NAND_READY;
	uint32_t caddr,paddr;
	
	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{ 
    if(Address >= NAND_SIZE)
      return NAND_INVALID_ADDRESS;
    
		caddr = Address%NAND_PAGE_SIZE;
		paddr = Address/NAND_PAGE_SIZE;
    
    //printk("\nAddress = 0x%08X caddr = 0x%02X paddr = 0x%08X\n",Address, caddr, paddr);
     
    /* Page Read command and page address */
    *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_A; 
     
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
    *(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF);
      
    *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_TRUE1; 
           
    /* Get Data into Buffer */    
    for(index=0; index < size; index++)
    {
      pBuffer[index]= *(__IO uint8_t *)(NAND_DEVICE1 | DATA_AREA);
    }

		status = FSMC_NAND_GetStatus();
    osSemaphoreRelease(FSMC_NandSem);
		return status;
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
* Attention		   : None
*******************************************************************************/
uint32_t FSMC_NAND_WriteSpareArea(uint8_t *pBuffer, uint32_t Address, uint32_t size)
{
	uint32_t index,status = NAND_ERROR;
	uint32_t caddr;
	uint32_t paddr;
	uint32_t add = Address;

	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{ 
		add = add & (~(Nand_Block_Size-1));
		caddr = NAND_PAGE_SIZE;
		paddr = add/NAND_PAGE_SIZE;
		//printk("\nWR add = 0X%08X, caddr = 0X%08X, paddr = 0X%08X \n",add,caddr,paddr);

		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_WRITE0;

		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF); 

		/* Write the data */ 
		for(index=0; index < size; index++)
		{
		  *(__IO uint8_t *)(NAND_DEVICE1 | DATA_AREA) = pBuffer[index];
		}
	    
		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_WRITE_TRUE1;

		status = FSMC_NAND_GetStatus();
    osSemaphoreRelease(FSMC_NandSem);
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
	uint32_t index,status;
	uint32_t caddr;
	uint32_t paddr;
	uint32_t add = Address;

	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{ 
		add = add & (~(Nand_Block_Size-1));
		caddr = NAND_PAGE_SIZE;
		paddr = add/NAND_PAGE_SIZE;
//		printk("\nRD add = 0X%08X, caddr = 0X%08X, paddr = 0X%08X \n",add,caddr,paddr);

		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_A;

		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( caddr&0xFF     );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((caddr>>8)&0x0F );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( paddr&0xFF     );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>8)&0xFF );
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ((paddr>>16)&0xFF); 

		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_TRUE1;

		/* Get Data into Buffer */
		for (index=0;index < size; index++)
		{
		  pBuffer[index] = *(__IO uint8_t *)(NAND_DEVICE1 | DATA_AREA);
		}
		
		status = FSMC_NAND_GetStatus();
    osSemaphoreRelease(FSMC_NandSem);
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
	uint32_t status = NAND_ERROR;
	uint8_t addr1,addr2,addr3;

	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{ 
		addr1 = Address;
		addr2 = Address >> 8;
		addr3 = Address >> 16;

		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_ERASE0;
	  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = addr1;  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = addr2;
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = addr3;
			
		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_ERASE1; 
    delay_ms(2);

		status = FSMC_NAND_GetStatus();
    osSemaphoreRelease(FSMC_NandSem);
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

  *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_RESET;

  return (NAND_READY);
}

/******************************************************************************
* Function Name  : FSMC_NAND_Badblock
* Description    : This routine badblock check the NAND FLASH
* Input          : None
* Output         : None
* Return         : NAND_READY
* Attention		 : None
*******************************************************************************/
INT8U FSMC_NAND_Badblock(INT32U addr,INT32U *err)
{
	uint16_t caddr;
	uint32_t paddr;
	uint8_t  ret=0;

	if(FSMC_NandSem == NULL)
	{
		return ERROR;
	}
	if(osSemaphoreWait(FSMC_NandSem, 100) == osOK)
	{ 
		addr = addr & (~(Nand_Block_Size-1));
		caddr = NAND_PAGE_SIZE;
		paddr = addr/NAND_PAGE_SIZE;
		//printk("add = 0X%08X, caddr = 0X%08X, paddr = 0X%08X ",addr,caddr,paddr);

		 *(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_A;

		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( caddr&0xFF      );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( (caddr>>8)&0x0F ); 
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( paddr&0xFF      );  
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( (paddr>>8)&0xFF );
		*(__IO uint8_t *)(NAND_DEVICE1 | ADDR_AREA) = (uint8_t) ( (paddr>>16)&0x03);  

		*(__IO uint8_t *)(NAND_DEVICE1 | CMD_AREA) = NAND_CMD_AREA_TRUE1;

		/* Get Data into Buffer */    
		ret= *(__IO uint8_t *)(NAND_DEVICE1 | DATA_AREA);
		
		*err = FSMC_NAND_GetStatus();
		//printk("  %d[%X]\n",ret,*err);
    osSemaphoreRelease(FSMC_NandSem);
		return ret;
	}
	return ret;

}

/******************************************************************************
* Function Name  : Nand_Read_BadblockData
* Description    : This routine read badblock data the NAND FLASH
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void Nand_Read_BadblockData(void)
{
	uint16_t i, j, badcount;    
  uint32_t addr,err;
	uint32_t baseAddr=NAND_BADBLOCK_BASE;
  uint8_t status = 0;
	st_BadBlockData badblock;
	uint8_t checksum;         
  uint8_t max_badcount;
  
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
	if(i == 10) return;
	max_badcount = NAND_BAD_BLOCK_NUM-i;

  if(!RefreshBadblockTable) {
    Nand_Read_Buffer((uint8_t*)&badblock, baseAddr, sizeof(st_BadBlockData));
    if(badblock.BadBlockCount <= NAND_BAD_BLOCK_NUM) 
    {
      checksum = badblock.BadBlockCount;
      for(i = 0; i < badblock.BadBlockCount; i++) 
      {
        checksum += badblock.BadBlockAddr[i]&0xff;
        checksum += (badblock.BadBlockAddr[i]>>8)&0xff;
      }
      if(checksum == badblock.BadBlockCheckSum) { 
        BadBlockData = badblock;
        return;
      }
    }
  }
	memset((uint8_t*)&badblock,0,sizeof(st_BadBlockData));
	for(i = 0; i < Nand_Block_Num; i++) {
		addr = i*Nand_Block_Size+NAND_PAGE_SIZE;
		status = FSMC_NAND_Badblock(addr, &err);
		if(status == 0) {
      badcount = badblock.BadBlockCount;
      for(j = badcount; j < max_badcount; j++) 
      {
        addr = j * NAND_BLOCK_SIZE + NAND_BADBLOCK_DATA_ADDR + NAND_PAGE_SIZE;
        status = FSMC_NAND_Badblock(addr, &err);
        if(status == 0) 
        {
          badblock.BadBlockAddr[badblock.BadBlockCount++] = Nand_Bad_Check_Block_Num;   
        }
        else
          break;
      }      
			if(badblock.BadBlockCount < max_badcount)
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
  RefreshBadblockTable = 0;
}

/******************************************************************************
* Function Name  : Nand_Check_Badblock
* Description    : This routine badblock check the NAND FLASH
* Input          : addr
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
INT32U Nand_Check_Badblock(INT32U addr)
{
	uint16_t i, block_index = 0;
	uint32_t new_addr;
  
	if(BadBlockData.BadBlockCount > 0 &&BadBlockData.BadBlockCount <= NAND_BAD_BLOCK_NUM) 
	{
		block_index = addr/Nand_Block_Size;
    if(block_index < Nand_Bad_Check_Block_Num) 
    {    
      for(i = 0; i < BadBlockData.BadBlockCount; i++) 
      {
        if(BadBlockData.BadBlockAddr[i] == block_index) 
        {
          new_addr = NAND_BADBLOCK_DATA_ADDR + i * NAND_BLOCK_SIZE + (addr % Nand_Block_Size);
          addr = new_addr; 
          block_index = addr/Nand_Block_Size;
        }	
      }
    }
	}
	return addr;
}

/******************************************************************************
* Function Name  : Nand_Write_Buffer
* Description    : This routine write buffer the NAND FLASH
* Input          : buffer, addr, length
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
INT32U Nand_Write_Buffer(INT8U *pBuffer, INT32U addr, INT16U len)
{
  uint32_t status = NAND_READY,eaddr, i;
   
  if(len == 0) {
    return 0;
  }
  addr = Nand_Check_Badblock(addr);
  eaddr = addr+len;
 
  if(addr%Nand_Block_Size == 0) {//0x40000 = one block size
    for(i = 0; i < 5; i++) {
      status = FSMC_NAND_EraseBlock(addr/NAND_PAGE_SIZE); 
      if(status == NAND_READY) break;
      else delay_ms(1);
    }
  }
  else if((addr%Nand_Block_Size)+len > Nand_Block_Size) {//0x40000 = one block size
    for(i = 0; i < 5; i++) {
      status = FSMC_NAND_EraseBlock(eaddr/NAND_PAGE_SIZE); 
      if(status == NAND_READY) break;
      else delay_ms(1);
    }
  }

  for(i = 0; i < 5; i++) {
    status = FSMC_NAND_WriteSmallPage(pBuffer,addr,len);
    if(status == NAND_READY) break;
    else delay_ms(1);
  }
  
  return status;
}

/******************************************************************************
* Function Name  : Nand_Read_Buffer
* Description    : This routine read buffer the NAND FLASH
* Input          : buffer, addr, length
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
INT32U Nand_Read_Buffer(INT8U *pBuffer, INT32U addr, INT16U len)
{
	uint32_t status, i;

	addr = Nand_Check_Badblock(addr);
  for(i = 0; i < 5; i++) {
    status = FSMC_NAND_ReadSmallPage(pBuffer,addr, len);
    if(status == NAND_READY) break;
    else delay_ms(1);
  }
  
	return status;
}

#ifdef NAND_DBG_ENABLE
/******************************************************************************
* Function Name  : NAND Debug Function
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandWriteRead_test(void)
{
	uint32_t addr,index,ret;
	uint8_t data;

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
	MemoryDisplay((uint32_t)RxBuffer,NAND_PAGE_SIZE);
	printk("....NAND Read return = %02X\n",ret);

	return NO_DISPLAY;
}

/******************************************************************************
* Function Name  : dbg_nandErase_test
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandErase_test(void)
{
	uint32_t addr,ret;
  
	if(CLI_GetArgCnt() < 1) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));

	ret = FSMC_NAND_EraseBlock(addr/NAND_PAGE_SIZE);
	printk("\n[%02X]NAND Erase Block 0x%08X\n",ret,addr/NAND_PAGE_SIZE);
	return NO_DISPLAY;

}

/******************************************************************************
* Function Name  : dbg_nandWrite_test
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandWrite_test(void)
{
	uint32_t addr,ret,index;
	uint8_t data;

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

/******************************************************************************
* Function Name  : dbg_nandread_test
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandread_test(void)
{
	uint8_t cmd,addr,size;
	uint8_t tmp[50];


	if(CLI_GetArgCnt() < 3) return INV_PRM;
	cmd = comparison_num((char *)P_ARG(1));
	addr = comparison_num((char *)P_ARG(2));
	size = comparison_num((char *)P_ARG(3));

	FSMC_NAND_ReadData(cmd,addr,tmp,size);
	MemoryDisplay((INT32U)tmp,size);
		
	return NO_DISPLAY;
}

/******************************************************************************
* Function Name  : dbg_nandblockread_test
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandblockread_test(void)
{
	uint32_t addr;
	//uint8_t RxBuffer [NAND_PAGE_SIZE];

	if(CLI_GetArgCnt() < 1) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));
  
	FSMC_NAND_ReadSmallPage (RxBuffer, addr, NAND_PAGE_SIZE);
	printk("\nNAND Page Read address = 0x%08X\n",addr);
	MemoryDisplay((uint32_t)RxBuffer,NAND_PAGE_SIZE);

	return NO_DISPLAY;
}

/******************************************************************************
* Function Name  : dbg_nandspareread_test
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandspareread_test(void)
{
	uint32_t addr;

	if(CLI_GetArgCnt() < 1) return INV_PRM;
	addr = comparison_num((char *)P_ARG(1));

	FSMC_NAND_ReadSpareArea (RxBuffer, addr, 1);
	printk("\nNAND Spare Read address = 0x%04X\n",addr);
	MemoryDisplay((uint32_t)RxBuffer,NAND_SPARE_AREA_SIZE);

	return NO_DISPLAY;
}

/******************************************************************************
* Function Name  : dbg_nandsparewrite_test
* Attention		   : None
*******************************************************************************/
INT16U dbg_nandsparewrite_test(void)
{
	uint32_t addr,ret,index;
	uint8_t data=0;

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

#endif
/******************************************************************************
* Function Name  : CMD_NAND_checkbb
* Attention		   : None
*******************************************************************************/
INT16U CMD_NAND_checkbb(void)
{
	uint32_t i, bbcnt=0,err;
	uint8_t bbstat;

	printk("\n");
	for(i=0;i<NAND_SIZE/Nand_Block_Size;i++)
	{
		//printk("[%04d] ",i);
		bbstat = FSMC_NAND_Badblock(Nand_Block_Size*i, &err);
		if ( bbstat != 255 ) {
			printk("[%X]ADDR[%04X]:0x%08X : %d",err,i,Nand_Block_Size*i,bbstat);
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

/******************************************************************************
* Function Name  : CMD_NAND_id
* Attention		   : None
*******************************************************************************/
u8 Nand_Id[10];
void Check_Nand_Id(void)
{
  u8 nand_type;
  u16 addr;
    
  addr = DRV_SETTING_PARM + (u8 *)&Setting.NandType - (u8 *)&Setting; 
  FRAMByteRead((u8 *)&nand_type, addr);
  Nand_Block_Count = NAND_BLOCK_COUNT; //Default MT29F8G08ABABA 8Gb
  Nand_Block_Size = NAND_PAGE_SIZE*Nand_Block_Count;
  Nand_Block_Num = NAND_BLOCK_NUM; //Default MT29F8G08ABABA 8Gb
  Nand_Bad_Check_Block_Num = NAND_BAD_CHECK_BLOCK_NUM;
  
	FSMC_NAND_ReadData(NAND_CMD_READID, 0, Nand_Id, 5);
  if(Nand_Id[0] == 0x2C) {  //Micron
    switch(Nand_Id[1]){
      case 0x38: //MT29F8G08ABABA 8Gb, x8, 3.3V
        if(nand_type != 1) {
          nand_type = 1;
          FRAMByteWrite(nand_type, addr);
          RefreshBadblockTable = 1;
        }
        break;
      case 0xD3: //MT29F8G08ABACA, MT29F8G08ADAFA 8Gb, x8, 3.3V
        if(Nand_Id[3] == 0xA6) {
          Nand_Block_Count = 64;
          Nand_Block_Size = NAND_PAGE_SIZE*Nand_Block_Count;
          Nand_Block_Num = 4096;
          Nand_Bad_Check_Block_Num = Nand_Block_Num-58*2;
          if(nand_type != 2) {
            nand_type = 2;
            FRAMByteWrite(nand_type, addr);
            RefreshBadblockTable = 1;
          }
        }
        break;
      default: //Unknown device ID
        break;
    }
  }
  return;
}

INT16U CMD_NAND_id(void)
{
	uint8_t rdata[10]={0};

	FSMC_NAND_ReadData(NAND_CMD_READID, 0, rdata, 5);

	printk("\nID 0 read\n");
	MemoryDisplay((uint32_t)rdata,5);

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
		case 0x38:printk("MT29F8G08ABABA 8Gb, x8, 3.3V\n");
			break;
		default:printk("Unknown device ID\n");
			break;
	}
   
  return NO_DISPLAY;
} 


/*
********************************************************************************
* Description : Nand_Read_SecData
* Arguments   : *buffer, address, length
* Return      : 
* Note        : Read Nand sec data
******************************************************************************** 
*/

void Nand_Read_SecData(u8 *pBuffer, u32 addr, u16 len)
{
  if(addr < DataSaveAddr && addr >= DataSaveAddr - (DataSaveAddr%NAND_PAGE_SIZE) ) {
    memcpy(pBuffer, (u8*)(Nand_Save_Buff+(addr%NAND_PAGE_SIZE)),len);
  }
  else {
    Nand_Read_Buffer(pBuffer, addr, len);
  }
}

/*
********************************************************************************
* Description : Nand_Write_Buffer
* Arguments   : *buffer, address, length
* Return      : 
* Note        : Write Nand Data
******************************************************************************** 
*/
void Nand_Write_SaveData(void)
{
  u16 size;
  u16 n;
  
  if(!Save_SecData_Flag) return; 

  if(DataSaveAddr%64)
    DataSaveAddr = (DataSaveAddr+(64-DataSaveAddr%64));
  
  size = sizeof(tsSecData);
  for(n = 0; n < Save_SecData_Cnt; n++) {
    memcpy((u8*)(Nand_Save_Buff+BK_Sram_Addr),(u8*)&Save_SecData_Buff[n],size);
    DataSaveAddr = (DataSaveAddr+size)%NAND_SIZE_DATA;
    BK_Sram_Addr = DataSaveAddr%NAND_PAGE_SIZE;

    if(BK_Sram_Addr == 0) {
      Nand_Write_Buffer(Nand_Save_Buff, DataSaveAddr-NAND_PAGE_SIZE, NAND_PAGE_SIZE);
      memset(Nand_Save_Buff,0,NAND_PAGE_SIZE);
    }
    DailyData.NoOfSecData++;
  }
  // RTC bk ram redundancy start
  RTC_BK_Addr->Save_Addr = DataSaveAddr;
  RTC_BK_Addr->Sent_Addr = DataSentAddr;
  
  FRAMMultiWrite((u8 *)&DataSaveAddr,DRV_SAVE_ADDR,4); 
  FRAMMultiWrite((u8 *)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
  // RTC bk ram redundancy end

  Save_SecData_Cnt = 0;
  Save_SecData_Flag = 0;
}

/*
********************************************************************************
* Description : Nand_Read_DailyData
* Arguments   : length, address 
* Return      : 
* Note        : Write Nand Daily Data
******************************************************************************** 
*/
u8 Nand_Read_DailyData(u16 prev_cnt, u8* dest)
{
  u32 addr;
  u32 page_cnt;
  u32 index;
  u8 resp = 0;
  
  if(prev_cnt > DailyData.Index) 
    resp = 0;
  else 
    resp = 1;
 
  if(prev_cnt == 0 || DailyData.Index == 0) {
    memcpy(dest,(u8*)&DailyData,sizeof(stDailyData));
    return resp;
  }
  if(prev_cnt > NAND_DAILY_DATA_PAGE_SIZE - (2 * NAND_BLOCK_COUNT)) {
    prev_cnt = NAND_DAILY_DATA_PAGE_SIZE - (2 * NAND_BLOCK_COUNT);
    resp = 0;
  }
  if(prev_cnt > DailyData.Index)
    index = 0;
  else 
    index = DailyData.Index-prev_cnt;
  
  page_cnt = index%NAND_DAILY_DATA_PAGE_SIZE;
  addr = NAND_DAILY_DATA_ADDR + page_cnt*NAND_PAGE_SIZE;
  Nand_Read_Buffer(dest, addr, sizeof(stDailyData));
  return resp;
}


/*
********************************************************************************
* Description : Nand_Read_DailyData_LG
* Arguments   : day_data 
* Return      : 
* Note        : Read Nand Daily Data
******************************************************************************** 
*/
u8 Nand_Read_DailyData_LG(u8* day_data)
{
  rtc_time rt;
  time_t reqt;
  u32 day_cnt, i;
  u8 valid_flag = 0;
  
  rt.tm_year = day_data[0];
  rt.tm_mon = day_data[1];
  rt.tm_mday = day_data[2];
  rt.tm_hour = 0;
  rt.tm_min = 0;
  rt.tm_sec = 0;
  reqt = RTCValTotime(rt);
  
  day_cnt = CalDayCnt(reqt, RTC_LVal);
  
  if(day_cnt > 180) day_cnt = 180;
  
  for(i = 0; i <= day_cnt; i++) {
    IWDG_ReloadCounter();  
    memset((u8*)&TmpDailyData, 0, sizeof(stDailyData)); 
    Nand_Read_DailyData(day_cnt-i, (u8*)&TmpDailyData);
    if(IsSameDay(TmpDailyData.Time, reqt)) {
      valid_flag = 1;
      return valid_flag;
    }
  }
  return valid_flag;
}

/*
********************************************************************************
* Description : Nand_Read_DailySetting
* Arguments   : length, address 
* Return      : 
* Note        : Write Nand Daily Setting
******************************************************************************** 
*/
u8 Nand_Read_DailySetting(u16 prev_cnt, u8* dest)
{
  u32 addr;
  u32 page_cnt;
  u32 index;
  u8 resp = 0;
  
  if(prev_cnt > DailyData.Index) 
    resp = 0;
  else 
    resp = 1;
 
  if(prev_cnt == 0 || DailyData.Index == 0) {
    memcpy(dest,(u8*)&Setting,sizeof(st_Setting));
    return resp;
  }
  if(prev_cnt > NAND_DAILY_DATA_PAGE_SIZE - 2*NAND_BLOCK_COUNT) {
    prev_cnt = NAND_DAILY_DATA_PAGE_SIZE - 2*NAND_BLOCK_COUNT;
    resp = 0;
  }
  if(prev_cnt > DailyData.Index)
    index = 0;
  else 
    index = DailyData.Index-prev_cnt;
  
  page_cnt = index%NAND_DAILY_DATA_PAGE_SIZE;
  addr = NAND_DAILY_DATA_ADDR + page_cnt*NAND_PAGE_SIZE + sizeof(stDailyData);
  Nand_Read_Buffer(dest, addr, sizeof(st_Setting));
  return resp;
}

///*
//********************************************************************************
//* Description : Nand_Write_DailyData
//* Arguments   : Nonr
//* Return      : 
//* Note        : Write Nand Daily Data
//******************************************************************************** 
//*/
//u8 Nand_Read_DailyDriverInfo(u16 prev_cnt, u8* dest)
//{
//  u32 addr;
//  u32 page_cnt;
//  u32 index;
//  u8 resp = 0;
//  
//  if(prev_cnt > DailyData.Index) 
//    resp = 0;
//  else 
//    resp = 1;
// 
//  if(prev_cnt == 0 || DailyData.Index == 0) {
//    memcpy(dest,(u8*)&Driver_Info[0],sizeof(st_DRIVER_INFO)*9);
//    return resp;
//  }
//  if(prev_cnt > NAND_DAILY_DATA_PAGE_SIZE - 2*NAND_BLOCK_COUNT) {
//    prev_cnt = NAND_DAILY_DATA_PAGE_SIZE - 2*NAND_BLOCK_COUNT;
//    resp = 0;
//  }
//  if(prev_cnt > DailyData.Index)
//    index = 0;
//  else 
//    index = DailyData.Index-prev_cnt;
//  
//  page_cnt = index%NAND_DAILY_DATA_PAGE_SIZE;
//  addr = NAND_DAILY_DATA_ADDR + page_cnt*NAND_PAGE_SIZE + sizeof(stDailyData)+sizeof(st_Setting);
//  Nand_Read_Buffer(dest, addr, sizeof(st_DRIVER_INFO)*9);
//  return resp;
//}

/*
********************************************************************************
* Description : Nand_Write_DailyData
* Arguments   : Nonr
* Return      : 
* Note        : Write Nand Daily Data
******************************************************************************** 
*/
u8 NandDailyBuffer[1024];
void Nand_Write_DailyData(void)
{
  u32 addr;
  u32 page_cnt;
  
  page_cnt = DailyData.Index%NAND_DAILY_DATA_PAGE_SIZE;
  addr = NAND_DAILY_DATA_ADDR + page_cnt*NAND_PAGE_SIZE;
//  Nand_Write_Buffer((u8*)&DailyData, addr, sizeof(stDailyData));
  memset(NandDailyBuffer, 0, 1024);
  DailyData.DailyDataType = 1;
  memcpy(NandDailyBuffer, (u8*)&DailyData, sizeof(stDailyData));
  memcpy(&NandDailyBuffer[sizeof(stDailyData)], (u8*)&Setting, sizeof(st_Setting));
//  memcpy(&NandDailyBuffer[sizeof(stDailyData)+sizeof(st_Setting)], (u8*)&Driver_Info[0], sizeof(st_DRIVER_INFO)*9);
//  Nand_Write_Buffer(NandDailyBuffer, addr, sizeof(stDailyData)+sizeof(st_Setting)+sizeof(st_DRIVER_INFO)*9);
  Nand_Write_Buffer(NandDailyBuffer, addr, sizeof(stDailyData)+sizeof(st_Setting));
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : i2cfram_FM24CL64B.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.1
*
* Date               : 09/23/2015
*
* Description        : I2C_DEV line interface configuration
*
* FRAM Size			     : 64Kbit(8KByte) : 0x00 ~ 0x1FFF 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define I2CFRAM_GLOBALS
#include "includes.h"


#define I2C_BUF_SIZE	1024
#define TIMEOUT			  30


__IO I2C_Dir I2C_DIR;
__IO uint8_t I2C_BLOCKED    = 0;
__IO uint8_t I2CADDRESS     = 0;
__IO uint8_t rx_buffer_len  = 0;
__IO uint8_t tx_buffer_len  = 0;
__IO uint8_t tx_buffer_ix   = 0;
__IO uint8_t rx_buffer_ix   = 0;
__IO uint8_t tx_buffer[I2C_BUF_SIZE];
__IO uint8_t *rx_buffer_ptr;

/*
********************************************************************************
* Description : framSemaphore_Init
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
void framSemaphore_Init(void)
{
	BSP_I2C1Sem = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_I2C1Sem);
}

/*
********************************************************************************
* Description : i2c_write
* Arguments   : buffer, len
* Return      : 
* Note        : 
******************************************************************************** 
*/

uint8_t i2c_write(uint8_t *buffer, uint8_t len) 
{
  uint32_t startime;

	assert_param(len <= I2C_BUF_SIZE && len > 0);

	if(BSP_I2C1Sem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_I2C1Sem, ( portTickType ) 0 )  == pdTRUE)
	{
		// disable interrupts
		I2C_ITConfig(I2C_DEV, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, DISABLE);
	        
		/*
		 * check if I2C interace is in use
		*/
		if(I2C_BLOCKED == 0)
		{
			I2CADDRESS = SLAVE_ADDRESS;
			I2C_DIR = TX;
			// set buffer length and start index
			tx_buffer_len = len;
			tx_buffer_ix = 0;
			rx_buffer_len = 0;
			rx_buffer_ix = 0;
	                
			__IO uint8_t *tmp_buffer_ptr = tx_buffer;
			uint8_t i;
			for(i=0; i<len; ++i) 
			{
				*tmp_buffer_ptr++ = *buffer++; // copies faster than using indexed arrays
			}
	                    
			// start with ACK
			I2C_AcknowledgeConfig(I2C_DEV, ENABLE);
			// enable I2V2 event, buffer and error interrupt
			I2C_ITConfig(I2C_DEV, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);

			// notify that transfer has started
			I2C_BLOCKED = 1; 

			// send start condition
			I2C_GenerateSTART(I2C_DEV, ENABLE);
		}
		else
		{
			xSemaphoreGive(BSP_I2C1Sem);
			return ERROR;
		}
		/*
		 * wait till finished
		*/
		//while(I2C_BLOCKED == 1);
	 
		startime =  systick_uptime();
		while(I2C_BLOCKED == 1)
		{
		   if ((systick_uptime() - startime) > TIMEOUT)
		   {
				break;
		   }
		}
		if(I2C_BLOCKED == 1)	//time out
		{
			I2C_BLOCKED = 0;
			xSemaphoreGive(BSP_I2C1Sem);
			return ERROR;
		}
		xSemaphoreGive(BSP_I2C1Sem);
		return OK;
	}
	return ERROR;
}
 
/*
********************************************************************************
* Description : i2c_read
* Arguments   : tx_buf, txlen, rx_buffer, rxlen
* Return      : state
* Note        : 
******************************************************************************** 
*/ 

uint8_t i2c_read(uint8_t *tx_buf, uint8_t txlen, uint8_t *rx_buffer, uint8_t rxlen)
{
  uint32_t startime;

	assert_param(len <= I2C_BUF_SIZE && rxlen > 0);
    assert_param(txlen <= 2);

	if(BSP_I2C1Sem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_I2C1Sem, ( portTickType ) 0 )  == pdTRUE)
	{
		// disable interrupts
		I2C_ITConfig(I2C_DEV, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, DISABLE);
	                
		/*
		 * check if I2C interface is in use
		 */
		if(I2C_BLOCKED == 0)
		{
			I2CADDRESS = SLAVE_ADDRESS;
			I2C_DIR = TXREG;
			rx_buffer_len = rxlen;
			rx_buffer_ix = 0;        
			rx_buffer_ptr = rx_buffer;
			tx_buffer_ix = 0;
			tx_buffer_len = txlen;

			__IO uint8_t *tmp_buffer_ptr = tx_buffer;
			uint8_t i;
			for(i=0; i<txlen; ++i) {
				*tmp_buffer_ptr++ = *tx_buf++; // copies faster than using indexed arrays
			}
			// start with ACK
			I2C_AcknowledgeConfig(I2C_DEV, ENABLE);

			// enable I2V2 event, buffer and error interrupt
			I2C_ITConfig(I2C_DEV, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);

			// notify that transfer has started
			I2C_BLOCKED = 1; 

			// send start condition
			I2C_GenerateSTART(I2C_DEV, ENABLE);
		}
		else
		{
			xSemaphoreGive(BSP_I2C1Sem);
      return ERROR;
		}
		/*
		 * wait till finished
		*/
		startime =  systick_uptime();

		while(I2C_BLOCKED == 1)
		{
		   if ((systick_uptime() - startime) > TIMEOUT)
		   {
			   break;
		   }
		}
		if(I2C_BLOCKED == 1)	//time out
		{
			I2C_BLOCKED = 0;
			xSemaphoreGive(BSP_I2C1Sem);
			return ERROR;
		}
		xSemaphoreGive(BSP_I2C1Sem);
	  
		return OK;
	}
	return ERROR;
}
 

/*
********************************************************************************
* Description : i2c_is_busy
* Arguments   : none
* Return      : state
* Note        : 
******************************************************************************** 
*/  
uint8_t i2c_is_busy(void)
{
   return I2C_BLOCKED;
}

/*
********************************************************************************
* Description : eepwrite
* Arguments   : address, data
* Return      : state
* Note        : 
******************************************************************************** 
*/  
int eepwrite(int address,char data)
{
	int ret = 0;
	uint8_t buff[3];
	
	buff[0] = (address >> 8) & 0x1F;
	buff[1] = (address & 0xFF);
	buff[2] = data;
	ret = i2c_write(buff, 3);

	return ret;
}

/*
********************************************************************************
* Description : eepread
* Arguments   : address, data
* Return      : state
* Note        : 
******************************************************************************** 
*/  
int eepread(INT16U address, char *data)
{
	int ret = 0;
	uint8_t buff[2];
	
	buff[0] = (address >> 8) & 0x1F;
	buff[1] = (address & 0xFF);

	ret = i2c_read(buff, 2, (uint8_t *)data, 1);

	return ret;

}

/*
********************************************************************************
* Description : eepwrite_multi
* Arguments   : address, data, len
* Return      : state
* Note        : 
******************************************************************************** 
*/  
int eepwrite_multi(u16 addr, u8 *data, u16 len)
{
  u16 i, m, p, n;
  int ret = 0;
  u8 buff[32+2];
  int idx;
  
  m = 32-(addr%32);
  
  if(m >= len) {
    buff[0] = (addr >> 8) & 0x1F;
    buff[1] = (addr & 0xFF);
    memcpy(&buff[2], data, len);
    ret = i2c_write(buff, len+2);
  }
  else {
    buff[0] = (addr >> 8) & 0x1F;
    buff[1] = (addr & 0xFF);
    memcpy(&buff[2], data, m);
    ret = i2c_write(buff, m+2);
    if(ret == ERROR) return ret;

    addr = addr+m;
    len = len-m;
    idx = m;
    p = len/32;
    n = len%32;
    for(i = 0; i < p; i++) {
      buff[0] = (addr >> 8) & 0x1F;
      buff[1] = (addr & 0xFF);
      memcpy(&buff[2], &data[idx], 32);
      ret = i2c_write(buff, 34);
      if(ret == ERROR) return ret;
      addr += 32;
      idx += 32;
    }
    if(n > 0) {
      buff[0] = (addr >> 8) & 0x1F;
      buff[1] = (addr & 0xFF);
      memcpy(&buff[2], &data[idx], n);
      ret = i2c_write(buff, n+2);
    }
  }
  return ret;
}

/*
********************************************************************************
* Description : eepread_multi
* Arguments   : address, data, len
* Return      : state
* Note        : 
******************************************************************************** 
*/  
int eepread_multi(INT16U address, char *data, int len)
{
	int ret = 0;
	uint8_t buff[2];
  u16 i, p, n;
  u16 addr, idx = 0;

  addr = address;
  p = len/64;
  n = len%64;

  for(i = 0; i < p; i++) {
    buff[0] = (addr >> 8) & 0x1F;
    buff[1] = (addr & 0xFF);
    ret = i2c_read(buff, 2, (uint8_t *)&data[idx], 64);
    if(ret == ERROR) return ret;
    addr += 64;
    idx += 64;
  }
  if(n > 0) {
    buff[0] = (addr >> 8) & 0x1F;
    buff[1] = (addr & 0xFF);
    ret = i2c_read(buff, 2, (uint8_t *)&data[idx], n);
  }
	
//	int ret = 0;
//	uint8_t buff[2];
//
//	buff[0] = (address >> 8) & 0x1F;
//	buff[1] = (address & 0xFF);
//
//	ret = i2c_read(buff, 2, (uint8_t *)data, len);

	return ret;
}

/*
********************************************************************************
* Description : WriteArray_i2c
* Arguments   : dest, src, sz
* Return      : state
* Note        : 
******************************************************************************** 
*/ 
int WriteArray_i2c(int dest, char *src,int sz)
{
	int ret;
	
  ret = eepwrite_multi(dest, (u8*)src, sz);
  return ret;

//	int i,ret;
//  
//	for ( i=0;i<sz;i++){
//		ret = eepwrite(dest+i,src[i]);
//		if(ret == ERROR) return ERROR;
//	}
//	return OK;
}

/*
********************************************************************************
* Description : WriteArray_i2c_multi
* Arguments   : dest, src, sz
* Return      : state
* Note        : 
******************************************************************************** 
*/ 
int WriteArray_i2c_multi(int dest, char *src,int sz)
{  
	int i,ret, rcnt = 3;
  
  for(i = 0; i < rcnt; i++) {
    ret = WriteArray_i2c(dest, src, sz);
    if(ret == OK) break;
  }
  if(ret == ERROR) {
    return ret;
  }

  return ret;
}

/*
********************************************************************************
* Description : ReadArray_i2c
* Arguments   : target, src, sz
* Return      : state
* Note        : 
******************************************************************************** 
*/ 
int ReadArray_i2c(char *target,int src,int sz)
{
  int ret;
  
  ret = eepread_multi(src, target, sz);
  return ret;
  
//	int i,ret;
//	
//	for ( i=0;i<sz;i++){
//		ret = eepread(src+i,&target[i]);
//		if(ret == ERROR) return ERROR;
//	}
//	return OK;
}

/*
********************************************************************************
* Description : ReadArray_i2c_multi
* Arguments   : target, src, sz
* Return      : state
* Note        : 
******************************************************************************** 
*/
int ReadArray_i2c_multi(char *target,int src,int sz)
{
  int i, ret, rcnt = 3;
  
  for(i = 0; i < rcnt; i++) {
    ret = ReadArray_i2c(target, src, sz);
    if(ret == OK)
      return ret;
  }
  return ret;
}

/*
********************************************************************************
* Description : FRAM_Checksum
* Arguments   : pszData, nLength
* Return      : state
* Note        : 
******************************************************************************** 
*/
INT8S FRAM_Checksum(char *pszData, int nLength)
{
	int nCount;
	INT8S ucChkSum=0;

	for(nCount=0; nCount<nLength; nCount++){
		ucChkSum+=pszData[nCount];
	}

	return ucChkSum;
}

/*
********************************************************************************
* Description : FRAM_Write_ChkSum
* Arguments   : mp, addr, len
* Return      : state
* Note        : 
******************************************************************************** 
*/
void FRAM_Write_ChkSum(char* mp, u16 addr, u32 len)
{
  char *tp;
  int i;
  u16 cs = 0;
  
  tp = mp;
  for(i = 0; i < len-2; i++) {
    cs += *tp++;
  }
  *tp = cs & 0xff;
  tp++;
  *tp = (cs>>8) & 0xff;
  
  WriteArray_i2c(addr, mp, len); 
}

/*
********************************************************************************
* Description : FRAM_Read_ChkSum
* Arguments   : mp, addr, len
* Return      : state
* Note        : 
******************************************************************************** 
*/
int FRAM_Read_ChkSum(char* mp, u16 addr, u32 len)
{
  char *tp;
  int i;
  u16 cs = 0, rs = 0;
  
  ReadArray_i2c(mp, addr, len); 
  
  tp = mp;
  for(i = 0; i < len-2; i++) {
    cs += *tp++;
  }
  rs = *tp;
  tp++;
  rs |= *tp << 8;

  if(cs == rs) return 0;
  printk("fram checksum error!!\n");
  
  return 1; 
}

/*
********************************************************************************
* Description : BKRAM_Write_ChkSum
* Arguments   : mp, addr, len
* Return      : state
* Note        : 
******************************************************************************** 
*/
void BKRAM_Write_ChkSum(char* mp, u16 addr, u32 len)
{
  char *tp;
  int i;
  u16 cs = 0;
  
  tp = mp;
  for(i = 0; i < len-2; i++) {
    cs += *tp++;
  }
  *tp = cs & 0xff;
  tp++;
  *tp = (cs>>8) & 0xff;
  
//  write_to_backup_sram((INT8U *)&mp, len, addr);
}

/*
********************************************************************************
* Description : FRAM_Wait
* Arguments   : add_offset, save
* Return      : state
* Note        : 
******************************************************************************** 
*/
INT8U FRAM_WaitFlag;
uint8_t FRAM_Wait(void) 
{
  uint32_t wcnt = 0;
  
  while(FRAM_WaitFlag) {
    if (wcnt > 500)
     return ERROR;
    vTaskDelay(1);
    wcnt++;
  }
  return OK;
}


void FRAM_page_readwrite(INT8U add_offset, INT8U save)	// 각 구조체 변수들은 다른곳에서 항상 update가 되므로 읽지않고 바로 쓴다.
{
	INT8S checksum;

	switch(add_offset)
	{
		case FRAM_SCR_SYSTEM_SAVE:
			if(save == SAVE)
			{
				WriteArray_i2c_multi(FRAM_PARMSAVE_BASE,(char *)&SysSet,sizeof(stSysControl));
				checksum = FRAM_Checksum((char *)&SysSet,sizeof(stSysControl));
				WriteArray_i2c_multi((int)(FRAM_PARMSAVE_BASE+0xFF),(char *)&checksum,1);
			}
			else
			{
				ReadArray_i2c_multi((char *)&SysSet,FRAM_PARMSAVE_BASE,sizeof(stSysControl));
			}
			break;
    case FRAM_SCR_MAP_SAVE:
			if(save == SAVE)
			{
				WriteArray_i2c_multi(FRAM_MAPSAVE_BASE,(char *)&ScrMap,sizeof(stScrMap));
				checksum = FRAM_Checksum((char *)&ScrMap,sizeof(stScrMap));
				WriteArray_i2c_multi((int)(FRAM_MAPSAVE_BASE+0x2FF),(char *)&checksum,1);
			}
			else
			{
				ReadArray_i2c_multi((char *)&ScrMap,FRAM_MAPSAVE_BASE,sizeof(stScrMap));
			}          
      break;
    case FRAM_SCR_FACTOR_SAVE:
			if(save == SAVE)
			{
				WriteArray_i2c_multi(FRAM_FACTOR_BASE,(char *)&ScrFactor,sizeof(stScrFactor));
				checksum = FRAM_Checksum((char *)&ScrFactor,sizeof(stScrFactor));
				WriteArray_i2c_multi((int)(FRAM_FACTOR_BASE+0x7F),(char *)&checksum,1);
			}
			else
			{
				ReadArray_i2c_multi((char *)&ScrFactor,FRAM_FACTOR_BASE,sizeof(stScrFactor));
			}          
      break;
    case FRAM_SCR_CALIB_SAVE:
			if(save == SAVE)
			{
				WriteArray_i2c_multi(FRAM_CALIB_BASE,(char *)&ScrCalib,sizeof(stScrCalib));
				checksum = FRAM_Checksum((char *)&ScrCalib,sizeof(stScrCalib));
				WriteArray_i2c_multi((int)(FRAM_CALIB_BASE+0x7F),(char *)&checksum,1);
			}
			else
			{
				ReadArray_i2c_multi((char *)&ScrCalib,FRAM_CALIB_BASE,sizeof(stScrCalib));
			}          
      break;
    case FRAM_SCR_MAF_SAVE:
			if(save == SAVE)
			{
				WriteArray_i2c_multi(FRAM_MAF_BASE,(char *)&ScrMaf,sizeof(stScrMaf));
				checksum = FRAM_Checksum((char *)&ScrMaf,sizeof(stScrMaf));
			}
			else
			{
				ReadArray_i2c_multi((char *)&ScrMaf,FRAM_MAF_BASE,sizeof(stScrMaf));
			}          
      break;
		default:
			break;
	}
}


/*
********************************************************************************
* Description : dbg_fram_read
* Arguments   : none
* Return      : state
* Note        : 
******************************************************************************** 
*/
INT16U dbg_fram_read(void)
{
	char temp, buf[0x10];
	int addr, len, i, j, ret = 0;
  
	memset(buf, 0x0, sizeof(buf));
	
	if(CLI_GetArgCnt() < 2) return INV_PRM;
	
	addr = comparison_num((char *)P_ARG(1));
	len = comparison_num((char *)P_ARG(2));
  
  printk("\n");

	for(i=0; i<len; i++)
	{
		ret = ReadArray_i2c((char *)&temp, addr+i, 1);
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
			printk("\n%05X : ",addr+i);
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
	printk("[...%s...]\n",ret == OK ? "OK":"ERROR");
	printk("\n");	  
	
	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_fram_write
* Arguments   : none
* Return      : state
* Note        : 
******************************************************************************** 
*/
INT16U dbg_fram_write(void)				
{
	char ret=0,wdata = 0;
	int addr = 0;
	char buf = 0xff;
	
	if(CLI_GetArgCnt() < 2) return INV_PRM;
	
	addr = comparison_num((char *)P_ARG(1));
	wdata = comparison_num((char *)P_ARG(2));
	ret = WriteArray_i2c(addr, &wdata ,1);
	printk("\nFRAM write [%s] [0x%02X]\n",ret == OK ? "OK":"ERROR",wdata);
	ret = ReadArray_i2c(&buf,addr,1);
	printk("\nFRAM read [%s] [0x%02X]\n",ret == OK ? "OK":"ERROR",buf);

	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_fram_write_multi
* Arguments   : none
* Return      : state
* Note        : 
******************************************************************************** 
*/
INT16U dbg_fram_write_multi(void)				
{
	char ret=0,wdata[10] = {0};
	int addr = 0, i;
	char buff[10] = {0};
	
	if(CLI_GetArgCnt() < 2) return INV_PRM;
	
	addr = comparison_num((char *)P_ARG(1));
  memset(wdata,comparison_num((char *)P_ARG(2)),10);
	ret = WriteArray_i2c(addr, wdata ,10);
	printk("\nFRAM write [%s] ",ret == OK ? "OK":"ERROR");
  for(i = 0; i < 10; i++)
    printk("[0x%02X] ",wdata[i]);
  printk("\n");
	ret = ReadArray_i2c((char *)buff,addr,10);
	printk("\nFRAM read [%s] ",ret == OK ? "OK":"ERROR");
  for(i = 0; i < 10; i++)
    printk("[0x%02X] ",buff[i]);
  printk("\n");
	printk("\nFRAM compare [%s] \n",memcmp(wdata,buff,10) == 0 ? "FRAM_OK":"FRAM_ERROR");

	return NO_DISPLAY;
}






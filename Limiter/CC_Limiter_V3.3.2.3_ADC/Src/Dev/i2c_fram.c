/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : i2c_Gsensor.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : Gsensor interface configuration(MMA8451QT)
*********************************************************************************************************
*/

/*
*****************************************************************************************
*                                             INCLUDE FILES
*****************************************************************************************
*/

#define FRAM_GLOBALS

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private define ------------------------------------------------------------*/
#define FAIL				        0
#define OK					        1

#define PORT_OUT            0
#define PORT_IN             1

#define SW_I2C2_WAIT_TIME   5   //100KHz

#define FRAM_WAIT_TIME    100
#define SDA2_BIT          EEPROM_SDA_Pin
#define SCL2_BIT          EEPROM_SCL_Pin

#define SDA2_SETBIT()     HAL_GPIO_WritePin(EEPROM_SDA_GPIO_Port, EEPROM_SDA_Pin, GPIO_PIN_SET)
#define SDA2_RESETBIT()   HAL_GPIO_WritePin(EEPROM_SDA_GPIO_Port, EEPROM_SDA_Pin, GPIO_PIN_RESET)
#define SCL2_SETBIT()     HAL_GPIO_WritePin(EEPROM_SCL_GPIO_Port, EEPROM_SCL_Pin, GPIO_PIN_SET)
#define SCL2_RESETBIT()   HAL_GPIO_WritePin(EEPROM_SCL_GPIO_Port, EEPROM_SCL_Pin, GPIO_PIN_RESET)
#define SDA2_Read()       HAL_GPIO_ReadPin(EEPROM_SDA_GPIO_Port, EEPROM_SDA_Pin)

#define DEVICE_FRAM_ID    0xA0

osSemaphoreDef(I2C_FramSem);
osSemaphoreId I2C_FramSem = NULL;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t I2C2_ucNack_C, I2C2_ucNack_C2;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
********************************************************************************
* Description : I2C2_Init
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void I2C2_FRAM_Init(void)
{
  SDA2_SETBIT();
  SCL2_SETBIT();
  I2C_FramSem = osSemaphoreCreate(osSemaphore(I2C_FramSem) , 1);
}

/**
  * @brief  SDA2_PortSet
  * @param  None
  * @retval None
  */  	
void SDA2_PortSet(uint8_t type) //0: out 1: in
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  switch(type) {
    case 0:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;
    case 1:           
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;
  }  
  
  GPIO_InitStruct.Pin = EEPROM_SDA_Pin;     
  HAL_GPIO_Init(EEPROM_SDA_GPIO_Port, &GPIO_InitStruct);
}


/**
  * @brief  I2C2_Start
  * @param  None
  * @retval None
  */  	
void I2C2_Start(void)
{
  SDA2_PortSet(PORT_OUT);

  SCL2_SETBIT();
  SDA2_SETBIT();   delay_us(SW_I2C2_WAIT_TIME);
  SDA2_RESETBIT(); delay_us(SW_I2C2_WAIT_TIME);
  SCL2_RESETBIT(); delay_us(SW_I2C2_WAIT_TIME);  
}

/**
  * @brief  I2C2_Stop
  * @param  None
  * @retval None
  */  	
void I2C2_Stop(void)
{
  SDA2_PortSet(PORT_OUT);
  
  SDA2_RESETBIT();  
  SCL2_RESETBIT(); delay_us(SW_I2C2_WAIT_TIME);
  
  SCL2_SETBIT();   delay_us(SW_I2C2_WAIT_TIME);
  SDA2_SETBIT();   delay_us(SW_I2C2_WAIT_TIME);
  
  SDA2_PortSet(PORT_IN);
}

/**
  * @brief  I2C2_Clock
  * @param  None
  * @retval None
  */  	
void I2C2_Clock(void)
{
  SCL2_SETBIT();   delay_us(SW_I2C2_WAIT_TIME);
  SCL2_RESETBIT(); delay_us(SW_I2C2_WAIT_TIME);
}

/**
  * @brief  I2C2_Ackn
  * @param  None
  * @retval ackn flag data
  */  	
uint8_t I2C2_Ackn(void)
{  
  uint8_t ackn = 0;  // Temp RAM for Ackn flag  
  
  SDA2_PortSet(PORT_IN);
  
  SCL2_RESETBIT();
  SCL2_SETBIT();   delay_us(SW_I2C2_WAIT_TIME);

  if(SDA2_Read()) ackn = 1;
  else            ackn = 0;
  
  SCL2_RESETBIT();
  
  SDA2_PortSet(PORT_OUT); delay_us(SW_I2C2_WAIT_TIME);
  
  return ackn;  
}

/**
  * @brief  Write_I2C2_Byte
  * @param  1byte
  * @retval None
  */  	
void Write_I2C2_Byte(uint8_t byte)
{
  uint8_t i;

  SDA2_PortSet(PORT_OUT);

  SCL2_RESETBIT();  delay_us(SW_I2C2_WAIT_TIME);
  
  for(i=0; i<8; i++) {
    if((byte & 0x80) == 0x80)   SDA2_SETBIT();
    else                        SDA2_RESETBIT();
    delay_us(SW_I2C2_WAIT_TIME);
    
    SCL2_SETBIT();
    byte = byte << 1;
    delay_us(SW_I2C2_WAIT_TIME);
    
    SCL2_RESETBIT();
  }

  SDA2_PortSet(PORT_IN);
  
  delay_us(SW_I2C2_WAIT_TIME);
  
  if(I2C2_Ackn() == 1)  	I2C2_ucNack_C = 1; 

}

/**
  * @brief  Read_I2C2_Byte
  * @param  ack
  * @retval rdata
  */  	
uint8_t Read_I2C2_Byte(uint8_t ack)
{
  uint8_t i, buff = 0;
  
  SDA2_PortSet(PORT_IN);
  
  for(i=0; i<8; i++) {
    buff <<= 1;
    
    delay_us(SW_I2C2_WAIT_TIME);
    SCL2_SETBIT();
    delay_us(SW_I2C2_WAIT_TIME);
     
    if(SDA2_Read())  buff |= 0x01;
     
    SCL2_RESETBIT();
  }  
  delay_us(SW_I2C2_WAIT_TIME);
  SDA2_PortSet(PORT_OUT);    
  
  if(ack == 0) {
    SDA2_RESETBIT();
    delay_us(SW_I2C2_WAIT_TIME);
    I2C2_Clock();
  }
  else {
    SDA2_SETBIT();
    delay_us(SW_I2C2_WAIT_TIME);
    I2C2_Clock();
    
    SDA2_RESETBIT();
    delay_us(SW_I2C2_WAIT_TIME);
  }

  return buff;    
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
  uint8_t i;
  uint8_t dev_addr = DEVICE_FRAM_ID;
  
  dev_addr = dev_addr + 0; //+1 read / +0 write
  I2C2_ucNack_C = 0;  
  
	if(I2C_FramSem == NULL)
	{
		printk("%s err\n",__func__);
		return 0;
	}
	if(osSemaphoreWait(I2C_FramSem, 50) == osOK)
	{  
    I2C2_Start();
    Write_I2C2_Byte(dev_addr); 
    if(I2C2_ucNack_C) {
      osSemaphoreRelease(I2C_FramSem);
      return 0;  
    }
    for(i=0; i < txlen; i++) {
      Write_I2C2_Byte(*tx_buf++); 
      if(I2C2_ucNack_C) {
        osSemaphoreRelease(I2C_FramSem);
        return 0;  
      }
    }

    dev_addr = dev_addr + 1; //+1 read / +0 write  
    I2C2_Start();
    Write_I2C2_Byte(dev_addr); 
    if(I2C2_ucNack_C) {
      osSemaphoreRelease(I2C_FramSem);
      return 0;  
    }
    while(rxlen > 0)
    {
      if(rxlen == 1)
        *rx_buffer++ = Read_I2C2_Byte(1);       // Move RX data to address PRxData
      else
        *rx_buffer++ = Read_I2C2_Byte(0);       // Move RX data to address PRxData
      rxlen--;                                  // Decrement RX byte counter
    } 
    I2C2_Stop();
    osSemaphoreRelease(I2C_FramSem);
  }
  return 1;
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
  uint8_t dev_addr = DEVICE_FRAM_ID;
  
  dev_addr = dev_addr + 0; //+1 read / +0 write
  I2C2_ucNack_C = 0;  
	if(I2C_FramSem == NULL)
	{
		printk("%s err\n",__func__);
		return 0;
	}
	if(osSemaphoreWait(I2C_FramSem, 50) == osOK)
	{   
    I2C2_Start();
    Write_I2C2_Byte(dev_addr);
    if(I2C2_ucNack_C) {
      osSemaphoreRelease(I2C_FramSem);
      return 0;  
    }

    while(len > 0 && !I2C2_ucNack_C)
    {
      Write_I2C2_Byte(*buffer++);
      len--;                            // Decrement RX byte counter
    } 
    osSemaphoreRelease(I2C_FramSem);    
    I2C2_Stop();
  }
  return 1;  
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
int eepwrite_multi(uint16_t addr, uint8_t *data, uint16_t len)
{
  uint16_t i, m, p, n;
  int ret = 0;
  uint8_t buff[32+2];
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
  uint16_t i, p, n;
  uint16_t addr, idx = 0;

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
	
  ret = eepwrite_multi(dest, (uint8_t*)src, sz);
  return ret;
}

/*
********************************************************************************
* Description : WriteArray_i2c_multi
* Arguments   : dest, src, sz
* Return      : state
* Note        : 
******************************************************************************** 
*/ 
char I2CTmpBuff[128];
int WriteArray_i2c_multi(int dest, char *src,int sz)
{  
	int i,ret, rcnt = 3;
  
  for(i = 0; i < rcnt; i++) {
    ret = WriteArray_i2c(dest, src,sz);
    if(ret == OK) break;
  }
  if(ret == ERROR) {
    return ret;
  }
  if(sz > 128) sz = 128;
  for(i = 0; i < rcnt; i++) {
    ret = ReadArray_i2c(I2CTmpBuff, dest, sz);
    if(ret == OK) {
      if(memcmp(I2CTmpBuff, src, sz) == 0)
        return ret;
    }
  }

  return ret;
}

	
/*
********************************************************************************
* Description : I2C2_Read
* Arguments   : address *data length
* Return      : rdata
* Note        : 
******************************************************************************** 
*/ 
u8 I2C2_Read(u16 addr, u8 *data, u16 len)
{
  u8 dev_addr = DEVICE_FRAM_ID, tmpl,tmph;

  I2C2_ucNack_C2 = 0;
  tmph = (addr>>8)&0xff;
  tmpl = addr&0xff;

  //portENTER_CRITICAL();
  I2C2_Start();

  Write_I2C2_Byte(dev_addr); 
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }

  Write_I2C2_Byte(tmph); //+1 read / +0 write
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }  
  Write_I2C2_Byte(tmpl); //+1 read / +0 write
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }  

  I2C2_Start();
  dev_addr = (dev_addr)+1;   //+1 read / +0 write

  Write_I2C2_Byte(dev_addr); 
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }

  while(len > 0) {
    if(len == 1)
      *data = Read_I2C2_Byte(1);               // Move RX data to address PRxData
    else
      *data++ = Read_I2C2_Byte(0);               // Move RX data to address PRxData
    len--;                        // Decrement RX byte counter
    delay_us(SW_I2C2_WAIT_TIME*2);
  } 
  
  I2C2_Stop();    
  //portEXIT_CRITICAL();
  return 1;
}

/*
********************************************************************************
* Description : I2C2_Write
* Arguments   : address *data length
* Return      : rdata
* Note        : 
******************************************************************************** 
*/ 	
u8 I2C2_Write(u16 addr, u8 *data, u16 len)
{
  u8 dev_addr = DEVICE_FRAM_ID, tmpl,tmph;

  I2C2_ucNack_C2 = 0;
  tmph = (addr>>8)&0xff;
  tmpl = addr&0xff;

  //portENTER_CRITICAL();
  I2C2_Start();

  Write_I2C2_Byte(dev_addr); //+1 read / +0 write
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }  
  Write_I2C2_Byte(tmph); //+1 read / +0 write
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }  
  Write_I2C2_Byte(tmpl); //+1 read / +0 write
  if(I2C2_ucNack_C2) {
    //portEXIT_CRITICAL(); 
    return 0;
  }  
  
  while(len > 0 && !I2C2_ucNack_C2)
  {
    Write_I2C2_Byte(*data++);  
    len--;                        // Decrement RX byte counter
    delay_us(SW_I2C2_WAIT_TIME*2);
  } 
  I2C2_Stop();    
  
  //portEXIT_CRITICAL();
  
  return 1;
}


/*
********************************************************************************
* Description : FRAMByteWrite
* Arguments   : data, addr
* Return      : None
* Note        : 
******************************************************************************** 
*/ 
void FRAMByteWrite(u8 data, u16 addr)
{
	if(I2C_FramSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(I2C_FramSem, 50) == osOK)
	{

    I2C2_Write(addr, &data, 1);
    delay_us(FRAM_WAIT_TIME);
    
    osSemaphoreRelease(I2C_FramSem);
  }
  
  return;
}

/*
********************************************************************************
* Description : FRAMMultiWrite
* Arguments   : data, addr, len
* Return      : None
* Note        : 
******************************************************************************** 
*/ 
void FRAMMultiWrite(u8 *data, u16 addr, u16 len)
{
  u16 i, m, p, n;
  u16 faddr, flen;
  u8* fdata;
  
	if(I2C_FramSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(I2C_FramSem, 50) == osOK)
	{
    m = 32-(addr%32);
    
    if(m >= len) {
      I2C2_Write(addr, data, len);
      delay_us(FRAM_WAIT_TIME);
    }
    else {
      I2C2_Write(addr, data, m);
      delay_us(FRAM_WAIT_TIME);

      faddr = addr+m;
      flen = len-m;
      fdata = data + m;

      p = flen/32;
      n = flen%32;
      for(i = 0; i < p; i++) {
        I2C2_Write(faddr+i*32, fdata+i*32, 32);
        delay_us(FRAM_WAIT_TIME);      
        IWDG_ReloadCounter(); 
      }
      I2C2_Write(faddr+p*32, fdata+p*32, n);
      delay_us(FRAM_WAIT_TIME);
    }  
    
    osSemaphoreRelease(I2C_FramSem);
  }
}

/*
********************************************************************************
* Description : FRAMByteRead
* Arguments   : data, addr
* Return      : None
* Note        : 
******************************************************************************** 
*/ 
void FRAMByteRead(u8* data, u16 addr)
{
	if(I2C_FramSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(I2C_FramSem, 50) == osOK)
	{
    I2C2_Read(addr, data, 1);
    delay_us(FRAM_WAIT_TIME);

    osSemaphoreRelease(I2C_FramSem);
  }
  return;
}

/*
********************************************************************************
* Description : FRAMMultiRead
* Arguments   : data, addr, len
* Return      : None
* Note        : 
******************************************************************************** 
*/ 
void FRAMMultiRead(u8 *data, u16 addr, u16 len)
{
	if(I2C_FramSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(osSemaphoreWait(I2C_FramSem, 50) == osOK)
	{
    I2C2_Read(addr, data, len);
    delay_us(FRAM_WAIT_TIME);

    osSemaphoreRelease(I2C_FramSem);
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
	ret = ReadArray_i2c((char *)&buf,addr,1);
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
  if(addr < FRAM_RES_BASE) addr = FRAM_RES_BASE;
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


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
*                                             INCLUDE FILES
*****************************************************************************************
*************************************************************************************************************************
*/

#define GSENSOR_GLOBALS
#include "includes.h"


#define SW_I2C1_WAIT_TIME   5
#define PORT_OUT            0
#define PORT_IN             1

#define SDA1_BIT            G_SDA_Pin
#define SCL1_BIT            G_SCL_Pin

#define SDA1_SETBIT()       HAL_GPIO_WritePin(GPIOC, SDA1_BIT,GPIO_PIN_SET)
#define SDA1_RESETBIT()     HAL_GPIO_WritePin(GPIOC, SDA1_BIT,GPIO_PIN_RESET)
#define SCL1_SETBIT()       HAL_GPIO_WritePin(GPIOA, SCL1_BIT,GPIO_PIN_SET)
#define SCL1_RESETBIT()     HAL_GPIO_WritePin(GPIOA, SCL1_BIT,GPIO_PIN_RESET)

#define SDA1_Read()         (GPIOC->IDR & SDA1_BIT)

#define DEVICE_ACC_ID       0x38
#define LIS2DH12_ACC_ID     0x30

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int16_t GS_Val[3],GS_Buff[3][10];
int16_t GS_Val_Conv[3], GS_Val_Max[3], GS_Val_Dir[3];
int16_t GS_Offset[3], GS_Val_Set[3]; 

uint8_t I2C1_ucNack_C, I2C1_ucNack_C2;
uint8_t I2C1_Set_Direction;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void I2C1_ACC_Init(void);

/**
  * @brief  SDA1_PortSet
  * @param  None
  * @retval None
  */  	
void SDA1_PortSet(uint8_t type) //0: out 1: in
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
  
  GPIO_InitStruct.Pin = G_SDA_Pin;     
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


/**
  * @brief  I2C1_Start
  * @param  None
  * @retval None
  */  	
void I2C1_Start(void)
{
  SDA1_PortSet(PORT_OUT);

  SCL1_SETBIT();
  SDA1_SETBIT();   delay_us(SW_I2C1_WAIT_TIME);
  SDA1_RESETBIT(); delay_us(SW_I2C1_WAIT_TIME);
  SCL1_RESETBIT(); delay_us(SW_I2C1_WAIT_TIME);  
}

/**
  * @brief  I2C1_Stop
  * @param  None
  * @retval None
  */  	
void I2C1_Stop(void)
{
  SDA1_PortSet(PORT_OUT);
  
  SDA1_RESETBIT();  
  SCL1_RESETBIT(); delay_us(SW_I2C1_WAIT_TIME);
  
  SCL1_SETBIT();   delay_us(SW_I2C1_WAIT_TIME);
  SDA1_SETBIT();   delay_us(SW_I2C1_WAIT_TIME);
  
  SDA1_PortSet(PORT_IN);
}

/**
  * @brief  I2C1_Clock
  * @param  None
  * @retval None
  */  	
void I2C1_Clock(void)
{
  SCL1_SETBIT();   delay_us(SW_I2C1_WAIT_TIME);
  SCL1_RESETBIT(); delay_us(SW_I2C1_WAIT_TIME);
}

/**
  * @brief  I2C1_Ackn
  * @param  None
  * @retval ackn flag data
  */  	
uint8_t I2C1_Ackn(void)
{  
  uint8_t ackn = 0;  // Temp RAM for Ackn flag  
  
  SDA1_PortSet(PORT_IN);
  
  SCL1_RESETBIT();
  SCL1_SETBIT();   delay_us(SW_I2C1_WAIT_TIME);

  if(SDA1_Read()) ackn = 1;
  else            ackn = 0;
  
  SCL1_RESETBIT();
  
  SDA1_PortSet(PORT_OUT); delay_us(SW_I2C1_WAIT_TIME);
  
  return ackn;  
}

/**
  * @brief  Write_I2C1_Byte
  * @param  1byte
  * @retval None
  */  	
void Write_I2C1_Byte(uint8_t byte)
{
  uint8_t i;

  SDA1_PortSet(PORT_OUT);

  SCL1_RESETBIT();  delay_us(SW_I2C1_WAIT_TIME);
  
  for(i=0; i<8; i++) {
    if((byte & 0x80) == 0x80)   SDA1_SETBIT();
    else                        SDA1_RESETBIT();
    delay_us(SW_I2C1_WAIT_TIME);
    
    SCL1_SETBIT();
    byte = byte << 1;
    delay_us(SW_I2C1_WAIT_TIME);
    
    SCL1_RESETBIT();
  }

  SDA1_PortSet(PORT_IN);
  
  delay_us(SW_I2C1_WAIT_TIME);
  
  if(I2C1_Ackn() == 1)  	I2C1_ucNack_C = 1; 

}

/**
  * @brief  Read_I2C1_Byte
  * @param  ack
  * @retval rdata
  */  	
uint8_t Read_I2C1_Byte(uint8_t ack)
{
  uint8_t i, buff = 0;
  
  SDA1_PortSet(PORT_IN);
  
  for(i=0; i<8; i++) {
    buff <<= 1;
    
    delay_us(SW_I2C1_WAIT_TIME);
    SCL1_SETBIT();
    delay_us(SW_I2C1_WAIT_TIME);
     
    if(SDA1_Read())  buff |= 0x01;
     
    SCL1_RESETBIT();
  }  
  delay_us(SW_I2C1_WAIT_TIME);
  SDA1_PortSet(PORT_OUT);    
  
  if(ack == 0) {
    SDA1_RESETBIT();
    delay_us(SW_I2C1_WAIT_TIME);
    I2C1_Clock();
  }
  else {
    SDA1_SETBIT();
    delay_us(SW_I2C1_WAIT_TIME);
    I2C1_Clock();
    
    SDA1_RESETBIT();
    delay_us(SW_I2C1_WAIT_TIME);
  }

  return buff;    
}

/**
  * @brief  I2C1_Read
  * @param  address *data length
  * @retval rdata
  */  	
uint8_t I2C1_Read(uint8_t dev_addr, uint8_t addr, uint8_t *data, uint8_t len)
{
  I2C1_ucNack_C = 0;

  I2C1_Start();
//  dev_addr = (dev_addr<<1)+0;   //+1 read / +0 write
  Write_I2C1_Byte(dev_addr); 
  if(I2C1_ucNack_C) return 0;
  Write_I2C1_Byte(addr); 
  if(I2C1_ucNack_C) return 0;

  I2C1_Start();
  dev_addr = (dev_addr)+1;   //+1 read / +0 write
  Write_I2C1_Byte(dev_addr); 
  if(I2C1_ucNack_C) return 0;

  while(len > 0)
  {
    if(len == 1)
      *data++ = Read_I2C1_Byte(1);               // Move RX data to address PRxData
    else
      *data++ = Read_I2C1_Byte(0);               // Move RX data to address PRxData
    len--;                        // Decrement RX byte counter
  } 
  I2C1_Stop();    
  return 1;
}

/**
  * @brief  I2C1_Write
  * @param  address *data length
  * @retval rdata
  */  	
uint8_t I2C1_Write(uint8_t dev_addr, uint8_t addr, uint8_t *data, uint8_t len)
{
  I2C1_ucNack_C = 0;
    
  I2C1_Start();
//  dev_addr = (dev_addr<<1)+0;   //+1 read / +0 write
  Write_I2C1_Byte(dev_addr); //+1 read / +0 write
  if(I2C1_ucNack_C) return 0;  
  Write_I2C1_Byte(addr); //+1 read / +0 write
  if(I2C1_ucNack_C) return 0;  
  
  while(len > 0 && !I2C1_ucNack_C)
  {
    Write_I2C1_Byte(*data++);
    len--;                            // Decrement RX byte counter
  } 
  I2C1_Stop();    
  return 1;
}

/**
  * @brief  I2C1_Init
  * @param  None
  * @retval None
  */  	
u8 GSenScale;
u8 GSenConByte;
u8 GSenID;
void I2C1_ACC_Init(void)
{
  uint8_t ctrl_reg_values[] = {0x97, 0x00, 0x00, LIS2DH12_RANGE_8GA, 0x00, 0x00};
  uint8_t resp = 0,i;
  
  SDA1_SETBIT();
  SCL1_SETBIT();

  for(i = 0; i < 3; i++) {
    resp = I2C1_Read(DEVICE_ACC_ID, 0x0D, &GSenID, 1);
    if(resp == 1) break;
  }
  if(GSenID == 0x1A) {
    GSenConByte = 0x40;
    I2C1_Write(DEVICE_ACC_ID, 0x2B, &GSenConByte, 1);
    delay_ms(10);
  #if GSENSOR_1G_VAL == 256
    GSenConByte = 0x00;        //Stanby Mode g-Sensor
    I2C1_Write(DEVICE_ACC_ID, 0x2A, &GSenConByte, 1);
    GSenConByte = 0x02; 
    I2C1_Write(DEVICE_ACC_ID, 0x0E, &GSenConByte, 1); 
    delay_ms(10);
  #endif  
    GSenConByte = 0x01;        //active g-Sensor
    I2C1_Write(DEVICE_ACC_ID, 0x2A, &GSenConByte, 1);
  }
  else {
    for(i = 0; i < 3; i++) {
      resp = I2C1_Read(LIS2DH12_ACC_ID, 0x0F, &GSenID, 1);
      if(resp == 1) break;
    }
    I2C1_Write(LIS2DH12_ACC_ID, 0xA0, ctrl_reg_values, 6);
  }
}


/**
  * @brief  I2C1_Read_Data
  * @param  address *data length
  * @retval rdata
  */  	
void I2C1_ACC_Read(void)
{
  int16_t gval[3], dirconval[3];
  uint8_t i, data[6];
  double tmp;
  uint16_t gabs, mabs;
  
  if(GSenID == 0x1A) {
    I2C1_Read(DEVICE_ACC_ID, 0x01, data, 6);
    gval[0] = ( (int16_t)((uint16_t)(data[2]<<8) + data[3]) >> 2 )/4; 
    gval[1] = ( (int16_t)((uint16_t)(data[0]<<8) + data[1]) >> 2 )/4; 
    gval[2] = ( (int16_t)((uint16_t)(data[4]<<8) + data[5]) >> 2 )/4; 
  }
  else {
    I2C1_Read(LIS2DH12_ACC_ID, 0xA8, data, 6);
    gval[0] = ( (int16_t)((uint16_t)(data[3]<<8) + data[2]) >> 2 )/4; 
    gval[1] = ( (int16_t)((uint16_t)(data[1]<<8) + data[0]) >> 2 )/4; 
    gval[2] = ( (int16_t)((uint16_t)(data[5]<<8) + data[4]) >> 2 )/4; 
  }
    
  for(i = 0; i < 3; i++) {
#ifdef GSENSOR_AVERAGING   
  int32_t gsum[3];
  uint8_t j;
    gsum[i] = 0;
    for(j = 0; j < 9; j++) {
      GS_Buff[i][j] = GS_Buff[i][j+1];
      gsum[i] += GS_Buff[i][j];
    }
    GS_Buff[i][j] = gval[i];
    gsum[i] += GS_Buff[i][j];
    
    GS_Val_Dir[i] = (int16_t)(gsum[i]/10) - GS_Offset[i]; //2.1.3
#else
    GS_Val_Dir[i] = gval[i] - GS_Offset[i]; //2.1.3
#endif    
    tmp = (double)GS_Val_Dir[i]*9.81/(double)GSENSOR_1G_VAL*10.0;
    dirconval[i] = (int16_t)tmp; //2.1.3
    vTaskDelay(1);    
  }
  
  I2C1_Set_Direction = 0;
  switch(I2C1_Set_Direction) { //1.4.0.0
    case 0:
      GS_Val[0] = GS_Val_Dir[0];
      GS_Val[1] = GS_Val_Dir[1];
      GS_Val[2] = GS_Val_Dir[2];
      GS_Val_Conv[0] = dirconval[0];
      GS_Val_Conv[1] = dirconval[1];
      GS_Val_Conv[2] = dirconval[2];
      break;
    case 1:
      GS_Val[0] = GS_Val_Dir[0];
      GS_Val[1] = GS_Val_Dir[2];
      GS_Val[2] = GS_Val_Dir[1];
      GS_Val_Conv[0] = dirconval[0];
      GS_Val_Conv[1] = dirconval[2];
      GS_Val_Conv[2] = dirconval[1];
      break;
    case 2:
      GS_Val[0] = GS_Val_Dir[1];
      GS_Val[1] = GS_Val_Dir[0];
      GS_Val[2] = GS_Val_Dir[2];
      GS_Val_Conv[0] = dirconval[1];
      GS_Val_Conv[1] = dirconval[0];
      GS_Val_Conv[2] = dirconval[2];
      break;
    case 3:
      GS_Val[0] = GS_Val_Dir[1];
      GS_Val[1] = GS_Val_Dir[2];
      GS_Val[2] = GS_Val_Dir[0];
      GS_Val_Conv[0] = dirconval[1];
      GS_Val_Conv[1] = dirconval[2];
      GS_Val_Conv[2] = dirconval[0];
      break;
    case 4:
      GS_Val[0] = GS_Val_Dir[2];
      GS_Val[1] = GS_Val_Dir[0];
      GS_Val[2] = GS_Val_Dir[1];
      GS_Val_Conv[0] = dirconval[2];
      GS_Val_Conv[1] = dirconval[0];
      GS_Val_Conv[2] = dirconval[1];
      break;
    case 5:
      GS_Val[0] = GS_Val_Dir[2];
      GS_Val[1] = GS_Val_Dir[1];
      GS_Val[2] = GS_Val_Dir[0];
      GS_Val_Conv[0] = dirconval[2];
      GS_Val_Conv[1] = dirconval[1];
      GS_Val_Conv[2] = dirconval[0];
      break;
  }
  
  for(i = 0; i < 3; i++) {
    gabs = abs(GS_Val_Conv[i]);
    mabs = abs(GS_Val_Max[i]);
    if(gabs > mabs)
      GS_Val_Max[i] = GS_Val_Conv[i];  
  }
  osDelay(1);  
}



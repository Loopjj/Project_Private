/*
*************************************** (C) COPYRIGHT 2017, 2018 LOOP *************************************
*
* File Name          : spi_lcd.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : spi lcd process
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define LCD_GLOBALS

#include "includes.h"
#include "eng_font.h"
#include "han_font.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SW_GPIO_WAIT_TIME 3

#define LCD_RST_SET()			HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)
#define LCD_RST_RESET()		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define LCD_A0_SET()		  HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET)
#define LCD_A0_RESET()		HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET)
#define LCD_SDA_SET()		  HAL_GPIO_WritePin(LCD_SDA_GPIO_Port, LCD_SDA_Pin, GPIO_PIN_SET)
#define LCD_SDA_RESET()		HAL_GPIO_WritePin(LCD_SDA_GPIO_Port, LCD_SDA_Pin, GPIO_PIN_RESET)
#define LCD_SCL_SET()		  HAL_GPIO_WritePin(LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_PIN_SET)
#define LCD_SCL_RESET()		HAL_GPIO_WritePin(LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_PIN_RESET)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
osSemaphoreDef(sem_lcd);
osSemaphoreId sem_lcd = NULL;

/* Private functions ---------------------------------------------------------*/

void lcd_delay_us(unsigned int nCount)
{
  nCount *= 15; //cortex m4
  for(; nCount!=0; nCount--) {
  }
}
	
void lcd_delay_ms(unsigned int nCount)
{
  for(; nCount!=0; nCount--) lcd_delay_us(1000);
}

void lcd_init_semaphore(void)
{
}

//// Init lcd
void lcd_init(void)
{
  lcd_delay_ms(100);   //// LCD 구동전 CPU가 먼저 돌때 LCD쪽 컨덴서 작동 시간 확보해줌..
  if(sem_lcd == NULL)
    sem_lcd = osSemaphoreCreate(osSemaphore(sem_lcd) , 1);
  if(Contrast < CONTRAST_MIN || Contrast > CONTRAST_MAX) 
    Contrast = CONTRAST_DEFAULT;
  
  lcd_reset();
  LCD_A0_RESET();
  lcd_delay_ms(1);
  lcd_send_cmd(0xe2);  //system reset     
  lcd_delay_ms(1);
  lcd_send_cmd(0x40);  //DISPLAY START LINE  SET
  lcd_send_cmd(0xA0);  //ADC SELECT -> NORMAL 
  lcd_send_cmd(0xA6);  //DISPLAY NORMAL (REVERSE 0XA7) 
  lcd_send_cmd(0xA4);  //DISPLAY ALL POINTS OFF (ON 0XA5)
  lcd_send_cmd(0xA2); //set 1/6 bias,1/33duty
  lcd_send_cmd(0xc8); //Common Output Mode Select, com0~com31
  lcd_send_cmd(0x22); //set (1+Rb/Ra) 010:4.0; 100:5.0 ; 111: 6.4
  lcd_send_cmd(0x81); //Electronic Volume Mode Set
//  lcd_send_cmd(25); //Electronic Volume Register Set 0X00 ~ 0X3F /
  lcd_send_cmd(Contrast); //Electronic Volume Register Set 0X00 ~ 0X3F /
  lcd_send_cmd(0x29); //power control set
  lcd_delay_ms(1); //1ms
  lcd_send_cmd(0x2B); //power control set
  lcd_delay_ms(10); //1ms
  lcd_send_cmd(0x2F); //power control set
  lcd_delay_ms(1); //1ms
  lcd_send_cmd(0xaf); //Dispaly On
  lcd_delay_ms(2);

  lcd_clear();
}

//// hardware reset reset
void lcd_reset(void)
{
//  LCD_RST_SET();
//  LCD_SDA_SET();
//  LCD_SCL_SET();
//  lcd_delay_ms(2);
  
  LCD_RST_RESET();
  lcd_delay_ms(10);
  LCD_RST_SET();
  lcd_delay_ms(10);
}

//// refresh lcd
void lcd_refresh(void)
{
//  if(!LCD_Refresh_Flag) return;  
  lcd_reset();
  LCD_A0_RESET();
  lcd_delay_ms(1);
  lcd_send_cmd(0xe2);  //system reset     
  lcd_delay_ms(1);
  lcd_send_cmd(0x40);  //DISPLAY START LINE  SET
  lcd_send_cmd(0xA0);  //ADC SELECT -> NORMAL 
  lcd_send_cmd(0xA6);  //DISPLAY NORMAL (REVERSE 0XA7) 
  lcd_send_cmd(0xA4);  //DISPLAY ALL POINTS OFF (ON 0XA5)
  lcd_send_cmd(0xA2); //set 1/6 bias,1/33duty
  lcd_send_cmd(0xc8); //Common Output Mode Select, com0~com31
  lcd_send_cmd(0x22); //set (1+Rb/Ra) 010:4.0; 100:5.0 ; 111: 6.4
  lcd_send_cmd(0x81); //Electronic Volume Mode Set
  lcd_send_cmd(Contrast); //Electronic Volume Register Set 0X00 ~ 0X3F /
//  lcd_send_cmd(25); //Electronic Volume Register Set 0X00 ~ 0X3F /
  lcd_send_cmd(0x29); //power control set
  lcd_delay_ms(1); //1ms
  lcd_send_cmd(0x2B); //power control set
  lcd_delay_ms(10); //1ms
  lcd_send_cmd(0x2F); //power control set
  lcd_delay_ms(1); //1ms
  lcd_send_cmd(0xaf); //Dispaly On
  lcd_delay_ms(2);
  
//  LCD_Refresh_Flag = 0;
}
  

//// adjust lcd contrast
void lcd_contrast(unsigned char val)  
{
  lcd_send_cmd(0x40);  //DISPLAY START LINE  SET
  lcd_send_cmd(0xA0);  //ADC SELECT -> NORMAL 
  lcd_send_cmd(0xA6);  //DISPLAY NORMAL (REVERSE 0XA7) 
  lcd_send_cmd(0xA4);  //DISPLAY ALL POINTS OFF (ON 0XA5)
  lcd_send_cmd(0xA2); //set 1/6 bias,1/33duty
  lcd_send_cmd(0xc8); //Common Output Mode Select, com0~com31
  lcd_send_cmd(0x22); //set (1+Rb/Ra) 010:4.0; 100:5.0 ; 111: 6.4
  lcd_send_cmd(0x81);  //ELECTRIC  VOLUME (CONTRAST CMD)
  lcd_send_cmd(val);  //CONTRAST VAL 0X00 ~ 0X3F //new
  lcd_send_cmd(0xaf); //Dispaly On
  Contrast_Flag = 1;  
}

//// clear lcd buffer
void lcd_clear(void)
{
  unsigned char page, col;

  for(page=0; page<4; page++)
  {
    lcd_send_cmd(PAGE_ADDRESS+page);
    lcd_send_cmd(COLUMN_ADDRESS_HI);
    lcd_send_cmd(COLUMN_ADDRESS_LO);

    for(col=0;col<128;col++)
    {
      lcd_send_data(0x00);
    } 
  }
}

//// adjust lcd backlight
#define  PERIOD_VALUE       (666 - 1)  /* Period Value  */
void lcd_backlight(unsigned char val)
{
  setPWM(htim4, TIM_CHANNEL_2, PERIOD_VALUE, val);
}

//// write a byte to lcd buffer  //gpio spi
#define LCD_SIGNAL_DELAY  1
void lcd_write(unsigned char data)
{
  int i;
  
  for(i = 7; i >=0; i--) {
    if(((data>>i)&0x01)) 
      LCD_SDA_SET();
    else
      LCD_SDA_RESET();
    lcd_delay_us(LCD_SIGNAL_DELAY);
    LCD_SCL_RESET();
    lcd_delay_us(LCD_SIGNAL_DELAY);
    LCD_SCL_SET();
    lcd_delay_us(LCD_SIGNAL_DELAY);
  }
  LCD_SDA_SET();
  lcd_delay_us(LCD_SIGNAL_DELAY);  
}

//// write a command byte  
void lcd_send_cmd(unsigned char cmd)
{
	if(sem_lcd == NULL)
	{
		return;
	}  
  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{ 
    LCD_A0_RESET();
    lcd_delay_us(LCD_SIGNAL_DELAY);   
    lcd_write(cmd);  
    
    osSemaphoreRelease(sem_lcd);
  }    
}

//// write a data byte  	
void lcd_send_data(unsigned char data)
{
	if(sem_lcd == NULL)
	{
		return;
	}  
  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{ 
    LCD_A0_SET();
    lcd_delay_us(LCD_SIGNAL_DELAY);   
    lcd_write(data);  
    
    osSemaphoreRelease(sem_lcd);
  }    
}

//// set position of data byte (for 128x32 x -> point : 0 ~ 128  y -> char line: 0 ~ 4)
void lcd_set_xy(unsigned char x, unsigned char y)
{
  lcd_send_cmd(PAGE_ADDRESS + y);
  lcd_send_cmd(COLUMN_ADDRESS_HI + (x >> 4));
  lcd_send_cmd(COLUMN_ADDRESS_LO + (x & 0x0F));
}

//// draw bitmap on lcd
void lcd_bitmap(unsigned char x, unsigned char y, unsigned char sx, unsigned char sy, unsigned char mode, unsigned char *bitmap)
{
  int i,j;
  
//	if(sem_lcd == NULL)
//	{
//		return;
//	}  
//  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{ 
    for(i=0; i<sy/8; i++) {
      lcd_set_xy(x, (i+y));         
      for(j=0; j<sx; j++)
        if(!mode)  lcd_send_data(bitmap[(i*sx)+j]);  
        else lcd_send_data(~bitmap[(i*sx)+j]); 
    }
//    osSemaphoreRelease(sem_lcd);
//  }    
}

//// draw rectangle on lcd
void lcd_draw_rect(unsigned char x, unsigned char x2, unsigned char y)
{
  unsigned char i;
  
//	if(sem_lcd == NULL)
//	{
//		return;
//	}  
//  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{   
    lcd_set_xy(x, y);
    lcd_send_data(0x7e);
    
    for (i =1; i < x2 - x - 1; i++)
      lcd_send_data(0x42);
            
    lcd_send_data(0x7e);
//    osSemaphoreRelease(sem_lcd);
//  }       
}

//// fill a box on lcd
void lcd_fill_box(unsigned char x, unsigned char x2, unsigned char y, unsigned char thickness)
{
  unsigned char data = 0;
  unsigned char i;
	
  switch(thickness) {
    case 1:
            data = 0x08;
            break;
    case 2:
            data = 0x3c;
            break;
    case 3:
            data = 0x3e;
            break;
    case 4:
            data = 0x7e;
            break;
    default : break;
  }
  
//	if(sem_lcd == NULL)
//	{
//		return;
//	}  
//  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{   
    lcd_set_xy(x, y);
    for (i=0; i < x2 - x; i++)
      lcd_send_data(data);
//    osSemaphoreRelease(sem_lcd);
//  }    
}

//// put a character on lcd (5x7 font-->6x8 size)
void lcd_putc(unsigned char x, unsigned char y, unsigned char mode, char ch)
{
  int i;  
    
  lcd_set_xy(x, y);
  for (i=0; i<=4; i++) {    
    if(!mode) lcd_send_data(eng_font[((ch - 0x20) * 5) + i]); 
    else      lcd_send_data(~eng_font[((ch - 0x20) * 5) + i]);
  }
  if(!mode)  lcd_send_data(0x00);     
  else       lcd_send_data(0xFF);      
}

//// put string on lcd (5x7 font-->6x8 size)
void lcd_puts(unsigned char x, unsigned char y, unsigned char mode, unsigned char* string)
{
  int i;  
  unsigned char cp = 0;

  lcd_set_xy(x, y);  
  while(string[cp] != '\0' && cp < 22) {
    for (i=0; i<=4; i++) {
      if(!mode) lcd_send_data(eng_font[((string[cp] - 0x20) * 5) + i]);
      else      lcd_send_data(~eng_font[((string[cp] - 0x20) * 5) + i]);
    }  
    if(!mode)  lcd_send_data(0x00);     
    else       lcd_send_data(0xFF);    
    cp++;
  }   
}

//// printf on lcd (5x7 font-->6x8 size)
void lcd_printf(unsigned char x, unsigned char y, unsigned char mode, const char *fmt,...)
{
  char string[26];
  va_list ap;   
  
//	if(sem_lcd == NULL)
//	{
//		return;
//	}  
//  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{  
    memset((unsigned char*)string,0,26);
    lcd_set_xy(x, y);     
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
    
    lcd_puts(x, y, mode, (unsigned char*)string);
//    osSemaphoreRelease(sem_lcd);
//  }      
}

//// put a big character on lcd (8x16 font)
void lcd_putc_big(unsigned char x, unsigned char y, unsigned char mode, unsigned char ch)
{
  int i;
  
  lcd_set_xy(x, y);         
  for(i=0; i<16; i++) {
    if(i == 8)
      lcd_set_xy(x, y+1);         
    if(!mode)
      lcd_send_data(E_font[ch][i]);
    else
      lcd_send_data(~E_font[ch][i]);
  }
}

unsigned int ks_code_conversion(unsigned short kssm)	/* convert KSSM(완성형) to KS(조합형) */
{
  unsigned char hb, lb;
  unsigned int index, ks;

  hb = kssm >> 8;
  lb = kssm & 0x00FF;

  if(kssm >= 0xB0A1 && kssm <= 0xC8FE)  { 
      index = (hb - 0xB0)*94 + lb - 0xA1;
      ks  =  KS_Table[index][0] * 256;
      ks |=  KS_Table[index][1];
      return ks;
  }
  else
    return 1;
}

//// put a hangul character on lcd (16x16 font)
void lcd_putc_han(unsigned char x, unsigned char y, unsigned char mode, unsigned short han_code)
{
  int i;

  unsigned char cho_5bit, joong_5bit, jong_5bit;
  unsigned char cho_bul, joong_bul, jong_bul=0, jong_flag;
  unsigned int cho_idx, joong_idx, jong_idx;
  unsigned char han_buff[32];		// 32 byte hangul font buffer

  han_code = ks_code_conversion(han_code);

  cho_5bit = table_cho[(han_code >> 10) & 0x001F];
  joong_5bit = table_joong[(han_code >> 5) & 0x001F];
  jong_5bit = table_jong[han_code & 0x001F];
  
  if(jong_5bit == 0)	{			// don't have jongsung 
    jong_flag = 0;
    cho_bul = bul_cho1[joong_5bit];
    if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 0;
    else                                    joong_bul = 1;
  }
  else	{					// have jongsung
    jong_flag = 1;
    cho_bul = bul_cho2[joong_5bit];
    if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 2;
    else                                    joong_bul = 3;
    jong_bul = bul_jong[joong_5bit];
  }
  
  cho_idx = cho_bul*20 + cho_5bit;		//  chosung font
  joong_idx = 8*20 + joong_bul*22 + joong_5bit;	//  joongsung font
  if(jong_flag == 1)	jong_idx = 8*20 + 4*22 +jong_bul*28 + jong_5bit;		// if jongsung
    
  for(i = 0; i < 32; i++) {
    han_buff[i] = K_font[cho_idx][i]|K_font[joong_idx][i];
    if(jong_flag == 1) han_buff[i] |= K_font[jong_idx][i];
  }
  
  lcd_set_xy(x, y);         
  for(i=0; i<32; i++) {
    if(i == 16)
      lcd_set_xy(x, y+1);         
    if(!mode)
      lcd_send_data(han_buff[i]);
    else
      lcd_send_data(~han_buff[i]);
  }
}

//// put a big string on lcd (8x16 font)
void lcd_puts_big(unsigned char x, unsigned char y, unsigned char mode, unsigned char* string)
{
  unsigned char cp = 0;
  unsigned short han_code;

  while(string[cp] != '\0' && cp < 16) {
    if(string[cp] < 0x80) {
      lcd_putc_big((x+cp*8), y, mode, string[cp]);
      cp++;
    }
    else {
      han_code = string[cp]*0x100+string[cp+1];
      lcd_putc_han((x+cp*8), y, mode, han_code);
      cp += 2;
    }
  }   
}

void lcd_puts_big_len(unsigned char x, unsigned char y, unsigned char mode, unsigned char* string, unsigned char len)
{
  unsigned short i, han_code;

  for(i = 0; i < len; i++) {
    if(string[i] < 0x80) {
      lcd_putc_big((x+i*8), y, mode, string[i]);
    }
    else {
      han_code = string[i]*0x100+string[i+1];
      lcd_putc_han((x+i*8), y, mode, han_code);
      i++;
    }
  }   
}

//// printf on lcd (5x7 font-->6x8 size)
void lcd_big_printf(unsigned char x, unsigned char y, unsigned char mode, const char *fmt,...)
{
  char string[18];
  va_list ap;    

//	if(sem_lcd == NULL)
//	{
//		return;
//	}  
//  if(osSemaphoreWait(sem_lcd, 50) == osOK) 	{   
    memset((unsigned char*)string,0,18);
    lcd_set_xy(x, y);     
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
    
    lcd_puts_big(x, y, mode, (unsigned char*)string);
//    osSemaphoreRelease(sem_lcd);
//  }    
}

/******************* (C) COPYRIGHT 2018 LOOP  ******************END OF FILE****/
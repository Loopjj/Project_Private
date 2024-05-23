/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : spi_lcd.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : spi lcd header
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __LCD_H
#define __LCD_H

#ifdef LCD_GLOBALS
#define LCD_EXT
#else
#define LCD_EXT	extern
#endif 

/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DISPLAY_ON		      0xAF
#define DISPLAY_OFF		      0xAE
#define START_LINE		      0x40	// ~ 0x7F
#define PAGE_ADDRESS		    0xB0	// ~ 0xB8
#define COLUMN_ADDRESS_LO	  0x00	// ~ 0x0F
#define COLUMN_ADDRESS_HI	  0x10	// ~ 0x1F
#define HORIZONTAL_NORMAL	  0xA0
#define HORIZONTAL_REVERSE	0xA1
#define DISPLAY_NORMAL		  0xA6
#define DISPLAY_INVERSE		  0xA7
#define ALL_POINT_OFF		    0xA4
#define ALL_POINT_ON		    0xA5
#define LCD_BIAS_1OF9		    0xA2
#define LCD_BIAS_1OF7		    0xA3
#define R_M_W_MODE		      0xE0
#define R_M_W_MODE_END		  0xEE
#define LCD_RESET		        0xE2
#define VERTICAL_NORMAL		  0xC0
#define VERTICAL_REVERSE	  0xC8
#define ALL_POWER_OFF		    0x28
#define ALL_POWER_ON		    0x2F
#define V5_VOLTAGE_RATIO	  0x20	// ~ 0x27
#define LCD_CONTRAST		    0x81	// + 0x01 ~ 0x3F
#define BOOSTER_RATIO		    0xF8	// + 0x00 ~ 0x01

#define STATUS_BUSY		      0x80
#define STATUS_ADC		      0x40
#define STATUS_ON_OFF		    0x20
#define STATUS_RESET		    0x10

#define CONTRAST_MAX     0x20
#define CONTRAST_MIN     0x10
#define CONTRAST_DEFAULT 0x19

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//- LCD Driver ---------------------------------------------------------------//
LCD_EXT u8 LCD_Refresh_Flag;
LCD_EXT u8 Contrast, Contrast_Flag;

LCD_EXT void lcd_init_semaphore(void);
LCD_EXT void lcd_init(void);
LCD_EXT void lcd_refresh(void);
LCD_EXT void lcd_reset(void);
LCD_EXT void lcd_backlight(unsigned char val);
LCD_EXT void lcd_contrast(unsigned char val);
LCD_EXT void lcd_clear(void);
LCD_EXT void lcd_write(unsigned char data);
LCD_EXT void lcd_send_cmd(unsigned char cmd);
LCD_EXT void lcd_send_data(unsigned char data);

LCD_EXT void lcd_set_xy(unsigned char x, unsigned char y);
LCD_EXT void lcd_draw_rect(unsigned char x, unsigned char x2, unsigned char y);
LCD_EXT void lcd_fill_box(unsigned char x, unsigned char x2, unsigned char y, unsigned char thickness);
LCD_EXT void lcd_bitmap(unsigned char x, unsigned char y, unsigned char sx, unsigned char sy, unsigned char mode, unsigned char *bitmap);

LCD_EXT void lcd_putc(unsigned char x, unsigned char y, unsigned char mode, char ch);
LCD_EXT void lcd_puts(unsigned char x, unsigned char y, unsigned char mode, unsigned char* string);
LCD_EXT void lcd_printf(unsigned char x, unsigned char y, unsigned char mode, const char *fmt,...);

LCD_EXT void lcd_putc_big(unsigned char x, unsigned char y, unsigned char mode, unsigned char ch);
LCD_EXT void lcd_puts_big(unsigned char x, unsigned char y, unsigned char mode, unsigned char* string);
LCD_EXT void lcd_puts_big_len(unsigned char x, unsigned char y, unsigned char mode, unsigned char* string, unsigned char len);
LCD_EXT void lcd_big_printf(unsigned char x, unsigned char y, unsigned char mode, const char *fmt,...);

#endif
/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/
/*
*************************************** (C) COPYRIGHT 2013 loop *************************************
*
* File Name          : i2cfram_FM24CL64B.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/14/2014
*
* Description        : I2C1 line interface configuration
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	RTCI2C_H
#define	RTCI2C_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef I2CRTC_GLOBALS
#define RTC_EXT
#else
#define RTC_EXT extern
#endif

#define RTC_DEV				I2C1    // rtc¿¬°á 


#define RTC_ADDRESS			0x60

#define RTC_TIMEOUT			((uint32_t)0x2000)
#define RTC_LONG_TIMEOUT		((uint32_t)(10 * RTC_TIMEOUT))

//typedef enum {TX = 0, RX = 1, TXREG = 2} RTC_Dir;


#define S35390A_CMD_STATUS1   0
#define S35390A_CMD_STATUS2   1
#define S35390A_CMD_TIME1     2
#define S35390A_CMD_INT1	  4

#define S35390A_BYTE_YEAR     0
#define S35390A_BYTE_MONTH    1
#define S35390A_BYTE_DAY      2
#define S35390A_BYTE_WDAY     3
#define S35390A_BYTE_HOURS    4
#define S35390A_BYTE_MINS     5
#define S35390A_BYTE_SECS     6

#define S35390A_FLAG_POC      0x01
#define S35390A_FLAG_BLD      0x02
#define S35390A_FLAG_24H      0x40
#define S35390A_FLAG_RESET    0x80
#define S35390A_FLAG_TEST     0x01

#define S35390A_FLAG_INT1FE		0x80
#define S35390A_FLAG_INT1ME		0x40
#define S35390A_FLAG_TEST		0x01

#define S35390A_HZ_1			0x80
#define S35390A_HZ_2			0x40
#define S35390A_HZ_4			0x20
#define S35390A_HZ_8			0x10


typedef union {

	struct _U8{
		INT8U data;
	}_u8;
	struct {
		INT8U rw  :1;
		INT8U cmd :3;
		INT8U addr:4;
	}s35390a;

}i2cdev_cmd_t;



RTC_EXT void rtcSemaphore_Init(void);
RTC_EXT uint8_t rtcwrite(char cmd, char *data,int len);
RTC_EXT uint8_t rtcread(char cmd, char *data, int len);
RTC_EXT int Extern_RTC_Init(void);
RTC_EXT int s35390a_set_datetime( struct tm *tm_ptr);
RTC_EXT uint8_t s35390a_get_datetime(struct tm *tm_ptr);


RTC_EXT INT16U rtc_Read_test(void);
RTC_EXT INT16U rtc_Write_test(void);
RTC_EXT INT16U dbg_rtc_setTest(void);
RTC_EXT INT16U dbg_rtc_gettimeTest(void);

#define RTC_COMMANDS \
	{"rr",   rtc_Read_test, ALL_DIR, "rr addr data"},\
	{"rw",   rtc_Write_test, ALL_DIR, "rw addr data"},\
	{"rset",   dbg_rtc_setTest, ALL_DIR, "rset yy mm dd hh mm ss wd"},\
	{"rget",   dbg_rtc_gettimeTest, ALL_DIR, "rget"},\

#endif
/*
*************************************** (C) COPYRIGHT 2014 loop *************************************
*
* File Name          : i2c_RTC.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 10/20/2014
*
* Description        : S-35390A RTC Chip interface configuration
*********************************************************************************************************
*/

/*
*                                             INCLUDE FILES
*****************************************************************************************
*************************************************************************************************************************
*/

#define I2CRTC_GLOBALS
#include "includes.h"



#define FROMBCD(x)      (((x) >> 4) * 10 + ((x) & 0xf)) 
#define TOBCD(x)        (((x) / 10 * 16) + ((x) % 10)) 

#define EXRTC_SDA	GPIO_Pin_7
#define EXRTC_SCL	GPIO_Pin_6
#define ACK_data	0
#define NACK_data	1

INT8U RTC_SAVE_ADDR=0;


static const uint8_t nibbletab[] = {
/* 0x0 0000 -> 0000 */  0x0,
/* 0x1 0001 -> 1000 */  0x8,
/* 0x2 0010 -> 0100 */  0x4,
/* 0x3 0011 -> 1100 */  0xc,
/* 0x4 0100 -> 0010 */  0x2,
/* 0x5 0101 -> 1010 */  0xa,
/* 0x6 0110 -> 0110 */  0x6,
/* 0x7 0111 -> 1110 */  0xe,
/* 0x8 1000 -> 0001 */  0x1,
/* 0x9 1001 -> 1001 */  0x9,
/* 0xa 1010 -> 0101 */  0x5,
/* 0xb 1011 -> 1101 */  0xd,
/* 0xc 1100 -> 0011 */  0x3,
/* 0xd 1101 -> 1011 */  0xb,
/* 0xe 1110 -> 0111 */  0x7,
/* 0xf 1111 -> 1111 */  0xf,
 }; 
static uint8_t bitreverse(uint8_t x) 
{
	return (nibbletab[x & 0xf] << 4) | nibbletab[x >> 4];
}

void rtcSemaphore_Init(void)
{
	BSP_I2C1Sem = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_I2C1Sem);
}

void rDelay()
{
	int nCount = 0x0f;//0xfff;
	for(; nCount != 0; nCount--);
}

void SDA_Set()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = EXRTC_SDA;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, EXRTC_SDA);
}
void SDA_Reset()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = EXRTC_SDA;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, EXRTC_SDA);
}

void SCL_Set()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = EXRTC_SCL;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, EXRTC_SCL);
}

void SCL_Reset()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = EXRTC_SCL;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, EXRTC_SCL);
}

void I2C_clock()
{
	SCL_Set();
	rDelay();
	SCL_Reset();
	rDelay();
}

void I2C_start()
{
	SDA_Set();
	rDelay(); 
	SCL_Set(); 
	rDelay();
	SDA_Reset(); 
	rDelay(); 
	SCL_Reset();
	rDelay();
}

void I2C_stop()
{
	SDA_Reset(); 
	rDelay(); 
	SCL_Set(); 
	rDelay(); 
	SDA_Set(); 
	rDelay(); 
}

void I2C_nack()
{
	SDA_Set(); 
	I2C_clock();
}
void I2C_ack()
{
	SDA_Reset(); 
	I2C_clock();
}

void I2C_write(char d)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(d&(0x80>>i))	SDA_Set(); 
		else			SDA_Reset();
		I2C_clock(); 
	}
	I2C_nack();
}

char I2C_read(char ack)
{
	char i,data=0;
	SDA_Set();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = EXRTC_SDA;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	for(i=0;i<8;i++)
	{
		rDelay(); 
		SCL_Set(); 
		rDelay();
		if(GPIO_ReadInputDataBit(GPIOB, EXRTC_SDA))
		data |= 0x80>>i;
		SCL_Reset();
	}
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	if(ack==ACK_data)I2C_ack();
	else I2C_nack();
	return data;
}




uint8_t rtc_write(uint8_t addr, uint8_t *buffer, uint8_t len) 
{
	char i=0;

	if(BSP_I2C1Sem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_I2C1Sem, ( portTickType ) 10 )  == pdTRUE)
	{

		I2C_start();
		I2C_write(addr);
		for(i=0;i<len;i++)
		{
			I2C_write(buffer[i]);
		}
		I2C_stop();
		rDelay();
		xSemaphoreGive(BSP_I2C1Sem);
		return OK;
	}
	return ERROR;
}


uint8_t rtc_read(uint8_t addr, uint8_t *rx_buffer, uint8_t rxlen)
{
	char i=0;
	if(BSP_I2C1Sem == NULL)
	{
		printk("%s err\n",__func__);
		return ERROR;
	}
	if(xSemaphoreTake(BSP_I2C1Sem, ( portTickType ) 10 )  == pdTRUE)
	{

		I2C_start();
		I2C_write(addr);
		//for(i=0;i<rxlen;i++)
		//	rx_buffer[i]=I2C_read(NACK_data);
		if(rxlen == 1)
			rx_buffer[i]=I2C_read(NACK_data);
		else
		{
			for(i=0;i<rxlen-1;i++)
				rx_buffer[i]=I2C_read(ACK_data);
			rx_buffer[i]=I2C_read(NACK_data);
		}
		I2C_stop();

		xSemaphoreGive(BSP_I2C1Sem);
		return OK;
	}
	return ERROR;
}



uint8_t rtcwrite(char cmd, char *data,int len)
{

	uint8_t ret = 0,i;
	uint8_t buff[10];
	i2cdev_cmd_t tCmd={0};

	tCmd.s35390a.rw = 0;
	tCmd.s35390a.cmd = cmd;
	tCmd.s35390a.addr =  (RTC_ADDRESS >> 4) & 0x0f;
	
	RTC_SAVE_ADDR = tCmd._u8.data ;

	for(i=0; i< len; i++)
	{
		buff[i] = data[i];
	}
	ret = rtc_write(RTC_SAVE_ADDR, buff, len);

	return ret;
}

uint8_t rtcread(char cmd, char *data, int len)
{
	uint8_t ret = 0;
	i2cdev_cmd_t tCmd={0};

	tCmd.s35390a.rw = 1;
	tCmd.s35390a.cmd = cmd;
	tCmd.s35390a.addr =  (RTC_ADDRESS >> 4) & 0x0f;

	RTC_SAVE_ADDR = tCmd._u8.data ;

	//ret = rtc_read(buff, 0, (uint8_t *)data, len);
	ret = rtc_read(RTC_SAVE_ADDR, (uint8_t *)data, len);

	return ret;

}

int s35390a_set_datetime( struct tm *tm_ptr)
{
	uint8_t i, err;
    char buf[7];

    printk("%s: tm is secs=%d, mins=%d, hours=%d mday=%d, "
    "mon=%d, year=%d, wday=%d\n", __func__, tm_ptr->tm_sec,
    tm_ptr->tm_min, tm_ptr->tm_hour, tm_ptr->tm_mday, tm_ptr->tm_mon, tm_ptr->tm_year,
    tm_ptr->tm_wday);

    buf[S35390A_BYTE_YEAR]  = TOBCD(tm_ptr->tm_year % 100);
    buf[S35390A_BYTE_MONTH] = TOBCD(tm_ptr->tm_mon );
    buf[S35390A_BYTE_DAY]   = TOBCD(tm_ptr->tm_mday);
    buf[S35390A_BYTE_WDAY]  = TOBCD(tm_ptr->tm_wday);
    buf[S35390A_BYTE_HOURS] = TOBCD(tm_ptr->tm_hour);
    buf[S35390A_BYTE_MINS]  = TOBCD(tm_ptr->tm_min);
    buf[S35390A_BYTE_SECS]  = TOBCD(tm_ptr->tm_sec);

   /* This chip expects the bits of each byte to be in reverse order */
    for (i = 0; i < 7; ++i)
		buf[i] = bitreverse(buf[i]);

	err = rtcwrite( S35390A_CMD_TIME1, buf, sizeof(buf) );
    return err;
}

uint8_t s35390a_get_datetime(struct tm *tm_ptr)
{
    char buf[10];
	char tmp[10];
    uint8_t i, err;

	err = rtcread(S35390A_CMD_TIME1, tmp, 7);
	if (err == ERROR)
	{
		printk("extern rtc read error....\n");
		return err;
	}

    /* This chip returns the bits of each byte in reverse order */
    for (i = 0; i < 7; ++i)
		buf[i] = bitreverse(tmp[i]);

    tm_ptr->tm_sec  = FROMBCD(buf[S35390A_BYTE_SECS]);
    tm_ptr->tm_min  = FROMBCD(buf[S35390A_BYTE_MINS]);
	tm_ptr->tm_hour = FROMBCD(buf[S35390A_BYTE_HOURS] & 0x3F);
    tm_ptr->tm_wday = FROMBCD(buf[S35390A_BYTE_WDAY] & 0x07);
    tm_ptr->tm_mday = FROMBCD(buf[S35390A_BYTE_DAY]);
    tm_ptr->tm_mon  = FROMBCD(buf[S35390A_BYTE_MONTH]);
	tm_ptr->tm_year = FROMBCD(buf[S35390A_BYTE_YEAR]) + 2000;

    return err;
}


int s35390a_int1(void)
{
	INT8U	ret=0;
	unsigned char buf;

	buf = 0;
	ret = rtcread( S35390A_CMD_STATUS2, (char *)&buf, 1);
	if(!((buf & S35390A_FLAG_INT1FE) && !(buf & S35390A_FLAG_INT1ME) && !(buf & S35390A_FLAG_TEST))) 
	{
		buf |= S35390A_FLAG_INT1FE;
		buf &= ~S35390A_FLAG_INT1ME;  //1sec
		buf &= ~S35390A_FLAG_TEST;
		ret = rtcwrite(S35390A_CMD_STATUS2, (char *)&buf, 1);
	}

	buf = S35390A_HZ_1;
	ret = rtcwrite( S35390A_CMD_INT1, (char *)&buf, 1);
	return ret;
}	

int Extern_RTC_Init(void)
{
	INT8U ret = 0;
	INT8U data;

	ret = rtcread(S35390A_CMD_STATUS1, (char *)&data, 1);

	if( data & (S35390A_FLAG_POC | S35390A_FLAG_BLD) )
	{
		data |= S35390A_FLAG_24H | S35390A_FLAG_RESET;
		ret = rtcwrite(S35390A_CMD_STATUS1, (char *)&data, 1);
		printd("Extern rtc register 1 init...\n");
	}

	ret = rtcread(S35390A_CMD_STATUS2, (char *)&data, 1);
	if( data & S35390A_FLAG_TEST)
	{
		data &= ~S35390A_FLAG_TEST;
		ret = rtcwrite(S35390A_CMD_STATUS2, (char *)&data, 1);
		printd("Extern rtc register 2 init...\n");
	}
    return ret;
}


INT16U dbg_rtc_setTest(void)
{
	sysinfo_t *s = &_sysinfo 	;
	INT32U yy,mon,day,hh,mm,ss,wday;


	if(CLI_GetArgCnt() < 7) return INV_PRM;
	s->rtcSetFlag = EXT_CMDWAT;

	
	yy  = comparison_num((char *)P_ARG(1));
	mon = comparison_num((char *)P_ARG(2));
	day= comparison_num((char *)P_ARG(3));
	hh  = comparison_num((char *)P_ARG(4));
	mm  = comparison_num((char *)P_ARG(5));
	ss  = comparison_num((char *)P_ARG(6));
	wday= comparison_num((char *)P_ARG(7));

	MakeTimePtr(&s->tm_ptr,yy, mon, day, hh, mm, ss,wday);

	s->rtcSetFlag = EXT_RTCSET;

	return NO_DISPLAY;
}

INT16U dbg_rtc_gettimeTest(void)
{

	sysinfo_t *s = &_sysinfo 	;

	printk("Read RTC Sec = %s\n",btime2str(&s->tm_ptr, "y-m-d h:mi:s"));

    return NO_DISPLAY;

}



//char buf[1024];
INT16U rtc_Read_test(void)
{
	char buf[10];
	INT8U addr,len,i;
	sysinfo_t *s = &_sysinfo 	;

	memset(buf, 0x0, sizeof(buf));
	
	if(CLI_GetArgCnt() < 2) return INV_PRM;
	s->rtcSetFlag = EXT_CMDWAT;
	
	addr = comparison_num((char *)P_ARG(1));
	len = comparison_num((char *)P_ARG(2));
	
	rtcread(addr, (char *)&buf, len);

	printk("Read data \n");
	for(i=0;i<len;i++)
	{
		printk("[%02X]",buf[i]);
	}
	printk("\n");

	s->rtcSetFlag = EXT_RTCGET;
			
	return NO_DISPLAY;
}


INT16U rtc_Write_test(void)
{
	char buf[10];
	INT8U addr,data;
	sysinfo_t *s = &_sysinfo 	;
	//int addr,len,i, ret = 0;

	memset(buf, 0x0, sizeof(buf));
	
	if(CLI_GetArgCnt() < 2) return INV_PRM;

	s->rtcSetFlag = EXT_CMDWAT;
	
	addr = comparison_num((char *)P_ARG(1));
	data = comparison_num((char *)P_ARG(2));

	rtcwrite(addr, (char *)&data, 1);
	printk("Write RTC addr = [0x%02X], data = [0x%02X]\n",addr, data);

	s->rtcSetFlag = EXT_RTCGET;

			
	return NO_DISPLAY;
}

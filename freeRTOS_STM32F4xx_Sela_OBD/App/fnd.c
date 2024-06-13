/*
*************************************** (C) COPYRIGHT 2015 Loop *************************************
*
* File Name          : fnd.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 04/05/2015
*
* Description        : fnd display
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define FND_GLOBALS
#include "includes.h"



/*
*********************************************************************************************************
*                                               CONSTANT
*********************************************************************************************************
*/
INT16U FND_DispCnt = 0;


/*
*********************************************************************************************************
*                                                FUNCTIONS
*********************************************************************************************************
*/
void SetDataFND(INT8U data)
{
	if(data  & 0x01 )   FND_SET(A, 1);
	else				FND_SET(A, 0);
	if(data  & 0x02 )   FND_SET(B, 1);
	else				FND_SET(B, 0);
	if(data  & 0x04 )   FND_SET(C, 1);
	else				FND_SET(C, 0);
	if(data  & 0x08 )   FND_SET(D, 1);
	else				FND_SET(D, 0);
	if(data  & 0x10 )   FND_SET(E, 1);
	else				FND_SET(E, 0);
	if(data  & 0x20 )   FND_SET(F, 1);
	else				FND_SET(F, 0);
	if(data  & 0x40 )   FND_SET(G, 1);
	else				FND_SET(G, 0);
	if(data  & 0x80 )   FND_SET(P, 1);
	else				FND_SET(P, 0);
}

void SetFnd(INT8U cs)
{
	INT8U flag=0;

	if(FND_DispCnt < 100)			// DispCnt = Twinkling period
		flag = keyflag.twinkflag;
	else if(FND_DispCnt < 200)
		flag = 0;
	else FND_DispCnt = 0;
  
	switch(cs)
	{
	case 0:
    if(flag != 5) {
      SetDataFND( FND_DATA[0]  );
      FND_CS(1, 1);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
    else{
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
		break;
	case 1:
    if(flag != 4) {
      SetDataFND( FND_DATA[1]  );
      FND_CS(1, 0);
      FND_CS(2, 1);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
    else{
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
		break;
	case 2:
    if(flag != 3) {
      SetDataFND( FND_DATA[2] );
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 1);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
    else{
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
		break;
	case 3:
    if(flag != 2) {
      SetDataFND( FND_DATA[3] );
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 1);
      FND_CS(5, 0);
    }
    else{
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
		break;
	case 4:
    if(flag != 1) {
      SetDataFND( FND_DATA[4]  );
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 1);
    }
    else{
      FND_CS(1, 0);
      FND_CS(2, 0);
      FND_CS(3, 0);
      FND_CS(4, 0);
      FND_CS(5, 0);
    }
		break;
	default:
		break;
	}
  FND_DispCnt++;
}
/*
*********************************************************************************************************
*                                            fnd_callback_fun()
*
* Description : GPIO Set FND Control.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Timer Callback function ....Main.c().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void fnd_callback_fun(void)
{
	static INT8U nCount=0;

	SetFnd(nCount++);

	if(nCount >= 5) nCount = 0;
}


#define LED_NO	0		// led normal status
#define LED_TE	1		// led temperature status
#define LED_SD	2		// led sd card status
#define LED_PH	3		// led press high alarm
#define LED_PL	4		// led press low alarm


/*
*********************************************************************************************************
*                                            led_set_fun()
*
* Description : GPIO Set LED Control.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void led_set_fun(INT8U number, INT8U onoff)
{
	switch(number)
	{
	case LED_NO:
		if(onoff)	LED_SET(NO, 0);
		else		LED_SET(NO, 1);
		break;
	case LED_TE:
		if(onoff)	LED_SET(TE, 0);
		else		LED_SET(TE, 1);
		break;
	case LED_SD:
		if(onoff)	LED_SET(SD, 0);
		else		LED_SET(SD, 1);
		break;
	case LED_PH:
		if(onoff)	LED_SET(PH, 0);
		else		LED_SET(PH, 1);
		break;
	case LED_PL:
		if(onoff)	LED_SET(PL, 0);
		else		LED_SET(PL, 1);
		break;
	default:
		break;
	}
}

/*
*********************************************************************************************************
*                                            FirstLedFndCheck()
*
* Description : FND & LED IO Test
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
INT8U InitFirstLedFndCheck(void)
{
	static INT8U nStatus = 1;
	switch(nStatus)
	{
	case 1:
		FND_SET(A, 1), FND_CS(1, 1), FND_CS(2, 1), FND_CS(3, 1), FND_CS(4, 1), FND_CS(5, 1);
		nStatus = 2;
		break;
	case 2:
		FND_SET(B, 1);
		nStatus = 3;
		break;
	case 3:
		FND_SET(C, 1);
		nStatus = 4;
		break;
	case 4:
		FND_SET(D, 1);
		nStatus = 5;
		break;
	case 5:
		FND_SET(E, 1);
		nStatus = 6;
		break;
	case 6:
		FND_SET(F, 1);
		nStatus = 7;
		break;
	case 7:
		FND_SET(G, 1);
		nStatus = 8;
		break;
	case 8:
		FND_SET(P, 1);
		nStatus = 9;
		break;
	case 9:
		FND_SET(A, 0),FND_SET(B, 0),FND_SET(C, 0),FND_SET(D, 0),FND_SET(E, 0),FND_SET(F, 0),FND_SET(G, 0),FND_SET(P, 0);
		FND_CS(1, 0), FND_CS(2, 0), FND_CS(3, 0), FND_CS(4, 0), FND_CS(5, 0);
		nStatus = 10;
		break;
	case 10:
		led_set_fun(0,ON);
		nStatus = 11;
		break;
	case 11:
		led_set_fun(1,ON);
		nStatus = 12;
		break;
	case 12:
		led_set_fun(2,ON);
		nStatus = 13;
		break;
	case 13:
		led_set_fun(3,ON);
		nStatus = 14;
		break;
	case 14:
		led_set_fun(4,ON);
		nStatus = 15;
		break;
	case 15:
		led_set_fun(0,OFF),led_set_fun(1,OFF),led_set_fun(2,OFF),led_set_fun(3,OFF),led_set_fun(4,OFF);
		nStatus = 0;
		break;


	default:
		nStatus = 0;
		break;
	}
	return nStatus;
}


/*
********************************************************************************
* Description : Buzzer Test
* Arguments   : None
* Return      : 
* Note        : 
******************************************************************************** 
*/

INT16U dbg_buzzer_test(void)
{

	INT8U data;
	if(CLI_GetArgCnt() < 1) return INV_PRM;
	
	data = comparison_num((char *)P_ARG(1));

	IO_SET(BU, data);
	printk("BUZZER Cotrol  = %d\n",data);

	return NO_DISPLAY;
}

INT16U dbg_doc_test(void)
{

	INT8U data;
	if(CLI_GetArgCnt() < 1) return INV_PRM;
	
	data = comparison_num((char *)P_ARG(1));

	IO_SET(DO, data);
	printk("DOC Cotrol  = %d\n",data);
	return NO_DISPLAY;
}


/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/
/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : commqueue.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/05/2014
*
* Description        : command background configuration
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	COMMQUEUE_H
#define	COMMQUEUE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef  COMMQUEUE_GLOBALS
#define COMMQUE_EXT
#else
#define COMMQUE_EXT  extern
#endif

#define MAXQUE_CH				5
#define COMM_RX_QUE_SIZE      512            /* Number of characters in Rx ring buffer            */
#define COMM_TX_QUE_SIZE      512			  /* Number of characters in Tx ring buffer            */


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/






typedef struct {
	xQueueHandle BufTxq;
	xQueueHandle BufRxq;
} combuffer_t;

COMMQUE_EXT combuffer_t com[MAXQUE_CH];




COMMQUE_EXT int uart_puts(INT8U ch, INT8U *pdata, INT32U SendCount);
COMMQUE_EXT void uart_init(void);
COMMQUE_EXT INT8U uart_getchar(INT8U ch,INT8U *err);
COMMQUE_EXT void xQueueInitCom(INT8U ch);


#endif
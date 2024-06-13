/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : commrtos.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 07/05/2014
*
* Description        : command background rtos configuration
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/


#define COMMRTOS_GLOBALS
#include "includes.h"


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

#define TX_STAT_BUSY	1
#define TX_STAT_IDLE	0

#define comBLOCK_TIME_WAITING_FOR_INPUT	( ( portTickType ) 100 )
			
typedef xSemaphoreHandle	Com_Sem;
/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
typedef struct {
	INT16U	 RingBufRxCtr;					/* Rx 링 버퍼에 저장된 데이터 수					*/
	Com_Sem  RingBufRxSem;					/* 수신 세마포어의 포인터							*/
	INT8U	 *RingBufRxInPtr;				/* 다음 데이터를 저장할 곳을 가리키는 포인터		*/
	INT8U	 *RingBufRxOutPtr;				/* 다음에 읽어갈 데이터를 가리키는 포인터			*/
	INT8U	 RingBufRx[COMM_RX_BUF_SIZE]; 	/* 수신용 링 버퍼 저장영역(Rx)						*/

	INT16U	 RingBufTxCtr;					/* Tx 링 버퍼에 저장된 데이터 수					*/
	Com_Sem  RingBufTxSem;					/* 송신 세마포어의 포인터							*/
	INT8U	 *RingBufTxInPtr;				/* 다음 데이터를 저장할 곳을 가리키는 포인터		*/
	INT8U	 *RingBufTxOutPtr;			 	/* 다음에 읽어갈 데이터를 가리키는 포인터			*/
	INT8U	 RingBufTx[COMM_TX_BUF_SIZE];	/* 송신용 링 버퍼 저장영역(Tx)						*/

} COMM_RING_BUF;
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif


/*
*********************************************************************************************************
*											 GLOBAL VARIABLE
*********************************************************************************************************
*/

static INT8U		 tx_stat[MAXCOMM_CH] = {0};

#pragma location = "EXTCCRAM"
static COMM_RING_BUF CommBuf[MAXCOMM_CH];

static USART_TypeDef *pUART_BASE[5] = {USART1,USART2,USART3,UART4,UART5};

//static DMA_Stream_TypeDef *pDMA_TXBASE[3] = {DMA1_Stream4, DMA1_Stream7, DMA1_Stream2};
//static DMA_Stream_TypeDef *pDMA_RXBASE[3] = {DMA1_Stream5, DMA1_Stream6, DMA1_Stream3};

xSemaphoreHandle USB_TxSem = NULL;


/*
*********************************************************************************************************
*                                              FUNCTIONS
*********************************************************************************************************
*/


INT16U Comm_GetTxBufCnt(INT8U ch)
{
	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	return pbuf->RingBufTxCtr;
}


INT16U Comm_GetRxBufCnt(INT8U ch)
{
	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	return pbuf->RingBufRxCtr;
}
INT8U Comm_balanceCheck(INT8U ch)
{
	INT8U ret;

	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	if(	pbuf->RingBufRxInPtr > pbuf->RingBufRxOutPtr)
	{
		ret = COMM_RX_UNBALANCE;
	}
	else
		ret = COMM_NO_ERR;
	return ret;
}



void Comm_Tx_Stat(INT8U ch, INT8U data)
{


	portENTER_CRITICAL();
	tx_stat[ch] = data;
	portEXIT_CRITICAL();
}


void __sub_Uart_RxInt(INT8U ch)
{
	INT8U c;
	
	c = (INT8U)USART_ReceiveData(pUART_BASE[ch]);
	CommPutRxChar(ch, c);
}


void __sub_Uart_TxInt(INT8U ch)
{
	INT8U c, err;

	c = CommGetTxChar(ch, &err);
	if(err != COMM_TX_EMPTY)
		USART_SendData(pUART_BASE[ch],c);	
	else{
		tx_stat[ch] = TX_STAT_IDLE;
	}
}


void __sub_Can_RxInt(CAN_TypeDef* CANx, INT8U ch)
{
	INT8U buf[25];
	INT32U i=0, ret=0;
	COMM_RING_BUF *pbuf;
 
	memset((char *)buf,0,sizeof(buf));
  ret = CAN_Receive_mk(CANx, CAN_FIFO0, buf);
	for(i=0; i<ret; i++) {
    pbuf = &CommBuf[ch];
    if (pbuf->RingBufRxCtr < COMM_RX_BUF_SIZE) {	
      pbuf->RingBufRxCtr++;							 
      *pbuf->RingBufRxInPtr++ = buf[i];						
      if (pbuf->RingBufRxInPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE]) { 
        pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
      }
    }
  }
}


void CommInit(void)
{
	INT32U nCount;
	COMM_RING_BUF *pbuf;

	for(nCount=0;nCount<MAXCOMM_CH;nCount++)
	{
		pbuf = &CommBuf[nCount];
		
		pbuf->RingBufRxCtr = 0;
		pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
		pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
		pbuf->RingBufRxSem = NULL;
		vSemaphoreCreateBinary(pbuf->RingBufRxSem);

		pbuf->RingBufTxCtr = 0;
		pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
		pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
		pbuf->RingBufTxSem = NULL;
		vSemaphoreCreateBinary(pbuf->RingBufTxSem);

		tx_stat[nCount] = TX_STAT_IDLE;	
	}
	vSemaphoreCreateBinary(USB_TxSem);
}


void CommReInit(int num)
{

	COMM_RING_BUF *pbuf;
	//INT8U err;

	pbuf = &CommBuf[num];

	portENTER_CRITICAL();

	pbuf->RingBufRxCtr    = 0;
	pbuf->RingBufRxInPtr  = &pbuf->RingBufRx[0];
	pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];

	pbuf->RingBufTxCtr    = 0;
	pbuf->RingBufTxInPtr  = &pbuf->RingBufTx[0];
	pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];

	tx_stat[num] = TX_STAT_IDLE;				

	portEXIT_CRITICAL();
}


BOOLEAN CommIsEmpty(INT8U ch)
{

	BOOLEAN	empty;
	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	portENTER_CRITICAL();
	if (pbuf->RingBufRxCtr > 0) {						 
		empty = FALSE;									 
	} else {
		empty = TRUE;									 
	}
	portEXIT_CRITICAL();
	return (empty);
}


BOOLEAN CommIsFull(INT8U ch)
{


	BOOLEAN	full;
	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	portENTER_CRITICAL();
	if (pbuf->RingBufTxCtr < COMM_TX_BUF_SIZE) {
		full = FALSE;									  
	} else {
		full = TRUE;									
	}
	portEXIT_CRITICAL();
	return (full);
}


void CommPutRxChar(INT8U ch, INT8U c)
{

	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];
	portENTER_CRITICAL();
	if (pbuf->RingBufRxCtr < COMM_RX_BUF_SIZE) {	
		pbuf->RingBufRxCtr++;							 
		*pbuf->RingBufRxInPtr++ = c;						
		if (pbuf->RingBufRxInPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE]) { 
			pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
		}
	}
	portEXIT_CRITICAL();
}

INT8U CommGetChar(INT8U ch, INT16U to, INT8U *err)
{
	INT8U c;

	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	if(pbuf->RingBufRxSem == NULL)
	{
		printd("%s err\n",__func__);
		*err = (COMM_TX_TIMEOUT);
		return (NUL);
	}

	if(xSemaphoreTake(pbuf->RingBufRxSem, ( portTickType ) to )  == pdTRUE)
	{
		portENTER_CRITICAL();
		if ((pbuf->RingBufRxCtr > 0)  || (Comm_balanceCheck(ch) == COMM_RX_UNBALANCE) )
		{
			if(pbuf->RingBufRxCtr)
				pbuf->RingBufRxCtr--;	

			c = *pbuf->RingBufRxOutPtr++;					  
			if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE]) {	
				pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
			}
			portEXIT_CRITICAL();
			*err = COMM_NO_ERR;
			xSemaphoreGive(pbuf->RingBufRxSem);
			return (c);
		}
		else
		{
			portEXIT_CRITICAL();
			*err = COMM_RX_EMPTY;
			xSemaphoreGive(pbuf->RingBufRxSem);
			return (NUL);
		}
	}
	return (NUL);
}


INT8U CommGetTxChar(INT8U ch, INT8U *err)
{
	INT8U c;
	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	if (pbuf->RingBufTxCtr > 0) {						 
		pbuf->RingBufTxCtr--;							
		c = *pbuf->RingBufTxOutPtr++;					
		if (pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[COMM_TX_BUF_SIZE]) {	 
			pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
		}
		*err = COMM_NO_ERR;
		return (c);								
	} else {
		*err = COMM_TX_EMPTY;
		return (NUL);									
	}


}

void BSP_usart_int_en(INT8U ch)
{
	USART_ITConfig(pUART_BASE[ch], USART_IT_TC, ENABLE);
}


INT8U CommPutChar(INT8U ch, INT8U c, INT16U to)
{

	COMM_RING_BUF *pbuf;

	pbuf = &CommBuf[ch];

	if(pbuf->RingBufTxSem == NULL)
	{
		printk("%s err\n",__func__);
		return (COMM_TX_TIMEOUT);
	}
	if(xSemaphoreTake(pbuf->RingBufTxSem, ( portTickType ) to )  == pdTRUE)
	{
		portENTER_CRITICAL();
		pbuf->RingBufTxCtr++;								  
		*pbuf->RingBufTxInPtr++ = c;							
		if (pbuf->RingBufTxInPtr == &pbuf->RingBufTx[COMM_TX_BUF_SIZE])
		{	 
			pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
		}

		if (pbuf->RingBufTxCtr == 1)
		{						
			if(tx_stat[ch]==TX_STAT_IDLE)
			{
				tx_stat[ch] = TX_STAT_BUSY;	
				BSP_usart_int_en(ch);
				__sub_Uart_TxInt(ch);
				portEXIT_CRITICAL();					
			}
			else
			{
				portEXIT_CRITICAL();
			}
		}
		else{
			portEXIT_CRITICAL();
		}
	}

	return (COMM_NO_ERR);
}
#if 1		//Serial Port USE

void CommPutCharStr(INT8U port, INT8U *pdata, INT32U SendCount)
{
	int i;

	//portENTER_CRITICAL();
	for(i=0;i<SendCount;i++)
	{
		USART_SendData(USART2,pdata[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
 	}
	//portEXIT_CRITICAL();
}


#else		//USB USE

#define USB_PACKET_SIZE	63
//define DEV_EP_TX_DIS   0x0000
//define DEV_EP_TX_STALL 0x0010
#define DEV_EP_TX_NAK   0x0020
//define DEV_EP_TX_VALID 0x0030

void CommPutCharStr(INT8U port, INT8U *pdata, INT32U SendCount)
{
	INT32U i, packet_cnt, rest_packet_cnt;

	if(port == GUI_PORT)
	{
		if( USB_TxSem != NULL)
		{

			if(xSemaphoreTake(USB_TxSem, ( portTickType ) 0 ) == pdTRUE )
			{
				if(SendCount > USB_PACKET_SIZE){					// 보낼 데이터가 63byte 이상이면 
					
					packet_cnt = SendCount / USB_PACKET_SIZE;			
					rest_packet_cnt = SendCount % USB_PACKET_SIZE;

					for(i=0; i<packet_cnt; i++){					// 63byte 단위로 나눠서 보낸다 
					  USB_SIL_Write(EP1_IN, &pdata[i*USB_PACKET_SIZE], USB_PACKET_SIZE);
					  SetEPTxValid(ENDP1);
					  while(GetEPTxStatus(ENDP1) != DEV_EP_TX_NAK);// OS_Sched();
				    }   
					USB_SIL_Write(EP1_IN, &pdata[i*USB_PACKET_SIZE], rest_packet_cnt);
				        SetEPTxValid(ENDP1);												
				}
				else{												// 보낼 데이터가 63byte 보다 작으면 
					USB_SIL_Write(EP1_IN, pdata, SendCount);
				        SetEPTxValid(ENDP1);
				}
				xSemaphoreGive(USB_TxSem);
			}
			else{
				printk("sem pend err  %s\n",__func__);
			}
		}
	}

	
}
#endif

void SendModemUart(int port, char *pdata, int SendCount)
{
	int i;

	portENTER_CRITICAL();
	for(i=0;i<SendCount;i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
		USART_SendData(USART3,pdata[i]);	
 	}
	portENABLE_INTERRUPTS();
}



/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

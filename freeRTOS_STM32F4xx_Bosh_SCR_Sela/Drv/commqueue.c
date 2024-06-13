


#define  COMMQUEUE_GLOBALS
#include "includes.h"




INT8U uart_getchar(INT8U ch,INT8U *err)
{
	INT8U c;
	portBASE_TYPE res;


	res = xQueueReceive(com[ch].BufRxq, &c, 10);

	if (res == pdPASS){
		*err = COMM_NO_ERR;
		return c;
	}
	else{
		*err = COMM_RX_EMPTY;
		return EOF;
	}
}
void xQueueInitCom(INT8U ch)
{
	xQueueReset(com[ch].BufRxq);
	xQueueReset(com[ch].BufTxq);
}


int uart_putchar(INT8U ch, INT8U c)
{
	portBASE_TYPE res;

	res = xQueueSendToBack(com[ch].BufTxq, (const void *)&c, 0);

	if (res == pdPASS)
		return c;
	else
		return EOF;
}


int uart_puts(INT8U ch, INT8U *pdata, INT32U SendCount)
{
	int i=0;
	for(i=0;i<SendCount;i++)
	{
		if(uart_putchar(ch, pdata[i]) == EOF)
			return -EOF;
	}
	return i;
}


static void uart_Sendtask(void *params)
{
	INT8U c;

#if 1
	for (;;) {
		///f (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET) {
		//if(pdPASS == xQueueReceive(com[2].BufTxq, &c, portMAX_DELAY))
		if(pdPASS == xQueueReceive(com[2].BufTxq, &c, 100))
		{
			USART_SendData(USART2, c);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
		}
		else 
		{
			vTaskDelay(1);
		}
	}
#else
	for(;;)
	{
		if(TmrChk(TMR_UARTSEND) == 0)
		{
			TmrSetT(TMR_UARTSEND, 10);
			TmrStart(TMR_UARTSEND);
			if(pdPASS == xQueueReceive(com[2].BufTxq, &c, portMAX_DELAY))
			{
				USART_SendData(USART2, c);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
			}
		}
		vTaskDelay(1);
	}
#endif
}

void Gui_uart_task_init(void)
{
	com[2].BufTxq = xQueueCreate(COMM_TX_QUE_SIZE, sizeof(INT8U));
	com[2].BufRxq = xQueueCreate(COMM_RX_QUE_SIZE, sizeof(INT8U));

	xTaskCreate(uart_Sendtask, (signed char *)"UART2", configMINIMAL_STACK_SIZE, (void *)NULL, COM_UART2_PRIO, NULL);
}


void uart_init(void)
{
	Gui_uart_task_init();
}

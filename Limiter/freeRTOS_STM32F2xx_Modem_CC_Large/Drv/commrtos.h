/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : commrtos.h
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
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	COMMRTOS_H
#define	COMMRTOS_H


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef  COMMRTOS_GLOBALS
#define COMMRTOS_EXT
#else
#define COMMRTOS_EXT  extern
#endif

#define MAXCOMM_CH				5
#define COMM_RX_BUF_SIZE      512             /* Number of characters in Rx ring buffer            */
#define COMM_TX_BUF_SIZE         1        /* Number of characters in Tx ring buffer            */


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/



                               
#define NUL                    0x00

                                                /* �����ڵ�                                           */
#define COMM_NO_ERR            0                /* �Լ� ȣ���� ���������� ����Ǿ���                  */
#define COMM_BAD_CH            1                /* ��ȿ�� ä���� ��������                             */
#define COMM_RX_EMPTY          2                /* ���� ���۰� �����. �����Ͱ� ����                  */
#define COMM_TX_FULL           3                /* �۽� ���۰� ������ á��. �����͸� ������ �� ����   */
#define COMM_TX_EMPTY          4                /* �۽� ���۰� �����                                 */
#define COMM_RX_TIMEOUT        5                /* ������ ���� ��� �� Ÿ�Ӿƿ� ���� �߻�             */
#define COMM_TX_TIMEOUT        6                /* ������ �۽� ��� �� Ÿ�Ӿƿ� ���� �߻�             */
#define COMM_RX_UNBALANCE	   7


#define COMM_PARITY_NONE       0                /* �и�Ƽ ���� ���                                   */
#define COMM_PARITY_ODD        1
#define COMM_PARITY_EVEN       2



/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
COMMRTOS_EXT INT8U Comm_balanceCheck(INT8U ch);

COMMRTOS_EXT void __sub_Uart_RxInt(INT8U ch);
COMMRTOS_EXT void __sub_Uart_TxInt(INT8U ch);

COMMRTOS_EXT void __sub_Can_RxInt(INT8U ch, INT8U data);

COMMRTOS_EXT INT16U Comm_GetTxBufCnt(INT8U ch);
COMMRTOS_EXT INT16U Comm_GetRxBufCnt(INT8U ch);
COMMRTOS_EXT void Comm_Tx_Stat(INT8U ch, INT8U data);

COMMRTOS_EXT void CommInit(void);
COMMRTOS_EXT void CommReInit(int num);

COMMRTOS_EXT BOOLEAN CommIsEmpty(INT8U ch);
COMMRTOS_EXT BOOLEAN CommIsFull(INT8U ch);

COMMRTOS_EXT void CommPutRxChar(INT8U ch, INT8U c);
COMMRTOS_EXT INT8U CommGetChar(INT8U ch, INT16U to, INT8U *err);

COMMRTOS_EXT INT8U CommGetTxChar(INT8U ch, INT8U *err);
COMMRTOS_EXT INT8U CommPutChar(INT8U ch, INT8U c, INT16U to);
COMMRTOS_EXT INT8U CommPutStr(INT8U ch, char *pdata,int SendCount);

COMMRTOS_EXT void CommPutCharStr(INT8U port, INT8U *pdata, INT32U SendCount);
COMMRTOS_EXT void modemPutCharStr(INT8U *pdata, INT32U SendCount);





#endif
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

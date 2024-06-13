/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : tmr.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 07/05/2014
*
* Description        : tmr configuration
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define TMR_GLOBALS
#include "includes.h"


/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

//static OS_EVENT *TmrSemTenths;
//static OS_STK TmrTaskStk[TMR_TASK_STK_SIZE];
static INT8U TmrTickCtr;


/*
*********************************************************************************************************
*                                        LOCAL FUNCTION PROTOTYPES 
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               FUNCTIONS 
*********************************************************************************************************
*/


void TmrCfgFnct(INT32U n, void(*fnct)(void *), void *arg)
{
    TMR *ptmr;

    if(n < TMR_MAX_TMR){
		ptmr = &TmrTbl[n];
		portENTER_CRITICAL();
		ptmr->TmrFnct = fnct;              /* Ÿ�̸ӿ� ����� �Լ� �����͸� �����Ѵ�             */
		ptmr->TmrFnctArg = arg;            /* ����� �Լ��� ���� ���������� �����͸� �����Ѵ�    */
		portEXIT_CRITICAL();
    }
}



INT32U TmrChk(INT32U n)
{
    INT32U val;

    val = 0;
	
    if (n < TMR_MAX_TMR) {
        portENTER_CRITICAL();
        val = TmrTbl[n].TmrCtr;
        portEXIT_CRITICAL();
    }
    return (val);
}



void TmrFormat(INT32U n, char *s)
{
    INT32U min;
    INT32U sec;
    INT32U tenths;
    INT32U val;

    if(n < TMR_MAX_TMR){
        portENTER_CRITICAL();
        val    = TmrTbl[n].TmrCtr;             /* ��ȯ�� ���� Ÿ�̸��� ���� �����Ѵ�            */
        portEXIT_CRITICAL();
        min    = (INT32U)(val / 60000);
        sec    = (INT32U)((val - min * 60000) / 1000);
        tenths = (INT32U)(val % 10);
        s[0]   = min / 10 + '0';               /* TIMER�� ASCII�� ��ȯ�Ѵ�                      */
        s[1]   = min % 10 + '0';
        s[2]   = ':';
        s[3]   = sec / 10 + '0';
        s[4]   = sec % 10 + '0';
        s[5]   = '.';
        s[6]   = tenths / 100  + '0';
        s[7]   = tenths / 10   + '0';
		s[8]   = tenths        + '0';
    }
}

void  TmrReset (INT32U n)
{
    TMR *ptmr;

    if(n < TMR_MAX_TMR){
        ptmr         = &TmrTbl[n];
        portENTER_CRITICAL();
        ptmr->TmrCtr = ptmr->TmrInit;                       /* ī���͸� ���ε� �Ѵ�              */
        portEXIT_CRITICAL();
    }
}



void TmrSetMST(INT32U n, INT32U min, INT32U sec, INT32U tenths)
{
    TMR *ptmr;
    INT32U val;

    if(n < TMR_MAX_TMR){
        ptmr          = &TmrTbl[n];
        val           = (INT32U)min * 600 + (INT32U)sec * 10 + (INT32U)tenths;
        portENTER_CRITICAL();
        ptmr->TmrInit = val;
        ptmr->TmrCtr  = val;
        portEXIT_CRITICAL();
    }
}


void TmrSetT(INT32U n, INT32U tenths)
{
    TMR *ptmr;

    if(n < TMR_MAX_TMR){
        ptmr          = &TmrTbl[n];
        portENTER_CRITICAL();
        ptmr->TmrInit = tenths / (1000 / 100);
        ptmr->TmrCtr  = tenths / (1000 / 100);
        portEXIT_CRITICAL();
    }
}



void TmrSignalTmr(void)
{
	TmrTickCtr++;
	if(TmrTickCtr >= 1){
		TmrTickCtr = 0;
	}
}



void TmrStart(INT32U n)
{

    if(n < TMR_MAX_TMR){
        portENTER_CRITICAL();
        TmrTbl[n].TmrEn = TRUE;
        portEXIT_CRITICAL();
    }
}



void TmrStop(INT32U n)
{

    if(n < TMR_MAX_TMR){
        portENTER_CRITICAL();
        TmrTbl[n].TmrEn = FALSE;
        portEXIT_CRITICAL();
    }
}



void TmrTask(void *data)
{
	
    TMR *ptmr;

    INT32U i;
    void(*pfnct)(void *);                       /* Ÿ�̸Ӱ� ����� �� ����� �Լ�                */
    void *parg;                                 /* ���� �Լ��� ���޵Ǵ� ����                     */

	data  = data;
    pfnct = (void (*)(void *))0;                /* �Լ� �����͸� �����Ѵ�                        */
    parg  = (void *)0;

	
	ptmr = &TmrTbl[0];
    for (i = 0; i < TMR_MAX_TMR; i++) {             /* ��� Ÿ�̸Ӹ� �����ϰ� ��Ȱ��ȭ�Ѵ�      */
        ptmr->TmrEn   = FALSE;
        ptmr->TmrCtr  = 0;
        ptmr->TmrInit = 0;
        ptmr->TmrFnct = NULL;
        ptmr++;
    }
    TmrTickCtr   = 0;

	while(DEF_TRUE){
		
		vTaskDelay(10);

		ptmr = &TmrTbl[0];                      /* Ÿ�̸� ���̺��� ���� �ּҷ� ����Ʈ�Ѵ�        */
		portENTER_CRITICAL();
		for (i = 0; i < TMR_MAX_TMR; i++) {
			if (ptmr->TmrEn == TRUE) {          /* Ÿ�̸Ӱ� Ȱ��ȭ���� ��츸 ī���͸� ����      */
				if (ptmr->TmrCtr > 0) {
					ptmr->TmrCtr--;
					if (ptmr->TmrCtr == 0) {            /* Ÿ�̸Ӱ� ����ƴ��� Ȯ��              */
						ptmr->TmrEn = FALSE;            /* ��, Ÿ�̸� ����                       */
						pfnct       = ptmr->TmrFnct;    /* ������ �Լ��� �� ����������...        */
						parg        = ptmr->TmrFnctArg; /* ... �����͸� ��´�                   */
					}
				}
			}
			if(pfnct != (void (*)(void *))0){    /* Ÿ�̸� ���� �Լ��� �����ؾ� �ϴ��� �����Ѵ�  */
				(*pfnct)(parg);                       
				pfnct = (void (*)(void *))0;
			}
			ptmr++;
		}
		portEXIT_CRITICAL();
	}
}




/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

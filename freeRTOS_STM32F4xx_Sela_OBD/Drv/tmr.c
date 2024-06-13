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
		ptmr->TmrFnct = fnct;              /* 타이머에 사용자 함수 포인터를 저장한다             */
		ptmr->TmrFnctArg = arg;            /* 사용자 함수에 대한 전달인자의 포인터를 저장한다    */
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
        val    = TmrTbl[n].TmrCtr;             /* 변환을 위해 타이머의 값을 복사한다            */
        portEXIT_CRITICAL();
        min    = (INT32U)(val / 60000);
        sec    = (INT32U)((val - min * 60000) / 1000);
        tenths = (INT32U)(val % 10);
        s[0]   = min / 10 + '0';               /* TIMER를 ASCII로 변환한다                      */
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
        ptmr->TmrCtr = ptmr->TmrInit;                       /* 카운터를 리로드 한다              */
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
    void(*pfnct)(void *);                       /* 타이머가 만료될 때 실행될 함수                */
    void *parg;                                 /* 위의 함수로 전달되는 인자                     */

	data  = data;
    pfnct = (void (*)(void *))0;                /* 함수 포인터를 리셋한다                        */
    parg  = (void *)0;

	
	ptmr = &TmrTbl[0];
    for (i = 0; i < TMR_MAX_TMR; i++) {             /* 모든 타이머를 리셋하고 비활성화한다      */
        ptmr->TmrEn   = FALSE;
        ptmr->TmrCtr  = 0;
        ptmr->TmrInit = 0;
        ptmr->TmrFnct = NULL;
        ptmr++;
    }
    TmrTickCtr   = 0;

	while(DEF_TRUE){
		
		vTaskDelay(10);

		ptmr = &TmrTbl[0];                      /* 타이머 테이블의 시작 주소로 포인트한다        */
		portENTER_CRITICAL();
		for (i = 0; i < TMR_MAX_TMR; i++) {
			if (ptmr->TmrEn == TRUE) {          /* 타이머가 활성화됐을 경우만 카운터를 감소      */
				if (ptmr->TmrCtr > 0) {
					ptmr->TmrCtr--;
					if (ptmr->TmrCtr == 0) {            /* 타이머가 만료됐는지 확인              */
						ptmr->TmrEn = FALSE;            /* 예, 타이머 정지                       */
						pfnct       = ptmr->TmrFnct;    /* 실행할 함수와 그 전달인자의...        */
						parg        = ptmr->TmrFnctArg; /* ... 포인터를 얻는다                   */
					}
				}
			}
			if(pfnct != (void (*)(void *))0){    /* 타이머 만료 함수를 실행해야 하는지 점검한다  */
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

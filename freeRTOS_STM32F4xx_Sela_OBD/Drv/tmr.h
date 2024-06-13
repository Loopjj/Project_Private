/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : tmr.h
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
*                                                 MODULE
*********************************************************************************************************
*/


#ifndef  TMR_H
#define  TMR_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   TMR_GLOBALS
#define  TMR_EXT
#else
#define  TMR_EXT  extern
#endif


/*
*********************************************************************************************************
*                                             PRIVATE DEFINE 
*********************************************************************************************************
*/


//#define TMR_DLY_TICKS       (OS_TICKS_PER_SEC / 100)	
//#define TMR_TASK_PRIO       TASK_TMR_PRIO	            
//#define TMR_TASK_STK_SIZE   128

#define TMR_MAX_TMR         TMR_MAX

//#define TMR_USE_SEM         0

#define TMRDLUL(B,O)        (B+O)
/*
*********************************************************************************************************
*                                             TYPE DEFINE 
*********************************************************************************************************
*/

typedef enum{
	TMR_FIRST = 1,
	TMR_DDD,
	TMR_LED,
	TMR_USBINTERVAL,
  TMR_DLINTERVAL,
	TMR_MAX
}tmr_num_t;

typedef struct tmr {                           /* 타이머 데이터 구조체                               */
    INT32U	TmrEn;                             /* 타이머가 활성화 되었는지를 알려주는 프래그         */
    INT32U	TmrCtr;                            /* 타이머의 현재 값 (카운트 다운)                     */
    INT32U	TmrInit;                           /* 타이머의 초기 값 (타이머가 세트될 때)              */
    void	(*TmrFnct)(void *);                /* 만료될 때 실행될 함수                              */
    void	*TmrFnctArg;                       /* 사용자 정의 함수로 전달되는 인자                   */
} TMR;


/*
*********************************************************************************************************
*                                            GLOBALE VARIABLE 
*********************************************************************************************************
*/

TMR_EXT TMR TmrTbl[TMR_MAX_TMR]; 


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/


TMR_EXT void TmrCfgFnct(INT32U n, void (*fnct)(void *), void *arg);
TMR_EXT INT32U TmrChk(INT32U n);
TMR_EXT void TmrFormat(INT32U n, char *s);
TMR_EXT void TmrInit(void);
TMR_EXT void TmrReset(INT32U n);
TMR_EXT void TmrSetMST(INT32U n, INT32U min, INT32U sec, INT32U tenths);
TMR_EXT void TmrSetT(INT32U n, INT32U tenths);
TMR_EXT void TmrSignalTmr(void);	// Must be.. Call by Timer ISR..
TMR_EXT void TmrStart(INT32U n);
TMR_EXT void TmrStop(INT32U n);
TMR_EXT void TmrTask(void *data);

#endif
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

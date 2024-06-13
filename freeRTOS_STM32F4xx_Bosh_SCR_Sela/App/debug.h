/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : debug.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 07/10/2014
*
* Description        : command line interface configuration
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef	DEBUG_H
#define	DEBUG_H


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef DEBUG_GLOBALS
#define DEBUG_EXT
#else
#define DEBUG_EXT extern
#endif


/*
*********************************************************************************************************
*                                                 DEFINE
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLE
*********************************************************************************************************
*/

DEBUG_EXT int ddd_on;



DEBUG_EXT void ddd_task(void);


DEBUG_EXT INT16U dbg_ddd_test(void);
DEBUG_EXT INT16U dbg_start_thread_test(void);
DEBUG_EXT INT16U dbg_end_thread_test(void);

DEBUG_EXT INT16U dbg_mafadc_test(void);



#define DEBUG_COMMANDS \
	{"ddd",   dbg_ddd_test, ALL_DIR, "ddd"},\
	{"st",   dbg_start_thread_test, ALL_DIR, "thread init"},\
	{"et",   dbg_end_thread_test, ALL_DIR, "thread stop"},\
	{"maf",   dbg_mafadc_test, ALL_DIR, "maf xxxx"},\




#endif

/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

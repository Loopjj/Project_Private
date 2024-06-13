/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : cli.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 07/05/2014
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

#ifndef	CLI_H
#define	CLI_H


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef CLI_GLOBALS
#define CLI_EXT
#else
#define CLI_EXT extern
#endif


/*
*********************************************************************************************************
*                                                 DEFINE
*********************************************************************************************************
*/

#define BS			0x08
#define LF			0x0A
#define CRT			0x0D

#define FATAL		1
#define OK_END		0
#define INV_PRM		202
#define INV_CMD		200
#define NO_DISPLAY	201

#define ALL_DIR		0xFF

#define P_ARG(X)	Argument(X)



/*
*********************************************************************************************************
*                                              TYPE DEFINE
*********************************************************************************************************
*/

typedef struct  COMMAND_T{
	char *cmd_name;
	INT16U (*cmd_func)(void);
	INT8U mode;
	char *cmd_help;
} command_t;



/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/


CLI_EXT void put_ch_console(INT8U data);
CLI_EXT void printd(char *fmt, ...);
CLI_EXT void printk(const char *fmt, ...);

CLI_EXT char *Argument(INT32U num);		
CLI_EXT INT32U CLI_GetArgCnt(void);
CLI_EXT void CLI_SetConsolPort(INT32U Port);
CLI_EXT char uart_console(void);
CLI_EXT void cmd_confirm_run(void);
CLI_EXT INT32U comparison_num( char *str );
CLI_EXT float comparison_float( char *str );
CLI_EXT INT16U current_fuction_list(void);
CLI_EXT void DisplayLogo(void);

CLI_EXT void Com3PutCharStr(int port,INT8U *data,int len);


CLI_EXT void Task_CLI(void * pvParameters);


#endif

/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

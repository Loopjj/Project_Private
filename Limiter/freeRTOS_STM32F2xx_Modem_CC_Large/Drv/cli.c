/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : cli.c
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
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define CLI_GLOBALS
#include "includes.h"



/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/
#define PRINTK_BUFF_SIZE		512
#define COMMAND_MAX				0x20				// ���ٿ� �����ִ� ���ڼ�.
#define COMMAND_STACK_MAX		0x04				// command history buffer
#define COMMAND_STACK_MAXMASK	(COMMAND_STACK_MAX-1)    
#define ARGUMENT_MAX			0x08				// argument limit


/*
*********************************************************************************************************
*                                              LOCAL MACRO
*********************************************************************************************************
*/

#define NB_OF(x)		(sizeof(x)/sizeof(x[0]))
#define COMMAND_CNT		NB_OF(commands)


static const char eLine[]={27,'[','2','K',0};

#if 0
static const char eNone[]={27,'[',MODE_NONE,'m',0};
static const char eBold[]={27,'[',MODE_BOLD,'m',0};
static const char eRevers[]={27,'[',MODE_REVERSED,'m',0};

static const char eYELLOW[]={27,'[',MODE_BOLD,'m',27,'[','3',COL_YELLOW,'m',0};
static const char eGREEN[]={27,'[',MODE_BOLD,'m',27,'[','3',COL_GREEN,'m',0};
static const char eRED[]={27,'[',MODE_BOLD,'m',27,'[','3',COL_RED,'m',0};
static const char eMAGENTA[]={27,'[',MODE_BOLD,'m',27,'[','3',COL_MAGENTA,'m',0};
#endif


const command_t commands[]={
	{"ls", current_fuction_list, ALL_DIR, ""},
	{"help", current_fuction_list, ALL_DIR, "HELP list"},
	BSP_COMMANDS 
	SPIFLASH_DRV_COMMANDS
	BSP_IOCCOMMAND
	RTC_COMMANDS
	BSPU_COMMANDS
	DEBUG_COMMANDS
	NAND_COMMANDS
  I2C_COMMANDS
	//CAN_COMMANDS
  INIT_COMMANDS  
	{0,0,0,0}
};


/*
*********************************************************************************************************
*                                             LOCAL VARIABLE
*********************************************************************************************************
*/

static const INT8U fls[] = "\n=============================================start\n";
static const INT8U fle[] = "\n=============================================end\n";
static char prompt_buffer[30]={0};
static INT8U command_stack[COMMAND_STACK_MAX][COMMAND_MAX]={0};
static INT32U cmd_cnt=0;
static INT8U current_mode=1;		
static INT32U Console_Port;


static char dprint_buf[PRINTK_BUFF_SIZE]={0};


static char kprint_buf[PRINTK_BUFF_SIZE]={0};



xSemaphoreHandle PromptSem = NULL;
xSemaphoreHandle PrintkSem = NULL;

extern USART_TypeDef *pUART_BASE[2];

/*
*********************************************************************************************************
*                                             LOCAL FUNCTION
*********************************************************************************************************
*/


void CLI_SetConsolPort(INT32U Port)
{
	Console_Port = Port;
	memset(dprint_buf, 0x0, PRINTK_BUFF_SIZE);
}

void dput_ch_console(INT8U data)
{
	while(USART_GetFlagStatus(pUART_BASE[Console_Port],USART_FLAG_TXE) == RESET);// OS_Sched();
	USART_SendData(pUART_BASE[Console_Port],data);
}

void put_ch_console(INT8U data)
{

    dput_ch_console(data);
}


void printd(char *fmt, ...) 
{
	INT32S	len,i=0;
	va_list ap;
 
 	va_start(ap, fmt);
 	len = vsprintf(dprint_buf, fmt, ap);
	va_end(ap);

	if ( len > 0 ){
		while(len--){
			if ( dprint_buf[i] == '\n' ){
				dput_ch_console('\n');
				dput_ch_console('\r');
			}
			else {																   
				dput_ch_console(dprint_buf[i]);
			}
			i++;
		}
	}
}



void printk(const char *fmt, ...) 
{

    INT32S	len,i=0;
	va_list ap;
    
	if( PrintkSem != NULL)
	{
		if(xSemaphoreTake(PrintkSem, ( portTickType ) 10 ) == pdTRUE )
		{
 			va_start(ap, fmt);
 			len = vsprintf(kprint_buf, fmt, ap);
			va_end(ap);

			if ( len > 0 )
			{
				while(len--){
					if ( kprint_buf[i] == '\n' ){
						put_ch_console('\n');
						put_ch_console('\r');
					}
					else {																   
						put_ch_console(kprint_buf[i]);
					}
					i++;
				}
			}
			xSemaphoreGive(PrintkSem);
		}
	}
}

void UART2_SendData(INT8U data)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);// OS_Sched();
	USART_SendData(USART2,data);
}

void UART2_SendDataStr(INT8U *data,int len)
{
	int i=0;
	for(i=0;i< len; i++)
		UART2_SendData(data[i]);
}

void UART2_print(char *fmt, ...) 
{
	INT32S	len,i=0;
	va_list ap;
 
 	va_start(ap, fmt);
 	len = vsprintf(dprint_buf, fmt, ap);
	va_end(ap);

	if ( len > 0 ){
		while(len--){
			if ( dprint_buf[i] == '\n' ){
				UART2_SendData('\n');
				UART2_SendData('\r');
			}
			else {																   
				UART2_SendData(dprint_buf[i]);
			}
			i++;
		}
	}
}

// �����Էµ� Ŀ����� argument�� �ּҸ� �����Ѵ�. 
static char *argument[ARGUMENT_MAX] = {0};
char *Argument(INT32U num)		
{
	return argument[num];
}


static INT32U ulArgumentCount=0;
INT32U	CLI_GetArgCnt(void)
{
	return ulArgumentCount;
}


// 0x�� �����س���. 10������ 16���� ����..
INT32U comparison_num( char *str )
{
	INT32U temp=0;
	if ( str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) sscanf((const char *)str,"0x%x", &temp);
	else sscanf((const char *)str,"%d", &temp);
	return temp;
}
// 0x�� �����س���. 10������ 16���� ����..
float comparison_float( char *str )
{
	char buf[50];
	float temp=0;

	memset(buf,0,sizeof(buf));
	memcpy(buf, str, strlen(str));
	sscanf(   buf, "%f", &temp);

	printk("in cli = %s[%.8f]\n",str,temp);
	return temp;
}



void prompt(char *name)
{
	if(PromptSem != NULL)
	{
		if(xSemaphoreTake(PromptSem, ( portTickType ) 10 )  == pdTRUE)
		{
			printk("OBDPro]");
			Term_Set_Display_Attribute_Mode( MODE_NONE );
			xSemaphoreGive(PromptSem);
		}
	}
}


void DisplayLogo(void)
{
	memcpy((char *)prompt_buffer,(char *)PROMPT_NAME,sizeof(strlen(PROMPT_NAME)));
	printd("\nloop %s V%d.%d %s\nBuild: %s %s\n\n",PRODUCT_TYPE,VERSION_NUM/10,VERSION_NUM%10,HW_NAME ,__DATE__, __TIME__);
	printd("OBDPro]");
}


void MsgCommand(int ret)
{
	switch(ret){
	case FATAL:
		printk("\n\n Fatal error check system .. \n\n");
		break;
	case OK_END:
		printk("\n\n Ok..  \n\n");
		break;
	case INV_PRM:
		printk("\n\n Invalid parameter \n\n");
		break;
	case INV_CMD:
		printk("\n\n Command not found.. \n\n");
		break;
	case NO_DISPLAY:
		break;
	
	}
}


// ���ڿ��� �޾Ƽ� ���ɸ���Ʈ���� ã�Ƴ��� �Լ���ȣ�� ��ȯ�Ѵ�. ���ٸ� �������� ��ȯ.
int find_command(char *cmd)
{
	int i,ret=-1;

	for(i=0;i<COMMAND_CNT-1;i++)
	{
		ret = (INT16S)strcmp(commands[i].cmd_name, cmd);	 
	
		if (!ret)
			return i;
	}
	return -1;
}


// ������ �м����� �ش� �Լ��� �����Ų��.
void cmd_confirm_run(void)		
{
	int cmd_number = -1,ret = -1;

		cmd_number = find_command( argument[0] );
		
		if ( cmd_number < 0 ) {
			ret = INV_CMD;
		}
		else if ( commands[cmd_number].mode & current_mode){// ������ ����ɼ��ִ� ����ΰ� Ȯ���ϰ� �׸��ɽ���. (���丮)
            if ( commands[cmd_number].cmd_func != NULL ){
                ret=(*(commands[cmd_number].cmd_func))();
            }
		}
		else ret = INV_CMD;

	MsgCommand(ret);
}


// �Լ� ����Ʈ�� ����Ѵ�.
INT16U current_fuction_list(void)
{
	INT16U i;

	printk((char *)fls);

	for (i=0;i<COMMAND_CNT;i++){
		if ( commands[i].mode & current_mode){
			printk("\t");
			printk((char *)commands[i].cmd_name);
						
			if ( commands[i].cmd_help != 0 ){
				printk("\t:\t");
				printk((char *)commands[i].cmd_help);
			}
			printk("\n");
		}
	}
	printk((char *)fle);
	
	return NO_DISPLAY;
}


// �Է¹��� �� ���� ���ɰ� argument 1,2,3... ���� ������. argument count�� ����.
INT32U parsing_command( char *command )
{
	INT32U i=0,j=0;
	INT32U GetArgCount = 0;

	argument[j++]=&command[0];	// command's address save
											
	/* for command stack */
	memcpy(command_stack[cmd_cnt++],command,COMMAND_MAX);
	cmd_cnt &= COMMAND_STACK_MAXMASK;

	/* argument devide */
	while( command[i++] != '\0' ){
		if ( (command[i]==' ') && (command[i+1]!=' ') && (command[i+1]!=0) ){
			GetArgCount ++;		// arg counting..
			argument[j++] = &command[i+1];
		}
	}


	for(i=1;i<j;i++)
		*(argument[i]-1) = '\0' ;	//Ending charactor for printk display of each argument.
	
	return GetArgCount;

}


static char command_line[COMMAND_MAX]={0};
char uart_console(void)	// must be in while loop!
{
	static INT32U line_ptr=0;
	static INT8U esc_mode=0;
	
	INT8U err;
	char data=0;

	err = FALSE;

	while( 1 ){

		data = CommGetChar(Console_Port,10, &err);

		if ( err == COMM_NO_ERR ){

			switch (data){

			case 0x1b:
				esc_mode++;
				break;
			case '[':
				if (esc_mode) 
					esc_mode++;
				break;

			case '\r':  // Enter Key
			case '\n':
				if (line_ptr){
					command_line[line_ptr]='\0';
					line_ptr=0;
					ulArgumentCount = parsing_command(command_line);
					
					cmd_confirm_run();
				}

				//printk("\n");
				put_ch_console('\r');
				put_ch_console('\n');
				
				prompt(prompt_buffer);

				esc_mode = 0;
				break;

			case BS: // Back Space
				if (line_ptr){
					put_ch_console(BS);
					put_ch_console(' ');
					put_ch_console(BS);
					line_ptr--;
				}
				esc_mode = 0;
				break;

			default:
				if ( line_ptr >= COMMAND_MAX ){	// ������ �� ���ۿ뷮���� ����Ǹ� ���ڸ������Ѵ�.
					line_ptr--;
					put_ch_console(BS);
					put_ch_console(data);				
					command_line[line_ptr++]=data;
				}
				else {
					
					if ( esc_mode ) { // ǥ�� �ƽ�Ű���̾ƴ� Ȯ��ƽ�Ű�ڵ� �� Ű������ ȭ��ǥ���� Ư��Ű���������� 
						switch(data){
							
						case 'A':		// Up Key
							if(command_stack[((cmd_cnt-1)&COMMAND_STACK_MAXMASK)][0]!=0){

								// ���� �� ���� ����� 
								printk("%s\r",eLine);

								// ������Ʈ �Ѹ��� 
								prompt(prompt_buffer);

								// �׵��� �����ص״��� �ϳ� �ڿ��� �ѷ��ش�.
								cmd_cnt--;
								cmd_cnt &= COMMAND_STACK_MAXMASK;
								printk( (char *)command_stack[ cmd_cnt ]);

								// �ѷ��ֱ⸸�ϸ�ȵǰ� ���ݱ��� ����� �Է��߾��ٰ� �ؾ��Ѵ�. 
								memcpy(command_line ,command_stack[cmd_cnt],COMMAND_MAX);

								// ���������͵� �̵����ѳ��� 
								line_ptr = strlen((char *)command_stack[cmd_cnt]);

								// �̷������ָ� ���� ����ú���� ���ɾ �ٽ� ģ ���� �ȴ�. 
							}
							break;
							
						case 'B':		// Down Key
							if(command_stack[((cmd_cnt+1)&COMMAND_STACK_MAXMASK)][0]!=0){
								printk("%s\r",eLine);
								prompt(prompt_buffer);
								cmd_cnt++;
								cmd_cnt&=COMMAND_STACK_MAXMASK;
								printk((char *)command_stack[cmd_cnt]);
								memcpy(command_line,command_stack[cmd_cnt],COMMAND_MAX);
								line_ptr = strlen((const char *)command_stack[cmd_cnt]);
							}

							break;

						case 'C':		// Left Key
							break;
							
						case 'D':		// Right Key
							break;
						}
					}
					else {
						put_ch_console(data);
						command_line[line_ptr++]=data;
					}
				}		
				esc_mode = 0;
				break;
			}
		}
		else
			break;
	}
	return 0;
}


//********************************************************************************************* CLI Task

void Task_CLI(void * pvParameters)
{
	sysinfo_t *s = (sysinfo_t *)pvParameters 	;

	PromptSem = xSemaphoreCreateMutex();
	PrintkSem = xSemaphoreCreateMutex();
	BSP_UART1RX_IntEnable(ENABLE);
	for(;;)
	{
		s->twatchCliFlag = 1;
		//uart_console();
		//ddd_task();
		vTaskDelay(10);
		
		s->twatchCliFlag = 0;
	}
}




/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/
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
#define COMMAND_MAX				  0x20				// 한줄에 쓸수있는 글자수.
#define COMMAND_STACK_MAX		0x04				// command history buffer
#define COMMAND_STACK_MAXMASK	(COMMAND_STACK_MAX-1)    
#define ARGUMENT_MAX			  0x08				// argument limit

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
	//BSP_COMMANDS
  GPS_COMMANDS
  NAND_COMMANDS
	RTC_COMMANDS
	//BSPU_COMMANDS
	//DEBUG_COMMANDS
  I2C_COMMANDS
  //CAN_COMMANDS
  //INIT_COMMANDS  
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
static char dprint_buf[PRINTK_BUFF_SIZE]={0};
static char kprint_buf[PRINTK_BUFF_SIZE]={0};

osSemaphoreDef(sem_0);
osSemaphoreDef(sem_1);
osSemaphoreId PromptSem = NULL;
osSemaphoreId PrintkSem = NULL;

/*
*********************************************************************************************************
*                                             LOCAL FUNCTION
*********************************************************************************************************
*/

void dput_ch_console(INT8U data)
{
  if(CMPortEnable)   
    UART_SendData(DEBUG_UART_PORT, data);
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
    
	if(PrintkSem != NULL) { 
    if(osSemaphoreWait(PrintkSem, 100) == osOK)
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
            UART_SendData(DEBUG_UART_PORT, kprint_buf[i]);  //put_ch_console(kprint_buf[i]);
          }
          i++;
        }
      }
      osSemaphoreRelease(PrintkSem);
    }
  }
}


// 현재입력된 커멘드의 argument의 주소를 리턴한다. 
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


// 0x를 구별해낸다. 10진수와 16진수 감별..
INT32U comparison_num( char *str )
{
	INT32U temp=0;
	if ( str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) sscanf((const char *)str,"0x%x", &temp);
	else sscanf((const char *)str,"%d", &temp);
	return temp;
}

// 0x를 구별해낸다. 10진수와 16진수 감별..
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
		if(osSemaphoreWait(PromptSem, 100)  == osOK)
		{
			printk("DTG>");
			Term_Set_Display_Attribute_Mode( MODE_NONE );
			osSemaphoreRelease(PromptSem);
		}
	}
}


void DisplayLogo(void)
{
	memcpy((char *)prompt_buffer,(char *)PROMPT_NAME,sizeof(strlen(PROMPT_NAME)));
	printd("\nGPS Tracker %s V%d.%d %s\nBuild: %s %s\n\n",PRODUCT_TYPE,VERSION_NUM/10,VERSION_NUM%10,HW_NAME ,__DATE__, __TIME__);
	printd("GPSPro>");
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

// 문자열을 받아서 명령리스트에서 찾아내어 함수번호를 반환한다. 없다면 에러문자 반환.
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

// 명령을 분석한후 해당 함수를 실행시킨다.
void cmd_confirm_run(void)		
{
	int cmd_number = -1,ret = -1;

		cmd_number = find_command( argument[0] );
		
		if ( cmd_number < 0 ) {
			ret = INV_CMD;
		}
		else if ( commands[cmd_number].mode & current_mode){// 명령이 실행될수있는 모드인가 확인하고 그명령실행. (디랙토리)
            if ( commands[cmd_number].cmd_func != NULL ){
                ret=(*(commands[cmd_number].cmd_func))();
            }
		}
		else ret = INV_CMD;

	MsgCommand(ret);
}

// 함수 리스트를 출력한다.
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

// 입력받은 한 줄을 명령과 argument 1,2,3... 으로 구별함. argument count로 리턴.
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
	INT16U tmptail;
	char data=0;
  
	while (CLI_RxHead != CLI_RxTail ) { 
    tmptail = (CLI_RxTail + 1) & CLI_RX_BUFFER_MASK;/* Calculate buffer index */
    
    CLI_RxTail = tmptail;                /* Store new index */
    
    data = CLI_RxBuf[tmptail];           /* Return data */    
 
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
      if ( line_ptr >= COMMAND_MAX ){	// 한줄이 길어서 버퍼용량까지 진행되면 제자리걸음한다.
        line_ptr--;
        put_ch_console(BS);
        put_ch_console(data);				
        command_line[line_ptr++]=data;
      }
      else {
        
        if ( esc_mode ) { // 표준 아스키값이아닌 확장아스키코드 즉 키보드의 화살표같은 특수키가들어왔을때 
          switch(data){
            
          case 'A':		// Up Key
            if(command_stack[((cmd_cnt-1)&COMMAND_STACK_MAXMASK)][0]!=0){

              // 지금 그 라인 지우고 
              printk("%s\r",eLine);

              // 프롬프트 뿌리고 
              prompt(prompt_buffer);

              // 그동안 저장해뒀던거 하나 뒤엣것 뿌려준다.
              cmd_cnt--;
              cmd_cnt &= COMMAND_STACK_MAXMASK;
              printk( (char *)command_stack[ cmd_cnt ]);

              // 뿌려주기만하면안되고 지금까지 사람이 입력했었다고 해야한다. 
              memcpy(command_line ,command_stack[cmd_cnt],COMMAND_MAX);

              // 라인포인터도 이동시켜놓고 
              line_ptr = strlen((char *)command_stack[cmd_cnt]);

              // 이렇게해주면 내가 전에첬었던 명령어를 다시 친 격이 된다. 
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
	return 0;
}

void CLI_Init(void)
{
	PromptSem = osSemaphoreCreate(osSemaphore(sem_0) , 1);
	PrintkSem = osSemaphoreCreate(osSemaphore(sem_1) , 1);
}

//********************************************************************************************* CLI Task
/* Task_CLI function */

void Task_CLI(void const * argument)
{
	sysinfo_t *s = (sysinfo_t *)argument 	;

	for(;;)
	{
		s->twatchCliFlag = 1;
    
#ifdef CLI_CONSOL_VIEW
    uart_console();
    ddd_task();
#else
    if(Setting.FW_Type == EXT_MODEM ||  BLE_SmartFlag) {      
      ModemTool_Process();
      if(DTG_IntFlag) {
        if(ReceiveMediaFlag == BLESCANNER)
          BLE_time_check();
        else 
          RFID_time_check();
        if(DTG_SendFlag && !BLE_SmartFlag)
          DTG_SecData_Send();
        DTG_IntFlag = 0;
      }
    }
#endif      
		s->twatchCliFlag = 0;
    osDelay(1);
	}
}


/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/

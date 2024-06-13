/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : debug.c
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
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define DEBUG_GLOBALS
#include "includes.h"


static void ddd_View(void)
{
	sysinfo_t *s = &_sysinfo 	;
	
	if(TmrChk(TMR_DDD) != 0) return;
	TmrSetT(TMR_DDD, 300);
	TmrStart(TMR_DDD);

	Term_Save_Cursor_Position();
	Term_Set_Cursor_Position(0,0);

	Term_Erase_Line();
	printk("================================ System Information ================================\n");
	Term_Erase_Line();
	printk("\n");
	Term_Erase_Line();
	Term_Set_Display_Attribute_Mode(MODE_BOLD);
	Term_Set_Display_Colour(COL_FOREGROUND,COL_YELLOW);	
	printk("<< ADC  >>\n");
	Term_Erase_Line();
	printk("   MAF,      T1,     T2,     P1,  UREA_T, UREA_L, BAT,   TEMP,    Speed,    NOx(in),  NOx(out)\n");
	Term_Erase_Line();
	printk("  %04d,     %04d,   %04d,    %04d,  %04d,   %04d   %04d,  %04d          \n",
		adcdata[ADC_AVR_MAF], adcdata[ADC_AVR_T1], adcdata[ADC_AVR_T2],
		adcdata[ADC_AVR_P1], adcdata[ADC_AVR_UREAT], adcdata[ADC_AVR_UREAL],
		adcdata[ADC_AVR_BAT], adcdata[ADC_AVR_TMP36_TEMP]);
	Term_Erase_Line();
	printk("\n");
	Term_Erase_Line();
	Term_Set_Display_Attribute_Mode(MODE_BOLD);
	Term_Set_Display_Colour(COL_FOREGROUND,COL_GREEN);
	Term_Erase_Line();
	printk("MAF to Voltage = %.2f,offset = %.2f\n",(double)(0.00080566*adcdata[ADC_AVR_MAF]) *2.0 + MafvoltageOffset(adcdata[ADC_AVR_MAF]),MafvoltageOffset(adcdata[ADC_AVR_MAF]));
#if 0
	if(adcdata[ADC_AVR_MAF] <1000)
		printk("MAF to Voltage = %.2f\n",(double)((5.0/4096.0)*adcdata[ADC_AVR_MAF]) *2.0 -0.5);
	else if(adcdata[ADC_AVR_MAF] >1000 && adcdata[ADC_AVR_MAF] <= 1200 )
		printk("MAF to Voltage = %.2f\n",(double)((5.0/4096.0)*adcdata[ADC_AVR_MAF]) *2.0 -0.9);
	else if(adcdata[ADC_AVR_MAF] >1200 && adcdata[ADC_AVR_MAF] <= 1700 )
		printk("MAF to Voltage = %.2f\n",(double)((5.0/4096.0)*adcdata[ADC_AVR_MAF]) *2.0 -0.7);
	else if(adcdata[ADC_AVR_MAF] >1700 )
		printk("MAF to Voltage = %.2f\n",(double)((5.0/4096.0)*adcdata[ADC_AVR_MAF]) *2.0 -1.4);
#endif

	Term_Erase_Line();
	printk("<<Sensor>>\n");
	Term_Erase_Line();
	printk(" %04dkg/h,  %+-d'C, %+-d'C,  %02dBar, %04d,   %04d,   %.1f,  %+-d'C ,  [%03d],   %+-04dppm,  %+-04dppm\n",
		SysSts.MafKg_H,
		SysSts.T1_temp,
		SysSts.T2_temp,
		SysSts.P1_bar,
		adcdata[ADC_AVR_UREAT],
		SysSts.TankLevelP,
		SysSts.BattVoltage/100.0,
		s->Scrst->Temperature,
		SysSts.Noxppm[0],
		SysSts.Noxppm[1]);
	Term_Erase_Line();
	printk("\n");
	Term_Erase_Line();

	Term_Set_Display_Attribute_Mode(MODE_NONE);
	Term_Erase_Line();

	printk("DosingPeriodBackup = [%d],DosingControlPeriod = [%d],supply.ContrlSet = [%d]\n",s->DosingPeriodBackup,s->DosingControlPeriod,s->Scrst->supply.ContrlSet);
	Term_Erase_Line();
  printk("\n");
	Term_Erase_Line();

	Term_Erase_Line();
	printk("====================================================================================\n");
	Term_Erase_Line();

	Term_Restore_Cursor_Position();
}



void ddd_task(void)
{
	if ( ddd_on ){
		ddd_View();
	}
}

INT16U dbg_ddd_test(void)
{
	if ( ddd_on )
		ddd_on = 0;
	else
		ddd_on = 1;

	return NO_DISPLAY;
}

#define NAGENTS_PRIO	 ( tskIDLE_PRIORITY + 8 )
xTaskHandle			xCreatedClientTask	;

void process_echo_request(void *sd)
{

	while(1)
	{
		printk("sys thread new.....\n");
		vTaskDelay(1000);
	}
}

INT16U dbg_start_thread_test(void)
{
	 xCreatedClientTask = sys_thread_new(
					"test", 
					process_echo_request, 
					NULL, 
					DEFAULT_THREAD_STACKSIZE, 
					NAGENTS_PRIO);

	return NO_DISPLAY;
}
INT16U dbg_end_thread_test(void)
{
	vTaskDelete(xCreatedClientTask);
	printk("delete thread task....\n");

	return NO_DISPLAY;
}

INT16U dbg_mafadc_test(void)
{
	INT16U adc=0;
	INT16U ret = 0;
	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	// 중요한 액션이므로 안전 장치...

	adc = comparison_num((char*)P_ARG(1));
	
	ret =  MassAirFlowSensor();
	printk("\ninput = [%d] Maf ret value....%d\n",adc,ret);

	return NO_DISPLAY;
}

#if 0
struct fs_file *file;
INT16U dbg_fsopen_test(void)
{
	unsigned char recv_buffer[1500];

	if (strncmp((char *)recv_buffer,"GET /1.txt",10)==0)
	{
		file = (struct fs_file *)fs_open("/1.txt");
		fs_read(file, recv_buffer, 10);
		printk("[%s]\n",recv_buffer);
	}
	else{
		printk("file open fail....\n");
        }

	return NO_DISPLAY;
}

INT16U dbg_fsclose_test(void)
{

	if(file) fs_close(file);
	
	return NO_DISPLAY;
}
#endif




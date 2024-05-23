/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : debug.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
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

/*
********************************************************************************
* Description : ddd_View
* Arguments   : None
* Return      : 
* Note        : real time data view
******************************************************************************** 
*/
static void ddd_View(void)
{
	//sysinfo_t *s = &_sysinfo;
  //INT16U Bat;
  
  //Bat = BatteryVoltage();
  TMR_DDD++;
	if(TMR_DDD > 300) {
    TMR_DDD = 0;

    Term_Save_Cursor_Position();
    Term_Set_Cursor_Position(0,0);

    Term_Set_Display_Attribute_Mode(MODE_BOLD);
    Term_Set_Display_Colour(COL_FOREGROUND,COL_GREEN);
    Term_Erase_Line();
    printk("================================ System Information ================================\n");
    Term_Erase_Line();
    printk("\n");
    Term_Erase_Line();
    Term_Set_Display_Attribute_Mode(MODE_BOLD);
    Term_Set_Display_Colour(COL_FOREGROUND,COL_YELLOW);	
    printk("<< Time\t\t\tBattery\tSendCount\tGsensor >>\n");
    Term_Erase_Line();
  //	printk("   %02d/%02d/%02d %02d:%02d:%02d\t%02d.%02dV\t%04d\t\tx=%d y=%d z=%d\n", \
      s->tm_ptr.tm_year,  s->tm_ptr.tm_mon,  s->tm_ptr.tm_mday,  s->tm_ptr.tm_hour,  \
      s->tm_ptr.tm_min,  s->tm_ptr.tm_sec, (Bat/100), (Bat%100), SaveTimeCount, GS_Val[0], GS_Val[1], GS_Val[2]);  
  //  Term_Erase_Line();    
  //	printk("<< Data Address  >>\n");
  //	Term_Erase_Line();
  //	printk("   DataAddr = 0x%04x,\tSentAddr = 0x%04x\n",ExtFram.DataAddr, ExtFram.SentAddr);  
  //  Term_Erase_Line();    
    printk("\n");
    Term_Erase_Line();
    Term_Set_Display_Attribute_Mode(MODE_BOLD);
    Term_Set_Display_Colour(COL_FOREGROUND,COL_GREEN);
    printk("====================================================================================\n");
    Term_Erase_Line();
    Term_Set_Display_Attribute_Mode(MODE_NONE);

    Term_Restore_Cursor_Position();
  }
}


/*
********************************************************************************
* Description : ddd_task
* Arguments   : None
* Return      : 
* Note        :
******************************************************************************** 
*/
void ddd_task(void)
{
	if ( ddd_on ){
		ddd_View();
	}
}

/*
********************************************************************************
* Description : dbg_ddd_test
* Arguments   : None
* Return      : 
* Note        :
******************************************************************************** 
*/
INT16U dbg_ddd_test(void)
{
	if ( ddd_on )
		ddd_on = 0;
	else
		ddd_on = 1;

	return NO_DISPLAY;
}

/*
********************************************************************************
* Description : dbg_ddd_test
* Arguments   : None
* Return      : 
* Note        :
******************************************************************************** 
*/
INT16U dbg_debug_enable(void)
{
	if (debug_on)
		debug_on = 0;
	else
		debug_on = 1;

	return NO_DISPLAY;
}


/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/


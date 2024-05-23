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
//#include "terminaldrv.h"

static void ddd_View(void)
{
	sysinfo_t *s = &_sysinfo;
  
	if(TmrChk(TMR_DDD) != 0) return;
	TmrSetT(TMR_DDD, 300);
	TmrStart(TMR_DDD);

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
	printk("<< Time            \tSaveTimeCount>>                                                 \n");
	Term_Erase_Line();
	printk("   %02d/%02d/%02d %02d:%02d:%02d\t %d\n", s->tm_ptr.tm_year,  s->tm_ptr.tm_mon,  s->tm_ptr.tm_mday,  s->tm_ptr.tm_hour,  s->tm_ptr.tm_min,  s->tm_ptr.tm_sec, SaveTimeCount);  
  printk("<< Data Address  >>                                                                  \n");
	Term_Erase_Line();   
	printk("\n");
	Term_Erase_Line();
	Term_Set_Display_Attribute_Mode(MODE_BOLD);
	Term_Set_Display_Colour(COL_FOREGROUND,COL_GREEN);
	printk("====================================================================================\n");
	Term_Erase_Line();
	Term_Set_Display_Attribute_Mode(MODE_NONE);

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


FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("0:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}


INT16U dbg_fsmount_test(void)
{
	uint32_t free, total;
    char buffer[50];

	printk("sd card mount and file write.....\n");
	if (f_mount(&SD_Fs, "0:", 1) == FR_OK) 
	{
		/* Mounted ok */
		/* Write only once */
		/* Try to open SD card file */
		if (f_open(&SD_Fil, "0:sd_file.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS) == FR_OK)
		{       
			/* We want to write only once */
			/* Get total and free space on SD card */
			TM_FATFS_DriveSize(&total, &free);

			/* Put data */
			f_puts("This is my first file with SD card and FatFs\n", &SD_Fil);
			f_puts("with SD card library from stm32f4-discovery.com\n", &SD_Fil);
			f_puts("----------------------------------------------------\n", &SD_Fil);
			f_puts("SD card total and free space:\n\n", &SD_Fil);
			/* Total space */
			sprintf(buffer, "Total: %8u kB; %5u MB; %2u GB\n", total, total / 1024, total / 1048576); 
			f_puts(buffer, &SD_Fil);
			/* Free space */
			sprintf(buffer, "Free:  %8u kB; %5u MB; %2u GB\n", free, free / 1024, free / 1048576);
			f_puts(buffer, &SD_Fil);
			f_puts("----------------------------------------------------\n", &SD_Fil);
			/* Close SD card file */
			f_close(&SD_Fil);  
		}
		f_mount(0, "0:", 1);
	}

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




/**	 
 * |---------------------------------------------------------------------- 
 * | Copyright (C) Tilen Majerle, 2014 
 * |  
 * | This program is free software: you can redistribute it and/or modify 
 * | it under the terms of the GNU General Public License as published by 
 * | the Free Software Foundation, either version 3 of the License, or 
 * | any later version. 
 * |   
 * | This program is distributed in the hope that it will be useful, 
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * | GNU General Public License for more details. 
 * |  
 * | You should have received a copy of the GNU General Public License 
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 * |---------------------------------------------------------------------- 
 */ 
#include "includes.h"


/* Private variables */ 
USB_OTG_CORE_HANDLE     USB_OTG_Core; 
USBH_HOST               USB_Host; 
uint8_t                 TM_USB_MSCHOST_INT_Initialized = 0; 
TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_INT_Result; 


TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_Init() { 
	/* Set default */ 
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Disconnected; 
	/* Init Host Library */ 
	USBH_Init(	&USB_OTG_Core, 
			#ifdef USE_USB_OTG_FS 
				USB_OTG_FS_CORE_ID, 
			#else 
				USB_OTG_HS_CORE_ID, 
			#endif 
				&USB_Host, 
				&USBH_MSC_cb,  
				&USR_Callbacks); 
	/* Process */ 
	TM_USB_MSCHOST_Process(); 
	/* Initialized */ 
	TM_USB_MSCHOST_INT_Initialized = 1; 
	/* Is connected already? */ 
	return TM_USB_MSCHOST_Device(); 
} 


void TM_USB_MSCHOST_Process(void) { 
	/* if library is initialized */ 
	if (TM_USB_MSCHOST_INT_Initialized) { 
		/* Process */ 
		USBH_Process(&USB_OTG_Core, &USB_Host); 
	} 
} 


TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_Device(void) { 
	/* Check if library is initialized */ 
	if (!TM_USB_MSCHOST_INT_Initialized) { 
		return TM_USB_MSCHOST_Result_LibraryNotInitialized; 
	} 
	/* Return status, handled by USB library */ 
	return TM_USB_MSCHOST_INT_Result; 
} 

FRESULT TM_FATFS_USBDriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("1:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}


//********************************************************************************************* USB Task

void Task_USB(void * pvParameters)
{
	static uint8_t usb_write = 1;
	uint32_t free, total;
        char buffer[50];

	sysinfo_t *s = (sysinfo_t *)pvParameters 	;

	TM_USB_MSCHOST_Init();

	for(;;)
	{
		s->twatchUSBFlag = 1;
		TM_USB_MSCHOST_Process();
		if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected)
		{
			if (usb_write)
			{
				if (f_mount(&USB_Fs, "1:", 1) == FR_OK)
				{
					if (f_open(&USB_Fil, "1:usb_file.txt", FA_READ | FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
					{
						TM_FATFS_USBDriveSize(&total, &free);

						/* Put data */                                                                           
						f_puts("This is my first file with USB and FatFS\n", &USB_Fil);                          
						f_puts("with USB MSC HOST library from stm32f4-discovery.com\n", &USB_Fil);              
						f_puts("----------------------------------------------------\n", &USB_Fil);              
						f_puts("USB total and free space:\n\n", &USB_Fil);                                       
						/* Total space */                                                                        
						sprintf(buffer, "Total: %8u kB; %5u MB; %2u GB\n", total, total / 1024, total / 1048576);
						f_puts(buffer, &USB_Fil);                                                                
						/* Free space */                                                                         
						sprintf(buffer, "Free:  %8u kB; %5u MB; %2u GB\n", free, free / 1024, free / 1048576);   
						f_puts(buffer, &USB_Fil);                                                                
						f_puts("----------------------------------------------------\n", &USB_Fil);              
						/* Close USB file */                                                                     
						f_close(&USB_Fil);                                                                       

						usb_write = 0;
					}
				}
				f_mount(0, "1:", 1); 
			}
		}
		vTaskDelay(1);
		s->twatchUSBFlag = 0;
	}
}
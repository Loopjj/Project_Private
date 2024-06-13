/*
********************************************************************************	
*
*              (c) Copyright 2014, loopsystem
*                      All rights reserved
*
* Filename    : drivefree.c
* Description : total, free size
********************************************************************************
*/
#include <string.h>
#include "includes.h"
//#define __DEBUG

FILINFO Finfo;
FATFS *fs;
DWORD fre_clust, fre_sect, tot_sect;
INT8U firstfile = 0;

/*
********************************************************************************
* Description : scan files
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
FRESULT scan_files (
	char* path		/* Pointer to the working buffer with start path */
)
{
	DIR dirs;
	FRESULT res;
	INT32U totalsize, freesize;
	INT32U ctime, mtime;

#if _USE_LFN
   static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
   Finfo.lfname = lfn;
   Finfo.lfsize = sizeof(lfn);
#endif

	/* Get volume information and free clusters of drive 0 */
  res = f_getfree("", &fre_clust, &fs);
  /* Get total sectors and free sectors */
  tot_sect = (fs->n_fatent - 2) * fs->csize;
  fre_sect = fre_clust * fs->csize;	
	
	totalsize = (tot_sect / 2);
	freesize = (fre_sect / 2);
  
	res = f_opendir(&dirs, path);
	if (res == FR_OK) {
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (_FS_RPATH && Finfo.fname[0] == '.') continue;
			if (!(Finfo.fattrib & AM_DIR)) {
				ctime = Finfo.fdate;
				ctime = (ctime<<16 | Finfo.ftime);
				if(!firstfile) {
					mtime = ctime;
					firstfile = 1;
				}
				else {
					if(ctime < mtime) {
						mtime = ctime;
					}
				}
#ifdef __DEBUG
				xprintf(PSTR("\rctime = %lu.\n"), ctime);
#endif
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
    f_closedir(&dirs);
	}
	else {
		AccFiles = 0;
		AccSize = 0;
	}

#ifdef __DEBUG
	printk("\r%u files, %lu bytes.\n%u folders.\n"
				 "%lu KB total disk space.\n%lu KB available.\n",
			AccFiles, AccSize, AccDirs,	totalsize, freesize);
#endif
	firstfile = 0;
  if((freesize < (totalsize*0.2)) || (AccFiles >= MAX_FILE)) {
    if(AccFiles < DELETE_FACTOR)
      DelFiles = 1;
    else
      DelFiles = AccFiles - (MAX_FILE - DEL_FILE);
    
    return FR_DISK_FULL;
  }
	return res;
}


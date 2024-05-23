#ifndef __SYS_STRUCT_H__
#define __SYS_STRUCT_H__


#ifdef SYS_GLOBALS
#define SYS_EXT
#else
#define SYS_EXT extern
#endif

#define GUI_PACKET_BUFFSZ   1024
#define FILE_NAME_SIZE		20

typedef struct{
	int nPort;
	int bStart_flag;		      // .
	int bEnd_flag;			      // .
	int nPacketCnt;			      // 
	int nTmr;				          // polling interval.
	char pRxBuf[GUI_PACKET_BUFFSZ];		  // rxbuff의 위치..[0x400]
	char pRxBuf_dec[GUI_PACKET_BUFFSZ];	// rxbuff의 위치..[0x400]
	char *pTxBuf;			                  // txbuff의 위치..
	int lastPacketlen;
	INT16U last_CRC16;
}commProcess_t;



typedef __packed struct{
	INT8U	project_code[10];			
	INT8U	boot_seq[10];
	INT16U	download_code;
	INT32U	file_size;
	INT16U	file_crc;
}Prjcode_t;

typedef /*__packed*/ struct{
	INT8U Sys_firstboot;
	INT8U reserve[200];
}SysControl_t;



typedef struct
{
	INT32U		keyon_flag	 ;
	INT8U		OffCount	 ;
	INT8U		keyon_st	 ;
	INT8U		poweroff_save;
	INT8U		nStatus		 ;
}keyon_t;

typedef struct
{
	INT16U	ver;
	INT8U	mode;
	INT8U	press;
}scr_t;



#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
typedef struct
{
	INT8U		major_ver						;
	INT8U		minor_ver						;
	INT8U		tInfiniteLoop				;
	INT8U		twatchCliFlag				;
	INT8U		twatchCliCount			;
	INT8U		twatchTmrFlag				;
	INT8U		twatchTmrCount			;
	INT8U		twatchCanFlag				;
	INT8U		twatchCanCount			;
	INT8U		twatchUSBFlag				;
	INT8U		twatchUSBCount			;
	INT8U		twatchKeyFlag				;
	INT8U		twatchKeyCount			;
	INT8U		twatchObdFlag				;	
  INT8U		twatchObdCount			;  
	INT8U		twatchModFlag				;	
  INT8U		twatchModCount			;    
  INT8U		twatchGpsFlag				;	
  INT8U		twatchGpsCount			; 
  
	keyon_t		key								;
	struct tm	tm_ptr						;
	char		filename[FILE_NAME_SIZE]		;

	scr_t		Scrst							;


	//SysControl_t *Scrset						;
	//SysStatus_t  *Scrst							;



}sysinfo_t;
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif



 
SYS_EXT Prjcode_t tPrjcode;					//boot & download Struct,Bootloder랑 같은 구조여야 한다.틀리면 안된다....

SYS_EXT sysinfo_t _sysinfo ;				//Status Struct
SYS_EXT SysControl_t SysSet;				//Control Struct
SYS_EXT void sys_struct_init(void);
SYS_EXT INT8U MakeFileName(INT8U Path, char *Target, char *Sorce, INT8U size);




#endif
#ifndef __SYS_STRUCT_H__
#define __SYS_STRUCT_H__


#ifdef SYS_GLOBALS
#define SYS_EXT
#else
#define SYS_EXT extern
#endif


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
	INT32U		nloop_count  ;
	INT32U		pulse_flag	 ;
	INT32U		pulse_count	 ;
	INT32U		pulse_period ;
	INT32U		speed		     ;
}speed_t;

typedef struct
{
	INT32U		nloop_count  ;
	INT32U		pulse_flag	 ;
	INT32U		pulse_count	 ;
	INT32U		pulse_period ;
	INT32U		rpm	  	     ;
}rpm_t;


typedef struct
{
	INT32U	keyon_flag ;
	INT8U		OffCount	 ;
	INT8U		keyon_st	 ;
	INT8U		poweroff_save;
	INT8U		nStatus		 ;
}keyon_t;

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
	INT8U		twatchTrkFlag				;	
  INT8U		twatchTrkCount			;  
	INT8U		twatchModemFlag			;	
  INT8U		twatchModemCount		;  
	INT8U		twatchGpsFlag				;	
  INT8U		twatchGpsCount			; 
	INT8U		twatchJigFlag				;	
  INT8U		twatchJigCount			;   
	INT8U		twatchDtgFlag				;	
  INT8U		twatchDtgCount			;
  
  speed_t pSpeed              ;
  rpm_t   pRpm                ;
	struct tm	tm_ptr						;
} sysinfo_t;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif



 
SYS_EXT Prjcode_t tPrjcode;					//boot & download Struct,Bootloder랑 같은 구조여야 한다.틀리면 안된다....

SYS_EXT sysinfo_t _sysinfo ;				//Status Struct
SYS_EXT SysControl_t SysSet;				//Control Struct
SYS_EXT void sys_struct_init(void);




#endif
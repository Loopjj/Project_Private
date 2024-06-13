#ifndef __SYS_STRUCT_H__
#define __SYS_STRUCT_H__

#ifdef SYS_GLOBALS
#define SYS_EXT
#else
#define SYS_EXT extern
#endif

typedef struct
{
	INT32U	keyon_flag	  ;
	INT8U		keyon_st	    ;
	INT8U		nStatus		    ;
}keyon_t;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
typedef struct
{
	INT8U		tInfiniteLoop					;
	INT8U		twatchCliFlag					;
	INT8U		twatchCliCount				;
	INT8U		twatchTmrFlag					;
	INT8U		twatchTmrCount				;
	INT8U		twatchGuiFlag					;
	INT8U		twatchGuiCount				;
	INT8U		twatchObdFlag					;
	INT8U		twatchObdCount				;
	INT8U		twatchEcuFlag					;
	INT8U		twatchEcuCount				;
	INT8U		twatchAdcFlag					;
	INT8U		twatchAdcCount				;
	INT8U		twatchScrFlag					;
	INT8U		twatchScrCount				;

	keyon_t key									  ;
	INT8U		ScrControlFlag				;
	INT8U		ScrDebugPrintFlag			;
	INT16U	DosingControlPeriod		;
	INT16U	DosingPeriodBackup		;

	stSysControl *Scrset					;
	stSysStatus  *Scrst						;
}sysinfo_t;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif



SYS_EXT sysinfo_t _sysinfo ;
SYS_EXT void sys_struct_init(void);




#endif
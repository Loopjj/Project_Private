#define SYS_GLOBALS
#include "includes.h"


void struct_init( sysinfo_t *s)
{
	s->tInfiniteLoop	= 0;	//Task가 무한 루프에 들어가면 flag set->iwdg에서 reset 들어간다...
	s->twatchCliFlag	= 1;
	s->twatchCliCount	= 0;
	s->twatchTmrFlag	= 1;
	s->twatchTmrCount	= 0;
	s->twatchGuiFlag	= 1;
	s->twatchGuiCount	= 0;
	s->twatchObdFlag	= 1;
	s->twatchObdCount	= 0;
	s->twatchEcuFlag	= 1;
	s->twatchEcuCount	= 0;
	s->twatchAdcFlag	= 1;
	s->twatchAdcCount	= 0;
	s->twatchScrFlag  = 1;
	s->twatchScrCount	= 0;

	s->ScrControlFlag	     = 0;
	s->ScrDebugPrintFlag   = 0;

	s->key.keyon_flag	     = 0;
	s->key.keyon_st		     = ON;
	s->key.nStatus		     = 0;
	s->DosingControlPeriod = 0;
	s->DosingPeriodBackup  = 0;

	s->Scrset				= &SysSet;
	s->Scrst				= &SysSts;
}


void sys_struct_init(void)
{
	sysinfo_t *s = &_sysinfo 	;
	struct_init( s );
}


                                        
                                        
                                        
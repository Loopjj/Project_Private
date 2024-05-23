#define SYS_GLOBALS
#include "includes.h"


void struct_init( sysinfo_t *s)
{
	s->major_ver		= 0;	
	s->minor_ver		= 1;
	s->tInfiniteLoop	= 0;	//Task가 무한 루프에 들어가면 flag set->iwdg에서 reset 들어간다...
	s->twatchCliFlag	= 1;
	s->twatchCliCount	= 0;
	s->twatchTmrFlag	= 1;
	s->twatchTmrCount	= 0;
	s->twatchCanFlag	= 1;
	s->twatchCanCount	= 0;
	s->twatchUSBFlag	= 1;
	s->twatchUSBCount	= 0;
	s->twatchKeyFlag	= 1;
	s->twatchKeyCount	= 0;
	s->twatchTrkFlag	= 1;
	s->twatchTrkCount	= 0;
	s->twatchModemFlag	= 1;
	s->twatchModemCount	= 0;
	s->twatchGpsFlag	= 1;
	s->twatchGpsCount	= 0;
	s->twatchDtgFlag	= 1;
	s->twatchDtgCount	= 0;

}


void sys_struct_init(void)
{
	sysinfo_t *s = &_sysinfo 	;
	struct_init( s );
}



                                        
                                        
                                        
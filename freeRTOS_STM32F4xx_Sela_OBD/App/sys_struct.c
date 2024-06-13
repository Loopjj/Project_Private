#define SYS_GLOBALS
#include "includes.h"


void struct_init( sysinfo_t *s)
{
	s->major_ver		= 0;	
	s->minor_ver		= 1;
	s->tInfiniteLoop	= 0;
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
	s->twatchObdFlag	= 1;
	s->twatchObdCount	= 0;
	s->twatchModFlag	= 1;
	s->twatchModCount	= 0;
	s->twatchGpsFlag	= 1;
	s->twatchGpsCount	= 0;  
	s->key.keyon_flag	= 0;
	s->key.keyon_st		= ON;
	s->key.poweroff_save = FALSE;
	s->key.nStatus		   = 0;
	s->key.OffCount		   = 0;

	memset((char *)&s->filename[0], 0x00, FILE_NAME_SIZE);
}


void sys_struct_init(void)
{
	sysinfo_t *s = &_sysinfo 	;
	struct_init( s );
}

#define PATH_PO			2
#define FILE_EXTEN		4
#define FILE_NAMEMAX	7

INT8U MakeFileName(INT8U Path, char *Target, char *Sorce, INT8U size)
{
	char tmp[FILE_NAME_SIZE];
	memset(tmp, 0 ,FILE_NAME_SIZE);

	if(size > FILE_NAME_SIZE - FILE_NAMEMAX)		//hms0:xxxxxxxxxxxxx.xcl : Name 13자리까지만 허용한다.
		return 0;

	if(Path == SAVE_SD)	// SD Card Memory save file name....
	{
		memcpy( tmp, "0:", PATH_PO );
	}
	else				// USB Memory save file name....
	{
		memcpy( tmp, "1:", PATH_PO );
	}
	memcpy(&tmp[PATH_PO]     , &Sorce[0], size                        );
	memcpy(&tmp[size+PATH_PO], ".xls"   , FILE_EXTEN                  );
	memcpy(Target            , tmp      , (size+PATH_PO + FILE_EXTEN) );
	return (size+2);
}


                                        
                                        
                                        
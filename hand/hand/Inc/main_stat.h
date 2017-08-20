#ifndef MAIN_STAT_
#define MAIN_STAT_




typedef enum _main_stat
{
	power_on,
	password_ok,
	control,
	set_current,
	set_encoding,
	
}enmu_main_stat;


extern enmu_main_stat main_stat ;
extern signed char smg_value[4];
extern unsigned char smg_cur ;
extern unsigned char smg_cur_begin ;
extern unsigned char smg_cur_end ;


#endif

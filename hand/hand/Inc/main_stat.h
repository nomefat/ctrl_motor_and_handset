#ifndef MAIN_STAT_
#define MAIN_STAT_




typedef enum _main_stat
{
	rf_error,
	power_on,
	password_ok,
	control_,
	set_current,
	set_encoding,
	
}enmu_main_stat;

typedef enum _control_stat
{
	find_dev,
	find_none_dev,
	find_ok_dev,
	set_zhengzhuan_sec,
	set_fangzhuan_sec,
	set_left_day
}enmu_control_stat;



extern enmu_main_stat main_stat ;
extern signed char smg_value[4];
extern signed char smg_cur ;
extern signed char smg_cur_begin ;
extern signed char smg_cur_end ;



#define FLASH_ADDRESS (0x08000000+0x10000-4096)

#define TIME_OUT_POWER_OFF 900






void write_password(unsigned int psw);
void read_password(void);



#endif


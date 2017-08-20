#include "main.h"
#include "stm32f1xx_hal.h"
#include "btn_display.h"
#include "main_stat.h"



int time_sec;



enmu_main_stat main_stat ;

signed char smg_value[4] = {-1,-1,-1,-1};

unsigned char smg_cur = 0;
unsigned char smg_cur_begin = 0;
unsigned char smg_cur_end = 4;


void main_stat_poll(void)
{
	
	if(main_stat == power_on)
	{
			led(LED1,0);
			led(LED2,0);
			led(LED3,0);
			if(time_sec%2)
			{
				if(smg_value[0] == -1)
					sed_smg(0,0xbf);
				if(smg_value[1] == -1)
					sed_smg(1,0xbf);
				if(smg_value[2] == -1)				
					sed_smg(2,0xbf);
				if(smg_value[3] == -1)				
					sed_smg(3,0xbf);
			}
			else
			{
				if(smg_value[0] == -1)
					sed_smg(0,0xff);
				if(smg_value[1] == -1)
					sed_smg(1,0xff);
				if(smg_value[2] == -1)				
					sed_smg(2,0xff);
				if(smg_value[3] == -1)				
					sed_smg(3,0xff);			
			}
	}
	else if(main_stat == password_ok)
	{		
		sed_smg(0,0xff);
		sed_smg(1,0xff);			
		sed_smg(2,0xff);
		sed_smg(3,0xff);	
		
		if(time_sec%3 == 0)
		{
			led(LED1,1);
			led(LED2,0);
			led(LED3,0);
		}
		else if(time_sec%3 == 1)
		{
			led(LED1,0);
			led(LED2,1);
			led(LED3,0);
		}
		else if(time_sec%3 == 2)
		{
			led(LED1,0);
			led(LED2,0);
			led(LED3,1);
		}		
	}
	
	
	
}






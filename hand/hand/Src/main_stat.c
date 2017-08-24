#include "main.h"
#include "stm32f1xx_hal.h"
#include "btn_display.h"
#include "main_stat.h"
#include "bsp_spi_nrf.h"


int time_sec;
int time_100ms;

uint32_t password = 0;

enmu_main_stat main_stat ;

signed char smg_value[4] = {-1,-1,-1,-1};

unsigned char smg_cur = 0;
unsigned char smg_cur_begin = 0;
unsigned char smg_cur_end = 4;


void main_stat_poll(void)
{

	if(main_stat == rf_error)  
	{
		if(time_100ms%2)		
		{
			sed_smg(0,E);
			sed_smg(1,R);
			sed_smg(2,R);
			sed_smg(3,E);
			if(NRF_Check() == SUCCESS)
			{
				main_stat = power_on;
			}
		}
		else
		{
			sed_smg(0,0xff);			
			sed_smg(1,0xff);	
			sed_smg(2,0xff);	
			sed_smg(3,0xff);				
		}
	}
	else if(main_stat == power_on)            //开机状态，等待输入密码
	{
			led(LED1,0);
			led(LED2,0);
			led(LED3,0);
			if(time_100ms%2)
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
	else if(main_stat == password_ok)  //密码已经验证成功
	{		
		
		if(time_100ms%9 == 0)
		{
			led(LED1,1);
			led(LED2,0);
			led(LED3,0);
		}
		else if(time_100ms%9 == 3)
		{
			led(LED1,0);
			led(LED2,1);
			led(LED3,0);
		}
		else if(time_100ms%9 == 6)
		{
			led(LED1,0);
			led(LED2,0);
			led(LED3,1);
		}		
	}
	else if(main_stat == control)  //控制电机状态
	{
		
	}
	else if(main_stat == set_current)  //设置电流限值状态
	{
		
	}		
	else if(main_stat == set_encoding) //设置编码状态
	{
		
	}		
	
}


void read_password(void)
{
	password = *((uint32_t *)FLASH_ADDRESS );
	if(password == 0xffffffff)
	{
		write_password(1234);
		password = 1234;
	}
}
	HAL_StatusTypeDef ret1;
	HAL_StatusTypeDef ret2;
	HAL_StatusTypeDef ret3;

void write_password(uint32_t psw)
{
	int delay = 1000000;

	
	FLASH_PageErase(FLASH_ADDRESS);	
	while(delay--);
	ret1 = HAL_FLASH_Unlock();		
	ret2 = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_ADDRESS,psw);
	delay = 1000000;
	while(delay--);
	ret3 = HAL_FLASH_Lock();	
}






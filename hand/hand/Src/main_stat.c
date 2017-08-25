#include "main.h"
#include "stm32f1xx_hal.h"
#include "btn_display.h"
#include "main_stat.h"
#include "bsp_spi_nrf.h"


int time_sec;
int time_100ms;

uint32_t password = 0;

enmu_main_stat main_stat ;

enmu_control_stat control_stat;

signed char smg_value[4] = {-1,-1,-1,-1};

unsigned char smg_cur = 0;
unsigned char smg_cur_begin = 0;
unsigned char smg_cur_end = 4;


int8_t zhengzhuan_sec = -1;
int8_t fangzhuan_sec = -1;

int8_t current_value = -1;

void main_stat_poll(void)
{

	if(main_stat == rf_error)    //射频错误
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

		if(control_stat==find_dev)        //找设备
		{

			
		}
		else if(control_stat==find_none_dev)  //没找到设备  闪烁编号
		{
			if(time_100ms%2)
			{
				sed_smg_number(0,smg_value[0]);
				sed_smg_number(1,smg_value[1]);			
				sed_smg_number(2,smg_value[2]);			
				sed_smg_number(3,smg_value[3]);
			}
			else
			{
				sed_smg(0,0xff);
				sed_smg(1,0xff);			
				sed_smg(2,0xff);			
				sed_smg(3,0xff);			
			}			
		}
		else if(control_stat==find_ok_dev)    //已经找到设备 定时读取设备的状态
		{
			sed_smg(0,0xbf);
			sed_smg(1,0xbf);			
			sed_smg(2,0xbf);	
			if(time_100ms%3 == 0) //定时询问设备状态
			{
				
			}				
		}
		else if(control_stat==set_zhengzhuan_sec)    //获取设备正转时间 可以修改
		{
			if(zhengzhuan_sec == -1 && time_100ms%3 == 0)
			{
				 //获取正转时间
			}
		}
		else if(control_stat==set_zhengzhuan_sec)    //获取设备反转时间 可以修改
		{
			if(fangzhuan_sec == -1 && time_100ms%3 == 0)
			{
				//获取反转时间
			}			
		}





		
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


void write_password(uint32_t psw)
{
	FLASH_EraseInitTypeDef pEraseInit;
	uint32_t PageError	;
	
	HAL_FLASH_Unlock();	
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	pEraseInit.PageAddress = FLASH_ADDRESS;
	pEraseInit.NbPages = 1;
	
	HAL_FLASHEx_Erase(&pEraseInit,&PageError);	

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_ADDRESS,psw);

	HAL_FLASH_Lock();	
}






#include "main.h"
#include "stm32f1xx_hal.h"
#include "btn_display.h"
#include "main_stat.h"
#include "bsp_spi_nrf.h"
#include "rf.h"





uint32_t time_sec;
uint32_t time_100ms;

uint32_t password = 0;

enmu_main_stat main_stat ;

enmu_control_stat control_stat;

enmu_control_stat set_current_stat;


signed char smg_value[4] = {-1,-1,-1,-1};

unsigned char smg_cur = 0;
unsigned char smg_cur_begin = 0;
unsigned char smg_cur_end = 4;


int8_t zhengzhuan_sec = -1;
int8_t fangzhuan_sec = -1;

uint32_t current_value = -1;

uint16_t dev_id = -1;

uint32_t find_dev_begin = 0;

uint16_t timeout_power_off = 0;

uint32_t rf_get_sec_flag = 0;


void main_stat_poll(void)
{
	static uint32_t now_time = 0;
	
/*---------------------------------射频错误----------------------------------------------------------*/		
	if(main_stat == rf_error)    //
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
/*-------------------------------开机状态，等待输入密码------------------------------------------*/	
	else if(main_stat == power_on)            //
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
/*--------------------------------密码已经验证成功-------------------------------------*/		
	else if(main_stat == password_ok)  //
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
/*------------------------------控制电机状态---------------------------------------------*/		
	else if(main_stat == control)  //
	{

		if(control_stat==find_dev)        //找设备
		{
			if(find_dev_begin >0)
			{
				if(time_100ms%2 == 0 && now_time != time_100ms)
				{
					led(LED2,1);
					now_time = time_100ms;
					rf_send_cmd(dev_id,CMD_HAND_GET_DEV_STAT,0);    //搜索设备
					
				}	
				else if(time_100ms%2 == 1 )
					led(LED2,0);
				
				if(time_sec - find_dev_begin >3)
				{
					find_dev_begin = 0;
					control_stat = find_none_dev;
					led(LED2,0);
				}
			}
			
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
			if(time_sec%2 == 0 && now_time != time_sec)
			{
				led(LED2,1);
				now_time = time_sec;
				rf_send_cmd(dev_id,CMD_HAND_GET_DEV_STAT,0);    //定时获取状态
				
			}	
			else if(time_sec%2 == 1 )
				led(LED2,0);		
			
		}
		else if(control_stat==set_zhengzhuan_sec)    //获取设备正转时间 可以修改
		{
			if(time_100ms%2 == 0 && now_time != time_100ms && zhengzhuan_sec<0)
			{
				led(LED2,1);
				now_time = time_100ms;
				rf_send_cmd(dev_id,CMD_HAND_GET_DEV_ZZ_SEC,0);    //搜索设备
				
			}	
			else if(time_100ms%2 == 1 )
				led(LED2,0);
			
			
			if(time_sec - rf_get_sec_flag>3)
			{
				rf_get_sec_flag = 0;
				control_stat = find_ok_dev;
				led(LED2,0);
			}
		}
		else if(control_stat==set_zhengzhuan_sec)    //获取设备反转时间 可以修改
		{
			if(time_100ms%2 == 0 && now_time != time_100ms && fangzhuan_sec<0)
			{
				led(LED2,1);
				now_time = time_100ms;
				rf_send_cmd(dev_id,CMD_HAND_GET_DEV_FZ_SEC,0);    //搜索设备
				
			}	
			else if(time_100ms%2 == 1 )
				led(LED2,0);
			
			if(time_sec - rf_get_sec_flag>3)
			{
				rf_get_sec_flag = 0;
				control_stat = find_ok_dev;
				led(LED2,0);
			}			
		}
		
	}
/*-------------------------------设置电流限值状态---------------------------------------*/		
	else if(main_stat == set_current)  //
	{
		if(set_current_stat==find_dev)        //找设备
		{
			if(find_dev_begin >0)
			{
				if(time_100ms%2 == 0 && now_time != time_100ms)
				{
					led(LED1,1);
					now_time = time_100ms;
					rf_send_cmd(dev_id,CMD_HAND_GET_DEV_CURRENT_L,0);    //搜索设备
					
				}	
				else if(time_100ms%2 == 1 )
					led(LED1,0);
				
				if(time_sec - find_dev_begin >3)
				{
					find_dev_begin = 0;
					set_current_stat = find_none_dev;
					led(LED1,0);
				}
			}
			
		}
		else if(set_current_stat==find_none_dev)  //没找到设备  闪烁编号
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
		else if(set_current_stat==find_ok_dev)    //已经找到设备 定时读取设备的状态
		{
			sed_smg(0,0xbf);
			sed_smg(1,0xbf);			
			sed_smg(2,0xbf);	
			smg_cur_begin = 3;
			if(time_100ms%3 == 0 && current_value == -1) //定时询问设备状态
			{
				led(LED1,1);
				rf_send_cmd(dev_id,CMD_HAND_GET_DEV_CURRENT_L,0);    //搜索设备
			}	
			else
				led(LED1,0);
		}		
	}		
/*------------------------------设置编码状态--------------------------------------------*/		
	else if(main_stat == set_encoding) //
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






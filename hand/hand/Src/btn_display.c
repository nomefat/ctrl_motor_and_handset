#include "main.h"
#include "stm32f1xx_hal.h"
#include "btn_display.h"
#include "main_stat.h"
#include "rf.h"




typedef struct _btn_status_count
{
	unsigned int btn_down_count;          //按键按下计数，10ms计一次
	unsigned int btn_up_count;          //按键按下计数，10ms计一次	
}struct_btn_status_count;






typedef struct _btn_event
{
	char down;
	char up;
}strcut_btn_event;



typedef struct _btn_io{

	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;

}strcut_btn_io;









struct_btn_status_count btn_status_count[22];
strcut_btn_event btn_event[22];


unsigned char Num_code[11] = {Num0,Num1,Num2,Num3,Num4,Num5,Num6,Num7,Num8,Num9,Numblk};

unsigned char display_code[5] = {0xf0,0xf0,0xf0,0xf0,0xff};

strcut_btn_io btn_io[] = {

	{R1_GPIO_Port,R1_Pin},
	{R2_GPIO_Port,R2_Pin},
	{R3_GPIO_Port,R3_Pin},
	{R4_GPIO_Port,R4_Pin},
	{R5_GPIO_Port,R5_Pin},
	{R6_GPIO_Port,R6_Pin},
	{R7_GPIO_Port,R7_Pin},
			
};

unsigned char btn_to_num[] = {0,0,0,7,4,1,F_STAR,0,0,0,8,5,2,0,0,0,0,9,6,3,F_JIN};


int power_stat = 0;
extern uint32_t password;

extern TIM_HandleTypeDef htim1;

extern void rf_send(void *pdata,uint8_t len);

extern uint8_t data[10];
	
extern enmu_control_stat control_stat;
extern enmu_control_stat set_current_stat;

	
extern int8_t zhengzhuan_sec ;
extern int8_t fangzhuan_sec ;	
extern	int8_t current_value;
extern	uint16_t dev_id ;
	
extern	int8_t find_dev_begin;
extern uint16_t timeout_power_off;	

extern int8_t rf_get_sec_flag;


void btn_0_9_callback(int i);
void btn_enter();



// 10ms 调用一次
void btn_scan(void)
{
	uint16_t read_pin;
	int i = 0;
	

	
	HAL_GPIO_WritePin(V1_GPIO_Port,V1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V2_GPIO_Port,V2_Pin,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(V3_GPIO_Port,V3_Pin,GPIO_PIN_SET);	

	for(i=0;i<7;i++)
	{
		if(GPIO_PIN_SET == HAL_GPIO_ReadPin(btn_io[i].GPIOx,btn_io[i].GPIO_Pin))
		{
			btn_status_count[i].btn_down_count = 0;
			btn_status_count[i].btn_up_count++;
			if(btn_status_count[i].btn_up_count>10 && btn_event[i].down >0)
				btn_event[i].up = 1;
		}
		else                  //按下
		{
			btn_status_count[i].btn_up_count = 0;
			btn_status_count[i].btn_down_count++;
			if(btn_status_count[i].btn_down_count>10)
				btn_event[i].down = 1;
			if(btn_status_count[i].btn_down_count>100)
				btn_event[i].down = 2;
		}
	}
	
	
	
	
	HAL_GPIO_WritePin(V1_GPIO_Port,V1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(V2_GPIO_Port,V2_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(V3_GPIO_Port,V3_Pin,GPIO_PIN_SET);	
	read_pin = GPIOA->IDR;
	for(i=0;i<7;i++)
	{
		if(GPIO_PIN_SET == HAL_GPIO_ReadPin(btn_io[i].GPIOx,btn_io[i].GPIO_Pin))
		{
			btn_status_count[i+7].btn_down_count = 0;
			btn_status_count[i+7].btn_up_count++;
			if(btn_status_count[i+7].btn_up_count>10 && btn_event[i+7].down >0)
				btn_event[i+7].up = 1;
		}
		else                  //按下
		{
			btn_status_count[i+7].btn_up_count = 0;
			btn_status_count[i+7].btn_down_count++;
			if(btn_status_count[i+7].btn_down_count>10)
				btn_event[i+7].down = 1;
			if(btn_status_count[i+7].btn_down_count>100)
				btn_event[i+7].down = 2;
		}
	}
	
	
	
	HAL_GPIO_WritePin(V1_GPIO_Port,V1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(V2_GPIO_Port,V2_Pin,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(V3_GPIO_Port,V3_Pin,GPIO_PIN_RESET);	
	read_pin = GPIOA->IDR;	
	for(i=0;i<7;i++)
	{
		if(GPIO_PIN_SET == HAL_GPIO_ReadPin(btn_io[i].GPIOx,btn_io[i].GPIO_Pin))
		{
			btn_status_count[i+14].btn_down_count = 0;
			btn_status_count[i+14].btn_up_count++;
			if(btn_status_count[i+14].btn_up_count>10 && btn_event[i+14].down >0)
				btn_event[i+14].up = 1;
		}
		else                  //按下
		{
			btn_status_count[i+14].btn_up_count = 0;
			btn_status_count[i+14].btn_down_count++;
			if(btn_status_count[i+14].btn_down_count>10)
				btn_event[i+14].down = 1;
			if(btn_status_count[i+14].btn_down_count>100)
				btn_event[i+14].down = 2;
		}
	}	
}





void desplay_scan(void)
{
	static unsigned char wei = 0;
	
	if(wei==0){
		HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led4_GPIO_Port,led4_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led5_GPIO_Port,led5_Pin,GPIO_PIN_SET);
		
		
	}
	else if(wei==1){
		HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led4_GPIO_Port,led4_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led5_GPIO_Port,led5_Pin,GPIO_PIN_SET);
	}
	else if(wei==2){
		HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(led4_GPIO_Port,led4_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led5_GPIO_Port,led5_Pin,GPIO_PIN_SET);
	}
	else if(wei==3){
		HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led4_GPIO_Port,led4_Pin,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(led5_GPIO_Port,led5_Pin,GPIO_PIN_SET);
	}
	else if(wei==4){
		HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led4_GPIO_Port,led4_Pin,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(led5_GPIO_Port,led5_Pin,GPIO_PIN_RESET);
	}

	if(display_code[wei] & (1<<0))
		HAL_GPIO_WritePin(a_GPIO_Port,a_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(a_GPIO_Port,a_Pin,GPIO_PIN_RESET);		

	if(display_code[wei] & (1<<1))
		HAL_GPIO_WritePin(b_GPIO_Port,b_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(b_GPIO_Port,b_Pin,GPIO_PIN_RESET);		

	if(display_code[wei] & (1<<2))
		HAL_GPIO_WritePin(c_GPIO_Port,c_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(c_GPIO_Port,c_Pin,GPIO_PIN_RESET);		
	
	if(display_code[wei] & (1<<3))
		HAL_GPIO_WritePin(d_GPIO_Port,d_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(d_GPIO_Port,d_Pin,GPIO_PIN_RESET);		

	if(display_code[wei] & (1<<4))
		HAL_GPIO_WritePin(e_GPIO_Port,e_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(e_GPIO_Port,e_Pin,GPIO_PIN_RESET);	

	if(display_code[wei] & (1<<5))
		HAL_GPIO_WritePin(f_GPIO_Port,f_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(f_GPIO_Port,f_Pin,GPIO_PIN_RESET);	

	if(display_code[wei] & (1<<6))
		HAL_GPIO_WritePin(g_GPIO_Port,g_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(g_GPIO_Port,g_Pin,GPIO_PIN_RESET);	

	if(display_code[wei] & (1<<7))
		HAL_GPIO_WritePin(h_GPIO_Port,h_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(h_GPIO_Port,h_Pin,GPIO_PIN_RESET);		
	
	wei++;
	if(wei>4)
		wei=0;
}

void btn_power()
{
		
		if(HAL_GPIO_ReadPin(power_btn_GPIO_Port,power_btn_Pin) == GPIO_PIN_SET)
		{
			btn_status_count[21].btn_down_count = 0;
			btn_status_count[21].btn_up_count++;
			if(btn_status_count[21].btn_up_count>10 && btn_event[21].down >0)
				btn_event[21].up = 1;
		}
		else                  //按下
		{
			btn_status_count[21].btn_up_count = 0;
			btn_status_count[21].btn_down_count++;
			if(btn_status_count[21].btn_down_count>10)
				btn_event[21].down = 1;
			if(btn_status_count[21].btn_down_count>100)
				btn_event[21].down = 2;
		}	
}

extern int time_sec;
extern int time_100ms;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int time = 0;
	time++;
	if(time%500 == 0)
	{
		timeout_power_off++;
		if(timeout_power_off > TIME_OUT_POWER_OFF)
		{
			beep_long();
			HAL_GPIO_WritePin(power_enable_GPIO_Port,power_enable_Pin,GPIO_PIN_RESET); //关机	
		}
		time_sec++;
	}
	if(time%50 == 0)
	{

		time_100ms++;
	}	
	
	btn_scan();
	desplay_scan();
	btn_power();
}


void btn_handle(void)
{
	int i=0;
	if(btn_event[21].down >0 && btn_event[21].up >0)
	{
		if(power_stat == 0)  //开机按键
		{
			btn_event[21].down = 0;
			btn_event[21].up = 0;
			power_stat = 1;
//			HAL_GPIO_WritePin(power_enable_GPIO_Port,power_enable_Pin,GPIO_PIN_SET); //开机使能
		}
		else
		{
			btn_event[21].down = 0;
			btn_event[21].up = 0;			
			beep_long();
			HAL_GPIO_WritePin(power_enable_GPIO_Port,power_enable_Pin,GPIO_PIN_RESET); //关机	
		}
	}
	
	for(i=0;i<21;i++)
	{
		if(btn_event[i].up >0 && i!=6 && i!=20)   //抛除CTRL 按键
			break;
	}
	if(i>=21)
		return;
	
	
	timeout_power_off = 0;
	btn_event[i].up = 0;
	btn_event[i].down = 0;
	
	if(i==3 || i==4|| i==5|| i==10|| i==11|| i==12|| i==13|| i==17|| i==18|| i==19)
		btn_0_9_callback(i);
	
	if(i==F_MAKE_CODE)    //编码按钮
	{
		if(main_stat == password_ok) 
		{
			beep();
			main_stat = set_encoding;
			led(LED1,0);
			led(LED2,0);
			led(LED3,1);
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xf7);
			sed_smg(1,0xf7);			
			sed_smg(2,0xf7);
			sed_smg(3,0xf7);				
		}		
	}
	else if(i==F_CONTROL)    //控制按钮
	{
		if(main_stat == password_ok) 
		{
			beep();
			main_stat = control;
			led(LED1,1);
			led(LED2,0);
			led(LED3,0);	
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xfe);
			sed_smg(1,0xfe);			
			sed_smg(2,0xfe);
			sed_smg(3,0xfe);					
		}
	}
	else if(i==F_CURRENT)   //电流设定按钮
	{
		if(main_stat == password_ok) 
		{
			beep();
			main_stat = set_current;
			led(LED1,0);
			led(LED2,1);
			led(LED3,0);
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xbf);
			sed_smg(1,0xbf);			
			sed_smg(2,0xbf);
			sed_smg(3,0xbf);				
		}		
	}
	else if(i==F_UP)   //上
	{
		beep();
		if(control_stat==find_ok_dev && main_stat == control)  //控制状态下，已经找到设备
		{
			rf_send_cmd(dev_id,CMD_HAND_ZZ,0);    //发送正转指令
		}		
	}
	else if(i==F_DOWN)   //下
	{
		beep();
		if(control_stat==find_ok_dev && main_stat == control)  //控制状态下，已经找到设备
		{
			rf_send_cmd(dev_id,CMD_HAND_FZ,0);    //发送反转指令
		}			
	}	
	else if(i==F_ENTER)  //确认按键
	{
		btn_enter();
	}
	else if(i==F_BACK)   //退格按钮
	{
		beep();
		if(control_stat==find_ok_dev && main_stat == control)  //控制状态下，已经找到设备
		{
			control_stat = set_zhengzhuan_sec;
			rf_get_sec_flag = time_sec;
			zhengzhuan_sec = -1;
			smg_cur_begin = 2;
			sed_smg(0,S);
			sed_smg(1,0XBF);		
			sed_smg(2,0XBF);	
			sed_smg(3,0XBF);				
		}					
		else if(control_stat==set_zhengzhuan_sec && main_stat == control)  //控制状态下,获取正转时间
		{
			control_stat = set_fangzhuan_sec;
			rf_get_sec_flag = time_sec;
			fangzhuan_sec = -1;
			sed_smg(0,R);	
			sed_smg(1,0XBF);		
			sed_smg(2,0XBF);	
			sed_smg(3,0XBF);				
		}				
		else if(control_stat==set_fangzhuan_sec && main_stat == control)  //控制状态下，获取反转时间
		{
			control_stat = set_zhengzhuan_sec;
			rf_get_sec_flag = time_sec;
			zhengzhuan_sec = -1;
			sed_smg(0,S);		
			sed_smg(1,0XBF);		
			sed_smg(2,0XBF);	
			sed_smg(3,0XBF);	
			
		}						
	}
	else if(i==F_EXIT)    //退出按钮
	{
		beep();
		if((control_stat==set_fangzhuan_sec ||control_stat==set_zhengzhuan_sec) && main_stat == control)	//退出到控制状态 找到设备状态
		{
			control_stat = find_ok_dev;
		}			
		else if(main_stat == control && control_stat!=set_zhengzhuan_sec && control_stat!=set_fangzhuan_sec)  //退出到待机状态
		{
			main_stat = password_ok;
			control_stat = find_dev;
			smg_cur_begin = 0;
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xff);
			sed_smg(1,0xff);			
			sed_smg(2,0xff);
			sed_smg(3,0xff);			
		}
		else if(main_stat == set_current)  //退出到待机状态
		{
			main_stat = password_ok;
			smg_cur_begin = 0;
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xff);
			sed_smg(1,0xff);			
			sed_smg(2,0xff);
			sed_smg(3,0xff);			
		}		
		else if(main_stat == set_encoding)  //退出到待机状态
		{
			main_stat = password_ok;
			smg_cur_begin = 0;
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xff);
			sed_smg(1,0xff);			
			sed_smg(2,0xff);
			sed_smg(3,0xff);			
		}				
	}

	
}




void btn_0_9_callback(int i)
{
	beep();
	
	if(smg_cur>=smg_cur_end)
		smg_cur = smg_cur_begin;
		
	sed_smg_number(smg_cur,btn_to_num[i]); 
	smg_value[smg_cur] = btn_to_num[i];
	smg_cur++;
	if(smg_cur>=smg_cur_end)
		smg_cur = smg_cur_begin;	

	display_code[0] |= 0x80;
	display_code[1] |= 0x80;
	display_code[2] |= 0x80;
	display_code[3] |= 0x80;
	
	display_code[smg_cur] &= 0x7f;

	if(control_stat==find_none_dev && main_stat == control)
	{
		control_stat = find_dev;
	}
	
}


//确认按钮
void btn_enter()
{
	beep();
	rf_send(data,4);
	if(main_stat == power_on)   //开机状告下按下确认键 进行密码校验
	{
		if(smg_value[0] == (password/1000) && smg_value[1] == (password%1000/100) && smg_value[2] == (password%100/10) && smg_value[3] == (password%10))
		{
			main_stat = password_ok;
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xff);
			sed_smg(1,0xff);			
			sed_smg(2,0xff);
			sed_smg(3,0xff);				
		}
		else if(btn_event[6].up == 1 && smg_value[0] == 6 && smg_value[1] == 6 && smg_value[2] == 6 && smg_value[3] == 6)  //超级密码  * 6 6 6 6
		{
			main_stat = password_ok;
			btn_event[6].up = 0;
			btn_event[6].down = 0;			
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			sed_smg(0,0xff);
			sed_smg(1,0xff);			
			sed_smg(2,0xff);
			sed_smg(3,0xff);				
		}			
		else
		{
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;
		}
	}
	else if(main_stat == password_ok)     //密码正确 进入待机状态
	{
		if(btn_event[6].up == 1 && smg_value[0] !=-1 && smg_value[1] !=-1 && smg_value[2] !=-1 && smg_value[3] !=-1 )		 //待机状态按下*+确认按键后 默认为修改密码功能
		{
			password = smg_value[0]*1000+smg_value[1]*100+smg_value[2]*10+smg_value[3];
			btn_event[6].up = 0;
			btn_event[6].down = 0;
			write_password(password);
			smg_value[0] = -1;
			smg_value[1] = -1;
			smg_value[2] = -1;
			smg_value[3] = -1;		
			main_stat = power_on;
		}
	}

	else if(main_stat == control)   //控制状态
	{
		if(control_stat==find_dev) 
		{
			dev_id = smg_value[0]*1000+smg_value[1]*100+smg_value[2]*10+smg_value[3];
			find_dev_begin = time_sec;
		}
		else if (control_stat==find_none_dev)
		{
			dev_id = smg_value[0]*1000+smg_value[1]*100+smg_value[2]*10+smg_value[3];
			find_dev_begin = time_sec;	
			control_stat = find_dev	;		
			sed_smg_number(0,smg_value[0]);
			sed_smg_number(1,smg_value[1]);			
			sed_smg_number(2,smg_value[2]);			
			sed_smg_number(3,smg_value[3]);			
		}
		
		else if(control_stat ==set_fangzhuan_sec )	 //反转显示状态
		{
			fangzhuan_sec = -1;
			sed_smg(2,0xbf);
			sed_smg(3,0xbf);
			led(LED2,1);
			rf_send_cmd(dev_id,CMD_HAND_SET_DEV_FZ_SEC,smg_value[2]*10+smg_value[3]);    //搜索设备
			led(LED2,0);
			//发送修改反转时间指令
		}		
		else if(control_stat==set_zhengzhuan_sec)  //正转显示状态
		{
			zhengzhuan_sec = -1;
			sed_smg(2,0xbf);
			sed_smg(3,0xbf);			
			led(LED2,1);
			rf_send_cmd(dev_id,CMD_HAND_SET_DEV_ZZ_SEC,smg_value[2]*10+smg_value[3]);    //搜索设备
			led(LED2,0);
			//发送修改正转时间指令
		}
		
	}
		
	else if(main_stat == set_current)  //设置电流状态
	{
		if(set_current_stat==find_dev) 
		{
			dev_id = smg_value[0]*1000+smg_value[1]*100+smg_value[2]*10+smg_value[3];
			find_dev_begin = time_sec;
		}
		else if (set_current_stat==find_none_dev)
		{
			dev_id = smg_value[0]*1000+smg_value[1]*100+smg_value[2]*10+smg_value[3];
			find_dev_begin = time_sec;	
			set_current_stat = find_dev	;		
			sed_smg_number(0,smg_value[0]);
			sed_smg_number(1,smg_value[1]);			
			sed_smg_number(2,smg_value[2]);			
			sed_smg_number(3,smg_value[3]);			
		}		
		
		else if(set_current_stat==find_ok_dev)
		{
		
		 if(smg_value[3] !=-1 )	 //没有按键值
		 {
			 current_value = -1;
			 sed_smg(3,0xbf);			 
			 led(LED1,1);
			 rf_send_cmd(dev_id,CMD_HAND_SET_DEV_CURRENT_L,smg_value[3]); 
			 led(LED1,1);


		 }
		 
	 }
	}	
}




void beep()
{
	int delay = 10000;
	HAL_GPIO_WritePin(beep_GPIO_Port,beep_Pin,GPIO_PIN_SET); 
	while(delay--);
	HAL_GPIO_WritePin(beep_GPIO_Port,beep_Pin,GPIO_PIN_RESET); 
}

void beep_long(void)
{
	int delay = 3000000;
	HAL_GPIO_WritePin(beep_GPIO_Port,beep_Pin,GPIO_PIN_SET); 
	while(delay--);
	HAL_GPIO_WritePin(beep_GPIO_Port,beep_Pin,GPIO_PIN_RESET); 
}


void led(int index,int stat)
{
	if(stat)
	{
		display_code[4] &= ~(1<<index);
	}
	else
	{
		display_code[4] |= (1<<index);
	}
}


void led_flash(int index)
{
	int i = 100000;
	display_code[4] &= ~(1<<index);
	while(i--);
	display_code[4] |= (1<<index);

}




void sed_smg_number(int index, int num)
{
	display_code[index] = Num_code[num];
	
}


void sed_smg(int index, int num)
{
	display_code[index] = num;
	
}




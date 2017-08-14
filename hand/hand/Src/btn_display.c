#include "main.h"
#include "stm32f1xx_hal.h"




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


struct_btn_status_count btn_status_count[21];
strcut_btn_event btn_event[21];

unsigned char display_code[5];


extern TIM_HandleTypeDef htim1;




// 10ms 调用一次
void btn_scan(void)
{
	uint16_t read_pin;
	int i = 0;
	

	
	HAL_GPIO_WritePin(V1_GPIO_Port,V1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V2_GPIO_Port,V2_Pin,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(V3_GPIO_Port,V3_Pin,GPIO_PIN_SET);	
	read_pin = GPIOA->IDR;
	for(i=0;i<7;i++)
	{
		if((1<<i) & read_pin)
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
		if((1<<i) & read_pin)
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
		if((1<<i) & read_pin)
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



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	btn_scan();
	desplay_scan();
}






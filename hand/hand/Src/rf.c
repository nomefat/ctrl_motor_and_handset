#include "main.h"
#include "stm32f1xx_hal.h"
#include "bsp_spi_nrf.h"
#include "rf.h"
#include "main_stat.h"
#include "btn_display.h"



extern SPI_HandleTypeDef hspi1;
extern enum_rf_mode rf_mode;
extern u8 rxbuf[32];
extern uint16_t dev_id;
extern enmu_control_stat control_stat;
extern enmu_control_stat set_current_stat;
extern enmu_control_stat set_id_stat;
extern int8_t zhengzhuan_sec ;
extern int8_t fangzhuan_sec ;
extern int32_t left_day ;
extern int8_t current_value;
extern uint32_t hand_code ;

struct_rf_cmd rf_cmd;



void delspy_zz(int8_t time);
void delspy_fz(int8_t time);
void delspy_stop(void);


void rf_send(void *pdata,uint8_t len)
{
	NRF_TX_Mode();
	NRF_Tx_Dat(pdata,len);
}


void rf_send_cmd(unsigned short id,unsigned char cmd,unsigned int data)
{
	uint32_t *p;
	
	p = (uint32_t *)rf_cmd.data;
	rf_cmd.head = 0xaa55;
	rf_cmd.cmd = cmd;
	rf_cmd.code = hand_code;
	*p = data;
	rf_cmd.id = id;
	rf_cmd.crc = 0;
	rf_send(&rf_cmd,sizeof(rf_cmd));
}




	
void rf_data_handle(void)
{

	uint16_t id = 0;
	uint16_t code = 0;
	
	struct_rf_cmd* ptr = (struct_rf_cmd* )rxbuf;	
	id = dev_id>>8;
	id += dev_id<<8;
	
	code = ptr->code >> 8;
	code += ptr->code << 8;
	
	beep();
	
	if(ptr->head !=0x55aa)
		return;
	
	if((ptr->id != id || hand_code != code) && ptr->cmd != CMD_DEV_NEW_CODE)
		return;
	
	
	switch(ptr->cmd)
	{
		case  CMD_DEV_STAT :  
			if(main_stat == control_)		//控制状态	
			{
				if(control_stat == find_dev) //正在找设备
				{
					control_stat = find_ok_dev;   
				}
				if(control_stat == find_ok_dev) //已经找到设备
				{
					if(ptr->data[0] == forward)
					{
						delspy_zz(ptr->data[1]);
					}
					else if(ptr->data[0] == reverse)
					{
						delspy_fz(ptr->data[1]);
					}
					else
					{
						delspy_stop();
					}
				}
			}
			else if(main_stat == set_encoding) //设置编码状态
			{
				if(set_id_stat==find_dev)
				{
					set_id_stat = find_ok_dev;
					id = rxbuf[3] + rxbuf[2]*256;
					sed_smg_number(0,id/1000);
					sed_smg_number(1,id%1000/100);	
					sed_smg_number(2,id%100/10);
					sed_smg_number(3,id%10);						
				}
			}
				
			break;     //  50   //设备返回状态	
			
    case CMD_DEV_ZZ_SEC  :         
				zhengzhuan_sec = ptr->data[0];
				sed_smg(1,0XBF);
				sed_smg_number(2,ptr->data[0]/10);	
				sed_smg_number(3,ptr->data[0]%10);			
				break;     //  51   //设备返回正转时间
		
		case CMD_DEV_FZ_SEC   :         
				fangzhuan_sec = ptr->data[0];
				sed_smg(1,0XBF);
				sed_smg_number(2,ptr->data[0]/10);	
				sed_smg_number(3,ptr->data[0]%10);	
				break;          //   52   //设备返回反转时间	
			
		case CMD_DEV_NEW_ID   :        
			if(main_stat == set_encoding)
			{
				id = ptr->data[0] + ptr->data[1]*256;
				sed_smg_number(0,id/1000);	
				sed_smg_number(1,id%1000/100);	
				sed_smg_number(2,id%100/10);	
				sed_smg_number(3,id%10);	
				smg_value[0] = id/1000;
				smg_value[1] = id%1000/100;
				smg_value[2] = id%100/10;
				smg_value[3] = id%10;					
			}			
		break;      // 53   //设备返回新ID，然后会切换ID
		
		case CMD_DEV_NEW_CODE  :       
			if(main_stat == set_encoding)
			{
				id = ptr->data[0] + ptr->data[1]*256;
				sed_smg_number(0,id/1000);	
				sed_smg_number(1,id%1000/100);	
				sed_smg_number(2,id%100/10);	
				sed_smg_number(3,id%10);		
				smg_value[0] = id/1000;
				smg_value[1] = id%1000/100;
				smg_value[2] = id%100/10;
				smg_value[3] = id%10;				
			}
		break;     // 54   //设备返回新CODE 然后会切换CODE
		
		case CMD_DEV_CURRENT_L :      
				if(set_current_stat==find_dev)
				{
					set_current_stat = find_ok_dev;
					smg_cur= 3;
					smg_cur_begin = 3;
				}
				current_value = ptr->data[0];		
				sed_smg_number(3,ptr->data[0]);				
				break;   // 55   //设备返回电流等级
		
		case CMD_DEV_LOCK_TIME   :      
				left_day = ptr->data[0];
				left_day += ptr->data[1]*256;
				sed_smg_number(1,left_day/100);	
				sed_smg_number(2,left_day%100/10);	
				sed_smg_number(3,left_day%10);	
		
				break;    // 56	 //返回锁定时间
		
		default : break;
	}
}



void delspy_zz(int8_t time)
{
	static int index = 0;
	uint8_t dis[] = {0xfe,0xbf,0xf7};

	sed_smg(0,E);	
	sed_smg_number(1,time/10);
	sed_smg_number(2,time%10);	
	sed_smg(3,dis[index++]);
	if(index>2)
		index = 0;
	
}
void delspy_fz(int8_t time)
{
	static int index = 0;
	uint8_t dis[] = {0xf7,0xbf,0xfe};
	
	sed_smg(0,_E);	
	sed_smg_number(1,time/10);
	sed_smg_number(2,time%10);	
	sed_smg(3,dis[index++]);
	if(index>2)
		index = 0;	
}
void delspy_stop(void)
{
	sed_smg(0,S);	
	sed_smg_number(1,0);
	sed_smg_number(2,0);		
	sed_smg(3,0xbf);
	
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_PIN_3 == rf_irq_Pin)
	{
		if(rf_mode == rf_tx)
		{
			NRF_Tx_over();
			NRF_go_RX_mode();
		}
		else if(rf_mode == rf_rx)
		{
			NRF_Rx_Dat();
			rf_data_handle();
		}
	}
}


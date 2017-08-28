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
extern int8_t zhengzhuan_sec ;
extern int8_t fangzhuan_sec ;
extern uint32_t current_value;


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
	*p = data;
	rf_cmd.id = id;
	rf_cmd.crc = 0;
	rf_send(&rf_cmd,sizeof(rf_cmd));
}





void rf_data_handle(void)
{
	struct_rf_cmd* ptr = (struct_rf_cmd* )rxbuf;
	uint16_t id = 0;
	
	id = dev_id>>8;
	id += dev_id<<8;
	
	beep();
	
	if(ptr->head !=0x55aa)
		return;
	
	if(ptr->id != id)
		return;
	
	
	switch(ptr->cmd)
	{
		case  CMD_DEV_STAT :     
			if(control_stat == find_dev)
			{
				control_stat = find_ok_dev;   
			}
			if(control_stat == find_ok_dev)
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
				
			break;     //  50   //设备返回状态	
			
    case CMD_DEV_ZZ_SEC  :         
				zhengzhuan_sec = ptr->data[0];
				sed_smg_number(2,ptr->data[0]/10);	
				sed_smg_number(3,ptr->data[0]%10);			
				break;     //  51   //设备返回正转时间
		
		case CMD_DEV_FZ_SEC   :         
				fangzhuan_sec = ptr->data[0];
				sed_smg_number(2,ptr->data[0]/10);	
				sed_smg_number(3,ptr->data[0]%10);	
				break;          //   52   //设备返回反转时间	
			
		case CMD_DEV_NEW_ID   :        
			
		break;      // 53   //设备返回新ID，然后会切换ID
		
		case CMD_DEV_NEW_CODE  :       

		break;     // 54   //设备返回新CODE 然后会切换CODE
		
		case CMD_DEV_CURRENT_L :      
				current_value = fangzhuan_sec = ptr->data[0];		
				sed_smg_number(3,ptr->data[0]);				
				break;   // 55   //设备返回电流等级
		
		case CMD_DEV_LOCK_TIME   :      
				
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
	sed_smg(3,dis[index]);
	
}
void delspy_fz(int8_t time)
{
	static int index = 0;
	uint8_t dis[] = {0xf7,0xbf,0xfe};
	
	sed_smg(0,_E);	
	sed_smg_number(1,time/10);
	sed_smg_number(2,time%10);	
	sed_smg(3,dis[index]);
	
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


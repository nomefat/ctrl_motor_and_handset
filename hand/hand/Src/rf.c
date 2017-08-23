#include "main.h"
#include "stm32f1xx_hal.h"
#include "bsp_spi_nrf.h"






extern SPI_HandleTypeDef hspi1;
extern enum_rf_mode rf_mode;


void rf_send(void *pdata,uint8_t len)
{
	NRF_TX_Mode();
	NRF_Tx_Dat(pdata,len);
}


void rf_data_handle(void)
{
	
}








void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_PIN_3 == rf_irq_Pin)
	{
		if(rf_mode == rf_tx)
		{
			NRF_Tx_over();
		}
		else if(rf_mode == rf_rx)
		{
			NRF_Rx_Dat();
			rf_data_handle();
		}
	}
}


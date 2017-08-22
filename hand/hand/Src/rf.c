#include "main.h"
#include "stm32f1xx_hal.h"
#include "bsp_spi_nrf.h"






extern SPI_HandleTypeDef hspi1;



void rf_send(void *pdata,uint8_t len)
{
	NRF_TX_Mode();
	NRF_Tx_Dat(pdata,len);
}






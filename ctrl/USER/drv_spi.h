/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   SPI配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__


#ifndef __USE_SOFT_SPI_INTERFACE__
#include "stm8s_clk.h"
#endif
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#include "main.h"


//SPI硬件接口IO定义
#define SPI_CLK_GPIO_PORT			GPIOC
#define SPI_CLK_GPIO_PIN			GPIO_PIN_5

#define SPI_MISO_GPIO_PORT			GPIOC
#define SPI_MISO_GPIO_PIN			GPIO_PIN_7

#define SPI_MOSI_GPIO_PORT			GPIOC
#define SPI_MOSI_GPIO_PIN			GPIO_PIN_6

#define SPI_NSS_GPIO_PORT			GPIOA
#define SPI_NSS_GPIO_PIN			GPIO_PIN_3


#define spi_set_nss_high( )			GPIO_WriteHigh( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN )		//NSS置高，取消片选
#define spi_set_nss_low( )			GPIO_WriteLow( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN )	//NSS置低，片选从设备



#ifdef __USE_SOFT_SPI_INTERFACE__					
/** 软件模拟SPI操作函数定义 */

#define spi_set_clk_high( )			GPIO_WriteHigh( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN )		//时钟置高
#define spi_set_clk_low( )			GPIO_WriteLow( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN )	//时钟置低

#define spi_set_mosi_hight( )		GPIO_WriteHigh( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN )	//输出置高（主机）
#define spi_set_mosi_low( )			GPIO_WriteLow( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN )	//输出置低（主机）

#define spi_get_miso( )				( 0 == GPIO_ReadInputDataBit( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN )) ? 0 : 1 // 读取输入状态

#endif


void drv_spi_init( void );
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );


#endif


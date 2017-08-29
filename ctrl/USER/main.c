
/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：ADC模数转换    
 * 实验平台：风驰电子STM8开发板
 * 库版本  ：V2.0.0
 * 作者    ：ling_guansheng
 * 博客    ：
 *修改时间 ：2011-12-20
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "intrinsics.h"
#include "uart.h"
#include "adc.h"
#include "tim1.h"
#include "stm8s_gpio.h"
#include "pwm.h"
#include "drv_spi.h"
#include "drv_RF24L01.h"
#include "stm8s_flash.h"



void adc_handle();
void time_left_handle();
void read_param();
void write_param();
void init_param();
void Delay(u16 nCount);
void ad_once();
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u32 time_sec = 0;
u32 AD_result;
u16 Conversion_Value;

extern u16 adc_1250mv;

u8 do_motor_stop_once = 0;

struct__system_param system_param;     //系统参数
enummotor_status motor_status;



void EEPROM_INIT(void)
{
  FLASH_DeInit();
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}


u8 read = 0;
int main(void)
{
   u8 len ;
  /* Infinite loop */
  
  /*设置内部时钟16M为主时钟*/ 
 
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  /*!<Set High speed internal clock  */

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1,ENABLE);
    
    
  init_param();
  PWM_Init();
  EEPROM_INIT();
  PWM_set_DutyCycle(0);
  gpio_init();
  drv_spi_init();
  
  NRF24L01_Gpio_Init( );
  RF24L01_Init( );
  RF24L01_Set_Mode( MODE_RX );
      
      
  adc_get_125();
  adc_get_125();
  adc_get_125();
  NRF24L01_check();

  read_param();
  adc_init();  
  __enable_interrupt();
   while (1)
   {
     adc_handle();
     time_left_handle();
     rf_rx_();
     if(do_motor_stop_once == 1)
     {
       do_motor_stop_once = 0;
       motor_stop();
     }
     
   }

  
  
}

void Delay(u16 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
u32 current = 0;
void adc_handle()
{
  
  
  
  if((ADC1->CSR & 0x80) == 0x80)
  {
      ADC1->CSR &= 0x7f;
      AD_result = 0;
      Conversion_Value = ADC1->DB0RL;
      Conversion_Value += (u16)(ADC1->DB0RH)<<8;
      AD_result +=Conversion_Value;
      Conversion_Value = ADC1->DB1RL;
      Conversion_Value += (u16)(ADC1->DB1RH)<<8;
      AD_result +=Conversion_Value;  
      Conversion_Value = ADC1->DB2RL;
      Conversion_Value += (u16)(ADC1->DB2RH)<<8;
      AD_result +=Conversion_Value;    
      Conversion_Value = ADC1->DB3RL;
      Conversion_Value += (u16)(ADC1->DB3RH)<<8;
      AD_result +=Conversion_Value;    
      Conversion_Value = ADC1->DB4RL;
      Conversion_Value += (u16)(ADC1->DB5RH)<<8;
      AD_result +=Conversion_Value;    
       Conversion_Value = ADC1->DB5RL;
      Conversion_Value += (u16)(ADC1->DB5RH)<<8;
      AD_result +=Conversion_Value;   
      Conversion_Value = ADC1->DB6RL;
      Conversion_Value += (u16)(ADC1->DB6RH)<<8;
      AD_result +=Conversion_Value;  
      Conversion_Value = ADC1->DB7RL;
      Conversion_Value += (u16)(ADC1->DB7RH)<<8;
      AD_result +=Conversion_Value;  
      Conversion_Value = ADC1->DB8RL;
      Conversion_Value += (u16)(ADC1->DB8RH)<<8;
      AD_result +=Conversion_Value;  
      Conversion_Value = ADC1->DB9RL;
      Conversion_Value += (u16)(ADC1->DB9RH)<<8;
      AD_result +=Conversion_Value;    
  
      AD_result = AD_result/10;
  }
  else
    return;
  
  current =  (AD_result*12500)/adc_1250mv;
  current = 0;

  if(current > system_param.current_threshold[system_param.current_chose])
  {
    led(LED3,1);
    led(LED2,1);
    if(motor_status == forward)
    {
      motor_status = forward_overflow_to_stop;
      motor_stop();
    }
    else if(motor_status == reverse)
    {
      motor_status = reverse_voceflow_to_stop;
      motor_stop();
    }
  }


}





void read_param()
{
  u8 i = 0;
  u8 *ptr = (u8 *)(&system_param);
  u8 size = sizeof(struct__system_param);
  
  if((*(PointerAttr uint8_t *) (uint16_t)0x4000) == 0x56 )
  {
    for(i=0;i<size;i++)
    {
      ptr[i] = (*(PointerAttr uint8_t *) (uint16_t)(0x4000+i));
    }
  }
  else
  {
    write_param();
  }
}



void write_param()
{
  u8 i = 0;
  u8 *ptr = (u8 *)(&system_param);
  u8 size = sizeof(struct__system_param);

  for(i=0;i<size;i++)
  {
     FLASH_ProgramByte((0x4000+i),0);
  }  
  
  
  for(i=0;i<size;i++)
  {
     FLASH_ProgramByte((0x4000+i),ptr[i]);
  }
  
}



void time_left_handle()
{
  static u32 t = 0;
  
  if(time_sec%60 == 0 && t!=time_sec)
  {
    t = time_sec;
    if(system_param.time_left>0)
      system_param.time_left--;
    write_param();    
  }
}


void init_param()
{
  system_param.head = 0x56;
  system_param.area_code = 0x00;
  system_param.id = 1234;
  system_param.current_chose = 0;
  system_param.forward_time = 99;
  system_param.reverse_time = 99;
  system_param.time_left = 259200;
  system_param.current_threshold[0] = 300;
  system_param.current_threshold[1] = 500;
  system_param.current_threshold[2] = 800;
  system_param.current_threshold[3] = 1000;
  system_param.current_threshold[4] = 1200;  
}





#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/

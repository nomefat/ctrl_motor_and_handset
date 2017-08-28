

/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ********************/

#include "stm8s.h"
#include "uart.h"
#include "adc.h"
#include "drv_RF24L01.h"
#include "pwm.h"



extern void Delay(u16 nCount);
u8 RxBuffer[RxBufferSize];
u8 UART_RX_NUM=0;
extern u8 zz_time;
extern u8 fz_time;
extern u32 AD_result;
extern u8 do_motor_stop_once;
extern enummotor_status motor_status;


#pragma vector=1
__interrupt void TRAP_IRQHandler(void)
{
  
}
#pragma vector=2
__interrupt void TLI_IRQHandler(void)
{
  
}
#pragma vector=3
__interrupt void AWU_IRQHandler(void)
{
  
}
#pragma vector=4
__interrupt void CLK_IRQHandler(void)
{
  
  
}
#pragma vector=5
__interrupt void EXTI_PORTA_IRQHandler(void)
{
  
}
#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void)
{
  
}
#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  
}
#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{
  
}
#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void)
{
   
}
#ifdef STM8S903
#pragma vector=0xA
__interrupt void EXTI_PORTF_IRQHandler(void)
{
  
}
#endif
#ifdef STM8S208
#pragma vector=0xA
__interrupt void CAN_RX_IRQHandler(void)
{
  
}
#pragma vector=0xB
__interrupt void CAN_TX_IRQHandler(void)
{
  
}
#endif
#pragma vector=0xC
__interrupt void SPI_IRQHandler(void)
{
  
}

extern enummotor_status motor_status;
extern u32 time_sec;
extern u16 pwm_val;
u16 time_ms = 0;
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
{

  if(time_ms%100 == 0)
  {
    if((motor_status == forward || motor_status == reverse) && pwm_val<1000)
    {
      pwm_val += 50;
      PWM_set_DutyCycle(pwm_val);
    }
  }
  
  
  if(time_ms++ >=1000)
  {
    time_ms = 0;
    time_sec++;   
    if(zz_time>0)
    {
      zz_time--;
      if(zz_time == 0 && motor_status == forward)
      {
        motor_status = forward_time_to_stop;
        do_motor_stop_once = 1;
      }
    }
    if(fz_time>0)
    {
      fz_time--;
      if(fz_time == 0 && motor_status == reverse)
      {
        motor_status = reverse_time_to_stop;
        do_motor_stop_once = 1;
      }
    }

  }

 
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void)
{
  
}
#ifdef STM8S903
#pragma vector=0xF
__interrupt void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void)
{
  
}
#pragma vector=0x10
__interrupt void TIM5_CAP_COM_IRQHandler(void)
{
  
}
#else
#pragma vector=0xF
__interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void)
{
     
}
#pragma vector=0x10
__interrupt void TIM2_CAP_COM_IRQHandler(void)
{
  
}
#endif
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
#pragma vector=0x11
__interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void)
{
  
}
#pragma vector=0x12
__interrupt void TIM3_CAP_COM_IRQHandler(void)
{
  
}
#endif
//#ifndef STM8S105
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
#pragma vector=0x13
__interrupt void UART1_TX_IRQHandler(void)
{
  
}
#pragma vector=0x14
__interrupt void UART1_RX_IRQHandler(void)
{ 
   u8 Res;
    if(UART1_GetITStatus(UART1_IT_RXNE )!= RESET)  
    {/*接收中断(接收到的数据必须是0x0d 0x0a结尾)*/
	Res =UART1_ReceiveData8();
        /*(USART1->DR);读取接收到的数据,当读完数据后自动取消RXNE的中断标志位*/
	if(( UART_RX_NUM&0x80)==0)/*接收未完成*/
	{
	    if( UART_RX_NUM&0x40)/*接收到了0x0d*/
		{
		  if(Res!=0x0a) UART_RX_NUM=0;/*接收错误,重新开始*/
		  else  UART_RX_NUM|=0x80;	/*接收完成了 */
		}
            else /*还没收到0X0D*/
              {	
                if(Res==0x0d) UART_RX_NUM|=0x40;
                else
                  {
                    RxBuffer[ UART_RX_NUM&0X3F]=Res ;
                     UART_RX_NUM++;
                      if( UART_RX_NUM>63) UART_RX_NUM=0;/*接收数据错误,重新开始接收*/  
                  }		 
	  }
	}  		 
		}
  
  
}
#endif
#pragma vector=0x15
__interrupt void I2C_IRQHandler(void)
{
  
}
#ifdef STM8S105
#pragma vector=0x16
__interrupt void UART2_TX_IRQHandler(void)
{
   
}
#pragma vector=0x17
__interrupt void UART2_RX_IRQHandler(void)
{
   
}
#endif
#if defined(STM8S207) || defined(STM8S208)
#pragma vector=0x16
__interrupt void UART3_TX_IRQHandler(void)
{
  
}
#pragma vector=0x17
__interrupt void UART3_RX_IRQHandler(void)
{
  
}
#endif
#if defined(STM8S207) || defined(STM8S208)
#pragma vector=0x18
__interrupt void ADC2_IRQHandler(void)
{
       /* Get converted value */
    Conversion_Value = ADC2_GetConversionValue();
//    Send_ADC_Value(Conversion_Value);
    ADC2_ClearITPendingBit();
}
#else
#pragma vector=0x18
__interrupt void ADC1_IRQHandler(void)
{
  /*
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
  */
  ADC1_ClearITPendingBit(ADC1_IT_EOC);
}
#endif
#ifdef STM8S903
#pragma vector=0x19
__interrupt void TIM6_UPD_OVF_TRG_IRQHandler(void)
{
  
}
#else
#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
  
}
#endif
#pragma vector=0x1A
__interrupt void EEPROM_EEC_IRQHandler(void)
{
  
}


/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/
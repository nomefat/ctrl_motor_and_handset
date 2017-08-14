
/******************** (C) COPYRIGHT  ��۵���Ƕ��ʽ���������� ********************
 * �ļ���  ��main.c
 * ����    ��ADCģ��ת��    
 * ʵ��ƽ̨����۵���STM8������
 * ��汾  ��V2.0.0
 * ����    ��ling_guansheng
 * ����    ��
 *�޸�ʱ�� ��2011-12-20
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "intrinsics.h"
#include "uart.h"
#include "adc.h"
#include "tim1.h"
void Delay(u16 nCount);
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




typedef struct _system_param
{
  u32 time_left ;                    // ʣ������ ��Ϊ0ʱ���ɲ���
  u16 area_code ;                    //������
  u16 id;                        //����ID
  u8 forward_time;              //��תʱ��
  u8 reverse_time;              //��תʱ��
  u8 current_threshold[5];      //������ֵ
  
  
}struct__system_param; 



struct__system_param system_param;     //ϵͳ����



enum enum_motor_status
{
  none;                
  forward;                  //��ת
  forward_to_stop;          //��תֹͣ
  forward_time_to_stop;     //��תʱ�䵽��ֹͣ
  reverse;                  //��ת
  reverse_to_stop;          //��תֹͣ
  reverse_time_to_stop;     //��תʱ�䵽��ֹͣ
  forward_overflow_to_stop;  //��ת���������ƺ�ֹͣ
  reverse_voceflow_to_stop;  //��ת���������ƺ�ֹͣ
}motor_status;










int main(void)
{
   u8 len ;
  /* Infinite loop */
  
  /*�����ڲ�ʱ��16MΪ��ʱ��*/ 
 
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  /*!<Set High speed internal clock  */

   Uart_Init();
   UART1_SendString("ADC_CONVERSION AD_Value:",sizeof("ADC_CONVERSION AD_Value:"));
    ADC_Init();
    #if CONVERSIONMODE ==CONVERSIONMODE_CONTINUOUS
    Tim1_Init();
    #endif
   Delay(0xffff);
   Delay(0xffff);
   Delay(0xffff);
  __enable_interrupt();
   while (1)
   {
     
  
      if(UART_RX_NUM&0x80)
      {
        len=UART_RX_NUM&0x3f;/*�õ��˴ν��յ������ݳ���*/
        UART1_SendString("You sent the messages is:",sizeof("You sent the messages is"));
        UART1_SendString(RxBuffer,len);
        UART1_SendByte('\n');
        UART_RX_NUM=0;
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

/******************* (C) COPYRIGHT ��۵���Ƕ��ʽ���������� *****END OF FILE****/

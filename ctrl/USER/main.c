
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
void Delay(u16 nCount);
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




typedef struct _system_param
{
  u32 time_left ;                    // 剩余秒数 ，为0时不可操作
  u16 area_code ;                    //区域码
  u16 id;                        //本机ID
  u8 forward_time;              //正转时间
  u8 reverse_time;              //反转时间
  u8 current_threshold[5];      //电流阈值
  
  
}struct__system_param; 



struct__system_param system_param;     //系统参数



enum enum_motor_status
{
  none;                
  forward;                  //正转
  forward_to_stop;          //正转停止
  forward_time_to_stop;     //正转时间到了停止
  reverse;                  //反转
  reverse_to_stop;          //反转停止
  reverse_time_to_stop;     //反转时间到了停止
  forward_overflow_to_stop;  //正转电流超限制后停止
  reverse_voceflow_to_stop;  //反转电流超限制后停止
}motor_status;










int main(void)
{
   u8 len ;
  /* Infinite loop */
  
  /*设置内部时钟16M为主时钟*/ 
 
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
        len=UART_RX_NUM&0x3f;/*得到此次接收到的数据长度*/
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

/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/

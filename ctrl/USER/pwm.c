
#include "pwm.h"
#include "stm8s_tim1.h"
#include "stm8s_adc1.h"
#include "drv_RF24L01.h"

u16 adc_1250mv = 0;
u16 pwm_val;
extern enummotor_status motor_status;


void PWM_Init(void)
{
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);   //输出推拉，低电平，10MHz  
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP,1000,0);
  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_DISABLE,TIM1_OUTPUTNSTATE_DISABLE,0,TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_RESET,TIM1_OCNIDLESTATE_RESET);
  TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE); 
  TIM1_OC3PreloadConfig(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE); 
  TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE);
  TIM1_Cmd(ENABLE);
  
}

void PWM_set_DutyCycle( uint16_t TIM1_Pulse)
{
    /* Set the Pulse value */
  
    pwm_val = TIM1_Pulse;
    if(pwm_val>1000)
      pwm_val = 1000;
    TIM1->CCR3H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR3L = (uint8_t)(TIM1_Pulse);  
}



void gpio_init(void)
{
  GPIO_Init( GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_SLOW  );
  GPIO_Init( GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_OD_HIZ_SLOW  );
  GPIO_Init( GPIOD, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT  );
  GPIO_Init( GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_SLOW  );
  GPIO_Init( GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_SLOW  );  
  GPIO_Init( GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT  );

}


void led(char index,char val)
{
  if(index==LED2)
  {
    if(val==1)
      GPIO_WriteLow(GPIOD,GPIO_PIN_6);
    else
      GPIO_WriteHigh(GPIOD,GPIO_PIN_6);
  }
  else if(index==LED3)
  {
    if(val==1)
      GPIO_WriteLow(GPIOD,GPIO_PIN_5);
    else
      GPIO_WriteHigh(GPIOD,GPIO_PIN_5);    
  }
}


//电机正转
void motor_forward(void)
{
  motor_status = forward;
  GPIO_WriteLow(GPIOB,GPIO_PIN_4);  
  GPIO_WriteHigh(GPIOB,GPIO_PIN_5);  
}

//电机反转
void motor_reverse(void)
{
  motor_status = reverse;
  GPIO_WriteHigh(GPIOB,GPIO_PIN_4);  
  GPIO_WriteLow(GPIOB,GPIO_PIN_5);  
}

//电机反转
void motor_stop(void)
{
  PWM_set_DutyCycle(0);
  drv_delay_ms(50);
  GPIO_WriteLow(GPIOB,GPIO_PIN_4);  
  GPIO_WriteLow(GPIOB,GPIO_PIN_5);  
}



void adc_init()
{
 // CLK->PCKENR2 |= (1<<4);  
  
   ADC1->CR1= 0x62;           //  设置AD转换时钟1/12，连续转换   
   ADC1->CR2 = 0x08;           // 设置数据为右对齐
   ADC1->CR3 = 0x80;           // 使用数据缓冲
   ADC1->CSR = 0x24;           // 选择AD转换通道4  使能中断
   ADC1->TDRL=0xff;  //关闭施密特触发器

   ADC1->CR1 |= 0X01;
   ADC1->CR1 |= 0X01;
}

void adc_get_125()
{
   u16 delay = 0;
   u16 v;
   
   ADC1->CR1= 0x62;           //  设置AD转换时钟1/12，连续转换   
   ADC1->CR2 = 0x08;           // 设置数据为右对齐
   ADC1->CR3 = 0x00;           // 使用数据缓冲
   ADC1->CSR = 0x07;           // 选择AD转换通道4  使能中断
   ADC1->TDRL=0xff;  //关闭施密特触发器

   ADC1->CR1 |= 0X01;
   ADC1->CR1 |= 0X01;  
   
   delay= 100000;
   while(delay--);
   
   adc_1250mv = ADC1->DRL;
   v = ADC1->DRH;
   adc_1250mv += v<<8;
   
}
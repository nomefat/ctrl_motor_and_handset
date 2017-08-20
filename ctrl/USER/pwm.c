
#include "pwm.h"
#include "stm8s_tim1.h"



void PWM_Init(void)
{
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);   //输出推拉，低电平，10MHz  
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP,100,0);
  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_DISABLE,TIM1_OUTPUTNSTATE_DISABLE,0,TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_RESET,TIM1_OCNIDLESTATE_RESET);
  TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE); 
  TIM1_OC3PreloadConfig(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE); 
  TIM1_Cmd(ENABLE);
  
}

void PWM_set_DutyCycle( uint16_t TIM1_Pulse)
{
    /* Set the Pulse value */
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
  GPIO_WriteLow(GPIOB,GPIO_PIN_4);  
  GPIO_WriteHigh(GPIOB,GPIO_PIN_5);  
}

//电机反转
void motor_reverse(void)
{
  GPIO_WriteHigh(GPIOB,GPIO_PIN_4);  
  GPIO_WriteLow(GPIOB,GPIO_PIN_5);  
}


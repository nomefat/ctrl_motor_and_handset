
#include "pwm.h"
#include "stm8s_tim1.h"



void PWM_Init(void)
{

  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP,100,0);
  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_DISABLE,TIM1_OUTPUTNSTATE_DISABLE,0,TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_RESET,TIM1_OCNIDLESTATE_RESET);
  TIM1_OC3PreloadConfig(ENABLE);

  TIM1_Cmd(ENABLE);
  
}

void PWM_set_DutyCycle( uint16_t TIM1_Pulse)
{
    /* Set the Pulse value */
    TIM1->CCR3H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR3L = (uint8_t)(TIM1_Pulse);  
}




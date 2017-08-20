
#ifndef __PWM_H
#define __PWM_H

#include "stm8s.h"



void PWM_Init(void);
void PWM_set_DutyCycle( uint16_t TIM1_Pulse);
void gpio_init(void);
void led(char index,char val);
void motor_forward(void);
void motor_reverse(void);



#define LED2 2
#define LED3 3








#endif

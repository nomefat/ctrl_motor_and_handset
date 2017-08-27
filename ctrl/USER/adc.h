#ifndef __ADC_H
#define __ADC_H
#include "stm8s.h"

#define CONVERSIONMODE   0
#define CONVERSIONMODE_SINGLE  1
#define CONVERSIONMODE_CONTINUOUS  0

void ADC_Init(void);

#endif
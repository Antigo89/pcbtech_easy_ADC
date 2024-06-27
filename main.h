#ifndef H_MAIN
#define H_MAIN 1

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"

//Settings
//full 10bit=1023 10%=102 (0x0066) 80%=819 (0x0333)
#define LIM_MIN 0x0066
#define LIM_MAX 0x0333

//prototype function
void RCC_Init(void);
void led_Init(void);
void ADC1_Init(void);
void TIM2_Init(void);

#endif
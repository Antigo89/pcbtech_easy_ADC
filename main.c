/*
File    : main.c
Software "Kurs STM32 PCBtech"
Lesson 8: ADC1 Analog Watchdog.
Student: antigo1989@gmail.com
*/

#include "main.h"

//global values
volatile uint32_t counter = 0;

/*********************************main************************************/
int main(void) {
  //System init
  SystemInit();
  RCC_Init();
  __enable_irq();
  //ADC init
  ADC1_Init();
  //Timer init 
  TIM2_Init();
  //GPIO init
  led_Init();
 
  while(1){
    
    __NOP();
  }
}

/***********************interrupts function**************************/
void ADC_IRQHandler(void){
  if(ADC1->SR & ADC_SR_JEOC){
    if(ADC1->SR & ADC_SR_AWD){
      GPIOE->BSRR |= GPIO_BSRR_BR13;
      
    }else{
      GPIOE->BSRR |= GPIO_BSRR_BS13;
    }
  }
  ADC1->SR &= ~(ADC_SR_JEOC);
  ADC1->SR &= ~(ADC_SR_AWD);
}

/****************************** function**********************************/
void ADC1_Init(void){
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  //PA5
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER |= GPIO_MODER_MODE5; //PA5 - Analog
  //ADC1
  ADC1->SMPR2 |= ADC_SMPR2_SMP5_0;
  ADC1->JSQR &= ~(ADC_JSQR_JL); //1 measurement
  ADC1->JSQR |= (5 << ADC_JSQR_JSQ4_Pos); //CH5
  ADC1->CR1 |= (0b01<<ADC_CR1_RES_Pos); //10bit
  ADC1->CR1 |= ADC_CR1_JAWDEN|ADC_CR1_AWDSGL|(5 << ADC_CR1_AWDCH_Pos); //CH5 - Analog Watchdog
  ADC1->HTR = LIM_MAX;
  ADC1->LTR = LIM_MIN;
  ADC1->CR2 |= ADC_CR2_JEXTEN_1|(0b0011<<ADC_CR2_JEXTSEL_Pos); //EXT Trigger TIM2
  ADC1->CR1 |= ADC_CR1_JEOCIE; //Interrupt enable
  NVIC_EnableIRQ(ADC_IRQn);
  ADC1->CR2 |= ADC_CR2_ADON; //ADC enable
}

void TIM2_Init(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 41;
  TIM2->CR1 |= TIM_CR1_URS;
  TIM2->CR1 &= ~(TIM_CR1_CMS|TIM_CR1_DIR);
  TIM2->ARR = 1999;
  TIM2->CR2 |= (0b010<<TIM_CR2_MMS_Pos); //master
  TIM2->CR1 |= TIM_CR1_CEN;
  TIM2->EGR |= TIM_EGR_UG;
}

void led_Init(void){
  // Clock BUS Initial
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  GPIOE->MODER |= GPIO_MODER_MODE13_0|GPIO_MODER_MODE14_0|GPIO_MODER_MODE15_0;
  //LED turn off
  GPIOE->BSRR |= GPIO_BSRR_BS13|GPIO_BSRR_BS14|GPIO_BSRR_BS15;
}
/*************************** End of file ****************************/

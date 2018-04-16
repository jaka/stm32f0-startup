#include "system.h"

static void _clock_hsi48mhz(void)
{
  FLASH->ACR = FLASH_ACR_LATENCY;
  RCC->CR2 |= RCC_CR2_HSI48ON;
  while (!(RCC->CR2 & RCC_CR2_HSI48RDY));
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_HSI48;
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI48);
}

static void _delay_ms(uint16_t ms)
{
  TIM6->ARR = ms - 1;
  TIM6->CR1 = TIM_CR1_CEN;
  while(!(TIM6->SR & TIM_SR_UIF));
  TIM6->CR1 &= ~TIM_CR1_CEN;
  TIM6->SR &= ~TIM_SR_UIF;
}


void _main(void)
{
  /* HSI48 */
  _clock_hsi48mhz();

  /* GPIO */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

  /* LED */
  GPIOA->MODER &= ~GPIO_MODER_MODER5;
  GPIOA->MODER |= GPIO_MODER_MODER5_0; /* Output */

  /* Timer 6 - delay */
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  TIM6->PSC = 47999;

  for (;;) 
  {
    _delay_ms(100);
    
    GPIOA->BSRR = GPIO_BSRR_BS_5;
     
    _delay_ms(100);
    
    GPIOA->BSRR = GPIO_BSRR_BR_5;
  } 
}

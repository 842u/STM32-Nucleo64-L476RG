#ifndef INTERRUPT_BUTTON_H
#define INTERRUPT_BUTTON_H

#include "stm32l4xx_hal.h"

#define SW1_PORT GPIOC
#define SW1_PIN GPIO_PIN_5
#define SW1_IRQn EXTI9_5_IRQn

#define DEBOUNCE_TIMER TIM4
#define DEBOUNCE_TIMER_PRESCALER 80000 - 1
#define DEBOUNCE_TIMER_PERIOD 10 - 1
#define DEBOUNCE_TIMER_IRQn TIM4_IRQn

void interruptButtonInit(void);
void EXTI9_5_IRQHandler(void);
void TIM4_IRQHandler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

typedef void (*ledColorFunction)(volatile int *exitFlag);

extern volatile int interruptFlag;

#endif

#ifndef ADC_UART_USB_H
#define ADC_UART_USB_H

#include "stm32l4xx_hal.h"
#include <stdio.h>

void ADC_UART_USB_Init(void);
void TIM4_IRQHandler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif

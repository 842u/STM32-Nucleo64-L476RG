#ifndef USB_UART_H
#define USB_UART_H

#include "onBoardLD2.h"
#include "stm32l4xx_hal.h"

extern uint8_t LD2_state;

void UART_USB_Init(void);
void UART4_HandleReceive(void);
void TIM4_IRQHandler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif

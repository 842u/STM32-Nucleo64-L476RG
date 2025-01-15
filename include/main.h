#ifndef MAIN_H
#define MAIN_H

#include "stm32l4xx_hal.h"

#define ON_BOARD_B1_PIN GPIO_PIN_13
#define ON_BOARD_B1_PORT GPIOC
#define ON_BOARD_B1_DEBOUNCE_DELAY 10

void setupOnBardB1(GPIO_InitTypeDef *GPIOInitStruct);
void handleOnBoardB1(int *delayMultiplier);

#endif

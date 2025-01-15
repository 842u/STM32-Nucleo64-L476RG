#ifndef ON_BOARD_B1_H
#define ON_BOARD_B1_H

#include "stm32l4xx_hal.h"

#define ON_BOARD_B1_PIN GPIO_PIN_13
#define ON_BOARD_B1_PORT GPIOC
#define ON_BOARD_B1_DEBOUNCE_DELAY 10

void onBoardB1Init(void);
void onBoardB1Handler(int *delayMultiplier);

#endif

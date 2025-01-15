#ifndef ON_BOARD_LD2_H
#define ON_BOARD_LD2_H

#include "stm32l4xx_hal.h"

#define ON_BOARD_LD2_BLINK_DELAY_BASE 50
#define ON_BOARD_LD2_PIN GPIO_PIN_5
#define ON_BOARD_LD2_PORT GPIOA

void onBoardLD2Init(void);
void onBoardLD2Blink(int delay);

#endif

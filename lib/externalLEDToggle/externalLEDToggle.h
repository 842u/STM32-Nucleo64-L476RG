#ifndef EXTERNAL_LED_TOGGLE_H
#define EXTERNAL_LED_TOGGLE_H

#include "stm32l4xx_hal.h"

#define SW1_PIN GPIO_PIN_8
#define SW1_PORT GPIOC
#define SW1_DEBOUNCE_DELAY 10

#define D1_PIN GPIO_PIN_6
#define D1_PORT GPIOC

void externalLEDToggleInit(void);
void SW1Handler(void);

#endif

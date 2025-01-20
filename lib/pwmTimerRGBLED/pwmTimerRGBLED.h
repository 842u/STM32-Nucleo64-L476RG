#ifndef PWM_TIMER_RGB_LED_H
#define PWM_TIMER_RGB_LED_H

#include "stm32l4xx_hal.h"

#define D1_TIMER TIM3
#define D1_PORT GPIOC

#define D1_TIMER_PRESCALER 79
#define D1_TIMER_PERIOD 999

#define D1_R_CHANNEL TIM_CHANNEL_4
#define D1_R_PIN GPIO_PIN_9

#define D1_G_CHANNEL TIM_CHANNEL_3
#define D1_G_PIN GPIO_PIN_8

#define D1_B_CHANNEL TIM_CHANNEL_1
#define D1_B_PIN GPIO_PIN_6

void pwmTimerRGBLEDInit(void);
void setD1Color(int redPercentage, int greenPercentage, int bluePercentage);

#endif

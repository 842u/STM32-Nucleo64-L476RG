#include "stm32l4xx_hal.h"

void setupOnBoardLD2(GPIO_InitTypeDef *GPIOInitStruct);
void blinkOnBoardLD2(int delay);

void setupOnBardB1(GPIO_InitTypeDef *GPIOInitStruct);
void handleOnBoardB1(int *delayMultiplier);

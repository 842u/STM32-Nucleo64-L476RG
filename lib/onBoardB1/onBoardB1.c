#include "onBoardB1.h"

static void onBoardB1Setup(void) {
  GPIO_InitTypeDef GPIOInitStruct = {0};

  GPIOInitStruct.Pin = ON_BOARD_B1_PIN;
  GPIOInitStruct.Mode = GPIO_MODE_INPUT;
  GPIOInitStruct.Pull = GPIO_PULLUP;
  GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(ON_BOARD_B1_PORT, &GPIOInitStruct);
}

void onBoardB1Init(void) {
  __HAL_RCC_GPIOC_CLK_ENABLE();
  onBoardB1Setup();
}

void onBoardB1Handler(int *delayMultiplier) {
  if (HAL_GPIO_ReadPin(ON_BOARD_B1_PORT, ON_BOARD_B1_PIN) == GPIO_PIN_RESET) {
    HAL_Delay(ON_BOARD_B1_DEBOUNCE_DELAY);

    while (HAL_GPIO_ReadPin(ON_BOARD_B1_PORT, ON_BOARD_B1_PIN) ==
           GPIO_PIN_RESET)
      ;

    if (*delayMultiplier > 5) {
      *delayMultiplier = 1;
    } else {
      *delayMultiplier *= 2;
    }

    HAL_Delay(ON_BOARD_B1_DEBOUNCE_DELAY);
  }
}

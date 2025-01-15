#include "onBoardLD2.h"

static void obBoardLD2Setup(void) {
  GPIO_InitTypeDef GPIOInitStruct = {0};

  GPIOInitStruct.Pin = ON_BOARD_LD2_PIN;
  GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIOInitStruct.Pull = GPIO_NOPULL;
  GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(ON_BOARD_LD2_PORT, &GPIOInitStruct);
}

void onBoardLD2Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  obBoardLD2Setup();
}

void onBoardLD2Blink(int delay) {
  HAL_GPIO_TogglePin(ON_BOARD_LD2_PORT, ON_BOARD_LD2_PIN);
  HAL_Delay(delay);
}

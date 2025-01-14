#include "stm32l4xx_hal.h"

#define ON_BOARD_LD2_PIN GPIO_PIN_5
#define ON_BOARD_LD2_PORT GPIOA
#define ON_BOARD_LD2_BLINK_DELAY 250

void setupOnBoardLD2(GPIO_InitTypeDef *GPIOInitStruct);
void blinkOnBoardLD2(void);

int main() {
  HAL_Init();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIOInitStruct = {0};
  setupOnBoardLD2(&GPIOInitStruct);

  while (1) {
    blinkOnBoardLD2();
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

void setupOnBoardLD2(GPIO_InitTypeDef *GPIOInitStruct) {
  GPIOInitStruct->Pin = ON_BOARD_LD2_PIN;
  GPIOInitStruct->Mode = GPIO_MODE_OUTPUT_PP;
  GPIOInitStruct->Pull = GPIO_NOPULL;
  GPIOInitStruct->Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_BOARD_LD2_PORT, GPIOInitStruct);
}

void blinkOnBoardLD2(void) {
  HAL_GPIO_TogglePin(ON_BOARD_LD2_PORT, ON_BOARD_LD2_PIN);
  HAL_Delay(ON_BOARD_LD2_BLINK_DELAY);
}

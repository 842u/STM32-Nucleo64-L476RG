#include "main.h"

int main() {
  HAL_Init();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef LD2InitStruct = {0};
  setupOnBoardLD2(&LD2InitStruct);

  GPIO_InitTypeDef B1InitStruct = {0};
  setupOnBardB1(&B1InitStruct);

  int delayMultiplier = 1;

  while (1) {
    handleOnBoardB1(&delayMultiplier);
    blinkOnBoardLD2(ON_BOARD_LD2_BLINK_DELAY_BASE * delayMultiplier);
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

void blinkOnBoardLD2(int delay) {
  HAL_GPIO_TogglePin(ON_BOARD_LD2_PORT, ON_BOARD_LD2_PIN);
  HAL_Delay(delay);
}

void setupOnBardB1(GPIO_InitTypeDef *GPIOInitStruct) {
  GPIOInitStruct->Pin = ON_BOARD_B1_PIN;
  GPIOInitStruct->Mode = GPIO_MODE_INPUT;
  GPIOInitStruct->Pull = GPIO_PULLUP;
  GPIOInitStruct->Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_BOARD_B1_PORT, GPIOInitStruct);
}

void handleOnBoardB1(int *delayMultiplier) {
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

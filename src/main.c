#include "main.h"
#include "onBoardLD2.h"

int main() {
  HAL_Init();
  onBoardLD2Init();

  GPIO_InitTypeDef B1InitStruct = {0};
  setupOnBardB1(&B1InitStruct);

  int delayMultiplier = 1;

  while (1) {
    handleOnBoardB1(&delayMultiplier);
    onBoardLD2Blink(ON_BOARD_LD2_BLINK_DELAY_BASE * delayMultiplier);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

void setupOnBardB1(GPIO_InitTypeDef *GPIOInitStruct) {
  __HAL_RCC_GPIOC_CLK_ENABLE();

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

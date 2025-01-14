#include "stm32l4xx_hal.h"

#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

int main() {
  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIOInitStruct = {0};
  GPIOInitStruct.Pin = LED_PIN;
  GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIOInitStruct.Pull = GPIO_NOPULL;
  GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PORT, &GPIOInitStruct);

  while (1) {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    HAL_Delay(250);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

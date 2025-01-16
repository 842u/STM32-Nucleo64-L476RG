#include "externalLEDToggle.h"

static void D1Setup(void) {
  GPIO_InitTypeDef D1GPIOInit = {0};
  D1GPIOInit.Pin = D1_PIN;
  D1GPIOInit.Mode = GPIO_MODE_OUTPUT_PP;
  D1GPIOInit.Pull = GPIO_NOPULL;
  D1GPIOInit.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(D1_PORT, &D1GPIOInit);
}

static void SW1Setup(void) {
  GPIO_InitTypeDef SW1GPIOInit = {0};
  SW1GPIOInit.Pin = SW1_PIN;
  SW1GPIOInit.Mode = GPIO_MODE_INPUT;
  SW1GPIOInit.Pull = GPIO_PULLUP;
  SW1GPIOInit.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(SW1_PORT, &SW1GPIOInit);
}

void externalLEDToggleInit(void) {
  __HAL_RCC_GPIOC_CLK_ENABLE();
  D1Setup();
  SW1Setup();
}

void SW1Handler(void) {
  if (HAL_GPIO_ReadPin(SW1_PORT, SW1_PIN) == GPIO_PIN_RESET) {
    HAL_Delay(SW1_DEBOUNCE_DELAY);

    while (HAL_GPIO_ReadPin(SW1_PORT, SW1_PIN) == GPIO_PIN_RESET)
      ;

    HAL_GPIO_TogglePin(D1_PORT, D1_PIN);

    HAL_Delay(SW1_DEBOUNCE_DELAY);
  }
}

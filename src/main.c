#include "main.h"
#include "onBoardLD2.h"

int main() {
  HAL_Init();
  onBoardLD2Init();

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_UART4_TX_initStruct = {0};
  GPIO_UART4_TX_initStruct.Pin = GPIO_PIN_10;
  GPIO_UART4_TX_initStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_UART4_TX_initStruct.Pull = GPIO_NOPULL;
  GPIO_UART4_TX_initStruct.Alternate = GPIO_AF8_UART4;
  GPIO_UART4_TX_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_UART4_TX_initStruct);

  while (1) {
    onBoardLD2Blink(500);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

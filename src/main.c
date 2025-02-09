#include "main.h"
#include "onBoardLD2.h"

UART_HandleTypeDef huart4;
TIM_HandleTypeDef htim4;

uint8_t LD2_state = '0';

int main() {
  HAL_Init();
  onBoardLD2Init();

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_UART4_CLK_ENABLE();
  __HAL_RCC_TIM4_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_UART4_TX_initStruct = {0};
  GPIO_UART4_TX_initStruct.Alternate = GPIO_AF8_UART4;
  GPIO_UART4_TX_initStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_UART4_TX_initStruct.Pin = GPIO_PIN_10;
  GPIO_UART4_TX_initStruct.Pull = GPIO_NOPULL;
  GPIO_UART4_TX_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_UART4_TX_initStruct);

  GPIO_InitTypeDef GPIO_UART4_RX_initStruct = {0};
  GPIO_UART4_RX_initStruct.Alternate = GPIO_AF8_UART4;
  GPIO_UART4_RX_initStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_UART4_RX_initStruct.Pin = GPIO_PIN_11;
  GPIO_UART4_RX_initStruct.Pull = GPIO_PULLUP;
  GPIO_UART4_RX_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_UART4_RX_initStruct);

  UART_InitTypeDef UART4_initStruct = {0};
  UART4_initStruct.BaudRate = 9600;
  UART4_initStruct.HwFlowCtl = UART_HWCONTROL_NONE;
  UART4_initStruct.Mode = UART_MODE_TX_RX;
  UART4_initStruct.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  UART4_initStruct.OverSampling = UART_OVERSAMPLING_16;
  UART4_initStruct.Parity = UART_PARITY_NONE;
  UART4_initStruct.StopBits = UART_STOPBITS_1;
  UART4_initStruct.WordLength = UART_WORDLENGTH_8B;

  huart4.Instance = UART4;
  huart4.Init = UART4_initStruct;
  HAL_UART_Init(&huart4);

  TIM_Base_InitTypeDef TIM4_initStruct = {0};
  TIM4_initStruct.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  TIM4_initStruct.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TIM4_initStruct.CounterMode = TIM_COUNTERMODE_UP;
  TIM4_initStruct.Prescaler = 80000 - 1;
  TIM4_initStruct.Period = 1000 - 1;

  htim4.Instance = TIM4;
  htim4.Init = TIM4_initStruct;
  HAL_TIM_Base_Init(&htim4);
  HAL_TIM_Base_Start_IT(&htim4);

  HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);

  while (1) {
    HAL_UART_Receive(&huart4, &LD2_state, 1, 50);
    if (LD2_state == '0') {
      HAL_GPIO_WritePin(ON_BOARD_LD2_PORT, ON_BOARD_LD2_PIN, GPIO_PIN_RESET);
    } else if (LD2_state == '1') {
      HAL_GPIO_WritePin(ON_BOARD_LD2_PORT, ON_BOARD_LD2_PIN, GPIO_PIN_SET);
    }
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

void TIM4_IRQHandler(void) { HAL_TIM_IRQHandler(&htim4); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  __HAL_TIM_SET_COUNTER(&htim4, 0);

  if (htim->Instance == TIM4 && LD2_state == '0') {
    uint8_t data_TX[] = "LED OFF\n";
    HAL_UART_Transmit(&huart4, data_TX, sizeof(data_TX) - 1, 50);
  } else if (htim->Instance == TIM4 && LD2_state == '1') {
    uint8_t data_TX[] = "LED ON\n";
    HAL_UART_Transmit(&huart4, data_TX, sizeof(data_TX) - 1, 50);
  }
}

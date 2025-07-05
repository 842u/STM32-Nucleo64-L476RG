/**
 * FT232R USB UART IC used for UART -> USB conversion.
 * For more info check docs for electric schematic.
 */

#include "ADC_UART_USB.h"

static UART_HandleTypeDef huart4;
static TIM_HandleTypeDef htim4;

static void GPIO_UART4_TX_Setup(void) {
  GPIO_InitTypeDef GPIO_UART4_TX_initStruct = {0};
  GPIO_UART4_TX_initStruct.Alternate = GPIO_AF8_UART4;
  GPIO_UART4_TX_initStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_UART4_TX_initStruct.Pin = GPIO_PIN_10;
  GPIO_UART4_TX_initStruct.Pull = GPIO_NOPULL;
  GPIO_UART4_TX_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_UART4_TX_initStruct);
}

static void GPIO_UART4_RX_Setup(void) {
  GPIO_InitTypeDef GPIO_UART4_RX_initStruct = {0};
  GPIO_UART4_RX_initStruct.Alternate = GPIO_AF8_UART4;
  GPIO_UART4_RX_initStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_UART4_RX_initStruct.Pin = GPIO_PIN_11;
  GPIO_UART4_RX_initStruct.Pull = GPIO_PULLUP;
  GPIO_UART4_RX_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_UART4_RX_initStruct);
}

static void UART4_Setup(void) {
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
}

static void TIM4_Setup(void) {
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
}

static void NVIC_Setup(void) {
  HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

void ADC_UART_USB_Init(void) {
  __HAL_RCC_UART4_CLK_ENABLE();
  __HAL_RCC_TIM4_CLK_ENABLE();
  GPIO_UART4_TX_Setup();
  GPIO_UART4_RX_Setup();
  UART4_Setup();
  TIM4_Setup();
  NVIC_Setup();
}

void TIM4_IRQHandler(void) { HAL_TIM_IRQHandler(&htim4); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  __HAL_TIM_SET_COUNTER(&htim4, 0);

  if (htim->Instance == TIM4) {
    uint8_t data_TX[] = "LED OFF\n";
    HAL_UART_Transmit(&huart4, data_TX, sizeof(data_TX) - 1, 50);
  }
}

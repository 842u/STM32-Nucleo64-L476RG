/**
 * FT232R USB UART IC used for UART -> USB conversion.
 * For more info check docs for electric schematic.
 */

#include "ADC_UART_USB.h"

static UART_HandleTypeDef huart4;
static TIM_HandleTypeDef htim4;
static ADC_HandleTypeDef hadc1;

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

static void GPIO_ADC1_IN_Setup(void) {
  GPIO_InitTypeDef GPIO_ADC1_IN_initStruct = {0};
  GPIO_ADC1_IN_initStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_ADC1_IN_initStruct.Pin = GPIO_PIN_0;
  GPIO_ADC1_IN_initStruct.Pull = GPIO_NOPULL;
  GPIO_ADC1_IN_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_ADC1_IN_initStruct);
}

static void ADC1_Setup(void) {
  ADC_InitTypeDef ADC1_initStruct = {0};
  ADC1_initStruct.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  ADC1_initStruct.Resolution = ADC_RESOLUTION_12B;
  ADC1_initStruct.DataAlign = ADC_DATAALIGN_RIGHT;
  ADC1_initStruct.ScanConvMode = ADC_SCAN_DISABLE;
  ADC1_initStruct.EOCSelection = ADC_EOC_SINGLE_CONV;
  ADC1_initStruct.LowPowerAutoWait = ENABLE;
  ADC1_initStruct.ContinuousConvMode = DISABLE;
  ADC1_initStruct.DiscontinuousConvMode = DISABLE;
  ADC1_initStruct.DMAContinuousRequests = DISABLE;
  ADC1_initStruct.Overrun = ADC_OVR_DATA_PRESERVED;
  ADC1_initStruct.OversamplingMode = DISABLE;

  ADC_ChannelConfTypeDef ADC1_CH5_confStruct = {0};
  ADC1_CH5_confStruct.Channel = ADC_CHANNEL_5;
  ADC1_CH5_confStruct.Rank = ADC_REGULAR_RANK_1;
  ADC1_CH5_confStruct.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
  ADC1_CH5_confStruct.SingleDiff = ADC_SINGLE_ENDED;
  ADC1_CH5_confStruct.OffsetNumber = ADC_OFFSET_NONE;
  ADC1_CH5_confStruct.Offset = 0;

  hadc1.Instance = ADC1;
  hadc1.Init = ADC1_initStruct;

  HAL_ADC_Init(&hadc1);
  HAL_ADC_ConfigChannel(&hadc1, &ADC1_CH5_confStruct);
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
  __HAL_RCC_ADC_CLK_ENABLE();
  ADC1_Setup();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_ADC1_IN_Setup();

  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_UART4_TX_Setup();
  GPIO_UART4_RX_Setup();

  __HAL_RCC_UART4_CLK_ENABLE();
  UART4_Setup();

  __HAL_RCC_TIM4_CLK_ENABLE();
  TIM4_Setup();

  NVIC_Setup();
}

void TIM4_IRQHandler(void) { HAL_TIM_IRQHandler(&htim4); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM4) {
    __HAL_TIM_SET_COUNTER(&htim4, 0);

    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
      uint32_t adcValue = HAL_ADC_GetValue(&hadc1);

      char msg[32];
      int len = sprintf(msg, "ADC: %lu\r\n", adcValue);

      HAL_UART_Transmit(&huart4, (uint8_t *)msg, len, 50);
    }

    HAL_ADC_Stop(&hadc1);
  }
}

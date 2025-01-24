/**
 * External interrupt generation from SW1.
 * Interrupt flag used to break out of the infinite color functions from
 * pwmTimerRGBLED lib.
 * TIM4 debouncing multiple interrupts from SW1 noise.
 */

#include "interruptButton.h"

static TIM_HandleTypeDef htim4;

volatile int interruptFlag = 0;

void gpioSW1Setup(void) {
  GPIO_InitTypeDef gpioSW1Config = {0};
  gpioSW1Config.Mode = GPIO_MODE_IT_FALLING;
  gpioSW1Config.Pin = SW1_PIN;
  gpioSW1Config.Pull = GPIO_PULLUP;
  gpioSW1Config.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(SW1_PORT, &gpioSW1Config);
}

static void timerDebounceSetup(void) {
  htim4.Instance = DEBOUNCE_TIMER;
  htim4.Init.Prescaler = DEBOUNCE_TIMER_PRESCALER;
  htim4.Init.Period = DEBOUNCE_TIMER_PERIOD;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim4);
}

static void nvicSetup(void) {
  HAL_NVIC_SetPriority(SW1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(SW1_IRQn);

  HAL_NVIC_SetPriority(DEBOUNCE_TIMER_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DEBOUNCE_TIMER_IRQn);
}

void interruptButtonInit(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM4_CLK_ENABLE();
  gpioSW1Setup();
  timerDebounceSetup();
  nvicSetup();
}

void EXTI9_5_IRQHandler(void) {
  if (__HAL_GPIO_EXTI_GET_IT(SW1_PIN) != RESET) {
    __HAL_GPIO_EXTI_CLEAR_IT(SW1_PIN);
    if (__HAL_TIM_GET_COUNTER(&htim4) == 0) {
      __HAL_TIM_SET_COUNTER(&htim4, 0);
      HAL_TIM_Base_Start_IT(&htim4);
    }
  }
}

void TIM4_IRQHandler(void) { HAL_TIM_IRQHandler(&htim4); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == DEBOUNCE_TIMER &&
      HAL_GPIO_ReadPin(SW1_PORT, SW1_PIN) == GPIO_PIN_RESET) {
    HAL_TIM_Base_Stop_IT(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    interruptFlag = 1;
  }
}

#include "main.h"
#include "pwmTimerRGBLED.h"

#define SW1_PORT GPIOC
#define SW1_PIN GPIO_PIN_5
#define SW1_IRQn EXTI9_5_IRQn

#define DEBOUNCE_TIMER TIM4
#define DEBOUNCE_TIMER_PRESCALER 80000 - 1
#define DEBOUNCE_TIMER_PERIOD 10 - 1
#define DEBOUNCE_TIMER_IRQn TIM4_IRQn

void EXTI9_5_IRQHandler(void);
void TIM4_IRQHandler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void SysTick_Handler(void);

typedef void (*ledColorFunction)(volatile int *exitFlag);

volatile int interruptFlag = 0;

TIM_HandleTypeDef htim4;

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();
  __HAL_RCC_TIM4_CLK_ENABLE();

  GPIO_InitTypeDef gpioSW1Config = {0};
  gpioSW1Config.Mode = GPIO_MODE_IT_FALLING;
  gpioSW1Config.Pin = SW1_PIN;
  gpioSW1Config.Pull = GPIO_PULLUP;
  gpioSW1Config.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(SW1_PORT, &gpioSW1Config);

  htim4.Instance = DEBOUNCE_TIMER;
  htim4.Init.Prescaler = DEBOUNCE_TIMER_PRESCALER;
  htim4.Init.Period = DEBOUNCE_TIMER_PERIOD;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim4);

  HAL_NVIC_SetPriority(SW1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(SW1_IRQn);

  HAL_NVIC_SetPriority(DEBOUNCE_TIMER_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DEBOUNCE_TIMER_IRQn);

  ledColorFunction ledModes[5] = {setD1Red, setD1Green, setD1Blue, setD1White,
                                  setD1Rainbow};

  while (1) {
    for (int i = 0, currentMode = 0; i < 5; i++, currentMode++) {
      ledModes[currentMode](&interruptFlag);
    }
  }
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

void SysTick_Handler(void) { HAL_IncTick(); }

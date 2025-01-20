#include "main.h"

#define D1_TIMER TIM3
#define D1_PORT GPIOC

#define D1_R_CHANNEL TIM_CHANNEL_4
#define D1_R_PIN GPIO_PIN_9

#define D1_G_CHANNEL TIM_CHANNEL_3
#define D1_G_PIN GPIO_PIN_8

#define D1_B_CHANNEL TIM_CHANNEL_1
#define D1_B_PIN GPIO_PIN_6

TIM_HandleTypeDef htim3;

int main() {
  HAL_Init();
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef gpioD1RConfig = {0};
  gpioD1RConfig.Mode = GPIO_MODE_AF_PP;
  gpioD1RConfig.Pin = D1_R_PIN;
  gpioD1RConfig.Pull = GPIO_NOPULL;
  gpioD1RConfig.Speed = GPIO_SPEED_HIGH;
  gpioD1RConfig.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(D1_PORT, &gpioD1RConfig);

  GPIO_InitTypeDef gpioD1GConfig = {0};
  gpioD1GConfig.Mode = GPIO_MODE_AF_PP;
  gpioD1GConfig.Pin = D1_G_PIN;
  gpioD1GConfig.Pull = GPIO_NOPULL;
  gpioD1GConfig.Speed = GPIO_SPEED_HIGH;
  gpioD1GConfig.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(D1_PORT, &gpioD1GConfig);

  GPIO_InitTypeDef gpioD1BConfig = {0};
  gpioD1BConfig.Mode = GPIO_MODE_AF_PP;
  gpioD1BConfig.Pin = D1_B_PIN;
  gpioD1BConfig.Pull = GPIO_NOPULL;
  gpioD1BConfig.Speed = GPIO_SPEED_HIGH;
  gpioD1BConfig.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(D1_PORT, &gpioD1BConfig);

  htim3.Instance = D1_TIMER;
  htim3.Init.Prescaler = 79;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_PWM_Init(&htim3);

  TIM_OC_InitTypeDef pwmD1RConfig = {0};
  pwmD1RConfig.OCMode = TIM_OCMODE_PWM1;
  pwmD1RConfig.Pulse = 100;
  pwmD1RConfig.OCPolarity = TIM_OCPOLARITY_LOW;
  pwmD1RConfig.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &pwmD1RConfig, D1_R_CHANNEL);

  TIM_OC_InitTypeDef pwmD1GConfig = {0};
  pwmD1GConfig.OCMode = TIM_OCMODE_PWM1;
  pwmD1GConfig.Pulse = 100;
  pwmD1GConfig.OCPolarity = TIM_OCPOLARITY_LOW;
  pwmD1GConfig.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &pwmD1GConfig, D1_G_CHANNEL);

  TIM_OC_InitTypeDef pwmD1BConfig = {0};
  pwmD1BConfig.OCMode = TIM_OCMODE_PWM1;
  pwmD1BConfig.Pulse = 100;
  pwmD1BConfig.OCPolarity = TIM_OCPOLARITY_LOW;
  pwmD1BConfig.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &pwmD1BConfig, D1_B_CHANNEL);

  HAL_TIM_PWM_Start(&htim3, D1_R_CHANNEL);
  HAL_TIM_PWM_Start(&htim3, D1_G_CHANNEL);
  HAL_TIM_PWM_Start(&htim3, D1_B_CHANNEL);

  while (1) {
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

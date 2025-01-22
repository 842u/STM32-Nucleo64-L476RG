#include "pwmTimerRGBLED.h"

static TIM_HandleTypeDef htim3;

static void gpioD1Setup(void) {
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
}

/*
** Lower timer frequency to 1kHz.
** Default timer input clock is 80MHz, derived from system clock.
** clock / ((prescaler + 1) * (period + 1))
*/
static void timerD1Setup(void) {
  htim3.Instance = D1_TIMER;
  htim3.Init.Prescaler = D1_TIMER_PRESCALER;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = D1_TIMER_PERIOD;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_PWM_Init(&htim3);
}

static void pwmD1Setup(void) {
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
}

static void pwmD1Start(void) {
  HAL_TIM_PWM_Start(&htim3, D1_R_CHANNEL);
  HAL_TIM_PWM_Start(&htim3, D1_G_CHANNEL);
  HAL_TIM_PWM_Start(&htim3, D1_B_CHANNEL);
}

static double calculatePercentage(double percentage, double value) {
  if (percentage < 0.0) {
    percentage = 0.0;
  } else if (percentage > 100.0) {
    percentage = 100.0;
  }

  return (percentage / 100.0) * value;
}

static void setD1Color(int redPercentage, int greenPercentage,
                       int bluePercentage) {
  __HAL_TIM_SET_COMPARE(&htim3, D1_R_CHANNEL,
                        calculatePercentage(redPercentage, D1_TIMER_PERIOD));
  __HAL_TIM_SET_COMPARE(&htim3, D1_G_CHANNEL,
                        calculatePercentage(greenPercentage, D1_TIMER_PERIOD));
  __HAL_TIM_SET_COMPARE(&htim3, D1_B_CHANNEL,
                        calculatePercentage(bluePercentage, D1_TIMER_PERIOD));
}

void pwmTimerRGBLEDInit(void) {
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  gpioD1Setup();
  timerD1Setup();
  pwmD1Setup();
  pwmD1Start();
}

void setD1Red(void) { setD1Color(100, 0, 0); }
void setD1Green(void) { setD1Color(0, 100, 0); }
void setD1Blue(void) { setD1Color(0, 0, 100); }
void setD1White(void) { setD1Color(100, 100, 100); }

void setD1Rainbow(int *redPercentage, int *greenPercentage, int *bluePercentage,
                  int brightnessPercentage) {
  const int transitions[8][3] = {
      {1, 0, 0},  // Red increase
      {0, 1, 0},  // Green increase
      {-1, 0, 0}, // Red decrease
      {0, 0, 1},  // Blue increase
      {0, -1, 0}, // Green decrease
      {1, 0, 0},  // Red increase
      {0, 0, -1}, // Blue decrease
      {-1, 0, 0}  // Red decrease
  };

  for (int state = 0; state < 8; state++) {
    for (int i = 0; i <= brightnessPercentage; i++) {
      *redPercentage += transitions[state][0];
      *greenPercentage += transitions[state][1];
      *bluePercentage += transitions[state][2];

      setD1Color(*redPercentage, *greenPercentage, *bluePercentage);
      HAL_Delay(D1_RAINBOW_DELAY + (100 - brightnessPercentage));
    }
  }
}

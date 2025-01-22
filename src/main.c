#include "main.h"
#include "pwmTimerRGBLED.h"

#define SW1_PORT GPIOC
#define SW1_PIN GPIO_PIN_5
#define SW1_IRQn EXTI9_5_IRQn

void EXTI9_5_IRQHandler(void);
void SysTick_Handler(void);

typedef void (*ledColorFunction)(int *exitFlag);

int interruptFlag = 0;

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();

  GPIO_InitTypeDef gpioSW1Config = {0};
  gpioSW1Config.Mode = GPIO_MODE_IT_FALLING;
  gpioSW1Config.Pin = SW1_PIN;
  gpioSW1Config.Pull = GPIO_PULLUP;
  gpioSW1Config.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(SW1_PORT, &gpioSW1Config);

  HAL_NVIC_SetPriority(SW1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(SW1_IRQn);

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
    interruptFlag = 1;
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

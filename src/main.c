#include "main.h"
#include "pwmTimerRGBLED.h"

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();

  while (1) {
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

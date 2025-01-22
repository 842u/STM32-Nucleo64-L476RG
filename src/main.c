#include "main.h"
#include "pwmTimerRGBLED.h"

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();

  setD1Rainbow(100);
}

void SysTick_Handler(void) { HAL_IncTick(); }

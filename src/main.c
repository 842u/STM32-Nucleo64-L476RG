#include "main.h"
#include "pwmTimerRGBLED.h"

#define BLINK_DELAY 250

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();

  int redPercentage = 0;
  int greenPercentage = 0;
  int bluePercentage = 0;

  while (1) {
    setD1Rainbow(&redPercentage, &greenPercentage, &bluePercentage, 100);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

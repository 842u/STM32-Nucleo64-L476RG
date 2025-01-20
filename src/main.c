#include "main.h"
#include "pwmTimerRGBLED.h"

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();

  while (1) {
    setD1Color(15, 15, 15);
    HAL_Delay(1000);

    setD1Color(15, 0, 0);
    HAL_Delay(1000);

    setD1Color(0, 15, 0);
    HAL_Delay(1000);

    setD1Color(0, 0, 15);
    HAL_Delay(1000);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

#include "main.h"
#include "interruptButton.h"
#include "pwmTimerRGBLED.h"

int main() {
  HAL_Init();
  pwmTimerRGBLEDInit();
  interruptButtonInit();

  ledColorFunction ledModes[5] = {setD1Red, setD1Green, setD1Blue, setD1White,
                                  setD1Rainbow};

  while (1) {
    for (int i = 0, currentMode = 0; i < 5; i++, currentMode++) {
      ledModes[currentMode](&interruptFlag);
    }
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

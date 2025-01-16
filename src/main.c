#include "main.h"
#include "externalLEDToggle.h"

int main() {
  HAL_Init();
  externalLEDToggleInit();

  while (1) {
    SW1Handler();
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

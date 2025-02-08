#include "main.h"
#include "onBoardLD2.h"

int main() {
  HAL_Init();
  onBoardLD2Init();

  while (1) {
    onBoardLD2Blink(500);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

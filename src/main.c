#include "main.h"
#include "onBoardB1.h"
#include "onBoardLD2.h"

int main() {
  HAL_Init();

  onBoardLD2Init();
  onBoardB1Init();

  int delayMultiplier = 1;

  while (1) {
    onBoardB1Handler(&delayMultiplier);
    onBoardLD2Blink(ON_BOARD_LD2_BLINK_DELAY_BASE * delayMultiplier);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

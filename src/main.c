#include "main.h"
#include "UART_USB.h"

int main() {
  HAL_Init();
  UART_USB_Init();

  while (1) {
    UART4_HandleReceive();
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

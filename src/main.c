#include "main.h"
#include "ADC_UART_USB.h"

int main() {
  HAL_Init();
  ADC_UART_USB_Init();

  while (1) {
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

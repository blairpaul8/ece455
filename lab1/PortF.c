#include "PortF.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

void PortF_Init(void) {

  // Enable the GPIOF peripheral
  SYSCTL_RCGCGPIO_R |= (1 << 5);

  // Wait for GPIOF to be ready
  while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0) {
  }

  // Set Pins to be digital
  GPIO_PORTF_DEN_R |= 0x0E;

  // Initialize PF3, PF2, and PF1 as outputs
  GPIO_PORTF_DIR_R |= 0x0E;

  // Initialize PF4 and PF0 as inputs
  GPIO_PORTF_DIR_R &= ~0x11;
}

#include "PortF.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

void PortF_Init(void) {

  // Enable the GPIOF peripheral
  SYSCTL_RCGCGPIO_R |= (1 << 5);

  // Wait for GPIOF to be ready
  while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0) {
  }

  // Unlock the PortF to be able to write to
  // the commit register. Automatically locks after a write
  // which will happen below when we write to the CR register
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // Note this hex value is the unlock key

  // Unlock PF0
  GPIO_PORTF_CR_R |= 0x01;

  // Set Pins to be digital
  GPIO_PORTF_DEN_R |= 0x1F;

  // Initialize PF3, PF2, and PF1 as outputs
  GPIO_PORTF_DIR_R |= 0x0E;

  // Initialize PF4 and PF0 as inputs
  GPIO_PORTF_DIR_R &= ~0x11;

  // Set PF0 and PF4 to Pull Up Resistors
  GPIO_PORTF_PUR_R |= 0x11;
}

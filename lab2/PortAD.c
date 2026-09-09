#include "PortAD.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

// Bit Specific Address macros will be written below

#define SW1_PRESSED 0x02 // PD1
#define SW2_PRESSED 0x03 // PF2
//
// Think ! Which port requires all the steps, which one not?
void PortA_Init(void) {
  // Initialize PA2, PA3, and PA4 as outputs
  // Enable the GPIOA peripheral
  SYSCTL_RCGCGPIO_R |= (1 << 0);

  // Wait for GPIOA to be ready
  while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0) {
  }

  // Set Pins to be digital
  GPIO_PORTA_DEN_R |= 0x1C;

  // Initialize PA3, PA2, and PA4 as outputs
  GPIO_PORTA_DIR_R |= 0x1C;
}

void PortD_Init(void) {
  // Initialize PD1 and PD2 as inputs
  // Enable the GPIOD peripheral
  SYSCTL_RCGCGPIO_R |= (1 << 3);

  // Wait for GPIOD to be ready
  while ((SYSCTL_PRGPIO_R & (1 << 3)) == 0) {
  }

  // Set Pins to be digital
  GPIO_PORTD_DEN_R |= 0x06;

  // Initialize PD1 and PD2 as inputs for the switches
  GPIO_PORTD_DIR_R &= ~0x06;

  // Set PF0 and PF4 to Pull Up Resistors
  GPIO_PORTD_PUR_R |= 0x06;
}

// Returns only the state of the buttons
// You may change this function if you wish
uint32_t push_buttons(enum PushButton button) {
  uint32_t state = 0xFF; // Initialize to invalid value

  if (button == SW1) {
    state = (GPIO_PORTD_DATA_R & SW1_PRESSED) ? 1 : 0;

  } else if (button == SW2) {
    state = (GPIO_PORTD_DATA_R & SW2_PRESSED) ? 1 : 0;
  }

  return state;
}
// Add more function here

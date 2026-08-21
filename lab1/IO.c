#include "IO.h"

#include "stdbool.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

// Place the definition for bit specific addressing here.
// The below example is for PortA pin 5
// #define PA5 (*((volatile uint32_t *)0x40004080))

// Place your pushbutton and led functions here

// Part of PART 3B
// Turns off all LEDs
void LEDs_off(void) {}

// Should return the button states
uint32_t left_push_button(void) {

  // read the state of GPIO Pin 0
  uint32_t ret_val = 42;
  return ret_val;
}

// Turns Red LED On
void Red_on(void) {

  // Set Pin 1 to high to turn on LED
  //
  GPIO_PORTF_DATA_R |= (1 << 1);
}

void Red_off(void) {
  // Set Pin 1 to low to turn on LED
  //
  GPIO_PORTF_DATA_R &= ~(1 << 1);
}

// Turns Blue LED On
void Blue_on(void) {}

// Turns ALL LED On
void ALL_on(void) {}

// Turns Green LED On
void Green_on(void) {}

// A General Purpose Delay
void delay(void) {}

// Use this space to write some more function for PART3B

// Part of Part 3C
// Bit Specific Addressing Demonstration
// A Bit Specific demonstration for Green LED
void Green_BS(void) {}

// A Bit Specific demonstration for Red LED
void Red_BS(void) {}

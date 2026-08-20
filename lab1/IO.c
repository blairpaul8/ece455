#include "IO.h"
#include "tm4c123gh6pm.h"

// Place the definition for bit specific addressing here.
// The below example is for PortA pin 5
// #define PA5 (*((volatile uint32_t *)0x40004080))

// Place your pushbutton and led functions here

// Part of PART 3B
// Turns off all LEDs
void LEDs_off(void) {}

// Should return the button states
uint32_t pushbuttons(void) {

  // Set to 42 because the compiler expects a return value.
  // Your function only needs to return a value denoting the button states
  // rather than the answer to life, the universe, and everything...
  return 42;
}

// Turns Red LED On
void Red_on(void) {}

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

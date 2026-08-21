#include "IO.h"

#include "stdbool.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "../ti_sdk/driverlib/sysctl.h"

// Place the definition for bit specific addressing here.
// The below example is for PortA pin 5
// #define PA5 (*((volatile uint32_t *)0x40004080))

// Place your pushbutton and led functions here
// Should return the button states
uint32_t left_push_button(void) {
  // TODO: Need to implement a function for left and right push buttons
  return 42;
}

// Part of PART 3B
// Turns off all LEDs
void LEDs_off(void) {
  GPIO_PORTF_DATA_R &= ~(1 << 1);
  GPIO_PORTF_DATA_R &= ~(1 << 2);
  GPIO_PORTF_DATA_R &= ~(1 << 3);
}

// Turns Red LED On
// PF1
void Red_on(void) {

  // Set Pin 1 to high to turn on LED
  //
  GPIO_PORTF_DATA_R |= (1 << 1);
}

// Turn Red Led off
//
void Red_off(void) {
  // Set Pin 1 to low to turn on LED
  GPIO_PORTF_DATA_R &= ~(1 << 1);
}

// Turns Blue LED On
// PF2
void Blue_on(void) {
  // Set Pin 2 to high to turn on LED
  //
  GPIO_PORTF_DATA_R |= (1 << 2);
}

// Turns ALL LED On
//
void ALL_on(void) {
  GPIO_PORTF_DATA_R |= (1 << 1);
  GPIO_PORTF_DATA_R |= (1 << 2);
  GPIO_PORTF_DATA_R |= (1 << 3);
}

// Turns Green LED On
// PF3
void Green_on(void) {

  // Set Pin 3 to high to turn on LED
  //
  GPIO_PORTF_DATA_R |= (1 << 3);
}

// Turns Green LED Off
// PF3
void Green_off(void) {

  // Set Pin 3 to high to turn on LED
  //
  GPIO_PORTF_DATA_R &= ~(1 << 3);
}

// A General Purpose Delay
void delay(void) {
  // TODO: I think we can write our own delay function with the clock registers
  // or a timer instead of this function.
  // Using this in main to test blinking leds.
  SysCtlDelay(SysCtlClockGet() / 6);
}

// Use this space to write some more function for PART3B

// Part of Part 3C
// Bit Specific Addressing Demonstration
// A Bit Specific demonstration for Green LED
void Green_BS_ON(void) {
  // index the bits like an array
  GPIO_PORTF_DATA_BITS_R[1 << 3] = 0xFF;
}

void Green_BS_OFF(void) {
  // index the bits like an array
  GPIO_PORTF_DATA_BITS_R[1 << 3] = 0x00;
}

// A Bit Specific demonstration for Red LED
void Red_BS_ON(void) {
  // index the bits like an array
  GPIO_PORTF_DATA_BITS_R[1 << 1] = 0xFF;
}

void Red_BS_OFF(void) {
  // index the bits like an array
  GPIO_PORTF_DATA_BITS_R[1 << 1] = 0x00;
}

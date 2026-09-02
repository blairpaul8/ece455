#include "IO.h"

#include "stdbool.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

// Systick Values
#define CLOCK_HZ 16000000
#define CYCLES_PER_MS (CLOCK_HZ / 1000)

// Place the definition for bit specific addressing here.
// The below example is for PortA pin 5
// #define PA5 (*((volatile uint32_t *)0x40004080))
#define PF1 (*((volatile uint32_t *)0x40025008))
#define PF3 (*((volatile uint32_t *)0x40025020))

// Place your pushbutton and led functions here
// Should return the button states

// return the state of left push button
uint32_t left_push_button(void) {
  uint32_t state = (GPIO_PORTF_DATA_R & 0x10) ? 1 : 0;
  return state;
}

// return the state of right push button
uint32_t right_push_button(void) {
  uint32_t state = (GPIO_PORTF_DATA_R & 0x01) ? 1 : 0;
  return state;
}

// Part of PART 3B
// Turns off all LEDs
void LEDs_off(void) {
  GPIO_PORTF_DATA_R &= ~(1 << 1);
  GPIO_PORTF_DATA_R &= ~(1 << 2);
  GPIO_PORTF_DATA_R &= ~(1 << 3);
}

void PortA_init(void) {
  SYSCTL_RCGCGPIO_R |= 1;

  // Wait for GPIOA to be ready
  while ((SYSCTL_PRGPIO_R & 1) == 0) {
  }

  // Set PA5 to digital
  GPIO_PORTA_DEN_R |= 0x20;

  // Set PA5 to output
  GPIO_PORTA_DIR_R |= 0x20;
}

// Turns Red LED On
// PF1
void External_Led_on(void) {

  // Set Pin 5 to high to turn on LED
  GPIO_PORTA_DATA_R |= (1 << 5);
}

// Turn External Led off
// PF2
void External_Led_off(void) {
  // Set Pin 5 to low to turn off LED
  GPIO_PORTA_DATA_R &= ~(1 << 5);
}

// Turns Red LED On
// PF1
void Red_on(void) {

  // Set Pin 1 to high to turn on LED
  GPIO_PORTF_DATA_R |= (1 << 1);
}

// Turn Red Led off
// PF1
void Red_off(void) {
  // Set Pin 1 to low to turn on LED
  GPIO_PORTF_DATA_R &= ~(1 << 1);
}

// Turns Blue LED On
// PF2
void Blue_on(void) {
  // Set Pin 2 to high to turn on LED
  GPIO_PORTF_DATA_R |= (1 << 2);
}

// Turns Blue LED Off
// PF2
void Blue_off(void) {
  // Set Pin 2 to high to turn on LED
  GPIO_PORTF_DATA_R &= ~(1 << 2);
}

// Turns Green LED On
// PF3
void Green_on(void) {
  // Set Pin 3 to high to turn on LED
  GPIO_PORTF_DATA_R |= (1 << 3);
}

// Turns Green LED Off
// PF3
void Green_off(void) {
  // Set Pin 3 to high to turn on LED
  GPIO_PORTF_DATA_R &= ~(1 << 3);
}

void systick_init(void) {
  // disable systick during setup
  NVIC_ST_CTRL_R = 0;

  // Max reload value
  NVIC_ST_RELOAD_R = 0x00FFFFFF;

  // write to clear current register
  NVIC_ST_CURRENT_R = 0;

  // enable systick
  NVIC_ST_CTRL_R = 0x00000005;
}

void systick_wait(uint32_t delay) {
  // num ticks to wait
  NVIC_ST_RELOAD_R = delay - 1;

  // write to clear current register
  NVIC_ST_CURRENT_R = 0;

  // wait for count flag
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {
  }
}

// A General Purpose Delay
void delay_1ms(uint32_t delay) {
  uint32_t i;
  for (i = 0; i < delay; i++) {
    systick_wait(CYCLES_PER_MS);
  }
}

// Use this space to write some more function for PART3B

// Part of Part 3C
// Bit Specific Addressing Demonstration
// A Bit Specific demonstration for Green LED
void Green_BS_ON(void) {
  // index the bits like an array
  PF3 = 0xFF;
}

void Green_BS_OFF(void) {
  // index the bits like an array
  PF3 = 0x00;
}

// A Bit Specific demonstration for Red LED
void Red_BS_ON(void) {
  // index the bits like an array
  PF1 = 0xFF;
}

void Red_BS_OFF(void) {
  // index the bits like an array
  PF1 = 0x00;
}

void toggle_red(void) {
  Red_on();
  delay_1ms(500);
  Red_off();
  delay_1ms(500);
}

void toggle_blue(void) {
  Blue_on();
  delay_1ms(250);
  Blue_off();
  delay_1ms(250);
}

void toggle_green(void) {
  Green_on();
  delay_1ms(500);
  Green_off();
  delay_1ms(500);
}

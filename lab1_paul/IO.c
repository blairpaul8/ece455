#include "IO.h"

#include <stdbool.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"

// Note: Utilized ternary operator ?
// in place of doing an if else statement in
// a number of these functions.
//
// (some condition) ? executed if true : else false excute this
//
// This is the same as doing
//
// if (some condition) {
//  execute this
// }
// else {
//  execute this instead
// }

// Systick Values
#define CLOCK_HZ 16000000
#define CYCLES_PER_MS (CLOCK_HZ / 1000)

// Place the definition for bit specific addressing here.
// The below example is for PortA pin 5
// #define PA5 (*((volatile uint32_t *)0x40004080))

#define PF1 (*((volatile uint32_t *)0x40025008))
#define PF3 (*((volatile uint32_t *)0x40025020))

// Initialize Port A for external Led
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

// Place your pushbutton and led functions here
// Should return the button states

// return the state of the specified push button
uint32_t push_buttons(enum PushButton button) {
  uint32_t state = 0xFF; // Initialize to invalid value

  if (button == SW1) {
    state = (GPIO_PORTF_DATA_R & SW1_PRESSED) ? 1 : 0;

  } else if (button == SW2) {
    state = (GPIO_PORTF_DATA_R & SW2_PRESSED) ? 1 : 0;
  }

  return state;
}

/***********************
 *                     *
 *   Part of PART 3B   *
 *                     *
 **********************/

// Control all Leds based on state passed in
// Utilizing one function to control all leds
// with a single state
void control_all_leds(enum LedState state) {
  if (state == ON) {
    control_led(RED, ON);
    control_led(BLUE, ON);
    control_led(GREEN, ON);

  } else if (state == OFF) {
    control_led(RED, OFF);
    control_led(BLUE, OFF);
    control_led(GREEN, OFF);
  }
}

// Control external Led
// Based on state passed in
// PA5
void control_external_led(enum LedState state) {
  // Set Pin A5 base on state passed in
  (state == ON) ? (GPIO_PORTA_DATA_R |= (1 << 5))
                : (GPIO_PORTA_DATA_R &= ~(1 << 5));
}

// Control LED state
// Utilizing one function to control each led
// by passing a pin and state into the function
void control_led(enum LedPin pin, enum LedState state) {
  // Set pin based on state passed in
  (state == ON) ? (GPIO_PORTF_DATA_R |= (1 << pin))
                : (GPIO_PORTF_DATA_R &= ~(1 << pin));
}

// Part of Part 3C
// Bit Specific Addressing
// Single bit specific addressing function used
// based on the pin and state passed in.
void control_BS_led(enum LedPin pin, enum LedState state) {
  if (pin == RED) {

    // either set the bit high or low based on state passed in
    (state == ON) ? (PF1 = 0xFF) : (PF1 = 0x00);

  } else if (pin == GREEN) {

    (state == ON) ? (PF3 = 0xFF) : (PF3 = 0x00);
  }
}

// Toggle the specified led on and off
void toggle_led(enum LedPin led) {
  control_led(led, ON);
  delay_1ms(500);
  control_led(led, OFF);
  delay_1ms(500);
}

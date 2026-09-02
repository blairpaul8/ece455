#include "IO.h"

#include <stdbool.h>
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
uint32_t push_button_state(enum PushButton button) {
  uint32_t state = 0xFF; // Initialize to invalid value

  if (button == LEFT) {
    state = (GPIO_PORTF_DATA_R & 0x10) ? 1 : 0;

  } else if (button == RIGHT) {
    state = (GPIO_PORTF_DATA_R & 0x01) ? 1 : 0;
  }

  return state;
}

/***********************
 *                     *
 *   Part of PART 3B   *
 *                     *
 **********************/

// Control all Leds based on state passed in
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
// PA5
void control_external_led(enum LedState state) {
  // Set Pin A5 base on state passed in
  (state == ON) ? (GPIO_PORTA_DATA_R |= (1 << 5))
                : (GPIO_PORTA_DATA_R &= ~(1 << 5));
}

// Control LED state
//
void control_led(enum LedPin pin, enum LedState state) {
  // Set pin based on state passed in
  (state == ON) ? (GPIO_PORTF_DATA_R |= (1 << pin))
                : (GPIO_PORTF_DATA_R &= ~(1 << pin));
}

// Part of Part 3C
// Bit Specific Addressing
//
void control_BS_led(enum LedPin pin, enum LedState state) {
  // index the bits like an array
  switch (pin) {
  case RED:
    (state == ON) ? (PF1 = 0xFF) : (PF1 = 0x00);
    break;
  case GREEN:
    (state == ON) ? (PF3 = 0xFF) : (PF3 = 0x00);
    break;
  case BLUE:
    // Don't need to support BS addressing for Blue
    break;
  }
}

// Toggle the specified led on and off
void toggle_led(enum LedPin led) {
  control_led(led, ON);
  delay_1ms(500);
  control_led(led, OFF);
  delay_1ms(500);
}

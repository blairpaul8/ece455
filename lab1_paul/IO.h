#ifndef _IO_h
#define _IO_h
#include "stdint.h"

// Place prototypes for your pushbutton and LED functions here
// you may need to add more function here

enum LedState { OFF, ON };

enum LedPin { RED = 1, BLUE = 2, GREEN = 3 };

// Turn on All LEDs
void leds_on(void);

// Turns off all Leds
void leds_off(void);

// Initialize PortA for external led
void PortA_init(void);

// returns the state of the push button on PF0
uint32_t left_push_button(void);

// returns the state of the push button on PF4
uint32_t right_push_button(void);

//
void External_Led_on(void);

//
void External_Led_off(void);

// Contol led state
void control_led(enum LedPin pin, enum LedState state);

// Initialize Systick for delay function
void systick_init(void);

// ticks to wait
void systick_wait(uint32_t delay);

// delay for 1 millisecond
void delay_1ms(uint32_t);

void Green_BS_ON(void);

void Green_BS_OFF(void);

void Red_BS_ON(void);

void Red_BS_OFF(void);

void toggle_red(void);
void toggle_blue(void);
void toggle_led(enum LedPin led);

#endif

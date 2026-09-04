#ifndef _IO_h
#define _IO_h
#include <stdint.h>

// Place prototypes for your pushbutton and LED functions here
// you may need to add more function here
// Initialize PortA for external led

#define SW1_PRESSED 0x10 // PF4
#define SW2_PRESSED 0x01 // PF0

// Enums for better readability
// Decided to add these to minimize number of subroutines
// utilizing these enums they can be used as function
// parameters to control leds or read a certain push button
enum LedState { OFF, ON };
enum LedPin { RED = 1, BLUE = 2, GREEN = 3 };
enum PushButton { SW1, SW2 };

// Initialize Port A for external Led
void PortA_init(void);

// Initialize Systick and delay function
void systick_init(void);
void systick_wait(uint32_t delay);
void delay_1ms(uint32_t);

// Get state of the push buttons
uint32_t push_buttons(enum PushButton button);

// Led Control Functions
void control_all_leds(enum LedState state);
void control_external_led(enum LedState state);
void control_led(enum LedPin pin, enum LedState state);
void control_BS_led(enum LedPin pin, enum LedState state);

void Red_on(void);

// Toggle specific led on and off
void toggle_led(enum LedPin led);

#endif

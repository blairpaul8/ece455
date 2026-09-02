#ifndef _IO_h
#define _IO_h
#include <stdint.h>

// Place prototypes for your pushbutton and LED functions here
// you may need to add more function here
// Initialize PortA for external led

enum LedState { OFF, ON };

enum LedPin { RED = 1, BLUE = 2, GREEN = 3 };

enum PushButton { LEFT, RIGHT };

// Initialize Port A for external Led
void PortA_init(void);

// Initialize Systick for delay function
void systick_init(void);

// ticks to wait
void systick_wait(uint32_t delay);

// delay for 1 millisecond
void delay_1ms(uint32_t);

// returns the state of the push button
uint32_t push_button_state(enum PushButton button);

// Control all Leds based on state passed in
void control_all_leds(enum LedState state);

// Control external Led attched to PA5
void control_external_led(enum LedState state);

// Contol individual led state
void control_led(enum LedPin pin, enum LedState state);

// Control led with bit specific addressing
void control_BS_led(enum LedPin pin, enum LedState state);

// Toggle specific led on and off
void toggle_led(enum LedPin led);

#endif

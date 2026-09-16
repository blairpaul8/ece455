#ifndef _PART_2B_
#define _PART_2B_

#include <stdint.h>

// **need to change pin numbers
#define RED 2
#define YELLOW 3
#define GREEN 4

// same with switches
#define SW1 1 // PD1
#define SW2 2 // PD2
#define SW1_PRESSED 0x01
#define SW2_PRESSED 0x10

void led_on(uint32_t pin);

void led_off(uint32_t pin);

uint32_t switch_state(uint32_t switch_num);

void count_up();
void count_down();

// Might not need this function
// void toggle_leds();

#endif // _PART_2B_

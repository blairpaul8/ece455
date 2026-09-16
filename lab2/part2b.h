#ifndef _PART_2B_
#define _PART_2B_

#include <stdint.h>

#define RED 1
#define YELLOW 2
#define GREEN 3

#define SW1 5
#define SW2 6
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

#ifndef _PART_2B_
#define _PART_2B_

#include <stdint.h>

#define RED 2
#define YELLOW 3
#define GREEN 4

// TODO: Switch SW1/2 back to PD1/2
#define SW1 0 // PD1
#define SW2 4 // PD2
#define SW1_PRESSED 0x10
#define SW2_PRESSED 0x01 // Need to update these this is for PF0/4

void led_on(uint32_t pin);

void led_off(uint32_t pin);

uint32_t switch_state(uint32_t switch_num);

void count_up();
void count_down();

// Might not need this function
// void toggle_leds();

#endif // _PART_2B_

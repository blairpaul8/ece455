#ifndef _IO_h
#define _IO_h
#include "stdint.h"

// Place prototypes for your pushbutton and LED functions here
// you may need to add more function here

// Turns off all Leds
void LEDs_off(void);

// returns the state of the push button on PF0
uint32_t left_push_button(void);

// returns the state of the push button on PF4
uint32_t right_push_button(void);

// Turn on Red LED
void Red_on(void);

// Turn off Red LED
void Red_off(void);

// Turn on Blue LED
void Blue_on(void);

// Turn on Green LED
void Green_on(void);

// Turn on All LEDs
void All_on(void);

#endif

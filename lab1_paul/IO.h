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

//
void External_Led_on(void);

//
void External_Led_off(void);

// Turn on Red LED
void Red_on(void);

// Turn off Red LED
void Red_off(void);

// Turn on Blue LED
void Blue_on(void);

// Turn off Blue LED
void Blue_off(void);

// Turn on Green LED
void Green_on(void);

// Turn off Green LED
void Green_off(void);

// Turn on All LEDs
void ALL_on(void);

void delay(void);

void Green_BS_ON(void);

void Green_BS_OFF(void);

void Red_BS_ON(void);

void Red_BS_OFF(void);

void toggle_red(void);
void toggle_blue(void);
void toggle_green(void);

#endif

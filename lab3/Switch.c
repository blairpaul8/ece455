// Switch.c
// Lab 3: UART and DAC Interfacing
// ECE 455 / ECE 555, Embedded Systems Design, Fall 2026
// Team Members name: <Student1>; <Student2>
// Due Date: Friday, October 2, 2026, 11:59 p.m.
//
// Part 2B: complete Switch_Init (PE1-0 as inputs) and Switch_In (debounced
// read). Delay20ms is given; set its count for the 50 MHz bus clock.
// This software configures the on-board switches and LEDs.

// Port B bits 2-0 have the 3-bit DAC

// Port E is for switches PE0(SW1) and PE1 (SW2)

#include <stdint.h>
#include "tm4c123gh6pm.h"

//---------------------Delay20ms---------------------
// wait 20ms for switches to stop bouncing
// Input: none
// Output: none
void Delay20ms(void) {
  unsigned long volatile time;
  // 20ns per cycle at 50Mhz so 20ms is ~1,000,000 cycles if
  // this loop takes 3 then time needs to be ~333,000
  time = 333000; // You should modify this value

  while (time > 0) {
    time--;
  } // This while loop takes approximately 3 cycles
}

//---------------------Switch_Init---------------------
// initialize switch interface
// Input: none
// Output: none
void Switch_Init(void) {
  volatile unsigned long delay;
  // Following function initializw PE0 and PE1 to be used as switch
  SYSCTL_RCGCGPIO_R |= 0x10; // activate port E (bit 4 = Port E)
  delay = SYSCTL_RCGCGPIO_R; // allow time to finish activating

  GPIO_PORTE_AMSEL_R &= ~0x03;      // no analog on PE1-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // regular GPIO function
  GPIO_PORTE_DIR_R &= ~0x03;        // PE1-0 input
  GPIO_PORTE_AFSEL_R &= ~0x03;      // no alt function
  GPIO_PORTE_DEN_R |= 0x03;         // digital enable
}

//---------------------Switch_In---------------------
// read the values of the two switches
// Input: none
// Output: 0x00,0x01,0x10,0x11 from the two switches
//         0 if no switch is pressed
// bit1 PE1 SW1 switch
// bit0 PE0 SW2 switch
unsigned long Switch_In(void) {
  unsigned long first, second;
  // read the switch status
  first = GPIO_PORTE_DATA_R & 0x03;

  // Delay 20 ms, call 10 ms twice (Think, why it is written differently from
  // previous assignemnt. what could be the reason?
  Delay20ms();

  // read switch status again
  second = GPIO_PORTE_DATA_R & 0x03;

  // if both read shows same status (pressed) then only, take decision that
  // switch is pressed
  if (first == second) {
    return first;
  }

  return 0; // remoce this
}

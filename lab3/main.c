// Lab 3: UART and DAC Interfacing
// ECE 455 / ECE 555, Embedded Systems Design, Fall 2026
// Team Members name: <Student1>; <Student2>
// Due Date: Friday, October 2, 2026, 11:59 p.m.
//
// Background Setup: the bus clock must be 50 MHz. Open PLL.c and set SYSDIV2
// so that 400 MHz / (SYSDIV2 + 1) = 50 MHz (the table at the end of PLL.c).
// Every period constant in Sound.h assumes 50 MHz.

#include "stdint.h"
#include "PLL.h"
#include "UART.h"
#include "Sound.h"
#include "Switch.h"
#include "tm4c123gh6pm.h"

// Change this number to the current part of the lab you are working on
#define LAB_PART 1 // 1 for Part1, 2 for Part 2, 3 for Part 3

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

int main() {

  unsigned long i;

  // Functions
  DisableInterrupts(); // The function to disarm the Interrupts - Disabling the
                       // interrupt before the PLL configuration
  PLL_Init();          // bus clock at 50 MHz

  if (LAB_PART == 1) {
    // change following module in UART.c to configure UART0 for 8-bit (no
    // parity, One stop) and 9600 baud
    UART_Init(); // Initialize UART

    // No need to change the following line- if done correctly then Putty Should
    // display the correct message
    UART_printf("UART Initialization complete"); // Instructor given function to
                                                 // print string
    Newline(); // Instructor given function to go to new line

    while (1) {
      // The code for Part 1 is here
    }
  }

  // Part 2 Begins
  else if (LAB_PART == 2) {
    DisableInterrupts(); // Disabling the interrupt during setup
    Switch_Init();       // Initialization for Switches

    // need to generate a 100 Hz sine wave
    // table size is 16, so need 100Hz*16=1.6 kHz interrupt
    // bus is 80MHz, so SysTick period is 50000kHz/1.6kHz = 31250

    Sound_Init(31250);  // initialize SysTick timer, 100 Hz and DAC
    EnableInterrupts(); // enable after all initialization are done

    // If DAC is initalized completely then, you will be able to here 100 Hz
    // tone on Headphone
    for (i = 0; i < 8; i++) {
      DAC_Out(i);
    }

    while (1) {
      // The code for Part 2C comes here
    }
  }

  // Part 3 Begins
  else if (LAB_PART == 3) {

    // Initalize UART
    // Initialize DAC

    while (1) {
      // The code should demonstrate Part 3
    }
  }

  // Shouldn't reach unless you set an incorrect value for LAB_PART
  else
    return 0;
}

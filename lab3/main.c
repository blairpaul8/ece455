// Lab 3: UART and DAC Interfacing
// ECE 455 / ECE 555, Embedded Systems Design, Fall 2026
// Team Members name: Paul Blair; Benjamin Drumwright
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
#include <stdint.h>

// Change this number to the current part of the lab you are working on
#define LAB_PART 1 // 1 for Part1, 2 for Part 2, 3 for Part 3

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

uint32_t convert_input(char a, uint32_t input);

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

    UART_printf("UART Initialization complete"); // Instructor given function

    Newline(); // Instructor given function to go to new line

    while (1) {
      // The code for Part 1 is here
      UART_printf("Select an option below.");
      Newline();
      UART_printf("0    Convert millimeter to centimeter");
      Newline();
      UART_printf("1    Convert centimeter to meter");
      Newline();
      UART_printf("2    Convert meter to centimeter");
      Newline();
      Newline();

      char selection = UART_InChar();

      UART_OutChar(selection);

      Newline();

      UART_printf("Input a value to convert >> ");

      uint32_t input = UART_inUDec();
      Newline();

      uint32_t output = convert_input(selection, input);
      UART_OutUDec(output);

      Newline();
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
      unsigned long sw = Switch_In();

      if (sw == 0x00) { // no press, no sound
        //
        NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;

      } else if (sw == 0x01) { // SW1 only, 1 kHz

        NVIC_ST_RELOAD_R =
            3125 - 1; // 3125 = 1KHZ period, 50,000,000 / (1000 * 16)
        NVIC_ST_CURRENT_R = 0;
        NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
      } else if (sw == 0x02) { // SW2 only, 2kHz

        NVIC_ST_RELOAD_R =
            1563 - 1; // 1563 = 2kHz period, 50,000,000 / (2000 * 16)
        NVIC_ST_CURRENT_R = 0;
        NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;

      } else { // both pressed, no sound

        NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
      }
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

uint32_t convert_input(char a, uint32_t input) {
  uint32_t retval = 0xFF;
  switch (a) {
  case '0':
    // convert millimeter to centimeter
    retval = input * .10;
    break;
  case '1':
    retval = input / 100;
    break;
  case '2':
    retval = input * 100;
    break;
  default:
    UART_printf("Invalid argument to convert_input.");
    break;
  }
  return retval;
}

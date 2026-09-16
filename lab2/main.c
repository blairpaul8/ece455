#include "PortAD.h"
#include "SysTick.h"
#include "PLL.h"
#include "part2b.h"
#include "stdint.h"
#include <stdint.h>
// #include "tm4c123gh6pm.h"

// Change this number to the current part of the lab you are working on
#define LAB_PART                                                               \
  2 // 1 for Part2A, 2 for Part 2B, 3 for Part 3A, 4 for Part 3B (Without
    // overlapping), and 5 for Part 3B (With overlapping)

int main() {

  // call the port initialization function
  PortA_Init();
  PortD_Init();
  PortF_Init();

  if (LAB_PART == 1) {
    // This should demonstrate the functionality for Part 2A
    SysTick_Init(); // Function to initialize the SysTick

    while (1) {
      // This should demonstrate the functionality for Part 2A
      // call appropriate delay
      // GPIO_PORTA_DATA_R &=  	// Turn off when not testing
      //
      if (switch_state(SW1) == 1 && switch_state(SW2) == 1) {

        led_on(GREEN);
        SysTick_Delay1s_16MHz();
        led_off(GREEN);
        SysTick_Delay1s_16MHz();

      } else if (switch_state(SW1) == 0 && switch_state(SW2) == 1) {

        led_on(RED);
        SysTick_Delay1s_16MHz();
        led_off(RED);
        SysTick_Delay1s_16MHz();

      } else if (switch_state(SW1) == 1 && switch_state(SW2) == 0) {

        // turn on RED, YELLOW, GREEN
        led_on(RED);
        SysTick_Delay1s_16MHz();
        led_on(YELLOW);
        SysTick_Delay1s_16MHz();
        led_on(GREEN);
        SysTick_Delay1s_16MHz();

        // all led off
        led_off(RED);
        led_off(YELLOW);
        led_off(GREEN);
        SysTick_Delay1s_16MHz();

      } else if (switch_state(SW1) == 0 && switch_state(SW2) == 0) {
        // flash on RYG
        led_on(RED);
        led_on(YELLOW);
        led_on(GREEN);

        SysTick_Delay1s_16MHz();

        // turn all off
        led_off(RED);
        led_off(YELLOW);
        led_off(GREEN);

        SysTick_Delay1s_16MHz();
      }
    }
  }

  else if (LAB_PART == 2) {
    // This should demonstrate the functionality for Part 2B
    PLL_Init();
    SysTick_Init_Interrupts();

    while (1) {
      // Do nothing except wait for a SysTick Interrupt and action taken care at
      // interrupt handler routine
    }
  }

  else if (LAB_PART == 3) {
    // Write the code that demonstrate the functionality for Part 3A
  }

  else if (LAB_PART == 4) {
    // This should demonstrate the functionality for Part 3B
    // Write the code below for "1011" Sequence detectror - without overalapping

  }

  else if (LAB_PART == 5) {
    // This should demonstrate the functionality for Part 3B
    // Write the code below for "1011" Sequence detectror - with overalapping
    uint8_t pattern = 0;
    uint8_t input = 0;
    if (switch_state(SW1)) {
      input = 0;
    } else if (switch_state(SW2)) {
      input = 1;
    }

    pattern <<= 1;
    pattern |= input;

    // & pattern with 0x0F to only check bottom 4 bits.
    if ((pattern & 0x0F) == 11) {
      led_off(RED);
      led_on(GREEN);
    } else {
      led_off(GREEN);
      led_on(RED);
    }
  }

  // Shouldn't reach unless you set an incorrect value for LAB_PART
  else
    return 0;
}

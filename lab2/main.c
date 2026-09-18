#include "PortAD.h"
#include "SysTick.h"
#include "PLL.h"
#include "part2b.h"
#include "stdint.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

// Change this number to the current part of the lab you are working on
#define LAB_PART                                                               \
  4 // 1 for Part2A, 2 for Part 2B, 3 for Part 3A, 4 for Part 3B (Without
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
    PLL_Init();
    SysTick_Init();

    struct State {
      uint32_t Out;
      uint32_t Time;
      const struct State *Next[4];
    };

    typedef const struct State STyp;

    #define G &FSM[0]
    #define Y &FSM[1]
    #define R &FSM[2]

    STyp FSM[3] = {
      {0x10, 300, {G, Y, G, Y}},
      {0x08, 100, {R, R, R, R}},
      {0x04, 300, {R, R, G, G}},
    };

    STyp *Pt = G;
    while(1){
      uint32_t Light;
      Light = Pt->Out;

      GPIO_PORTA_DATA_R &= ~0x1C;
      GPIO_PORTA_DATA_R |= Light;

      for (uint32_t t = 0; t < Pt->Time / 100; t++) {
        SysTick_Delay1s_25MHz();
      }

      int Input;
      // uses switches as binary input
      if (switch_state(SW1) == 1 && switch_state(SW2) == 1) Input = 0;
      if (switch_state(SW1) == 1 && switch_state(SW2) == 0) Input = 1;
      if (switch_state(SW1) == 0 && switch_state(SW2) == 1) Input = 2;
      if (switch_state(SW1) == 0 && switch_state(SW2) == 0) Input = 3;
      Pt = Pt->Next[Input];

    }
  }

  else if (LAB_PART == 4) {
    // This should demonstrate the functionality for Part 3B
    // Write the code below for "1011" Sequence detectror - without overalapping
    PLL_Init();
    SysTick_Init_Interrupts();
    change_interrupt(0);

    while (1) {
      // wait for interrupts
    }

  }

  else if (LAB_PART == 5) {
    // This should demonstrate the functionality for Part 3B
    // Write the code below for "1011" Sequence detectror - with overalapping
    PLL_Init();
    SysTick_Init_Interrupts();
    change_interrupt(1);

    while (1) {
      // wait for interrupts
    }
  }
  // Shouldn't reach unless you set an incorrect value for LAB_PART
  else
    return 0;
}

// Programming Assignment 1: Getting Started
// Team Members name: <Student1>; <Student2>
// Due Date: Sept 4, 2026

#include "PortF.h"
#include "IO.h"
#include "stdint.h"
#include <stdint.h>

// Change this number to the current part
#define PART 1 // 1 for PART3B, 2 for PART3C , 3 for PART4

#define SWITCH_1 push_buttons(SW1)
#define SWITCH_2 push_buttons(SW2)

#define HIGH 1
#define LOW 0

int main() {
  PortF_Init();
  systick_init();

  if (PART == 1) { // Should Execute only for PART 3B

    while (1) {

      // you may need to call your functions created IO.c file

      // when both switches are unpressed toggle red led
      while ((SWITCH_1 == HIGH) && (SWITCH_2 == HIGH)) {
        // when one switch is released turn red off and break out of the loop
        if (SWITCH_1 == LOW || SWITCH_2 == LOW) {
          control_led(RED, OFF);
          break;
        }
        toggle_led(RED);
      }

      // when both are pressed
      while ((SWITCH_1 == LOW) && (SWITCH_2 == LOW)) {

        control_all_leds(OFF);
        delay_1ms(500);
        control_all_leds(ON);

        // when a switch is released we need to break out
        if (SWITCH_1 == HIGH || SWITCH_2 == HIGH) {
          control_all_leds(OFF);
          break;
        }

        delay_1ms(500);

        control_led(GREEN, OFF);

        if (SWITCH_1 == HIGH || SWITCH_2 == HIGH) {
          control_all_leds(OFF);
          break;
        }

        delay_1ms(500);
        control_led(BLUE, OFF);
        control_led(GREEN, ON);

        if (SWITCH_1 == HIGH || SWITCH_2 == HIGH) {
          control_all_leds(OFF);
          break;
        }

        delay_1ms(500);
        control_all_leds(OFF);

        delay_1ms(500);
      }

      // if only sw1 is pressed turn on green led
      if (SWITCH_1 == LOW && SWITCH_2 == HIGH) {
        control_led(GREEN, ON);
      }
      control_led(GREEN, OFF);

      // if only sw2 is pressed turn on blue led
      if (SWITCH_1 == HIGH && SWITCH_2 == LOW) {
        control_led(BLUE, ON);
      }
      control_led(BLUE, OFF);
    }
  } else if (PART == 2) { // Should Execute only for PART 3C

    while (1) {
      // Write the code that demonstrate PART3B functionality

      control_BS_led(RED, ON);
      control_BS_led(GREEN, OFF);

      delay_1ms(500);

      control_BS_led(GREEN, ON);
      control_BS_led(RED, OFF);

      delay_1ms(500);

      // count up to 3
      while (SWITCH_1 == LOW && SWITCH_2 == HIGH) {
        control_all_leds(OFF);
        delay_1ms(500);

        // red on green off 0b01
        control_BS_led(RED, ON);
        delay_1ms(500);

        // red off green on 0b10
        control_BS_led(RED, OFF);
        control_BS_led(GREEN, ON);
        delay_1ms(500);

        // red on and green on 0b11
        control_BS_led(RED, ON);
        delay_1ms(500);
      }

      // count down from 3
      while (SWITCH_1 == HIGH && SWITCH_2 == LOW) {
        // off is 0b00
        control_all_leds(OFF);
        delay_1ms(500);

        // red on and green on 0b11
        control_BS_led(GREEN, ON);
        control_BS_led(RED, ON);
        delay_1ms(500);

        // red off green on 0b10
        control_BS_led(RED, OFF);
        control_BS_led(GREEN, ON);
        delay_1ms(500);

        // red on green off 0b01
        control_BS_led(GREEN, OFF);
        control_BS_led(RED, ON);
        delay_1ms(500);
      }
    }
  }

  else if (PART == 3) { // Should Execute only for PART 4
    // init port a for external led
    PortA_init();

    while (1) {
      // Write the code that demonstrate PART4 functionality

      // toggle external led
      control_external_led(ON);
      delay_1ms(500);
      control_external_led(OFF);
      delay_1ms(500);
    }
  }
  // Shouldn't reach unless you set an incorrect value for LAB_PART
  else
    return 0;
}

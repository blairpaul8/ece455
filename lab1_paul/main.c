// Programming Assignment 1: Getting Started
// Team Members name: <Student1>; <Student2>
// Due Date: Sept 4, 2026

#include "PortF.h"
#include "IO.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

// Change this number to the current part
#define PART 2 // 1 for PART3B, 2 for PART3C , 3 for PART4

int main() {
  PortF_Init();
  systick_init();

  if (PART == 1) { // Should Execute only for PART 3B

    while (1) {
      // you may need to call your functions created IO.c file
      while (push_button_state(LEFT) == 1 && push_button_state(RIGHT) == 1) {
        if (push_button_state(LEFT) == 0 || push_button_state(RIGHT) == 0) {
          control_led(RED, OFF);
          break;
        }
        toggle_led(RED);
      }
      while ((push_button_state(LEFT) == 0) &&
             (push_button_state(RIGHT) == 0)) {

        control_all_leds(OFF);
        delay_1ms(500);
        control_all_leds(ON);

        if (push_button_state(LEFT) == 1 || push_button_state(RIGHT) == 1) {
          control_all_leds(OFF);
          break;
        }

        delay_1ms(500);

        control_led(GREEN, OFF);

        if (push_button_state(LEFT) == 1 || push_button_state(RIGHT) == 1) {
          control_all_leds(OFF);
          break;
        }

        delay_1ms(500);
        control_led(BLUE, OFF);
        control_led(GREEN, ON);

        if (push_button_state(LEFT) == 1 || push_button_state(RIGHT) == 1) {
          control_all_leds(OFF);
          break;
        }

        delay_1ms(500);
        control_all_leds(OFF);

        delay_1ms(500);
      }

      if (push_button_state(LEFT) == 0 && push_button_state(RIGHT) == 1) {
        control_led(GREEN, ON);
      } else {
        control_led(GREEN, OFF);
      }

      if (push_button_state(RIGHT) == 0 && push_button_state(LEFT) == 1) {
        control_led(BLUE, ON);
      } else {
        control_led(BLUE, OFF);
      }
    }
  } else if (PART == 2) { // Should Execute only for PART 3C

    while (1) {
      // Write the code that demonstrate PART3B functionality

      control_BS_led(RED, ON);
      control_BS_led(GREEN, OFF);

      delay_1ms(1000);

      control_BS_led(GREEN, ON);
      control_BS_led(RED, OFF);

      delay_1ms(1000);

      while (push_button_state(LEFT) == 0 && push_button_state(RIGHT) == 1) {
        control_all_leds(OFF);
        delay_1ms(1000);

        // red on green off 0b01
        control_BS_led(RED, ON);
        delay_1ms(1000);

        // red off green on 0b10
        control_BS_led(RED, OFF);
        control_BS_led(GREEN, ON);
        delay_1ms(1000);

        // red on and green on 0b11
        control_BS_led(RED, ON);
        delay_1ms(1000);
      }

      while (push_button_state(LEFT) == 1 && push_button_state(RIGHT) == 0) {
        // off is 0b00
        control_all_leds(OFF);
        delay_1ms(1000);

        // red on and green on 0b11
        control_BS_led(GREEN, ON);
        control_BS_led(RED, ON);
        delay_1ms(1000);

        // red off green on 0b10
        control_BS_led(RED, OFF);
        control_BS_led(GREEN, ON);
        delay_1ms(1000);

        // red on green off 0b01
        control_BS_led(GREEN, OFF);
        control_BS_led(RED, ON);
        delay_1ms(1000);
      }
    }
  }

  else if (PART == 3) { // Should Execute only for PART 4
    //
    PortA_init();

    while (1) {
      // Write the code that demonstrate PART4 functionality
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

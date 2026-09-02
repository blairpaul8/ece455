// Programming Assignment 1: Getting Started
// Team Members name: <Student1>; <Student2>
// Due Date: Sept 4, 2026

#include "PortF.h"
#include "IO.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

// Change this number to the current part
#define PART 1 // 1 for PART3B, 2 for PART3C , 3 for PART4

int main() {
  PortF_Init();
  systick_init();

  if (PART == 1) { // Should Execute only for PART 3B

    while (1) {
      // you may need to call your functions created IO.c file
      while (left_push_button() == 1 && right_push_button() == 1) {
        if (left_push_button() == 0 || right_push_button() == 0) {
          control_led(RED, OFF);
          break;
        }
        toggle_red();
      }
      while ((left_push_button() == 0) && (right_push_button() == 0)) {
        LEDs_off();
        delay_1ms(250);
        delay_1ms(250);
        ALL_on();
        if (left_push_button() == 1 || right_push_button() == 1) {
          LEDs_off();
          break;
        }
        delay_1ms(250);
        delay_1ms(250);
        Green_off();
        if (left_push_button() == 1 || right_push_button() == 1) {
          LEDs_off();
          break;
        }
        delay_1ms(250);
        delay_1ms(250);
        Blue_off();
        Green_on();
        if (left_push_button() == 1 || right_push_button() == 1) {
          LEDs_off();
          break;
        }
        delay_1ms(250);
        delay_1ms(250);
        LEDs_off();
        delay_1ms(250);
        delay_1ms(250);
      }

      if (left_push_button() == 0 && right_push_button() == 1) {
        Green_on();
      } else {
        Green_off();
      }

      if (right_push_button() == 0 && left_push_button() == 1) {
        Blue_on();
      } else {
        Blue_off();
      }
    }
    */
  }
  /*
    else if (PART == 2) { // Should Execute only for PART 3C

      while (1) {
        // Write the code that demonstrate PART3B functionality

        // while ((left_push_button() == 0 && right_push_button() == 0) ||
        //       (left_push_button() == 1 && right_push_button() == 1)) {
        Red_BS_ON();
        Green_BS_OFF();
        delay_1ms(250);
        delay_1ms(250);
        Green_BS_ON();
        Red_BS_OFF();
        delay_1ms(250);
        delay_1ms(250);
        //}

        while (left_push_button() == 0 && right_push_button() == 1) {
          // off is 0b00
          LEDs_off();
          delay_1ms(250);
          // red on green off 0b01
          Red_BS_ON();
          delay_1ms(250);
          delay_1ms(250);
          // red off green on 0b10
          Red_BS_OFF();
          Green_BS_ON();
          delay_1ms(250);
          delay_1ms(250);
          // red on and green on 0b11
          Red_BS_ON();
          delay_1ms(250);
          delay_1ms(250);
        }

        while (left_push_button() == 1 && right_push_button() == 0) {
          // off is 0b00
          LEDs_off();
          delay_1ms(250);
          // red on and green on 0b11
          Green_BS_ON();
          Red_BS_ON();
          delay_1ms(250);
          delay_1ms(250);
          // red off green on 0b10
          Red_BS_OFF();
          Green_BS_ON();
          delay_1ms(250);
          delay_1ms(250);
          // red on green off 0b01
          Green_BS_OFF();
          Red_BS_ON();
          delay_1ms(250);
          delay_1ms(250);
        }
      }
    }

    else if (PART == 3) { // Should Execute only for PART 4
      //
      PortA_init();

      while (1) {
        // Write the code that demonstrate PART4 functionality
        External_Led_on();
        delay_1ms(250);
        External_Led_off();
        delay_1ms(250);
      }
    }
    */
  // Shouldn't reach unless you set an incorrect value for LAB_PART
}
return 0;
}

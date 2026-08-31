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

  if (PART == 1) { // Should Execute only for PART 3B

    while (1) {
      // you may need to call your functions created IO.c file

      while ((left_push_button() == 0) && (right_push_button() == 0)) {
        Blue_on();
        Red_on();
        Green_on();
        if (left_push_button() == 1 || right_push_button() == 1) {
          LEDs_off();
          break;
        }
        delay();
        delay();
        Green_off();
        if (left_push_button() == 1 || right_push_button() == 1) {
          LEDs_off();
          break;
        }
        delay();
        delay();
        Blue_off();
        Green_on();
        if (left_push_button() == 1 || right_push_button() == 1) {
          LEDs_off();
          break;
        }
        delay();
        delay();
        LEDs_off();
        delay();
        delay();
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
  }

  else if (PART == 2) { // Should Execute only for PART 3C

    while (1) {
      // Write the code that demonstrate PART3B functionality

      // while ((left_push_button() == 0 && right_push_button() == 0) ||
      //       (left_push_button() == 1 && right_push_button() == 1)) {
      Red_BS_ON();
      Green_BS_OFF();
      delay();
      delay();
      Green_BS_ON();
      Red_BS_OFF();
      delay();
      delay();
      //}

      while (left_push_button() == 0 && right_push_button() == 1) {
        // off is 0b00
        LEDs_off();
        delay();
        // red on green off 0b01
        Red_BS_ON();
        delay();
        delay();
        // red off green on 0b10
        Red_BS_OFF();
        Green_BS_ON();
        delay();
        delay();
        // red on and green on 0b11
        Red_BS_ON();
        delay();
        delay();
      }

      while (left_push_button() == 1 && right_push_button() == 0) {
        // off is 0b00
        LEDs_off();
        delay();
        // red on and green on 0b11
        Green_BS_ON();
        Red_BS_ON();
        delay();
        delay();
        // red off green on 0b10
        Red_BS_OFF();
        Green_BS_ON();
        delay();
        delay();
        // red on green off 0b01
        Green_BS_OFF();
        Red_BS_ON();
        delay();
        delay();
      }
    }
  }

  else if (PART == 3) { // Should Execute only for PART 4

    while (1) {
      // Write the code that demonstrate PART4 functionality
    }
  }
  // Shouldn't reach unless you set an incorrect value for LAB_PART
  else
    return 0;
}

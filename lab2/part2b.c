#include "part2b.h"
#include "tm4c123gh6pm.h"

#include <stdint.h>

void led_on(uint32_t pin) { GPIO_PORTA_DATA_R |= (1 << pin); }

void led_off(uint32_t pin) { GPIO_PORTA_DATA_R &= ~(1 << pin); }

uint32_t switch_state(uint32_t switch_num) {

  uint32_t state = 0xFF; // Initialize to invalid value

  if (switch_num == SW1) {

    // if pin is high return 1 else return 0
    state = (GPIO_PORTD_DATA_R & SW1_PRESSED) ? 1 : 0;

  } else if (switch_num == SW2) {

    // if pin is high return 1 else return 0
    state = (GPIO_PORTD_DATA_R & SW2_PRESSED) ? 1 : 0;
  }

  return state;
}

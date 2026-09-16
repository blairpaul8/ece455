#include "part2b.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"

#include <stdint.h>

void led_on(uint32_t pin) { GPIO_PORTA_DATA_R |= (1 << pin); }

void led_off(uint32_t pin) { GPIO_PORTA_DATA_R &= ~(1 << pin); }

uint32_t switch_state(uint32_t switch_num) {
  uint32_t state = 0xFF; // Initialize to invalid value

  if (switch_num == SW1) {
    // TODO: switch back to port D
    state = (GPIO_PORTF_DATA_R & SW1_PRESSED) ? 1 : 0;

  } else if (switch_num == SW2) {
    // TODO: switch back to port D
    state = (GPIO_PORTF_DATA_R & SW2_PRESSED) ? 1 : 0;
  }

  return state;
}

void count_up() {
  // all off 000   0
  SysTick_Delay1s_25MHz();

  // red on 001   1
  led_on(RED);
  SysTick_Delay1s_25MHz();

  // yellow on 010   2
  led_off(RED);
  led_on(YELLOW);
  SysTick_Delay1s_25MHz();

  // red and yellow on 011    3
  led_on(RED);
  SysTick_Delay1s_25MHz();

  // green on 100    4
  led_off(RED);
  led_off(YELLOW);
  led_on(GREEN);
  SysTick_Delay1s_25MHz();

  // red and green on 101   5
  led_on(RED);
  SysTick_Delay1s_25MHz();

  // yeallow and green on 110   6
  led_off(RED);
  led_on(YELLOW);
  SysTick_Delay1s_25MHz();

  // all on 111   6
  led_on(RED);
  SysTick_Delay1s_25MHz();
}

void count_down() {
  // Update function
  int i = 0;
  i++;
}

/*
void toggle_leds() {
  // make sure all leds are off.
  led_off(RED);
  led_off(YELLOW);
  led_off(GREEN);

  // red on
  led_on(RED);
  SysTick_Delay1s_25MHz();

  // yellow on
  led_off(RED);
  led_on(YELLOW);
  SysTick_Delay1s_25MHz();

  // toggle green on
  led_off(YELLOW);
  led_on(GREEN);
  SysTick_Delay1s_25MHz();
}

*/

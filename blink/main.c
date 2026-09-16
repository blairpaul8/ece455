#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

// Systick Values
#define CLOCK_HZ 16000000
#define CYCLES_PER_MS (CLOCK_HZ / 1000)

void systick_init(void) {
  // disable systick during setup
  NVIC_ST_CTRL_R = 0;

  // Max reload value
  NVIC_ST_RELOAD_R = 0x00FFFFFF;

  // write to clear current register
  NVIC_ST_CURRENT_R = 0;

  // enable systick
  NVIC_ST_CTRL_R = 0x00000005;
}

void systick_wait(uint32_t delay) {
  // num ticks to wait
  NVIC_ST_RELOAD_R = delay - 1;

  // write to clear current register
  NVIC_ST_CURRENT_R = 0;

  // wait for count flag
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {
  }
}

// A General Purpose Delay
void delay_1ms(uint32_t delay) {
  uint32_t i;
  for (i = 0; i < delay; i++) {
    systick_wait(CYCLES_PER_MS);
  }
}

int main() {
  // Enable the GPIOF peripheral
  SYSCTL_RCGCGPIO_R |= (1 << 5);

  // Wait for GPIOF to be ready
  while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0) {
  }

  // initialize PF1 as digital
  GPIO_PORTF_DEN_R |= 0x2;

  // set PF1 to output
  GPIO_PORTF_DATA_R |= 0x2;

  while (1) {
    GPIO_PORTF_DATA_R |= (1 << 1);
    delay_1ms(1000);
    GPIO_PORTF_DATA_R &= ~(1 << 1);
    delay_1ms(1000);
  }
  return 0;
}

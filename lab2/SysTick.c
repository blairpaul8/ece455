#include "SysTick.h"
#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "part2b.h"
#include <stdint.h>

#define CLOCK_25MHZ 25000000
#define CYCLES_PER_MS_25MHZ 25000
#define CLOCK_16MHZ 16000000

#define RED_TICKS 200 // 1s on 1s off > 2000ms / 10ms
#define YELLOW_TICKS 300
#define GREEN_TICKS 400

// Used in part 2B
volatile uint32_t g_handler_calls;

// Initialize Systick
void SysTick_Init(void) {
  // disable systick during setup
  NVIC_ST_CTRL_R = 0;

  NVIC_ST_RELOAD_R = 0x00FFFFFF;

  NVIC_ST_CURRENT_R = 0;

  // enable systick
  NVIC_ST_CTRL_R = 0x00000005;
}

// Configure SysTick to generate an interrupt every 10ms, used in part 2B
// Assume 16 MHz clock
void SysTick_Init_Interrupts(void) {
  g_handler_calls = 0; // Initialize counter as 0
  NVIC_ST_CTRL_R = 0;

  NVIC_ST_RELOAD_R = 249999; // (0.010 s * 25,000,000hz) - 1

  NVIC_ST_CURRENT_R = 0;

  // enable systick
  NVIC_ST_CTRL_R = 0x00000007;
}

void SysTick_Wait(uint32_t delay) {}

// Clock speed is 16 MHz
static void SysTick_Delay100us_16MHz(void) {
  // Use the Systick Timer to generate a 1ms delay

  // Choose the number of clock ticks to wait
  NVIC_ST_RELOAD_R = (CLOCK_16MHZ * 0.0001) -1;

  NVIC_ST_CURRENT_R = 0; // Any value written to write clears it
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {
  } // Wait for count flag
}

// Write code to generate a 1 sec delay
// Your code should call SysTick_Delay1ms()
void SysTick_Delay1s_16MHz(void) {
  for(int i = 0; i < 10000; i++){
    SysTick_Delay100us_16MHz();
  }
}

// Write code to generate 1ms delay assuming a clock speed of 25MHz
static void SysTick_Delay100us_25MHz(void) {
  // clock at 25MHz has 25,000,000 cycles per second
  // to get a 100us delay multiply by .0001
  NVIC_ST_RELOAD_R = (CLOCK_25MHZ * .0001) - 1;

  NVIC_ST_CURRENT_R = 0;

  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {
  }
}

// Write code to generate a 1 sec delay when the clock speed is 25MHz
// Your code should call SysTick_Delay100us_25MHz()
void SysTick_Delay1s_25MHz(void) {
  // 1,000,000 us in 1 second
  // Thus call delay 100us 10,000 times
  for (uint32_t i = 0; i < 10000; i++) {
    SysTick_Delay100us_25MHz();
  }
}

// Interrupt handling routine should be written here
void SysTick_Handler(void) {
  g_handler_calls++;
  if (switch_state(SW1) == 0) {
    count_up();
  } else if (switch_state(SW2) == 0) {
    // count down
  } else {
    uint32_t red = g_handler_calls % RED_TICKS;
    uint32_t yellow = g_handler_calls % YELLOW_TICKS;
    uint32_t green = g_handler_calls % GREEN_TICKS;

    (red < 100) ? led_on(RED) : led_off(RED);
    (yellow < 100) ? led_on(YELLOW) : led_off(YELLOW);
    (green < 100) ? led_on(GREEN) : led_off(GREEN);
    
  }

}

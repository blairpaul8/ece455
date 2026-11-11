// Servo.c
// Lab 5, Part 3: PWM to an SG90 servo (3A) and input capture of the chip's own
// PWM output (3B).
// ECE 455 / ECE 555, Fall 2026.
//
// Hardware
//   Bus clock 80 MHz (lab5.c calls PLL_Init(Bus80MHz)).
//   PB6  M0PWM0, PWM out to the servo signal wire (orange).
//   PB4  T1CCP0, input capture; a jumper wire from PB6.
//   Servo power: red to VBUS (5 V), brown to GND. NEVER to the 3.3 V rail.
//   The LCD from Part 1 is available for displaying your measurements
//   (ST7735_SetCursor, ST7735_OutString, ST7735_OutUDec).
//
// Sources to read first
//   Servo limits: Volume 2 Section 6.8, Lab 6.4, PDF page 456.
//   PWM module: Volume 2 Section 6.3, Program 6.8, PDF pages 425 to 428.
//   Input capture: Volume 2 Section 6.1 and 6.1.4, PDF pages 401 and 414.
// lab5.c calls Servo_Main() when its LAB_PART is 3.

#include <stdint.h>
#include "Servo.h"
#include "ST7735.h"
#include "tm4c123gh6pm.h"

void DelayWait10ms(uint32_t n);   // in lab5.c, busy-wait, n times 10 ms

// ======================= Part 3A: PWM =======================
// Suggested clocking: PWM clock = 80 MHz / 64 = 1.25 MHz, one tick is 0.8 us,
// so the 20 ms period is 25,000 ticks and 1.5 ms is 1875 ticks.
#define PWM_TICK_NS       800
#define PWM_PERIOD_TICKS  (SERVO_PERIOD_US*1000/PWM_TICK_NS)   // 25000
// PWM0 generator 0, output A on PB6. See Program 6.8.
// Input: period and duty in PWM clock ticks, duty < period
void PWM0_Init(uint16_t period, uint16_t duty){
  // 1) activate PWM0 and port B
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
  // 2) PB6: alternate function, PCTL = M0PWM0, digital enable
  GPIO_PORTB_AFSEL_R |= 0x40;           // enable alt funct on PB6
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xF0FFFFFF)+0x04000000; // PWM0
  GPIO_PORTB_AMSEL_R &= ~0x40;          // disable analog fun on PB6
  GPIO_PORTB_DEN_R |= 0x40;             // enable digital I/O on PB6
  // 3) SYSCTL_RCC_R: use the PWM divider, choose the divisor
  SYSCTL_RCC_R = 0x001A0000 |           // 3) use PWM divider
    (SYSCTL_RCC_R & (~0x000E0000));     //    configure for /64 divider
  // 4) generator 0: down-counting mode; GENA low on LOAD, high on CMPA down
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // 5) LOAD = period - 1;
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  // 6) CMPA = duty - 1
  PWM0_0_CMPA_R = duty - 1;             // 6) count value when output rises
  // 7) start generator 0 and enable the M0PWM0 output
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000001;          // enable PB6/M0PWM0
}

// Set the pulse width in microseconds. Enforce SERVO_MIN_US and SERVO_MAX_US here,
// before the value reaches the register. Returns the width actually applied.
uint32_t PWM0_SetPulseWidth(uint32_t microseconds){
  if(microseconds < SERVO_MIN_US){
    microseconds = SERVO_MIN_US;
  }
  if(microseconds > SERVO_MAX_US){
    microseconds = SERVO_MAX_US;
  }

  PWM0_0_CMPA_R = (microseconds*1000/PWM_TICK_NS) - 1; // 6) count value when output rises

  return microseconds;
}

// ======================= Part 3B: input capture =======================
// Timer1A on PB4 (T1CCP0), edge-time mode, both edges. Section 6.1.4.

void Capture_Init(void){
  // 1) activate Timer1 and port B; PB4 input, alternate function, PCTL = T1CCP0
  SYSCTL_RCGCTIMER_R |= 0x02;      // activate timer1
  SYSCTL_RCGCGPIO_R |= 0x02;       // activate port B
  First = 0;                       // first will be wrong
  Count = 0;                       // set on subsequent
  GPIO_PORTB_DIR_R &= ~0x10;       // make PB4 input
  GPIO_PORTB_AFSEL_R |= 0x10;      // enable alt funct on PB4
  GPIO_PORTB_DEN_R |= 0x10;        // configure PB4 as T1CCP0
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFF0FFFF)+0x00070000;

  // 2) disable Timer1A; 16-bit mode; TAMR: capture, edge-time, count direction
  TIMER1_CTL_R &= ~0x00000001;     // disable timer1A during setup
  TIMER1_CFG_R = 0x00000004;       // configure for 16-bit capture mode
  TIMER1_TAMR_R = 0x00000007;      // configure for edge event
  
  // 3) CTL: both edges; TAILR = 0xFFFF; TAPR = 0xFF (24-bit range)
  TIMER1_CTL_R |= 0x0000000C;      // both edges
  TIMER1_TAILR_R = 0x0000FFFF;     // start value
  TIMER1_TAPR_R = 0xFF;            // activate prescale, creating 24-bit

  // 4) enable the capture-event interrupt (IMR), clear ICR
  TIMER1_IMR_R |= 0x00000004;      // enable capture match interrupt
  TIMER1_ICR_R = 0x00000004;       // clear timer1A capture match flag

  // 5) NVIC: Timer1A is interrupt 21 (NVIC_EN0_R bit 21), priority in NVIC_PRI5_R
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00004000; // Timer1A=priority 2
  NVIC_EN0_R = 1<<21;              // enable interrupt 21 in NVIC

  // 6) enable Timer1A
  TIMER1_CTL_R |= 0x00000001;      // timer1A 24-b, both edges, interrupts
  EnableInterrupts();
}

// Both edges arrive here. Decide which edge it was (read PB4), timestamp it,
// and compute the pulse width (rising to falling) and the period (rising to rising).
void Timer1A_Handler(void){
  // acknowledge the capture event (TIMER1_ICR_R)
  TIMER1_ICR_R = 0x00000004;       // acknowledge timer1A capture
  if(GPIO_PORTB_DATA_R&0x10){      // PB4 high, rising edge
    Period = (First - TIMER1_TAR_R)&0x00FFFFFF; // 12.5ns resolution
    First = TIMER1_TAR_R;          // setup for next
  } 
  
  else{                           // PB4 low, falling edge
    PulseWidth = (First - TIMER1_TAR_R)&0x00FFFFFF; // from rise to fall
    Count = Count + 1;
  }
}

uint32_t Capture_PulseWidth_us(void){    return 0; } // replace with your solution
uint32_t Capture_Period_us(void){        return 0; } // replace with your solution
uint32_t Capture_PulseWidth_ticks(void){ return 0; } // replace with your solution
uint32_t Capture_Count(void){            return 0; } // replace with your solution


// ======================= Part 3 main (instructor given, complete) =======================
// Called from lab5.c with LAB_PART 3. Do not change it.
//   LAB_STAGE 1: 3A only. Sweep the servo from 0.5 ms to 2.5 ms and back, about
//                two seconds each way, then hold 1.5 ms (the centre) forever.
//   LAB_STAGE 2: 3A, then 3B. After the sweep, hold 0.5, 1.5 and 2.5 ms for two
//                seconds each, forever, and show on the LCD what the input capture
//                measured: the commanded width, the measured width and period in
//                microseconds, the measured width in timer ticks, and the pulse count.
#define LAB_STAGE 1

// One LCD line: label, number, and spaces so a shorter number overwrites a longer one
static void ShowLine(uint32_t line, char *label, uint32_t value){
  ST7735_SetCursor(0, line);
  ST7735_OutString(label);
  ST7735_OutUDec(value);
  ST7735_OutString("    ");
}

void Servo_Main(void){ uint32_t us;
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetCursor(0, 0);
  ST7735_OutString("Lab 5 Part 3");
  PWM0_Init(PWM_PERIOD_TICKS, SERVO_CENTER_US*1000/PWM_TICK_NS);   // 25000 ticks, 1875 ticks
#if LAB_STAGE >= 2
  Capture_Init();
#endif
  // 3A: sweep from one limit to the other and back, 10 us every 10 ms
  for(us = SERVO_MIN_US; us <= SERVO_MAX_US; us += 10){ PWM0_SetPulseWidth(us); DelayWait10ms(1); }
  for(us = SERVO_MAX_US; us >= SERVO_MIN_US; us -= 10){ PWM0_SetPulseWidth(us); DelayWait10ms(1); }
  PWM0_SetPulseWidth(SERVO_CENTER_US);
#if LAB_STAGE == 1
  ST7735_SetCursor(0, 2);
  ST7735_OutString("3A sweep done");
  while(1){}
#endif
  // 3B: hold each of the three commanded widths for two seconds and display the measurement.
  while(1){ static const uint32_t test[3] = {SERVO_MIN_US, SERVO_CENTER_US, SERVO_MAX_US}; uint32_t i, t;
    for(i = 0; i < 3; i++){
      PWM0_SetPulseWidth(test[i]);
      for(t = 0; t < 20; t++){
        DelayWait10ms(10);
        ShowLine(2, "cmd us  ", test[i]);
        ShowLine(3, "meas us ", Capture_PulseWidth_us());
        ShowLine(4, "per us  ", Capture_Period_us());
        ShowLine(5, "ticks   ", Capture_PulseWidth_ticks());
        ShowLine(6, "pulses  ", Capture_Count());
      }
    }
  }
}

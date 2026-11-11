// Servo.h
// Lab 5, Part 3: PWM to an SG90 servo (3A) and input capture of the chip's own
// PWM output (3B).
// ECE 455 / ECE 555, Fall 2026.
#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

// ---- Part 3A: servo limits, Volume 2 Section 6.8, Lab 6.4 (PDF page 456) ----
#define SERVO_PERIOD_US    20000   // repeat the command every 20 ms (about 50 Hz)
#define SERVO_MIN_US         500   // shorter than 0.5 ms overdrives the servo
#define SERVO_CENTER_US     1500   // 1.5 ms commands the 90 degree position
#define SERVO_MAX_US        2500   // longer than 2.5 ms overdrives the servo

// PWM0 generator 0, output A on PB6 (M0PWM0). 80 MHz bus, PWM clock divided by 64.
// period and duty are in PWM clock ticks of 0.8 us.
void PWM0_Init(uint16_t period, uint16_t duty);
// Set the pulse width in microseconds, clamped to SERVO_MIN_US..SERVO_MAX_US.
// Returns the width actually applied.
uint32_t PWM0_SetPulseWidth(uint32_t microseconds);

// ---- Part 3B: input capture on PB4 (T1CCP0), both edges, 12.5 ns ticks ----
void Capture_Init(void);
uint32_t Capture_PulseWidth_us(void);    // most recent high time, microseconds
uint32_t Capture_Period_us(void);        // most recent rising-to-rising time, microseconds
uint32_t Capture_PulseWidth_ticks(void); // the same in 12.5 ns ticks, for the resolution question
uint32_t Capture_Count(void);            // number of complete pulses measured

// The Part 3 main, called from lab5.c when LAB_PART is 3. Does not return.
void Servo_Main(void);

#endif

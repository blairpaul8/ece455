// Main.c
// Lab 4, Part 2: slide potentiometer on PE2 / AIN1, sampled every 500 ms by a
// SysTick interrupt, converted to a fixed-point distance and printed on PuTTY.
// ECE 455 / ECE 555, Fall 2026.
//
// This project runs the PLL at 5 MHz (PLL.h, SYSDIV2 = 79). The given UART_Init
// divisors (115200 baud) and the SysTick reload passed in main both assume 5 MHz.
// Slide potentiometer: pin 1 to GND, pin 2 (wiper) to PE2, pin 3 to +3.3 V.

#include <stdint.h>
#include "PLL.h"
#include "ADCCode.h"
#include "UART.h"
#include "tm4c123gh6pm.h"

// Global declarations
unsigned long Size = 1000;                // potentiometer travel in mm; change it to match your part
unsigned char String[10];              // string to output to the screen
unsigned long Distance;                // potentiometer distance in units of 0.001 cm
unsigned long ADCvalue;                // 12-bit 0 to 4095 sample
volatile unsigned long Flag;           // 1 means Distance holds a new value, 0 means it has been printed

// Students need to write these functions
unsigned long Convert(unsigned long sample);
void SysTick_Init_Interrupts(unsigned long period);
void SysTick_Handler(void);

// Instructor given function
void ConvertDistancetostring(unsigned long n);

int main(void){
  PLL_Init();                          // PLL at 5 MHz
  UART_Init();                         // UART init, check IBRD and FBRD
  ADC0_InitSWTriggerSeq3_Ch1();        // ADC initialization, PE2 / AIN1
  SysTick_Init_Interrupts(2500000);    // pass the value that gives an interrupt every 500 ms
  while(1){
    // Is "Flag" set?
    // If yes, then clear Flag and print the value using UART_printf
    if(Flag){
      Flag = 0;
      UART_printf(String);
      UART_printf((unsigned char *)"\r\n");
    }
  }
}

/*------------ Student written functions ------------*/

//******** Convert ****************
// Convert a 12-bit binary ADC sample into a 32-bit unsigned fixed-point distance
// (resolution 0.001 cm). Calibration data is gathered using known distances and
// reading the ADC value measured on PE2. Overflow and dropout should be considered.
//   (sample / 4095) * Size, then multiplied so the result is in 0.001 cm units.
//   "Size" is a global variable, the length of the potentiometer's travel in mm.
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001 cm)
unsigned long Convert(unsigned long sample){
return (sample * Size * 100 + 4095/2) / 4095; 
}

// Initialize SysTick interrupts to trigger at 2 Hz, 500 ms.
// Input: 32-bit desired reload value
// Output: none
void SysTick_Init_Interrupts(unsigned long period){
  // disable SysTick during setup
  NVIC_ST_CTRL_R = 0;

  // reload value
  NVIC_ST_RELOAD_R = period - 1;

  // clear the current value
  NVIC_ST_CURRENT_R = 0;

  // set the priority 2
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000;

  // enable SysTick with the core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;
}

// Executes every 500 ms: collect a sample, convert it, store it in the mailbox.
// Use the handler to read the ADC value, call Convert(), then call the pre-written
// ConvertDistancetostring(), and set the global Flag before exiting.
void SysTick_Handler(void){

// read the 12-bit sample from PE2
ADCvalue = ADC0_In();

// convert to 0.001 cm units
Distance = Convert(ADCvalue);

// fill the global String[]
ConvertDistancetostring(Distance);

// tell main a new value is ready
 Flag = 1;
}

/*------------ Instructor written function ------------*/
// Converts a 32-bit distance into an ASCII string
// Input: 32-bit number to be converted (resolution 0.001 cm)
// Output: store the conversion in the global variable String[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//     4 to "0.004 cm"
//    31 to "0.031 cm"
//   102 to "0.102 cm"
//  2210 to "2.210 cm"
// 10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void ConvertDistancetostring(unsigned long n){
  int i;                               // used in for loops
  if(n > 9999){                        // 10000 to "*.*** cm"
    for(i = 1; i < 5; i++)
      String[i] = '*';
  }
  else{                                // get digits from least to most significant
    for(i = 4; i > 0; i--){
      String[i] = n % 10 + 0x30;
      n = n / 10;
    }
  }
  // Rest of String
  String[0] = String[1];
  String[1] = '.';
  String[5] = ' ';
  String[6] = 'c';
  String[7] = 'm';
  String[8] = ' ';
  String[9] = 0;
}

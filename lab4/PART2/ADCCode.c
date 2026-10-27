// ADCCode.c
// Lab 4, Part 2B. ECE 455 / ECE 555, Fall 2026.
//
// ADC0_InitSWTriggerSeq3_Ch1 is Part 2B (student written). ADC0_In is given.
// All the steps in the init are needed; write the register operation each step describes.

#include <stdint.h>
#include "ADCCode.h"
#include "tm4c123gh6pm.h"

//------------ADC0_InitSWTriggerSeq3_Ch1------------
// Part 2B. Configure ADC0 to read the voltage at PE2 (AIN1) at 125 K samples/sec
// using sequencer 3, software trigger.
// Input: none
// Output: none
void ADC0_InitSWTriggerSeq3_Ch1(void){
  // 1) activate ADC0

  // 2) activate clock for Port E
  
  // 3) allow time for the clock to stabilize
  
  // 4) make PE2 input
  
  // 5) enable alternate function on PE2
  
  // 6) disable digital I/O on PE2
  
  // 7) enable analog function on PE2
  
  // 8) configure for 125 K samples/sec (ADC0_PC_R)
  
  
  // 9) sequencer 3 is highest priority
  
  // 10) disable sample sequencer 3
  
  // 11) seq3 is software trigger
  
  // 12) channel AIN1 (PE2) (ADC0_SSMUX3_R)
  
  // 13) SSCTL3: no TS0, no D0, yes IE0 and END0
  
  // 14) disable SS3 interrupts
  
  // 15) enable sample sequencer 3

}

//------------ADC0_In------------
// Given. Busy-wait analog to digital conversion on sequencer 3.
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC0_In(void){ uint32_t result;
  ADC0_PSSI_R = 0x0008;                // 1) initiate SS3
  while((ADC0_RIS_R&0x08) == 0){};     // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;       // 3) read result
  ADC0_ISC_R = 0x0008;                 // 4) acknowledge completion
  return result;
}

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
  SYSCTL_RCGCADC_R |= 0x01;

  // 2) activate clock for Port  
  SYSCTL_RCGCGPIO_R |= 0x10;

  // 3) allow time for the clock to stabilize
  while((SYSCTL_PRGPIO_R & 0x10) == 0){}

  // 4) make PE2 input
  GPIO_PORTE_DIR_R &= ~0x04;
  
  // 5) enable alternate function on PE2
  GPIO_PORTE_AFSEL_R |= 0x04;
  
  // 6) disable digital I/O on PE2
  GPIO_PORTE_DEN_R &= ~0x04;
  
  // 7) enable analog function on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;
  
  // 8) configure for 125 K samples/sec (ADC0_PC_R)
  ADC0_PC_R &= ~0x0F;
  ADC0_PC_R |= 0x01;
  
  // 9) sequencer 3 is highest priority
  ADC0_SSPRI_R = 0x0123;

  // 10) disable sample sequencer 3
  ADC0_ACTSS_R &= ~0x08;

  // 11) seq3 is software trigger
  ADC0_EMUX_R &= ~0xF000;

  // 12) channel AIN1 (PE2) (ADC0_SSMUX3_R)
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R & ~0x000F) + 1;

  // 13) SSCTL3: no TS0, no D0, yes IE0 and END0
  ADC0_SSCTL3_R = 0x0006;

  // 14) disable SS3 interrupts
  ADC0_IM_R &= ~0x0008;

  // 15) enable sample sequencer 3
  ADC0_ACTSS_R |= 0x08;
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

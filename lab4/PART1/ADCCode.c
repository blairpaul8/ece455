#include <stdint.h>
#include "ADCCode.h"
#include "tm4c123gh6pm.h"

//------------ADC0_Temp_Init------------
// Part 1A. Configure ADC0 sequencer 3 to read the internal temperature sensor
// at 500 K samples/sec, software trigger.
void ADC0_Temp_Init(void){
  // We do not need to initialize a GPIO pin since we are measuring temperature.
  // 1) activate ADC0 and allow time to stabilize
  SYSCTL_RCGCADC_R |= 0x01;
  while((SYSCTL_PRADC_R & 0x01) == 0){}   // wait until ADC0 is ready

  // 2) configure for 500 K samples/sec (ADC0_PC_R)
  ADC0_PC_R = 0x05;

  // 3) sequencer 3 is highest priority (ADC0_SSPRI_R)
  ADC0_SSPRI_R = 0x0123;

  // 4) disable sample sequencer 3 during setup (ADC0_ACTSS_R)
  ADC0_ACTSS_R &= ~0x08;

  // 5) seq3 is software trigger (ADC0_EMUX_R)
  ADC0_EMUX_R &= ~0xF000;

  // No need to choose a channel since we are measuring temperature:
  // 6) SSCTL3: TS0 = 1 selects the temperature sensor, IE0 = 1, END0 = 1, D0 = 0
  ADC0_SSCTL3_R = 0x0E;
  
  // 7) enable sample sequencer 3 before we sample (ADC0_ACTSS_R)
  ADC0_ACTSS_R |= 0x08;

}

//------------ADC0_In------------
// Part 1A. Busy-wait analog to digital conversion on sequencer 3.
// Input: none
// Output: 12-bit result of the ADC conversion
uint32_t ADC0_In(void) { 
  uint32_t result = 0;

  // 1) initiate SS3 (ADC0_PSSI_R)
  ADC0_PSSI_R = 0x0008;

  // 2) wait for conversion done (ADC0_RIS_R bit 3)
  while ((ADC0_RIS_R & 0x08) == 0)  {}

  // 3) read result (ADC0_SSFIFO3_R, 12 bits)
  result =  ADC0_SSFIFO3_R & 0xFFF;

  // 4) acknowledge completion (ADC0_ISC_R)
  ADC0_ISC_R = 0x0008;
  
  return result;
}

//------------ADC0_Thermistor_Init------------
// Part 1C. Configure ADC0 sequencer 2 to read PE3 / AIN0, the midpoint of the
// thermistor voltage divider, software trigger. Sequencer 3 stays on the
// temperature sensor, so both readings are available in the same program.
// Call ADC0_Temp_Init first: it turns on the ADC clock and sets the sample rate.
// Input: none
// Output: none
void ADC0_Thermistor_Init(void){
  // 1) activate the clock for Port E and allow time to stabilize
  SYSCTL_RCGCGPIO_R |= 0x10;
  while((SYSCTL_PRGPIO_R & 0x10) == 0){}

  // 2) make PE3 an input
  GPIO_PORTE_DIR_R &= ~0x08;

  // 3) enable the alternate function on PE3
  GPIO_PORTE_AFSEL_R |= 0x08;

  // 4) disable digital I/O on PE3
  GPIO_PORTE_DEN_R &= ~0x08;

  // 5) enable the analog function on PE3
  GPIO_PORTE_AMSEL_R |= 0x08;

  // 6) disable sample sequencer 2 during setup
  ADC0_ACTSS_R &= ~0x04;

  // 7) seq2 is software trigger (ADC0_EMUX_R bits 11 to 8)
  ADC0_EMUX_R &= ~0x0F00;

  // 8) first sample of seq2 is channel AIN0 (ADC0_SSMUX2_R)
  ADC0_SSMUX2_R = (ADC0_SSMUX2_R & ~0x0F) | 0x00;

  // 9) SSCTL2: TS0 = 0, IE0 = 1, END0 = 1, D0 = 0
  ADC0_SSCTL2_R = 0x06;

  // 10) enable sample sequencer 2
  ADC0_ACTSS_R |= 0x04;
}

//------------ADC0_Thermistor_In------------
// Part 1C. Busy-wait analog to digital conversion on sequencer 2.
// Input: none
// Output: 12-bit result of the ADC conversion
uint32_t ADC0_Thermistor_In(void) { 
  uint32_t result = 0;
  // 1) initiate SS2
  ADC0_PSSI_R = 0x0004;

  // 2) wait for conversion done (ADC0_RIS_R bit 2)
  while((ADC0_RIS_R & 0x04) == 0){}

  // 3) read result (ADC0_SSFIFO2_R, 12 bits)
  result = ADC0_SSFIFO2_R & 0xFFF;

  // 4) acknowledge completion
  ADC0_ISC_R = 0x0004;

  return result;
}

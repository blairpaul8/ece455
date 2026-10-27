// ADCCode.h
// Lab 4, Parts 1A and 1C. ECE 455 / ECE 555, Fall 2026.
#include <stdint.h>

// Part 1A: ADC0 sequencer 3 on the internal temperature sensor, 500 K samples/sec
void ADC0_Temp_Init(void);

// Part 1A: busy-wait read of sequencer 3, 12-bit result
uint32_t ADC0_In(void);

// Part 1C: ADC0 sequencer 2 on PE3 / AIN0, the thermistor divider midpoint
void ADC0_Thermistor_Init(void);

// Part 1C: busy-wait read of sequencer 2, 12-bit result
uint32_t ADC0_Thermistor_In(void);

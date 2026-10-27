// main.c
// Lab 4, Part 1 internal temperature sensor and external NTC thermistor.
// ECE 455 / ECE 555, Fall 2026.
//
// Brief description of the program:
// This lab demonstrates a simple environmental monitoring unit. The program reads
// the internal analog temperature sensor of the TM4C123 through the on-board ADC.
// The temperature is converted to Celsius and Fahrenheit and printed to the console.
// In Part 1C, a second, external temperature sensor (an NTC thermistor in a voltage
// divider on PE3) is read alongside it.
//
// The UART signals are configured only for displaying console messages. They are
// not required for operation of the ADC.
// - UART0 peripheral, GPIO Port A: U0Rx on PA0, U0Tx on PA1
// - The given UART_Init runs at 115200 baud with the 16 MHz default clock (no PLL).
//
// NOTE: The internal temperature sensor is not calibrated.  This example
// just takes the raw temperature sensor sample and converts it using the
// equation found in the datasheet.
//

#define LAB_PART 2       // 1 = Part 1A and 1B only, 2 = Part 1A-1D

#include <stdbool.h>
#include <stdint.h>
#include <math.h>        // logf, for the Beta equation in Part 1C
#include "ADCCode.h"
#include "UART.h"
#include "tm4c123gh6pm.h"

// ---------------- Part 1A: student written functions ----------------
uint32_t Convert_Raw_To_V(uint32_t raw);
uint32_t Convert_V_To_C(uint32_t mV);
uint32_t Get_TempC(void);
uint32_t Convert_C_To_F(uint32_t TempC);

// ---------------- Part 1C: student written functions ----------------
uint32_t  Convert_Thermistor_To_C(uint32_t raw);
uint32_t  Get_ThermistorC(void);

int main(void){
  unsigned char keystroke;     // variable to note the key pressed
  uint32_t val;                // internal sensor temperature

  UART_Init();                 // UART0 at 16 MHz, 115200 baud
  ADC0_Temp_Init();            // initialize ADC0 to take temperature readings
#if LAB_PART >= 2
  ADC0_Thermistor_Init();      // Part 1C: sequencer 2 on PE3 / AIN0
  UART_printf("PART 2 enabled");
  Newline();
#endif

  UART_printf("Initialization Complete..."); Newline();
  UART_printf("Press C/c for Celsius or F/f for Fahrenheit"); Newline(); Newline();

  // Continually read and display the temperature
  while(1){
    keystroke = Getchar();     // wait for the user to enter a key
    Newline();
    val = Get_TempC();
    if(keystroke == 'c' || keystroke == 'C'){
      UART_printf("Temperature in Celsius: ");
      UART_OutUDec(val);

      // Part 1D: also print the thermistor temperature on the same line
      #if LAB_PART >= 2
        uint32_t thermistor_val = Get_ThermistorC();
        UART_printf("     Thermistor: ");
        UART_OutUDec(thermistor_val);
      #endif

      Newline();
      
      
      Newline();
    }
    else if(keystroke == 'f' || keystroke == 'F'){
      UART_printf("Temperature in Fahrenheit: ");
      val = Convert_C_To_F(val);
      UART_OutUDec(val);

      // Part 1D: also print the thermistor temperature on the same line
      #if LAB_PART >= 2
        uint32_t thermistor_val = Get_ThermistorC();
        thermistor_val = Convert_C_To_F(thermistor_val);
        UART_printf("     Thermistor: ");
        UART_OutUDec(thermistor_val);
      #endif

      Newline();
      
      
      Newline();
    }
    else{
      UART_printf("Press Valid key!");
      Newline();
    }
  }
}

/*
 *  NOTE: You need to complete the following functions
 */

// ====================== Part 1A ======================

// Convert a raw 12-bit ADC value to millivolts.
// The reference is 3.3 V and the data sheet's equation divides by 4096.
// Input: raw ADC value, 0 to 4095
// Output: millivolts
uint32_t Convert_Raw_To_V(uint32_t raw){
  // 12 bit ADC so range is 0 - 4096.
  // 3.3v VREF = 3300 mV
  uint32_t milliVolts = (raw * 3300) / 4096;
  UART_printf("Millivolts: ");
  UART_OutUDec(milliVolts);
  Newline();
  return milliVolts;
}

// Convert the sensor voltage in millivolts to Celsius.
// Equation from the "Tiva TM4C123GH6PM Microcontroller Data Sheet", Figure 13-11:
//   Voltage (mV) / 1000 = 2.7 - ((TempC + 55) / 75)
// so TempC = 147.5 - 75 * Voltage(V) = 147.5 - 0.075 * Voltage(mV)
// Input: millivolts
// Output: temperature in Celsius
uint32_t Convert_V_To_C(uint32_t mV){
  uint32_t temp_c = 147.5 - (0.075 * mV);

  return temp_c;
}

// Returns the temperature in Celsius.
// You should call ADC0_In(), Convert_Raw_To_V() and Convert_V_To_C().
// Try averaging multiple readings together to get more stable outputs.
uint32_t Get_TempC(void){
  uint32_t raw_result = 0;
  // Read the raw result from ADC0 (average 20 readings)
  for (uint8_t i = 0; i < 20; i++) {
    raw_result += ADC0_In();
  }

  raw_result /= 20;

  uint32_t mV = Convert_Raw_To_V(raw_result);

  uint32_t degreesC = Convert_V_To_C(mV);
  // Convert raw to millivolts, then millivolts to Celsius, and return it
  return degreesC;
}

// Convert a temperature from Celsius to Fahrenheit.
// Complete this equation: T in F = 32 + (T in C * 1.8)
uint32_t Convert_C_To_F(uint32_t TempC){
  uint32_t degreesF = 32 + (TempC * 1.8);

  return degreesF;
}

// ====================== Part 1C ======================
// Divider and thermistor constants. R0 and B come from the Vishay NTCLE100E3103JB0
// data sheet: R25 = 10 kohm, B25/85 = 3977 K. 
#define R_FIXED   10000.0f    // fixed resistor, +3.3 V to PE3, ohms
#define R0        10000.0f    // thermistor resistance at T0, ohms
#define T0_KELVIN 298.15f     // 25 C
#define BETA      3977.0f     // B25/85, kelvin
#define VCC_MV    3300.0f

// Convert a raw 12-bit sample of the thermistor divider midpoint to Celsius.
//   1) voltage at the midpoint, from the raw sample and the 3.3 V reference
//   2) thermistor resistance, from the divider equation and your fixed resistor
//   3) temperature from the Beta equation: 1/T = 1/T0 + (1/B) ln(R/R0), T in kelvin
// Input: raw ADC value, 0 to 4095
// Output: temperature in Celsius
uint32_t Convert_Thermistor_To_C(uint32_t raw){
  // 1) voltage at the divider midpoint, in mV
  float v_mV = ((float)raw * VCC_MV) / 4096.0f;

  // 2) thermistor resistance from the divider equation
  //    V = VCC * Rt / (Rt + R_FIXED)  ->  Rt = R_FIXED * V / (VCC - V)
  if(raw == 0){ return 0; }              // avoids ln(0) if the pin is shorted to GND
  float rt = R_FIXED * v_mV / (VCC_MV - v_mV);

  // 3) Beta equation: 1/T = 1/T0 + (1/B) * ln(R/R0), T in kelvin
  float invT = (1.0f / T0_KELVIN) + (1.0f / BETA) * logf(rt / R0);
  float tempC = (1.0f / invT) - 273.15f;

  if(tempC < 0.0f){ tempC = 0.0f; }      // return type is unsigned
  return (uint32_t)tempC;
}

// Returns the thermistor temperature in Celsius, averaging 20 samples first.
// You should call ADC0_Thermistor_In(), Convert_Thermistor_To_C.
uint32_t Get_ThermistorC(void){
  uint32_t raw_result = 0;
  // Read the raw result from ADC0 (average 20 readings)
  for (uint8_t i = 0; i < 20; i++) {
    raw_result += ADC0_Thermistor_In();
  }

  raw_result /= 20;

  uint32_t degreesC = Convert_Thermistor_To_C(raw_result);

  // Convert raw to millivolts, then millivolts to Celsius, and return it
  return degreesC;
}
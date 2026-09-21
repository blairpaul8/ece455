// Sound.c, derived from SysTickInts.c
// Lab 3: UART and DAC Interfacing
// ECE 455 / ECE 555, Embedded Systems Design, Fall 2026
// Team Members name: <Student1>; <Student2>
// Due Date: Friday, October 2, 2026, 11:59 p.m.
//
// Part 2B: complete DAC_Init, Sound_Init, DAC_Out and SysTick_Handler.
// Part 3B: switch the handler to the 32-element shortwave.


// Use the SysTick timer to request interrupts at a particular period.

// Port B bits 2-0 have the 3-bit DAC

// Port E, SW1(PE0) and SW2(PE2)
// SysTick ISR: Calls SineWave 

#include <stdint.h>
#include "tm4c123gh6pm.h"

unsigned char Index;  

// 3-bit 16-element sine wave
const unsigned char SineWave[16] = {4,5,6,7,7,7,6,5,4,3,2,1,1,1,2,3};

//4-bit 32-element Sine wave for Piano
const unsigned shortwave[32] = {	
  8,9,11,12,13,14,14,15,15,15,14,	
  14,13,12,11,9,8,7,5,4,3,2,	
  2,1,1,1,2,2,3,4,5,7};	

// **************DAC_Init*********************
// Initialize 3-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){unsigned long volatile delay;
  // activate port B
  // allow time to finish activating
	// no analog 
	// regular function
  // make PB2-0 out
  // disable alt funct on PB2-0
  // enable digital I/O on PB2-0
}

// **************Sound_Init*********************
// Initialize Systick periodic interrupts, this is basically demonstrate SysTick usage
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none
void Sound_Init(unsigned long period){
  DAC_Init();          // Port B is DAC
  Index = 0;
	// your code begins below
	
  // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value should be as per the value passed in period
  // any write to current clears it
  // priority 1      
  // enable SysTick with core clock and interrupts
}

// **************DAC_Out*********************
// output to DAC
// Input: 3-bit data, 0 to 7 
// Output: none
void DAC_Out(unsigned long data){
  // write the value to Data register at PortB
}

// Interrupt service routine
// Executed every 20ns*(period) at a 50 MHz bus clock
void SysTick_Handler(void){
 
	// Increment the Index and make sure it should roll over to 0 after 0x0F, i.e. 15 in decimal,  
	// corresponding to size of the variable SineWave[16]
  
	// calll DAC_out function and pass 
  DAC_Out(SineWave[Index]); 						// Comment this line for Part 3
	
	// Pass shortwave samples for Piano - in Part 3
}



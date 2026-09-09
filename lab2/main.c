#include "PortAD.h"
#include "SysTick.h"
#include "PLL.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

// Change this number to the current part of the lab you are working on
#define LAB_PART 1     // 1 for Part2A, 2 for Part 2B, 3 for Part 3A, 4 for Part 3B (Without overlapping), and 5 for Part 3B (With overlapping) 




int main(){
	
	// call the port initialization function
	PortA_Init();
	PortD_Init();
	
	if(LAB_PART==1){
		// This should demonstrate the functionality for Part 2A
		SysTick_Init();                   // Function to initialize the SysTick
		
		while(1){
			// This should demonstrate the functionality for Part 2A
			// call appropriate delay
			//GPIO_PORTA_DATA_R &=  	// Turn off when not testing
		}
	}
	
	else if(LAB_PART==2){
		// This should demonstrate the functionality for Part 2B
		PLL_Init();
		SysTick_Init_Interrupts();
		
		
		while(1){
			// Do nothing except wait for a SysTick Interrupt and action taken care at interrupt handler routine
		}
	}
	
	else if(LAB_PART==3){
		// Write the code that demonstrate the functionality for Part 3A
	}
	
	else if(LAB_PART==4){
		// This should demonstrate the functionality for Part 3B
		// Write the code below for "1011" Sequence detectror - without overalapping
	}
		
			else if(LAB_PART==5){
		// This should demonstrate the functionality for Part 3B
		// Write the code below for "1011" Sequence detectror - with overalapping
	}
		
	// Shouldn't reach unless you set an incorrect value for LAB_PART
	else return 0;
}

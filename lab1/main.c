// Programming Assignment 1: Getting Started
// Team Members name: <Student1>; <Student2>
// Due Date: Sept 4, 2026

#include "PortF.h"
#include "IO.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"



// Change this number to the current part
#define PART 1        // 1 for PART3B, 2 for PART3C , 3 for PART4

int main(){
	PortF_Init();
	
	
	if(PART==1){																									// Should Execute only for PART 3B
		
		while(1){
			// you may need to call your functions created IO.c file
			
			
		}
	}
		
	else if(PART==2){																						 // Should Execute only for PART 3C
		
		while(1){
			// Write the code that demonstrate PART3B functionality
			
			
		}
	}

	else if(PART==3){																						 // Should Execute only for PART 4
		
		while(1){
			// Write the code that demonstrate PART4 functionality
			
			
		}
	}
	// Shouldn't reach unless you set an incorrect value for LAB_PART
	else return 0;
	
}

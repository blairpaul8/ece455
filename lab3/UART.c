// UART.c
// Lab 3: UART and DAC Interfacing
// ECE 455 / ECE 555, Embedded Systems Design, Fall 2026
// Team Members name: <Student1>; <Student2>
// Due Date: Friday, October 2, 2026, 11:59 p.m.
//
// Part 1A: complete UART_Init for 9600 baud, 8N1, at the 50 MHz bus clock.
// Part 1B: complete UART_GetChar, UART_inUDec and UART_OutUDec.
// The instructor-defined functions at the end of this file must not be changed.
/*Some Example Modules are taken from Book: "
	"Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
*/
// this connection occurs in the USB debugging cable
// U0Rx (PA0) connected to serial port on PC
// U0Tx (PA1) connected to serial port on PC
// Ground connected ground in the USB cable


#include <stdint.h>
#include "UART.h"
#include "tm4c123gh6pm.h"


#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable


//------------UART_Init------------
// Initialize the UART for 9600 baud rate (50 MHz UART clock, after enabling the PLL),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
	// You should check all the values in below code and decide as per configuaration given
	
  SYSCTL_RCGCUART_R |= 0x00;            							// activate UART0
  SYSCTL_RCGCGPIO_R |= 0x00;            							// activate port A
	
	
  while((SYSCTL_PRGPIO_R&0x01) == 0){};
  UART0_CTL_R &= ~UART_CTL_UARTEN;      							// disable UART
		
	// IBRD and FBRD is very critical, follwoing is set for 115200 baud and PLL set at 80M, you should modify for your configuration
  UART0_IBRD_R = 43;                    							// IBRD = int(80,000,000 / (16 * 115,200)) = int(43.4028)=43
  UART0_FBRD_R = 25;                     							// FBRD = int(0.4028 * 64+0.5) = 26
  
	// 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
		
	// The following values are delibrately changed, you should find as per the instruction in comment
  UART0_CTL_R |= 0x01;                 							// enable UART
  GPIO_PORTA_AFSEL_R |=0x01 ;           						// enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x01 ;             						// enable digital I/O on PA1-0
  
	// configure PA1-0 as UART, check 0x00000023 this value
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000023;
	
			
  GPIO_PORTA_AMSEL_R &= 0x01;          // disable analog functionality on PA
}

//------------Getchar-----------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
// Should block until the user presses a key and then returns that value
// Returns a newline until it has been correctly implemented
char UART_GetChar(void){
  char retchar = '\n';
	
	// Add your code for following functions
	  // Wait until the Receive FIFO empty flag (RXFE) is 0
		// This means data has been received and is available
	
	// Put the result from data register to retchar
	
	// Return
	return retchar;
	
}
//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART_inUDec(void){
	uint32_t number=0, length=0;
	char character;
  character = UART_InChar();
  
	while(){ // accepts until <enter - i.e. CR> is typed
		
		// The next line checks that the input is a digit, 0-9.
		// If the character is not 0-9, it is ignored and not echoed
    if((character>='0') && (character<='9')) {
      number =    										// this line overflows if above 4294967295
      length++;
      UART_OutChar(character);       // Instructor defined function, no change
    }
		// Else If the input is a backspace, then the return number is
		// changed and a backspace is outputted to the screen
    else if((character==BS) && length){
      number /= ;												// write your logic here
      length--;
      UART_OutChar(character);					// Instructor defined function, no change
    }
		
		// Default accept
    character = UART_InChar();
  }
  return number;
}

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string


/* convert n to ASCII and sent */
}

// you can add more functions below

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*---------------Instructor Defnied Functions------------------------
 Following are Instructor Defined functions and you should not change any
-------------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// You should look following function, it may help to write up your code


//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}


//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_printf(char *pt){
  while(*pt){
    UART_OutChar(*pt);
    pt++;
  }
}


//---------------------Newline---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void Newline(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}


//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string

void UART_InString(char *bufPt, uint16_t max) {
int length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}

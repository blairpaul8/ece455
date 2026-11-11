// fixed.c
// Lab 5, Part 2. ECE 455 / ECE 555, Fall 2026.
// Contains the four functions that you need to write. lab5.c calls each of them
// and prints the expected string beside your output on the LCD.
//
// Hardware configuration (Part 1)
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO)
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "ST7735.h"
#include "fixed.h"

static int32_t xMin, xMax, yMin, yMax;   // the plot window, resolution 0.001

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD
Parameter LCD display
 12345    " *.***"
  2345    " 2.345"
 -8100    "-8.100"
  -102    "-0.102"
    31    " 0.031"
-12345    " *.***"
 */
void ST7735_sDecOut3(int32_t n){
  // 1) out of range: output " *.***" and return
  // 2) output '-' and negate if negative, else output ' '
  // 3) extract each digit (add '0' to get its ASCII code) and output it with
  //    ST7735_OutChar(): units, '.', tenths, hundredths, thousandths
  (void)n;
}

/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256.
 The full-scale range is from 0 to 999.99.
 If the integer part is larger than 256000, it signifies an error.
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD
Parameter LCD display
     0	  "  0.00"
     2	  "  0.01"
    64	  "  0.25"
   100	  "  0.39"
   500	  "  1.95"
   512	  "  2.00"
  5000	  " 19.53"
 30000	  "117.19"
255997	  "999.99"
256000	  "***.**"
*/
void ST7735_uBinOut8(uint32_t n){
  // 1) out of range: output "***.**" and return
  // 2) the resolution is 1/256 = 2^-8, so the low 8 bits are the fraction;
  //    convert the fraction to two decimal places (times 100, divide by 256)
  // 3) integer part as three characters with leading blanks, not leading zeros
  // 4) '.', then the two fraction digits
  (void)n;
}

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
  // Fill the screen with black (ST7735_FillScreen)
  // Set the cursor to (0,0) and print the title in white (ST7735_SetTextColor,
  // ST7735_OutString), then restore the text colour
  (void)title;
  // No change to the following four lines
  xMin = minX;
  xMax = maxX;
  yMin = minY;
  yMax = maxY;
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
  // For each point inside the window:
  //   a = (bufX[i] - xMin) * 127 / (xMax - xMin)          pixel column, 0 to 127
  //   b = (yMax - bufY[i]) * 127 / (yMax - yMin) + 32     pixel row, 32 to 159 (the
  //                                                       title takes the top 32 rows)
  //   ST7735_DrawPixel(a, b, ST7735_RED)
  (void)num; (void)bufX; (void)bufY; (void)xMin; (void)xMax; (void)yMin; (void)yMax;
}

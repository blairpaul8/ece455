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

extern uint16_t StTextColor;

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
void ST7735_sDecOut3(int32_t n) {
  // 1) out of range: output " *.***" and return
  if((n > 9999) || (n < -9999)) {
    ST7735_OutChar(' ');
    ST7735_OutChar('*');
    ST7735_OutChar('.');
    ST7735_OutChar('*');
    ST7735_OutChar('*');
    ST7735_OutChar('*');
    return;
  }

  // 2) output '-' and negate if negative, else output ' '
  if(n < 0) {
    ST7735_OutChar('-');
    n = -n;
  } else {
    ST7735_OutChar(' ');
  }

  // 3) extract each digit and output it
  ST7735_OutChar((n / 1000) + '0');        // units
  ST7735_OutChar('.');
  ST7735_OutChar(((n / 100) % 10) + '0');  // tenths
  ST7735_OutChar(((n / 10) % 10) + '0');   // hundredths
  ST7735_OutChar((n % 10) + '0');          // thousandths
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
void ST7735_uBinOut8(uint32_t n) {
  uint32_t h, ip, fp;

  // 1) out of range: output "***.**" and return
  if(n >= 256000){
    ST7735_OutChar('*');
    ST7735_OutChar('*');
    ST7735_OutChar('*');
    ST7735_OutChar('.');
    ST7735_OutChar('*');
    ST7735_OutChar('*');
    return;
  }

  // 2) n/256 in hundredths, rounded: (n*100 + 128) / 256
  h = (n * 100 + 128) / 256;
  if(h > 99999) h = 99999;      // 255999 would round up to 1000.00
  ip = h / 100;                 // integer part, 0..999
  fp = h % 100;                 // fraction, 0..99

  // 3) integer part, three characters, leading blanks
  if(ip >= 100){
    ST7735_OutChar((ip / 100) + '0');
    ST7735_OutChar(((ip / 10) % 10) + '0');
  } else if(ip >= 10){
    ST7735_OutChar(' ');
    ST7735_OutChar((ip / 10) + '0');
  } else {
    ST7735_OutChar(' ');
    ST7735_OutChar(' ');
  }
  ST7735_OutChar((ip % 10) + '0');

  // 4) '.', then two fraction digits
  ST7735_OutChar('.');
  ST7735_OutChar((fp / 10) + '0');
  ST7735_OutChar((fp % 10) + '0');
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
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
  uint16_t oldColor = StTextColor;       // save current text colour

  // Fill the screen with black
  ST7735_FillRect(0, 32, 128, 128, ST7735_BLACK);

  // Cursor to (0,0), print title in white, then restore the text colour
  ST7735_SetCursor(0, 0);
  ST7735_SetTextColor(ST7735_WHITE);
  ST7735_OutString(title);
  ST7735_SetTextColor(oldColor);

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
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
  uint32_t i;
  int32_t x, y;
  int32_t a, b;

  for(i = 0; i < num; i++){
    x = bufX[i];
    y = bufY[i];

    // neglect points outside the window
    if((x < xMin) || (x > xMax) || (y < yMin) || (y > yMax)){
      continue;
    }

    // pixel column 0..127 and pixel row 32..159 (title uses the top 32 rows)
    a = (int32_t)(((int64_t)(x - xMin) * 127) / (xMax - xMin));
    b = (int32_t)(((int64_t)(yMax - y) * 127) / (yMax - yMin)) + 32;

    ST7735_DrawPixel(a, b, ST7735_RED);
  }
}

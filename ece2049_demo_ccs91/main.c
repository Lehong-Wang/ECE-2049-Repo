#include <msp430.h>

#include <stdio.h>
#include <stdlib.h>

#include "peripherals.h"

#include "lab_3.h"
// Function Prototypes
void init(void);



// Declare globals here
const char demo_song[] = {0x11, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00};





// Main
void main(void)

{
  init();

//   int a = 1;
//   char disp[3] = "   ";
//   disp[1] = a+'0';
//   char* d = malloc(4);
//   memset(d, 0x30, 4);
//   char c[4] = "";
//   d[0] = 'h';
////   d[1] = 'h';
//   d[2] = 'h';
//   d[3] = 'h';
//   strncpy(c,d,4);
//
////   char d[2] = "12";
//   Graphics_clearDisplay(&g_sContext);
//   Graphics_drawStringCentered(&g_sContext, disp, 3, 48, 40, TRANSPARENT_TEXT);
//   Graphics_drawStringCentered(&g_sContext, d, 4, 48, 60, TRANSPARENT_TEXT);
//   Graphics_drawStringCentered(&g_sContext, c, 4, 48, 70, TRANSPARENT_TEXT);
//   Graphics_flushBuffer(&g_sContext);


    displayWholeTime(26000000);






}



// Initialize
void init(){
  WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                               // You can then configure it properly, if desired
  // Useful code starts here
  initLeds();

  configDisplay();
  configKeypad();

  // *** Intro Screen ***
  Graphics_clearDisplay(&g_sContext); // Clear the display

}







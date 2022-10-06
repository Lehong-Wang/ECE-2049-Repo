#include <msp430.h>

#include <stdio.h>
#include <stdlib.h>

#include "peripherals.h"

#include "lab_2.h"
#include "lab_3.h"
// Function Prototypes
void init(void);

// time is shared globaly to all files
unsigned long timer = 0;
unsigned long interval_counter = 0;
unsigned long total_time_count = 0;


void main(void)

{
  init();
  InitTimerA2();
  configLab3();

  mainLoopLab3();
}



// Initialize
void init(void){
WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                           // You can then configure it properly, if desired
// Useful code starts here
initLeds();

configDisplay();
configKeypad();

// *** Intro Screen ***
Graphics_clearDisplay(&g_sContext); // Clear the display

}



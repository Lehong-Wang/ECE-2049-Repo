
#include <stdio.h>
#include <stdlib.h>
#include <msp430.h>

#include "peripherals.h"
#include "lab_1.h"

// Function Prototypes
void init(void);








// Main
void main(void){
    init();
    full_lab_1_code();
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



















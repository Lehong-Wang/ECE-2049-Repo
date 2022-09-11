
#include <msp430.h>

#include "peripherals.h"

// Function Prototypes
void init(void);

// Declare globals here

// Main
void main(void)

{
    init();






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



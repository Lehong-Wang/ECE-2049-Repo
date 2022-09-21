#include <msp430.h>

#include <stdio.h>
#include <stdlib.h>

#include "peripherals.h"

#include "lab_2.h"
// Function Prototypes
void init(void);



// Declare globals here
const char demo_song[] = {0x11, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00};





// Main
void main(void)

{
    init();

//    printf("%d",NOTE_TABLE[1]);



    while (true){
        if (getKey() == '*'){
            TurnBuzzerOn(128);
        }

        if (getKey() == '#'){
            TurnBuzzerOff();
        }

        if (getKey() == '0'){
            ParseSong((unsigned char*) &demo_song);
        }

        if (getKey() == '1'){
            TurnBuzzerOn(256);
        }

        if (getKey() == '2'){
            TurnBuzzerOn(512);
        }

        if (getKey() == '3'){
            TurnBuzzerOn(300);
        }

    }








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










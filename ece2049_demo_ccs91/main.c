
#include <msp430.h>

#include "peripherals.h"


// Function Prototypes
void init(void);

// Declare globals here

// Main
void main(void)

{
    init();
    //enum state
    enum a{START, MAKE_ALIEN, COUNT_DOWN, GAMING, WIN, LOSE};
    enum a state;
    state=START;

    //array
    int data_array[15][5];
    int column, row;
    int generate_alien;
    int rand(void);

    int level=0;
    unsigned long i, j;
    int loop_counter=0;

    unsigned char dispThree[3];

//    while(1){

        loop_counter++;

        switch(state){

            case(START):
                    Graphics_clearDisplay(&g_sContext);

                    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "SPACE INVADER", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "Press * to start", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

                    Graphics_flushBuffer(&g_sContext);

//                    if (get_key=='*')
                        Graphics_clearDisplay(&g_sContext);
                        state=MAKE_ALIEN;

            case(MAKE_ALIEN):


                    for(row=0;row<5;row++)
                        {
                            for(column=0;column<15;column++)
                                {
                                    generate_alien=rand()%9;

                                    data_array[row][column] = generate_alien;

                                }


                        }

                    state=COUNT_DOWN;

            case(COUNT_DOWN):

                i = 1;
                j = 3;

                while(j>=1){

                    dispThree[0] = ' ';
                    dispThree[2] = ' ';
                    dispThree[1] = j + '0';

                    Graphics_drawStringCentered(&g_sContext, "READY", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, dispThree, AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
                    Graphics_flushBuffer(&g_sContext);

                    //delay
                    while (i <= 40000) {
                        i = i + 1;
                    }

                    j = j - 1;
                    i = 1;
                }

                state=GAMING;

            case(GAMING):






            case(WIN):


                while (i <= 40000) {
                     i = i + 1;
                }

                Graphics_clearDisplay(&g_sContext);
                BuzzerOn();
                Graphics_drawStringCentered(&g_sContext, "NEXT LEVEL", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);

                while (i <= 80000) {
                     i = i + 1;
                }

                BuzzerOff();
                Graphics_clearDisplay(&g_sContext);

                state=GAMING;

            case(LOSE):

                while (i <= 40000) {
                     i = i + 1;
                }

                Graphics_clearDisplay(&g_sContext);
                BuzzerOn();
                Graphics_drawStringCentered(&g_sContext, "YOU", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "HAVE BEEN", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "DEFEATED!!!", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

                while (i <= 60000) {
                     i = i + 1;
                }

                Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 70, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "to restart", AUTO_STRING_LENGTH, 48, 80, TRANSPARENT_TEXT);

//                if (get_key=='*')
                    Graphics_clearDisplay(&g_sContext);
                    state=MAKE_ALIEN;

        }


//    }





}


void modify_array_key(){

    int found, i, column, row;

    found = 0;

    for(column=0; column<15; column++){

        for(row=0; row<5; row++){

            if(data_array[row] == get_key){

                found = 1;
            }
            else{

                break;
            }
        }
    }

    if(found == 1){
        data_array[column][row] = 0;
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



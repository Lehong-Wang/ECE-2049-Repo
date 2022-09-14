
#include <stdio.h>
#include <stdlib.h>
#include <msp430.h>

#include "peripherals.h"

// Function Prototypes
void init(void);
unsigned char get_key(void);
void swDelay(char);
void generate_alian(int);
void add_to_screne(int content, int x, int y);
void display_on_screne();
void clear_screne();
void update_screne();
void modify_array();
bool check_win();
bool check_lose();




// Declare globals here

const unsigned int SCRENE_COL = 5;
const unsigned int SCRENE_ROW = 5;
const unsigned int TOTAL_ROW = 15;
const unsigned int GRID_WIDTH = 16;

unsigned int data_matrix[TOTAL_ROW][SCRENE_COL];

unsigned int screne_bottom = 0;

unsigned int last_key_pressed = 0;








// Main
void main(void){
    init();


    enum state{
        START, MAKE_ALIAN, COUNT_DOWN, GAMING, WIN, LOSE
    };
    unsigned int level = 1;
    unsigned long loop_counter = 0;
    enum state state = START;

    unsigned int i = 0;
    unsigned char dispThree[3];
    unsigned int win_stage = 0;



    while (true){
        loop_counter ++;

        switch (state){

            case (START):
                if (loop_counter == 2){
                Graphics_clearDisplay(&g_sContext);

                Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "SPACE INVADER", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press * to start", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

                Graphics_flushBuffer(&g_sContext);
                }
                // look for key *
                if (get_key() == '*'){
                    Graphics_clearDisplay(&g_sContext);
                    state = MAKE_ALIAN;
                    loop_counter = 1;
                }
                break;

            case (MAKE_ALIAN):
                if (loop_counter == 2){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "ALIENS", AUTO_STRING_LENGTH, 48, 7, TRANSPARENT_TEXT);
                    generate_alian(level);
                }
                if (loop_counter > 50000){
                    state = COUNT_DOWN;
                    loop_counter = 1;
                }
                break;

            case (COUNT_DOWN) : { // if declear local variable in switch
                                    // need braket to limit the scope
                unsigned int count_down ;

                dispThree[0] = ' ';
                dispThree[2] = ' ';
                if (loop_counter == 2){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "READY", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    count_down = 3;

                }
                // display every 2000 loops
                if (loop_counter % 2000 == 0){
                    if (count_down == 0){
                        state=GAMING;
                        loop_counter = 1;
                    }
                    else{
                        dispThree[1] = count_down + '0';
                        Graphics_clearDisplay(&g_sContext);
                        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 48, 40, TRANSPARENT_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        count_down --;
                    }
                }
                break;
            }

            case (GAMING) :

                // start with 5 refresh per move
                // get faster every 2 level

                // every 100, refresh screen
                if (loop_counter % 100 == 0){
                    // every 500, move down once
                    unsigned int time_reduction = 0;
                    time_reduction = 50 * level - 50;
                    if (time_reduction < 200)
                        time_reduction = 200;
                    if (loop_counter > 500 - time_reduction){
                        screne_bottom ++;
                        loop_counter = 1;
                    }
                    // refresh screen
                    clear_screne();
                    update_screne();
                    display_on_screne();

                    if (check_win()){
                        clear_screne();
//                        swDelay(5);
                        state = WIN;
                        loop_counter = 1;
                    }
                    if (check_lose()){
//                        swDelay(5);
                        state = LOSE;
                        loop_counter = 1;
                    }
                }
                modify_array();
                break;

            case WIN : {
                if (loop_counter == 2){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "YOU WIN !", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);

                }
                if (loop_counter > 3000 && win_stage == 0){
                    Graphics_drawStringCentered(&g_sContext, "* to continue", AUTO_STRING_LENGTH, 48, 50, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "# to restart", AUTO_STRING_LENGTH, 48, 70, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    win_stage = 1;
                }
                unsigned char key;
                key = get_key();
                if (key == '*'){
                    state = MAKE_ALIAN;
                    level ++;
                    loop_counter = 1;
                    win_stage = 0;
                    screne_bottom = 0;
                }
                if (key == '#'){
                    state = START;
                    level = 1;
                    loop_counter = 1;
                    win_stage = 0;
                    screne_bottom = 0;
                }
                break;
            }

            case LOSE: {
                if (loop_counter == 20000){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "YOU", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "HAVE BEEN", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "DEFEATED!!!", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);                    Graphics_flushBuffer(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "# to restart", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }

                    unsigned char key;
                    key = get_key();
                    if (key == '#'){
                        state = START;
                        level = 1;
                        loop_counter = 1;
                        screne_bottom = 0;

                    }
                break;
            }













            default :
                Graphics_clearDisplay(&g_sContext);

                Graphics_drawStringCentered(&g_sContext, "SPACE INVADER", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                swDelay(5);
                return;













        }




    }






}




//
//    generate_alian(3);
//
//while (true){
//    loop_counter ++;
//    if (loop_counter > 500){
//        i++;
//        loop_counter = 1;
//    }
//
//    if (loop_counter % 100 == 0){
//        screne_bottom = i;
//        clear_screne();
//        update_screne();
//
//        display_on_screne();
//
//        if (check_win()){
//            clear_screne();
//            swDelay(5);
//            return;
//        }
//        if (check_lose()){
//            swDelay(5);
//        }
//    }
//    modify_array();
//
//}

//    for (i=0; i<10; i++) {
//        screne_bottom = i;
//        clear_screne();
//        update_screne();
//
//        display_on_screne();
//        swDelay(5);
//    }




//        generate_alian(3);

//        for (i=0; i<10; i++) {
//            dispThree[1] = i+'0';
//    //        get_key();
//    //        generate_alian(3);
//            Graphics_clearDisplay(&g_sContext);
//            Graphics_drawStringCentered(&g_sContext, dispThree, AUTO_STRING_LENGTH, i*GRID_WIDTH, 3*GRID_WIDTH, TRANSPARENT_TEXT);
//
//
//    //        add_to_screne(i, i,3);
//            display_on_screne();
//            swDelay(10);
//        }

    //    printf("%d", state);

//    }









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


void swDelay(char numLoops)
{
    volatile unsigned int i,j;
    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}


void add_to_screne(int content, int grid_x, int grid_y){
    unsigned char dispThree[3];
    dispThree[0] = ' ';
    dispThree[2] = ' ';
    dispThree[1] = content+'0';
//    dispThree[1] = '*';

    int x = (grid_x+1)*GRID_WIDTH;
    int y = (grid_y+1)*GRID_WIDTH;
    Graphics_drawStringCentered(&g_sContext, dispThree, 3, x, y, TRANSPARENT_TEXT);
}

void display_on_screne(){
    int i = 0;
    int y = 0;
    int row_num = 0;
    unsigned char dispThree[3];
    dispThree[0] = ' ';
    dispThree[2] = ' ';
    // add row num to left of screne
    for (i=0; i<SCRENE_ROW+1; i++){
        row_num = i + screne_bottom;
        dispThree[1] = row_num+'0';
        // count y from bottom
        y = (SCRENE_ROW - i)*GRID_WIDTH;
//        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 10, y, 0);

    }

    Graphics_flushBuffer(&g_sContext);
}

void clear_screne(){
    Graphics_clearDisplay(&g_sContext);
}



unsigned char get_key(){
    unsigned char current_key = 0;
    current_key = getKey();
    return current_key;
}
//    unsigned char currKey=0, dispSz = 3;
//    unsigned char dispThree[3];
//    currKey = getKey();
//    if (currKey)
//    {
//
//        dispThree[1] = currKey;
//        // Draw the new character to the display
//        Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 48, 55, OPAQUE_TEXT);
//
//        // Refresh the display so it shows the new data
//        Graphics_flushBuffer(&g_sContext);
//
//        // wait awhile before clearing LEDs
//        swDelay(1);
//        setLeds(0);
//    }





void generate_alian(int level){
    int i = 0;
    int j = 0;
    int num = 0;
    for (i=0; i<TOTAL_ROW; i++)
        for (j=0; j<SCRENE_COL; j++)
            data_matrix[i][j] = 0;

//    Graphics_clearDisplay(&g_sContext);

    for (i=0; i<=level; i++){
        for (j=0; j<SCRENE_COL; j++){
            num = rand() % 5;
            data_matrix[SCRENE_ROW+i][j] = num;
            add_to_screne(num, j, i);
        }
    }
    display_on_screne();
//    swDelay(5);
}



void update_screne(){
    Graphics_clearDisplay(&g_sContext);
    int i = 0;
    int j = 0;
    int num = 0;

    // everytime screen refresh, can press key again
    last_key_pressed = 0;
    for (i=screne_bottom; i<SCRENE_ROW+screne_bottom; i++){
        for (j=0; j<SCRENE_COL; j++){
            num = data_matrix[i][j];
            if (num != 0){
                add_to_screne(num, j, SCRENE_ROW-(i-screne_bottom)-1);
            }
        }
    }
    display_on_screne();
//    swDelay(10);
}



void modify_array(){

    unsigned int col, row, key_char, key;

    key_char = get_key();
    key = key_char - '0';
    // only process same key once
    if (key == last_key_pressed || key == 0)
        return;

    // only look for alian on screne
    for(row=0; row<screne_bottom+SCRENE_ROW; row++){

        for(col=0; col<SCRENE_COL; col++){

            if(data_matrix[row][col] == key){
                data_matrix[row][col] = 0;
                last_key_pressed = key;
                break;
            }

        }
    }

}


bool check_win(){
    unsigned int row = 0, col = 0, data = 0;
    for(row=0; row<TOTAL_ROW; row++){
        for(col=0; col<SCRENE_COL; col++){
            data = data_matrix[row][col];
            if (data != 0)
                return false;
        }
    }
    return true;
}


bool check_lose(){
    bool is_lose = false;
    unsigned int col = 0;
    for (col=0; col<SCRENE_COL; col++)
        is_lose |= data_matrix[screne_bottom][col];
    return is_lose;
}



















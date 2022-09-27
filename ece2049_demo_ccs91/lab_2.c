/*
 * lab_2.c
 *
 *  Created on: Sep 21, 2022
 *      Author: lhwang
 */

# include "lab_2.h"


const unsigned int NOTE_TABLE[] =
        {
         440,   // A
         466,   // B flat
         494,   // B
         523,   // C
         554,   // C sharp
         587,   // D
         622,   // E flat (Eb)
         659,   // E
         698,   // F
         740,   // F sharp
         784,   // G
         831,   // A flat
         880,   // A
        };

unsigned long timer = 0;
unsigned long interval_counter = 0;




/*
 * Function for testing
 */
void ParseSong(unsigned char* song_array_pointer){
    unsigned char* current_pointer = song_array_pointer;
    unsigned char current_note_pack = *current_pointer;
    unsigned char note_index;
    unsigned int note;
    unsigned char duration;


    while (current_note_pack != 0){
        // lower 4 bit is note, higher 4 bit is duration
        note_index = current_note_pack & 0x0f;
        duration = (current_note_pack & 0xf0) >> 4;
        note = NOTE_TABLE[note_index];

        TurnBuzzerOn(note);
        show_on_screne(note + 1000*duration);
        swDelay(duration);

//        break;
        // move to next packet
        current_pointer ++;
        current_note_pack = *current_pointer;
    }

    TurnBuzzerOff();

    return;
}





















// void check_lose(unsigned char debet_key){
//     if (debet_key != 0){

//     }
// }


// assign note to button 1-4
unsigned char getButton(unsigned char note_index) {
    unsigned char button_num = 0;
    button_num = note_index % 4 + 1;
    return button_num + '0';
}







void main_loop(void){

    enum state {
        START, MENU, COUNT_DOWN, GAMING, LOSE, WIN,
    };

   enum state state = START;


    // unsigned int loop_counter = 0;

    unsigned char* current_pointer;
    unsigned char current_note_pack = *current_pointer;

    unsigned char debet_key = 0;
    unsigned char current_key = 0;

    unsigned char note_index = 0;
    unsigned int note = 0xff;
    unsigned char duration = 0xff;

    unsigned char pressed_key = 0;
    unsigned char miss_num = 0;

    unsigned char final_miss = 0;
    unsigned char final_index = 0;
    
    const unsigned char demo_song[] = {0x11, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00};
    const unsigned char bebo_song[] = {0x11, 0x26, 0x11, 0x26, 0x11, 0x26, 0x11, 0x26, 0x00};
    current_pointer = (unsigned char) &demo_song;

    bool song_chosen = false;

    timer = 0;

    while (true){
        // loop_counter ++;

        switch (state) {


            // Welcome screen
            // press * to continue
            case (START):
                if (timer == 1){
                Graphics_clearDisplay(&g_sContext);

                Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "GITAR HERO", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press * to start", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

                Graphics_flushBuffer(&g_sContext);
                }
                // look for key *
                if (get_key() == '*'){
                    Graphics_clearDisplay(&g_sContext);
                    state = MENU;
                    timer = 0;
                }
                break;



            case MENU : {

                if (timer == 2){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "Choose your song", AUTO_STRING_LENGTH, 48, 20, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "1. Demo Song", 20, 48, 30, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "2. BeBo Song", 20, 48, 40, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "3. Demo Song", 20, 48, 50, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "4. Demo Song", 20, 48, 60, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }

                switch (getKey()){
                    case '1':
                        current_pointer = (unsigned char) &demo_song;
                        song_chosen = true;
                        break;
                    case '2':
                        current_pointer = (unsigned char) &bebo_song;
                        song_chosen = true;
                        break;
                    default:
                        // current_pointer = (unsigned char) &demo_song;
                        break;
                }

                if (song_chosen){
                    state = COUNT_DOWN;
                    timer = 0;
                    song_chosen = false;
                    Graphics_clearDisplay(&g_sContext);

                }

                break;
            }




            // display count down screne
            // count 3 2 1
            case (COUNT_DOWN) : { // if declear local variable in switch
                                    // need braket to limit the scope
                unsigned int count_down;
                unsigned char dispThree[3];

                dispThree[0] = ' ';
                dispThree[2] = ' ';
                if (timer == 2){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "READY", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                    setLeds(15);
                    TurnBuzzerOn(700);
                    count_down = 3;

                }
                // display every 1 s
                if (timer > 2 && timer % 10 == 0){
                    if (count_down == 0){
                        state = GAMING;
                        timer = 0;
                        Graphics_clearDisplay(&g_sContext);
                    }
                    else{
                        // display count down
                        dispThree[1] = count_down + '0';
                        Graphics_clearDisplay(&g_sContext);
                        Graphics_drawStringCentered(&g_sContext, dispThree, 3, 48, 40, TRANSPARENT_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        setLeds((1 << count_down) - 1);
                        TurnBuzzerOn(300 + 100*count_down);
                        count_down --;
                    }
                }
                break;
            }




            case GAMING : {
               // within duration of note
                if (timer <= 10){
//                    LightLED(current_key - '0');
                    pressed_key = getKey();
                    // clear key
                    if (pressed_key != 0){
                        if (debet_key == 0){
                            if (current_key == pressed_key) {
                                current_key = 0;
                                // reverse all LED
                                P6OUT = (P6OUT & ~(BIT4|BIT3|BIT2|BIT1)) + (~P6OUT & (BIT4|BIT3|BIT2|BIT1));
//                                P6OUT |= (BIT4|BIT3|BIT2|BIT1);
                            }
                        }
                        else {
                            if (debet_key == pressed_key) {
                                debet_key = 0;
                                // reverse all LED
                                P6OUT = (P6OUT & ~(BIT4|BIT3|BIT2|BIT1)) + (~P6OUT & (BIT4|BIT3|BIT2|BIT1));
                            }
                        }
                    }
                    // DisplayStatus(current_key, pressed_key, miss_num, current_pointer-&demo_song, debet_key);
                }

                else if (timer > 10){
                    timer = 0;
                    TurnBuzzerOff();

                    // if debet_key != 0, lose 
                    if (debet_key != 0){

                        // Display miss number
                        Graphics_clearDisplay(&g_sContext);
                        char miss_string[10] = "Miss:             ";
                        miss_string[7] = debet_key;
                        Graphics_drawStringCentered(&g_sContext, miss_string, 10, 48, 35, TRANSPARENT_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        
                        debet_key = 0;
                        miss_num ++;

                        if (miss_num >= 3){
                            state = LOSE;
                            timer = 0;
                            final_index = current_pointer-&demo_song;
                            current_pointer = &demo_song;
                            final_miss = miss_num;
                            miss_num = 0;
                            continue;
                        }
                    }



                    debet_key = current_key;
                    current_pointer ++;
                    
                    current_note_pack = *current_pointer;
                    note_index = current_note_pack & 0x0f;
                    duration = (current_note_pack & 0xf0) >> 4;
                    note = NOTE_TABLE[note_index];

                    // if at end of song, win
                    if (current_note_pack == 0){
                        state = WIN;
                        timer = 0;
                        current_pointer = &demo_song;
                        final_miss = miss_num;
                        miss_num = 0;
                        debet_key = 0;
                        continue;
                    }

                    current_key = getButton(note_index);

                    // sound buzzer
                    TurnBuzzerOn(note);
                    LightLED(current_key - '0');


                    
                }
            }
            break;

            case LOSE: {
                if (timer == 5){
                    char miss_string[10] = "Miss:             ";
                    char index_string[10] = "Index:           ";
                    miss_string[7] = final_miss+'0';
                    index_string[7] = final_index+'0';
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "YOU LOSE", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, miss_string, 10, 48, 35, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, index_string, 10, 48, 45, TRANSPARENT_TEXT);                    Graphics_flushBuffer(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "# to restart", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }

            

                unsigned char key;
                key = get_key();
                if (key == '#'){
                    state = START;
                    timer = 0;
                }

                break;
            }



            // display win screen
            // * to continue, # to restart
            case WIN : {
                // win screen
                if (timer == 5){
                    char miss_string[10] = "Miss:             ";
                    miss_string[7] = final_miss+'0';
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "YOU WIN !", AUTO_STRING_LENGTH, 48, 20, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, miss_string, 10, 48, 40, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "* to continue", AUTO_STRING_LENGTH, 48, 60, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "# to restart", AUTO_STRING_LENGTH, 48, 80, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }
                unsigned char key;
                key = get_key();
                if (key == '*'){
                    state = START;
                    timer = 0;
                }
                if (key == '#'){
                    state = START;
                    timer = 0;
                }
                break;
            }




            default:

            break;
















        }



    }

}










void LightLED(unsigned int led_index){
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
    if (led_index == 1)
        P6OUT |= BIT2;
    if (led_index == 2)
        P6OUT |= BIT1;
    if (led_index == 3)
        P6OUT |= BIT3;
    if (led_index == 4)
        P6OUT |= BIT4;
        
}






/*
 * Enable a PWM-controlled buzzer on P3.5
 * This function makes use of TimerB0.
 */
void TurnBuzzerOn(unsigned int frequency)
{
    // Initialize PWM output on P3.5, which corresponds to TB0.5
    P3SEL |= BIT5; // Select peripheral output mode for P3.5
    P3DIR |= BIT5;

    TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
    TB0CTL  &= ~TBIE;                       // Explicitly Disable timer interrupts for safety

    // Now configure the timer period, which controls the PWM period
    // Doing this with a hard coded values is NOT the best method
    // We do it here only as an example. You will fix this in Lab 2.
    TB0CCR0   = (unsigned int) 32768 / frequency;                    // Set the PWM period in ACLK ticks
    TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts

    // Configure CC register 5, which is connected to our PWM pin TB0.5
    TB0CCTL5  = OUTMOD_7;                   // Set/reset mode for PWM
    TB0CCTL5 &= ~CCIE;                      // Disable capture/compare interrupts
    TB0CCR5   = TB0CCR0/2;                  // Configure a 50% duty cycle
}

/*
 * Disable the buzzer on P7.5
 */
void TurnBuzzerOff(void)
{
    // Disable both capture/compare periods
    TB0CCTL0 = 0;
    TB0CCTL5 = 0;
}



void DisplayStatus(unsigned char key, unsigned char pressed_key, unsigned char miss_num, unsigned char song_index, unsigned char debet_key){
    Graphics_clearDisplay(&g_sContext);
    char key_string[10] = "Key:           ";
    char pressed_string[10] = "Press:            ";
    char miss_string[10] = "Miss:             ";
    char index_string[10] = "Index:           ";
    char debet_string[10] = "Debet:           ";

    key_string[7] = key;
    pressed_string[7] = pressed_key;
    miss_string[7] = miss_num + '0';
    index_string[7] = song_index + '0';
    debet_string[7] = debet_key;

    Graphics_drawStringCentered(&g_sContext, key_string, 10, 48, 10, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, pressed_string, 10, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, miss_string, 10, 48, 40, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, index_string, 10, 48, 55, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, debet_string, 10, 48, 70, TRANSPARENT_TEXT);

    Graphics_flushBuffer(&g_sContext);
}





/*
 * Set MCLK, SMCLK, DCOCLK = 4 M Hz
 * Timer_A2 = 1 M Hz
 */ 
void InitClock(void){
    P5SEL |= (BIT5|BIT4|BIT3|BIT2); // enable P5 2-5 function mode
    UCSCTL6 &= ~(BIT0<<12 | BIT8 | BIT4 | BIT1 | BIT0); // turn on clock
    UCSCTL2 = 0x0001; // set N = 1, D = 1
    UCSCTL3 = 0x0051; // FLL fFLLREFCLK = XT2 n = 2
    // DCOCLK = 4M Hz
//    UCSCTL4 = UCSCTL4 & 0xff00 + 0x0033; // SMCLK, MCLK = DCOCLK
    UCSCTL5 &= 0xff00; // SMCLK, MCLK source devide = 0

}


void InitTimerA2(void){
//    TA2CTL = 0x0296; // SMCLK, /2, clear, enable interrupt
//    TA2CCTL0 |= 0x0010; // enable interrupt
//    TA2CCR0 = 1000; // 1000 tick per interrupt, 1 ms per interrupt
//    _BIS_SR(GIE); // enable interrupt


    TA2CTL = TASSEL_1 + MC_1 + ID_0;
    TA2CCR0 = 327;       // 327+1 = 328 ACLK tics = ~1/100 seconds
    TA2CCTL0 = CCIE;
    _BIS_SR(GIE);
}


#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR (void)

{
    // timer ++;

     interval_counter ++;
     if (interval_counter > 10){
         interval_counter = 0;
         timer ++;
 //        P6OUT ^= (BIT4);

     }
}



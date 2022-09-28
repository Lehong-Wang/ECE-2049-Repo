/*
 * lab_2.c
 *
 *  Created on: Sep 21, 2022
 *      Author: lhwang
 */

# include "lab_2.h"


const unsigned int NOTE_TABLE[] =
        {
         440,   // A4           // 0
         466,   // B flat
         494,   // B
         523,   // C
         554,   // C sharp
         587,   // D
         622,   // E flat (Eb)
         659,   // E
         698,   // F            // 8
         740,   // F sharp
         784,   // G
         831,   // A flat
         880,   // A5
         932,   // B flat
         988,   // B
         1046,  // C
         1109,  // C sharp      // 16
         1175,  // D
         1244,   // E flat
         1318,   // E
         1397,   // F
         1479,   // F sharp
         1568,   // G
         1661,   // A flat
         1760,   // A6          // 24

        };

unsigned long timer = 0;
unsigned long interval_counter = 0;
const unsigned int GRAPH_LEN = 16;
unsigned char note_histogram[GRAPH_LEN];
unsigned char line1[GRAPH_LEN];
unsigned char line2[GRAPH_LEN];
unsigned char line3[GRAPH_LEN];
unsigned char line4[GRAPH_LEN];



/*
 * Function for testing
 * 4 bit duration, 4 bit index
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
    unsigned char duration = 0x1;

    unsigned char pressed_key = 0;
    unsigned char miss_num = 0;

    unsigned char final_miss = 0;
    unsigned char final_index = 0;
    
    // for every note:
    // bit 0-4: note index
    // bit 5-7: note duration
    // duration 765 | 43210 index
    // eg. 0x56 -> duration = 0x2, index = 0x16
    // index 0x1F is reserved for scilent note

    const unsigned char demo_song[] = {0x21, 0x26, 0x25, 0x34, 0x33, 0x22, 0x21, 0x00};
    const unsigned char bebo_song[] = {0x21, 0x26, 0x21, 0x26, 0x2C, 0x2D, 0x2C, 0x2D, 0x00};
    const unsigned char Hanabi[] = {0x27, 0x3F, 0x2A, 0x4C, 0x28, 0x25, 0x23, 0x22, 0x25, 0x28, 0x6A, 0x2A, 0x28, 0x47, 0x4A, 0x28, 0x45, 0x25, 0x27, 0x3F, 0x2A, 0x4C, 0x28, 0x25, 0x23, 0x22, 0x25, 0x28, 0x6A, 0x2A, 0x28, 0x47, 0x4A, 0x28, 0x25, 0x25, 0x45, 0x87, 0x00};
    const unsigned char Super_Mario[] = {0x33, 0x33, 0x3F, 0x33, 0x3F, 0x2F, 0x31, 0x3F, 0x33, 0x7F, 0x2A, 0x7F, 0x2F, 0x5F, 0x2A, 0x5F, 0x27, 0x5F, 0x2C, 0x3F, 0x3E, 0x3F, 0x3D, 0x2C, 0x3F, 0x2A, 0x33, 0x36, 0x38, 0x3F, 0x34, 0x36, 0x3F, 0x2F, 0x25, 0x2E, 0x5F, 0x00};
    const unsigned char White_Ark[] = {0x22, 0x24, 0x45, 0x4A, 0x47, 0x25, 0x24, 0x22, 0x24, 0x25, 0x24, 0x22, 0x20, 0x42, 0x3F, 0x2A, 0x2A, 0x2A, 0x2A, 0x20, 0x25, 0x27, 0x2A, 0x27, 0x2A, 0x2C, 0x2B, 0x2A, 0x2A, 0x00};
    const unsigned char Kokoro_Umi[] = {0x2A, 0x20, 0x21, 0x47, 0x21, 0x20, 0x2C, 0x4A, 0x27, 0x48, 0x4A, 0x2A, 0x20, 0x21, 0x47, 0x21, 0x20, 0x2C, 0x4A, 0x27, 0x48, 0x4A, 0x2A, 0x20, 0x21, 0x47, 0x21, 0x20, 0x2C, 0x4A, 0x27, 0x48, 0x4A, 0x00};
    current_pointer = (unsigned char) &demo_song;

    bool song_chosen = false;
    unsigned char* selected_song = 0;
    bool enjoy_only = false;
    unsigned char unit_duration = 1;


    timer = 0;

    while (true){

        // anytime, press * to quit
        if (get_key() == '*'){
            state = START;
            P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
            Graphics_clearDisplay(&g_sContext);
            TurnBuzzerOff();
            timer = 0;

        }

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
                // press number to choose song
                // can choose to not play and only listen to song
                //      toggle this setting with '0'
                //      LED 4 on means listen only, off means play
                //      only listen will skip count down
                if (timer == 2){
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "Choose your song", AUTO_STRING_LENGTH, 48, 10, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "1. Demo Song", 20, 48, 20, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "2. BeBo Song", 20, 48, 30, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "3. Hanabi", 20, 48, 40, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "4. Super Mario", 20, 48, 50, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "5. White Ark", 20, 48, 60, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "6. Kokoro Umi", 20, 48, 70, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "Press 0 to enjoy song", 20, 48, 85, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }

                switch (getKey()){
                    case '1':
                        selected_song = (unsigned char*) &demo_song;
                        song_chosen = true;
                        break;
                    case '2':
                        selected_song = (unsigned char*) &bebo_song;
                        song_chosen = true;
                        break;
                    case '3':
                        selected_song = (unsigned char) &Hanabi;
                        song_chosen = true;
                        break;
                    case '4':
                        selected_song = (unsigned char) &Super_Mario;
                        song_chosen = true;
                        break;
                    case '5':
                        selected_song = (unsigned char) &White_Ark;
                        song_chosen = true;
                        break;
                    case '6':
                        selected_song = (unsigned char) &Kokoro_Umi;
                        song_chosen = true;
                        break;
                    case '0':
                        P6OUT = (P6OUT & ~BIT4) + (~P6OUT & BIT4);
                        // enjoy_only = (P6OUT & BIT4) >> 4;
                        break;
                    default:
                        // current_pointer = (unsigned char) &demo_song;
                        break;
                }

                if (song_chosen){
                    state = COUNT_DOWN;
                    timer = 0;
                    song_chosen = false;
                    current_pointer = selected_song;
                    unit_duration = 10;
                    enjoy_only = (P6OUT & BIT4) >> 4;
                    if (enjoy_only){
                        unit_duration = 2;
                        state = GAMING;
                    }


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



            // press key pad number coorsponding to LED number
            //      can delay one note (eg. note 3 can still be pressed when note 4 is playing, but not note 5)
            //      if note is not pressed in time, it will count towards a miss
            //      3 miss will be lose
            // flip all LEDs if you hit a not successfully
            // display warning when you miss a note
            // display a frequency map when playing
            //      not display when only listening to song
            case GAMING : {
               // within duration of note
                if (timer <= unit_duration * duration){
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

                else if (timer > unit_duration * duration){
                    timer = 0;
                    TurnBuzzerOff();
                    Graphics_clearDisplay(&g_sContext);


                    // if debet_key != 0, miss 
                    if (!enjoy_only && debet_key != 0){

                        // Display miss number
                        char miss_string[10] = "Miss:             ";
                        miss_string[7] = debet_key;
                        Graphics_drawStringCentered(&g_sContext, miss_string, 10, 48, 35, TRANSPARENT_TEXT);
                        Graphics_flushBuffer(&g_sContext);

                        debet_key = 0;
                        miss_num ++;
                        // 3 miss is lose
                        if (miss_num >= 3){
                            state = LOSE;
                            timer = 0;
                            // where you lose
                            final_index = current_pointer - selected_song;
                            // pointer to start of song
                            current_pointer = selected_song;
                            final_miss = miss_num;
                            miss_num = 0;
                            continue;
                        }
                    }

                    // if didn't press key immidiatly, it goes into debet
                    debet_key = current_key;
                    current_pointer ++;
                    // 3 bit duration, 5 bit index
                    current_note_pack = *current_pointer;
                    note_index = current_note_pack & 0x1f;
                    duration = (current_note_pack & 0xe0) >> 5;
                    // 0x1f is no sound
                    if (note_index == 0x1f)
                        note = 0;
                    else
                        note = NOTE_TABLE[note_index];

                    // if at end of song, win
                    if (current_note_pack == 0){
                        state = WIN;
                        timer = 0;
                        current_pointer = selected_song;
                        final_miss = miss_num;
                        miss_num = 0;
                        debet_key = 0;
                        continue;
                    }
                    // current key needed to press
                    current_key = getButton(note_index);

                    // sound buzzer
                    TurnBuzzerOn(note);
                    LightLED(current_key - '0');
                    if (!enjoy_only)
                        ShowHistogram(current_key - '0');

                }
            }
            break;


            // display lose screen
            // * to continue, # to restart
            case LOSE: {
                if (timer == 5){
                    char miss_string[10] = "Miss:             ";
                    char index_string[10] = "Index:           ";
                    miss_string[7] = final_miss+'0';
                    if (final_index > 9)
                        index_string[6] = final_index / 10 + '0';
                    index_string[7] = final_index % 10 + '0';
                    Graphics_clearDisplay(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "YOU LOSE", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, miss_string, 10, 48, 35, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, index_string, 10, 48, 45, TRANSPARENT_TEXT);                    Graphics_flushBuffer(&g_sContext);
                    Graphics_drawStringCentered(&g_sContext, "* to go back", AUTO_STRING_LENGTH, 48, 60, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "# to retry", AUTO_STRING_LENGTH, 48, 80, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }

                unsigned char key;
                key = get_key();
                if (key == '*'){
                    state = START;
                    timer = 0;
                }
                if (key == '#'){
                    state = COUNT_DOWN;
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
                    Graphics_drawStringCentered(&g_sContext, "* to go back", AUTO_STRING_LENGTH, 48, 60, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "# to retry", AUTO_STRING_LENGTH, 48, 80, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }
                unsigned char key;
                key = get_key();
                if (key == '*'){
                    state = START;
                    timer = 0;
                }
                if (key == '#'){
                    state = COUNT_DOWN;
                    timer = 0;
                }
                break;
            }


            default:
            break;

        }

    }

}




/***
 * generate a frequency graph from current playing note
 * current note is high (4)
 * other notes are random (0-2)
 * over 4 rows will cause display problems
 */ 
void ShowHistogram(unsigned char note_index){
    unsigned int len = sizeof(note_histogram)/sizeof(char);
    unsigned int i;
    unsigned char value = 0;
    unsigned char pixel = '#';
    unsigned char blank = ' ';
    for (i=0; i<len; i++){
        note_histogram[i] = rand() % 3 + '0';
        // if (i != 0)
        //     note_histogram[i] = (note_histogram[i-1] + note_histogram[i])/2;
        // note_histogram[i] = i + '0';
    }
    unsigned char hist_index = note_index;
    note_histogram[hist_index] = 4 + '0';
    // note_histogram[0] = 0x23;
    // note_histogram[1] = 0x23;
    // note_histogram[2] = 0x23;
    // note_histogram[2] = 0x23;


    
//    Graphics_drawStringCentered(&g_sContext, note_histogram, len, 48, 42, TRANSPARENT_TEXT);

    for (i=0; i<len; i++) {
        value = note_histogram[i]-'0';

        // if (value >= 5)
        //     line5[i] = pixel;
        // else
        //     line5[i] = blank;
        if (value >= 4)
            line4[i] = pixel;
        else
            line4[i] = blank;
        if (value >= 3)
            line3[i] = pixel;
        else
            line3[i] = blank;
        if (value >= 2)
            line2[i] = pixel;
        else
            line2[i] = blank;
        if (value >= 1)
            line1[i] = pixel;
        else
            line1[i] = blank;
        
    }
    Graphics_drawStringCentered(&g_sContext, line1, len, 48, 86, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, line2, len, 48, 78, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, line3, len, 48, 70, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, line4, len, 48, 62, TRANSPARENT_TEXT);
    // Graphics_drawStringCentered(&g_sContext, line5, len, 48, 54, TRANSPARENT_TEXT);

    Graphics_flushBuffer(&g_sContext);


}




/**
 * Light LED by index
 */
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
    if (frequency == 0){
        TurnBuzzerOff();
        return;
    }
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


/**
 * function for debuging
 */
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

/**
 * Interrupt Service Rutinue
 */

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




// Below is trash

void configKeypadMulti(void)
{
    P1SEL &= ~(BIT5|BIT4|BIT3|BIT2);
    P2SEL &= ~(BIT5|BIT4);
    P4SEL &= ~(BIT3);

    P2DIR |= (BIT5|BIT4);
    P1DIR |= BIT5;
    P2OUT |= (BIT5|BIT4);
    P1OUT |= BIT5;

    P1DIR &= ~(BIT2|BIT3|BIT4);
    P4DIR &= ~(BIT3);
    P4REN |= (BIT3);
    P1REN |= (BIT2|BIT3|BIT4);
    P4OUT |= (BIT3);
    P1OUT |= (BIT2|BIT3|BIT4);
}


unsigned int getMultiKey(void)
{
    unsigned int ret_val = 0;

    P1OUT &= ~BIT5;
    P2OUT |= (BIT5|BIT4);
    // Now check value from each rows
    if ((P4IN & BIT3)==0)
        ret_val &= BIT0;
    if ((P1IN & BIT2)==0)
        ret_val &= BIT3;
    if ((P1IN & BIT3)==0)
        ret_val &= BIT6;
    if ((P1IN & BIT4)==0)
        ret_val &= BIT0 << 9;
    P1OUT |= BIT5;

    P2OUT &= ~BIT4;
    // Now check value from each rows
    if ((P4IN & BIT3)==0)
        ret_val &= BIT1;
    if ((P1IN & BIT2)==0)
        ret_val &= BIT4;
    if ((P1IN & BIT3)==0)
        ret_val &= BIT7;
    if ((P1IN & BIT4)==0)
        ret_val &= BIT0 << 10;
    P2OUT |= BIT4;

    // Set Col1 = ?, Col2 = ? and Col3 = ?
    P2OUT &= ~BIT5;
    // Now check value from each rows
    if ((P4IN & BIT3)==0)
        ret_val &= BIT2;
    if ((P1IN & BIT2)==0)
        ret_val &= BIT5;
    if ((P1IN & BIT3)==0)
        ret_val &= BIT0 << 8;
    if ((P1IN & BIT4)==0)
        ret_val &= BIT0 << 11;
    P2OUT |= BIT5;

    return(ret_val);
}


void configUserLED(char inbits){
    unsigned char mask = 0;

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    if (inbits & BIT0)
        mask |= BIT4;   // Right most LED P6.4
    if (inbits & BIT1)
        mask |= BIT3;   // next most right LED P.3
    if (inbits & BIT2)
        mask |= BIT1;   // third most left LED P6.1
    if (inbits & BIT3)
        mask |= BIT2;   // Left most LED on P6.2
    P6OUT |= mask;
}


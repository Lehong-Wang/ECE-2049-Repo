/*
 * lab_2.c
 *
 *  Created on: Sep 21, 2022
 *      Author: lhwang
 */

# include "lab_2.h"


const unsigned short NOTE_TABLE[] =
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

//NOTE_TABLE[1];
// =  466



void ParseSong(unsigned char* song_array_pointer){
    unsigned char* current_pointer = song_array_pointer;
    unsigned char current_note_pack = *current_pointer;
    unsigned char note_index;
    unsigned char note;
    unsigned char duration;


    while (current_note_pack != 0){
        // lower 4 bit is note, higher 4 bit is duration
        note_index = current_note_pack & 0x0f;
        duration = current_note_pack & 0xf0 >> 4;
        note = NOTE_TABLE[note_index];

        TurnBuzzerOn(note);
        swDelay(duration);

//        break;
        // move to next packet
        current_pointer ++;
        current_note_pack = *current_pointer;
    }

    TurnBuzzerOff();

    return;
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
    TB0CCR0   = frequency;                    // Set the PWM period in ACLK ticks
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






void swDelay(unsigned char numLoops)
{
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2021

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}






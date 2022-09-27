/*
 * lab_2.h
 *
 *  Created on: Sep 21, 2022
 *      Author: lhwang
 */

#ifndef LAB_2_H_
#define LAB_2_H_


#include "peripherals.h"
#include "lab_1.h"


extern const unsigned int NOTE_TABLE[];





void ParseSong(unsigned char* song_array_pointer);




unsigned char getButton(unsigned char note_index);

void main_loop(void);




void LightLED(unsigned int led_index);
void TurnBuzzerOn(unsigned int frequency);
void TurnBuzzerOff(void);
void DisplayStatus(unsigned char key, unsigned char pressed_key, unsigned char miss_num, unsigned char song_index, unsigned char debet_key);

void InitClock(void);
void InitTimerA2(void);


















#endif /* LAB_2_H_ */




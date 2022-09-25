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


void TurnBuzzerOn(unsigned int frequency);
void TurnBuzzerOff(void);

























#endif /* LAB_2_H_ */




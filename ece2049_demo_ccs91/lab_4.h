

#ifndef LAB_4_H_
#define LAB_4_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peripherals.h"
#include "lab_3.h"


extern unsigned long total_time_count;
extern unsigned long timer;
extern unsigned long interval_counter;


void DACInit(void);
void DACSetValue(unsigned int dac_code);

void generateWave(int index);

void ADCWave(long time, int timer_frequency);
void squareWave(long time, int timer_frequency);
void sawtoothWave(long time, int timer_frequency);
void triangleWave(long time, int timer_frequency);
void dynamicTriangleWave(long time, int timer_frequency);

void mainLoopLab4(void);

void dispWaveInfoScreen(void);

int getLastPressedKey(void);
void configLab4ADC(void);
int getA1ADC(void);
float getA1Votage(void);
void configLab4(void);











#endif

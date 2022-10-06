
#ifndef LAB_3_H_
#define LAB_3_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peripherals.h"


extern unsigned long total_time_count;
extern unsigned long timer;
extern unsigned long interval_counter;


 // Temperature Sensor Calibration = Reading at 30 degrees C is stored at addr 1A1Ah
 // See end of datasheet for TLV table memory mapping
 #define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
 // Temperature Sensor Calibration = Reading at 85 degrees C is stored at addr 1A1Ch                                            //See device datasheet for TLV table memory mapping
 #define CALADC12_15V_85C  *((unsigned int *)0x1A1C)


// struct to store parsed time information
struct Time {
 unsigned long total_time;
 unsigned int seconds;
 unsigned int minutes;
 unsigned int hours;
 unsigned int days;
 unsigned int months;
};


void configTemp(void);
void configScroll(void);
void configButton(void);
void configADC(void);
void configLab3(void);

int getPressedButton(void);

int getTempADC(void);
int getAverageADC(int new_adc);
char* parseTemp(int temp_adc, bool is_C);
void displayNewTemp(bool is_C);

int getScrollADC(void);
int mapToDateSelection(int adc, int index);

struct Time parseTime(unsigned long total_time);
long unparseTime(struct Time parsed_time);

void displayWholeTime(struct Time parsed_time);
void displayDate(struct Time parsed_time);
void displayTime(struct Time parsed_time);

void displayEditScreen(void);

void mainLoopLab3(void);

char* intToChar(int number);



#endif

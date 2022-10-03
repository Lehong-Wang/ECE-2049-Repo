
#ifndef LAB_3_H_
#define LAB_3_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peripherals.h"



// unsigned long time_count;

struct Time {
  unsigned long total_time;
  unsigned int seconds;
  unsigned int minutes;
  unsigned int hours;
  unsigned int days;
  char month[10];
};


void configTemp(void);
void configScroll(void);
void configTimer(void);
void configLab3(void);

int getTempADC(void);

struct Time parseTime(unsigned long total_time);

void displayWholeTime(unsigned long total_time);

char* intToChar(int number);














#endif

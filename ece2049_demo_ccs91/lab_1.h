
/*
 * lab_1.h
 *
 *  Created on: Sep 24, 2022
 *      Author: lhwang
 */

#ifndef LAB_1_H_
#define LAB_1_H_


#include "peripherals.h"






// Function Prototypes
unsigned char get_key(void);
void swDelay(char);
void generate_alian(int);
void add_to_screne(int content, int x, int y);
void display_on_screne();
void clear_screne();
void show_on_screne(unsigned int content);
void update_screne();
void modify_array();
bool check_win();
bool check_lose();

void mainLoopLab1(void);







#endif /* LAB_1_H_ */

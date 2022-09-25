
/*
 * lab_1.h
 *
 *  Created on: Sep 24, 2022
 *      Author: lhwang
 */

#ifndef LAB_1_H_
#define LAB_1_H_


#include "peripherals.h"




// extern const unsigned int SCRENE_COL;
// extern const unsigned int SCRENE_ROW;
// extern const unsigned int TOTAL_ROW;
// extern const unsigned int GRID_WIDTH;

// extern unsigned int data_matrix[][];

// extern unsigned int screne_bottom;

// extern unsigned int last_key_pressed;







// Function Prototypes
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

void full_lab_1_code(void);







#endif /* LAB_1_H_ */

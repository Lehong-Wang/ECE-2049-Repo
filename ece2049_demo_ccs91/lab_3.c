# include "lab_3.h"


float degC_per_bit;

int left_button_press_count = -1;     // press once will be 0

const char MONTH_STR[12][4] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};
// number of days before the month
const int MONTH_DAY[13] = {
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365,
};




// ############################### Config ##################################



/**
 * @brief configure ONLY the temperature sensor
 *        DO NOT USE THIS FUNCTION
 *        for debuging only
 */
void configTemp(void){
  
  REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
                          // internal reference voltages to
  	  	  	  // ADC12_A control registers
  ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON;     // Internal ref = 1.5V
  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
  // Using ADC12MEM0 to store reading
  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense
  	                                    // ACD12SREF_1 = internal ref = 1.5v
  __delay_cycles(100);                    // delay to allow Ref to settle
 
}

/**
 * @brief configure ONLY the scroll wheel
 *        DO NOT USE THIS FUNCTION
 *        for debuging only
 */
void configScroll(void){
  // Configure P8.0 as digital IO output and set it to 1
  // This supplied 3.3 volts across scroll wheel potentiometer
  // See schematic at end or MSP-EXP430F5529 board users guide
  P8SEL &= ~BIT0;
  P8DIR |= BIT0;
  P8OUT |= BIT0;

  P6SEL &= ~BIT0;
  P6DIR &= ~BIT0;

  REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
                          // internal reference voltages to
                          // ADC12_A control registers
  ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
  // Use ADC12MEM1 register for conversion results
  ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_0;   // ADC12INCH5 = Scroll wheel = A5
                                            // ACD12SREF_0 = Vref+ = Vcc
  __delay_cycles(100);                      // delay to allow Ref to settle
}

/**
 * @brief configure the S1-S4 buttons at the bottom left of the board
 */
void configButton(void){
  P7SEL &= ~BIT0;    // P7.0 S1
  P7DIR &= ~BIT0;    // input
  P7REN |= BIT0;
  P7OUT |= BIT0;    // pull up register

  P3SEL &= ~BIT6;    // P3.4 S2
  P3DIR &= ~BIT6;
  P3REN |= BIT6;
  P3OUT |= BIT6;

  P2SEL &= ~BIT2;    // P2.2 S3
  P2DIR &= ~BIT2;
  P2REN |= BIT2;
  P2OUT |= BIT2;

  P7SEL &= ~BIT4;    // P7.4 S4
  P7DIR &= ~BIT4;
  P7REN |= BIT4;
  P7OUT |= BIT4;
  __delay_cycles(100); 
}


/**
 * @brief configre the ADC used for this lab
 *        use this function ONLY
 */
void configADC(void){
  // Configure P8.0 as digital IO output and set it to 1
  // This supplied 3.3 volts across scroll wheel potentiometer
  // See schematic at end or MSP-EXP430F5529 board users guide
  P8SEL &= ~BIT0;
  P8DIR |= BIT0;
  P8OUT |= BIT0;
  // scroll wheel use P6.0 which is A0
  P6SEL &= ~BIT0;
  P6DIR &= ~BIT0;


   REFCTL0 &= ~REFMSTR; // Reset REFMSTR to hand over control of
                        // internal reference voltages to
   	  	  	  	  	    // ADC12_A control registers
  // ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
  ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON | ADC12MSC;     // Internal ref = 1.5V

//    ADC12CTL1 = ADC12SHP | ADC12CONSEQ0 | ADC12CSTARTADD0;                     // Enable sample timer, sequence of conversion, start at 0
   ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                     // Enable sample timer, sequence of conversion

   ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;   // ADC i/p ch A10 = temp sense
  	                                          // ACD12SREF_1 = internal ref = 1.5v
                                              // Use ADC12MEM0 register for conversion results
   ADC12MCTL1 = ADC12SREF_0 + ADC12INCH_0 + ADC12EOS;   // ADC12INCH5 = Scroll wheel = A0
                                                        // ACD12SREF_0 = Vref+ = Vcc
   __delay_cycles(100);                      // delay to allow Ref to settle
}


/**
 * @brief wraper for all configs
 */
void configLab3(void){
  // configTemp();
  // configScroll();
  configADC();
  configButton();
  ADC12CTL0 |= ADC12ENC;		    // Enable conversion
  __delay_cycles(100); 
}


/**
 * @brief Get the index of button pressed (1-4)
 *        if multiple pressed, return one with the smallest index
 *        only return one signal every press
 * @return int 
 */
int getPressedButton(void){
  static int last_press = 0;
  int pressed = 0;
  if (~P7IN & BIT0)
    pressed = 1;
  else if (~P3IN & BIT6)
    pressed = 2;
  else if (~P2IN & BIT2)
    pressed = 3;
  else if (~P7IN & BIT4)
    pressed = 4;
  // if same as last pressed, return 0
  if (pressed == last_press)
    pressed = 0;
  // else if (pressed != 0)
  else
    last_press = pressed;

  return pressed;
}



// ############################### Temperature ##################################



/**
 * @brief Get the Temprature ADC value
 * @return int 
 */
int getTempADC(void){
  // float degC_per_bit;
  unsigned int bits30, bits85;
  // Use calibration data stored in info memory
  bits30 = CALADC12_15V_30C;
  bits85 = CALADC12_15V_85C;
  degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));

  int temp_adc = 0;
  ADC12CTL0 &= ~ADC12SC; 		// clear the start bit
  ADC12CTL0 |= ADC12SC;               // Sampling and conversion start
          // Single conversion (single channel)

  // Poll busy bit waiting for conversion to complete
  while (ADC12CTL1 & ADC12BUSY)
  __no_operation();
  temp_adc = ADC12MEM0 & 0x0fff;               // Read results if conversion done

  __no_operation();                       // SET BREAKPOINT HERE

  return temp_adc;
}

/**
 * @brief Get the Average ADC value
 * @param new_adc   give it the new ADC value
 * @return int    average over the last 30 measurements
 */
int getAverageADC(int new_adc) {
  static int adc_array[30] = {0};
  static int index = 0;
  int i = 0;
  long sum = 0;
  int num = 0;
  int average = 0;
  // loop around the array
  adc_array[index] = new_adc;
  index ++;
  index = index % 30;

  for (i = 0; i < 30; i++){
    sum += adc_array[i];
    if (adc_array[i] != 0)
      num ++;     // count non-zero values
  }
  average = (int)(sum / num);
  return average;
}



char* parseTemp(int temp_adc, bool is_C){
//  char* temp_char = (char*) malloc(5);
//  memset(temp_char, 0x20, 5);   // set all to space to pervent string ending
  char temp_char[5] = {0};

  float temp = 0;
  temp = (float)((long)temp_adc - CALADC12_15V_30C) * degC_per_bit +30.0;

  if (! is_C)
    temp = (temp * 9/5) + 32.0;

  temp_char[0] = (temp / 100) + '0';      // hundred
  if (temp_char[0] == '0')
    temp_char[0] = ' ';         // get rid of 0 at front
  temp_char[1] = ((int)(temp / 10) % 10) + '0';     // ten
  temp_char[2] = ((int)temp % 10) + '0';     // one
  temp_char[3] = '.';
  temp_char[4] = ((int)(temp * 10) % 10) + '0';     // one tenth

  return temp_char;
}

/**
 * @brief Display the average temperature on LCD
 *        only this need to be called
 * @param is_C  
 */
void displayNewTemp(bool is_C){
  int adc = getTempADC();
  adc = getAverageADC(adc);
  char* temp_char = parseTemp(adc, is_C);
  char temp_string[5] = {0x20};
  strcpy(temp_string, temp_char);

  char label[15] = "Temprature (F)";
  if (is_C)
    strcpy(label, "Temprature (C)");

  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, label, 15, 48, 30, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, temp_string, 5, 48, 55, TRANSPARENT_TEXT);
  Graphics_flushBuffer(&g_sContext);
}



// ############################### Scroll ##################################




/**
 * @brief Get the Scroll ADC value
 * @return int 
 */
int getScrollADC(void){
  int adc = 0;
  ADC12CTL0 &= ~ADC12SC; 		// clear the start bit
  ADC12CTL0 |= ADC12SC;     // Sampling and conversion start
                            // Single conversion (single channel)
  // Poll busy bit waiting for conversion to complete
  while (ADC12CTL1 & ADC12BUSY)
    __no_operation();
  adc = ADC12MEM1 & 0x0fff;   // Read results if conversion done
  return adc;
}


/**
 * @brief map ADC value to the current selected value range
 * @param adc 
 * @param index selection index
 * @return int coorsponding value
 */
int mapToDateSelection(int adc, int index){
  int full_range = 0;
  int value = 0;
  // edit select table: month, day, hour, minute, second
  const static int select_table[5] = {12, 31, 24, 60, 60};
  full_range = select_table[index];
  
  value = full_range * (adc / 4096.0);
  // value 0 = day 1
  if (index == 1)
    value ++;

  return value;
}





// ############################### Time ##################################





/**
 * @brief   parse a given total second
 * @param time  total time in seconds (max is 1 year)
 *              pass 1 year will be chopped
 * @return struct Time  
 */
struct Time parseTime(unsigned long total_time){
  unsigned long time = total_time;
  // if longer than 1 year, reduce
  if (time >= 31536000)
    time = time % 31536000;

  // 60 s -> min
  int second = time % 60;
  time = (unsigned long) time / 60;
  // 60 min -> hour
  int minute = time % 60;
  time = (unsigned long) time / 60;
  // 24 hour -> day
  int hour = time % 24;
  time = (unsigned long) time / 24;  // number of days

  // day of year table
  // char month[10] = "";
  int month = 0;
  int day = 0;
  // time 0 is day 1
  time ++;

  // compare day number to each month and calculate month and date
  int i = 0;
  for (i=0; i<12; i++){
    // number of day less than month i+1
    if (time <= MONTH_DAY[i+1]){
      // month is month i
      month = i;
      // day is month i+1 - month i
      day = time - MONTH_DAY[i];
      break;
    }
  }

  struct Time result_time;
  result_time.total_time = total_time;
  result_time.seconds = second;
  result_time.minutes = minute;
  result_time.hours = hour;
  result_time.days = day;
  result_time.months = month;
  // strcpy(result_time.month, month);

  return result_time;
}


/**
 * @brief Compute the total time based on struct Time
 * @param parsed_time 
 * @return long   total seconds
 */
long unparseTime(struct Time parsed_time){
  // day_num = month_day + day
  unsigned int total_day = parsed_time.days + MONTH_DAY[parsed_time.months];
  // if greater than next month day, chop off extra
  if (total_day > MONTH_DAY[parsed_time.months+1])
    total_day = MONTH_DAY[parsed_time.months+1];
  // time 0 = day 1, now reverse
  total_day --;

  const unsigned int total_hour = total_day * 24 + parsed_time.hours;
  const unsigned long total_minute = (long)total_hour * 60 + parsed_time.minutes;
  const unsigned long total_second = (long)total_minute * 60 + parsed_time.seconds;

  return total_second * 100;
}




// ############################### Display ##################################




/**
 * @brief Display month to second of given Time struct
 * @param parsed_time 
 */
void displayWholeTime(struct Time parsed_time){
  Graphics_clearDisplay(&g_sContext);

  // struct Time parsed_time = parseTime(total_time);
  char disp_str[4] = {0};

  char* second_str = intToChar(parsed_time.seconds);
  strncpy(disp_str, second_str, 2);
  Graphics_drawStringCentered(&g_sContext, "Second:", AUTO_STRING_LENGTH, 30, 20, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 70, 20, TRANSPARENT_TEXT);
  free(second_str);

  char* minute_str = intToChar(parsed_time.minutes);
  strncpy(disp_str, minute_str, 2);
  Graphics_drawStringCentered(&g_sContext, "Minute:", AUTO_STRING_LENGTH, 30, 30, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 70, 30, TRANSPARENT_TEXT);
  free(minute_str);

  char* hour_str = intToChar(parsed_time.hours);
  strncpy(disp_str, hour_str, 2);
  Graphics_drawStringCentered(&g_sContext, "Hour:", AUTO_STRING_LENGTH, 30, 40, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 70, 40, TRANSPARENT_TEXT);
  free(hour_str);

  char* day_str = intToChar(parsed_time.days);
  strncpy(disp_str, day_str, 2);
  Graphics_drawStringCentered(&g_sContext, "Day:", AUTO_STRING_LENGTH, 30, 50, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 70, 50, TRANSPARENT_TEXT);
  free(day_str);

  char month_str[4] = {0};
  strncpy(month_str, MONTH_STR[parsed_time.months], 3);
  Graphics_drawStringCentered(&g_sContext, "Month:", AUTO_STRING_LENGTH, 30, 60, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, month_str, AUTO_STRING_LENGTH, 70, 60, TRANSPARENT_TEXT);

  Graphics_flushBuffer(&g_sContext);

}


/**
 * @brief Display Date, shity code
 * @param parsed_time 
 */
void displayDate(struct Time parsed_time) {

  Graphics_clearDisplay(&g_sContext);

  const int disp_y = 55;
  char disp_str[4] = {0};

  Graphics_drawStringCentered(&g_sContext, "Date:", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);

  char* day_str = intToChar(parsed_time.days);
  strncpy(disp_str, day_str, 2);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 58, disp_y, TRANSPARENT_TEXT);
  free(day_str);

  char month_str[4] = {0};
  strncpy(month_str, MONTH_STR[parsed_time.months], 3);
  Graphics_drawStringCentered(&g_sContext, month_str, sizeof(month_str), 38, disp_y, TRANSPARENT_TEXT);

  Graphics_flushBuffer(&g_sContext);
}


/**
 * @brief Display Time, shity code
 * @param parsed_time 
 */
void displayTime(struct Time parsed_time) {
  Graphics_clearDisplay(&g_sContext);

  const int disp_y = 55;
  char disp_str[4] = {0};

  Graphics_drawStringCentered(&g_sContext, "Time:", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);

  char* second_str = intToChar(parsed_time.seconds);
  strncpy(disp_str, second_str, 2);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 72, disp_y, TRANSPARENT_TEXT);
  free(second_str);

  strncpy(disp_str, " : ", 3);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 60, disp_y, TRANSPARENT_TEXT);

  char* minute_str = intToChar(parsed_time.minutes);
  strncpy(disp_str, minute_str, 2);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 48, disp_y, TRANSPARENT_TEXT);
  free(minute_str);

  strncpy(disp_str, " : ", 3);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 36, disp_y, TRANSPARENT_TEXT);

  char* hour_str = intToChar(parsed_time.hours);
  strncpy(disp_str, hour_str, 2);
  Graphics_drawStringCentered(&g_sContext, disp_str, 2, 24, disp_y, TRANSPARENT_TEXT);
  free(hour_str);

  Graphics_flushBuffer(&g_sContext);
}







/**
 * @brief Editing Time screen
 *        click S1 to select which to edit
 *        use scroll wheel to adjust value
 *        moving to the next selection will save the current one
 */
void displayEditScreen(void){
  static last_time = 0;
  static refresh_time = 0;
  static last_selected_num = 0;
  // only do stuff every 0.1 second
  if (last_time != timer){
    // refresh time every 1 second
   if (refresh_time == 10){
      struct Time parsed_time = parseTime(total_time_count/100);
      displayWholeTime(parsed_time);
      refresh_time = 0;
   }

    const int selection_index = left_button_press_count % 5;
    // get selected number by scroll
    const int adc = getScrollADC();
    const int selected_num = mapToDateSelection(adc, selection_index);
    char show_str[4] = {' '};
    // if month, show month string
    if (selection_index == 0)
      strncpy(show_str, MONTH_STR[selected_num], sizeof(show_str));
    // else show number
    else {
      const char* parsed_str = intToChar(selected_num);
      strncpy(show_str, parsed_str, sizeof(show_str));
    }
    // blink blinder paremeters
    const int blink_x = 70;
    const int blink_y = 60 - 10 * selection_index;
    const char blink_space[5] = " --- ";
    // revert blink_hide everytime
    static bool blink_hide = true;
    blink_hide = ! blink_hide;
    
    // blink the selecting number
    if (blink_hide) {
      // make space not transparent will hide the text underneath
      Graphics_drawStringCentered(&g_sContext, blink_space, 5, blink_x, blink_y, 1);
      Graphics_flushBuffer(&g_sContext);
    }
    else {
      Graphics_drawStringCentered(&g_sContext, show_str, sizeof(show_str), blink_x, blink_y, 1);
      Graphics_flushBuffer(&g_sContext);
    }

    // if press left button, save the value to global time count
    static int last_left_button_press_count = 0;
    // if left button is pressed, save and move on
    if (last_left_button_press_count != left_button_press_count){
      struct Time parsed_time = parseTime(total_time_count/100);
      // save current selection to Time struct
      const int to_save_index = (selection_index+4)%5;
      switch (to_save_index) {
        case 0:
          parsed_time.months = last_selected_num;
          break;
        case 1:
          parsed_time.days = last_selected_num;
          break;
        case 2:
          parsed_time.hours = last_selected_num;
          break;
        case 3:
          parsed_time.minutes = last_selected_num;
          break;
        case 4:
          parsed_time.seconds = last_selected_num;
          break;
      }
      // save time struct to total_time_count
      total_time_count = unparseTime(parsed_time);
    }

    last_time = timer;
    last_left_button_press_count = left_button_press_count;
    refresh_time ++;
    last_selected_num = selected_num;

  }
}





// ############################### Main Function ##################################




void mainLoopLab3(void){

  enum state {
    DISPLAY, EDIT,
  };
  enum state state = DISPLAY;

  enum display_state {
    DATE, TIME, TEMP_C, TEMP_F,
  };
  enum display_state display_state = DATE;

  // configire everything
  configLab3();

  while(1) {
    // constantly checking for button press
    int pressed_button = getPressedButton();
    // go into Edit mode 
    if (pressed_button == 1){
      left_button_press_count ++;     // used in edit selection
      state = EDIT;
      struct Time time = parseTime(total_time_count/100);
      displayWholeTime(time);
    }
    // go into Display mode and reset edit selection
    if (pressed_button == 2){
      state = DISPLAY;
      display_state = DATE;
      left_button_press_count = -1;

    }
      

    switch (state) {

      case DISPLAY: {
        // refresh every second
        if (timer > 10) {
          struct Time time = parseTime(total_time_count/100);
          switch (display_state) {

            case DATE:
              displayDate(time);
              break;

            case TIME:
              displayTime(time);
              break;

            case TEMP_C:
              displayNewTemp(true);
              break;

            case TEMP_F:
              displayNewTemp(false);
              break;
          }
          timer = 0;
          // rotate among four displays
          display_state = (display_state + 1) % 4;
        }

        break;
      }

      case EDIT:
        displayEditScreen();
        break;

    }
  }
}




// ############################### Helper ##################################


/**
 * @brief convert a given interger to a displayable string
 *          need to free memory after use
 * @param number    number should be less than 10 digit, can be negative
 * @return char*    pointer to string, need to free memory after use
 */
char* intToChar(int number){
  // if return string as char*, need to use malloc
  // char* string = malloc(10);
  // memset(string, 0x20, 10);   // set all to space to pervent string ending
  char string[10] = {0x20};
  char reverse_str[10] = {0x20};
  int index = 0;
  int reverse_index = 9;
  // if number = 0, return "0", pervent blank string
  if (number == 0){
    string[0] = '0';
    return string;
  }
  // if number < 0, put "-" at front and revert number
  if (number < 0){
    string[0] = '-';
    number = -number;
    index ++;
  }
  // parse each digit, record backwords in reverse_str
  while (number != 0){
    reverse_str[reverse_index] = number % 10 + '0';
    number = (int) number / 10;
    reverse_index --;
  }
  // copy to string
  for (reverse_index++; reverse_index <= 9; reverse_index++){
    string[index] = reverse_str[reverse_index];
    index ++;
  }
  return string;
}



















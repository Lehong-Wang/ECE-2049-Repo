# include "lab_3.h"


unsigned long time_count = 0;





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

void configScroll(void){

}

void configTimer(void);

void configLab3(void){

  ADC12CTL0 |= ADC12ENC;		    // Enable conversion

}


/**
 * @brief Get the Temprature ADC value
 * @return int 
 */
int getTempADC(void){
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
  char month[10] = "";
  int day = 0;
  // time 0 is day 1
  time ++;

  if (time <= 31){
    strcpy(month, "January");
    day = time;
  }
  else if (time <= 59){
    strcpy(month, "February");
    day = time - 31;
  }
  else if (time <= 90){
    strcpy(month, "March");
    day = time - 59;
  }
  else if (time <= 120){
    strcpy(month, "April");
    day = time - 90;
  }
  else if (time <= 151){
    strcpy(month, "May");
    day = time - 120;
  }
  else if (time <= 181){
    strcpy(month, "June");
    day = time - 151;
  }
  else if (time <= 212){
    strcpy(month, "July");
    day = time - 181;
  }
  else if (time <= 243){
    strcpy(month, "August");
    day = time - 212;
  }
  else if (time <= 273){
    strcpy(month, "September");
    day = time - 243;
  }
  else if (time <= 304){
    strcpy(month, "October");
    day = time - 273;
  }
  else if (time <= 334){
    strcpy(month, "November");
    day = time - 304;
  }
  else if (time <= 365){
    strcpy(month, "December");
    day = time - 334;
  }
  else{
    strcpy(month, "ERROR");
    day = 0;
  }

  struct Time result_time;
  result_time.total_time = total_time;
  result_time.seconds = second;
  result_time.minutes = minute;
  result_time.hours = hour;
  result_time.days = day;
  strcpy(result_time.month, month);

  return result_time;
}



void displayWholeTime(unsigned long total_time){
  Graphics_clearDisplay(&g_sContext);

  struct Time parsed_time = parseTime(total_time);

  char* second_str = intToChar(parsed_time.seconds);
  Graphics_drawStringCentered(&g_sContext, "Second:", AUTO_STRING_LENGTH, 30, 20, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, second_str, 2, 70, 20, TRANSPARENT_TEXT);
  free(second_str);

  char* minute_str = intToChar(parsed_time.minutes);
  Graphics_drawStringCentered(&g_sContext, "Minute:", AUTO_STRING_LENGTH, 30, 30, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, minute_str, 2, 70, 30, TRANSPARENT_TEXT);
  free(minute_str);

  char* hour_str = intToChar(parsed_time.hours);
  Graphics_drawStringCentered(&g_sContext, "Hour:", AUTO_STRING_LENGTH, 30, 40, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, hour_str, 2, 70, 40, TRANSPARENT_TEXT);
  free(hour_str);

  char* day_str = intToChar(parsed_time.days);
  Graphics_drawStringCentered(&g_sContext, "Day:", AUTO_STRING_LENGTH, 30, 50, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, day_str, 2, 70, 50, TRANSPARENT_TEXT);
  free(day_str);

  Graphics_drawStringCentered(&g_sContext, "Month:", AUTO_STRING_LENGTH, 30, 60, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, parsed_time.month, AUTO_STRING_LENGTH, 70, 60, TRANSPARENT_TEXT);

  Graphics_flushBuffer(&g_sContext);
}





/**
 * @brief convert a given interger to a displayable string
 *          need to free memory after use
 * @param number    number should be less than 10 digit, can be negative
 * @return char*    pointer to string, need to free memory after use
 */
char* intToChar(int number){
  // if return string as char*, need to use malloc
  char* string = malloc(10);
  memset(string, 0x20, 10);   // set all to space to pervent string ending
  char reverse_str[10] = "";
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



















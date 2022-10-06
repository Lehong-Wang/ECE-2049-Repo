#include <msp430.h>

#include <stdio.h>
#include <stdlib.h>

#include "peripherals.h"

#include "lab_2.h"
#include "lab_3.h"
// Function Prototypes
void init(void);
// void InitTimerA2(void);

// extern unsigned long timer = 0;
// extern unsigned long interval_counter = 0;
unsigned long timer = 0;
unsigned long interval_counter = 0;
unsigned long total_time_count = 0;

// Declare globals here
const char demo_song[] = {0x11, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00};
// unsigned char line3[];


extern unsigned int left_button_press_count;


// Main
void main(void)

{

  init();
  InitTimerA2();
  configLab3();

  mainLoopLab3();

  //  configTemp();
//  configScroll();
  // configADC();
  // configButton();
  // ADC12CTL0 |= ADC12ENC;		    // Enable conversion

  // bool is_c = false;
  // int adc = 0;
  // int value = 0;
  // char s[4] = {0};

  // Graphics_clearDisplay(&g_sContext);

  // Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
  // Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
  // Graphics_drawStringCentered(&g_sContext, "GITAR HERO", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
  // Graphics_drawStringCentered(&g_sContext, "Press * to start", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

  // Graphics_flushBuffer(&g_sContext);
  // swDelay(5);


  //  while (1){
  //     struct Time time = parseTime(total_time_count/100);
  //     displayDate(time);
  //     swDelay(2);
  //     displayTime(time);
  //     swDelay(2);

  //  }

    // if (getPressedButton() == 1)
    //   left_button_press_count++;

    // displayEditScreen();






    //     int pressed = 0;

    //     pressed = getPressedButton();

    //     LightLED(pressed);

    //     if (timer%10 == 0)
    //       LightLED(1);

    //     int a = getScrollADC();
    //     int b = getTempADC();
    //     __no_operation();                       // SET BREAKPOINT HERE

    //     displayNewTemp(is_c);
    //     is_c = !is_c;
    //     swDelay(1);

    // adc = getScrollADC();
    // value = mapToDateSelection(adc, 1);
    // char* str = intToChar(value);
    // strncpy(s, str, 2);

    // Graphics_clearDisplay(&g_sContext);
    // Graphics_drawStringCentered(&g_sContext, s, 2, 48, 45, 1);
    // Graphics_flushBuffer(&g_sContext);
    // swDelay(2);

   


//     struct Time parsed_time = parseTime(total_time_count);
//     displayWholeTime(parsed_time);
//     swDelay(3);




   

    // int adc = getScrollADC();

    // displayEditScreen(adc);







    // displayNewTemp(is_c);
    // is_c = !is_c;
    // swDelay(1);

    // Graphics_drawStringCentered(&g_sContext, "  ##   ", 5, 48, 25, 1);
    // Graphics_drawStringCentered(&g_sContext, "     ", 5, 48, 45, 1);
    // Graphics_flushBuffer(&g_sContext);




  



  // int a = timer;
  // int b = interval_counter;
  // swDelay(1);

  // Graphics_clearDisplay(&g_sContext);

  // Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
  // Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
  // Graphics_drawStringCentered(&g_sContext, "GITAR HERO", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
  // Graphics_drawStringCentered(&g_sContext, "Press * to start", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

  // Graphics_flushBuffer(&g_sContext);

  // swDelay(1);
  // InitTimerA2();

  // mainLoopLab2();


  //   int a = 1;
  //   char disp[3] = "   ";
  //   disp[1] = a+'0';
  //   char* d = malloc(4);
  //   memset(d, 0x30, 4);
  //   char c[4] = "";
  //   d[0] = 'h';
  ////   d[1] = 'h';
  //   d[2] = 'h';
  //   d[3] = 'h';
  //   strncpy(c,d,4);
  //
  ////   char d[2] = "12";
  //   Graphics_clearDisplay(&g_sContext);
  //   Graphics_drawStringCentered(&g_sContext, disp, 3, 48, 40, TRANSPARENT_TEXT);
  //   Graphics_drawStringCentered(&g_sContext, d, 4, 48, 60, TRANSPARENT_TEXT);
  //   Graphics_drawStringCentered(&g_sContext, c, 4, 48, 70, TRANSPARENT_TEXT);
  //   Graphics_flushBuffer(&g_sContext);


  // displayWholeTime(26000000);






}



// Initialize
void init(void){
WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                           // You can then configure it properly, if desired
// Useful code starts here
initLeds();

configDisplay();
configKeypad();

// *** Intro Screen ***
Graphics_clearDisplay(&g_sContext); // Clear the display

}






// /**
// * Set Timer A2 to 32768 Hz
// * Interrupt every 0.01 seconds
// */
// void InitTimerA2(void){

//    TA2CTL = TASSEL_1 + MC_1 + ID_0;
//    TA2CCR0 = 327;       // 327+1 = 328 ACLK tics = ~1/100 seconds
//    TA2CCTL0 = CCIE;
//    _BIS_SR(GIE);
// }

// /**
// * Interrupt Service Rutinue
// * timer increase every 0.1 seconds
// */
// #pragma vector=TIMER2_A0_VECTOR
// __interrupt void TIMER2_A0_ISR (void)

// {
//    interval_counter ++;
//    if (interval_counter > 10){
//        interval_counter = 0;
//        timer ++;
//        P6OUT ^= (BIT4);

//    }
// }







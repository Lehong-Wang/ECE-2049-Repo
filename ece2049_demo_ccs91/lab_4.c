
# include "lab_4.h"






// Example code form Lecture 20 notes

// ... etc ... 
/** ********************************************** 
 * Initialize the DAC and its associated SPI bus, 
 * using parameters defined in peripherals.h 
 ************************************************/ 
void DACInit(void) 
{ 
 // Configure LDAC and CS for digital IO outputs 
 DAC_PORT_LDAC_SEL &= ~DAC_PIN_LDAC; 
 DAC_PORT_LDAC_DIR |=  DAC_PIN_LDAC; 
 DAC_PORT_LDAC_OUT |= DAC_PIN_LDAC; // Deassert LDAC 
 
 DAC_PORT_CS_SEL   &= ~DAC_PIN_CS; 
 DAC_PORT_CS_DIR   |=  DAC_PIN_CS; 
 DAC_PORT_CS_OUT   |=  DAC_PIN_CS;  // Deassert CS 
} 
 




void DACSetValue(unsigned int dac_code) 
{ 
 // Start the SPI transmission by asserting CS (active low) 
 // This assumes DACInit() already called 
 DAC_PORT_CS_OUT &= ~DAC_PIN_CS; 
 
 // Write in DAC configuration bits. From DAC data sheet 
 // 3h=0011 to highest nibble. 
 // 0=DACA, 0=buffered, 1=Gain=1, 1=Out Enbl 
 dac_code |= 0x3000;   // Add control bits to DAC word 
 
 uint8_t lo_byte = (unsigned char)(dac_code & 0x00FF); 
 uint8_t hi_byte = (unsigned char)((dac_code & 0xFF00) >> 8);   
 
 // First, send the high byte 
 DAC_SPI_REG_TXBUF = hi_byte; 
 
 // Wait for the SPI peripheral to finish transmitting 
 while(!(DAC_SPI_REG_IFG & UCTXIFG)) { 
  _no_operation(); 
 } 
 
 // Then send the low byte 
 DAC_SPI_REG_TXBUF = lo_byte; 
 
 // Wait for the SPI peripheral to finish transmitting 
 while(!(DAC_SPI_REG_IFG & UCTXIFG)) { 
  _no_operation(); 
 } 
 
 // We are done transmitting, so de-assert CS (set = 1) 
 DAC_PORT_CS_OUT |=  DAC_PIN_CS; 
 
 // This DAC is designed such that the code we send does not   
// take effect on the output until we toggle the LDAC pin.    
// This is because the DAC has multiple outputs. This design   
// enables a user to send voltage codes to each output and 
 // have them all take effect at the same time. 
 DAC_PORT_LDAC_OUT &= ~DAC_PIN_LDAC;  // Assert LDAC 
  __delay_cycles(10);                 // small delay 
 DAC_PORT_LDAC_OUT |=  DAC_PIN_LDAC;  // De-assert LDAC 
} 
 
    
 
 




/**
 * @brief generate different wave based on index
 *          addjust the time source and frequency to match ISR
 * @param index  (number of button on keyboard)
 */
void generateWave(int index){

  const long time = total_time_count;
  // timer frequency / resolution in Hz
  const int timer_frequency = 1000;

  switch(index){
    case 1:
      ADCWave(time, timer_frequency);
    break;

    case 2:
      squareWave(time, timer_frequency);
    break;

    case 3:
      sawtoothWave(time, timer_frequency);
    break;

    case 4:
      triangleWave(time, timer_frequency);
    break;

    case 5:
      dynamicTriangleWave(time, timer_frequency);
    break;

    case 9:
      DACSetValue(4095);
    break;

    default:
      DACSetValue(0);
    break;
  }

}




/**
 * @brief   const votage controled by scroll wheel
 * @param time  variable used to store time
 * @param timer_frequency   frequency of time variable update/ count per second
 */
void ADCWave(long time, int timer_frequency){
  const int adc_value = getScrollADC();
  DACSetValue(adc_value);
  const float measured_voltage = getA1Votage();
}




/**
 * @brief 100 Hz square wave with max voltage controlled by scroll wheel
 */
void squareWave(long time, int timer_frequency){
  // number of time tick in the wave being generated
  // calculated by timer frequency / wave frequency
  // square wave of 100 Hz
  const int period = timer_frequency / 100;
  // how much percent into the period
  const int progress = time % period;
  const int adc = getScrollADC();
  
  // square wave, first half full voltage, last half zero
  if (progress > period/2)
    DACSetValue(adc);
  else
    DACSetValue(0);
}



/**
 * @brief 50 Hz sawtooth wave
 */
void sawtoothWave(long time, int timer_frequency){
  // sawtooth wave of 50 Hz
  const int period = timer_frequency / 50;
  const int progress = time % period;
  // voltage proportional to progress
  const int dac_value = (int)((float)progress / period * 4095);

  DACSetValue(dac_value);
}



/**
 * @brief 50 Hz triangle wave
 */
void triangleWave(long time, int timer_frequency){
  // triangle wave of 50 Hz
  const int period = timer_frequency / 50;
  const int progress = time % period;
  // first half rise, second half fall
  // 0 - 4095, 4095 - 0
  const int dac_value = (int)((float)progress / period * 4095 * 2);

  if (progress < period/2)
    DACSetValue(dac_value);
  else
    DACSetValue(4095*2 - dac_value);
}


/**
 * @brief frequency of triangle wave is based on scroll wheel ADC
 *          100 Hz - 1000 Hz
 */
void dynamicTriangleWave(long time, int timer_frequency){
  const int adc_value = getScrollADC();
  // Hz from 100 - 1000
  // casting might not work
  const int hz = (int)((float)adc_value / 4095 * 900 + 100);
  const int period = timer_frequency / hz;
  const int progress = time % period;
  const int dac_value = (int)((float)progress / period * 4095 * 2);

  if (progress < period/2)
    DACSetValue(dac_value);
  else
    DACSetValue(4095*2 - dac_value);
}





/**
 * @brief main loop for lab 4
 */
void mainLoopLab4(void){
  dispWaveInfoScreen();
  int index = 0;
  while(1){
    index = getLastPressedKey();
    generateWave(index);
//    const float measured_voltage = getA1Votage();
  }
}



/**
 * @brief Info screen
 *  which button to press to generate what wave
 */
void dispWaveInfoScreen(void){
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, "Wave Generator", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "1.Scroll Votage", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "2.Square Wave", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "3.Sawtooth Wave", AUTO_STRING_LENGTH, 48, 50, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "4.Triangle Wave", AUTO_STRING_LENGTH, 48, 60, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "5.Scroll Freq", AUTO_STRING_LENGTH, 48, 70, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "9.Full Voltage", AUTO_STRING_LENGTH, 48, 80, TRANSPARENT_TEXT);
  Graphics_flushBuffer(&g_sContext);
}



/**
 * @brief Get the Last Pressed Key
 *          refresh everytime another key is pressed
 * @return int  index of key
 */
int getLastPressedKey(void){
  static int last_key = 0;
  const unsigned char current_key = getKey();
  if (current_key != 0)
    last_key = (int) (current_key - '0');
  return last_key;
}




/**
 * @brief configre the ADC used for this lab
 *        use this function ONLY
 */
void configLab4ADC(void){

  // scroll wheel use P6.0 which is A0
  P6SEL &= ~BIT0;
  P6DIR &= ~BIT0;
  // P6.1 is A1
  P6SEL &= ~BIT1;
  P6DIR &= ~BIT1;


   REFCTL0 &= ~REFMSTR; // Reset REFMSTR to hand over control of
                        // internal reference voltages to
   	  	  	  	  	    // ADC12_A control registers
  // ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
  ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON | ADC12MSC;     // Internal ref = 1.5V

//    ADC12CTL1 = ADC12SHP | ADC12CONSEQ0 | ADC12CSTARTADD0;                     // Enable sample timer, sequence of conversion, start at 0
   ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                     // Enable sample timer, sequence of conversion

   ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_1;   // Vref = AVcc, A1
   ADC12MCTL1 = ADC12SREF_0 + ADC12INCH_0 + ADC12EOS;   // ADC12INCH5 = Scroll wheel = A0
                                                        // ACD12SREF_0 = Vref+ = Vcc
   __delay_cycles(100);                      // delay to allow Ref to settle
}


/**
 * @brief Get the A1 pin (P6.1) ADC value
 * @return int 
 */
int getA1ADC(void){
  int adc = 0;
  ADC12CTL0 &= ~ADC12SC; 		// clear the start bit
  ADC12CTL0 |= ADC12SC;     // Sampling and conversion start
                            // Single conversion (single channel)
  // Poll busy bit waiting for conversion to complete
  while (ADC12CTL1 & ADC12BUSY)
    __no_operation();
  adc = ADC12MEM0 & 0x0fff;   // Read results if conversion done
  return adc;
}


/**
 * @brief   calculate voltage on A1
 * @return float  voltage
 */
float getA1Votage(void){
  const int adc = getA1ADC();
  const float full_votage = 3.3;
  float measured_votage = full_votage * (float)adc / 4096;
  return measured_votage;
}

/**
 * @brief wraper for all configs
 */
void configLab4(void){

  configLab4ADC();
  ADC12CTL0 |= ADC12ENC;		    // Enable conversion
  __delay_cycles(100); 
}

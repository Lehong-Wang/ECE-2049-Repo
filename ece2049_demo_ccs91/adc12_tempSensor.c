///* --COPYRIGHT--,BSD_EX
// * Copyright (c) 2012, Texas Instruments Incorporated
// * All rights reserved.
// *
// * Redistribution and use in source and binary forms, with or without
// * modification, are permitted provided that the following conditions
// * are met:
// *
// * *  Redistributions of source code must retain the above copyright
// *    notice, this list of conditions and the following disclaimer.
// *
// * *  Redistributions in binary form must reproduce the above copyright
// *    notice, this list of conditions and the following disclaimer in the
// *    documentation and/or other materials provided with the distribution.
// *
// * *  Neither the name of Texas Instruments Incorporated nor the names of
// *    its contributors may be used to endorse or promote products derived
// *    from this software without specific prior written permission.
// *
// * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// *
// *******************************************************************************
// *
// *                       MSP430 CODE EXAMPLE DISCLAIMER
// *
// * MSP430 code examples are self-contained low-level programs that typically
// * demonstrate a single peripheral function or device feature in a highly
// * concise manner. For this the code may rely on the device's power-on default
// * register values and settings such as the clock configuration and care must
// * be taken when combining code from several examples to avoid potential side
// * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
// * for an API functional library-approach to peripheral configuration.
// *
// * --/COPYRIGHT--*/
////******************************************************************************
////  MSP430F552x Demo - ADC12, Sample A10 Temp and Convert to oC and oF
////
////  Description: A single sample is made on A10 with reference to internal
////  1.5V Vref. Software sets ADC12SC to start sample and conversion - ADC12SC
////  automatically cleared at EOC. ADC12 internal oscillator times sample
////  and conversion. In Mainloop MSP430 waits in LPM4 to save power until
////  ADC10 conversion complete, ADC12_ISR will force exit from any LPMx in
////  Mainloop on reti.
////  ACLK = n/a, MCLK = SMCLK = default DCO ~ 1.045MHz, ADC12CLK = ADC12OSC
////
////  Uncalibrated temperature measured from device to devive will vary do to
////  slope and offset variance from device to device - please see datasheet.
////
////  NOTE:1.REFMSTR bit in REFCTL0 regsiter is reset to allow the ADC12_A reference
////    control regsiters handle the reference setting. Upon resetting the REFMSTR
////    bit, all the settings in REFCTL are 'dont care' and the legacy ADC12
////    control bits (ADC12REFON, ADC12REF2_5, ADC12TCOFF and ADC12REFOUT) control
////    the reference system.
////    2. Use the TLV calibrated temperature to measure temperature
////   (the TLV CALIBRATED DATA IS STORED IN THE INFORMATION SEGMENT, SEE DEVICE DATASHEET)
////
////                MSP430F552x
////             -----------------
////         /|\|              XIN|-
////          | |                 |
////          --|RST          XOUT|-
////            |                 |
////            |A10              |
////
////   F. Chen
////   Texas Instruments Inc.
////   Dec. 2012
////   Built with IAR Embedded Workbench Version: 5.51.1 & Code Composer Studio V5.2.1
////******************************************************************************
//
//#include <msp430.h>
//
//// Temperature Sensor Calibration = Reading at 30 degrees C is stored at addr 1A1Ah
//// See end of datasheet for TLV table memory mapping
//#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
//// Temperature Sensor Calibration = Reading at 85 degrees C is stored at addr 1A1Ch                                            //See device datasheet for TLV table memory mapping
//#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)
//
//unsigned int in_temp;
//
//int main(void)
//{
//  volatile float temperatureDegC;
//  volatile float temperatureDegF;
//  volatile float degC_per_bit;
//  volatile unsigned int bits30, bits85;
//
//  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
//
//  REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
//                          // internal reference voltages to
//  	  	  	  // ADC12_A control registers
//
//  ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON;     // Internal ref = 1.5V
//
//  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
//
//  // Using ADC12MEM0 to store reading
//  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense
//  	                                    // ACD12SREF_1 = internal ref = 1.5v
//
//  __delay_cycles(100);                    // delay to allow Ref to settle
//  ADC12CTL0 |= ADC12ENC;           	  // Enable conversion
//
//  // Use calibration data stored in info memory
//  bits30 = CALADC12_15V_30C;
//  bits85 = CALADC12_15V_85C;
//  degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));
//
//
//  while(1)
//  {
//    ADC12CTL0 &= ~ADC12SC; 	// clear the start bit
//    ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
//    			        // Single conversion (single channel)
//
//    // Poll busy bit waiting for conversion to complete
//    while (ADC12CTL1 & ADC12BUSY)
//    	__no_operation();
//    in_temp = ADC12MEM0;      // Read in results if conversion
//
//    // Temperature in Celsius. See the Device Descriptor Table section in the
//    // System Resets, Interrupts, and Operating Modes, System Control Module
//    // chapter in the device user's guide for background information on the
//    // formula.
//    temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit +30.0;
//
//    // Temperature in Fahrenheit
//    temperatureDegF = (temperatureDegC * 9/5) + 32.0;
//
//    __no_operation();                       // SET BREAKPOINT HERE
//  }
//}

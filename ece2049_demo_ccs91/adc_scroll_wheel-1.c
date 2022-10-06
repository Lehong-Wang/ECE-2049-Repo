// /* --COPYRIGHT--,BSD_EX
//  * Copyright (c) 2012, Texas Instruments Incorporated
//  * All rights reserved.
//  *
//  * Redistribution and use in source and binary forms, with or without
//  * modification, are permitted provided that the following conditions
//  * are met:
//  *
//  * *  Redistributions of source code must retain the above copyright
//  *    notice, this list of conditions and the following disclaimer.
//  *
//  * *  Redistributions in binary form must reproduce the above copyright
//  *    notice, this list of conditions and the following disclaimer in the
//  *    documentation and/or other materials provided with the distribution.
//  *
//  * *  Neither the name of Texas Instruments Incorporated nor the names of
//  *    its contributors may be used to endorse or promote products derived
//  *    from this software without specific prior written permission.
//  *
//  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
//  * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//  * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  *
//  *******************************************************************************
//  *
//  *                       MSP430 CODE EXAMPLE DISCLAIMER
//  *
//  * MSP430 code examples are self-contained low-level programs that typically
//  * demonstrate a single peripheral function or device feature in a highly
//  * concise manner. For this the code may rely on the device's power-on default
//  * register values and settings such as the clock configuration and care must
//  * be taken when combining code from several examples to avoid potential side
//  * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
//  * for an API functional library-approach to peripheral configuration.
//  *
//  * --/COPYRIGHT--*/
//
// #include <msp430.h>
//#include "peripherals.h"
//
//
// unsigned int scroll;
// unsigned int temp;
//
// int main(void)
// {
//   WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//
//   // Configure P8.0 as digital IO output and set it to 1
//   // This supplied 3.3 volts across scroll wheel potentiometer
//   // See schematic at end or MSP-EXP430F5529 board users guide
//   P8SEL &= ~BIT0;
//   P8DIR |= BIT0;
//   P8OUT |= BIT0;
//
//   P6SEL &= ~BIT0;
//   P6DIR &= ~BIT0;
//
//
//   REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control of
//                                             // internal reference voltages to
//   	  	  	  	  	    // ADC12_A control registers
//   // ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
//  ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON | ADC12MSC;     // Internal ref = 1.5V
//
////    ADC12CTL1 = ADC12SHP | ADC12CONSEQ0 | ADC12CSTARTADD0;                     // Enable sample timer, sequence of conversion, start at 0
//   ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                     // Enable sample timer, sequence of conversion, start at 0
//
//
//   ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense
//  	                                    // ACD12SREF_1 = internal ref = 1.5v
//   // Use ADC12MEM0 register for conversion results
//   ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_0 + ADC12EOS;   // ADC12INCH5 = Scroll wheel = A5
//                        // ACD12SREF_0 = Vref+ = Vcc
////    ADC12MCTL2 = ADC12EOS;
//   __delay_cycles(100);                      // delay to allow Ref to settle
//   ADC12CTL0 |= ADC12ENC;		    // Enable conversion
//
//
//
//   while(1)
//   {
//     ADC12CTL0 &= ~ADC12SC; 		// clear the start bit
//     ADC12CTL0 |= ADC12SC;               // Sampling and conversion start
//     					// Single conversion (single channel)
//
//     // Poll busy bit waiting for conversion to complete
//     while (ADC12CTL1 & ADC12BUSY)
//     	__no_operation();
////     in_value = ADC12MEM0 & 0x0fff;               // Read results if conversion done
//     scroll = ADC12MEM1;               // Read results if conversion done
//     temp = ADC12MEM0;
//     __no_operation();                       // SET BREAKPOINT HERE
//   }
// }
//
//

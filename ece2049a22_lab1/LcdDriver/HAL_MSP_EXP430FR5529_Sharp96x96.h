/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

 //*****************************************************************************
//
// HAL_MSP-EXP430F5529_Sharp96x96.h - Prototypes for the Sharp96x96 LCD display driver.
//									   There is no output from Sharp96x96 LCD.
//
//                 MSP430F5529                    LCD Sharp96x96
//                -----------------             -----------------
//               |     UCB0CLK/P3.2|---------> |SCLK     EXTCOMIN|-- 0
//            /|\|                 |           |            	 |
//             | |    UCB0SIMO/P3.0|---------> |SI        EXTMODE|-- 0
//             --|RST              |           |              	 |
//               |             P6.6|---------> |SCS              |
//               |             	   |		   |         	     |
//               |             P1.6|---------> |DISP             |
//               |                 |		   |                 |
//               |                 | 	       |                 |
//               |                 |            -----------------
//
//*****************************************************************************

#ifndef __HAL_MSP_EXP430F5529_SHARPLCD_H__
#define __HAL_MSP_EXP430F5529_SHARPLCD_H__

#include<msp430.h>

#ifdef USE_DRIVERLIB
#include "inc/hw_memmap.h"
#include "../driverlibHeaders.h"
#endif

//*****************************************************************************
//
// Parameters for tuning the performance characteristics of this driver
//
//*****************************************************************************
// Use only one manual draw call for the entire screen (using GrFlush()) instead
// of drawing to the display multiple times for each primitive.
#define SINGLE_DRAW

// Ensure that command and data writes to display are atomic by disabling ISRs
// before sending data.
//#define USE_ATOMIC_DRAW

// Use TI's driver library for all GPIO, SPI, and Timer operations
//#define USE_DRIVERLIB


//*****************************************************************************
//
// Pin definitions for LCD driver
//
//*****************************************************************************

/*
 * Pin assignment is as follows:
 * Power		P6.5
 * DISP			P1.6
 * CS			P6.6
 * MOSI			P3.0
 * SCLK			P3.2
 */
#define PORT_SPI_SEL						P3SEL
#define PORT_SPI_DIR						P3DIR
#define PORT_DISP_SEL						P1SEL
#define PORT_DISP_DIR						P1DIR
#define PORT_DISP_OUT						P1OUT

#define PORT_CS_SEL							P6SEL
#define PORT_CS_DIR							P6DIR
#define PORT_CS_OUT							P6OUT

#define PORT_PWR_SEL						P6SEL
#define PORT_PWR_DIR						P6DIR
#define PORT_PWR_OUT						P6OUT



#define PIN_SPI_MOSI						BIT0
#define PIN_SPI_SCLK						BIT2
#define PIN_PWR								BIT5
#define PIN_DISP							BIT6
#define PIN_CS								BIT6

#define SPI_REG_CTL0	UCB0CTL0
#define SPI_REG_CTL1	UCB0CTL1
#define SPI_REG_BRL		UCB0BR0
#define SPI_REG_BRH		UCB0BR1
#define SPI_REG_IFG		UCB0IFG
#define SPI_REG_STAT	UCB0STAT
#define SPI_REG_TXBUF	UCB0TXBUF
#define SPI_REG_RXBUF	UCB0RXBUF

/*
 * UCSI SPI Clock parameters
 * The actual clock frequency is given in number of
 * ticks of the specified clock source.
 *
 * For our configuration, we use the same frequency
 * as SMCLK, which is 1.25MHz.
 */
#define SPI_CLK_SRC		(UCSSEL__SMCLK)
#define SPI_CLK_TICKS	0

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   96
#define LCD_HORIZONTAL_MAX                 96

#ifdef USE_DRIVERLIB
// Ports from MSP430 connected to LCD
#define LCD_MOSI_PORT                       GPIO_PORT_P3
#define LCD_SCLK_PORT                       GPIO_PORT_P3
#define LCD_DISP_PORT                       GPIO_PORT_P1
#define LCD_POWER_PORT                      GPIO_PORT_P6
#define LCD_SCS_PORT	                    GPIO_PORT_P6

// Pins from MSP430 connected to LCD
#define LCD_MOSI_PIN                        GPIO_PIN0
#define LCD_SCLK_PIN                        GPIO_PIN2
#define LCD_POWER_PIN                       GPIO_PIN5
#define LCD_DISP_PIN                        GPIO_PIN6
#define LCD_SCS_PIN                         GPIO_PIN6

#endif


//*****************************************************************************
//
// Clears CS line
//
// This macro allows to clear the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
#ifdef USE_DRIVERLIB
#define DeassertCS()  	GPIO_setOutputLowOnPin(LCD_SCS_PORT, LCD_SCS_PIN)
#else
#define DeassertCS()   PORT_CS_OUT &= ~PIN_CS
#endif



//*****************************************************************************
//
// Set CS line
//
// This macro allows to set the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
#ifdef USE_DRIVERLIB
#define AssertCS()    GPIO_setOutputHighOnPin(LCD_SCS_PORT, LCD_SCS_PIN)
#else
#define AssertCS()    PORT_CS_OUT |= PIN_CS
#endif

//*****************************************************************************
//
// Waits until the SPI communication with the LCD is finished a command to
// the LCD Driver
//
// \param None
//
// \return None
//*****************************************************************************
#ifdef USE_DRIVERLIB
#define WaitUntilLcdWriteFinished()                \
        while  (USCI_B_SPI_isBusy(USCI_B0_BASE))
#else
#define WaitUntilLcdWriteFinished()					\
		do {										\
				while(SPI_REG_STAT & UCBUSY);		\
			} while (0)
#endif


//*****************************************************************************
//
// Writes command or data to the LCD Driver
//
// \param ucCmdData is the 8 or 16 bit command to send to the LCD driver
// Uses the SET_LCD_DATA macro
//
// \return None
//
//*****************************************************************************
#ifdef USE_DRIVERLIB
#define WriteCmdData(ucCmdData)                    	                 \
        do                                         	                 \
        {                                                            \
			while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,      \
                    USCI_B_SPI_TRANSMIT_INTERRUPT));                 \
            	USCI_B_SPI_transmitData(USCI_B0_BASE,ucCmdData);   	 \
        }                                                            \
        while(0)
#else
#define WriteCmdData(ucCmdData)										\
	do																\
	{																\
		/* Wait for the TX buffer to be ready */					\
		while(!(SPI_REG_IFG & UCTXIFG));							\
																	\
		/* Send the data */											\
		SPI_REG_TXBUF = ucCmdData;									\
	} while(0)
#endif

//*****************************************************************************
//
// Prepare to write memory
//
// This macro unlocks flash memory controller and
// sets access right on flash controller
//
// \return None
//
//*****************************************************************************
#define PrepareMemoryWrite()					\
	__no_operation();

//*****************************************************************************
//
// Finish memory writing
//
// This macro removes access rights on flash controller and
// locks flash memory controller.
//
// \return None
//
//*****************************************************************************
#define FinishMemoryWrite()						\
	__no_operation();

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Sharp96x96_Init(void);
#endif // __HAL_MSP-EXP430F5529_SHARPLCD_H__

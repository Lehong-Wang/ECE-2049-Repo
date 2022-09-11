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
// HAL_MSP-EXP430F5529_Sharp96x96.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <stdint.h>
#include "grlib.h"

#ifdef USE_DRIVERLIB
#include "inc/hw_memmap.h"
#include "../driverlibHeaders.h"
#endif

#include "HAL_MSP_EXP430FR5529_Sharp96x96.h"

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the Sharp96x96 display. This function
//! configures the GPIO pins used to control the LCD display when the basic
//! GPIO interface is in use. On exit, the LCD has been reset and is ready to
//! receive command and data writes.
//!
//! \return None.
//
//*****************************************************************************
void Sharp96x96_Init(void)
{
#ifdef USE_DRIVERLIB
	// Configure SCLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SCLK_PORT,
    											LCD_SCLK_PIN);

    // Configure MOSI
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_MOSI_PORT,
    											LCD_MOSI_PIN);

    // The LCD is powered using a GPIO pin, so configure it as output and drive it
	GPIO_setAsOutputPin(LCD_POWER_PORT,
	        LCD_POWER_PIN);
	
	GPIO_setOutputHighOnPin(LCD_POWER_PORT,
            LCD_POWER_PIN);
				
	// Turn on DISP, enabling the display
	GPIO_setAsOutputPin(LCD_DISP_PORT,
	        LCD_DISP_PIN);
	
	GPIO_setOutputHighOnPin(LCD_DISP_PORT,
            LCD_DISP_PIN);		
	
			
	// Configure LCD_SCS_PIN as output pin
    GPIO_setAsOutputPin(LCD_SCS_PORT,
    		LCD_SCS_PIN);
			
	DeassertCS();

	// Configure the SPI interface on USCI B0
    USCI_B_SPI_masterInit(USCI_B0_BASE,
                    USCI_B_SPI_CLOCKSOURCE_SMCLK,
					8000000,
                    1000000,
                    USCI_B_SPI_MSB_FIRST,
                    USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
                    USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW);

    USCI_B_SPI_enable(USCI_B0_BASE);

#else
	// Configure SCLK and MOSI for peripheral mode
	PORT_SPI_SEL |= (PIN_SPI_MOSI|PIN_SPI_SCLK);

	// The LCD is powered using a GPIO pin, so configure it as output
	PORT_PWR_SEL &= ~PIN_PWR;
	PORT_PWR_DIR |=  PIN_PWR;

	// Drive the power pin high to power the LCD
	PORT_PWR_OUT |=  PIN_PWR;

	// Configure the display enable signal DISP, drive it high to enable the LCD
	PORT_DISP_SEL &= ~PIN_DISP;
	PORT_DISP_DIR |=  PIN_DISP;
	PORT_DISP_OUT |=  PIN_DISP;

	// Configure the display chip select as an output
	PORT_CS_SEL &=  ~PIN_CS;
	PORT_CS_DIR |=   PIN_CS;

	// Initialize the chip select in a deasserted state
	DeassertCS();

	// Now configure UCSIB0 to function as our SPI controller

	// Disable the module so we can configure it
	SPI_REG_CTL1 |= UCSWRST;

	SPI_REG_CTL0 &= ~(UCCKPH|UCCKPL|UC7BIT|UCMSB); // Reset the controller config parameters
	SPI_REG_CTL1 &= ~UCSSEL_3; // Reset the clock configuration

	SPI_REG_CTL1 |= SPI_CLK_SRC; // Select SMCLK for our clock source

	// Set SPI clock frequency (which is the same frequency as SMCLK so this can apparently be 0)
	SPI_REG_BRL  =  ((uint16_t)SPI_CLK_TICKS) & 0xFF;         // Load the low byte
	SPI_REG_BRH  = (((uint16_t)SPI_CLK_TICKS) >> 8) & 0xFF;	  // Load the high byte

	// Configure for SPI master, synchronous, 3 wire SPI, MSB first, capture data on first edge,
	// and inactive low polarity
	SPI_REG_CTL0 |= (UCMST|UCSYNC|UCMODE_0|UCMSB|UCCKPH);

	// Reenable the module
	SPI_REG_CTL1 &= ~UCSWRST;
	SPI_REG_IFG  &= ~UCRXIFG;
#endif
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

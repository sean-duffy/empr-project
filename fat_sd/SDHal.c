/*
 * SDHal.cpp
 *
 *  Created on: May 20, 2011
 *      Author: Sagar
 *  Added to by PCC
 */
#include "stdio.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

#include "SDHal.h"

void spiInit()
{
	if(!get_system_state(SYSTEM_STATE_SDCARD_UP)){
		LPC_SC->PCONP |= 1 << 21; 				// SSP0
		LPC_SC->PCLKSEL0 |= 1 << 10; 				// SSP0_PCLK = CCLK
	
	
		LPC_PINCON->PINSEL0 &= ~(0x03 << 30);
		LPC_PINCON->PINSEL0 |= (0x02 << 30); 			// P0.15 as SCK0
	
		LPC_PINCON->PINSEL1 &= ~(0x3F); 			// clear all bits relevant to SSP0
		LPC_PINCON->PINSEL1 |= (0x02 << 2) | (0x02 << 4); 	// MISO0 and MOSI0 on P0.17 and P0.18 , P0.16 is GPIO , used as /CS manually
		LPC_GPIO0->FIODIR |= 1 << 16; 				// P0.16 in O/P mode.
		csHi();
	
		LPC_SSP0->CR0 |= 7 | ( 0xFF << 8) ; 			// 8 bitmode, CPOL = CPHA = 0 , SCR = 255. 
									// freq = PCLK /( 2* SCR). About 200kHz for PCLK = 100MHz
		LPC_SSP0->CPSR = 2;
		LPC_SSP0->CR1 = 1 << 1; 				// enable SSP0

		set_system_state(SYSTEM_STATE_SDCARD_UP);
	}
}
void spiLoSpeed()
{
	LPC_SSP0->CR1 = 0;					// turn OFF SSP0
	LPC_SSP0->CR0 |= 0xFF << 8;
	LPC_SSP0->CR1 = 1 << 1; 				// enable SSP0
}
void spiHiSpeed()
{
	LPC_SSP0->CR1 = 0;					// turn OFF SSP0
	LPC_SSP0->CR0 &= ~(0xFF << 8);
	LPC_SSP0->CR0 |= 0x01 << 8; 				// About 16.66Mhz for PCLK = 100MHz
	LPC_SSP0->CR1 = 1 << 1; 				// enable SSP0
}

uint8_t spiWrite(uint8_t data)
{
	uint8_t temp;
	LPC_SSP0->DR = data;
	while((LPC_SSP0->SR & (1 << 2)) == 0); 			// wait until byte is received
	temp = LPC_SSP0->DR;
	return temp;
}
void csLo()
{
	GPIO_ClearValue(0,((uint32_t)1<<11));			// Use the CMSIS code
//	LPC_GPIO0->FIOPIN &= ~(1 << 16);
}
void csHi()
{
	GPIO_SetValue(0,((uint32_t)1<<11));			// Use the CMSIS code
//	LPC_GPIO0->FIOPIN |= 1 << 16;
}


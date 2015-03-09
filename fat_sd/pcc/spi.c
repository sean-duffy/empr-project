/* Jumentum-SOC

  Copyright (C) 2007 by Daniel Marks

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Daniel L. Marks profdc9@gmail.com

*/

/* Support for SSP added by DLM */

/* BITBANG SPI support mostly added as a hack for LPC2378 to access SD/MMC
   card through the MMC/SD port since I don't know how to control it using
   their interface yet, and I would have to write my own driver to interface
   to ELM FAT FS */

#include "lpc17xx.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "stdio.h"
#include "types.h"
#include "stdlib.h"
#define LPC17XX_SSP1
#define CS_PORT_NUM 0
#define CS_PIN_NUM 11
#define SSPx LPC_SSP1



   


#include "spi.h"

void CS_Set(int32_t state)
{
	if (state){
		GPIO_SetValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
	}else{
		GPIO_ClearValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
	}
}

void spiInit(void)
{
	PINSEL_CFG_Type PinCfg;

	SSP_CFG_Type SSP_ConfigStruct;

	/*
	 * Initialize SPI pin connect
	 * P0.15 - SCK
	 * P0.16 - SSEL - used as GPIO
	 * P0.17 - MISO
	 * P0.18 - MOSI
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
#ifdef LPC17XX_SSP1
	PinCfg.Pinnum = 7;
#else	
	PinCfg.Pinnum = 15;
#endif
	PINSEL_ConfigPin(&PinCfg);
#ifdef LPC17XX_SSP1
	PinCfg.Pinnum = 8;
#else
	PinCfg.Pinnum = 17;
#endif
	PINSEL_ConfigPin(&PinCfg);
#ifdef LPC17XX_SSP1
	PinCfg.Pinnum = 9;
#else
	PinCfg.Pinnum = 18;
#endif
	PINSEL_ConfigPin(&PinCfg);
#ifdef LPC17XX_SSP1
	PinCfg.Pinnum = 6;
#else
//	PinCfg.Pinnum = 16;
	PinCfg.Pinnum = 11;
#endif
	PinCfg.Funcnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	GPIO_SetDir(CS_PORT_NUM, (1<<CS_PIN_NUM), 1);
	GPIO_SetValue(CS_PORT_NUM, (1<<CS_PIN_NUM));

	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(SSPx, &SSP_ConfigStruct);
	// Enable SSP peripheral
	SSP_Cmd(SSPx, ENABLE);
}

short  SPIWrite(unsigned char  * ptrBuffer, short  ui_Len)
{
  short  i,stat;

    for (i=0;i<ui_Len;i++)
    {
		while (!(SSPx->SR & SSP_SR_TNF));
		SSPx->DR = *ptrBuffer++;
		while (!(SSPx->SR & SSP_SR_RNE));
		stat = SSPx->DR;
	}
	return i;
}

/**********************************************************************/
short  SPIRead(unsigned char  * ptrBuffer, short  ui_Len)
{
  short  i,stat;

  for (i=0;i<ui_Len;i++)
  {
	while (!(SSPx->SR & SSP_SR_TNF));
	SSPx->DR = 0xFF;
	while (!(SSPx->SR & SSP_SR_RNE));
	*ptrBuffer = SSPx->DR;
	ptrBuffer++;
  }
  return i;
}

void spiMMCChipSelect (char select)
{
  if (select) {
	SSPx->CPSR = SPI_MMC_SPEED;	
	CS_Set(0);
  }
  else
  {
	CS_Set(1);
    while (!(SSPx->SR & SSP_SR_TNF));
    SSPx->DR = 0xff;
    //
    // Wait until TX fifo and TX shift buffer are empty
    //
    while (SSPx->SR & SSP_SR_BSY);
    while (!(SSPx->SR & SSP_SR_RNE));
    do
    {
      select = SSPx->DR;
    } while (SSPx->SR & SSP_SR_RNE);
  }
}

void spiSendByte (unsigned char c)
{
  while (!(SSPx->SR & SSP_SR_TNF));
  SSPx->DR = c;
  while (!(SSPx->SR & SSP_SR_RNE));
  c = SSPx->DR;
}

unsigned char spiReceiveByte (void)
{
  while (!(SSPx->SR & SSP_SR_TNF));
  SSPx->DR = 0xFF;
  while (!(SSPx->SR & SSP_SR_RNE));
  return SSPx->DR;
}

void spiReceivePtr (unsigned char *c)
{
  while (!(SSPx->SR & SSP_SR_TNF));
  SSPx->DR = 0xFF;
  while (!(SSPx->SR & SSP_SR_RNE));
  *c = SSPx->DR;
}

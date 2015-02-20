#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

#include "../I2C/i2c.h"
#include "../KEYPAD/keypad.h"
#include "../LCD/lcd.h"


#define noOfElements(x) (sizeof(x) / sizeof(x[0]))

void keypadInitInterrupt()
{	
	GPIO_IntCmd(0, 0x00800000, 1); //port zero, bit 23 mask, falling edge
	NVIC_EnableIRQ(EINT3_IRQn);
}

void keypadDisableInterrupt()
{	
	NVIC_DisableIRQ(EINT3_IRQn);
}

void keypadInit(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	//set all rows to high (rows are active low), and set up interrupt pin	

	PINSEL_CFG_Type PinCfg;

	
	PinCfg.OpenDrain = 0; //PINSEL_PINMODE_OPENDRAIN;
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	PinCfg.Funcnum = PINSEL_FUNC_3;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 9; 
	PINSEL_ConfigPin(&PinCfg);
	//set up pins as we want them
	uint8_t data[] = {0b00001111};	
	i2cWrite(LPC_I2C1, keypadAddr, data, noOfElements(data));
	keypadInitInterrupt(i2cPort, addr);

}

char keypadRead(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	//any button byte of first row ends in 0x7
	//second row = 0xb
	//third row = 0xd
	//fourth row = 0xe
	//0xf can be ignored

	uint8_t readValue[1];
	//first 4 bits are columns (active low)
	uint8_t data[1];	

	//turn on first column and scan rows
	data[0] = 0b01111111;
	i2cRead(LPC_I2C1, keypadAddr, data, noOfElements(data), readValue, 1);
	readValue[0] = readValue[0] & 0b00001111;

	if(readValue[0] == 0x7)
	{
		return '1';
	}

	if(readValue[0] == 0xb)
	{
		return '4';
	}
	
	if(readValue[0] == 0xd)
	{
		return '7';
	}
	
	if(readValue[0] == 0xe)
	{
		return '*';
	}

	//turn on second column and scan rows
	data[0] = 0b10111111;
	i2cRead(LPC_I2C1, keypadAddr, data, noOfElements(data), readValue, 1);
	readValue[0] = readValue[0] & 0b00001111;
	if(readValue[0] == 0x7)
	{
		return '2';
	}

	if(readValue[0] == 0xb)
	{
		return '5';
	}
	
	if(readValue[0] == 0xd)
	{
		return '8';
	}
	
	if(readValue[0] == 0xe)
	{
		return '0';
	}
	
	//turn on third column and scan rows
	data[0] = 0b11011111;
	i2cRead(LPC_I2C1, keypadAddr, data, noOfElements(data), readValue, 1);
	readValue[0] = readValue[0] & 0b00001111;
	if(readValue[0] == 0x7)
	{
		return '3';
	}

	if(readValue[0] == 0xb)
	{
		return '6';
	}
	
	if(readValue[0] == 0xd)
	{
		return '9';
	}
	
	if(readValue[0] == 0xe)
	{
		return '#';
	}

	//turn on fourth column and scan rows
	data[0] = 0b11101111;
	i2cRead(LPC_I2C1, keypadAddr, data, noOfElements(data), readValue, 1);
	readValue[0] = readValue[0] & 0b00001111;
	if(readValue[0] == 0x7)
	{
		return 'A';
	}

	if(readValue[0] == 0xb)
	{
		return 'B';
	}
	
	if(readValue[0] == 0xd)
	{
		return 'C';
	}
	
	if(readValue[0] == 0xe)
	{
		return 'D';
	}

	return '\0';
}

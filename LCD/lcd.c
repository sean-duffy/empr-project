#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "../I2C/i2c.h"
#include "lcd.h"



void lcdPrintChar(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t data)
{
	uint8_t val[] = {0x40, data};
	i2cWrite(i2cPort, addr, val, 2);
}

void lcdWrite(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t data)
{
	uint8_t val[] = {0x00, data};
	i2cWrite(i2cPort, addr, val, 2);
}

void delay(int multiplier)
{
	int i;
	for(i = 0 ; i < 96000 * multiplier ; i++);
}

void lcdInit(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t displayCursor)
{
	lcdWrite(i2cPort, addr, 0x35); //function set - 2 lines, no shift?, 
	isBusyWait(i2cPort, addr);
	//delay(2);

	lcdWrite(i2cPort, addr, 0x9F); //sets vlcd
	isBusyWait(i2cPort, addr);
	//delay(2);

	lcdWrite(i2cPort, addr, 0x34); //function set 2 lines
	isBusyWait(i2cPort, addr);
	//delay(2);

	//lcdWrite(i2cPort, addr, 0x07); //shift settings
	//isBusyWait(i2cPort, addr);
	//delay(2);

	if(displayCursor)
	{
		lcdWrite(i2cPort, addr, 0x0F); //as above, but cursor and blink on
		isBusyWait(i2cPort, addr);
		//delay(2);
	}
	else
	{
		lcdWrite(i2cPort, addr, 0x0C); //display control - sets display on, sets cursor off, and sets cursor blink off
	}
	
	lcdWrite(i2cPort, addr, 0x02); //move cursor to home (external code can use method for this)
	isBusyWait(i2cPort, addr);
	//delay(2);	

	clearDisplay(i2cPort, addr);
}

void clearDisplay(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	lcdWrite(i2cPort, addr, 0x01);

	/*char flagVal[30];
	int bacVal = readBACRegister(i2cPort, addr) & 0x80;
	int flagValLength = sprintf(flagVal, "Value: %x\r\n", bacVal);
	write_usb_serial_blocking(flagVal, flagValLength);*/

	isBusyWait(i2cPort, addr);

}

void resetCursor(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	lcdWrite(i2cPort, addr, 0x02);
	isBusyWait(i2cPort, addr);
}

void writeMessage(LPC_I2C_TypeDef* i2cPort, uint8_t addr, char* message, int blankSecondLine)
{
	int i = 0;
	//resetCursor(i2cPort, addr);
	clearDisplay(i2cPort, addr);
	while(message[i] != '\0')
	{
		/*char flagVal[30];
		int bacVal = readAddressCounter(i2cPort, addr);
		int flagValLength = sprintf(flagVal, "Value: %x\r\n", bacVal);
		write_usb_serial_blocking(flagVal, flagValLength);*/
		if(i == 16 && !blankSecondLine)
		{	
			//set DDRAM to second line
			lcdWrite(i2cPort, addr, 0xC0);
		}
		lcdPrintChar(i2cPort, addr, mapCharToLcdInt(message[i]));
		i++;
	}

	if(sizeof(message) < 32)
	{
		while(i < 32)
		{
			if(i == 16)
			{	
				//set DDRAM to second line
				lcdWrite(i2cPort, addr, 0xC0);
			}

			lcdPrintChar(i2cPort, addr, mapCharToLcdInt(' '));
			i++;
		}
	}
}

void enableDisplay(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	lcdWrite(i2cPort, addr, 0x0F);
	isBusyWait(i2cPort, addr);
}

void targetDDRAMZero(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	lcdWrite(i2cPort, addr, 0x80);
	isBusyWait(i2cPort, addr);
}

void setDDRAMAddress(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t targetAddr)
{
	lcdWrite(i2cPort, addr, targetAddr);
	isBusyWait(i2cPort, addr);
}


#define EXCLAM 0b10100001
#define QUOTE EXCLAM+1
#define HASH EXCLAM+2
#define PERCENT EXCLAM+4
#define AMP EXCLAM+5
#define SQUOTE EXCLAM+6
#define LBRAC EXCLAM+7
#define RBRAC EXCLAM+8
#define AST EXCLAM+9
#define PLUS EXCLAM+10
#define COMMA EXCLAM+11
#define MINUS EXCLAM+12
#define DOT EXCLAM+13
#define SLASH EXCLAM+14

#define EQUALS 0b10111101

#define LOWERCASESTART 0b11100001 //(a)

#define UPPERCASESTART 0b11000001 //(A)

#define NUMBERSTART 0b10110000 //(0)

#define SPACE 0b10100000 //( )

int mapCharToLcdInt(char c)
{
	if(c < 91)
	{
		return c + 128;
	}
	else
	{
		return c;
	}
}

uint8_t readBACRegister(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	uint8_t result[1];
	i2cRead(i2cPort, addr, NULL, 0, result, 1);	
	
	return result[0];
}

void isBusyWait(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	//delay(5);
	/*int i;
	while((i = readBACRegister(i2cPort, addr)) & 0x80)
	{
		char flagVal[30];
		//int bacVal = readBACRegister(i2cPort, addr);
		int flagValLength = sprintf(flagVal, "Busy Value: %x\r\n", (i & 0x80));
		write_usb_serial_blocking(flagVal, flagValLength);
	}*/

	while(readBACRegister(i2cPort, addr) & 0x80);
	//delay(2);
}

uint8_t readAddressCounter(LPC_I2C_TypeDef* i2cPort, uint8_t addr)
{
	return (readBACRegister(i2cPort, addr) & 0x7F);
}

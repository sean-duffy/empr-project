#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_gpio.h"

#include "LPC17xx.h"
#include "../UART/uart.h"
#include "../I2C/i2c.h"
#include "../LCD/lcd.h"
#include "keypad.h"

int printedToScreen = 0;

extern void EINT3_IRQHandler()
{	
	char readChar = keypadRead(LPC_I2C1, keypadAddr);

	//char toSend[30];
	//int sendingLen = sprintf(toSend, "Value: %c\r\n", readChar);
	//write_usb_serial_blocking(toSend, sendingLen);

	//lcdPrintChar(LPC_I2C1, LCDAddr, mapCharToLcdInt(readChar));
	//reset keypad state
	
	if(readChar != '\0')
	{
		lcdPrintChar(LPC_I2C1, LCDAddr, mapCharToLcdInt(readChar));
		//staticPrintFirstLine(LPC_I2C1, LCDAddr, readChar);
	}

	uint8_t data[] = {0b00001111};	
	i2cWrite(LPC_I2C1, keypadAddr, data, 1);

	GPIO_ClearInt(0, 0x00800000);
}

void main()
{
	//serial_init();
	i2cInit(LPC_I2C1, keypadAddr);
	keypadInit(LPC_I2C1, keypadAddr);
	lcdInit(LPC_I2C1, LCDAddr, 1);

	while(1)
	{
		
	}
}

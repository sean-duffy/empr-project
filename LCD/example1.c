#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"

#include "lcd.h"
#include "../I2C/i2c.h"

const char* testStr = "The Music Streaming System of the Decade";
int idx = 0;

#define LCDAddr 0x3B

void main()
{
	delay(5);
	i2cInit(LPC_I2C1, 100000);
	lcdInit(LPC_I2C1, LCDAddr, 1);
	writeMessage(LPC_I2C1, LCDAddr, testStr, 1);
	while(idx < 30)
    {
        writeMessage(LPC_I2C1, LCDAddr, &testStr[idx++], 1);
        delay(20);
    }

}


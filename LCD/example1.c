#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"

#include "lcd.h"
#include "../I2C/i2c.h"

const char* testStr = "The CAN Based Music Streaming System of the Decade";
const char* testStr2 = "Jashan Bhoora";

#define LCDAddr 0x3B

void main()
{
	delay(5);
	i2cInit(LPC_I2C1, 100000);
	lcdInit(LPC_I2C1, LCDAddr, 0);

    setFirstLineText((char*)testStr, 50);
    setSecondLineText((char*)testStr2, 13);
    //clearFirstLine(LPC_I2C1, LCDAddr);
    //clearSecondLine(LPC_I2C1, LCDAddr);
    while(1)
    {
        scrollAndPrintFirstLine(LPC_I2C1, LCDAddr);
        scrollAndPrintSecondLine(LPC_I2C1, LCDAddr);
        delay(20);
    }
}


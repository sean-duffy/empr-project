#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"

#include "lcd.h"
#include "../I2C/i2c.h"

const char* testStr = "                The CAN Based Music Streaming System of the Decade";
const char* testStr2 = "                Jashan Bhoora";
//const char* testStr2 = "Chan: 12  Vol: 6";

#define LCDAddr 0x3B

void main()
{
	delay(5);
	i2cInit(LPC_I2C1, 100000);
	lcdInit(LPC_I2C1, LCDAddr, 0);
    //clearFirstLine(LPC_I2C1, LCDAddr);
    //writeVolCharToCGRAM(LPC_I2C1, LCDAddr);

    setFirstLineText((char*)testStr, 66);
    setSecondLineText((char*)testStr2, 29);
    while(1)
    {
        scrollAndPrintFirstLine(LPC_I2C1, LCDAddr);
        scrollAndPrintSecondLine(LPC_I2C1, LCDAddr);
        //staticPrintSecondLine(LPC_I2C1, LCDAddr, testStr2);
        delay(20);
    }

    //while(1);
}


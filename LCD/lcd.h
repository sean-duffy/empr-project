#ifndef LCDLIB_H
#define LCDLIB_H

#define LCDAddr 0x3B

void delay(int multiplier);
void lcdInit(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t displayCursor);
void clearDisplay(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void targetDDRAMZero(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void setDDRAMAddress(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t targetAddr);
void enableDisplay(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void writeMessage(LPC_I2C_TypeDef* i2cPort, uint8_t addr, char* message, int blankSecondLine);
void lcdWrite(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t data);
void lcdPrintChar(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t data);
void resetCursor(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
int mapCharToLcdInt(char c);

uint8_t readBACRegister(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void isBusyWait(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
uint8_t readAddressCounter(LPC_I2C_TypeDef* i2cPort, uint8_t addr);

#endif

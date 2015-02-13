#ifndef LCDLIB_H
#define LCDLIB_H

#define LCDAddr 0x3B

static char* firstLineText;
static unsigned int firstLinePos;
static unsigned int firstLineLength;

static char* secondLineText;
static unsigned int secondLinePos;
static unsigned int secondLineLength;

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

void setFirstLineText(char* arrayPointer, unsigned int lineLength);
void setSecondLineText(char* arrayPointer, unsigned int lineLength);
void scrollAndPrintFirstLine(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void scrollAndPrintSecondLine(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void clearFirstLine(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void clearSecondLine(LPC_I2C_TypeDef* i2cPort, uint8_t addr);

void staticPrintFirstLine(LPC_I2C_TypeDef* i2cPort, uint8_t addr, char* text);
void staticPrintSecondLine(LPC_I2C_TypeDef* i2cPort, uint8_t addr, char* text);

#endif

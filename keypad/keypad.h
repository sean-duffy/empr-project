#ifndef KEYPADLIB_H
#define KEYPADLIB_H
#define keypadAddr 0x21

void keypadInit(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
char keypadRead(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
void keypadInitInterrupt(LPC_I2C_TypeDef* i2cPort, uint8_t addr);

#endif



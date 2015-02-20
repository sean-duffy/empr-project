#ifndef I2CLIB_H
#define I2CLIB_H

void i2cInit(LPC_I2C_TypeDef* addr, uint32_t clkspeed);
void i2cWrite(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t data[], uint8_t dataSize);
void i2cRead(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t sendData[], uint8_t sendSize, uint8_t returnData[], uint8_t returnSize);

void enableI2COperations(LPC_I2C_TypeDef* addr);
void enableI2CInterrupt(LPC_I2C_TypeDef* addr);

#endif

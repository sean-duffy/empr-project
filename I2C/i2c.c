#include "lpc17xx_i2c.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "i2c.h"

void i2cInit(LPC_I2C_TypeDef* addr, uint32_t clkspeed)
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.OpenDrain = 0; //PINSEL_PINMODE_OPENDRAIN;
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	
	PinCfg.Funcnum = PINSEL_FUNC_3;
	PinCfg.Portnum = 0;	
	
	PinCfg.Pinnum = 0; //SDA1
	PINSEL_ConfigPin(&PinCfg);
	
	PinCfg.Pinnum = 1; //SCL1
	PINSEL_ConfigPin(&PinCfg);

	I2C_Init(addr, clkspeed); //this is an LPC library function
	enableI2COperations(addr);
}

void enableI2CInterrupt(LPC_I2C_TypeDef* addr)
{
	I2C_IntCmd(addr, TRUE);
}

void enableI2COperations(LPC_I2C_TypeDef* addr)
{
    I2C_Cmd(addr,ENABLE);
}

void i2cWrite(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t data[], uint8_t dataSize)
{
	I2C_M_SETUP_Type toSend;

	toSend.tx_data = data;
	toSend.tx_length = dataSize;
	toSend.rx_data = NULL;
	toSend.rx_length = 0;
	toSend.retransmissions_max = 3;
	toSend.sl_addr7bit = addr;

	I2C_MasterTransferData(i2cPort, &toSend, I2C_TRANSFER_POLLING);
}

void i2cRead(LPC_I2C_TypeDef* i2cPort, uint8_t addr, uint8_t sendData[], uint8_t sendSize, uint8_t returnData[], uint8_t returnSize)
{
	I2C_M_SETUP_Type toSend;
	//Status result;

	toSend.tx_data = sendData;
	toSend.tx_length = sendSize;

	toSend.rx_data = returnData;
	toSend.rx_length = returnSize;

	toSend.retransmissions_max = 3;
	toSend.sl_addr7bit = addr;

	I2C_MasterTransferData(i2cPort, &toSend, I2C_TRANSFER_POLLING);
}




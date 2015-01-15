#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "can.h"
#include "../UART/uart.h"

void init_can(LPC_CAN_TypeDef* can_interface, uint8_t id, uint32_t baud_rate)
{
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
        PinCfg.OpenDrain = 0;
        PinCfg.Pinmode = 0;
        PinCfg.Pinnum = 0;
        PinCfg.Portnum = 0;
        PINSEL_ConfigPin(&PinCfg);
        PinCfg.Pinnum = 1;
        PINSEL_ConfigPin(&PinCfg);

	CAN_Init(can_interface, baud_rate);
	//CAN_SetAFMode(can_interface, CAN_AccOff); //turn off filter for setting
	CAN_SetAFMode(can_interface, CAN_AccBP);
	//set filter here
	//turn on filter here
	
}

void enable_interrupt(LPC_CAN_TypeDef* can_interface)
{
	CAN_IRQCmd(can_interface, CANINT_FCE, ENABLE);
	CAN_IRQCmd(can_interface, CANINT_RIE, ENABLE);
	NVIC_EnableIRQ(can_interface);
}
void disable_interrupt(LPC_CAN_TypeDef* can_interface)
{
	CAN_IRQCmd(can_interface, CANINT_FCE, DISABLE);
	CAN_IRQCmd(can_interface, CANINT_RIE, DISABLE);
	NVIC_DisableIRQ(can_interface);
}



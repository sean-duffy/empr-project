#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"


#include "can.h"
#include "../UART/uart.h"

void CAN_IRQHandler(void)
{
	write_serial("Interrupt\n", 10);
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);
    //check receive interrupt (could be CAN2)
    if((IntStatus>>0)&0x01)
    {
        //CAN_ReceiveMsg(LPC_CAN2, &AFRxMsg[CANRxCount]);
        write_serial("Received\n", 9);
    }
}

void main()
{
	serial_init();
	write_serial("booted\n", 7);
	//init_can(LPC_CAN2, 250000);
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
         PinCfg.OpenDrain = 0;
         PinCfg.Pinmode = 0;
         PinCfg.Pinnum = 0;
         PinCfg.Portnum = 0;
         PINSEL_ConfigPin(&PinCfg);
         PinCfg.Pinnum = 1;
         PINSEL_ConfigPin(&PinCfg);
 
         PinCfg.Pinnum = 7;
         PinCfg.Portnum = 2;
         PINSEL_ConfigPin(&PinCfg);
         PinCfg.Pinnum = 8;
         PINSEL_ConfigPin(&PinCfg);

	CAN_Init(LPC_CAN1, 250000);
	CAN_Init(LPC_CAN2, 250000);

	//CAN_IRQCmd(LPC_CAN2, CANINT_FCE, ENABLE);
	CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);
	//CAN_IRQCmd(LPC_CAN2, CANINT_FCE, ENABLE);
         CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);
	NVIC_EnableIRQ(CAN_IRQn);

	CAN_SetAFMode(LPC_CANAF, CAN_AccBP);

	//enable_interrupt(LPC_CAN2);
	write_serial("loaded\n", 7);
	//delay();
	//write_serial("filtered\n", 9);
	//set_device_id(LPC_CAN1, 1 << 11);
	int i = 0, j = 0;
	char outVal[30];
	

	/*while(1)
	{
		i = CAN_GetCTRLStatus(LPC_CAN1, CANCTRL_STS);
		j = CAN_GetCTRLStatus(LPC_CAN2, CANCTRL_STS);
		int outValLength = sprintf(outVal, "CAN1: %d\r\n CAN2: %d\r\n", i, j);
		write_serial(outVal, outValLength);
	}*/
}

void delay()
{
	int i;
	for (i = 0; i < 9000000; i++);
}




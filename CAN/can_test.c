#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"


#include "can.h"
#include "../UART/uart.h"

void CAN_IRQHandler(void)
{
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN1);
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
	init_can(LPC_CAN1, 250000);
	enable_interrupt(LPC_CAN1);
	write_serial("loaded\n", 7);
}

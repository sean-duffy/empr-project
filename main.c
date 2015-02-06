#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"


#include "CAN/can.h"
#include "UART/uart.h"
#include "MIDI/midi.h"

CAN_MSG_Type RXMsg;

void CAN_IRQHandler(void)
{
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);

    if((IntStatus>>0)&0x01)
    {
        CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
        interpret_message(&RXMsg, 1);
    }
    
}

void CAN_InitMessage(void) {
    RXMsg.format = 0x00;
    RXMsg.id = 0x00;
    RXMsg.len = 0x00;
    RXMsg.type = 0x00;
    RXMsg.dataA[0] = RXMsg.dataA[1] = RXMsg.dataA[2] = RXMsg.dataA[3] = 0x00000000;
    RXMsg.dataB[0] = RXMsg.dataA[1] = RXMsg.dataA[2] = RXMsg.dataA[3] = 0x00000000;
}

void main()
{
    CAN_InitMessage();
    LPC_GPIO0->FIODIR |= (1 << 10);

    LPC_GPIO0->FIOSET &= 0b111111111111111110111111111;

    serial_init();
    write_serial("\r\n\r\nbooted\r\n", sizeof("\r\n\r\nbooted\r\n"));
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 4;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 5;
    PINSEL_ConfigPin(&PinCfg);

    //CAN_Init(LPC_CAN1, 250000);
    CAN_Init(LPC_CAN2, 250000);


    //CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);
    CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);
    NVIC_EnableIRQ(CAN_IRQn);

    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);

    write_serial("loaded\n\r", 9);
    
}

void delay()
{
    int i;
    for (i = 0; i < 9000000; i++);
}




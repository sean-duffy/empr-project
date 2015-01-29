#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"


#include "../can.h"
#include "../../UART/uart.h"

CAN_MSG_Type RXMsg;

void CAN_IRQHandler(void)
{
    write_serial("Interrupt\n", 10);
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);
    //check receive interrupt (could be CAN2)
    if((IntStatus>>0)&0x01)
    {
        CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
        write_serial("Received\n", 9);
    char toPrint[35];
    uint32_t recVal = (RXMsg.dataA[0])|(RXMsg.dataA[1]<<8)|(RXMsg.dataA[2]<<16)|(RXMsg.dataA[3]<<24);

    int recValLength = sprintf(toPrint, "Value: %x\r\n", recVal);

    write_serial(toPrint, recValLength);
 recVal = (RXMsg.dataB[0])|(RXMsg.dataB[1]<<8)|(RXMsg.dataB[2]<<16)|(RXMsg.dataB[3]<<24);

     recValLength = sprintf(toPrint, "Value: %x\r\n", recVal);

    write_serial(toPrint, recValLength);
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
    write_serial("booted\n", 7);
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
         PinCfg.OpenDrain = 0;
         PinCfg.Pinmode = 0;
         PinCfg.Pinnum = 4;
         PinCfg.Portnum = 0;
         PINSEL_ConfigPin(&PinCfg);
         PinCfg.Pinnum = 5;
         PINSEL_ConfigPin(&PinCfg);

    CAN_Init(LPC_CAN1, 250000);
    CAN_Init(LPC_CAN2, 250000);

    //CAN_IRQCmd(LPC_CAN2, CANINT_FCE, ENABLE);
    CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);
    //CAN_IRQCmd(LPC_CAN2, CANINT_FCE, ENABLE);
        CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);
    NVIC_EnableIRQ(CAN_IRQn);

    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);

    write_serial("loaded\n", 7);
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




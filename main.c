#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"


#include "./CAN/can.h"
#include "./UART/uart.h"

CAN_MSG_Type RXMsg;

void CAN_IRQHandler(void)
{
    //write_serial("Interrupt\n\r", 11);
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);

    if((IntStatus>>0)&0x01)
    {
        CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
        if(RXMsg.len == 0) //text start/end
        { 
            write_serial("Received text S/E\n\r", 19);
            /*char toPrint[50];

            uint32_t format = RXMsg.format;
            int recValLength = sprintf(toPrint, "Format: %x\r\n", format);
            write_serial(toPrint, recValLength);

            uint32_t id = RXMsg.id;
            recValLength = sprintf(toPrint, "Id: %x\r\n", id);
            write_serial(toPrint, recValLength);
            
            uint32_t len = RXMsg.len;
            recValLength = sprintf(toPrint, "Length: %x\r\n", len);
            write_serial(toPrint, recValLength);

            uint32_t type = RXMsg.type;
            recValLength = sprintf(toPrint, "Type: %x\r\n", type);
            write_serial(toPrint, recValLength);

            uint32_t recVal = (RXMsg.dataA[0])|(RXMsg.dataA[1]<<8)|(RXMsg.dataA[2]<<16)|(RXMsg.dataA[3]<<24);
            recValLength = sprintf(toPrint, "Value: %x\r\n", recVal);
            write_serial(toPrint, recValLength);

            recVal = (RXMsg.dataB[0])|(RXMsg.dataB[1]<<8)|(RXMsg.dataB[2]<<16)|(RXMsg.dataB[3]<<24);
            recValLength = sprintf(toPrint, "Value: %x\r\n", recVal);
            write_serial(toPrint, recValLength);

            //disable_interrupt();
            write_serial("\r\n", 2);*/
        }
        else if (RXMsg.len == 5) //music data
        {
            write_serial("Received MIDI\n\r", 15);
            uint8_t channel = RXMsg.dataA[0];
            uint8_t note = RXMsg.dataA[1];
            uint8_t volume = RXMsg.dataA[2];
            uint8_t type = RXMsg.dataA[3];
            uint8_t control = RXMsg.dataB[0];
            char toPrint[50];

            int recValLength = sprintf(toPrint, "Channel: %d\r\n", channel);      
            write_serial(toPrint, recValLength);

            recValLength = sprintf(toPrint, "Note: %d\r\n", note);
            write_serial(toPrint, recValLength);

            recValLength = sprintf(toPrint, "Volume: %d\r\n", volume);
            write_serial(toPrint, recValLength);

            recValLength = sprintf(toPrint, "Type: %d\r\n", type);
            write_serial(toPrint, recValLength);

            recValLength = sprintf(toPrint, "Control: %d\r\n", control);
            write_serial(toPrint, recValLength);

            write_serial("\r\n", 2);
        }

        else if(RXMsg.len == 8)
        {
            write_serial("Received text\r\n", 15);
        }

        else
        {
            write_serial("WTF\r\n", 5); 
        }   
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




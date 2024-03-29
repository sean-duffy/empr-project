#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "can.h"
#include "../UART/uart.h"

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }

CAN_MSG_Type received_message;

short unsigned int debug;

/*void CAN_IRQHandler() {
    debug_print("CAN Interrupt Fired", 19);
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);
    if ((IntStatus>>0)&0x01) {
        CAN_ReceiveMsg(LPC_CAN2, &received_message);
        char toPrint[35];
        uint32_t recVal = (received_message.dataA[0])|(received_message.dataA[1]<<8)|(received_message.dataA[2]<<16)|(received_message.dataA[3]<<24);

        int recValLength = sprintf(toPrint, "Value: %x\r\n", recVal);
        debug_print(toPrint);
        recVal = (RXMsg.dataB[0])|(RXMsg.dataB[1]<<8)|(RXMsg.dataB[2]<<16)|(RXMsg.dataB[3]<<24);

        recValLength = sprintf(toPrint, "Value: %x\r\n", recVal);

        debug_print(toPrint, recValLength);
    }
}*/

void CAN_init_message(void) {
    received_message.format = 0x00;
    received_message.id = 0x00;
    received_message.len = 0x00;
    received_message.type = 0x00;
    received_message.dataA[0] = received_message.dataA[1] = received_message.dataA[2] = received_message.dataA[3] = 0x00000000;
    received_message.dataB[0] = received_message.dataA[1] = received_message.dataA[2] = received_message.dataA[3] = 0x00000000;
}

void init_can(uint32_t baud_rate, short unsigned int debug_flag) {
    debug = debug_flag;
    debug_print("Initialising CAN module", 23);

    //enable CAN transceiver
    LPC_GPIO0->FIODIR |= (1 << 10);
    LPC_GPIO0->FIOSET &= 0b111111111111111110111111111;

    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 4;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 5;
    PINSEL_ConfigPin(&PinCfg);            
    
    CAN_Init(LPC_CAN2, baud_rate);

    CAN_init_message();
    
    CAN_SetAFMode(LPC_CANAF, CAN_AccBP); //accept all traffic on init
    debug_print("CAN Initialised", 15);

    CAN_init_message();
    LPC_GPIO0->FIODIR |= (1 << 10);
 
    CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);
    NVIC_EnableIRQ(CAN_IRQn);

    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);
}



void enable_interrupt() {
    //CAN_IRQCmd(LPC_CAN2, CANINT_FCE, ENABLE);  //fullcan interrupt (29bit identifiers)
    CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);  //interrupt on receive message
    NVIC_EnableIRQ(CAN_IRQn);
    debug_print("CAN interrupts enabled", 22);
}

void disable_interrupt() {
    //CAN_IRQCmd(LPC_CAN2, CANINT_FCE, DISABLE);
    CAN_IRQCmd(LPC_CAN2, CANINT_RIE, DISABLE);
    NVIC_DisableIRQ(CAN_IRQn);
    debug_print("CAN interrupts disabled", 23);
}

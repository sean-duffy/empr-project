#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "midi.h"
#include "../UART/uart.h"

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug)
{
    if(received_message.len == 0)
    {
        if ((RXMsg.id >> 25) == 1)
        {
            debug_print("Start: ",6);
        } 

        else if ((RXMsg.id >> 24) == 1)
        {
            debug_print("End\n\r",5);
        }      
    }

    else if (received_message.len == 5)
    {
        //music data return
    }
    
    else if(RXMsg.len == 8)
    {
        char a_print[30];
        uint32_t text_data;
        int suc_len;

        int i;
        for(i = 0; i < 4; i++){
            text_data = RXMsg.dataA[i];
            suc_len = sprintf(a_print, "%c", (char) text_data);
            debug_print(a_print, suc_len);
        }

        for(i = 0; i < 4; i++){
            text_data = RXMsg.dataB[i];
            suc_len = sprintf(a_print, "%c", (char) text_data);
            debug_print(a_print, suc_len);
        }
    }
}
}   

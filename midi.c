#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "midi.h"
#include "UART/uart.h"

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug)
{
    if(received_message->len == 0)
    {
        if ((received_message->id >> 25) == 1)
        {
            debug_print("Start", 5);
        } 

        else if ((received_message->id >> 24) == 1)
        {
            debug_print("\n\rEnd", 5);
        }      
    }

    else if (received_message->len == 5)
    {
        //music data return
        debug_print_nnl("n", 2);
    }
    
    /*else if(received_message->len == 8)
    {
        
        uint32_t text_data;
        int suc_len;

        int i;
        char text_message_A[4];
        for(i = 0; i < 4; i++)
        {
            text_message_A[i] = received_message->dataA[i];
            //suc_len = sprintf(a_print, "%c", text_message);
            debug_print_nnl(text_message_A, 5);
        }

        char text_message_B[4];
        for(i = 0; i < 4; i++)
        {
            text_message_B[i] = received_message->dataB[i];
            //suc_len = sprintf(a_print, "%c", text_message);
            debug_print_nnl(text_message_B, 5);
        }
    }*/

    else if(received_message->len == 8)
        {
            char a_print[30];
            int suc_len;

            int i;
            for(i = 0; i < 4; i++){
                suc_len = sprintf(a_print, "%c", (char) received_message->dataA[i]);
                debug_print_nnl(a_print, suc_len);
                debug_print((char)suc_len, 2);
            }

            for(i = 0; i < 4; i++){
                suc_len = sprintf(a_print, "%c", (char) received_message->dataB[i]);
                debug_print_nnl(a_print, suc_len);
                debug_print((char)suc_len, 2);
            }
        }
}

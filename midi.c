#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "midi.h"
#include "UART/uart.h"
#include <string.h>

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
        /*struct CAN_return_data music_data = 
        {
            {
                received_message->dataA[0],
                received_message->dataA[1],
                received_message->dataA[2],
                received_message->dataA[3],
                received_message->dataB[0]
            },
            {},
            1
        }*/
        //return music_data;
    }
    
    /*else if(received_message->len == 8)
    {
        
        uint32_t text_da passing argument 1 of 'write_serial' makes pointer from ita;
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
            char b_print[30];
            char concat[100];
            int charCount = 0;
            int len_A;
            int len_B;
            
            len_A = sprintf(a_print, "%c", (char)received_message->dataA[0]);
            charCount += len_A;
            strcpy(concat, a_print);

            int i;
            for(i = 1; i < 4; i++){
                len_A = sprintf(a_print, "%c", (char)received_message->dataA[i]);
                strcat(concat, a_print);
                //debug_print_nnl(a_print, len_A);
                charCount += len_A;
            }

            for(i = 0; i < 4; i++){
                len_B = sprintf(b_print, "%c", (char)received_message->dataB[i]);
                strcat(concat, b_print);
                //debug_print_nnl(b_print, len_B);
                charCount += len_B;
            }
    
            /*struct CAN_return_data inst_data = 
            {
                {},
                {
                    
                },
                0
            }*/

            //strcpy(concat, a_print);
            //strcat(concat, b_print);
            debug_print_nnl(concat, charCount);
        
        }
}

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "midi.h"
#include "UART/uart.h"
#include <string.h>

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug, struct CAN_return_data ret)
{

    static uint8_t flag = 0;
    static uint8_t count = 0;
    static char concat[100];
    static int char_count = 0;

    if(received_message->len == 0)
    { concat[0] = '\0';
        if ((received_message->id >> 25) == 1)
        {
            debug_print("Start", 5);
            flag = 1;
            concat[0] = '\0';
            char_count = 0;
        } 

        else if ((received_message->id >> 24) == 1)
        {
            debug_print("\n\rEnd", 5);
            flag = 0;
            count ++;
        }      
    }

    else if (received_message->len == 5)
    {
        //music data return
        debug_print_nnl("n", 2);
        count = 0;
        ret.midi_data.channel = received_message->dataA[0];
        ret.midi_data.channel = received_message->dataA[1];
        ret.midi_data.channel = received_message->dataA[2];
        ret.midi_data.channel = received_message->dataA[3];
        ret.midi_data.channel = received_message->dataB[0];
    }

    else if(received_message->len == 8)
        {
            char a_print[30];
            char b_print[30];
            int len_A = 0;
            int len_B = 0;
            int i;
            
            len_A = sprintf(a_print, "%c", (char)received_message->dataA[0]);
            char_count += len_A;
            strcat(concat, a_print);

            int i;
            for(i = 1; i < 4; i++){
                len_A = sprintf(a_print, "%c", (char)received_message->dataA[i]);
                strcat(concat, a_print);
                //debug_print_nnl(a_print, len_A);
                char_count += len_A;
            }

            for(i = 0; i < 4; i++){
                len_B = sprintf(b_print, "%c", (char)received_message->dataB[i]);
                strcat(concat, b_print);
                //debug_print_nnl(b_print, len_B);
                char_count += len_B;
            }
        }

    switch (count) {
        case 1:
            strcpy(ret.text_data.track, concat);
            i = strlen(ocncat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 2:
            strcpy(ret.text_data.bpm, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 3:
            strcpy(ret.text_data.ch0, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 4:
            strcpy(ret.text_data.ch1, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 5:
            strcpy(ret.text_data.ch2, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 6:
            strcpy(ret.text_data.ch3, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 7:
            strcpy(ret.text_data.ch4, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 8:
            strcpy(ret.text_data.ch5, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 9:
            strcpy(ret.text_data.ch6, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 10:
            strcpy(ret.text_data.ch7, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 11:
            strcpy(ret.text_data.ch8, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 12:
            strcpy(ret.text_data.ch9, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 13:
            strcpy(ret.text_data.ch10, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 14:
            strcpy(ret.text_data.ch11, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 15:
            strcpy(ret.text_data.ch12, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 16:
            strcpy(ret.text_data.ch13, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 17:
            strcpy(ret.text_data.ch14, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
        case 18:
            strcpy(ret.text_data.ch15, concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';
            debug_print_nnl(concat, char_count);
            break;
    }
}

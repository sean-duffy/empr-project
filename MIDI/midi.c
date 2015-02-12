#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "midi.h"
#include "UART/uart.h"
#include <string.h>

#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug, struct CAN_return_data *ret)
{

    static uint8_t flag = 0;
    static uint8_t count = 0;
    static char concat[100];
    static int char_count = 0;
    int i;


    if(received_message->len == 0)
    {
        if ((received_message->id >> 25) == 1)
        {
            //debug_print("Start", 5);
            if(count == 0){
                ret->done = 0;
            }

            flag = 1;
            memset(concat, 0, 100);
            concat[0] = '\0';
            char_count = 0;
        } 

        else if ((received_message->id >> 24) == 1)
        {
            //debug_print("\n\rEnd", 5);

            flag = 0;
            count ++;

            i = strlen(concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';

            switch (count) {
                case 1:
                    strcpy(ret->text_data.track, concat);
                    //debug_print_nnl(ret->text_data.track, strlen(ret->text_data.track));
                    break;
                case 2:
                    strcpy(ret->text_data.bpm, concat);
                    //debug_print_nnl(ret->text_data.bpm, strlen(ret->text_data.bpm));
                    break;
                default:
                    if(count > 2){
                        free(ret->text_data.channel[count-2]);
                        ret->text_data.channel[count-2] = (char*) calloc(30, sizeof(char));
                        strcpy(ret->text_data.channel[count-2], concat);
                        //debug_print_nnl(ret->text_data.channel[count-2], strlen(ret->text_data.channel[count-2]));
                }
            } 

            if(count==18){
                char temp[100];
                strcpy(temp, (ret->text_data.track) + 50);
                strcpy(ret->text_data.track, temp);

                ret->done = 1;
                count = 0;
            }
        
        }      
    }

    else if (received_message->len == 5)
    {
        //music data return
        debug_print_nnl("^", 2);
        ret->midi_data.channel = received_message->dataA[0];
        ret->midi_data.note = received_message->dataA[1];
        ret->midi_data.volume = received_message->dataA[2];
        ret->midi_data.type = received_message->dataA[3];
        ret->midi_data.control = received_message->dataB[0];
    }

    else if(received_message->len == 8)
        {
            // Text data packet
            char a_print[8];
            char b_print[32];
            int len_A = 0;
            int len_B = 0;


        for(i = 0; i < 4; i++){
                a_print[i] = (char) received_message->dataA[i];
                //len_A = sprintf(a_print, "%c", (char)received_message->dataA[i]);
                //strcat(concat, a_print);
                //debug_print_nnl(a_print, len_A);
                //char_count += len_A;
            }

            for(i = 0; i < 4; i++){
                a_print[4+i] = (char) received_message->dataB[i];
                //len_B = sprintf(b_print, "%c", (char)received_message->dataB[i]);
                //strcat(concat, b_print);
                //debug_print_nnl(b_print, len_B);
                //char_count += len_B;
            }

            strcat(concat, a_print);
        }
}

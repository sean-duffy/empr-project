#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "midi.h"
#include "UART/uart.h"
#include <string.h>
#include <stdlib.h>

#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug, struct CAN_return_data *ret) {
    static uint8_t count = 0;
    static char concat[100];
    int i;

    if (received_message->len == 0) {
        if ((received_message->id >> 25) == 1) {
        //Start packet
            if(count == 0) { 
                ret->done = 0;
            }

            memset(concat, 0, 100);
            concat[0] = '\0';
        } else if ((received_message->id >> 24) == 1) {
            //End packet
            count ++;
            i = strlen(concat);
            concat[i] = '\n';
            concat[i+1] = '\r';
            concat[i+2] = '\0';

            switch (count) {
                case 1:
                    strcpy(ret->text_data.track, concat);
                    break;
                case 2:
                    strcpy(ret->text_data.bpm, concat);
                    break;
                default:
                    if(count > 2){
                        free(ret->text_data.channel[count-2]);
                        ret->text_data.channel[count-2] = (char*) calloc(30, sizeof(char));
                        strcpy(ret->text_data.channel[count-2], concat);
                }
            } 

            if(count==18) {
                char temp[100];
                strcpy(temp, (ret->text_data.track) + 52);
                strcpy(ret->text_data.track, temp);

                ret->done = 1;
                ret->is_midi = 0;
                count = 0;
            }
        
        }      

    } else if (received_message->len == 5) {
        //music data return

        ret->midi_data.channel = received_message->dataA[0];
        ret->midi_data.note = received_message->dataA[1];
        ret->midi_data.volume = received_message->dataA[2];
        ret->midi_data.type = received_message->dataA[3];
        ret->midi_data.control = received_message->dataB[0];
        ret->is_midi = 1;
    } else if (received_message->len == 8) {
        // Text data packet
        char packet_c[8];

        for(i = 0; i < 4; i++) {
            packet_c[i] = (char) received_message->dataA[i];
        }

        for(i = 0; i < 4; i++) {
            packet_c[4+i] = (char) received_message->dataB[i];
        }

        strcat(concat, packet_c);
    }
}

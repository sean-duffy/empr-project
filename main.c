#include <string.h>
#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "CAN/can.h"
#include "UART/uart.h"
#include "SYNTH/oscillator.h"
#include "SYNTH/synth.h"
#include "SYNTH/instruments.h"
#include "MIDI/midi.h"

int debug = 1;
CAN_MSG_Type RXMsg;
struct CAN_return_data message;

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

uint8_t channel_playing = 2;

void CAN_IRQHandler(void) {
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);

    if((IntStatus>>0)&0x01) {
        CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
        interpret_message(&RXMsg, 1, &message);

        if (message.done) {
            debug_print_nnl(message.text_data.track, strlen(message.text_data.track));
            debug_print_nnl(message.text_data.bpm, strlen(message.text_data.bpm));
                
            int i;
            for (i=0; i<15; i++) {
                char setup[10];
                sprintf(setup, "%d: ", i);
                debug_print_nnl(setup, strlen(setup));
                debug_print_nnl(message.text_data.channel[i], strlen(message.text_data.channel[i]));
            }
            message.done = 0;
        }

        if (RXMsg.len == 5) {
            uint8_t channel = RXMsg.dataA[0];
            uint8_t note = RXMsg.dataA[1];
            uint8_t volume = RXMsg.dataA[2];
            uint8_t type = RXMsg.dataA[3];
            uint8_t control = RXMsg.dataB[0];
            if (channel == channel_playing) {
                if (volume == 0) {
                    note_off();
                } else {
                    note_on(get_freq(note));
                }
            }
        }
    }
}

void main() {

    serial_init();
    debug_print("Hello mbed", strlen("Hello mbed"));
    set_resolution(RESOLUTION);
   
    double wave_buf_1[RESOLUTION];
	double wave_buf_2[RESOLUTION];

    debug_print("set_voice_id", strlen("set_voice_id"));

    set_voice_by_id(5, wave_buf_1, wave_buf_2);

    init_dac();
    init_can(250000, 0);

    SysTick_Config(2400);
    note_on(get_freq(60));
    while (1); 
}

#include <string.h>
#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "CAN/can.h"
#include "UART/uart.h"
#include "SYNTH/oscillator.h"
#include "SYNTH/synth.h"
#include "MIDI/midi.h"

int debug = 1;
CAN_MSG_Type RXMsg;
struct CAN_return_data message;

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

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

        if (RXMsg.len == 0) { 
            //write_serial("Received text S/E\n\r", 19);
        } else if (RXMsg.len == 5) {
            uint8_t channel = RXMsg.dataA[0];
            uint8_t note = RXMsg.dataA[1];
            uint8_t volume = RXMsg.dataA[2];
            uint8_t type = RXMsg.dataA[3];
            uint8_t control = RXMsg.dataB[0];
            if (channel == 1) {
                if (volume == 0) {
                    note_off();
                } else {
                    note_on(get_freq(note));
                }

            }
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

void main() {
    int resolution = 360;
    set_resolution(resolution);
 
    double wave_buf[resolution];
    generate_sawtooth(wave_buf, resolution);

    voice_1.osc_1_buf = wave_buf;
    voice_1.osc_1_mix = 0.5;
    voice_1.osc_1_attack = 0.5;
    voice_1.osc_1_release = 0.8;
    voice_1.osc_2_buf = wave_buf;
    voice_1.osc_2_mix = 0;
    voice_1.osc_2_detune = 0;

    init_dac();
    init_can(250000, 0);

    set_voice(voice_1);
    SysTick_Config(2400);

    while (1);
}

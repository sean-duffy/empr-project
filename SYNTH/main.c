#include <string.h>
#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_gpio.h"

#include "../CAN/can.h"
#include "../UART/uart.h"
#include "./oscillator.h"
#include "./synth.h"
#include "./instruments.h"
#include "../MIDI/midi.h"

#include "../I2C/i2c.h"
#include "../LCD/lcd.h"
#include "../keypad/keypad.h"

int debug = 1;
CAN_MSG_Type RXMsg;
struct CAN_return_data message;

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

uint8_t channel_playing = 1;
char status_string[16];
char space_string[] = "                ";
char *first_line;

void CAN_IRQHandler(void) {
    int debug = 1;
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);

    if((IntStatus>>0)&0x01) {
        CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
        interpret_message(&RXMsg, 1, &message);

        if (message.done) {
            first_line = (char *) calloc(strlen(message.text_data.track) - 12 + 16 + 3 + strlen(message.text_data.bpm), sizeof(char));
            strcpy(first_line, space_string);
            strncat(first_line, message.text_data.track, strlen(message.text_data.track) - 12);
            strcat(first_line, " - ");
            strcat(first_line, message.text_data.bpm);

            message.done = 0;
        }

        if (message.is_midi) {
            if (message.midi_data.channel == channel_playing) {
                if (message.midi_data.volume == 0) {
                    note_off(0);
                } else {
                    note_on(get_freq(message.midi_data.note));
                }
            }
            message.is_midi = 0;
        }
    }
}


void main() {
    serial_init();
   
    double wave_buf_1[RESOLUTION];
	double wave_buf_2[RESOLUTION];

    set_voice_by_id(1, wave_buf_1, wave_buf_2);

    //Inits
    init_dac();
    SysTick_Config(2400);
    
    int xx;

    int note_1_id = note_on(get_freq(60));
    for(xx = 0; xx < 5000; xx++){}
    note_off(note_1_id);

    for(xx = 0; xx < 200000; xx++){}
    
    int note_2_id = note_on(get_freq(58));
    for(xx = 0; xx < 5000; xx++){}
    note_off(note_2_id);

    for(xx = 0; xx < 200000; xx++){}
    
    note_1_id = note_on(get_freq(52));
    note_2_id = note_on(get_freq(60));

    char buf[30];
    int l = sprintf(buf,"n_1 = %d, n_2 = %d", note_1_id, note_2_id);
    debug_print(buf, l);

    for(xx = 0; xx < 5000; xx++){}

    debug_print("Done", strlen("Done"));
    while (1);
}

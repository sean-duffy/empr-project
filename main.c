#include <string.h>
#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"

#include "CAN/can.h"
#include "UART/uart.h"
#include "SYNTH/oscillator.h"
#include "SYNTH/synth.h"
#include "MIDI/midi.h"

#include "I2C/i2c.h"
#include "LCD/lcd.h"

int debug = 1;
CAN_MSG_Type RXMsg;
struct CAN_return_data message;

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }
struct Voice voice_1;
uint8_t channel_playing = 2;
char status_string[16];
char *first_line;
char space_string[] = "                ";

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
            setFirstLineText(first_line, strlen(first_line));

            message.done = 0;
        }

        if (message.is_midi) {
            if (message.midi_data.channel == channel_playing) {
                if (message.midi_data.volume == 0) {
                    note_off();
                } else {
                    note_on(get_freq(message.midi_data.note));
                }
            }
        }
    }
}

void main() {
    int resolution = 360;
    set_resolution(resolution);
 
    double wave_buf_1[resolution];
    generate_sawtooth(wave_buf_1, resolution);

    double wave_buf_2[resolution];
    generate_square(wave_buf_2, resolution);

    voice_1.osc_1_buf = wave_buf_1;
    voice_1.osc_1_mix = 0.5;
    voice_1.osc_2_buf = wave_buf_2;
    voice_1.osc_2_mix = 0.5;
    voice_1.osc_2_detune = 0;
    voice_1.output_attack = 1;
    voice_1.output_release = 0.8;
    voice_1.envelope_on = 0;

	i2cInit(LPC_I2C1, 100000);
	lcdInit(LPC_I2C1, LCDAddr, 0);
    serial_init();

    init_dac();
    init_can(250000, 0);

    set_voice(voice_1);
    SysTick_Config(2400);

    sprintf(status_string, "Chan: %2d  Vol: %d", channel_playing, output_volume);
    staticPrintSecondLine(LPC_I2C1, LCDAddr, status_string);

    while (1);
}

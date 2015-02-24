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

extern void EINT3_IRQHandler() {
	char readChar = keypadRead(LPC_I2C1, keypadAddr);

    // Clear interrupt
    uint8_t data[] = {0b00001111};
    i2cWrite(LPC_I2C1, keypadAddr, data, 1);
    GPIO_ClearInt(0, 0x00800000);

    if (readChar == '#' && output_volume < 0.9) {
        output_volume += 0.1;
    } else if (readChar == '*' && output_volume > 0.1) {
        output_volume -= 0.1;
    }

    if (readChar == '9' && channel_playing < 15) {
        channel_playing += 1;
    } else if (readChar == '7' && channel_playing > 1) {
        channel_playing -= 1;
    }

    sprintf(status_string, "Chan: %2d  Vol: %f", channel_playing, output_volume * 10.0);
    write_second_line(&I2CConfigStruct, status_string, strlen(status_string));
}

void main() {
    serial_init();
   
    double wave_buf_1[RESOLUTION];
	double wave_buf_2[RESOLUTION];


    debug_print("p3\n", strlen("p3\n"));
    set_voice_by_id(1, wave_buf_1, wave_buf_2);

    //Inits
	i2cInit(LPC_I2C1, 100000);
    serial_init();

    debug_print("p0\n", strlen("p0\n"));
    //init_dac();
    
    init_can(250000, 0);

    debug_print("p5\n", strlen("p5\n"));
    SysTick_Config(2400);
    
    debug_print("p4\n", strlen("p4\n"));

    I2CConfigStruct.retransmissions_max = 3;
    I2CConfigStruct.sl_addr7bit = 59;

    uint8_t lcd_init[] = {0x00, 0x35, 0x9F, 0x34, 0x0C, 0x02};
    lcd_write_bytes(&I2CConfigStruct, lcd_init, sizeof(lcd_init));

    uint8_t clear[] = {0x00, 0x01};
    lcd_write_bytes(&I2CConfigStruct, clear, sizeof(clear));

    debug_print("p1\n", strlen("p1\n"));
    isBusyWait(LPC_I2C1, I2CConfigStruct.sl_addr7bit);

    sprintf(status_string, "Chan: %2d  Vol: %f", channel_playing, output_volume * 10.0);
    write_second_line(&I2CConfigStruct, status_string, strlen(status_string));

    keypadInit(LPC_I2C1, keypadAddr);

    debug_print("note_on", strlen("note_ond"));
    note_on(get_freq(60));

    while (1);
}

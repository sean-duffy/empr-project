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
#include "I2C/i2c.h"
#include "LCD/lcd.h"
#include "KEYPAD/keypad.h"
#include "STATEMACHINE/statemachine.h"

int debug = 1;
CAN_MSG_Type RXMsg;
struct CAN_return_data message;
int screenWritten = 0;

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

uint8_t channel_playing = 1;

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

extern void EINT3_IRQHandler()
{
    //keypadDisableInterrupt();
	char readChar = keypadRead(LPC_I2C1, keypadAddr);
	
	if(readChar != '\0')
	{
        
        int currState = getCurrentState();
        
        if(readChar == 'D') //return to default
        {
            setCurrentState(0);
            screenWritten = 0;
            return;
        }

		else if(currState == 0 && (readChar == '*' || readChar == '#')) //default to vol change
        {
            setCurrentState(1);
            screenWritten = 0;
            return;
        }
        
        else if(currState == 1 && (readChar == '*' || readChar == '#')) //actually change vol
        {
            int temp = getVolume();
            char outp[8];
            if(readChar == '*')
            {
                setVolume(temp - 1);
            }

            else
            {
                setVolume(temp + 1);
            }
            //resetStateTimeout();
            sprintf(outp, "%d", getVolume());
            staticPrintSecondLine(LPC_I2C1, LCDAddr, outp);
            
            return;
        }
        
        else if(currState == 0 && readChar == 'C') //default to channel change
        {
            setCurrentState(2);
            //start state timeout here
            return;
        }

        else if(currState == 2 && readChar == ('0' || '1')) //read first digit
        {
            setFirstNewChannelDigit(readChar == '0' ? 0 : 1);
            setCurrentState(3);
            //reset state timeout here
            return;
        }

        else if(currState == 3 && readChar == ('0' || '1' || '2' || '3' || '4' || '5' || '6' || '7' || '8' || '9')) //read second digit, apply then return to default
        {
            setSecondNewChannelDigit(readChar - '0');
            writeNewChannel();
            setCurrentState(0);
            //cancel state timeout here
            return;
        }

        else if(currState == 0 && readChar == 'A') //default to change voice state
        {
            setCurrentState(4);
            //start state timeout here
            return;
        }



        
        //setLastReadChar(readChar);
	}
    
    //reset keypad state
	uint8_t data[] = {0b00001111};	
	i2cWrite(LPC_I2C1, keypadAddr, data, 1);
	GPIO_ClearInt(0, 0x00800000);
    //keypadInitInterrupt();
}

void main() {  
    initialise_system();
    write_serial("Loaded\n\r", 8);
    int cs = 0;
    while (1)
    {   
        cs = getCurrentState();
        write_serial("looping\n\r", 9);
        if(cs == 0)
        {
            //continue; //synth systick manages printing to screen?
            if(!screenWritten)
            {
                //clearFirstLine(LPC_I2C1, LCDAddr);
                staticPrintFirstLine(LPC_I2C1, LCDAddr, "Default State"); 
                char secLine[17];
                sprintf(secLine, "Chan: %d   Vol: %d", getChannel(), getVolume());
                staticPrintSecondLine(LPC_I2C1, LCDAddr, secLine);
                screenWritten = 1;
            }
        }

        else if(cs == 1)
        {
            if(!screenWritten)
            {
                //print latest current volume values to screen instead of default
                //clearFirstLine(LPC_I2C1, LCDAddr);
                staticPrintFirstLine(LPC_I2C1, LCDAddr, "Volume State");
                screenWritten = 1;
            }
        }

        else if(cs == 2)
        {
            if(!screenWritten)
            {
                //print channel change display
                //clearFirstLine(LPC_I2C1, LCDAddr);
                staticPrintFirstLine(LPC_I2C1, LCDAddr, "Channel State");
                screenWritten = 1;
            }
        }
        
    }
}

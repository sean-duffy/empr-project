#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"

#include "../UART/uart.h"
#include "../CAN/can.h"
#include "../I2C/i2c.h"
#include "../KEYPAD/keypad.h"
#include "../LCD/lcd.h"
#include "../SYNTH/oscillator.h"
#include "../SYNTH/synth.h"
#include "statemachine.h"

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }

void initialise_system()
{
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

    //check function clash between state and synth with sean, state machine needs to
    //maintain a copy of the voice so it can be updated in voice change state
    //setCurrentVoice(voice_1);
    
    serial_init();
    i2cInit(LPC_I2C1, 100000);
    init_dac();
    init_can(250000, 0);
    keypadInit(LPC_I2C1, keypadAddr);
    lcdInit(LPC_I2C1, LCDAddr, 0);
    clearDisplay(LPC_I2C1, LCDAddr);
    set_voice(voice_1);
    SysTick_Config(2400);
}

int getVolume()
{
    return volumeMultiplier;
}

void setVolume(int vol)
{
    if (vol < 0)
    {
        volumeMultiplier = 0;
        return;
    }
    
    if(vol > 9)
    {
        volumeMultiplier = 9;
        return;
    }
    volumeMultiplier = vol;
}

int getChannel()
{
    return currentChannel;
}

void setChannel(int chan)
{
    currentChannel = chan;
}

int getCurrentState()
{
    return currentState;
}
void setCurrentState(int s)
{
    currentState = s;
}

int getCurrentVoice()
{
    return 0;
}

void setCurrentVoice(int v)
{
    return;
}

void resetStateTimeout()
{
    stateTimeoutActive = 1;
    stateTimeout = 0;
}

void clearStateTimeout()
{
    stateTimeoutActive = 0;
    stateTimeout = 0;
}

void incrementStateTimeout()
{
    stateTimeout++;
}

int getFirstNewChannelDigit()
{
    return newChannelFirstDigit;
}

void setFirstNewChannelDigit(int i)
{
    newChannelFirstDigit = i;
}

int getSecondNewChannelDigit()
{
    return newChannelSecondDigit;
}

void setSecondNewChannelDigit(int i)
{
    newChannelSecondDigit = i;
}

void writeNewChannel()
{
    setChannel(getFirstNewChannelDigit() * 10 + getSecondNewChannelDigit());   
}

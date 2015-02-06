#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"

#include "oscillator.h"
#include "synth.h"

int duration_passed = 0;
int resolution;
uint32_t note_length = 500;

double osc_1_inc = 30;
double osc_1_tick = 0;
double *osc_1_buf;
double osc_1_value;
double osc_1_mix;

double osc_2_inc = 30;
double osc_2_tick = 0;
double *osc_2_buf;
double osc_2_value;
double osc_2_mix;

double osc_3_inc = 30;
double osc_3_tick = 0;
double *osc_3_buf;
double osc_3_value;
double osc_3_mix;

int note_mute = 1;

double mix_inc = 0.00002;

double osc_mix;

void SysTick_Handler(void) {
    double output_value;

    if (osc_1_tick >= resolution) {
        osc_1_tick = 0;
    }

    if (osc_1_mix >= 1 || (osc_1_mix <= 0 && mix_inc < 0)) {
        mix_inc *= -1;
    }

    osc_1_value = osc_1_buf[(int) floor(osc_1_tick)];
    osc_2_value = osc_2_buf[(int) floor(osc_1_tick)];

    osc_mix = osc_1_value*osc_1_mix + osc_2_value*osc_2_mix;
    output_value = (int) floor((osc_mix + 1.0) * 300);

    DAC_UpdateValue(LPC_DAC, output_value * note_mute);
    osc_1_tick += osc_1_inc;

    osc_1_mix += mix_inc;
    osc_2_mix -= mix_inc;
}

void init_dac(void) {
    PINSEL_CFG_Type PinCfg; 

    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    
    //change pin number
    PinCfg.Pinnum = 26;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    DAC_Init (LPC_DAC);
}

void note_on(double freq, double length) {
    osc_1_inc = 0.00858141 * freq;
    osc_2_inc = osc_1_inc;

    note_mute = 1;
}

void note_off(void) {
    note_mute = 0;
}

double get_freq(int key_n) {
    // Convert piano key number to frequency
    double f = pow(2, (key_n - 69)/ 12.0) * 440;
    return f;
}

void set_voice(struct Voice voice) {
    osc_1_mix = voice.osc_1_mix;
    osc_2_mix = voice.osc_2_mix;
    osc_1_buf = voice.osc_1_buf;
    osc_2_buf = voice.osc_2_buf;
    mix_inc = 0;
}

void set_resolution(int new_resolution) {
    resolution = new_resolution;
}

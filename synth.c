#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "sound_modules.h"

int current_tick = 0;
int *wave_buf;
int duration_passed = 0;
int resolution = 360;

void SysTick_Handler(void) {
    if (current_tick >= 360) {
        current_tick = 0;
    }

    DAC_UpdateValue(LPC_DAC, wave_buf[current_tick]);
    current_tick += 5;
    duration_passed++;
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

void wave(double freq) {
    freq = 1/freq * 1389000;
    SysTick_Config((int) floor(freq));
}

double get_freq(int key_n){
    // Convert piano key number to frequency
    float f = pow(2, (key_n - 49)/ (float) 12) * 440;
    return (double) f;
}


int main(void) {
    init_dac();
    wave_buf = (int *) calloc (resolution, sizeof(int));
    generate_triangle(wave_buf, 2);
    int i;
    double freq;

    while(1) {
        //for (i = 40; i < 88; i++) {
        //    freq = get_freq(i);
        //    wave(freq);
        //    while (duration_passed != 5000);
        //    duration_passed = 0;
        //}
        freq = get_freq(49);
        wave(freq);
        while (duration_passed != 5000);
        duration_passed = 0;
    }

    return 0;
}

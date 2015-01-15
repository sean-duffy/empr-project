#include <stdio.h>
#include <math.h>

#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"

int current_tick = 0;
int sine_buff[360];
int duration_passed = 0;

void SysTick_Handler(void) {
    if (current_tick >= 360) {
        current_tick = 0;
    }

    DAC_UpdateValue(LPC_DAC, sine_buff[current_tick]);
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

void wave(double freq, double amplitude) {
    freq = 1/freq * 1389000;
    amplitude = amplitude * 309.1;
    int res = 360;

    int i;
    for(i = 0; i < 360; i++) {
        sine_buff[i] = (int) floor((amplitude * sin((2*M_PI/res)*i) + amplitude) / 2);
    }

    SysTick_Config((int) floor(freq));
}

int main(void) {
    init_dac();

    while(1) {
        wave(440, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(494, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(554, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(587, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(659, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(740, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(830, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(831, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

        wave(880, 2);
        while (duration_passed != 10000);
        duration_passed = 0;

    }

    return 0;
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"

#include "oscillator.h"

#define SECOND 1E9

int current_tick = 0;
double *wave_buf;
int duration_passed = 0;
int resolution = 100;
uint32_t note_length = 500;

typedef enum {SINE, SQUARE, TRIANGLE, SAWTOOTH} wave_t;

double osc_1_freq = 600;
int osc_1_tick = 0;
int osc_1_res;
wave_t osc_1_wave = SINE;

void SysTick_Handler(void) {
    double osc_1_value;

    osc_1_res = 1 / osc_1_freq * 16600;
    if (osc_1_tick >= osc_1_res) {
        osc_1_tick = 0;
    }

    if (osc_1_wave == SINE) {
        osc_1_value = point_sine(1.0/osc_1_res * osc_1_tick, 0) + 1.0;
    } else if (osc_1_wave == SQUARE) {
        osc_1_value = point_square(1.0/osc_1_res * osc_1_tick, 0) + 1.0;
    } else if (osc_1_wave == TRIANGLE) {
        osc_1_value = point_triangle(1.0/osc_1_res * osc_1_tick, 0) + 1.0;
    } else if (osc_1_wave == SAWTOOTH) {
        osc_1_value = point_sawtooth(1.0/osc_1_res * osc_1_tick, 0) + 1.0;
    }

    DAC_UpdateValue(LPC_DAC, (int) floor(osc_1_value * 300));

    osc_1_tick += 1;
}

void TIMER0_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT) == SET) {
            TIM_Cmd(LPC_TIM0, DISABLE);
            TIM_ResetCounter(LPC_TIM0);
            TIM_UpdateMatchValue(LPC_TIM0, 0, note_length * 10);
            TIM_Cmd(LPC_TIM0, ENABLE);
    }
    duration_passed++;
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
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

int init_timer(void) {
    PINSEL_CFG_Type PinCfg;
    TIM_TIMERCFG_Type TIM_ConfigStruct;
    TIM_MATCHCFG_Type TIM_MatchConfigStruct;

    // Configure P1.28 as MAT0.0
    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 28;
    PINSEL_ConfigPin(&PinCfg);

    // Initialize timer, prescale count time of 100uS
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
    TIM_ConfigStruct.PrescaleValue  = 100;
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);

    TIM_MatchConfigStruct.MatchChannel = 0;
    TIM_MatchConfigStruct.IntOnMatch = TRUE;
    TIM_MatchConfigStruct.ResetOnMatch = FALSE;
    TIM_MatchConfigStruct.StopOnMatch = FALSE;
    TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;

    TIM_MatchConfigStruct.MatchValue = note_length * 10;
    TIM_ConfigMatch(LPC_TIM0, &TIM_MatchConfigStruct);

    NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
    NVIC_EnableIRQ(TIMER0_IRQn);

    TIM_Cmd(LPC_TIM0,ENABLE);

    return 0;
}

void note(wave_t osc_wave, double freq, double length) {

    osc_1_wave = osc_wave;
    osc_1_freq = freq;

    while (duration_passed != 1);
    duration_passed = 0;
}

void rest(double length) {
    note_length = length;
    while (duration_passed != 1);
    duration_passed = 0;
}

double get_freq(int key_n) {
    // Convert piano key number to frequency
    double f = pow(2, (key_n - 49)/ 12.0) * 440;
    GPIO_SetValue(1, (1 << 18));
    return f;
}

int main(void) {
    GPIO_SetDir(1, (101101 << 18), 1);
    init_dac();
    init_timer();
    SysTick_Config(2275);

    int i;
    int v;
    int n;
    double freq;
    int arp[] = {40, 44, 47, 52, 47, 44};

    while (1) {
        for (v = 0; v < 4; v++) {
            for (n = 0; n < 4; n++) {
                for (i = 0; i < 6; i++) {
                    freq = get_freq(arp[i]);
                    note(v, freq, 125);
                }
            }
        }
    }

    double Cm[] = {40, 43, 47, 52, 55, 59, 64, 67, 71};
    double Bb[] = {38, 42, 45, 50, 54, 57, 62, 66, 69};
    double Fm[] = {33, 36, 40, 45, 48, 52, 57, 60, 64};

    double *arps[] = {Cm, Bb, Fm, Cm, Bb, Fm, Cm, Cm};

    //while (1) {
    //    for (n = 0; n < 8; n++) {
    //        for (i = 0; i < 9; i++) {
    //            freq = get_freq(arps[n][i]);
    //            
    //            note(SAWTOOTH, freq, 125);
    //        }
    //        for (i = 7; i > 0; i--) {
    //            freq = get_freq(arps[n][i]);
    //            note(SAWTOOTH, freq, 125);
    //        }
    //    }
    //}

    int range = 10;
    while (1) {
        for (i = 0; i < range; i++) {
            freq = get_freq(50);

            if (i > (range/2)) {
                freq += (range - i - (range/2));
            } else {
                freq += i;
            }

            note(SAWTOOTH, freq, 10);
        }
    }

    return 0;
}

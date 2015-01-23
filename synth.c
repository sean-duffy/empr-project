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

#define SECOND 1E9

int current_tick = 0;
double *wave_buf;
int duration_passed = 0;
int resolution = 360;
uint32_t note_length = 500;

int osc_1_value = 0;

double osc_1;
int osc_1_timer = 2000;
int osc_1_tick = 0;
double osc_1_point;

double osc_2;
int osc_2_timer = 30;
int osc_2_tick = 0;
double osc_2_point;

double wave_out;

void SysTick_Handler(void) {
    if(osc_1_tick > osc_1_timer)
        osc_1_tick = 0;

    if(osc_2_tick > osc_2_timer)
        osc_2_tick = 0;

    osc_1_point = (double) osc_1_tick/osc_1_timer;
    osc_2_point = (double) osc_2_tick/osc_2_timer;
    
    osc_1 = (point_noise_white(osc_1_point,0) + 1)/2;
    osc_2 = (point_sine(osc_2_point,0) + 1)/2;

    wave_out = mult_points(osc_1, osc_2, 1, 1);
    wave_out = mult_points(wave_out, osc_1,1,1);
    wave_out = mult_points(wave_out, osc_1,1,1);
    wave_out = add_points(wave_out, osc_1, 1,1);

    DAC_UpdateValue(LPC_DAC, (int) (wave_out*500));
    osc_1_tick++;
    osc_2_tick++;
}

void TIMER0_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT) == SET) {
            TIM_Cmd(LPC_TIM0, DISABLE);
            TIM_ResetCounter(LPC_TIM0);
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

void note(double *voice, double freq, double length) {
    wave_buf = voice;

    freq = 1/freq * 1389000;
    SysTick_Config((int) floor(freq)); // 2400
    note_length = length;
    while (duration_passed != 1);
    duration_passed = 0;
}

void rest(double length) {
    SysTick_Config(0);
    note_length = length;
    while (duration_passed != 1);
    duration_passed = 0;
}

double get_freq(int key_n){
    // Convert piano key number to frequency
    float f = pow(2, (key_n - 49)/ (float) 12) * 440;
    return (double) f;
}

int main(void) {
    init_dac();
    init_timer();

    wave_buf = (double *) calloc (resolution, sizeof(double));

    double voice_sine[resolution];
    generate_sine(voice_sine, 360);

    double voice_square[resolution];
    generate_square(voice_square, 360);

    double voice_triangle[resolution];
    generate_triangle(voice_triangle, 360);

    double voice_sawtooth[resolution];
    generate_sawtooth(voice_sawtooth, 360);
    

    note(voice_sine, get_freq(60), 125);
    /*
    double *voices[] = {voice_sine, voice_square, voice_triangle, voice_sawtooth};

    int i;
    int v;
    int n;
    double freq;
    double arp[] = {40, 44, 47, 52, 47, 44};

    while (1) {
        for (v = 0; v < 4; v++) {
            for (n = 0; n < 4; n++) {
                for (i = 0; i < 6; i++) {
                    freq = get_freq(arp[i]);
                    note(voices[v], freq, 125);
                }
            }
        }
    }

    double Cm[] = {40, 43, 47, 52, 55, 59, 64, 67, 71};
    double Bb[] = {38, 42, 45, 50, 54, 57, 62, 66, 69};
    double Fm[] = {33, 36, 40, 45, 48, 52, 57, 60, 64};

    double *arps[] = {Cm, Bb, Fm, Cm, Bb, Fm, Cm, Cm};

    while (1) {
        for (n = 0; n < 8; n++) {
            for (i = 0; i < 9; i++) {
                freq = get_freq(arps[n][i]);
                note(voice_sawtooth, freq, 125);
            }
            for (i = 8; i > 0; i--) {
                freq = get_freq(arps[n][i]);
                note(voice_sawtooth, freq, 125);
            }
        }
    }
    */
    //int range = 10;
    //while (1) {
    //    for (i = 0; i < range; i++) {
    //        freq = get_freq(50);

    //        if (i > (range/2)) {
    //            freq += (range - i - (range/2));
    //        } else {
    //            freq += i;
    //        }

    //        note(voice_sawtooth, freq, 10);
    //    }
    //}

    return 0;
}

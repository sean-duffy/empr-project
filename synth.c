#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "sound_modules.h"

#define SECOND 1E9

int current_tick = 0;
int *wave_buf;
int duration_passed = 0;
int resolution = 360;
uint32_t T1 = 500;

void SysTick_Handler(void) {
    if (current_tick >= 360) {
        current_tick = 0;
    }

    DAC_UpdateValue(LPC_DAC, wave_buf[current_tick]);
    current_tick += 5;
}

void TIMER0_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT) == SET) {
            TIM_Cmd(LPC_TIM0, DISABLE);
            TIM_ResetCounter(LPC_TIM0);
            TIM_UpdateMatchValue(LPC_TIM0, 0, T1 * 10);//MAT0.0
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

    T1 = 500;
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
    TIM_MatchConfigStruct.MatchValue = T1*10;
    TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);
    NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
    NVIC_EnableIRQ(TIMER0_IRQn);

    TIM_Cmd(LPC_TIM0,ENABLE);

    return 0;
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

void wait(double seconds) {

}

int main(void) {
    init_dac();
    init_timer();
    wave_buf = (int *) calloc (resolution, sizeof(int));
    generate_square(wave_buf, 2);
    int i;
    double freq;

    while(1) {
        for (i = 40; i < 88; i++) {
            freq = get_freq(i);
            wave(freq);
            while (duration_passed != 1);
            duration_passed = 0;
        }
    }

    return 0;
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"

#ifndef VOICE_STRUCT_GUARD
#define VOICE_STRUCT_GUARD

struct Voice {
    double *osc_1_buf;
    double osc_1_mix;

    double *osc_2_buf;
    double osc_2_detune;
    double osc_2_mix;
};

#endif

void init_dac(void);
int init_timer(void);
void set_resolution(int new_resolution);
void set_voice(struct Voice voice);
double get_freq(int key_n);
void play_note(struct Voice note_voice, double freq, double length);
void rest(double length);

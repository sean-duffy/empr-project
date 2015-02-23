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

#define RESOLUTION 360 

struct Voice {
    double *osc_1_buf;
    double osc_1_mix;
    double output_attack;
    double output_release;
    int envelope_on;

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
void note_on(double freq);
void note_off(void);

int scroll_counter;
double output_volume;

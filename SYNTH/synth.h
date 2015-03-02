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
#define RATE 0.00974999
#define NOTES_N 2

struct Voice {
    double *osc_1_buf;
    double osc_1_mix;
	
    int attack_len;
	int  decay_len;
	double sustain_level;
    int release_len;
	
    int envelope_on;

    double *osc_2_buf;
    double osc_2_detune;
    double osc_2_mix;
};

#endif

#ifndef guard
#define guard
struct Note {
		int active;
		
		int released;
		double tick;
		double inc;
		double value;
		
		double envelope;
		int ADSR_stage;
};

#endif

void init_dac(void);
int init_timer(void);
void set_resolution(int new_resolution);
void set_voice(struct Voice voice);
double get_freq(int key_n);
int note_on(double freq);
void note_off(int id);

int scroll_counter;
double output_volume;

char *first_line;

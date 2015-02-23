#include <stdio.h>
#include <math.h>
#include <stdlib.h>


/*#include "lpc_types.h"
/*#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
*/ // Removed for synth testing at home

#ifndef VOICE_STRUCT_GUARD
#define VOICE_STRUCT_GUARD

#define RESOLUTION 360
#define RATE 0.00974999
#define LFO_RATE 0.0005
#define ADSR_RATE 40

#define NOTES_N 1

struct Voice {
    double *osc_1_buf;
    double osc_1_mix;

    //Envelope
    int envelope_on;
    int attack_len;
	int  decay_len;
	double sustain_level;
    int release_len;
	
    //Delay
    int delay;

    //LFO
    double *lfo_buf;
    double lfo_freq;

};

#endif

#ifndef guard
#define guard
struct Note {
		int active;
        int released;
        
        double delay_tick;
	    double lfo_tick;	
        double tick;
		double inc;
        double value;
        double envelope;
        int ADSR_stage;
};

// ADDED FOR SYNTH TESTING
void SysTick_Handler(void);
//
struct synth_struct {
	int duration_passed;
	/* uint32_t*/ int note_length;
	int released;

	double osc_1_inc;
	double osc_1_tick;
	double *osc_1_buf;
	double osc_1_value;
	double osc_1_mix;

	int envelope_on;
	double output_envelope;
	double output_attack_inc;
	double output_release_inc;

	double osc_2_inc;
	double osc_2_tick;
	double *osc_2_buf;
	double osc_2_value;
	double osc_2_mix;

	double osc_3_inc;
	double osc_3_tick;
	double *osc_3_buf;
	double osc_3_value;
	double osc_3_mix;

	int note_mute;
	double mix_inc;
	double osc_mix;
	double output_volume;

	int scroll_counter;
	char *first_line;

	double dac_out;
};
// Functions

void init_dac(void);
int init_timer(void);
void set_resolution(int new_resolution);
void set_voice(struct Voice voice);
double get_freq(int key_n);
int note_on(double freq);
void note_off(void);

#endif


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "oscillator.h"
#include "synth.h"
#include "wave_sim.h"
#include "../LCD/lcd.h"
#include "../UART/uart.h"

#define debug_print(n, x) if(1) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(1) { write_serial(n, x); }

int duration_passed = 0;
int resolution = RESOLUTION;

int envelope_on;
double output_volume = 0.9;

//ADSR
double output_attack_inc = 0;
double output_decay_dec = 0;
double output_sustain_level = 0;
double output_release_dec = 0;

double osc_2_inc = 3;
double osc_2_tick = 0;
double *osc_2_buf;
double osc_2_value = 0;
double osc_2_mix;

double osc_1_inc = 3;
double osc_1_tick = 0;
double *osc_1_buf;
double osc_1_value;
//double osc_1_mix;

int note_mute = 1;
double mix_inc = 0.00002;
double osc_mix;

int scroll_counter = 0;
char *first_line;

int notes_n = 0;
int n_index = 0;
double output_value = 0;

struct Note notes[NOTES_MAX] = {{0}};

int find_empty_note_id(){
    int i;
	if (notes_n < NOTES_MAX){
		for(i=0; i < NOTES_MAX; i++){
			if(notes[i].active == 0){
				return i;
			}
		}
	}
	
	return -1;
}

void SysTick_Handler(void) {
	
	output_value = 0;
	
	for(n_index = 0; n_index < NOTES_MAX; n_index++){		
		if (notes[n_index].active == 0){
			continue;
		}
		
		if (notes[n_index].tick >= resolution) {
			notes[n_index].tick = 0;
		}

		notes[n_index].value = osc_1_buf[(int) floor(notes[n_index].tick)];
		//osc_1_value = osc_1_buf[(int) floor(osc_1_tick)];
		
		if (notes[n_index].envelope < 0) {
			notes[n_index].envelope = 0;
		}
		
		// Attack
		if (envelope_on){
			if (notes[n_index].released == 0){
				if (notes[n_index].ADSR_stage == 0){ // Attack Stage
					notes[n_index].envelope += output_attack_inc;
				} else if (notes[n_index].ADSR_stage == 1){ // Delay Stage
					notes[n_index].envelope += output_decay_dec;
				} else { // Sustain
					notes[n_index].envelope = output_sustain_level;
				}
				
				// Recheck Stages
				if (notes[n_index].envelope > 1) { 
					notes[n_index].ADSR_stage = 1; 
					notes[n_index].envelope = 1;
				}
				
				if ( notes[n_index].ADSR_stage == 1 && notes[n_index].envelope < output_sustain_level ) {
                    notes[n_index].ADSR_stage = 2;
                }
				
			} else if (notes[n_index].envelope > 0) { // Release
				if (notes[n_index].envelope > output_sustain_level) {
					notes[n_index].envelope = output_sustain_level;
				}
				notes[n_index].envelope += output_release_dec;
			} else {
                notes[n_index].active = 0; // Endes the note once it is released and finished
                notes_n--;
            }
		}

		notes[n_index].tick += notes[n_index].inc;
		notes[n_index].tick += notes[n_index].inc;
		
		output_value += notes[n_index].envelope * notes[n_index].value;
	}

	output_value = ((output_volume * output_value)+1) * 300 * note_mute;
	DAC_UpdateValue(LPC_DAC, output_value);
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

int note_on(double freq) {
	int note_id;
    note_id = find_empty_note_id();	

    if (note_id == NULL){
    char buf[10];
    int l = sprintf(buf, "null note_id = %d", note_id);
    debug_print(buf, l);
    }

    if(note_id == -1){
		return -1;
	} else {
		notes[note_id].id = note_id;
		notes[note_id].released = 0;
		notes[note_id].active = 1;
		notes[note_id].tick = 0;
		notes[note_id].inc = (double) RATE * freq;
		notes[note_id].value = 0;
		
		if(envelope_on){
			notes[note_id].envelope = 0;
			notes[note_id].ADSR_stage = 0;
		}

        notes_n++;
        return note_id;
	}
    
}

void note_off(int id) {
    if (id == -1){
        debug_print("id invalid", strlen("id invalid"));
    }
    if (envelope_on) {
        notes[id].released = 1;
    } else {
        notes[id].envelope = 0;
    }
}

double get_freq(int key_n) {
    // Convert piano key number to frequency
    double f = pow(2, (key_n - 69)/ 12.0) * 440;
    return f;
}

void set_voice(struct Voice voice) {
    osc_2_mix = voice.osc_2_mix;
    osc_1_buf = voice.osc_1_buf;
    osc_2_buf = voice.osc_2_buf;

	//Setup ADSR
    envelope_on = voice.envelope_on;
	output_sustain_level = voice.sustain_level;
    output_attack_inc =  + (float) 1/voice.attack_len ;
	output_decay_dec =  - (float) (1-voice.sustain_level)/voice.decay_len ;
    output_release_dec = - (float) (voice.sustain_level)/voice.release_len;
}


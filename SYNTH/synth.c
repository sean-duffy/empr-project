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
double *wave;
int scroll_counter = 0;
char *first_line;

double output_value = 0;
int output_delay = 0;

//LFO
double *lfo_wave;
double lfo_inc;
double lfo_tick;

//Note structures
struct Note note_1 = {0};
struct Note note_2 = {0};
struct Note *notes[NOTES_N] = {&note_1, &note_2};

int get_free_note_id(){
        return 0;
}

void SysTick_Handler(void) {
	
	output_value = 0;

    int i;
    for(i = 0; i < NOTES_N; i ++){

        //if (notes[i]->active == 0){ continue;} // Skip if not not active
        if (notes[i]->tick >= resolution) { notes[i]->tick = 0;} // Modulus tick by resolution
        if (notes[i]->lfo_tick>= resolution) { notes[i]->lfo_tick = 0;} // modulus lfo tick by resolution

        notes[i]->value = wave[(int) floor(notes[i]->tick)] * lfo_wave[(int) floor(notes[i]->lfo_tick)] * notes[i]->active;

        
        if (notes[i]->envelope < 0) {
            notes[i]->envelope = 0;
        }
        
        if(notes[i]->delay_tick < output_delay){
            notes[i]->delay_tick++;
            continue;
        }

        // ADSR
        //
        if (envelope_on){
            if (notes[i]->released == 0){
                if (notes[i]->ADSR_stage == 0){ // Attack Stage
                    notes[i]->envelope += output_attack_inc;
                } else if (notes[i]->ADSR_stage == 1){ // Delay Stage
                    notes[i]->envelope += output_decay_dec;
                } else { // Sustain
                    notes[i]->envelope = output_sustain_level;
                }
                
                // Recheck Stages
                if (notes[i]->envelope > 1) { 
                    notes[i]->ADSR_stage = 1; 
                    notes[i]->envelope = 1;
                }
                
                if ( notes[i]->ADSR_stage == 1 && notes[i]->envelope < output_sustain_level ) {
                    notes[i]->ADSR_stage = 2;
                }
                
            } else if (note_1.envelope > 0) { // Release
                if (notes[i]->envelope > output_sustain_level) {
                    notes[i]->envelope = output_sustain_level;
                }
                notes[i]->envelope += output_release_dec;
            } else {
                notes[i]->active = 0; // Endes the note once it is released and finished
            }

            notes[i]->tick += notes[i]->inc;
            notes[i]->lfo_tick += lfo_inc;
            output_value += notes[i]->envelope * notes[i]->value;
        }
    }
	
    output_value = ((output_volume * output_value)+1) * 400; 
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
        int id = get_free_note_id();

		notes[id]->released = 0;
		notes[id]->active = 1;
        notes[id]->delay_tick = 0;
        notes[id]->lfo_tick = 0;
		notes[id]->tick = 0;
		notes[id]->inc = (double) RATE * freq;
		notes[id]->value = 0;
		
		if(envelope_on){
			notes[id]->envelope = 0;
			notes[id]->ADSR_stage = 0;
		}

        return id;
}

void note_off() {
    int id = 0;
    if (envelope_on) {
        notes[id]->released = 1;
    } else {
        notes[id]->envelope = 0;
    }
}

double get_freq(int key_n) {
    // Convert piano key number to frequency
    double f = pow(2, (key_n - 69)/ 12.0) * 440;
    return f;
}

void set_voice(struct Voice voice) {
    wave = voice.osc_1_buf;

    output_delay = voice.delay;
    lfo_wave = voice.lfo_buf;
    lfo_inc = RATE * voice.lfo_freq;

	//Setup ADSR
    envelope_on = voice.envelope_on;
	output_sustain_level = voice.sustain_level;
    output_attack_inc =  + (float) 1/voice.attack_len ;
	output_decay_dec =  - (float) (1-voice.sustain_level)/voice.decay_len ;
    output_release_dec = - (float) (voice.sustain_level)/voice.release_len;
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "oscillator.h"
#include "synth.h"
#include "wave_sim.h"
//#include "LCD/lcd.h"

int duration_passed = 0;
int resolution = RESOLUTION;
int note_length = 500;

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

struct Note notes[NOTES_MAX] = {{-1}};

int i;
int find_empty_note_id(){
	if (notes_n < NOTES_MAX){
		for(i=0; i < NOTES_MAX; i++){
			if(notes[i].id == -1){
				return i;
			};
		}
	}
	
	return -1;
}

void SysTick_Handler(void) {

	//printf("\n-------\n");
	//printf("<id=%d>\n", notes[0].id);
	//printf("<released=%d>\n", notes[0].released);
	//printf("<tick=%f>", notes[0].tick);
	//printf("<inc=%f>", notes[0].inc);
	//printf("<val=%f>", notes[0].value);
    
	double output_value;
	
    if (notes[0].tick >= resolution) {
        notes[0].tick = 0;
    }
	
	/*
    if (scroll_counter > 30000) {
        scroll_counter = 0;
        scroll_first_line(&I2CConfigStruct, first_line, strlen(first_line));
    } else {
        scroll_counter++;
    }*/

	// Low pass filter
    //if (osc_1_mix >= 1 || (osc_1_mix <= 0 && mix_inc < 0)) {
    //    mix_inc *= -1;
    //}

	notes[0].value = osc_1_buf[(int) floor(notes[0].tick)];
    //osc_1_value = osc_1_buf[(int) floor(osc_1_tick)];
	
    if (notes[0].envelope < 0) {
        notes[0].envelope = 0;
    }

    //osc_mix = ((double) output_volume / 10.0) * (osc_1_value*osc_1_mix*output_envelope + osc_2_value*osc_2_mix);
    output_value = ((output_volume * notes[0].envelope * notes[0].value)+1) * 300;

    //DAC_UpdateValue(LPC_DAC, output_value * note_mute);
	plot_print(output_value * note_mute);
	
    // Attack
	if (envelope_on){
		if (notes[0].released == 0){
			if (notes[0].ADSR_stage == 0){ // Attack Stage
				notes[0].envelope += output_attack_inc;
			} else if (notes[0].ADSR_stage == 1){ // Delay Stage
				notes[0].envelope += output_decay_dec;
			} else { // Sustain
				notes[0].envelope = output_sustain_level;
			}
			
			// Recheck Stages
			if (notes[0].envelope > 1) { 
				notes[0].ADSR_stage = 1; 
				notes[0].envelope = 1;
			}
			
			if ( notes[0].ADSR_stage == 1 && notes[0].envelope < output_sustain_level ) { notes[0].ADSR_stage = 2;}
			
		} else if (notes[0].envelope > 0) { // Release
			if (notes[0].envelope > output_sustain_level) {
				notes[0].envelope = output_sustain_level;
			}
			notes[0].envelope += output_release_dec;
		}
	}

    notes[0].tick += notes[0].inc;
    notes[0].tick += notes[0].inc;
}

/*
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
}*/

void note_on(double freq) {
	
	int note_id = 0;//find_empty_note_id();
	if(note_id == -1){
		return;
	} else {
		notes_n++;
		notes[note_id].id = note_id;
		notes[note_id].released = 0;
		notes[note_id].tick = 0;
		notes[note_id].inc = (double) RATE * freq;
		notes[note_id].value = 0;
		
		if(envelope_on){
			notes[note_id].envelope = 0;
			notes[note_id].ADSR_stage = 0;
		}
	}
	
    //osc_1_inc = RATE * freq; // Bit rate callibrated to middle C
    //osc_2_inc = osc_1_inc;

    //if (envelope_on) {
    //    output_envelope = 0;
    //} else {
    //    output_envelope = 1;
    //}
}

void note_off(void) {
    if (envelope_on) {
        notes[0].released = 1;
    } else {
        notes[0].envelope = 0;
    }
	notes_n--;
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

void set_resolution(int new_resolution) {
    resolution = new_resolution;
}

int main(){

	init_print();
	
	double buf_1[RESOLUTION], buf_2[RESOLUTION];
	int i;
	set_voice_by_id(1, buf_1, buf_2);
	
	note_off();
	
	char buff[30];
	for(i = 0; i < 5000; i++){
		SysTick_Handler();
	}
	
	int ni = 0;
	while (ni < 128){
		note_on(get_freq(0 + ni));

		for(i = 0; i < 4000; i++){
			SysTick_Handler();
		}
		
		note_off();
		
		for(i = 0; i < 4000; i++){
			SysTick_Handler();
		}
		
		ni++;
	}
	/*
	note_on(get_freq(50));
	
	for(i = 0; i < 3000; i++){
		SysTick_Handler();
	}*/
	
	
	close_print();

}
#include "instruments.h"

// Initialises all instruments (voices)
void init_all_voices(){
	int i;
	for( i=0; i< sizeof(all_voices); i++){
		(*all_voices[i])();
	}
}

// voice initialisers go here ...
void init_voice_template(){
	//Combine two differnt waves
	double wave_buf_1[resolution];
	double wave_buf_2[resolution];
	//generate_triangle(wave_buf_1, resolution);
	//generate_noise(wave_buf_2, resolution);
	//low_pass_filter(wave_buf_1, resolution, 8);
	//low_pass_filter(wave_buf_2, resolution, 8);
	
	// Oscillator properties
	voice_template.osc_1_buf = wave_buf_1;
	voice_template.osc_1_mix = 0.8;
	voice_template.osc_2_buf = wave_buf_2;
	voice_template.osc_2_mix = 0.2;
	voice_template.osc_2_detune = 0;
	voice_template.output_attack = 0.8;
	voice_template.output_release = 0.8;
	voice_template.envelope_on = 1 ;
}

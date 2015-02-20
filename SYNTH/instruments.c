#include "instruments.h"

// voice initialisers go here ...

void init_voice_template(double wave_buf_1[], double wave_buf_2[]){
	
    //Apply effects
    generate_triangle(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_template.osc_1_buf = wave_buf_1;
    voice_template.osc_1_mix = 0.5;
    voice_template.osc_2_buf = wave_buf_2;
    voice_template.osc_2_mix = 0.5;
    voice_template.osc_2_detune = 0;
    voice_template.output_attack = 1;
    voice_template.output_release = 0.8;
    voice_template.envelope_on = 0;
}


void init_voice_1(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_square(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_1.osc_1_buf = wave_buf_1;
    voice_1.osc_1_mix = 0.5;
    voice_1.osc_2_buf = wave_buf_2;
    voice_1.osc_2_mix = 0.5;
    voice_1.osc_2_detune = 0;
    voice_1.output_attack = 1;
    voice_1.output_release = 0.8;
    voice_1.envelope_on = 0;
}

void init_voice_2(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_square(wave_buf_1, RESOLUTION);
    generate_triangle(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_2.osc_1_buf = wave_buf_1;
    voice_2.osc_1_mix = 0.5;
    voice_2.osc_2_buf = wave_buf_2;
    voice_2.osc_2_mix = 0.5;
    voice_2.osc_2_detune = 0;
    voice_2.output_attack = 1;
    voice_2.output_release = 0.8;
    voice_2.envelope_on = 0;
}

void init_voice_3(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_sawtooth(wave_buf_1, RESOLUTION);
    generate_noise(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_3.osc_1_buf = wave_buf_1;
    voice_3.osc_1_mix = 0.5;
    voice_3.osc_2_buf = wave_buf_2;
    voice_3.osc_2_mix = 0.5;
    voice_3.osc_2_detune = 0;
    voice_3.output_attack = 1;
    voice_3.output_release = 0.8;
    voice_3.envelope_on = 0;
}

void init_voice_4(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_sawtooth(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_4.osc_1_buf = wave_buf_1;
    voice_4.osc_1_mix = 0.5;
    voice_4.osc_2_buf = wave_buf_2;
    voice_4.osc_2_mix = 0.5;
    voice_4.osc_2_detune = 0;
    voice_4.output_attack = 1;
    voice_4.output_release = 0.8;
    voice_4.envelope_on = 0;
}

void init_voice_5(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_sawtooth(wave_buf_1, RESOLUTION);
    generate_square(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_5.osc_1_buf = wave_buf_1;
    voice_5.osc_1_mix = 0.5;
    voice_5.osc_2_buf = wave_buf_2;
    voice_5.osc_2_mix = 0.5;
    voice_5.osc_2_detune = 0;
    voice_5.output_attack = 1;
    voice_5.output_release = 0.8;
    voice_5.envelope_on = 0;
}

void init_voice_6(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_square(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2);
	
	// Oscillator properties
    voice_6.osc_1_buf = wave_buf_1;
    voice_6.osc_1_mix = 0.8;
    voice_6.osc_2_buf = wave_buf_2;
    voice_6.osc_2_mix = 0.2;
    voice_6.osc_2_detune = 0;
    voice_6.output_attack = 0.8;
    voice_6.output_release = 0.8;
    voice_6.envelope_on = 1;
}


void set_voice_by_id(int voice_id, double wave_buf_1[], double wave_buf_2[]){

    memset(wave_buf_1, '0', RESOLUTION);
    memset(wave_buf_2, '0', RESOLUTION);

    switch (voice_id) {
        case 1:
            init_voice_1(wave_buf_1, wave_buf_2);
            set_voice(voice_1);
            break;
        case 2:
            init_voice_2(wave_buf_1, wave_buf_2);
            set_voice(voice_2);
            break;
        case 3:
            init_voice_3(wave_buf_1, wave_buf_2);
            set_voice(voice_3);
            break;
        case 4:
            init_voice_4(wave_buf_1, wave_buf_2);
            set_voice(voice_4);
            break;
        case 5:
            init_voice_5(wave_buf_1, wave_buf_2);
            set_voice(voice_5);
            break;
        case 6:
            init_voice_6(wave_buf_1, wave_buf_2);
            set_voice(voice_6);
            break;
        default: 
            init_voice_template(wave_buf_1, wave_buf_2);
            set_voice(voice_template);
    }
}


#include "instruments.h"

int len_in_ms(int s){
    return s * ADSR_RATE;
}

void init_voice_1(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_sine(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_1.osc_1_buf = wave_buf_1;
	
    voice_1.attack_len = len_in_ms(1);
	voice_1.decay_len = len_in_ms(1);
	voice_1.sustain_level = 1;
    voice_1.release_len = len_in_ms(1);
    
    voice_1.delay = len_in_ms(1);

    voice_1.envelope_on = 1;
    
    voice_1.lfo_on = 1;
    voice_1.lfo_buf = wave_buf_2;
    voice_1.lfo_freq = 3; //Hz
    voice_1.lfo_mix = 0.3;
}

void init_voice_2(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_triangle(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_2.osc_1_buf = wave_buf_1;
	
    voice_2.attack_len = len_in_ms(5);
	voice_2.decay_len = len_in_ms(60);
	voice_2.sustain_level = 0;
    voice_2.release_len = len_in_ms(1);
    
    voice_2.delay = len_in_ms(0);

    voice_2.envelope_on = 1;
    voice_2.lfo_on = 1;

    voice_2.lfo_buf = wave_buf_2;
    voice_2.lfo_freq = 2; //Hz
    voice_2.lfo_mix = 0;
}


void init_voice_3(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_triangle(wave_buf_1, RESOLUTION);
    generate_noise(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_3.osc_1_buf = wave_buf_1;
	
    voice_3.attack_len = len_in_ms(1);
	voice_3.decay_len = len_in_ms(300);
	voice_3.sustain_level = 0;
    voice_3.release_len = len_in_ms(1);
    
    voice_3.delay = len_in_ms(1);

    voice_3.envelope_on = 1;
    voice_3.lfo_on = 1;

    voice_3.lfo_buf = wave_buf_2;
    voice_3.lfo_freq = 2; //Hz
    voice_3.lfo_mix = 0.5;
}

void init_voice_4(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_sawtooth(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_4.osc_1_buf = wave_buf_1;
	
    voice_4.attack_len = len_in_ms(500);
	voice_4.decay_len = len_in_ms(2);
	voice_4.sustain_level = 0.5;
    voice_4.release_len = len_in_ms(200);
    
    voice_4.delay = len_in_ms(200);

    voice_4.envelope_on = 1;
    voice_4.lfo_on = 1;

    voice_4.lfo_buf = wave_buf_2;
    voice_4.lfo_freq = 10; //Hz
    voice_4.lfo_mix = 0.5;
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
    }
}


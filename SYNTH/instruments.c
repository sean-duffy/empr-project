#include "instruments.h"

// voice initialisers go here ...

void init_voice_1(double wave_buf_1[], double wave_buf_2[]){

    //Apply effects
    generate_sine(wave_buf_1, RESOLUTION);
    generate_sine(wave_buf_2, RESOLUTION);
	
	// Oscillator properties
    voice_1.osc_1_buf = wave_buf_1;
    voice_1.osc_1_mix = 0.5;
    voice_1.osc_2_buf = wave_buf_2;
    voice_1.osc_2_mix = 0.5;
    voice_1.osc_2_detune = 0;
	
    voice_1.attack_len = 5000;
	voice_1.decay_len = 2000;
	voice_1.sustain_level = 0.5;
    voice_1.release_len = 100000;

    voice_1.envelope_on = 1;
}

void set_voice_by_id(int voice_id, double wave_buf_1[], double wave_buf_2[]){

    memset(wave_buf_1, '0', RESOLUTION);
    memset(wave_buf_2, '0', RESOLUTION);

    switch (voice_id) {
        case 1:
            init_voice_1(wave_buf_1, wave_buf_2);
            set_voice(voice_1);
            break;
    }
}


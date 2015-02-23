#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "oscillator.h"
#include "synth.h"
//#include "LCD/lcd.h" commented for synth testing
#include "wave_sim.h" // ADDED FOR SYNTH TESTING

static struct synth_struct synth = {100, 500, 0, 3};

void init_synth(){
	synth.duration_passed = 100;
	synth.note_length = 500; // CHANGED FOR SYNTH TESTING
	synth.released = 0;

	synth.osc_1_inc = 3;
	printf("osc_1_inc = %d\n", synth.osc_1_inc);
	synth.osc_1_tick = 0;

	synth.output_envelope = 1;
	synth.output_attack_inc = 0;
	synth.output_release_inc = 0;

	synth.osc_2_inc = 3;
	synth.osc_2_tick = 0;
	synth.osc_2_value = 0;

	synth.osc_3_inc = 30;
	synth.osc_3_tick = 0;
	synth.osc_3_value = 0;

	synth.mix_inc = 0.00002;
	synth.output_volume = 0.9;
	synth.scroll_counter = 0;
}

void SysTick_Handler(void) {
    double output_value;

    if (synth.osc_1_tick >= RESOLUTION) {
        synth.osc_1_tick = 0;
    }

    if (synth.osc_2_tick >= RESOLUTION) {
        synth.osc_2_tick = 0;
    }

	/* KEYPAD COMMENTED FOR SYNTH TESTING
    if (scroll_counter > 30000) {
        scroll_counter = 0;
        scroll_first_line(&I2CConfigStruct, first_line, strlen(first_line));
    } else {
        scroll_counter++;
    }*/

    //if (osc_1_mix >= 1 || (osc_1_mix <= 0 && mix_inc < 0)) {
    //    mix_inc *= -1;
    //}

    synth.osc_1_value = synth.osc_1_buf[(int) floor(synth.osc_1_tick)];
    synth.osc_2_value = synth.osc_2_buf[(int) floor(synth.osc_2_tick)];

    if (synth.output_envelope < 0) {
        synth.output_envelope = 0;
    }

    //osc_mix = ((double) output_volume / 10.0) * (osc_1_value*osc_1_mix*output_envelope + osc_2_value*osc_2_mix);
    synth.osc_mix = synth.output_volume * synth.output_envelope * (synth.osc_1_value*synth.osc_1_mix+ synth.osc_2_value*synth.osc_2_mix);
    output_value = (int) floor((synth.osc_mix + 1.0) * 300);

	// REPLACED WITH wave plotter
    // DAC_UpdateValue(LPC_DAC, output_value * note_mute);
	// plot_print(output_value * note_mute);
	// ---------------------------
	

    // Attack
    if (synth.output_envelope < 1 && synth.released == 0 && synth.envelope_on) {
        synth.output_envelope += synth.output_attack_inc;
    }

    // Release
    if (synth.output_envelope > 0 && synth.released == 1 && synth.envelope_on) {
        synth.output_envelope += synth.output_release_inc;
    }

	//printf("osc_1:%d\t", osc_1_tick);
    synth.osc_1_tick += synth.osc_1_inc;
	//printf("osc_1:%d\t", osc_1_tick);
	//printf("osc_1_inc:%d\n", osc_1_inc);
    synth.osc_1_tick += synth.osc_1_inc;
    synth.osc_2_tick += synth.osc_2_inc;

    //osc_1_mix += mix_inc;
    //osc_2_mix -= mix_inc;
	
	synth.duration_passed++;
}

/*void init_dac(void) {
    PINSEL_CFG_Type PinCfg; 

    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    
    //change pin number
    PinCfg.Pinnum = 26;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    DAC_Init (LPC_DAC);
}*/ // COMMENTED OUT FOR SYNTH TESTING AT HOME

void note_on(double freq) {
    synth.released = 0;
    synth.osc_1_inc = 0.00974999 * freq; // Bit rate callibrated to middle C
    synth.osc_2_inc = synth.osc_1_inc;

    if (synth.envelope_on) {
        synth.output_envelope = 0;
    } else {
        synth.output_envelope = 1;
    }
}

void note_off(void) {
    if (synth.envelope_on) {
        synth.released = 1;
    } else {
        synth.output_envelope = 0;
    }
}

double get_freq(int key_n) {
    // Convert piano key number to frequency
    double f = pow(2, (key_n - 69)/ 12.0) * 440;
    return f;
}

void set_voice(struct Voice voice) {
    synth.osc_1_mix = voice.osc_1_mix;
    synth.osc_2_mix = voice.osc_2_mix;
    synth.osc_1_buf = voice.osc_1_buf;
    synth.osc_2_buf = voice.osc_2_buf;
	synth.envelope_on = voice.envelope_on;
    synth.output_attack_inc = 0.001 * voice.output_attack;
    synth.output_release_inc = -0.001 * voice.output_release;
    synth.mix_inc = 0;
}

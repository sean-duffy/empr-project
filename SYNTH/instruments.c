#include "synth.h"
#include "instruments.h"

#define resolution 360

double wave_buf_1[resolution];
generate_triangle(wave_buf_1, resolution);

double wave_buf_2[resolution];
generate_noise(wave_buf_2, resolution);

low_pass_filter(wave_buf_1, resolution, 8);
low_pass_filter(wave_buf_2, resolution, 8);

eggs.osc_1_buf = wave_buf_1;
eggs.osc_1_mix = 0.8;
eggs.osc_2_buf = wave_buf_2;
eggs.osc_2_mix = 0.2;
eggs.osc_2_detune = 0;
eggs.output_attack = 0.8;
eggs.output_release = 0.8;
eggs.envelope_on = 1;



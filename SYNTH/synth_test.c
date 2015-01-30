<<<<<<< HEAD
#include "oscillator.h"
#include "synth.h"

int main(void) {
    init_dac();
    init_timer();
    int resolution = 360;
    set_resolution(resolution);

    double voice_sine[resolution];
    generate_sine(voice_sine, resolution);

    double voice_square[resolution];
    generate_square(voice_square, resolution);

    double voice_sawtooth[resolution];
    generate_sawtooth(voice_sawtooth, resolution);

    double voice_sawtooth_filter[resolution];
    generate_sawtooth(voice_sawtooth_filter, resolution);
    low_pass_filter(voice_sawtooth_filter, resolution, 16);

    struct Voice voice_1;
    voice_1.osc_1_buf = voice_sawtooth;
    voice_1.osc_1_mix = 1;
    voice_1.osc_2_buf = voice_sawtooth_filter;
    voice_1.osc_2_mix = 0;
    voice_1.osc_2_detune = 0;

    set_voice(voice_1);
    
    int i;
    int v;
    int n;
    double freq;
    double arp[] = {40, 44, 47, 52, 47, 44};

    //while (1) {
    //    for (v = 0; v < 4; v++) {
    //        for (n = 0; n < 4; n++) {
    //            for (i = 0; i < 6; i++) {
    //                freq = get_freq(arp[i]);
    //                note(voice_1, freq, 125);
    //            }
    //        }
    //    }
    //}

    double Cm[] = {40, 43, 47, 52, 55, 59, 64, 67, 71};
    double Bb[] = {38, 42, 45, 50, 54, 57, 62, 66, 69};
    double Fm[] = {33, 36, 40, 45, 48, 52, 57, 60, 64};

    double *arps[] = {Cm, Bb, Fm, Cm, Bb, Fm, Cm, Cm};

    while (1) {
        for (n = 0; n < 8; n++) {
            for (i = 0; i < 9; i++) {
                freq = get_freq(arps[n][i]);
                note(voice_1, freq, 125);
            }
            for (i = 7; i > 0; i--) {
                freq = get_freq(arps[n][i]);
                note(voice_1, freq, 125);
            }
        }
    }

    return 0;
}
||||||| merged common ancestors
=======
#include "oscillator.h"
#include "synth.h"

int main(void) {
    init_dac();
    init_timer();
    int resolution = 360;
    set_resolution(resolution);

    double voice_sine[resolution];
    generate_sine(voice_sine, resolution);

    double voice_square[resolution];
    generate_square(voice_square, resolution);

    double voice_sawtooth[resolution];
    generate_sawtooth(voice_sawtooth, resolution);

    double voice_sawtooth_filter[resolution];
    generate_sawtooth(voice_sawtooth_filter, resolution);
    low_pass_filter(voice_sawtooth_filter, resolution, 16);

    struct Voice voice_1;
    voice_1.osc_1_buf = voice_sawtooth;
    voice_1.osc_1_mix = 1;
    voice_1.osc_2_buf = voice_sawtooth_filter;
    voice_1.osc_2_mix = 0;
    voice_1.osc_2_detune = 0;

    set_voice(voice_1);
    
    int i;
    int v;
    int n;
    double freq;
    double arp[] = {40, 44, 47, 52, 47, 44};

    //while (1) {
    //    for (v = 0; v < 4; v++) {
    //        for (n = 0; n < 4; n++) {
    //            for (i = 0; i < 6; i++) {
    //                freq = get_freq(arp[i]);
    //                note(voice_1, freq, 125);
    //            }
    //        }
    //    }
    //}

    double Cm[] = {40, 43, 47, 52, 55, 59, 64, 67, 71};
    double Bb[] = {38, 42, 45, 50, 54, 57, 62, 66, 69};
    double Fm[] = {33, 36, 40, 45, 48, 52, 57, 60, 64};

    double *arps[] = {Cm, Bb, Fm, Cm, Bb, Fm, Cm, Cm};

    while (1) {
        for (n = 0; n < 8; n++) {
            for (i = 0; i < 9; i++) {
                freq = get_freq(arps[n][i]);
                play_note(voice_1, freq, 125);
            }
            for (i = 7; i > 0; i--) {
                freq = get_freq(arps[n][i]);
                play_note(voice_1, freq, 125);
            }
        }
    }

    return 0;
}
>>>>>>> Basic working example

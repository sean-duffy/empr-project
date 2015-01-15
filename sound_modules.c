#include <math.h>

void generate_sine(int *buf, double amplitude) {
    amplitude = amplitude * 309.1;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        buf[i] = (int) floor((amplitude * sin((2*M_PI/resolution)*i) + amplitude) / 2);
    }
}

void generate_square(int *buf, double amplitude) {
    amplitude = amplitude * 309.1;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        if (i < (resolution / 2)) {
            buf[i] = 1;
        } else {
            buf[i] = -1;
        }
    }
}

void generate_sawtooth(int *buf, double amplitude) {
    amplitude = amplitude * 309.1;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        if (i < (resolution / 2)) {
            buf[i] = amplitude * (i / (resolution / 2.0)) + amplitude;
        } else {
            buf[i] = amplitude * (i / (resolution / 2.0)) - amplitude;
        }
    }
}

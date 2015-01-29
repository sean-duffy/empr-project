#include <stdio.h>
#include <stdlib.h>
#include "oscillator.h"

int main(void) {
    double *wave_buf;
    int resolution = 360;

    wave_buf = (double *) calloc (resolution, sizeof(double));
    generate_sawtooth(wave_buf, resolution);
    low_pass_filter(wave_buf, resolution, 8);

    for (int i = 0; i < resolution; i++) {
        printf("%f\n", wave_buf[i]);
    }

    return 0;
}

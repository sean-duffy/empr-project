#include <stdio.h>
#include <stdlib.h>
#include "oscillator.h"

int main(void) {
    double *wave_buf;
    int resolution = 360;

    wave_buf = (double *) calloc (resolution, sizeof(double));
    generate_white_noise(wave_buf, resolution);

    int i;
    for (i = 0; i < resolution; i++) {
        printf("%f\n", wave_buf[i]);
    }

    return 0;
}

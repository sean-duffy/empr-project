#include <stdio.h>
#include <stdlib.h>
#include "oscillator.h"

int main(void) {
    double *wave_buf;
    int resolution = 360;

    wave_buf = (double *) calloc (resolution, sizeof(double));
    generate_square(wave_buf, resolution);

    for (int i = 0; i < resolution; i++) {
        printf("%f\n", wave_buf[i]);
    }

    return 0;
}

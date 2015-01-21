#include <stdio.h>
#include <stdlib.h>
#include "sound_modules.h"

int main(void) {
    int *wave_buf;
    int resolution = 360;

    wave_buf = (int *) calloc (resolution, sizeof(int));
    generate_sawtooth(wave_buf, 2);

    for (int i = 0; i < resolution; i++) {
        printf("%d\n", wave_buf[i]);
    }
}

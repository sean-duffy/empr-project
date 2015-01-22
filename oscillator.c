#include <math.h>

float point_sine(float point, float offset){
    point = fmod((point+offset),1.0) * 2*M_PI;
    return sin(point);
}

float point_sawtooth(float point, float offset){
    return fmod((point+offset+0.5), 1.0);
}

float point_square(float point, float offset){
    point = fmod((point+offset),1.0);

    if (point <0.5){
        return 1;
    } else {
        return 0;
    }
}

float point_triangle(float point, float offset){
    point = fmod((point+offset + 0.5),1.0);

    if (point < 0.5){
        return ((point*2) - 1);
    } else {
        return (1 - (point * 2));
    }
}

void generate_sine(int *buf, double amplitude) {
    amplitude = amplitude * 380;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        buf[i] = (int) floor((amplitude * point_sine((float) i/resolution, 0) + amplitude) / 2);
    }
}

void generate_sawtooth(int *buf, double amplitude) {
    amplitude = amplitude * 380;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        buf[i] = (int) floor((amplitude * point_sawtooth((float) i/resolution, 0) + amplitude) / 2);
    }
}

void generate_square(int *buf, double amplitude) {
    amplitude = amplitude * 402;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        buf[i] = (int) floor((amplitude * point_square((float) i/resolution, 0) + amplitude) / 2);
    }
}

void generate_triangle(int *buf, double amplitude) {
    amplitude = amplitude * 380;
    int resolution = 360;

    int i;
    for(i = 0; i < resolution; i++) {
        buf[i] = (int) floor((amplitude * point_triangle((float) i/resolution, 0) + amplitude) / 2);
    }
}

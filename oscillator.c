#include <math.h>
#include <stdlib.h>

// Wave functions
double point_sine(double point, double offset) {
    point = fmod((point + offset), 1.0) * 2*M_PI;
    return sin(point);
}

double point_sawtooth(double point, double offset) {
    return ((2*fmod((point + offset + 0.5), 1.0)) - 1);
}

double point_square(double point, double offset) {
    point = fmod((point + offset), 1.0);
    if (point < 0.5) {
        return 1;
    } else {
        return -1;
    }
}

double point_triangle(double point, double offset) {
    point = fmod((point + offset + 0.25),1.0);

    if (point < 0.5) {
        return (-1 + (point*4));
    } else {
        return (1 - ((point -0.5) * 4));
    }
}

// Noise functions

double point_noise_white(){
    int r = (rand() % 10000000);
    r = 2 * r -10000000;
    return (double) r/10000000;
}

double point_noise_pink(){
    // Tricky, will implement when required
    return 0;
}

// Operators
double add_points(double p_a, double p_b, double scale_a, double scale_b){
    return (p_a*scale_a + p_b*scale_b)/2;
}

double mult_points(double p_a, double p_b, double scale_a, double scale_b){
    return (p_a*scale_a * p_b*scale_b);
}

// Buffer generators
void generate_sine(double *buf, int res) {
    int i;
    for (i = 0; i < res; i++) {
        buf[i] = point_sine((double) i/ res, 0);
    }
}

void generate_square(double *buf, int res) {
    int i;
    for (i = 0; i < res; i++) {
        buf[i] = point_square((double) i/res, 0);
    }
}

void generate_sawtooth(double *buf, int res) {
    int i;
    for (i = 0; i < res; i++) {
        buf[i] = point_sawtooth((double) i/res, 0);
    }
}

void generate_triangle(double *buf, int res) {
    int i;
    for (i = 0; i < res; i++) {
        buf[i] = point_triangle((double) i/res, 0);
    }
}

void generate_white_noise(double *buf, int res) {
    int i;
    for (i = 0; i < res; i++) {
        buf[i] = point_noise_white();
    }
}   



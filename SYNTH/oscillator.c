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

double point_rand(){
    return (float) (rand()%100)/100.0;
}

// Buffer generators

void generate_noise(double *buf, int res) {
    int i;
    for (i = 0; i < res; i++) {
        buf[i] = point_rand();
    }
}

void generate_sine(double *buf, int res) {
    int i, x;
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

void low_pass_filter(double *buf, int buflen, int n) {
    int i;
    int j;
    double new_value;
    for (i = 0; i < buflen; i++) {
        if (i >= (n - 1)) {

            new_value = 0;
            for (j = 0; j < n; j++) {
                new_value += buf[i-j];
            }

            buf[i] = new_value / n;
        }
    }
}

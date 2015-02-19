#include <math.h>

double point_sine(double point, double offset);
double point_sawtooth(double point, double offset);
double point_square(double point, double  offset);
double point_triangle(double point, double  offset);    
double point_rand();

void generate_sine(double *buf, int res);
void generate_square(double *buf, int res);
void generate_sawtooth(double *buf, int res);
void generate_triangle(double *buf, int res);
void generate_noise(double *buf, int res);

void low_pass_filter(double *buf, int buflen, int n);

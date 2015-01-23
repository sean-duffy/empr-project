#include <math.h>

//Waves
double point_sine(double point, double offset);
double point_sawtooth(double point, double offset);
double point_square(double point, double  offset);
double point_triangle(double point, double  offset);    

//Noise
double point_noise_white();
double point_noise_pink();

//Operators
double add_points(double p_a, double p_b, double scale_a, double scale_b);
double mult_points(double p_a, double p_b, double scale_a, double scale_b);

//Generators
void generate_sine(double *buf, int res);
void generate_square(double *buf, int res);
void generate_sawtooth(double *buf, int res);
void generate_triangle(double *buf, int res);
void generate_white_noise(double *buf, int res);

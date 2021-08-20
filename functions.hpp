#include <random>
#include <fftw3.h>

double *get_random_array(int n);
double *interpolate_array(double *data, uint32_t n, uint32_t factor);
fftwf_complex *apply_phase_shift(fftwf_complex *bins, uint32_t n, double*rand_array, uint32_t factor, uint32_t cursor, float width);

/** Upstream function for the widener so I can test the
 * functions it uses
 */
#include "functions.hpp"
#include <iostream>
#include <complex>

std::default_random_engine generator;

#define PI 3.14159265358979323846264338327950

/**
 * returns a random array of length n of double in the range (0,1)
 */
double *get_random_array(int n)
{
    double *rand_array = new double[n];

    // populate the array
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (uint32_t i = 0; i < n; i++)
    {
        rand_array[i] = distribution(generator);
    }
    return rand_array;
}

/**
 * Returns a double array of length n*factor with new entries linearly
 * interpolated from the initial data array
 * 
 * @param data an n long list of double to fill in the gaps of
 * @param n the length of the data array
 * @param factor 2 to double the length of the array, 3 to triple etc
 * 
 * @return a double list of length n*double
 */
double *interpolate_array(double *data, uint32_t n, uint32_t factor)
{
    // VARIABLE USAGE FOR n = 5, factor = 3
    // rand_arr[0] = 0.131538
    // rand_arr[1] = 0.458650
    // rand_arr[2] = 0.218959
    // rand_arr[3] = 0.678865
    // rand_arr[4] = 0.934693
    // i = 0   j=0     prev=0.131538   next=0.45865    rot=0   min=3   ans = 0.131538
    // i = 1   j=0     prev=0.131538   next=0.45865    rot=1   min=2   ans = 0.240575
    // i = 2   j=0     prev=0.131538   next=0.45865    rot=2   min=1   ans = 0.349613
    // i = 3   j=1     prev=0.45865    next=0.218959   rot=0   min=3   ans = 0.45865
    // i = 4   j=1     prev=0.45865    next=0.218959   rot=1   min=2   ans = 0.378753

    double *output = new double[n * factor];
    for (size_t i = 0; i < n * factor; i++)
    {
        int rot = i % factor;
        int min_rot = factor - rot;

        int j = i / factor;
        int k = (j + 1 >= n) ? 0 : j + 1;

        double prev_entry = data[j];
        double next_entry = data[k];
        double ans = (prev_entry * min_rot + next_entry * rot) / factor;
        output[i] = ans;
    }
    return output;
}

std::complex<double> convert_to_std_form(fftwf_complex num)
{
    double im_part = num[1];
    double re_part = num[0];
    using namespace std::complex_literals;
    {
        std::complex<double> y = re_part + im_part * 1i;
        return y;
    } // namespace std::complex_literals
}

/**
 * Applies phase shift to the frequency bins based on the random array of length
 * n passed in.
 * 
 * cursor
 *  |
 *  v
 *  |   |   |   |   |   |   |   |   |   |   |   |    <- bins
 * ||||||||||||||||||||||||||||||||||||||||||||||||| <- rand_array
 * 
 * @param bins the frequency bins
 * @param n the length of the bins array
 * @param rand_array a random array of length n*factor
 * @param factor how much longer the random array is than the bins
 * @param cursor an int in the range (0, (factor*n)-1) that indicates where in the cycle we are
 * @param width number in range (0, 1) of how much to change the phase of the signal
 * 
 * @return bins with rotation applied
 */
fftwf_complex *apply_phase_shift(fftwf_complex *bins, uint32_t n, double *rand_array, uint32_t factor, uint32_t cursor, float width)
{
    fftwf_complex *output = fftwf_alloc_complex(sizeof(fftwf_complex) * n);

    for (size_t i = 0; i < n; i++)
    {
        if (1 <= i && i <= n/2)
        {
        using namespace std;
        {
            complex<double> y = convert_to_std_form(bins[i]);
            uint rand_arr_index = (i * factor + cursor) % (n * factor);
            double angle = width * (rand_array[rand_arr_index] - 0.5) * 2 * PI; // convert (0, 1) -> (-pi, pi)

            complex<double> result;
            using namespace std::complex_literals;
            {
                result = y * std::exp(1i * angle);
            } // namespace complex_literals;

            output[i][0] = result.real();
            output[i][1] = result.imag();

        } // namespace std
        }
        else
        {
            output[i][0] = bins[i][0];
            output[i][1] = bins[i][1];
        }
    
        }
        

    return output;
}
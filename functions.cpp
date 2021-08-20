/** Upstream function for the widener so I can test the
 * functions it uses
 */
#include "functions.h"
#include <iostream>

std::default_random_engine generator;

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
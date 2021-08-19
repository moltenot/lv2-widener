/** Upstream function for the widener so I can test the
 * functions it uses
 */
#include "functions.h"

std::default_random_engine generator;

/**
 * maintains the datafield list of random numbers of length sample_num
 */
double *recompute_random_nums(int sample_num)
{
    double *rand_array = new double[sample_num];

    // populate the array
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (uint32_t i = 0; i < sample_num; i++)
    {
        rand_array[i] = distribution(generator);
    }
    return rand_array;
}

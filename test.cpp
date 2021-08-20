/**
 * File to test function.cpp methods
 */
#include <iostream>
#include "functions.hpp"
#include <fftw3.h>

double *rand_array;
uint32_t sample_num;

void test_recompute_rand_nums()
{
    sample_num = 20;
    rand_array = get_random_array(sample_num);
    for (int i = 0; i < sample_num; i++)
    {
        std::cout << "random_array[" << i << "] = " << rand_array[i] << std::endl;
    }
}

void test_interpolate_array()
{
    int init_arr_size = 5;
    int factor = 3;

    rand_array = get_random_array(init_arr_size);
    for (size_t i = 0; i < 5; i++)
    {
        printf("rand_arr[%d] = %f\n", i, rand_array[i]);
    }
    std::cout << "\n"
              << std::endl;

    double *new_arr = interpolate_array(rand_array, init_arr_size, factor);
    for (size_t i = 0; i < init_arr_size * factor; i++)
    {
        std::cout << "new_arr[i] = " << new_arr[i] << std::endl;
    }
}

fftwf_complex *get_test_data()
{
    fftwf_complex *test_in = fftwf_alloc_complex(sizeof(fftwf_complex) * 8);
    test_in[0][0] = 1.107466;
    test_in[0][1] = 0.000000;
    test_in[1][0] = 4.789294;
    test_in[1][1] = -4.194024;
    test_in[2][0] = -2.773369;
    test_in[2][1] = +1.831120;
    test_in[3][0] = -1.762084;
    test_in[3][1] = +0.560828;
    test_in[4][0] = -1.615148;
    test_in[4][1] = +0.000000;
    test_in[5][0] = -1.762084;
    test_in[5][1] = +-0.560828;
    test_in[6][0] = -2.773369;
    test_in[6][1] = +-1.831120;
    test_in[7][0] = 4.789294;
    test_in[7][1] = 4.194024;
    return test_in;
}

void test_phase_shift()
{
    // get frequency bins
    fftwf_complex *input = get_test_data(); // this has length 8
    int input_length = 8;
    int factor = 3;

    for (size_t i = 0; i < input_length; i++)
    {
        std::cout << "input[" << i << "][0] = " << input[i][0] << "\t\t";
        std::cout << "input[" << i << "][1] = " << input[i][1] << std::endl;
    }

    // get rand_array
    std::cout << std::endl;
    double *rand_array = get_random_array(input_length);
    for (size_t i = 0; i < input_length; i++)
    {
        std::cout << "rand_array[" << i << "] = " << rand_array[i] << std::endl;
    }

    // interpolate array
    std::cout << std::endl;
    double *long_arr = interpolate_array(rand_array, input_length, factor);
    for (size_t i = 0; i < input_length * factor; i++)
    {
        std::cout << "long_arr[" << i << "] = " << long_arr[i] << std::endl;
    }

    // run phase_shift
    fftwf_complex *output;
    output = apply_phase_shift(input, input_length, long_arr, factor, 22);
    for (size_t i = 0; i < input_length; i++)
    {
        std::cout << "output[" << i << "][0] = " << output[i][0] << "\t\t";
        std::cout << "output[" << i << "][1] = " << output[i][1] << std::endl;
    }

    fftwf_free(input);
}

int main(void)
{
    test_phase_shift();
}

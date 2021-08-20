/**
 * File to test function.cpp methods
 */
#include <iostream>
#include "functions.h"

double *rand_array;
uint32_t sample_num;

void test_recompute_rand_nums()
{
    sample_num = 20;
    rand_array = recompute_random_nums(sample_num);
    for (int i = 0; i < sample_num; i++)
    {
        std::cout << "random_array[" << i << "] = " << rand_array[i] << std::endl;
    }
}

void test_interpolate_array()
{
    int init_arr_size = 5;
    int factor = 3;

    rand_array = recompute_random_nums(init_arr_size);
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

int main(void)
{
    test_interpolate_array();
}

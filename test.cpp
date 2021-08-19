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

int main(void)
{
    test_recompute_rand_nums();
}

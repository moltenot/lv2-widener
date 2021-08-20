/**
 * Tilt EQ written in the LV2 format using the fftw library for freqency analysis
 * and lv2-c++tools to allow the LV2 specification to be written in c++
 * 
 * @author Theo
 * @date Jul 2021
 */

#include <iostream>
#include <lv2plugin.hpp>
#include <complex>
#include <fftw3.h>
#include <random>
#include "functions.h"

// have to make sure these are the same as in tilteq.ttl
#define WIDTH_PORT_INDEX 0
#define INPUT_PORT_INDEX 1
#define OUTPUT_PORT_INDEX 2

#define PI 3.14159265358979323846264338327950

using namespace LV2;

class TiltEQ : public Plugin<TiltEQ>
{

protected:
    bool has_printed = false;
    std::default_random_engine generator;
    uint32_t sample_num = 0; // represents the number of samples we are getting given at the moment
    double *rand_array;      // array of random doubles which will be created to be of length sample_num
    uint32_t factor = 20;    // how slow the phase gets changed
    uint cursor;

<<<<<<< HEAD
    /**
     * returns a complex number that when multiplied by another complex number rotates
     * it some amount relative to the tilt_value given
     */
    std::complex<double> get_random_angle_shift(float tilt_value) {
        std::uniform_real_distribution<double> distribution(-tilt_value, tilt_value);
        double random_number = distribution(generator);

        // turn the random number on the range [-TILT , TILT] control into something on range [-pi , pi]
        double angle = random_number * PI;

        std::complex<double> z;
        using namespace std::complex_literals;
        {
            z = std::exp(angle*1i);
        } // namespace std::complex_literals
        return z;
    }

    /**
     * Make the control less linear so there is more articulation 0.0
     * and less around 1. Do this by squaring
     * 
     * @param tilt the control before modification
     */
    void desensitise_tilt(float *tilt) 
    {
        *tilt = pow(*tilt, 8);
    }

    /**
     * apply a random phase shift to the frequency bins
     * 
     * @param freq_bins the bins output from the forward fft
     * @param length the length of the freq_bin array
     */
    void apply_random_phase_shift(fftwf_complex *freq_bins, uint32_t length) {
        float tilt_value = *p(WIDTH_PORT_INDEX);
        desensitise_tilt(&tilt_value);
        

        for(uint32_t i = 0; i < length; i++) {
            std::complex<double> z = get_random_angle_shift(tilt_value); // complex number with mod 1 and arg theta
            std::complex<double> y; // complex number to be assigned the value of the frequency bin

            double real_part = freq_bins[i][0];
            double im_part = freq_bins[i][1];
            using namespace std::complex_literals;
            {
                y = real_part + im_part*1i;
            } // namespace std::complex_literals

            // compute the angle change and put it back
            std::complex<double> result = z*y;
            freq_bins[i][0] = (float) result.real();
            freq_bins[i][1] = (float) result.imag();
        }
    }
=======
>>>>>>> bf77d9e (removed old methods)

    /**
     * prints the numbers in a float array in 6 columns
     * 
     * @param data the data to print
     * @param n the length of the data
     */
    void print_nums(float *data, uint32_t n)
    {
        int num_cols = 6;
        for (uint32_t i = 0; i < n; i++)
        {
            printf("%f\t", data[i]);
            if (i % num_cols == 0)
            {
                printf("\n");
            }
        }
    }

    /**
     * prints an fftwf_complex array to stdout in 3 columns
     * 
     * @param data to print
     * @param n length of the data
     */
    void print_fft_complex(fftwf_complex *data, int n)
    {
        int num_cols = 3;

        for (int i = 0; i < n; i++)
        {
            printf("%lf + %lf i\t", data[i][0], data[i][1]);
            if (i % num_cols == 0)
            {
                printf("\n");
            }
        }
    }

    /**
     * Compute the forward fft going from the time domain to the frequency domain.
     * 
     * @param port pointer to the port buffer
     * @param n size of the buffer
     * 
     * @returns a pointer of type fftwf_complex of the same length as n
     */
    fftwf_complex *fft_forward(float *port, uint32_t n)
    {
        /* compute the forward fft of the input port of size n */

        fftwf_complex *out = fftwf_alloc_complex(sizeof(fftwf_complex) * n);

        int tmp = (int)n;
        const int *tmpptr = &tmp;

        // create a plan
        //TODO don't create the plan each time, although this depends on
        //      sample numbers change a lot
        fftwf_plan plan = fftwf_plan_dft_r2c(1, tmpptr, port,
                                             out, FFTW_ESTIMATE);
        fftwf_execute(plan);
        fftwf_destroy_plan(plan);

        // print_fft_complex(out, n);
        return out;
    }

    /**
     * Compute the backward fft going from the freqency domain to the time domain
     * 
     * @param freq_bins the freqency bins to transform
     * @param n then number of frequency bins
     * 
     * @returns a pointer to a buffer of length n
     */
    float *fft_backward(fftwf_complex *freq_bins, uint32_t n)
    {
        float *out = (float *)malloc(sizeof(float) * n);

        // convrt int to ptr
        int tmp = (int)n;
        const int *tmpptr = &tmp;

        fftwf_plan plan = fftwf_plan_dft_c2r(1, tmpptr, freq_bins, out, FFTW_ESTIMATE);
        fftwf_execute(plan);
        fftwf_destroy_plan(plan);
        return out;
    }

    /**
     * divides each element of the frequencies parameter by n. This is because fftw
     * does an unnormalized fft so this step is necessary if the backward fft is to
     * have the same ampltiude as the input data
     * 
     * @param frequencies the frequency bins
     * @param n the number of frequency bins
     */
    void normalize_amplitude(fftwf_complex *frequencies, uint32_t n)
    {
        /* divide each element of freqencies by n (in amplitude) */
        for (int i = 0; i < n; i++)
        {
            frequencies[i][0] = frequencies[i][0] / n;
            frequencies[i][1] = frequencies[i][1] / n;
        }
    }

public:
    TiltEQ(double rate)
        : Plugin<TiltEQ>(3) // since there are 3 ports in this plugin
    {
        printf("created tilteq with rate %f\n", rate);
    }

    void activate()
    {
        cursor = 0;
    }

    /**
     * runs the plugin on a chunk of data. currently it computes the forward fft,
     * then reverts it back to the time domain without doing anything, and writes
     * that to the output port.
     */
    void run(uint32_t sample_count)
    {
        if (sample_count != TiltEQ::sample_num)
        {
            sample_num = sample_count;
            rand_array = get_random_array(sample_count);
            rand_array = interpolate_array(rand_array, sample_count, factor);
        }

        fftwf_complex *freq_bins = fft_forward(p(INPUT_PORT_INDEX), sample_count);

        apply_phase_shift(freq_bins, sample_count, rand_array, factor, cursor);
        cursor++;
        cursor = cursor % sample_count;
        normalize_amplitude(freq_bins, sample_count);

        float *processed_signal = fft_backward(freq_bins, sample_count);

        // write to the output
        for (uint32_t i = 0; i < sample_count; ++i)
        {
            p(OUTPUT_PORT_INDEX)[i] = processed_signal[i];
        }

        fftwf_free(freq_bins);
    }
};

unsigned _ = TiltEQ::register_class("http://www.mystery_plugin.co.nz/");
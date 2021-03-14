#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

void dft(double* data, double* amp, int N) 
{
	double real = 0;
	double imag = 0;
	double sum_sqs = 0;
	double phase[N];

	
	//printf("DFT Results:\n");

	for (int k = 0; k<N; ++k) 
	{
		for (int n = 0; n<N; ++n)
		{
			double angle = (2*M_PI*k*n)/N;

			real += data[n] * cos(angle);
			imag += data[n] * sin(angle);
		}

		sum_sqs = real*real + imag*imag;

		//printf("%f\n", real);
		//printf("%f\n", imag);

		amp[k] = sqrt(sum_sqs);
		phase[k] = atan(imag/real);

		real = 0; imag = 0;
	}
	printf("\n");
}

void fft(double* data, int N)
{
	int order[N];
	int stages = (N / 8) + 2;
	int s = 0;

	double sorted_data[N];
	double sd_real[N];
	double sd_imag[N];
	double top;
	double bottom;
	double angle;
	double top_r;
	double top_i;
	double bottom_r;
	double bottom_i;
	double imag;
	double real;
	double bottom_r2;
	double bottom_i2;

	int rev = 0;

	// rearrange array based on bit reversed order concept
	for (int n = 0; n<N; ++n)
	{
		rev = reverse_bits(n, N);
		order[n] = rev;
	}

	/*
	for (int n = 0; n<N; ++n)
	{
		printf("%d\n", order[n]);
	}
	*/

	printf("ReOrdered Data:\n");
	for (int n = 0; n<N; ++n)
	{
		sorted_data[n] = data[order[n]];
		printf("%f\n", sorted_data[n]);
	}
	printf("\n");


	while (s < (stages-1))
	{
		// perform first stage butterfly
		if (s == 0)
		{
			// calculate 2 sample DFTs (first stage butterfly)
			for (int n=0; n<N; ++n)
			{
				if ((n%2 == 0))
				{
					top = sorted_data[n];
					bottom = sorted_data[n+1];
		
					sorted_data[n] = top + bottom;
					sorted_data[n+1] = top - bottom;
				}
			}

			s += 1;

			/*
			for (int n=0; n<N; ++n)
			{
				printf("%f\n", sorted_data[n]);
			}
			*/
		}
		
		// perform intermediate stage butterflies 
		else
		{
			int index = 0;
			int max_index = s * 4;
			int inc = ((N / 8) * 2) * s;
			int cycle = 0;
			int cycle_total = inc;
			int val = 0;
			int exp[N];
			int i = 0;

			while (1)
			{

				if (cycle == cycle_total)
				{
					// compute the next stage and update the data array
					int n = 0;
					int skip = 0;
					while (n < N)
					{
						top = sorted_data[n];
						bottom = sorted_data[n+inc];

						//printf("%d\n", exp[n]);
						angle = 2*M_PI*exp[n]/N;
						bottom_r = bottom * cos(angle);
						bottom_i = -1 * bottom * sin(angle);
						
						sd_real[n] = top + bottom_r;	
						sd_imag[n] = bottom_i;
						sd_real[n+inc] = top - bottom_r;
						sd_imag[n+inc] = -1 * bottom_i;

						skip += 1;

						if (skip == inc) 
						{ 
							n += (inc+1); 
							skip = 0;
						}
						else { n += 1; }
					}

					break;
				}

				exp[i] = val;
				i += 1;
				val += inc;
				index += 1;
			
				if (index == max_index)
				{
					val = order[max_index * (cycle+1)];

					cycle += 1;
					index = 0;
				}
				
			}

			s += 1;

			/*
			printf("\n");
			for (int n=0; n<N; ++n)
			{
				printf("%f\n", sd_real[n]);	
				printf("%f\n", sd_imag[n]);	
			}
			*/

		}
		
		if (s == stages-1)
		{
			int inc = ((N / 8) * 2) * s;
			for (int n=0; n<(N-inc); ++n)
			{
				top_r = sd_real[n];
				top_i = sd_imag[n];
				bottom_r = sd_real[n+inc];
				bottom_i = sd_imag[n+inc];
				
				angle = 2*M_PI*n/N;
				imag = -1 * sin(angle);
				real = cos(angle);

				bottom_r2 = (bottom_r * real) - (bottom_i * imag);
				bottom_i2 = (bottom_r * imag) + (bottom_i * real);

				sd_real[n] = top_r + bottom_r2;
				sd_imag[n] = top_i + bottom_i2;
				sd_real[n+inc] = top_r - bottom_r2;
				sd_imag[n+inc] = top_i - bottom_i2; 
			}

			for (int n=0; n<N; ++n)
			{
				printf("%f\n", sd_real[n]);	
				printf("%f\n", sd_imag[n]);	
			}

		}
	}

	/*
	printf("FFT Results:\n");

	for (int n=0; n<N; ++n) 
	{
		printf("%f\n", sd_real[n]);
		printf("%f\n", sd_imag[n]);
	}
	*/

	// perform second stage butterfly 
	// perform third stage butterfly
	
}

int reverse_bits(int num, int N)
{
	/*
	return bit reversed order
	*/

	int rev = 0;
	int count = 0;
	int max_count = (N / 8) + 2;

	while (count < max_count)
	{
		rev <<= 1;			// bitshift once to the left
		if (num & 1 == 1)	// if LSB is 1
		{
			rev ^= 1;		// set LSB to 1
		}
		num >>= 1;			// bitshift once to the right
		count += 1;
	}

	return rev;
}

void wave_gen(double *data, int N)
{
	double freq = 1.0;		// cycles per second
	double s_freq = 8.0;	// sample per second
	double time = 1.0;		// seconds

	double w = 2*M_PI*(freq/s_freq);	// angular frequency in sample space

	// "acquire" the data
	printf("Acquired Data:\n");
	for (int n = 0; n<N; ++n)
	{
		data[n] = sin(w*time*n);	
		printf("%f\n", data[n]);
	}
	printf("\n");
}


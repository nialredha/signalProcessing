#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "radix2.h"

int NUMBER_OF_STAGES = 0;

void dft(double* data, double* amp, int N) 
{
	double real = 0;
	double imag = 0;
	double sum_sqs = 0;
	double phase[N];
	
	for (int k = 0; k<N; ++k) 
	{
		for (int n = 0; n<N; ++n)
		{
			double angle = (2*M_PI*k*n)/N;

			real += data[n] * cos(angle);
			imag += data[n] * sin(angle);
		}

/*
		sum_sqs = real*real + imag*imag;

		amp[k] = sqrt(sum_sqs);
		phase[k] = atan(imag/real);
*/
		real = 0; imag = 0;
	}

	/*
	printf("\nDFT Results:\n");
	for (int i = 0; i<N; ++i) 
	{
		printf("%f\n", amp[i]);
	}
	printf("\n");
	*/
	
}

void fft(double* data, int N)
{
	int order[N];
	double sorted_data[N];
	double sorted_data_i[N];
	int rev = 0;
	int log_N = N;

	while (log_N > 0)
	{
		log_N >>= 1;
		if (log_N != 0)
		{
			NUMBER_OF_STAGES += 1;
		}
	}

	for (int n=0; n<N; ++n)
	{
		order[n] = 0;
		sorted_data[n] = 0;
		sorted_data_i[n] = 0;
	}

	for (int n = 0; n<N; ++n)
	{
		rev = reverse_bits(n, N);
		order[n] = rev;
	}

	for (int n = 0; n<N; ++n)
	{
		sorted_data[n] = data[order[n]];
	}

	// INTIALIZATION 
	int n = 0;
	int inc = 1;
	int k = 0;				// index for frequency data 
	int k_step = 0;
	int count = 0;

	double const_exp = 2.0*M_PI/N;

	for (int s=0; s<NUMBER_OF_STAGES; ++s)
	{
		// THE MAIN BUTTERFLY LOOP
		//printf("Stage: %d\n", s);

		while (n<N)
		{
			// top + (bottom * W^k)
			// top - (bottom * W^k)
			//printf("%d\n", k);

			
			double top = sorted_data[n];
			double top_i = sorted_data_i[n];
			double bottom = sorted_data[n+inc] * cos(const_exp*k) + 
							sorted_data_i[n+inc] * sin(const_exp*k);
			double bottom_i = sorted_data_i[n+inc] * cos(const_exp*k) - 
							  sorted_data[n+inc] * sin(const_exp*k);
/*
			if (s==1)
			{
				//printf("real = %f\n", cos(const_exp*2));
				printf("inc = %d\n", n+inc);
				printf("bot = %lf\n", bottom);
				printf("bot_i = %lf\n", bottom_i);
				printf("k = %d\n", k);
			}				  
			printf("\n");
			*/

			sorted_data[n] = top + bottom;
			sorted_data[n+inc] = top - bottom; 
			sorted_data_i[n] = top_i + bottom_i;
			sorted_data_i[n+inc] = top_i - bottom_i;

			k += k_step; 
			count += 1;

			if (count == inc)
			{
				n += (inc +1);
				count = 0;
				k = 0;
			}
			else 
			{
				n+=1;
			}
		}

		inc = inc * 2; // this signifies the end of a stage 
		k_step = N/(2*inc);
		n = 0;
/*
		if (s==0)
		{
			for (int i=0; i<N; ++i)
			{
				printf("%lf\n", sorted_data[i]);
			}
			printf("\n");
			for (int i=0; i<N; ++i)
			{
				printf("%lf\n", sorted_data_i[i]);
			}
			printf("\n");
		}
		*/
	}

	/*
	for (int n=0; n<N; ++n)
	{
		double temp = sorted_data[n]*sorted_data[n] + 
					  sorted_data_i[n]*sorted_data_i[n];
		temp = sqrt(temp);
		printf("%f\n", temp);
	}
	*/

	FILE *file;

	file = fopen("../data/FFT_data.csv", "w+");
	fprintf(file, "Time, Raw Data, Transformed Data\n");

	for (int n=0; n<N; ++n)
	{
		double temp = sorted_data[n]*sorted_data[n] +
					  sorted_data_i[n]*sorted_data_i[n];
		temp = sqrt(temp);

		fprintf(file, "%d, %lf, %lf\n", n, data[n], temp);
	}
}

int reverse_bits(int num, int N)
{
	/*
	return bit reversed order
	*/

	int rev = 0;
	int count = 0;
	int max_count = NUMBER_OF_STAGES;

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
	double freq = 8.0;		// cycles per second
	int s_freq = N;
	double time = 1.0;		// seconds

	double w = 2*M_PI*(freq/s_freq);	// angular frequency in sample space

	// "acquire" the data
	//printf("Acquired Data:\n");
	for (int n = 0; n<N; ++n)
	{
		data[n] = sin(w*time*n) + 0.5*sin(w*time*n/2);	
		//printf("%f\n", data[n]);
	}
	//printf("\n");
}

/*
void main() 
{
	int N = 32768;	// radix-2 algorithm requires N be a power of 2
	int log_N = N;

	clock_t start, stop;
	double cpu_time_used;

	while (log_N > 0)
	{
		log_N >>= 1;
		if (log_N != 0)
		{
			NUMBER_OF_STAGES += 1;
		}
	}

	double data[N]; 
	double amp[N];

	wave_gen(data, N);

	start = clock();
	fft(data, N);
	stop = clock();
	cpu_time_used = ((double) (stop - start)) / CLOCKS_PER_SEC;
	printf("FFT Computation Time: %f\n", cpu_time_used);

	start = clock();
	dft(data, amp, N);
	stop = clock();
	cpu_time_used = ((double) (stop - start)) /	CLOCKS_PER_SEC;
	printf("DFT Computation Time: %f\n", cpu_time_used);
}
*/

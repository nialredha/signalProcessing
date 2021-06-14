#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "radix2.h"

void main() 
{
	int N = 64;	// radix-2 algorithm requires N be a power of 2

	// determine stages necessary
	int log_N = N;
	int stages = 0;

	while (log_N > 0)
	{
		log_N >>= 1;
		//printf("%d\n", log_N);

		if (log_N != 0)
		{
			stages += 1;
		}
	}

	//printf("%d\n", stages);

	double data[N]; 
	double amp[N];

	clock_t start, stop;
	double cpu_time_used;

	wave_gen(data, N);

	start = clock();
	fft(data, N, stages);
	stop = clock();
	cpu_time_used = ((double) (stop - start)) / CLOCKS_PER_SEC;
	printf("FFT Computation Time: %f\n", cpu_time_used);

	start = clock();
	dft(data, amp, N);
	stop = clock();
	cpu_time_used = ((double) (stop - start)) /	CLOCKS_PER_SEC;
	printf("DFT Computation Time: %f\n", cpu_time_used);
}

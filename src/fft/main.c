#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "radix2.h"

void main() 
{
	int N = 32;	// radix-2 algorithm requires N be a power of 2

	double data[N]; 
	double amp[N];

	clock_t start, stop;
	double cpu_time_used;

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

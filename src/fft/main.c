#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "radix2.h"

void main() 
{
	int N = 16;	// radix-2 algorithm requires N be a power of 2

	double data[N]; 
	double amp[N];

	wave_gen(data, N);
	fft(data, N);
	dft(data, amp, N);
}

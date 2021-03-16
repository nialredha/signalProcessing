#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

void main() 
{
	int N = 16;	// radix-2 algorithm requires N be a power of 2
	//int num = 1;

	double data[N]; 
	double amp[N];

	wave_gen(data, N);
	fft(data, N);
	dft(data, amp, N);

	/*
	for (int n=0; n<N; ++n)
	{
		num = n;
		int rev = reverse_bits(num, N);
		printf("%d\n", rev);
	}
	*/
	
	

	
}

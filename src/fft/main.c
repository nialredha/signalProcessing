#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "wav.h"
#include "radix2.h"

unsigned char fbb[4];	// 4 byte buffer
unsigned char tbb[2];	// 2 byte buffer

int main(int argc, char **argv)
{
	char *filename = (char*)malloc(sizeof(char)*1024);

	// FIND FILE PATH FOR WAV FILE SPECIFIED 
	if (filename == NULL)
	{
		printf("ERROR in malloc\n");
		exit(1); // indicating abnormal termination of program
	}

	// get file path
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		strcpy(filename, cwd);
		
		// get filename from command line
		if (argc < 2)
		{
			printf("Please Specify WAV file\n");
			return 0;
		}

		// add filename to the dir
		strcat(filename, "/");
		strcat(filename, argv[1]);
		printf("Input Signal: \n%s\n", filename);
	}

	FILE *file = open_wav(filename);
	int *ch_data = read_wav(file);
	close_wav(file);
	

	int N = 262144;	// radix-2 algorithm requires N be a power of 2
	double data[N]; 

	clock_t start, stop;
	double cpu_time_used;

	wave_gen(data, N);

	start = clock();
	fft(data, N);
	stop = clock();
	cpu_time_used = ((double) (stop - start)) / CLOCKS_PER_SEC;
	printf("FFT Computation Time: %f\n", cpu_time_used);

	/*
	start = clock();
	dft(data, amp, N);
	stop = clock();
	cpu_time_used = ((double) (stop - start)) /	CLOCKS_PER_SEC;
	printf("DFT Computation Time: %f\n", cpu_time_used);
*/
}

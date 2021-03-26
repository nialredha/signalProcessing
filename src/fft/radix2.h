#ifndef RADIX2_H_	// include guard
#define RADIX2_H_

// function declarations 
void	dft(double* data, double* amp, int N);	// discrete fourier transform
void	fft(double* data, int N, int stages);	// radix-2 algorithm 
int		reverse_bits(int num, int N);			// bit reversed order calc
void	wave_gen(double *data, int N);			// wave generation for testing

#endif // RADIX2_H_

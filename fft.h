#ifndef FFT_H_	// include guard
#define FFT_H_

// function declarations 
void	dft(double* data, double* amp, int N);	// discrete fourier transform
void	fft(double* data, int N);				// radix-2 algorithm 
int		reverse_bits(int num, int N);			// bit reversed order calc
void	wave_gen(double *data, int N);	// wave generation for testing

#endif // FFT_H_

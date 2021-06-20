// Floating Point Addition and Multiplication

// References: 
// https://www.doc.ic.ac.uk/~eedwards/compsys/float/
// https://github.com/geohot/twitchcore/blob/master/tf32.c

#include <stdio.h>
#include <stdint.h>

#define S(x) (x>>31) // MSB is the sign bit
#define E(x) ((x>>23) & ((1<<8) - 1)) // Next 8 bits is ecponent 
#define M(x) (x & ((1<<23) - 1)) // Next 23 bits is mantissa 

void add(int fa, int fb)
{
	if (E(fa) <= E(fb))
	{
	}
}

int multiply(int fa, int fb) 
{
	int new_sign = S(fa) ^ S(fb); // XOR: if one is negative the product is
	int new_exponent = E(fa) + E(fb) - 127; // add bias twice subtract once
	int new_mantissa = ((uint64_t)((1<<23)|M(fa))) * 
					   ((uint64_t)((1<<23)|M(fb))) >> 23;

	return (new_sign<<31) | 
		   ((new_exponent&((1<<8) - 1)) << 23) |
		   (new_mantissa&((1<<23) - 1));
}

void float_point_rep(int x) 
{
	printf("    Sign: %d\n", S(x));
	printf("Exponent: %d\n", E(x));
	printf("Mantissa: %d\n", M(x));
	printf("\n");
}

void main() 
{
	float a = 1.3;
	float b = 4.5;
	float c = a*b;

	int fa = *(uint32_t*)(&a);
	int fb = *(uint32_t*)(&b); 
	int fc = *(uint32_t*)(&c);

	printf("\n");
	printf("%f * %f = %f\n", a, b, c);
	printf("%x * %x = %x\n", fa, fb, fc);
	printf("\n");

	float_point_rep(fa);
	float_point_rep(fb);
	float_point_rep(fc);

	int result = multiply(fa, fb);
	float_point_rep(result);

	printf("\n");
	printf("%f * %f = %x %f\n", a, b, result, *((float*)&result));
}

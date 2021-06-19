/******************************************************************
*Floating Point Addition, Subtraction, Multiplication, and Division
******************************************************************/

#include <stdio.h>
#include <stdint.h>

#define S(x) (x>>31) // MSB is the sign bit
#define E(x) (x>>23) & ((1<<8) - 1) // Next 8 bits is ecponent 
#define M(x) (x & ((1<<23) - 1)) // Next 23 bits is mantissa 


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

	printf("%f * %f = %f\n", a, b, c);
	printf("%x * %x = %x\n", fa, fb, fc);

	float_point_rep(fa);
	float_point_rep(fb);
	float_point_rep(fc);
}

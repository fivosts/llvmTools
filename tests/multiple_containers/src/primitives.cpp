#include "primitives.h"

int get_int(){

	int a = 2;
	a = a + 2;
	return a;
}

unsigned get_unsigned(){

	unsigned a = 2;
	a = a + 2;
	return a;
}

float get_float(){

	float a = 2.412;
	return a;
}

char get_char(){

	char a = 'a';
	return a;
}

long get_long(){

	long a = 22;
	return a;
}

int *get_int_ptr(){

	int* a = new int;
	*a = 23;
	return a;
}

float *get_float_ptr(){

	float* a = new float;
	*a = 2.28;
	return a;
}

char *get_char_ptr(){

	char* a = new char;
	*a = 'b';
	return a;
}

long *get_long_ptr(){

	long* a = new long;
	*a = 22;
	return a;
}
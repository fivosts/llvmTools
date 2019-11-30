#include <stdio.h>

void *test_void(){


	void *a;
	return a;
}

int main(){

	int a = 3;
	int b = 7;
	int c = a + b;

	void *k;
	k = (void*)test_void();
	printf("Addition of 3 and 7 is: %d\n", c);

	return 0;
}


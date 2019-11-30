#include <stdio.h>
#include "stdlib.h"

unsigned int a = 0;

void bar(){
	printf("In bar\n");
	return;
}

int foo(){
	bar();
	printf("In foo\n");
	return 2;
}

int main(){

	char **string_array = (char**)malloc(10*sizeof(char*));
	string_array[3] = "Hello";

	a = a + 5;

	if (a == 6){
		bar();
	}else{
		foo();
	}

	printf("%s\n", string_array[3]);	

	return 0;
}
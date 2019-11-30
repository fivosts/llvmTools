#include <iostream>
#include "primitives.h"
#include "containers.h"

int main(){

	myclass obj = get_myclass();
	int a = get_int();	
	float b = get_float();	
	char c = get_char();	
	long d = get_long();	
	std::string str = get_string();

	myclass ccla = get_myclass();

	return 24;

}
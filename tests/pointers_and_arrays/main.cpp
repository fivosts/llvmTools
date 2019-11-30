#include <iostream>
#include <string>

int **int_ptr(){
	int **a = new int*;
	
	*a = new int;
	return a;
}

void foo(){
	return;
}

int arr_int(int a[10], int b[]){

	return 0;
}

std::string *str_test(){

	std::string *a = new std::string;

	return a;
}

int main(){

	std::string *t = new std::string;

	t = str_test();

	return 0;
}
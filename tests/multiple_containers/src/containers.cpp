#include "containers.h"

std::string get_string(){
	std::string a = "Hi";

	before_return:
	return a;
}

// std::vector<int> get_vector(){

// 	std::vector<int> a = {1, 2, 3, 4, 5};

// 	before_return:
// 	return a;
// }

std::string *get_string_ptr(){
	std::string *a = new std::string;
	*a = "Hi";

	before_return:
	return a;
}

// std::vector<int> *get_vector_ptr(){

// 	std::vector<int> *a = new std::vector<int>;
// 	*a = {1, 2, 3, 4, 5};

// 	before_return:
// 	return a;
// }

myclass get_myclass(){

	myclass test_object;

	before_return:
	return test_object;
}

myclass *get_myclass_ptr(){

	myclass *test_object = new myclass;

	before_return:
	return test_object;
}
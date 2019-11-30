#include <iostream>
#include <string>
// #include <vector>

// std::vector<int> my_vector = {31, 33, 35, 37, 39, 41};

void foo(std::string &test_string){
	(void) test_string;
	return;
}

int main(){

	std::string foivos = "Ela malaka";
	std::string kostas = "Arkintia";
	std::string nikos = "Ti paparies les tora ???";

	foo(foivos);
	foo(kostas);
	foo(nikos);

	return 24;

}

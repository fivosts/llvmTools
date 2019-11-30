#include <vector>
#include <string>

class myclass{

public:
	myclass(){}
	~myclass(){}

private:
	int priv_member;
};

std::string get_string();
// std::vector<int> get_vector();
std::string *get_string_ptr();
// std::vector<int> *get_vector_ptr();
myclass get_myclass();
myclass *get_myclass_ptr();


#include <iostream>
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

struct A{
	int k;
};

class Annie{
public:
	virtual void test(){}
};

class asd : public Annie {
public:
	int ena;
	int dio;
	int tria;
	void test() override{}
};


class empty : public Annie{
	int tessera;
	void test() override{}
};


int main(){

	Annie *a = new asd();

	a->test();
	return 24;

}

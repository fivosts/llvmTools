ir:
	clang++ -std=c++11 -O0 -Xclang -disable-O0-optnone -emit-llvm ./src/main.cpp -S -o ./IR/main.ll
	llvm-link ./IR/* -o class_inheritance.bc
	llvm-dis class_inheritance.bc
	rm class_inheritance.bc

exec:
	clang++ -std=c++11 ./src/*.cpp -o class_inheritance.out

dis:
	llvm-dis opt-class_inheritance.bc
	rm opt-class_inheritance.bc

opt:
	clang++ -std=c++11 -x ir ./opt-class_inheritance.ll ../../Function_call_recorder/external_lib/external_lib.ll -o opt-class_inheritance.out

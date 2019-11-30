ir:
	clang++ -std=c++11 -emit-llvm ./src/main.cpp -S -o ./IR/main.ll
	llvm-link ./IR/* -o one_container.bc
	llvm-dis one_container.bc
	rm one_container.bc

exec:
	clang++ -std=c++11 ./src/*.cpp -o one_container.out

dis:
	llvm-dis opt-one_container.bc
	rm opt-one_container.bc

opt:
	clang++ -std=c++11 -x ir ./opt-one_container.ll ../../Function_call_recorder/external_lib/external_lib.ll -o opt-one_container.out


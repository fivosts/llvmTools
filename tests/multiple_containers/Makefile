ir:
	clang++ -std=c++11 -emit-llvm ./src/containers.cpp -S -o ./IR/containers.ll
	clang++ -std=c++11 -emit-llvm ./src/primitives.cpp -S -o ./IR/primitives.ll
	clang++ -std=c++11 -emit-llvm ./src/main.cpp -S -o ./IR/main.ll
	llvm-link ./IR/* -o multiple_containers.bc
	llvm-dis multiple_containers.bc
	rm multiple_containers.bc

exec:
	clang++ -std=c++11 ./src/*.cpp -o multiple_containers.out

dis:
	llvm-dis opt-multiple_containers.bc
	rm opt-multiple_containers.bc

opt:
	clang++ -std=c++11 -x ir ./opt-multiple_containers.ll ../../Function_call_recorder/external_lib/external_lib.ll -o opt-multiple_containers.out


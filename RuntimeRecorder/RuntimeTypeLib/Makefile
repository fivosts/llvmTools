CXX_FLAGS= -Wall -Wextra -std=c++11

ir:
	clang++ $(CXX_FLAGS) -emit-llvm valueRecorder.cpp -S -o valueRecorder.ll
	clang++ $(CXX_FLAGS) -emit-llvm -c valueRecorder.cpp -o valueRecorder.bc

rir:
	clang++ $(CXX_FLAGS) -DPREDUCE -emit-llvm valueRecorder.cpp -S -o valueRecorder.ll
	clang++ $(CXX_FLAGS) -DPREDUCE -emit-llvm -c valueRecorder.cpp -o valueRecorder.bc	

test:
	clang++ $(CXX_FLAGS) -DPREDUCE valueRecorder.cpp test_main.cpp -o external_test.out

g++:
	g++ $(CXX_FLAGS) valueRecorder.cpp test_main.cpp -o g_external_test.out

clean:
	rm external_test.out valueRecorder.ll g_external_test.out
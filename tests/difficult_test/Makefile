ir:
	clang++ -std=c++11 -emit-llvm ./src/api.cpp -S -o ./IR/api.ll
	clang++ -std=c++11 -emit-llvm ./src/db_communicator.cpp -S -o ./IR/db_communicator.ll
	clang++ -std=c++11 -emit-llvm ./src/game.cpp -S -o ./IR/game.ll
	clang++ -std=c++11 -emit-llvm ./src/main.cpp -S -o ./IR/main.ll
	clang++ -std=c++11 -emit-llvm ./src/poker_dealer.cpp -S -o ./IR/poker_dealer.ll
	clang++ -std=c++11 -emit-llvm ./src/poker_player.cpp -S -o ./IR/poker_player.ll
	llvm-link ./IR/* -o poker_sumulator.bc
	llvm-dis poker_simulator.bc
	rm poker_simulator.bc

exec:
	clang++ -std=c++11 -lsqlite3 ./src/*.cpp -o poker_simulator.out

dis:
	llvm-dis opt-poker_simulator.bc
	rm opt-poker_simulator.bc

opt:
	clang++ -std=c++11 -lsqlite3 -x ir ./opt-poker_simulator.ll ../../Function_call_recorder/external_lib/external_lib.ll -o opt-poker_simulator.out


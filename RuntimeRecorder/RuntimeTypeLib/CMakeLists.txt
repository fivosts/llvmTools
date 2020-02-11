add_custom_command(OUTPUT RuntimeTypeLib.bc
                   COMMAND clang++ -emit-llvm valueRecorder.cpp -c -o valueRecorder.bc)

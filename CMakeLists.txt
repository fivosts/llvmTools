cmake_minimum_required(VERSION 3.4.3)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -fno-rtti")

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_subdirectory(hello)
add_subdirectory(BasicTechniques)
add_subdirectory(RuntimeRecorder)
add_subdirectory(SCC_skeleton)

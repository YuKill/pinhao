cmake_minimum_required (VERSION 2.8.8)

project (pinhao0.1)

#set (CMAKE_VERBOSE_MAKEFILE ON)

include_directories ("${PROJECT_SOURCE_DIR}/include")

execute_process (COMMAND llvm-config --cxxflags --ldflags 
  OUTPUT_VARIABLE LLVM_CONFIG_N)

string (REPLACE "\n" " " LLVM_CONFIG "${LLVM_CONFIG_N}")

set (CMAKE_CXX_FLAGS "-O0 ${LLVM_CONFIG}")

add_subdirectory (lib)
add_subdirectory (tests)

cmake_minimum_required (VERSION 2.8.8)

project (pinhao0.1)

#set (CMAKE_VERBOSE_MAKEFILE ON)

include_directories ("${PROJECT_SOURCE_DIR}/include")

execute_process (COMMAND llvm-config --cxxflags --ldflags 
  OUTPUT_VARIABLE LLVM_CONFIG_N)

string (REPLACE "\n" " " LLVM_CONFIG_EXCEPTION "${LLVM_CONFIG_N}")
string (REPLACE "-fno-exceptions" "" LLVM_CONFIG "${LLVM_CONFIG_EXCEPTION}")

set (CMAKE_CXX_FLAGS "-O0 -g -Wall ${LLVM_CONFIG} -Wl,-rpath,/usr/local/lib")

add_subdirectory (lib)
add_subdirectory (tools)

# Test configuration
option(test "Compiles the tests" OFF)

if (test)
  enable_testing()
  add_subdirectory (tests)
endif()

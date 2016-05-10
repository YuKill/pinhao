## Setting the compiler flags with the command `llvm-config`, with options:
### - cxxflags
### - ldflags
##
## The other libs, and system flags are used for each compiling target.
## Some flags are removed in order to the `llvm` and the `yaml-cpp` be compatible:
### - pedantic
### - fno-exceptions
##

execute_process (COMMAND llvm-config --cxxflags --ldflags 
  OUTPUT_VARIABLE LLVM_CONFIG_N)

string (REPLACE "\n" " " LLVM_CONFIG_EXCEPTION "${LLVM_CONFIG_N}")
string (REPLACE "-fno-exceptions" "" LLVM_CONFIG_PED "${LLVM_CONFIG_EXCEPTION}")
string (REPLACE "-pedantic" "" LLVM_CONFIG "${LLVM_CONFIG_PED}")

set (CMAKE_CXX_FLAGS "-O0 -g -Wall ${LLVM_CONFIG} -Wl,-rpath,/usr/local/lib")


cmake_minimum_required (VERSION 2.8.8)

project (pinhao0.1)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include (LinkModule)
include (CompilerFlags)

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_subdirectory (lib)
add_subdirectory (tools)

# Test configuration
option(test "Compiles the tests" OFF)

if (test)
  enable_testing()
  add_subdirectory (tests)
endif()

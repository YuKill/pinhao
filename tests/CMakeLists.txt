
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(RunTests MainTest.cpp)

add_test(UnitTests RunTests)

target_link_libraries(RunTests
  gtest gtest_main pthread)

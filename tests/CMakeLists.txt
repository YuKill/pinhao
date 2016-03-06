
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(RunTests 
  MainTest.cpp
  FeatureInfoTest.cpp)

add_test(UnitTests RunTests)

target_link_libraries(RunTests
  Features CFGStaticFeatures
  gtest gtest_main pthread)

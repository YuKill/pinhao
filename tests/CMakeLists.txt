
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(RunFeatureInfoTest 
  MainTest.cpp
  FeatureInfoTest.cpp
  $<TARGET_OBJECTS:Features>)
add_test(FeatureInfoTest RunFeatureInfoTest)

add_executable(RunCFGStaticFeaturesTest
  MainTest.cpp
  CFGStaticFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(CFGStaticFeaturesTest RunCFGStaticFeaturesTest)

target_link_libraries(RunFeatureInfoTest
  gtest gtest_main shogun pthread)
target_link_libraries(RunCFGStaticFeaturesTest
  LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main shogun 
  rt dl tinfo pthread z m)

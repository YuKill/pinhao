
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

add_executable(RunFeatureSetTest
  MainTest.cpp
  FeatureSetTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(FeatureSetTest RunFeatureSetTest)

target_link_libraries(RunFeatureInfoTest
  LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunCFGStaticFeaturesTest
  LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunFeatureSetTest
  LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)

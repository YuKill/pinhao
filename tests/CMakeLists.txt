
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

configure_file (CFGStaticFeaturesBenchmarkTest.sh.in tmp/CFGStaticFeaturesBenchmarkTest.sh @ONLY)
file (COPY "${CMAKE_CURRENT_BINARY_DIR}/tmp/CFGStaticFeaturesBenchmarkTest.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" 
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
  GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
add_test(CFGStaticFeaturesBenchmarkTest CFGStaticFeaturesBenchmarkTest.sh)

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

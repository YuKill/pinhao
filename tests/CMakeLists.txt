
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(RunFeatureInfoTest 
  FeatureInfoTest.cpp
  $<TARGET_OBJECTS:Features>)
add_test(FeatureInfoTest RunFeatureInfoTest)

add_executable(RunCFGStaticFeaturesTest
  CFGStaticFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(CFGStaticFeaturesTest RunCFGStaticFeaturesTest)

add_executable(RunFeatureSetTest
  FeatureSetTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(FeatureSetTest RunFeatureSetTest)

add_executable(RunPAPIFeaturesTest
  PAPIFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:PAPIFeatures>)
add_test(PAPIFeaturesTest RunPAPIFeaturesTest)

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
target_link_libraries(RunPAPIFeaturesTest
  LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMAsmParser LLVMAsmPrinter LLVMCodeGen 
  LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader LLVMInstCombine 
  LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)

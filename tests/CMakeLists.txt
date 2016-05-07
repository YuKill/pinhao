
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(RunFeatureInfoTest 
  FeatureInfoTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>)
add_test(FeatureInfoTest RunFeatureInfoTest)

add_executable(RunCFGStaticFeaturesTest
  CFGStaticFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(CFGStaticFeaturesTest RunCFGStaticFeaturesTest)

add_executable(RunKeyIteratorTest
  KeyIteratorTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:CFGStaticFeatures>
  $<TARGET_OBJECTS:GeneFeatures>)
add_test(KeyIteratorTest RunKeyIteratorTest)

add_executable(RunFeatureSetTest
  FeatureSetTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:CFGStaticFeatures>
  $<TARGET_OBJECTS:GeneFeatures>
  $<TARGET_OBJECTS:PAPIFeatures>)
add_test(FeatureSetTest RunFeatureSetTest)

configure_file (CFGStaticFeaturesBenchmarkTest.sh.in tmp/CFGStaticFeaturesBenchmarkTest.sh @ONLY)
file (COPY "${CMAKE_CURRENT_BINARY_DIR}/tmp/CFGStaticFeaturesBenchmarkTest.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" 
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
  GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
add_test(CFGStaticFeaturesBenchmarkTest CFGStaticFeaturesBenchmarkTest.sh)

add_executable(RunPAPIFeaturesTest
  PAPIFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:PAPIFeatures>)
add_test(PAPIFeaturesTest RunPAPIFeaturesTest)

add_executable(RunYAMLTest
  YAMLTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:GeneFeatures>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(YAMLTest RunYAMLTest)

add_executable(RunGeneFeaturesTest
  GeneFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:GeneFeatures>)
add_test(GeneFeaturesTest RunGeneFeaturesTest)

add_executable(RunYamlOptionsTest
  YamlOptionsTest.cpp
  $<TARGET_OBJECTS:Support>)
add_test(YamlOptionsTest RunYamlOptionsTest)

add_executable(RunYamlOptionsGlobalTest
  YamlOptionsGlobalTest.cpp
  $<TARGET_OBJECTS:Support>)
add_test(YamlOptionsGlobalTest RunYamlOptionsGlobalTest)

configure_file (GeneFeaturesBenchmarkTest.sh.in tmp/GeneFeaturesBenchmarkTest.sh @ONLY)
file (COPY "${CMAKE_CURRENT_BINARY_DIR}/tmp/GeneFeaturesBenchmarkTest.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" 
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
  GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
add_test(GeneFeaturesBenchmarkTest GeneFeaturesBenchmarkTest.sh)

add_executable(RunOptimizerTest
  OptimizerTest.cpp
  $<TARGET_OBJECTS:Optimizer>
  $<TARGET_OBJECTS:Support>)
add_test(OptimizerTest RunOptimizerTest)

add_executable(RunGEOSFeaturesTest
  GEOSFeaturesTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:Optimizer>
  $<TARGET_OBJECTS:GEOSFeatures>)
add_test(GEOSFeaturesTest RunGEOSFeaturesTest)

add_executable(RunFunctionStaticCostFeatureTest
  FunctionStaticCostFeatureTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:StaticCostFeature>)
add_test(FunctionStaticCostFeatureTest RunFunctionStaticCostFeatureTest)

add_executable(RunPAPIWrapperTest
  PAPIWrapperTest.cpp)
add_test(PAPIWrapperTest RunPAPIWrapperTest)

add_executable(RunRandomTest
  RandomTest.cpp
  $<TARGET_OBJECTS:Support>)
add_test(RandomTest RunRandomTest)

add_executable(RunFormulaTest
  FormulaTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(FormulaTest RunFormulaTest)

add_executable(RunFormulaYAMLWrapperTest
  FormulaYAMLWrapperTest.cpp
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:CFGStaticFeatures>)
add_test(FormulaYAMLWrapperTest RunFormulaYAMLWrapperTest)

# -----------------------------------------= Linker =------------------------------------------

target_link_libraries(RunFeatureInfoTest
  FeatureYAMLWrapper
  LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunCFGStaticFeaturesTest
  FeatureYAMLWrapper
  LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunKeyIteratorTest
  FeatureYAMLWrapper
  LLVMIRReader LLVMBitReader LLVMAsmParser LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)
target_link_libraries(RunFeatureSetTest
  FeatureYAMLWrapper
  LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMX86Desc LLVMMCDisassembler LLVMX86Info 
  LLVMX86AsmPrinter LLVMX86Utils LLVMIRReader LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMAsmParser 
  LLVMAsmPrinter LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader 
  LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)
target_link_libraries(RunPAPIFeaturesTest
  FeatureYAMLWrapper
  LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMX86Desc LLVMMCDisassembler LLVMX86Info 
  LLVMX86AsmPrinter LLVMX86Utils LLVMIRReader LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMAsmParser 
  LLVMAsmPrinter LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader 
  LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)
target_link_libraries(RunYAMLTest
  FeatureYAMLWrapper
  LLVMIRReader LLVMBitReader LLVMAsmParser LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunGeneFeaturesTest
  FeatureYAMLWrapper
  LLVMIRReader LLVMBitReader LLVMAsmParser LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunYamlOptionsTest
  gtest gtest_main yaml-cpp pthread)
target_link_libraries(RunYamlOptionsGlobalTest
  gtest gtest_main yaml-cpp pthread)
target_link_libraries(RunOptimizerTest
  OptimizerYAMLWrapper
  LLVMBitWriter LLVMIRReader LLVMAsmParser LLVMipo LLVMVectorize LLVMCodeGen LLVMTarget LLVMInstrumentation 
  LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMMC LLVMBitReader LLVMInstCombine LLVMTransformUtils 
  LLVMipa LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunGEOSFeaturesTest
  GEOSWrapper YAMLWrapper
  GEOSMain GEOSCallCostReader GEOSProf GEOSProfileModule GEOSJITExecution GEOSCostEstimator 
  LLVMObjCARCOpts LLVMX86AsmParser LLVMX86CodeGen LLVMX86Desc LLVMX86Info LLVMMCDisassembler LLVMX86AsmPrinter 
  LLVMX86Utils LLVMSelectionDAG LLVMAsmPrinter LLVMipo LLVMVectorize LLVMCodeGen LLVMScalarOpts LLVMProfileData 
  LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMBitWriter LLVMIRReader LLVMAsmParser 
  LLVMMCJIT LLVMExecutionEngine LLVMTarget LLVMAnalysis LLVMRuntimeDyld LLVMObject LLVMMCParser LLVMBitReader 
  LLVMMC LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)
target_link_libraries(RunFunctionStaticCostFeatureTest
  SProfWrapper YAMLWrapper
  StaticProfiler
  LLVMIRReader LLVMBitReader LLVMAsmParser LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m)
target_link_libraries(RunPAPIWrapperTest
  PAPIWrapper
  LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMX86Desc LLVMMCDisassembler LLVMX86Info 
  LLVMX86AsmPrinter LLVMX86Utils LLVMIRReader LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMAsmParser 
  LLVMAsmPrinter LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader 
  LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)
target_link_libraries(RunRandomTest
  gtest gtest_main yaml-cpp pthread)
target_link_libraries(RunFormulaTest
  FeatureYAMLWrapper FormulaYAMLWrapper Formula 
  LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMX86Desc LLVMMCDisassembler LLVMX86Info 
  LLVMX86AsmPrinter LLVMX86Utils LLVMIRReader LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMAsmParser 
  LLVMAsmPrinter LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader 
  LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)
target_link_libraries(RunFormulaYAMLWrapperTest
  FeatureYAMLWrapper FormulaYAMLWrapper Formula 
  LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMX86Desc LLVMMCDisassembler LLVMX86Info 
  LLVMX86AsmPrinter LLVMX86Utils LLVMIRReader LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMAsmParser 
  LLVMAsmPrinter LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader 
  LLVMInstCombine LLVMInstrumentation LLVMTransformUtils LLVMipa LLVMMC LLVMAnalysis LLVMCore LLVMSupport
  gtest gtest_main yaml-cpp
  rt dl tinfo pthread z m papi)


find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(RunFeatureInfoTest 
  FeatureInfoTest.cpp)
add_test(FeatureInfoTest RunFeatureInfoTest)

add_executable(RunCFGStaticFeaturesTest
  CFGStaticFeaturesTest.cpp)
add_test(CFGStaticFeaturesTest RunCFGStaticFeaturesTest)

add_executable(RunKeyIteratorTest
  KeyIteratorTest.cpp)
add_test(KeyIteratorTest RunKeyIteratorTest)

add_executable(RunFeatureSetTest
  FeatureSetTest.cpp)
add_test(FeatureSetTest RunFeatureSetTest)

configure_file (CFGStaticFeaturesBenchmarkTest.sh.in tmp/CFGStaticFeaturesBenchmarkTest.sh @ONLY)
file (COPY "${CMAKE_CURRENT_BINARY_DIR}/tmp/CFGStaticFeaturesBenchmarkTest.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" 
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
  GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
add_test(CFGStaticFeaturesBenchmarkTest CFGStaticFeaturesBenchmarkTest.sh)

add_executable(RunPAPIFeaturesTest
  PAPIFeaturesTest.cpp)
add_test(PAPIFeaturesTest RunPAPIFeaturesTest)

add_executable(RunYAMLTest
  YAMLTest.cpp)
add_test(YAMLTest RunYAMLTest)

add_executable(RunGeneFeaturesTest
  GeneFeaturesTest.cpp)
add_test(GeneFeaturesTest RunGeneFeaturesTest)

add_executable(RunYamlOptionsTest
  YamlOptionsTest.cpp)
add_test(YamlOptionsTest RunYamlOptionsTest)

add_executable(RunYamlOptionsGlobalTest
  YamlOptionsGlobalTest.cpp)
add_test(YamlOptionsGlobalTest RunYamlOptionsGlobalTest)

configure_file (GeneFeaturesBenchmarkTest.sh.in tmp/GeneFeaturesBenchmarkTest.sh @ONLY)
file (COPY "${CMAKE_CURRENT_BINARY_DIR}/tmp/GeneFeaturesBenchmarkTest.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" 
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
  GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
add_test(GeneFeaturesBenchmarkTest GeneFeaturesBenchmarkTest.sh)

add_executable(RunOptimizerTest
  OptimizerTest.cpp)
add_test(OptimizerTest RunOptimizerTest)

add_executable(RunGEOSFeaturesTest
  GEOSFeaturesTest.cpp)
add_test(GEOSFeaturesTest RunGEOSFeaturesTest)

add_executable(RunFunctionStaticCostFeatureTest
  FunctionStaticCostFeatureTest.cpp)
add_test(FunctionStaticCostFeatureTest RunFunctionStaticCostFeatureTest)

add_executable(RunPAPIWrapperTest
  PAPIWrapperTest.cpp)
add_test(PAPIWrapperTest RunPAPIWrapperTest)

add_executable(RunRandomTest
  RandomTest.cpp)
add_test(RandomTest RunRandomTest)

add_executable(RunFormulaTest
  FormulaTest.cpp)
add_test(FormulaTest RunFormulaTest)

add_executable(RunFormulaYAMLWrapperTest
  FormulaYAMLWrapperTest.cpp)
add_test(FormulaYAMLWrapperTest RunFormulaYAMLWrapperTest)

add_executable(RunKnowledgeBaseTest
  KnowledgeBaseTest.cpp)
add_test(KnowledgeBaseTest RunKnowledgeBaseTest)

add_executable(RunSerialSetTest
  SerialSetTest.cpp)
add_test(SerialSetTest RunSerialSetTest)

# -----------------------------------------= Linker =------------------------------------------

pinhao_test_link (RunFeatureInfoTest)
pinhao_test_link (RunCFGStaticFeaturesTest
  CFGStaticFeatures)
pinhao_test_link (RunKeyIteratorTest
  CFGStaticFeatures GeneFeatures)
pinhao_test_link (RunFeatureSetTest
  CFGStaticFeatures GeneFeatures PAPIFeatures)
pinhao_test_link (RunPAPIFeaturesTest
  PAPIFeatures)
pinhao_test_link (RunYAMLTest
  CFGStaticFeatures GeneFeatures)
pinhao_test_link (RunGeneFeaturesTest
  GeneFeatures)
pinhao_test_link (RunYamlOptionsTest)
pinhao_test_link (RunYamlOptionsGlobalTest)
pinhao_test_link (RunOptimizerTest)
pinhao_test_link (RunGEOSFeaturesTest
  GEOSFeatures)
pinhao_test_link (RunFunctionStaticCostFeatureTest
  StaticCostFeature)
pinhao_test_link (RunPAPIWrapperTest)
pinhao_test_link (RunRandomTest)
pinhao_test_link (RunFormulaTest
  CFGStaticFeatures)
pinhao_test_link (RunFormulaYAMLWrapperTest
  CFGStaticFeatures)
pinhao_test_link (RunKnowledgeBaseTest)
pinhao_test_link (RunSerialSetTest)

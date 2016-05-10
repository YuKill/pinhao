add_subdirectory (GEOSFeatures)

add_library (CFGStaticFeatures SHARED
  CFGBasicBlockStaticFeatures.cpp
  CFGFunctionStaticFeatures.cpp
  CFGModuleStaticFeatures.cpp)

add_library (PAPIFeatures SHARED
  PAPIMultFeatures.cpp)

add_library (GeneFeatures SHARED
  FunctionsGeneFeature.cpp)

add_library (StaticCostFeature SHARED
  FunctionStaticCostFeature.cpp)

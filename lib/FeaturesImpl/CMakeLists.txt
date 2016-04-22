add_subdirectory (GEOSFeatures)

add_library (CFGStaticFeatures OBJECT
  CFGBasicBlockStaticFeatures.cpp
  CFGFunctionStaticFeatures.cpp
  CFGModuleStaticFeatures.cpp)

add_library (PAPIFeatures OBJECT
  PAPIMultFeatures.cpp)

add_library (GeneFeatures OBJECT
  FunctionsGeneFeature.cpp)

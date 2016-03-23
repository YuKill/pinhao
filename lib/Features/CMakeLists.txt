add_library (Features OBJECT
  Features.cpp
  FeatureRegistry.cpp
  FeatureInfo.cpp
  FeatureSet.cpp)

add_library (CFGStaticFeatures OBJECT
  CFGBasicBlockStaticFeatures.cpp
  CFGFunctionStaticFeatures.cpp
  CFGModuleStaticFeatures.cpp)

add_library (PAPIFeatures OBJECT
  PAPIMultFeatures.cpp)


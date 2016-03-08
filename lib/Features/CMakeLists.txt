add_library (Features 
  FeatureRegistry.cpp
  FeatureInfo.cpp
  Features.cpp)

add_library (CFGStaticFeatures SHARED
  CFGBasicBlockStaticFeatures.cpp)

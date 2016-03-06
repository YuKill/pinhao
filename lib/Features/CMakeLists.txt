add_library (Features 
  Feature.cpp
  FeatureInfo.cpp
  Features.cpp)

add_library (CFGStaticFeatures SHARED
  CFGBasicBlockStaticFeatures.cpp)

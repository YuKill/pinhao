add_library (CFGStaticFeaturesPass SHARED
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:CFGStaticFeatures>
  CFGStaticFeaturesPass.cpp
  CFGStaticFeaturesDiffPass.cpp)

target_link_libraries (CFGStaticFeaturesPass yaml-cpp)

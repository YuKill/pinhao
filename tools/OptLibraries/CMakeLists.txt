add_library (CFGStaticFeaturesPass SHARED
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:CFGStaticFeatures>
  CFGStaticFeaturesPass.cpp
  CFGStaticFeaturesDiffPass.cpp
  CFGStaticFeaturesGenPass.cpp)

add_library (GeneFeaturesPass SHARED
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:Support>
  $<TARGET_OBJECTS:GeneFeatures>
  FunctionsGeneFeaturePass.cpp)

target_link_libraries (CFGStaticFeaturesPass 
  FeatureYAMLWrapper
  yaml-cpp)
target_link_libraries (GeneFeaturesPass 
  FeatureYAMLWrapper
  yaml-cpp)


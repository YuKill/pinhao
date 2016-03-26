add_library (CFGStaticFeaturesPass SHARED
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:CFGStaticFeatures>
  CFGStaticFeaturesPass.cpp
  CFGStaticFeaturesDiffPass.cpp
  CFGStaticFeaturesGenPass.cpp)

add_library (GeneFeaturesPass SHARED
  $<TARGET_OBJECTS:Features>
  $<TARGET_OBJECTS:GeneFeatures>
  FunctionsGeneFeaturePass.cpp)

target_link_libraries (CFGStaticFeaturesPass yaml-cpp)
target_link_libraries (GeneFeaturesPass yaml-cpp)


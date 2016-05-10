add_library (CFGStaticFeaturesPass SHARED
  $<TARGET_OBJECTS:CFGStaticFeatures>
  CFGStaticFeaturesPass.cpp
  CFGStaticFeaturesDiffPass.cpp
  CFGStaticFeaturesGenPass.cpp)

add_library (GeneFeaturesPass SHARED
  $<TARGET_OBJECTS:GeneFeatures>
  FunctionsGeneFeaturePass.cpp)

pinhao_pass_link (CFGStaticFeaturesPass)
pinhao_pass_link (GeneFeaturesPass)

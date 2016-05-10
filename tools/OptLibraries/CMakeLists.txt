add_library (CFGStaticFeaturesPass SHARED
  CFGStaticFeaturesPass.cpp
  CFGStaticFeaturesDiffPass.cpp
  CFGStaticFeaturesGenPass.cpp)

add_library (GeneFeaturesPass SHARED
  FunctionsGeneFeaturePass.cpp)

pinhao_pass_link (CFGStaticFeaturesPass 
  CFGStaticFeatures)
pinhao_pass_link (GeneFeaturesPass
  GeneFeatures)

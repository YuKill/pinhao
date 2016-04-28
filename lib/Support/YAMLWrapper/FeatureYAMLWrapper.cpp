/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureYAMLWrapper.cpp
 */

#include "pinhao/Support/FeatureYAMLWrapper.h"

#include "pinhao/Features/Features.h"

using namespace pinhao;

/*
 * -----------------------------------
 *  Overloading for: StringFeature
 */
template<> void pinhao::YAMLWrapper::fill<StringFeature>(StringFeature &Sf, ConstNode &Node) {
  std::string FeatureName = Node["feature-name"].as<std::string>();
  Sf.setValueOf(FeatureName, Node["values"].as<std::string>());
}

template<> void pinhao::YAMLWrapper::append<StringFeature>(const StringFeature &Sf, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "feature-name" << YAML::Value << Sf.getName();
  E << YAML::Key << "values";
  E << YAML::Value << Sf.getValueOf(Sf.getName());
  E << YAML::EndMap;
}

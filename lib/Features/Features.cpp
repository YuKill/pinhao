/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Features.cpp
 * @brief This file implements the functions of the basic implementations
 * of the feature classes. Such as the StringFeature , the VectorFeature and
 * the MapVectorFeature.
 */

#include "pinhao/Features/Features.h"

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: Yamlfy<StringFeature>
 */

void Yamlfy<StringFeature>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << Value->getName();
  Emitter << YAML::Key << "values";
  Emitter << YAML::Value << Value->getValueOf(Value->getName());
  Emitter << YAML::EndMap;
}

void Yamlfy<StringFeature>::get(const YAML::Node &Node) {
  std::string FeatureName = Node["feature-name"].as<std::string>();
  Value->setValueOf(FeatureName, Node["values"].as<std::string>());
}

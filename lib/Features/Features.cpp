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
 * Class: StringFeature
 */
void StringFeature::appendYaml(YAML::Emitter &Emitter) {
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();

  Emitter << YAML::Key << "values";
  Emitter << YAML::Value << TheFeature;

  Emitter << YAML::EndMap;
}

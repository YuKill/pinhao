/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Features.cpp
 * @brief This file implements the functions of the basic implementations
 * of the feature classes. Such as the StringFeature , the VectorFeature and
 * the MapVectorFeature.
 */

#include "pinhao/Features/Features.h"
#include "pinhao/Support/YAMLWrapper.h"

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: StringFeature
 */
StringFeature::StringFeature(FeatureInfo *Info) : LinearFeature<std::string>(Info) {

}

void StringFeature::setValueOf(std::string FeatureName, std::string Value) {
  assert(FeatureName == this->Info->getName() && "FeatureName doesn't equal the name of this feature.");
  TheFeature = Value;
}   

const std::string& StringFeature::getValueOf(std::string FeatureName) const {
  assert(FeatureName == this->Info->getName() && "FeatureName doesn't equal the name of this feature.");
  return TheFeature;
}

void StringFeature::append(YAML::Emitter &Emitter) const {
  YAMLWrapper::append(*this, Emitter);
}

void StringFeature::get(const YAML::Node &Node) {
  YAMLWrapper::fill(*this, Node);
}

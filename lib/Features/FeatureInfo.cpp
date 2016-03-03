/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureInfo.cpp
 * @brief This file implements the overrided functions by the class
 * CompositeFeatureInfo.
 */

#include "pinhao/Features/FeatureInfo.h"

using namespace pinhao;

bool CompositeFeatureInfo::hasSubFeature(std::string SubFeatureName) {
  return FeaturesInfo.count(SubFeatureName) > 0;
}

std::string CompositeFeatureInfo::getSubFeatureDescription(std::string SubFeatureName) {
  assert(hasSubFeature(SubFeatureName) && "Composite feature does not have sub-feature specified.");
  return FeaturesInfo[SubFeatureName];
}

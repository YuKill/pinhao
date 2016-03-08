/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureRegistry.cpp
 * @brief This file implements the functions and defines the static
 * members of the FeatureRegistry class.
 */

#include "pinhao/Features/FeatureRegistry.h"
#include "pinhao/Features/Feature.h"

using namespace pinhao;

std::map<std::string, std::unique_ptr<const Feature>> FeatureRegistry::RegisteredFeatures;

bool FeatureRegistry::isNameRegistered(std::string Name) {
  for (auto &RegFeature : RegisteredFeatures) {
    auto FeaturePointer = &(RegFeature.second);

    if ((*FeaturePointer)->isComposite()) {
      if ((*FeaturePointer)->hasSubFeature(Name))
        return true;
    }
  }
  return false;
}

void FeatureRegistry::registerFeature(Feature *F) {
  assert(RegisteredFeatures.count(F->getName()) == 0 &&
      "Feature Name already registered.");

  if (F->isComposite()) {
    for (auto &FInfo : *F)
      assert(!isNameRegistered(FInfo.first) && "Sub-Feature name already registered.");
  }

  RegisteredFeatures.insert(std::make_pair(F->getName(), std::unique_ptr<Feature>(F)));
}

std::unique_ptr<Feature> FeatureRegistry::get(std::string FeatureName) {
  if (RegisteredFeatures.count(FeatureName) > 0) 
    return RegisteredFeatures[FeatureName]->clone();
  return std::unique_ptr<Feature>();
}

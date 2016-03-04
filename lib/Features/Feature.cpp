/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Feature.cpp
 * @brief This file implements the functions and defines the static
 * members of the Feature class.
 */

#include "pinhao/Features/Feature.h"

using namespace pinhao;

std::map<std::string, std::unique_ptr<Feature>> Feature::RegisteredFeatures;

bool Feature::isNameRegistered(std::string Name) {
  for (auto &RegFeature : RegisteredFeatures) {
    auto FeaturePointer = &(RegFeature.second);

    if ((*FeaturePointer)->isComposite()) {
      if ((*FeaturePointer)->hasSubFeature(Name))
        return true;
    }
  }
  return false;
}

void Feature::registerFeature(Feature *F) {
  assert(RegisteredFeatures.count(F->getName()) == 0 &&
      "Feature Name already registered.");

  if (F->isComposite()) {
    for (auto &FInfo : *F)
      assert(!isNameRegistered(FInfo.first) && "Sub-Feature name already registered.");
  }

  RegisteredFeatures.insert(std::make_pair(F->getName(), std::unique_ptr<Feature>(F)));
}

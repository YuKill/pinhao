/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeaturesSet.cpp
 */

#include "pinhao/Features/FeatureSet.h"

using namespace pinhao;

std::map<std::string, std::vector<bool>> FeatureSet::EnableFeature;

std::unique_ptr<FeatureSet> FeatureSet::get() {
  const int ParentFeature = 0;
  FeatureSet *Set = new FeatureSet();
  for (auto &Pair : EnableFeature) {
    if (!Pair.second[ParentFeature]) continue;
    Set->Features.insert(std::make_pair(Pair.first, FeatureRegistry::get(Pair.first)));
  }
  return std::unique_ptr<FeatureSet>(Set);
}

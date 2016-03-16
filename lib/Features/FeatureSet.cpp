/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeaturesSet.cpp
 */

#include "pinhao/Features/FeatureSet.h"

#include "llvm/PassRegistry.h"

using namespace pinhao;
using namespace llvm;

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

/*=--------------------------------------------=
 * class: FeatureSetWrapperPass
 */

char FeatureSetWrapperPass::ID = 0;

bool FeatureSetWrapperPass::runOnModule(Module &M) {
  if (!Set.get()) Set = std::shared_ptr<FeatureSet>(FeatureSet::get().release());
  for (auto &Pair : Set->Features) {
    if (!(Pair.second)->isProcessed())
      (Pair.second)->processModule(M);
  }
  return false;
}

static llvm::RegisterPass<FeatureSetWrapperPass> X("fset-pass", "The Pass for a FeatureSet.", false, false);

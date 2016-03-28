/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeaturesSet.cpp
 */

#include "pinhao/Features/FeatureSet.h"

#include "llvm/PassRegistry.h"

using namespace pinhao;
using namespace llvm;

/*=--------------------------------------------=
 * class: FeatureSet
 */

std::map<std::string, std::map<std::string, bool>> FeatureSet::EnabledFeatures;

std::unique_ptr<FeatureSet> FeatureSet::get() {
  FeatureSet *Set = new FeatureSet();
  for (auto &Pair : EnabledFeatures) {
    Set->Features.insert(std::make_pair(Pair.first, FeatureRegistry::get(Pair.first)));
  }
  return std::unique_ptr<FeatureSet>(Set);
}

/*=--------------------------------------------=
 * class: FeatureSetWrapperPass::iterator
 */
FeatureSet::iterator::iterator(EnabledFeaturesMap *M, EnabledFeaturesMap::iterator It, std::string Name) :
  Map(M), MapIterator(It), FeatureName(Name) {
    if (MapIterator != Map->end()) {
      if (FeatureName != "") 
        MapIterator = Map->find(FeatureName);
      if (MapIterator->second->isComposite()) 
        FeatureIterator = MapIterator->second->begin();
    }
    setPair();
  }

void FeatureSet::iterator::setPair() {
  if (MapIterator == Map->end()) return;
  Pair.first = MapIterator->first;
  if (MapIterator->second->isComposite())
    Pair.second = FeatureIterator->first;
  else
    Pair.second = "";
}

const FeatureSet::iterator::PairType *FeatureSet::iterator::operator->() const {
  return &Pair;
}

FeatureSet::iterator::PairType &FeatureSet::iterator::operator*() {
  return Pair;
}

void FeatureSet::iterator::incMapIterator() {
  ++MapIterator;
  if (MapIterator != Map->end() && MapIterator->second->isComposite()) 
    FeatureIterator = MapIterator->second->begin();
}

FeatureSet::iterator &FeatureSet::iterator::operator++() {
  if (FeatureName == "") {

    if (MapIterator->second->isComposite()) {
      ++FeatureIterator;
      if (FeatureIterator == MapIterator->second->end()) 
        incMapIterator();
    } else incMapIterator();

  } else {

    if (MapIterator->second->isComposite()) 
      ++FeatureIterator;
    if (!(MapIterator->second->isComposite()) || FeatureIterator == MapIterator->second->end())
      MapIterator = Map->end();

  } 
  setPair();
  return *this;
}

FeatureSet::iterator FeatureSet::iterator::operator++(int) {
  FeatureSet::iterator It(*this);
  operator++();
  return It;
}

bool FeatureSet::iterator::operator==(const iterator &It) {
  if (MapIterator != It.MapIterator) return false;
  if (MapIterator == Map->end()) return true;

  if (MapIterator->second->isComposite())
    return FeatureIterator == It.FeatureIterator; 
  return true;
}

bool FeatureSet::iterator::operator!=(const iterator &It) {
  return !(*this == It);
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

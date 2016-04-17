/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSet.cpp
 */

#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Optimizer/OptimizationInfo.h"

#include <algorithm>

using namespace pinhao;

OptimizationSet::OptimizationSet() {
  DefaultSequence = nullptr;
}

OptimizationSet::~OptimizationSet() {

}

bool OptimizationSet::hasEnabled(Optimization Opt) {
  for (auto Pair : EnabledOptimizations)
    if (Pair.first.getOptimization() == Opt)
      return true;
  return false;
}

void OptimizationSet::disableOptimization(Optimization Opt) {
  if (!hasEnabled(Opt)) return;
  for (auto I = EnabledOptimizations.begin(), E = EnabledOptimizations.end(); I != E; ++I)
    if (I->first.getOptimization() == Opt)
      EnabledOptimizations.erase(I);
}

void OptimizationSet::enableOptimization(Optimization Opt, uint64_t Repetition) {
  if (hasEnabled(Opt)) return;
  EnabledOptimizations.push_back(std::make_pair(OptimizationInfo(Opt), Repetition));
}

void OptimizationSet::enableOptimization(OptimizationInfo Info, uint64_t Repetition) {
  EnabledOptimizations.push_back(std::make_pair(Info, Repetition));
}

void OptimizationSet::enableOptimizations(std::vector<Optimization> Opts) {
  for (auto O : Opts)
    enableOptimization(O);
}

void OptimizationSet::enableAllOptimizations() {
  for (auto OName : Optimizations)
    enableOptimization(getOptimization(OName)); 
}

void OptimizationSet::enableModuleOptimizations() {
  for (auto OName : Optimizations)
    if (getPassKind(getOptimization(OName)) >= 4)
      enableOptimization(getOptimization(OName)); 
}

void OptimizationSet::enableFunctionOptimizations() {
  for (auto OName : Optimizations)
    if (getPassKind(getOptimization(OName)) < 4)
      enableOptimization(getOptimization(OName)); 
}

void OptimizationSet::generateRandomSequenceIfNone() {
  if (DefaultSequence) return;
  std::shared_ptr<OptimizationSet> Wrapper(this);
  DefaultSequence = OptimizationSequence::generate(Wrapper).release();
}

uint64_t OptimizationSet::size() {
  return EnabledOptimizations.size();
}

std::unique_ptr<OptimizationSet> OptimizationSet::getFromSequence(OptimizationSequence Seq) {
  OptimizationSet *Set = new OptimizationSet();

  std::map<Optimization, uint64_t> Repetition;
  for (auto &O : Seq) {
    if (Repetition.count(O.getOptimization()) > 0)
      Repetition[O.getOptimization()]++;
    else
      Repetition[O.getOptimization()] = 0;
  }

  for (auto Pair : Repetition)
    Set->enableOptimization(Pair.first, Pair.second); 

  return std::unique_ptr<OptimizationSet>(Set);
}

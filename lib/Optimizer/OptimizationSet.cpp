/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSet.cpp
 */

#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Optimizer/OptimizationProperties.h"

#include <algorithm>

using namespace pinhao;

Optimization pinhao::getOptimization(std::string OptName) {
  uint64_t Count = 0;
  for (auto S : Optimizations) {
    if (S == OptName) break;
    ++Count;
  }
  assert(Count != Optimizations.size() && "There is no such optimization name.");
  return static_cast<Optimization>(Count);
}

std::string pinhao::getOptimizationName(Optimization Opt) {
  return Optimizations[static_cast<int>(Opt)];
}

int getPassKind(Optimization Opt) {
  int PassKind;
  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  const llvm::PassInfo *PI = Registry->getPassInfo(getOptimizationName(Opt));
  llvm::Pass *Pass = PI->createPass();
  PassKind = Pass->getPassKind();
  delete Pass;
  return PassKind;
}

/*
 * ----------------------------------=
 *  Class: OptimizationSet
 */
OptimizationSet::OptimizationSet() {

}

bool OptimizationSet::hasEnabled(Optimization Opt) {
  return EnabledOptimizations.count(Opt) > 0;
}

void OptimizationSet::disableOptimization(Optimization Opt) {
  if (!hasEnabled(Opt)) return;
  EnabledOptimizations.erase(Opt);
}

void OptimizationSet::enableOptimization(Optimization Opt) {
  if (hasEnabled(Opt)) return;
  EnabledOptimizations.insert(std::make_pair(Opt, OptimizationProperties(Opt)));
}

void OptimizationSet::enableOptimization(Optimization Opt, OptimizationProperties Prop) {
  if (hasEnabled(Opt)) return;
  EnabledOptimizations.insert(std::make_pair(Opt, Prop));
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

void OptimizationSet::setOptimizationProperties(Optimization Opt, OptimizationProperties Prop) {
  if (hasEnabled(Opt))
    EnabledOptimizations[Opt] = Prop;
}

OptimizationProperties OptimizationSet::getOptimizationProperties(Optimization Opt) {
  assert(hasEnabled(Opt) && "Can't get property of disabled optimization.");
  return EnabledOptimizations[Opt];
}

void OptimizationSet::setDefaultSequence(OptimizationSequence OptSeq) {
  *DefaultSequence = OptSeq;
}

OptimizationSequence OptimizationSet::getDefaultSequence() {
  assert(DefaultSequence != nullptr && "Default Sequence is null.");
  return *DefaultSequence;
}

uint64_t OptimizationSet::size() {
  return EnabledOptimizations.size();
}

std::unique_ptr<OptimizationSet> OptimizationSet::getFromSequence(OptimizationSequence Seq) {
  OptimizationSet *Set = new OptimizationSet();
  for (auto &O : Seq) {
    if (!Set->hasEnabled(O)) 
      Set->enableOptimization(O);
    else {
      OptimizationProperties Properties = Set->getOptimizationProperties(O);
      ++Properties.Repetition;
      Set->setOptimizationProperties(O, Properties); 
    }
  }
  return std::unique_ptr<OptimizationSet>(Set);
}

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
  OptimizationInfo Info(Opt);
  return hasEnabled(Info);
}

bool OptimizationSet::hasEnabled(OptimizationInfo Info) {
  return EnabledOptimizations.count(Info) > 0;
}

void OptimizationSet::disableOptimization(Optimization Opt) {
  OptimizationInfo Info(Opt);
  disableOptimization(Info);
}

void OptimizationSet::disableOptimization(OptimizationInfo Info) {
  if (!hasEnabled(Info)) return;
  EnabledOptimizations.erase(Info);
}

void OptimizationSet::enableOptimization(Optimization Opt, uint64_t Repetition) {
  OptimizationInfo Info(Opt);
  enableOptimization(Info, Repetition);
}

void OptimizationSet::enableOptimization(OptimizationInfo Info, uint64_t Repetition) {
  if (hasEnabled(Info)) return;
  EnabledOptimizations.insert(std::make_pair(Info, Repetition));
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

void OptimizationSet::addOptimization(Optimization Opt) {
  OptimizationInfo Info(Opt);
  addOptimization(Info);
}

void OptimizationSet::addOptimization(OptimizationInfo Info) {
  if (hasEnabled(Info)) setRepetition(Info, getRepetition(Info) + 1);
  else enableOptimization(Info);
}

void OptimizationSet::setRepetition(Optimization Opt, uint64_t Rep) {
  OptimizationInfo Info(Opt);
  setRepetition(Info, Rep);
}

void OptimizationSet::setRepetition(OptimizationInfo Info, uint64_t Rep) {
  if (!hasEnabled(Info)) return;
  EnabledOptimizations[Info] = Rep;
}

uint64_t OptimizationSet::getRepetition(Optimization Opt) {
  OptimizationInfo Info(Opt);
  return getRepetition(Info);
}

uint64_t OptimizationSet::getRepetition(OptimizationInfo Info) {
  if (!hasEnabled(Info)) return 0;
  return EnabledOptimizations[Info];
}

void OptimizationSet::generateRandomSequenceIfNone() {
  if (DefaultSequence) return;
  DefaultSequence = OptimizationSequence::generate(*this).release();
}

uint64_t OptimizationSet::size() {
  return EnabledOptimizations.size();
}

std::unique_ptr<OptimizationSet> OptimizationSet::generate(OptimizationSequence Seq) {
  OptimizationSet *Set = new OptimizationSet();

  for (auto &Info : Seq) 
    Set->addOptimization(Info);

  return std::unique_ptr<OptimizationSet>(Set);
}

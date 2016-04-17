/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.cpp
 */

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationInfo.h"
#include "pinhao/Optimizer/OptimizationSequence.h"
#include "pinhao/Optimizer/OptimizationSet.h"

#include <ostream>

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: Yamlfy<OptimizationSequence>
 */
Yamlfy<OptimizationSequence>::Yamlfy(const OptimizationSequence *V) : 
  YamlfyTemplateBase<OptimizationSequence>(V) {

  }

void Yamlfy<OptimizationSequence>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  Emitter << YAML::BeginSeq;

  for (auto OptInfo : Value->Sequence) 
    Yamlfy<OptimizationInfo>(&OptInfo).append(Emitter, false);

  Emitter << YAML::EndSeq;
}

void Yamlfy<OptimizationSequence>::get(const YAML::Node &Node) {
  for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
    OptimizationInfo Info;
    Yamlfy<OptimizationInfo>(&Info).get(*I);
    Value->Sequence.push_back(Info);
  }
  Value->Set.reset(OptimizationSet::getFromSequence(*Value).release());
}

/*
 * ----------------------------------=
 * Class: OptimizationSequence
 */
struct OptimizationInfoCompare {
  bool operator()(const OptimizationInfo &lhs, const OptimizationInfo &rhs) const {
    return lhs.getOptimization() < rhs.getOptimization();
  }
};

std::unique_ptr<OptimizationSequence> 
OptimizationSequence::generate(std::shared_ptr<OptimizationSet> Set) {
  if (Set->size() == 0) 
    return std::unique_ptr<OptimizationSequence>();

  srand(time(0));
  double PickProbability = 0.5;
  std::map<OptimizationInfo, uint64_t, OptimizationInfoCompare> Repetition;

  OptimizationSequence *OptSeq = new OptimizationSequence();
  OptSeq->Set = Set;

  while (true) {
    bool Finish = true;
    for (auto &Pair : *Set) {
      if (Repetition.count(Pair.first) == 0)
        Repetition[Pair.first] = Pair.second;
      if (Repetition[Pair.first] == 0) continue;

      double Roll = (double)rand() / RAND_MAX; 
      if (Roll < PickProbability) {
        OptSeq->Sequence.push_back(Pair.first); 
        Repetition[Pair.first]--;
      }

      if (Repetition[Pair.first])
        Finish = false;
    }
    if (Finish) break;
  }

  return std::unique_ptr<OptimizationSequence>(OptSeq);
}

std::unique_ptr<OptimizationSequence> 
OptimizationSequence::generate(std::vector<Optimization> Sequence) {
  if (Sequence.size() == 0) 
    return std::unique_ptr<OptimizationSequence>();

  OptimizationSequence *OptSeq = new OptimizationSequence();
  for (auto O : Sequence)
    OptSeq->Sequence.push_back(OptimizationInfo(O));
  OptSeq->Set = OptimizationSet::getFromSequence(*OptSeq);
  return std::unique_ptr<OptimizationSequence>(OptSeq);
}

std::unique_ptr<OptimizationSequence> 
OptimizationSequence::randomize(uint64_t Size) {
  if (Size == 0) 
    return std::unique_ptr<OptimizationSequence>();

  srand(time(0));
  OptimizationSequence *OptSeq = new OptimizationSequence();
  while (OptSeq->Sequence.size() != Size) {
    int N = rand() % Optimizations.size(); 
    OptSeq->Sequence.push_back(OptimizationInfo(static_cast<Optimization>(N)));
  }
  OptSeq->Set = OptimizationSet::getFromSequence(*OptSeq);
  return std::unique_ptr<OptimizationSequence>(OptSeq);
}

void OptimizationSequence::populatePassManager(llvm::legacy::PassManager &PM) {
  for (auto O : Sequence)
    PM.add(O.createPass()); 
}

void OptimizationSequence::populateFunctionPassManager(llvm::legacy::FunctionPassManager &FPM) {
  for (auto O : Sequence) {
    llvm::Pass *P = O.createPass();
    assert(P->getPassKind() < 4 && "Cannot add Pass with PassKind over 4.");
    FPM.add(P); 
  }
}

void OptimizationSequence::print() {
  print(std::cout);
}

void OptimizationSequence::print(std::ostream &Out) {
  YAML::Emitter Emitter(Out);
  Yamlfy<OptimizationSequence>(this).append(Emitter, false);
}

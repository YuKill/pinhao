/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.cpp
 */

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationProperties.h"
#include "pinhao/Optimizer/OptimizationSequence.h"
#include "pinhao/Optimizer/OptimizationSet.h"

#include "llvm/Transforms/Scalar.h"

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
  Emitter << YAML::BeginMap;

  for (auto Opt : Value->Sequence) {
    OptimizationProperties Prop = Value->Set->getOptimizationProperties(Opt);

    Emitter << YAML::Key << getOptimizationName(Opt);
    Emitter << YAML::Value; 
    Yamlfy<OptimizationProperties>(&Prop).append(Emitter, false);
  }

  Emitter << YAML::EndMap;
}

void Yamlfy<OptimizationSequence>::get(const YAML::Node &Node) {
  Value->Set.reset(new OptimizationSet());
  std::shared_ptr<OptimizationSet> Set(Value->Set);
  for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
    Optimization Opt = getOptimization(I->first.as<std::string>());
    OptimizationProperties Prop(Opt);
    Yamlfy<OptimizationProperties>(&Prop).get(I->second);
    Set->enableOptimization(Opt, Prop);
  }
}

/*
 * ----------------------------------=
 * Class: OptimizationSequence
 */
llvm::Pass *OptimizationSequence::getPass(Optimization Opt) {
  OptimizationProperties Prop = Set->getOptimizationProperties(Opt);
  switch (Opt) {
    case Optimization::gvn:
      return llvm::createGVNPass(Prop.getArg<bool>(0));

    case Optimization::jumpThreading:
      return llvm::createJumpThreadingPass(Prop.getArg<int>(0));

    case Optimization::loopRotate:
      return llvm::createLoopRotatePass(Prop.getArg<int>(0));

    case Optimization::loopUnroll:
      return llvm::createLoopUnrollPass(Prop.getArg<int>(0), Prop.getArg<int>(1), Prop.getArg<int>(2), Prop.getArg<int>(3));

    case Optimization::loopUnswitch:
      return llvm::createLoopUnswitchPass(Prop.getArg<bool>(0));

    case Optimization::scalarrepl:
      return llvm::createScalarReplAggregatesPass(
          Prop.getArg<int>(0), 
          Prop.getArg<bool>(1), 
          Prop.getArg<int>(2), 
          Prop.getArg<int>(3), 
          Prop.getArg<int>(4));

    case Optimization::simplifycfg:
      return llvm::createCFGSimplificationPass(Prop.getArg<int>(0));

    default:
      break;
  };

  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  const llvm::PassInfo *PI = Registry->getPassInfo(getOptimizationName(Opt));
  return PI->createPass();
}

std::unique_ptr<OptimizationSequence> 
OptimizationSequence::generate(std::shared_ptr<OptimizationSet> Set) {
  if (Set->size() == 0) 
    return std::unique_ptr<OptimizationSequence>();

  srand(time(0));
  double PickProbability = 0.5;
  std::map<Optimization, uint64_t> Repetition;

  OptimizationSequence *OptSeq = new OptimizationSequence();
  OptSeq->Set = Set;

  while (true) {
    bool Finish = true;
    for (auto &Pair : *Set) {
      if (Repetition.count(Pair.first) == 0)
        Repetition[Pair.first] = Pair.second.Repetition;
      if (Repetition[Pair.first] == 0) continue;

      double Roll = rand() / RAND_MAX; 
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
  OptSeq->Sequence = Sequence;
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
    OptSeq->Sequence.push_back(static_cast<Optimization>(N));
  }
  OptSeq->Set = OptimizationSet::getFromSequence(*OptSeq);
  return std::unique_ptr<OptimizationSequence>(OptSeq);
}

void OptimizationSequence::populatePassManager(llvm::legacy::PassManager &PM) {
  for (auto O : Sequence)
    PM.add(getPass(O)); 
}

void OptimizationSequence::populateFunctionPassManager(llvm::legacy::FunctionPassManager &FPM) {
  for (auto O : Sequence) {
    llvm::Pass *P = getPass(O);
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

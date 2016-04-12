/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.cpp
 */

#include "pinhao/Optimizer/OptimizationSequence.h"
#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Optimizer/OptimizationProperties.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace pinhao;

static void printModule(llvm::Module *Module, std::string Path) {
  std::error_code Err;
  llvm::raw_fd_ostream *Out = new llvm::raw_fd_ostream(Path, Err, llvm::sys::fs::OpenFlags::F_RW);
  llvm::WriteBitcodeToFile(Module, *Out);   
  Out->flush();
  delete Out;
}

static llvm::Module *readModule(std::string Path) {
  llvm::LLVMContext &Context = llvm::getGlobalContext();
  llvm::SMDiagnostic Error;
  return llvm::parseIRFile(Path, Error, Context).release();
}

llvm::Module *pinhao::optimize(llvm::Module &Module, OptimizationSequence &Sequence) {
  std::string TmpName = ".tmp-" + std::to_string(time(0));

  fflush(stdout);
  pid_t Pid = fork();

  if (Pid == 0) {
    llvm::legacy::PassManager PM;
    Sequence.populatePassManager(PM);
    PM.run(Module);

    printModule(&Module, TmpName);
    exit(0);
  }

  int Return;
  waitpid(Pid, &Return, 0);
  if (Return != 0) {
    std::cerr << "Failed when applying optimizations." << std::endl;
    return nullptr;
  }

  return readModule(TmpName);
}

llvm::Module *pinhao::optimize(llvm::Module &Module, llvm::Function &Function, OptimizationSequence &Sequence) {
  std::string TmpName = ".tmp-" + std::to_string(time(0));

  fflush(stdout);
  pid_t Pid = fork();
  if (Pid == 0) {
    llvm::legacy::FunctionPassManager FPM(&Module);
    Sequence.populateFunctionPassManager(FPM);
    FPM.run(Function);

    printModule(&Module, TmpName);
    exit(0);
  }

  int Return;
  waitpid(Pid, &Return, 0);
  if (Return != 0) {
    std::cerr << "Failed when applying optimizations." << std::endl;
    return nullptr;
  }

  return readModule(TmpName);
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

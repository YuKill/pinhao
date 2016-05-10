/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Optimizations.cpp
 */

#include "pinhao/InitializationRoutines.h"
#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Optimizer/OptimizationSequence.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize.h"
#include "llvm/Transforms/IPO.h"

#include "llvm/IR/LegacyPassManager.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

using namespace pinhao;

void pinhao::initializeOptimizer() {
  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  llvm::initializeCore(*Registry);
  llvm::initializeTransformUtils(*Registry);
  llvm::initializeScalarOpts(*Registry);
  llvm::initializeInstCombine(*Registry);
  llvm::initializeAnalysis(*Registry);
  llvm::initializeVectorization(*Registry);
  llvm::initializeIPO(*Registry);
  llvm::initializeCodeGen(*Registry);
}

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

int pinhao::getPassKind(Optimization Opt) {
  int PassKind;
  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  const llvm::PassInfo *PI = Registry->getPassInfo(getOptimizationName(Opt));
  llvm::Pass *Pass = PI->createPass();
  PassKind = Pass->getPassKind();
  delete Pass;
  return PassKind;
}

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

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, OptimizationSet *Set) {
  Set->generateRandomSequenceIfNone(); 
  return applyOptimizations(Module, Set->DefaultSequence);
}

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, llvm::Function *Function, OptimizationSet *Set) {
  Set->generateRandomSequenceIfNone(); 
  return applyOptimizations(Module, Function, Set->DefaultSequence);
}

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, std::string FunctionName, OptimizationSet *Set) {
  Set->generateRandomSequenceIfNone(); 
  return applyOptimizations(Module, Module.getFunction(FunctionName), Set->DefaultSequence);
}

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, OptimizationSequence *Sequence) {
  std::string TmpName = ".tmp-" + std::to_string(time(0));

  fflush(stdout);
  pid_t Pid = fork();

  if (Pid == 0) {
    llvm::legacy::PassManager PM; 
    Sequence->populatePassManager(PM);
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

  llvm::Module *OptModule = readModule(TmpName);
  remove(TmpName.c_str());
  return OptModule;
}

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, std::string FunctionName, OptimizationSequence *Sequence) {
  return applyOptimizations(Module, Module.getFunction(FunctionName), Sequence);
}

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, llvm::Function *Function, OptimizationSequence *Sequence) {
  std::string TmpName = ".tmp-" + std::to_string(time(0));

  fflush(stdout);
  pid_t Pid = fork();
  if (Pid == 0) {
    llvm::legacy::FunctionPassManager FPM(&Module);
    Sequence->populateFunctionPassManager(FPM);
    FPM.run(*Function);

    printModule(&Module, TmpName);
    exit(0);
  }

  int Return;
  waitpid(Pid, &Return, 0);
  if (Return != 0) {
    std::cerr << "Failed when applying optimizations." << std::endl;
    return nullptr;
  }

  llvm::Module *OptModule = readModule(TmpName);
  remove(TmpName.c_str());
  return OptModule;
}

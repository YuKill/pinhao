/*-------------------------- PINHAO project --------------------------*/

/**
 * @file OptimizationSequence.cpp
 */

#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationInfo.h"
#include "pinhao/Optimizer/OptimizationSequence.h"
#include "pinhao/Optimizer/OptimizationSet.h"

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/Random.h"

#include "llvm/IR/Verifier.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/CodeGen/CommandFlags.h"

#include <ostream>

using namespace pinhao;

/*
 * ----------------------------------=
 * Class: OptimizationSequence
 */
std::unique_ptr<OptimizationSequence> 
OptimizationSequence::generate(OptimizationSet &Set, int Size) {
  if (Set.size() == 0) 
    return std::unique_ptr<OptimizationSequence>();

  if (Size == -1) Size = Set.size();

  srand(time(0));
  double PickProbability = 0.5;
  std::map<OptimizationInfo, uint64_t> Repetition;
  OptimizationSequence *OptSeq = new OptimizationSequence();

  while (true) {
    bool Finish = true;
    for (auto &Pair : Set) {
      if (Repetition.count(Pair.second.first) == 0)
        Repetition[Pair.second.first] = Pair.second.second;
      if (Repetition[Pair.second.first] == 0) continue;

      double Roll = (double)rand() / RAND_MAX; 
      if (Roll < PickProbability) {
        OptSeq->push_back(Pair.second.first); 
        Repetition[Pair.second.first]--;
      }

      if (Repetition[Pair.second.first])
        Finish = false;

      if ((unsigned)Size == OptSeq->size()) break;

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
    OptSeq->push_back(OptimizationInfo(O));

  return std::unique_ptr<OptimizationSequence>(OptSeq);
}

std::unique_ptr<OptimizationSequence> 
OptimizationSequence::randomize(uint64_t Size) {
  if (Size == 0) 
    return std::unique_ptr<OptimizationSequence>();

  srand(time(0));
  OptimizationSequence *OptSeq = new OptimizationSequence();
  while (OptSeq->size() != Size) {
    int N = rand() % Optimizations.size(); 
    OptSeq->push_back(OptimizationInfo(static_cast<Optimization>(N)));
  }
  return std::unique_ptr<OptimizationSequence>(OptSeq);
}

OptimizationInfo OptimizationSequence::getOptimization(uint64_t N) {
  assert(N < size() && "Trying to get an out of bounds optimization.");
  return (*this)[N];
}

void OptimizationSequence::populatePassManager(llvm::legacy::PassManager &PM) {
  for (auto O : *this) {
    llvm::Pass *P = O.createPass();
    if (P) PM.add(O.createPass()); 
    else std::cerr << "Failed creating pass: " << O.getName() << std::endl;
  }

  PM.add(llvm::createVerifierPass());
}

void OptimizationSequence::populateFunctionPassManager(llvm::legacy::FunctionPassManager &FPM) {
  for (auto O : *this) {
    llvm::Pass *P = O.createPass();
    assert(P->getPassKind() < 4 && "Cannot add Pass with PassKind over 4.");
    FPM.add(P); 
  }
}

static llvm::CodeGenOpt::Level GetCodeGenOptLevel(OptLevel OLevel) {
  if (OLevel == OptLevel::O1)
    return llvm::CodeGenOpt::Less;
  if (OLevel == OptLevel::O2)
    return llvm::CodeGenOpt::Default;
  if (OLevel == OptLevel::O3)
    return llvm::CodeGenOpt::Aggressive;
  return llvm::CodeGenOpt::None;
}

static TargetMachine* GetTargetMachine(Triple TheTriple, StringRef CPUStr,
    StringRef FeaturesStr, const TargetOptions &Options, OptLevel OLevel) {

  std::string Error;
  const llvm::Target *TheTarget = llvm::TargetRegistry::lookupTarget(MArch, TheTriple, Error);

  if (!TheTarget) {
    return nullptr;
  }

  return TheTarget->createTargetMachine(TheTriple.getTriple(), CPUStr, FeaturesStr, Options,
      llvm::Reloc::Default, llvm::CodeModel::JITDefault, GetCodeGenOptLevel(OLevel));

}

void OptimizationSequence::addDefaultPasses(llvm::Module &Module, llvm::legacy::PassManager &PM,
    llvm::legacy::FunctionPassManager &FPM) {

  /*
  llvm::Triple ModuleTriple(Module.getTargetTriple());
  std::string CPUStr, FeaturesStr;
  llvm::TargetMachine *Machine = nullptr;
  const llvm::TargetOptions Options = InitTargetOptionsFromCodeGenFlags();

  if (ModuleTriple.getArch()) {
    CPUStr = getCPUStr();
    FeaturesStr = getFeaturesStr();
    Machine = GetTargetMachine(ModuleTriple, CPUStr, FeaturesStr, Options, OLevel);
  }

  std::unique_ptr<llvm::TargetMachine> TM(Machine);
  setFunctionAttributes(CPUStr, FeaturesStr, Module);

  llvm::TargetLibraryInfoImpl TLII(ModuleTriple);
  PM.add(new llvm::TargetLibraryInfoWrapperPass(TLII));

  if (TM) {
    PM.add(llvm::createTargetTransformInfoWrapperPass(TM->getTargetIRAnalysis()));
    FPM.add(llvm::createTargetTransformInfoWrapperPass(TM->getTargetIRAnalysis()));
  } else {
    PM.add(llvm::createTargetTransformInfoWrapperPass(llvm::TargetIRAnalysis()));
    FPM.add(llvm::createTargetTransformInfoWrapperPass(llvm::TargetIRAnalysis()));
  }
  */

  populateWithOLevel(PM, FPM);
}

void OptimizationSequence::populateWithOLevel(llvm::legacy::PassManager &PM, 
    llvm::legacy::FunctionPassManager &FPM) {
  if (OLevel == OptLevel::None) return;

  llvm::PassManagerBuilder Builder;
  if (OLevel == OptLevel::Os || OLevel == OptLevel::Oz) {
    Builder.OptLevel = (int) OptLevel::O2;
    Builder.SizeLevel = (int) OLevel - (int) OptLevel::O3;
  } else {
    Builder.OptLevel = (int) OLevel;
    Builder.SizeLevel = 0;

    if (OLevel > OptLevel::O1) {
      Builder.Inliner = llvm::createFunctionInliningPass((int) OLevel, 0);
    } else {
      Builder.Inliner = llvm::createAlwaysInlinerPass();
    }

  }

  Builder.populateFunctionPassManager(FPM);
  Builder.populateModulePassManager(PM);
}

void OptimizationSequence::print(std::ostream &Out) {
  YAMLWrapper::print(*this, Out);
}

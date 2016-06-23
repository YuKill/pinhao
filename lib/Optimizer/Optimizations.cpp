/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Optimizations.cpp
 */

#include "pinhao/InitializationRoutines.h"
#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationSet.h"
#include "pinhao/Optimizer/OptimizationSequence.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/CommandFlags.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

using namespace pinhao;

void pinhao::initializeOptimizer() {
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();

  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  llvm::initializeCore(*Registry);
  llvm::initializeScalarOpts(*Registry);
  llvm::initializeObjCARCOpts(*Registry);
  llvm::initializeVectorization(*Registry);
  llvm::initializeIPO(*Registry);
  llvm::initializeAnalysis(*Registry);
  llvm::initializeIPA(*Registry);
  llvm::initializeTransformUtils(*Registry);
  llvm::initializeInstCombine(*Registry);
  llvm::initializeInstrumentation(*Registry);
  llvm::initializeTarget(*Registry);
  llvm::initializeCodeGen(*Registry);

  llvm::initializeCodeGenPreparePass(*Registry);
  llvm::initializeAtomicExpandPass(*Registry);
  llvm::initializeRewriteSymbolsPass(*Registry);
  llvm::initializeWinEHPreparePass(*Registry);
  llvm::initializeDwarfEHPreparePass(*Registry);
  llvm::initializeSjLjEHPreparePass(*Registry);
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

llvm::Module *pinhao::applyOptimizations(llvm::Module &Module, OptimizationSequence *Sequence) {
  std::string TmpName = ".tmp-" + std::to_string(time(0));

  fflush(stdout);
  pid_t Pid = fork();

  if (Pid == 0) {
    llvm::legacy::PassManager PM; 
    llvm::legacy::FunctionPassManager FPM(&Module);

    // Populating with target machine analysis pass.
    llvm::Triple ModuleTriple(Module.getTargetTriple());
    std::string CPUStr, FeaturesStr;
    llvm::TargetMachine *Machine = nullptr;
    const llvm::TargetOptions Options = InitTargetOptionsFromCodeGenFlags();

    if (ModuleTriple.getArch()) {
      CPUStr = getCPUStr();
      FeaturesStr = getFeaturesStr();
      Machine = GetTargetMachine(ModuleTriple, CPUStr, FeaturesStr, 
          Options, Sequence->OLevel);
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

    // Populating OLevel specific optimizations.
    Sequence->populateWithOLevel(PM, FPM);

    FPM.doInitialization();
    for (auto &F : Module)
      FPM.run(F);
    FPM.doFinalization();

    // Populating with Sequence.
    Sequence->populatePassManager(PM);
    PM.run(Module);

    printModule(&Module, TmpName);

    auto ReadCheck = readModule(TmpName);
    if (!ReadCheck)
      exit(1);

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
    llvm::legacy::PassManager PM;
    llvm::legacy::FunctionPassManager FPM(&Module);

    // Populating with target machine analysis pass.
    llvm::Triple ModuleTriple(Module.getTargetTriple());
    std::string CPUStr, FeaturesStr;
    llvm::TargetMachine *Machine = nullptr;
    const llvm::TargetOptions Options = InitTargetOptionsFromCodeGenFlags();

    if (ModuleTriple.getArch()) {
      CPUStr = getCPUStr();
      FeaturesStr = getFeaturesStr();
      Machine = GetTargetMachine(ModuleTriple, CPUStr, FeaturesStr, 
          Options, Sequence->OLevel);
    }

    std::unique_ptr<llvm::TargetMachine> TM(Machine);
    setFunctionAttributes(CPUStr, FeaturesStr, Module);

    if (TM) {
      FPM.add(llvm::createTargetTransformInfoWrapperPass(TM->getTargetIRAnalysis()));
    } else {
      FPM.add(llvm::createTargetTransformInfoWrapperPass(llvm::TargetIRAnalysis()));
    }

    Sequence->populateFunctionPassManager(FPM);

    FPM.doInitialization();
    FPM.run(*Function);
    FPM.doFinalization();

    printModule(&Module, TmpName);

    auto ReadCheck = readModule(TmpName);
    if (!ReadCheck)
      exit(1);

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

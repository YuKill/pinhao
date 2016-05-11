/*-------------------------- PINHAO project --------------------------*/

/**
 * @file JITExecutor.cpp
 */

#include "pinhao/Support/JITExecutor.h"

#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/TargetSelect.h"

using namespace pinhao;

void pinhao::initializeJITExecutor() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
}

JITExecutor::JITExecutor(llvm::Module &M) {
  Mod = llvm::CloneModule(&M);
  assert(Mod != nullptr && "Module nil.");
  Engine = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(Mod)).create();
  Engine->finalizeObject();
}

JITExecutor::~JITExecutor() {
  delete Engine;
}

int JITExecutor::run(std::vector<std::string> &Argv, char *const *Env) {
  return Engine->runFunctionAsMain(Mod->getFunction("main"), Argv, Env);
}


void JITExecutor::flushCache() {
  int Doubles = 30 * 1024 * 1024 / sizeof(double); // 30MB
  double Count = 0.0;

  double *Block = (double*) calloc(Doubles, sizeof(double));
  for (int I = 0; I < Doubles; ++I)
    Count += Block[I];
  free(Block);
}

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file JITExecutor.h
 * @brief This file defines a class that executes a module.
 */

#ifndef PINHAO_JIT_EXECUTOR_H
#define PINHAO_JIT_EXECUTOR_H

#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/TargetSelect.h"

#include <vector>

using namespace llvm;

namespace pinhao {

  /**
   * @brief Runs a given module at runtime.
   */
  class JITExecutor {

    Module *Mod;
    ExecutionEngine *Engine;

    public:

    JITExecutor(Module &M) {
      Mod = CloneModule(&M);
      assert(Mod != nullptr && "Module nil.");
      Engine = EngineBuilder(std::unique_ptr<Module>(Mod)).create();
      Engine->finalizeObject(); 
    }

    void run(std::vector<std::string> &Argv, char *const *Env) {
      Engine->runFunctionAsMain(Mod->getFunction("main"), Argv, Env);
    }


    void flushCache() {
      int Doubles = 30 * 1024 * 1024 / sizeof(double); // 30MB
      double Count = 0.0;

      double *Block = (double*) calloc(Doubles, sizeof(double));
      for (int I = 0; I < Doubles; ++I)
        Count += Block[I];
      free(Block);
    }

    ~JITExecutor() {
      delete Engine;
    }

    static void init() {
      InitializeNativeTarget();
      InitializeNativeTargetAsmPrinter();
      InitializeNativeTargetAsmParser();
    }

  };

}

#endif



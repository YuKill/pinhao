/*-------------------------- PINHAO project --------------------------*/

/**
 * @file JITExecutor.h
 * @brief This file defines a class that executes a module.
 */

#ifndef PINHAO_JIT_EXECUTOR_H
#define PINHAO_JIT_EXECUTOR_H

#include "pinhao/InitializationRoutines.h"

#include "llvm/ExecutionEngine/MCJIT.h"

#include <vector>

namespace pinhao {

  /// @brief Calls the llvm initialization procedures that the @a JITExecutor depends on.
  void initializeJITExecutor();

  /**
   * @brief Runs a given module at runtime.
   */
  class JITExecutor {
    private:
      llvm::Module *Mod;
      llvm::ExecutionEngine *Engine;

    public:
      /**
       * @brief Whenever a @a JITExecutor object is constructed, it copies the
       * @a llvm::Module provided, and creates an @a ExecutionEngine based on that.
       */
      JITExecutor(llvm::Module &M); 
      ~JITExecutor(); 

      /**
       * @brief Runs the module copied, with args @a Argv and enviroment variables
       * @a Env.
       */
      int run(std::vector<std::string> &Argv, char *const *Env); 

      /// @brief Fill the cache (30MB) with zeroes.
      void flushCache(); 

  };

}

#endif



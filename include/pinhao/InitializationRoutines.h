/*-------------------------- PINHAO project --------------------------*/

/**
 * @file InitializationRoutines.h
 */

#ifndef PINHAO_INITIALIZATION_ROUTINES_H
#define PINHAO_INITIALIZATION_ROUTINES_H

#include "llvm/IR/Module.h"

namespace pinhao {

  void initializeJITExecutor();

  void initializeOptimizer();

  void initializeFeatures(llvm::Module&);

  void initializeDynamicFeatures(llvm::Module&);
  void initializeGEOSFeatures(llvm::Module&);
  void initializeGEOSBasicBlockFreqFeature(llvm::Module&);
  void initializePAPIMultFeatures();

  void initializeStaticFeatures();
  void initializeCFGBasicBlockStaticFeatures();
  void initializeCFGFunctionStaticFeatures();
  void initializeCFGModuleStaticFeatures();
  void initializeFunctionsGeneFeature();
  void initializeFunctionStaticCostFeature();

}

#endif

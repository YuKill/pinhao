/*-------------------------- PINHAO project --------------------------*/

/**
 * @file InitializeRoutines.h
 */

#ifndef PINHAO_INITIALIZE_ROUTINES_H
#define PINHAO_INITIALIZE_ROUTINES_H

#include "llvm/IR/Module.h"

namespace pinhao {

  void initializeOptimizer();

  void initializeGEOSFeatures(llvm::Module&);
  void initializeGEOSBasicBlockFreqFeature(llvm::Module&);

}

#endif

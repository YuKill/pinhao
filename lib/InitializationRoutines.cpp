/*-------------------------- PINHAO project --------------------------*/

/**
 * @file InitializationRoutines.cpp
 */

#include "pinhao/InitializationRoutines.h"

using namespace pinhao;

void pinhao::initializeFeatures(llvm::Module &Module) {
  initializeDynamicFeatures(Module);
  initializeStaticFeatures();
}

void pinhao::initializeDynamicFeatures(llvm::Module &Module) {
  initializeGEOSFeatures(Module);
  initializeGEOSBasicBlockFreqFeature(Module);
  initializePAPIMultFeatures();
}

void pinhao::initializeStaticFeatures() {
  initializeCFGBasicBlockStaticFeatures();
  initializeCFGFunctionStaticFeatures();
  initializeCFGModuleStaticFeatures();
  initializeFunctionsGeneFeature();
  initializeFunctionStaticCostFeature();
}

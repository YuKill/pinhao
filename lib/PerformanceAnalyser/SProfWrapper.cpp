/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SProfWrapper.cpp
 */

#include "pinhao/PerformanceAnalyser/SProfWrapper.h"
#include "pinhao/Support/YamlOptions.h"
#include "pinhao/InitializationRoutines.h"

#include "sprof/InitializeRoutines.h"

using namespace pinhao;

void pinhao::initializeStaticProfilerPasses(llvm::PassRegistry &Registry) {
  llvm::initializeStaticProfilerPasses(Registry);
}

llvm::Pass *SProfWrapper::getSProfPass(StaticModuleCostPtr SMC) {
  return new llvm::StaticModuleCostPass(&SMC);
}

std::shared_ptr<llvm::StaticModuleCost> SProfWrapper::runSProfPass(llvm::Module &Module) {
  StaticModuleCostPtr SMC(new llvm::StaticModuleCost());
  llvm::legacy::PassManager PM; 
  PM.add(getSProfPass(SMC));
  PM.run(Module);
  return SMC;
}

double SProfWrapper::getFunctionCost(llvm::Module &Module, llvm::Function &Function) {
  StaticModuleCostPtr SMC = runSProfPass(Module);
  return SMC->Cost[&Function];
}

double SProfWrapper::getModuleCost(llvm::Module &Module) {
  StaticModuleCostPtr SMC = runSProfPass(Module);
  std::cerr << "ModuleCost: " << SMC->ModuleCost << std::endl;
  return SMC->ModuleCost;
}

SProfWrapper::CostMap SProfWrapper::getAllFunctionsCost(llvm::Module &Module) {
  StaticModuleCostPtr SMC = runSProfPass(Module);
  return SMC->Cost;
}


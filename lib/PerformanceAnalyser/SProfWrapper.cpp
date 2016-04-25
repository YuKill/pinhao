/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SProfWrapper.cpp
 */

#include "pinhao/PerformanceAnalyser/SProfWrapper.h"
#include "pinhao/Support/YamlOptions.h"

using namespace pinhao;

llvm::Pass *SProfWrapper::getSProfPass(StaticFunctionCostPtr SFC) {
  return new llvm::StaticFunctionCostPass(&SFC);
}

double SProfWrapper::sumAllFunctionsCost(SProfWrapper::CostMap &Map) {
  double TotalCost = 0.0;
  for (auto &Pair : Map)
    TotalCost += Pair.second;
  return TotalCost;
}

double SProfWrapper::getFunctionCost(llvm::Module &Module, llvm::Function &Function) {
  StaticFunctionCostPtr SFC(new llvm::StaticFunctionCost());
  llvm::legacy::FunctionPassManager FPM(&Module); 
  FPM.add(getSProfPass(SFC));
  FPM.run(Function);
  return SFC->Cost[&Function];
}

double SProfWrapper::getModuleCost(llvm::Module &Module) {
  CostMap Map = getAllFunctionsCost(Module);
  return sumAllFunctionsCost(Map);
}

SProfWrapper::CostMap SProfWrapper::getAllFunctionsCost(llvm::Module &Module) {
  StaticFunctionCostPtr SFC(new llvm::StaticFunctionCost());
  llvm::legacy::PassManager PM;
  PM.add(getSProfPass(SFC));
  PM.run(Module);
  return SFC->Cost;
}

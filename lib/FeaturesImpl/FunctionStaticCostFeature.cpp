/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FunctionsStaticCostFeature.cpp
 */

#include "pinhao/Features/MapFeature.h"
#include "pinhao/PerformanceAnalyser/SProfWrapper.h"

#include "llvm/IR/LegacyPassManager.h"

using namespace pinhao;

namespace {

  class FunctionStaticCostFeature : public MapFeature<std::string, double> {
    public:
      ~FunctionStaticCostFeature() {}
      FunctionStaticCostFeature(FeatureInfo *Info) : MapFeature<std::string, double>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;
  };

}

void FunctionStaticCostFeature::processModule(llvm::Module &Module) {
  if (this->isProcessed()) return;
  Processed = true;

  SProfWrapper::CostMap CostMap = SProfWrapper::getAllFunctionsCost(Module);

  uint64_t NamelessCount = 0;
  const std::string Nameless("Nameless");
  for (auto &Pair : CostMap) {
    std::string FunctionName = (Pair.first->getName().str() == "") ?
      Nameless + std::to_string(NamelessCount++) : Pair.first->getName().str();

    setValueOfKey(this->getName(), Pair.second, FunctionName);
  }
}

std::unique_ptr<Feature> FunctionStaticCostFeature::clone() const {
  FunctionStaticCostFeature *Clone = new FunctionStaticCostFeature(*this);
  return std::unique_ptr<Feature>(Clone);
} 

static RegisterFeature<FunctionStaticCostFeature> 
X(new FeatureInfo("static-cost", "Calculates a static cost of a function.", ValueType::Float, FeatureInfo::Static));

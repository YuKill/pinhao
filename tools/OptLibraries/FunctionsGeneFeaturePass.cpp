/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FunctionsGeneFeaturePass.cpp
 */

#include "pinhao/Features/Feature.h"

#include "llvm/Pass.h"

#include <vector>

using namespace llvm;
using namespace pinhao;

namespace {

  class FunctionsGeneFeaturePass : public ModulePass {
    private:
      std::shared_ptr<Feature> GeneFeature;

    public:
      static char ID;
      FunctionsGeneFeaturePass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) const override {
        Info.setPreservesAll(); 
      }

      std::shared_ptr<Feature> getFeature() { return GeneFeature; }

      bool runOnModule(Module &M) override;
  };
  
}

bool FunctionsGeneFeaturePass::runOnModule(Module &M) {
  GeneFeature.reset(FeatureRegistry::get("function-dna").release());
  GeneFeature->processModule(M); 
  return false;
}

char FunctionsGeneFeaturePass::ID = 0;
static RegisterPass<FunctionsGeneFeaturePass> X("dna-fn", "Extract Function DNA", false, false);

namespace {

  class FunctionsGeneFeaturePrinterPass : public ModulePass {
    public:
      static char ID;
      FunctionsGeneFeaturePrinterPass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) const override {
        Info.setPreservesAll(); 
        Info.addRequired<FunctionsGeneFeaturePass>();
      }
  
      bool runOnModule(Module &M) override;
  };

}

bool FunctionsGeneFeaturePrinterPass::runOnModule(Module &M) {
  std::shared_ptr<Feature> GeneFeature = getAnalysis<FunctionsGeneFeaturePass>().getFeature();
  GeneFeature->printYaml(std::cerr);
  return false;
}

char FunctionsGeneFeaturePrinterPass::ID = 0;
static RegisterPass<FunctionsGeneFeaturePrinterPass> Y("dna-fnprint", "Prints gene information collected.", false, false);

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGStaticFeaturesPass.cpp
 * @brief This file implements the @a CFGStaticFeaturesPass class.
 */

#include "pinhao/Features/Feature.h"

#include "llvm/Pass.h"

#include <vector>

using namespace llvm;
using namespace pinhao;

extern std::vector<std::pair<std::shared_ptr<Feature>, std::shared_ptr<Feature>>> CFGStaticFeaturesCollection;

namespace {

  class CFGStaticFeaturesPass : public ModulePass {
    private:
      std::vector<std::shared_ptr<Feature>> CFGFeatures;

    public:
      static char ID;
      CFGStaticFeaturesPass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) const override {
        Info.setPreservesAll(); 
      }

      uint64_t getVectorSize() { return CFGFeatures.size(); }
      std::shared_ptr<Feature> getNthFeature(int N) { return CFGFeatures[N]; }
  
      bool runOnModule(Module &M) override;
  };
  
}

bool CFGStaticFeaturesPass::runOnModule(Module &M) {
  CFGFeatures.push_back(FeatureRegistry::get("cfg_fn_static"));
  CFGFeatures.push_back(FeatureRegistry::get("cfg_md_static"));
  for (auto &F : CFGFeatures) {
    F->processModule(M); 
  }
  CFGStaticFeaturesCollection.push_back(std::make_pair(CFGFeatures[0], CFGFeatures[1]));
  return false;
}

char CFGStaticFeaturesPass::ID = 0;
static RegisterPass<CFGStaticFeaturesPass> X("cfg-sfeat", "Collects the static features of the CFG.", false, false);

namespace {

  class CFGStaticFeaturesPrinterPass : public ModulePass {
    public:
      static char ID;
      CFGStaticFeaturesPrinterPass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) const override {
        Info.setPreservesAll(); 
        Info.addRequired<CFGStaticFeaturesPass>();
      }
  
      bool runOnModule(Module &M) override;
  };

}

bool CFGStaticFeaturesPrinterPass::runOnModule(Module &M) {
  CFGStaticFeaturesPass &StaticFeatures = getAnalysis<CFGStaticFeaturesPass>();
  for (uint64_t I = 0, E = StaticFeatures.getVectorSize(); I < E; ++I) {
    StaticFeatures.getNthFeature(I)->printYaml(std::cerr);
  }
  return false;
}

char CFGStaticFeaturesPrinterPass::ID = 0;
static RegisterPass<CFGStaticFeaturesPrinterPass> Y("cfg-sfprint", "Prints the static features of the CFG collected.", false, false);

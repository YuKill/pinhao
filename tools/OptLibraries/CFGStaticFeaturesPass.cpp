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

namespace {

  class CFGStaticFeaturesPass : public ModulePass {
    private:
      std::vector<std::unique_ptr<Feature>> CFGFeatures;

    public:
      static char ID;
      CFGStaticFeaturesPass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) {
        Info.setPreservesAll(); 
      }
  
      bool runOnModule(Module &M) override;
  };
  
}

bool CFGStaticFeaturesPass::runOnModule(Module &M) {
  CFGFeatures.push_back(FeatureRegistry::get("cfg_fn_static")); 
  CFGFeatures.push_back(FeatureRegistry::get("cfg_md_static")); 
  for (auto &F : CFGFeatures) {
    F->processModule(M); 
    F->printYaml(std::cerr);
  }
  return false;
}

char CFGStaticFeaturesPass::ID = 0;
static RegisterPass<CFGStaticFeaturesPass> X("cfg-sfeat", "Collects the static features of the CFG.", false, false);

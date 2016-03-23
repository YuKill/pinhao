/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGStaticFeaturesDiffPass.cpp
 * @brief This file implements the @a CFGStaticFeaturesDiffPass class.
 * @details It is a llvm::Pass that prints the differences between two
 * data collections stored in @a CFGStaticFeaturesCollection.
 */

#include "pinhao/Features/Features.h"

#include "llvm/Pass.h"

#include <vector>

using namespace llvm;
using namespace pinhao;

std::vector<std::pair<std::shared_ptr<Feature>, std::shared_ptr<Feature>>> CFGStaticFeaturesCollection;

namespace {

  class CFGStaticFeaturesDiffPass : public ModulePass {

    public:
      static char ID;
      CFGStaticFeaturesDiffPass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) const override {
        Info.setPreservesAll(); 
      }
  
      bool runOnModule(Module &M) override;
  };
  
}

bool CFGStaticFeaturesDiffPass::runOnModule(Module &M) {
  YAML::Emitter Emitter(std::cerr);
  if (CFGStaticFeaturesCollection.size() > 1) {
    uint64_t Count = 0;
    for (auto I = CFGStaticFeaturesCollection.begin() + 1, E = CFGStaticFeaturesCollection.end();
        I != E; ++I) {
      Emitter << YAML::BeginMap;
      Emitter << YAML::Comment(std::string("## Difference: ") + std::to_string(Count) + std::string("x") + std::to_string(Count+1));
      ++Count;

      MappedFeature<std::string, uint64_t> *FunctionFeatures[] = { 
        static_cast<MappedFeature<std::string, uint64_t>*>((I-1)->first.get()), 
        static_cast<MappedFeature<std::string, uint64_t>*>(I->first.get()) 
      }; 
      uint64_t NamelessCount = 0;
      for (auto &F : M) {
        std::string FunctionName = F.getName();
        if (FunctionName == "") FunctionName = "Nameless" + std::to_string(NamelessCount++); 
        if (F.getBasicBlockList().size() <= 0) continue;
        if (!FunctionFeatures[0]->hasKey(FunctionName) && !FunctionFeatures[1]->hasKey(FunctionName)) continue;
        Emitter << YAML::Key << FunctionName;
        Emitter << YAML::Value << YAML::BeginMap;
        for (auto &InfoPair : *FunctionFeatures[0]) {
          int64_t ValueOne = FunctionFeatures[0]->getValueOfKey(InfoPair.first, FunctionName); 
          int64_t ValueTwo = FunctionFeatures[1]->getValueOfKey(InfoPair.first, FunctionName); 
          if (ValueOne == ValueTwo) continue;
          int64_t Diff = ValueTwo - ValueOne;
          Emitter << YAML::Key << InfoPair.first;
          Emitter << YAML::Value << std::string((Diff > 0) ? "+" : "") + std::to_string(Diff);
        }
        Emitter << YAML::EndMap;
      }

      Emitter << YAML::Key << "module";
      Emitter << YAML::Value << YAML::BeginMap;
      LinearFeature<uint64_t> *ModuleFeatures[] = {
        static_cast<LinearFeature<uint64_t>*>((I-1)->second.get()), 
        static_cast<LinearFeature<uint64_t>*>(I->second.get()) 
      };
      for (auto &InfoPair : *ModuleFeatures[0]) {
        int64_t ValueOne = ModuleFeatures[0]->getValueOf(InfoPair.first); 
        int64_t ValueTwo = ModuleFeatures[1]->getValueOf(InfoPair.first); 
        if (ValueOne == ValueTwo) continue;
        int64_t Diff = ValueTwo - ValueOne;
        Emitter << YAML::Key << InfoPair.first;
        Emitter << YAML::Value << std::string((Diff > 0) ? "+" : "") + std::to_string(Diff);
      }
      Emitter << YAML::EndMap;

      Emitter << YAML::EndMap;
      std::cerr << std::endl;
    }
  }
  return false;
}

char CFGStaticFeaturesDiffPass::ID = 0;
static RegisterPass<CFGStaticFeaturesDiffPass> X("cfg-sfdiff", 
    "Prints the difference between each consecutive static features collections.", false, false);

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGStaticFeaturesGenPass.cpp
 * @brief This class generates a cfg (control flow graph) with the static
 * features, based on the cfg, collected.
 */

#include "pinhao/Features/Features.h"

#include "llvm/Pass.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

#include <vector>
#include <unordered_map>

using namespace llvm;
using namespace pinhao;


namespace {

  class CFGStaticFeaturesGenPass : public ModulePass {
    private:
      std::unordered_map<llvm::BasicBlock*, std::string> Name;

      std::string getName(llvm::BasicBlock *BB) {
        if (Name.count(BB) == 0)
          Name[BB] = "BasicBlock" + std::to_string((unsigned long)BB);
        return Name[BB];
      }

    public:
      static char ID;
      CFGStaticFeaturesGenPass() : ModulePass(ID) {}

      void getAnalysisUsage(AnalysisUsage &Info) const override {
        Info.setPreservesAll(); 
      }

      bool runOnModule(Module &M) override;
  };

}

bool CFGStaticFeaturesGenPass::runOnModule(Module &M) {
  std::unique_ptr<MappedFeature<void*, uint64_t>> BBFeature;

  Feature *Tmp = FeatureRegistry::get("cfg_bb_static").release();
  BBFeature.reset(static_cast<MappedFeature<void*, uint64_t>*>(Tmp));

  BBFeature->processModule(M);

  int RunId    = rand();
  int Nameless = 0;

  for (auto &F : M) {
    if (F.getBasicBlockList().size() == 0) continue;
    std::error_code ErrInfo;
    std::string FunctionName = (F.getName().str().length() == 0) ? 
      std::string("Nameless") + std::to_string(Nameless++) : F.getName().str(); 

    std::string Filename = "graph-" + FunctionName + "." + std::to_string(RunId) + ".dot";
    llvm::raw_fd_ostream Stream(Filename, ErrInfo, llvm::sys::fs::OpenFlags::F_Text);

    Stream << "digraph \"Graph Features of function: '" << 
      F.getName().str() << "'\" {\n";
    for (auto &BB : F) {
      Stream << getName(&BB) << " [shape=record,label=\"{";
      for (auto &Pair : *(BBFeature.get())) {
          auto V = BBFeature->getValueOfKey(Pair.first, &BB);
          if (V) Stream << V << "::" << Pair.first << "\\l";
      }
      auto BrInst = llvm::dyn_cast<BranchInst>(BB.getTerminator());
      if (BrInst && BrInst->getNumSuccessors() > 1) Stream << " | { <V0> T | <V1> F }";

      Stream << "}\"];\n";

      for (unsigned S = 0, E = BB.getTerminator()->getNumSuccessors(); S < E; ++S) {
        Stream << "\t" <<  getName(&BB);
        if (BrInst && BrInst->getNumSuccessors() > 1) Stream << ":V" << S;
        Stream << " -> " << 
          getName(BB.getTerminator()->getSuccessor(S)) << ";\n";
      }
    }
    Stream << "}\n";
    Stream.close();
  }

  return false; 
}

char CFGStaticFeaturesGenPass::ID = 0;
static RegisterPass<CFGStaticFeaturesGenPass> X("cfg-sfgen", 
    "Generates the cfg of the functions with its static features.", false, false);

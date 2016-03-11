/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGFunctionStaticFeatures.cpp
 * @brief This file implements the @a CFGFunctionStaticFeatures class.
 */

#include "pinhao/Features/Features.h"

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"

using namespace pinhao;

namespace llvm {
  bool isCriticalEdge(const TerminatorInst*, unsigned, bool);
}

namespace {

  /**
   * @brief This class collects some static features for each Function in the Module.
   */
  class CFGFunctionStaticFeatures : public MapVectorFeature<void*, uint64_t> {
    private:

      /// @brief Gets the total of the sub-feature @a SubFeatureName of @a Key,
      /// and adds 1 to it.
      void addOneToSubFeatureOfKey(std::string SubFeatureName, void* Key) {
        uint64_t Quantity = 1 + getValueOfKey(SubFeatureName, Key);
        setValueOfKey(SubFeatureName, Quantity, Key);
      }

      /// @brief Processes a basic block of a function.
      void processBasicBlockOfFunction(llvm::BasicBlock &BasicBlock, llvm::Function *Function);

      /// @brief Gets the features collected by the @a CFGBasicBlockStaticFeatures class, and
      /// sums into the function.
      void copyFeaturesFromBB(void *BasicBlock, llvm::Function *Function);

      std::shared_ptr<Feature> BasicBlockFeatures;
      std::map<std::string, void*> FunctionPointer;

    public:
      ~CFGFunctionStaticFeatures() {}
      CFGFunctionStaticFeatures(FeatureInfo *Info, std::shared_ptr<Feature> *F = nullptr) : 
        MapVectorFeature<void*, uint64_t>(Info) {
          if (F) BasicBlockFeatures = *F; 
        }

      std::unique_ptr<Feature> clone() const override;

      /// @brief This function iterates over all basic blocks and collects
      /// information about each instruction.
      void processModule(llvm::Module &Module) override;
  };

}

void CFGFunctionStaticFeatures::copyFeaturesFromBB(void *BasicBlock, llvm::Function *Function) {
  const std::string Prefix = "bb_";
  const std::string FunctionPrefix = "fn_";
  MapVectorFeature<void*, uint64_t> *BBFeatures =
    static_cast<MapVectorFeature<void*, uint64_t>*>(BasicBlockFeatures.get());
  for (auto &Pair : *BasicBlockFeatures) {
    std::string FeatureName = FunctionPrefix + Pair.first.substr(Prefix.length(), std::string::npos);

    uint64_t BBTotal = BBFeatures->getValueOfKey(Pair.first, BasicBlock);
    uint64_t Total = BBTotal + getValueOfKey(FeatureName, Function);

    setValueOfKey(FeatureName, Total, Function);
  }
}

void CFGFunctionStaticFeatures::processBasicBlockOfFunction(llvm::BasicBlock &BasicBlock, llvm::Function *Function) {
  int NumPredecessors = 0, NumSuccessors = 0;

  NumSuccessors = BasicBlock.getTerminator()->getNumSuccessors();
  for (auto I = llvm::pred_begin(&BasicBlock), E = llvm::pred_end(&BasicBlock); I != E; ++I)
    ++NumPredecessors;

  addOneToSubFeatureOfKey("fn_bb", Function); 

  switch(NumPredecessors) {
    case 0: break;
    case 1: addOneToSubFeatureOfKey("fn_1pred_bb", Function); 
    case 2: addOneToSubFeatureOfKey("fn_2pred_bb", Function); 
    default: addOneToSubFeatureOfKey("fn_g2pred_bb", Function); 
  }

  switch(NumSuccessors) {
    case 0: break;
    case 1: addOneToSubFeatureOfKey("fn_1suc_bb", Function); 
    case 2: addOneToSubFeatureOfKey("fn_2suc_bb", Function); 
    default: addOneToSubFeatureOfKey("fn_g2suc_bb", Function); 
  }

  if (NumSuccessors == 1 && NumPredecessors == 1)
    addOneToSubFeatureOfKey("fn_1pred_1suc_bb", Function); 
  else if (NumSuccessors == 1 && NumPredecessors == 2)
    addOneToSubFeatureOfKey("fn_1pred_2suc_bb", Function); 
  else if (NumSuccessors == 2 && NumPredecessors == 1)
    addOneToSubFeatureOfKey("fn_2pred_1suc_bb", Function); 
  else if (NumSuccessors == 2 && NumPredecessors == 2)
    addOneToSubFeatureOfKey("fn_2pred_2suc_bb", Function); 
  else if (NumSuccessors > 2 && NumPredecessors > 2)
    addOneToSubFeatureOfKey("fn_g2pred_g2suc_bb", Function); 

  int TotalEdges = NumSuccessors + getValueOfKey("fn_cfg_edges", Function);
  setValueOfKey("fn_cfg_edges", TotalEdges, Function);

  for (auto I = 0; I < NumSuccessors; ++I)
    if (isCriticalEdge(BasicBlock.getTerminator(), I, true))
      addOneToSubFeatureOfKey("fn_cfg_crit_edges", Function); 

}

void CFGFunctionStaticFeatures::processModule(llvm::Module &Module) {
  if (!BasicBlockFeatures.get()) {
    BasicBlockFeatures.reset(FeatureRegistry::get("cfg_bb_static").release());
    BasicBlockFeatures->processModule(Module);
  }
  for (auto &Function : Module) {
    for (auto &BasicBlock : Function) {
      FunctionPointer[Function.getName()] = (void*) &Function;
      copyFeaturesFromBB(&BasicBlock, &Function); 
      processBasicBlockOfFunction(BasicBlock, &Function);
    } 
  }
}

std::unique_ptr<Feature> CFGFunctionStaticFeatures::clone() const {
  CFGFunctionStaticFeatures *Clone = new CFGFunctionStaticFeatures(*this);
  return std::unique_ptr<Feature>(Clone);
}

static std::map<std::string, std::string> SubFeatures = {
  { "fn_inst", "Number of Instructions" },
  { "fn_assign_inst", "Nof assignment instructions" },
  { "fn_binop_int_inst", "Nof integer binop instructions" },
  { "fn_binop_flt_inst", "Nof float binop instructions" },
  { "fn_terminator_inst", "Nof terminator instructions" },
  { "fn_binop_bitw_inst", "Nof bitwise binop instructions" },
  { "fn_vector_inst", "Nof vector instructions" },
  { "fn_memory_adress_inst", "Nof memory access and addressing instructions" },
  { "fn_aggregate_inst", "Nof aggregate instructions" },
  { "fn_conv_int_inst", "Nof integer conversion instructions" },
  { "fn_conv_flt_inst", "Nof float conversion instructions" },
  { "fn_call_inst", "Nof call instructions" },
  { "fn_callarg_ptr_inst", "Nof call instructions that has pointers as arguments" },
  { "fn_callarg_g4_inst", "Nof call instructions that have more than 4 arguments" },
  { "fn_callret_int_inst", "Nof call instructions that return an integer" },
  { "fn_callret_flt_inst", "Nof call instructions that return a float" },
  { "fn_callret_ptr_inst", "Nof call instructions that return a pointer" },
  { "fn_switch_inst", "Nof switch instructions" },
  { "fn_indirectbr_inst", "Nof indirect branches instructions" },
  { "fn_condbr_inst", "Nof conditional branches instructions" },
  { "fn_uncondbr_inst", "Nof unconditional branches instructions" },
  { "fn_load_inst", "Nof load instructions" },
  { "fn_store_inst", "Nof store instructions" },
  { "fn_getelemptr_inst", "Nof GetElemPtr instructions" },
  { "fn_phinode_inst", "Nof PHI nodes" },
  { "fn_cfg_edges", "Nof edges in a cfg" },
  { "fn_cfgcrit_edges", "Nof critical edges in a cfg" },
  { "fn_bb", "Number of BasicBlocks" },
  { "fn_1suc_bb", "Nof BBs with 1-suc" },
  { "fn_2suc_bb", "Nof BBs with 2-suc" },
  { "fn_g2suc_bb", "Nof BBs with -gt 2-suc" },
  { "fn_1pred_bb", "Nof BBs with 1-pred" },
  { "fn_2pred_bb", "Nof BBs with 2-pred" },
  { "fn_g2pred_bb", "Nof BBs with -gt 2-pred" },
  { "fn_1pred_1suc_bb", "Nof BBs with 1-pred/1-suc" },
  { "fn_1pred_2suc_bb", "Nof BBs with 1-pred/2-suc" },
  { "fn_2pred_1suc_bb", "Nof BBs with 2-pred/1-suc" },
  { "fn_2pred_2suc_bb", "Nof BBs with 2-pred/2-suc" },
  { "fn_g2pred_g2suc_bb", "Nof BBs with -gt 2-pred/ -gt 2-suc" },
  { "fn_l15inst_bb", "Nof BBs -lt 15 instructions" },
  { "fn_ge15le500inst_bb", "Nof BBs -ge 15 -le 500 instructions"},
  { "fn_g500inst_bb", "Nof BBs -gt 500 instructions" }
};

static RegisterFeature<CFGFunctionStaticFeatures> 
X(new CompositeFeatureInfo("cfg_fn_static", "Static Information of Functions at CFG", 
      FeatureInfo::IntType, FeatureInfo::Static, SubFeatures));

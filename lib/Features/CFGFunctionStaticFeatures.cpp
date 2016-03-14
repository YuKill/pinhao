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
  MapVectorFeature<void*, uint64_t> *BBFeatures =
    static_cast<MapVectorFeature<void*, uint64_t>*>(BasicBlockFeatures.get());
  for (auto &Pair : *BasicBlockFeatures) {
    std::string FeatureName = Pair.first;

    uint64_t BBTotal = BBFeatures->getValueOfKey(FeatureName, BasicBlock);
    uint64_t Total = BBTotal + getValueOfKey(FeatureName, Function);

    setValueOfKey(FeatureName, Total, Function);
  }
}

void CFGFunctionStaticFeatures::processBasicBlockOfFunction(llvm::BasicBlock &BasicBlock, llvm::Function *Function) {
  int NumPredecessors = 0, NumSuccessors = 0;

  NumSuccessors = BasicBlock.getTerminator()->getNumSuccessors();
  for (auto I = llvm::pred_begin(&BasicBlock), E = llvm::pred_end(&BasicBlock); I != E; ++I)
    ++NumPredecessors;

  addOneToSubFeatureOfKey("nof_bb", Function); 

  switch(NumPredecessors) {
    case 0: break;
    case 1: addOneToSubFeatureOfKey("nof_1pred_bb", Function); 
    case 2: addOneToSubFeatureOfKey("nof_2pred_bb", Function); 
    default: addOneToSubFeatureOfKey("nof_g2pred_bb", Function); 
  }

  switch(NumSuccessors) {
    case 0: break;
    case 1: addOneToSubFeatureOfKey("nof_1suc_bb", Function); 
    case 2: addOneToSubFeatureOfKey("nof_2suc_bb", Function); 
    default: addOneToSubFeatureOfKey("nof_g2suc_bb", Function); 
  }

  if (NumSuccessors == 1 && NumPredecessors == 1)
    addOneToSubFeatureOfKey("nof_1pred_1suc_bb", Function); 
  else if (NumSuccessors == 1 && NumPredecessors == 2)
    addOneToSubFeatureOfKey("nof_1pred_2suc_bb", Function); 
  else if (NumSuccessors == 2 && NumPredecessors == 1)
    addOneToSubFeatureOfKey("nof_2pred_1suc_bb", Function); 
  else if (NumSuccessors == 2 && NumPredecessors == 2)
    addOneToSubFeatureOfKey("nof_2pred_2suc_bb", Function); 
  else if (NumSuccessors > 2 && NumPredecessors > 2)
    addOneToSubFeatureOfKey("nof_g2pred_g2suc_bb", Function); 

  int TotalEdges = NumSuccessors + getValueOfKey("nof_cfg_edges", Function);
  setValueOfKey("nof_cfg_edges", TotalEdges, Function);

  for (auto I = 0; I < NumSuccessors; ++I)
    if (isCriticalEdge(BasicBlock.getTerminator(), I, true))
      addOneToSubFeatureOfKey("nof_cfg_crit_edges", Function); 

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
  { "nof_inst", "Number of Instructions" },
  { "nof_assign_inst", "Nof assignment instructions" },
  { "nof_binop_int_inst", "Nof integer binop instructions" },
  { "nof_binop_flt_inst", "Nof float binop instructions" },
  { "nof_terminator_inst", "Nof terminator instructions" },
  { "nof_binop_bitw_inst", "Nof bitwise binop instructions" },
  { "nof_vector_inst", "Nof vector instructions" },
  { "nof_memory_adress_inst", "Nof memory access and addressing instructions" },
  { "nof_aggregate_inst", "Nof aggregate instructions" },
  { "nof_conv_int_inst", "Nof integer conversion instructions" },
  { "nof_conv_flt_inst", "Nof float conversion instructions" },
  { "nof_call_inst", "Nof call instructions" },
  { "nof_callarg_ptr_inst", "Nof call instructions that has pointers as arguments" },
  { "nof_callarg_g4_inst", "Nof call instructions that have more than 4 arguments" },
  { "nof_callret_int_inst", "Nof call instructions that return an integer" },
  { "nof_callret_flt_inst", "Nof call instructions that return a float" },
  { "nof_callret_ptr_inst", "Nof call instructions that return a pointer" },
  { "nof_switch_inst", "Nof switch instructions" },
  { "nof_indirectbr_inst", "Nof indirect branches instructions" },
  { "nof_condbr_inst", "Nof conditional branches instructions" },
  { "nof_uncondbr_inst", "Nof unconditional branches instructions" },
  { "nof_load_inst", "Nof load instructions" },
  { "nof_store_inst", "Nof store instructions" },
  { "nof_getelemptr_inst", "Nof GetElemPtr instructions" },
  { "nof_phinode_inst", "Nof PHI nodes" },
  { "nof_cfg_edges", "Nof edges in a cfg" },
  { "nof_cfgcrit_edges", "Nof critical edges in a cfg" },
  { "nof_bb", "Number of BasicBlocks" },
  { "nof_1suc_bb", "Nof BBs with 1-suc" },
  { "nof_2suc_bb", "Nof BBs with 2-suc" },
  { "nof_g2suc_bb", "Nof BBs with -gt 2-suc" },
  { "nof_1pred_bb", "Nof BBs with 1-pred" },
  { "nof_2pred_bb", "Nof BBs with 2-pred" },
  { "nof_g2pred_bb", "Nof BBs with -gt 2-pred" },
  { "nof_1pred_1suc_bb", "Nof BBs with 1-pred/1-suc" },
  { "nof_1pred_2suc_bb", "Nof BBs with 1-pred/2-suc" },
  { "nof_2pred_1suc_bb", "Nof BBs with 2-pred/1-suc" },
  { "nof_2pred_2suc_bb", "Nof BBs with 2-pred/2-suc" },
  { "nof_g2pred_g2suc_bb", "Nof BBs with -gt 2-pred/ -gt 2-suc" },
  { "nof_l15inst_bb", "Nof BBs -lt 15 instructions" },
  { "nof_ge15le500inst_bb", "Nof BBs -ge 15 -le 500 instructions"},
  { "nof_g500inst_bb", "Nof BBs -gt 500 instructions" }
};

static RegisterFeature<CFGFunctionStaticFeatures> 
X(new CompositeFeatureInfo("cfg_fn_static", "Static Information of Functions at CFG", 
      FeatureInfo::IntType, FeatureInfo::Static, SubFeatures));

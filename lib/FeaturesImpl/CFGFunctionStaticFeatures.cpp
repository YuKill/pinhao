/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGFunctionStaticFeatures.cpp
 * @brief This file implements the @a CFGFunctionStaticFeatures class.
 */

#include "pinhao/Features/MapVectorFeature.h"

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"

using namespace pinhao;

namespace llvm {
  bool isCriticalEdge(const TerminatorInst*, unsigned, bool);
}

namespace {

  class CFGFunctionStaticFeatures : public MapVectorFeature<std::string, uint64_t> {
    private:

      void addOneToSubFeatureOfKey(std::string SubFeatureName, std::string Key) {
        uint64_t Quantity = 1 + getValueOfKey(SubFeatureName, Key);
        setValueOfKey(SubFeatureName, Quantity, Key);
      }

      void processBasicBlockOfFunction(llvm::BasicBlock &BasicBlock, std::string FunctionName);

      void copyFeaturesFromBB(void *BasicBlock, std::string FunctionName);

      std::shared_ptr<Feature> BasicBlockFeatures;
      uint64_t NamelessCount;

    public:
      ~CFGFunctionStaticFeatures() {}
      CFGFunctionStaticFeatures(FeatureInfo *Info, std::shared_ptr<Feature> *F = nullptr) : 
        MapVectorFeature<std::string, uint64_t>(Info) {
          if (F) BasicBlockFeatures = *F; 
        }

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;
  };

}

void CFGFunctionStaticFeatures::copyFeaturesFromBB(void *BasicBlock, std::string FunctionName) {
  MapVectorFeature<void*, uint64_t> *BBFeatures =
    static_cast<MapVectorFeature<void*, uint64_t>*>(BasicBlockFeatures.get());
  for (auto &Pair : *BasicBlockFeatures) {
    std::string FeatureName = Pair.first;

    uint64_t BBTotal = BBFeatures->getValueOfKey(FeatureName, BasicBlock);
    uint64_t Total = BBTotal + getValueOfKey(FeatureName, FunctionName);

    setValueOfKey(FeatureName, Total, FunctionName);
  }
}

void CFGFunctionStaticFeatures::processBasicBlockOfFunction(llvm::BasicBlock &BasicBlock, std::string FunctionName) {
  int NumPredecessors = 0, NumSuccessors = 0;

  NumSuccessors = BasicBlock.getTerminator()->getNumSuccessors();
  for (auto I = llvm::pred_begin(&BasicBlock), E = llvm::pred_end(&BasicBlock); I != E; ++I)
    ++NumPredecessors;

  addOneToSubFeatureOfKey("nof_bb", FunctionName); 

  switch(NumPredecessors) {
    case 0: break;
    case 1: addOneToSubFeatureOfKey("nof_1pred_bb", FunctionName); 
    case 2: addOneToSubFeatureOfKey("nof_2pred_bb", FunctionName); 
    default: addOneToSubFeatureOfKey("nof_g2pred_bb", FunctionName); 
  }

  switch(NumSuccessors) {
    case 0: break;
    case 1: addOneToSubFeatureOfKey("nof_1suc_bb", FunctionName); 
    case 2: addOneToSubFeatureOfKey("nof_2suc_bb", FunctionName); 
    default: addOneToSubFeatureOfKey("nof_g2suc_bb", FunctionName); 
  }

  if (NumSuccessors == 1 && NumPredecessors == 1)
    addOneToSubFeatureOfKey("nof_1pred_1suc_bb", FunctionName); 
  else if (NumSuccessors == 1 && NumPredecessors == 2)
    addOneToSubFeatureOfKey("nof_1pred_2suc_bb", FunctionName); 
  else if (NumSuccessors == 2 && NumPredecessors == 1)
    addOneToSubFeatureOfKey("nof_2pred_1suc_bb", FunctionName); 
  else if (NumSuccessors == 2 && NumPredecessors == 2)
    addOneToSubFeatureOfKey("nof_2pred_2suc_bb", FunctionName); 
  else if (NumSuccessors > 2 && NumPredecessors > 2)
    addOneToSubFeatureOfKey("nof_g2pred_g2suc_bb", FunctionName); 

  int TotalEdges = NumSuccessors + getValueOfKey("nof_cfg_edges", FunctionName);
  setValueOfKey("nof_cfg_edges", TotalEdges, FunctionName);

  for (auto I = 0; I < NumSuccessors; ++I)
    if (isCriticalEdge(BasicBlock.getTerminator(), I, true))
      addOneToSubFeatureOfKey("nof_cfg_crit_edges", FunctionName); 

}

void CFGFunctionStaticFeatures::processModule(llvm::Module &Module) {
  if (this->isProcessed()) return;
  Processed = true;

  NamelessCount = 0;
  if (!BasicBlockFeatures.get()) {
    BasicBlockFeatures.reset(FeatureRegistry::get("cfg_bb_static").release());
    BasicBlockFeatures->processModule(Module);
  }
  for (auto &Function : Module) {
    std::string FunctionName = Function.getName();
    if (FunctionName == "") 
      FunctionName = "Nameless" + std::to_string(NamelessCount++);
    for (auto &BasicBlock : Function) {
      copyFeaturesFromBB(&BasicBlock, FunctionName); 
      processBasicBlockOfFunction(BasicBlock, FunctionName);
    } 
  }
}

std::unique_ptr<Feature> CFGFunctionStaticFeatures::clone() const {
  CFGFunctionStaticFeatures *Clone = new CFGFunctionStaticFeatures(*this);
  Clone->Yaml.reset(new Yamlfy<MapVectorFeature<std::string, uint64_t>>(Clone));
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
  { "nof_cfg_crit_edges", "Nof critical edges in a cfg" },
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
      ValueType::Int, FeatureInfo::Static, SubFeatures));

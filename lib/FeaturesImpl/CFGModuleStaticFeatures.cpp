/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGFunctionStaticFeatures.cpp
 * @brief This file implements the @a CFGFunctionStaticFeatures class.
 */

#include "pinhao/Features/FeatureRegistry.h"
#include "pinhao/Features/MapVectorFeature.h"
#include "pinhao/Features/VectorFeature.h"

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"

using namespace pinhao;

namespace {

  class CFGModuleStaticFeatures : public VectorFeature<uint64_t> {
    private:
      std::shared_ptr<Feature> FunctionFeatures;

      void copyFeaturesFromFunction(std::string FunctionName); 

      void addNToSubFeature(std::string SubFeatureName, uint64_t N) {
        uint64_t Total = getValueOf(SubFeatureName) + N;
        setValueOf(SubFeatureName, Total);
      }

    public:
      ~CFGModuleStaticFeatures() {}
      CFGModuleStaticFeatures(FeatureInfo *Info, std::shared_ptr<Feature> *F = nullptr) : 
        VectorFeature<uint64_t>(Info) { 
          if (F) FunctionFeatures = *F; 
        }

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;
  };

}

void CFGModuleStaticFeatures::copyFeaturesFromFunction(std::string FunctionName) {
  MapVectorFeature<std::string, uint64_t> *FnFeatures =
    static_cast<MapVectorFeature<std::string, uint64_t>*>(FunctionFeatures.get());
  for (auto &Pair : *FunctionFeatures) {
    std::string FeatureName = Pair.first;
    addNToSubFeature(FeatureName, FnFeatures->getValueOfKey(FeatureName, FunctionName));
  }
}

void CFGModuleStaticFeatures::processModule(llvm::Module &Module) {
  if (this->isProcessed()) return;
  Processed = true;

  if (!FunctionFeatures.get()) {
    FunctionFeatures.reset(FeatureRegistry::get("cfg_fn_static").release());
    FunctionFeatures->processModule(Module);
  }
  uint64_t NamelessCount = 0;
  for (auto &Function : Module) {
    if (Function.getBasicBlockList().size() > 0) {
      std::string FunctionName = Function.getName();
      if (FunctionName == "") FunctionName = "Nameless" + std::to_string(NamelessCount++);
      copyFeaturesFromFunction(FunctionName);
      addNToSubFeature("nof_functions", 1);
    }
  }
}

std::unique_ptr<Feature> CFGModuleStaticFeatures::clone() const {
  CFGModuleStaticFeatures *Clone = new CFGModuleStaticFeatures(*this);
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
  { "nof_functions", "Number of Functions" },
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

static RegisterFeature<CFGModuleStaticFeatures> 
X(new CompositeFeatureInfo("cfg_md_static", "Static Information of Module at CFG", 
      ValueType::Int, FeatureInfo::Static, SubFeatures));

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGFunctionStaticFeatures.cpp
 * @brief This file implements the @a CFGFunctionStaticFeatures class.
 */

#include "pinhao/Features/FeatureRegistry.h"
#include "pinhao/Features/Features.h"

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"

using namespace pinhao;

namespace {

  /**
   * @brief This class collects some static features for each Function in the Module.
   */
  class CFGModuleStaticFeatures : public VectorFeature<uint64_t> {
    private:
      std::shared_ptr<Feature> FunctionFeatures;

      /// @brief Copy the value of all features collected from the @a FunctionFeatures
      /// and sums to the total of this vector.
      void copyFeaturesFromFunction(void *Function); 

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

void CFGModuleStaticFeatures::copyFeaturesFromFunction(void *Function) {
  const std::string Prefix = "fn_";
  const std::string ModPrefix = "md_";
  MapVectorFeature<void*, uint64_t> *FnFeatures =
    static_cast<MapVectorFeature<void*, uint64_t>*>(FunctionFeatures.get());
  for (auto &Pair : *FunctionFeatures) {
    std::string FeatureName = ModPrefix + Pair.first.substr(Prefix.length(), std::string::npos);
    addNToSubFeature(FeatureName, FnFeatures->getValueOfKey(Pair.first, Function));
  }
}

void CFGModuleStaticFeatures::processModule(llvm::Module &Module) {
  if (!FunctionFeatures.get()) {
    FunctionFeatures.reset(FeatureRegistry::get("cfg_fn_static").release());
    FunctionFeatures->processModule(Module);
  }
  for (auto &Function : Module) {
    if (Function.getBasicBlockList().size() > 0) {
      copyFeaturesFromFunction(&Function);
      addNToSubFeature("md_functions", 1);
    }
  }
}

std::unique_ptr<Feature> CFGModuleStaticFeatures::clone() const {
  CFGModuleStaticFeatures *Clone = new CFGModuleStaticFeatures(*this);
  return std::unique_ptr<Feature>(Clone);
}

static std::map<std::string, std::string> SubFeatures = {
  { "md_inst", "Number of Instructions" },
  { "md_assign_inst", "Nof assignment instructions" },
  { "md_binop_int_inst", "Nof integer binop instructions" }, 
  { "md_binop_flt_inst", "Nof float binop instructions" },
  { "md_terminator_inst", "Nof terminator instructions" },
  { "md_binop_bitw_inst", "Nof bitwise binop instructions" },
  { "md_vector_inst", "Nof vector instructions" },
  { "md_memory_adress_inst", "Nof memory access and addressing instructions" },
  { "md_aggregate_inst", "Nof aggregate instructions" },
  { "md_conv_int_inst", "Nof integer conversion instructions" },
  { "md_conv_flt_inst", "Nof float conversion instructions" },
  { "md_call_inst", "Nof call instructions" },
  { "md_callarg_ptr_inst", "Nof call instructions that has pointers as arguments" },
  { "md_callarg_g4_inst", "Nof call instructions that have more than 4 arguments" },
  { "md_callret_int_inst", "Nof call instructions that return an integer" },
  { "md_callret_flt_inst", "Nof call instructions that return a float" },
  { "md_callret_ptr_inst", "Nof call instructions that return a pointer" },
  { "md_switch_inst", "Nof switch instructions" },
  { "md_indirectbr_inst", "Nof indirect branches instructions" },
  { "md_condbr_inst", "Nof conditional branches instructions" },
  { "md_uncondbr_inst", "Nof unconditional branches instructions" },
  { "md_load_inst", "Nof load instructions" },
  { "md_store_inst", "Nof store instructions" },
  { "md_getelemptr_inst", "Nof GetElemPtr instructions" },
  { "md_phinode_inst", "Nof PHI nodes" },
  { "md_cfg_edges", "Nof edges in a cfg" },
  { "md_cfgcrit_edges", "Nof critical edges in a cfg" },
  { "md_bb", "Number of BasicBlocks" },
  { "md_1suc_bb", "Nof BBs with 1-suc" },
  { "md_2suc_bb", "Nof BBs with 2-suc" },
  { "md_g2suc_bb", "Nof BBs with -gt 2-suc" },
  { "md_1pred_bb", "Nof BBs with 1-pred" },
  { "md_2pred_bb", "Nof BBs with 2-pred" },
  { "md_g2pred_bb", "Nof BBs with -gt 2-pred" },
  { "md_1pred_1suc_bb", "Nof BBs with 1-pred/1-suc" },
  { "md_1pred_2suc_bb", "Nof BBs with 1-pred/2-suc" },
  { "md_2pred_1suc_bb", "Nof BBs with 2-pred/1-suc" },
  { "md_2pred_2suc_bb", "Nof BBs with 2-pred/2-suc" },
  { "md_g2pred_g2suc_bb", "Nof BBs with -gt 2-pred/ -gt 2-suc" },
  { "md_l15inst_bb", "Nof BBs -lt 15 instructions" },
  { "md_ge15le500inst_bb", "Nof BBs -ge 15 -le 500 instructions"},
  { "md_g500inst_bb", "Nof BBs -gt 500 instructions" }
};

static RegisterFeature<CFGModuleStaticFeatures> 
X(new CompositeFeatureInfo("cfg_md_static", "Static Information of Module at CFG", 
      FeatureInfo::IntType, FeatureInfo::Static, SubFeatures));

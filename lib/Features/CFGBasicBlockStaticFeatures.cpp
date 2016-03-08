/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGBasicBlockStaticFeatures.cpp
 * @brief This file implements the @a CFGBasicBlockStaticFeatures class.
 */

#include "pinhao/Features/Features.h"

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"

using namespace pinhao;

namespace {

  /**
   * @brief This class collects some static features for each BasicBlock in the Module.
   */
  class CFGBasicBlockStaticFeatures : public MapVectorFeature<void*, uint64_t> {
    private:
      /// @brief Updates the basic block @a BasicBlock informations based on the
      /// current instruction @a Instruction.
      void processInstructionOfBB(llvm::Instruction& Instruction, llvm::BasicBlock& BasicBlock);

      /// @brief Gets the total of the sub-feature @a SubFeatureName of @a Key,
      /// and adds 1 to it.
      void addOneToSubFeatureOfKey(std::string SubFeatureName, void* Key) {
        uint64_t Quantity = 1;
        const uint64_t *Pointer = getSubFeatureOfKey(SubFeatureName, Key);
        if (Pointer) Quantity += *Pointer;
        setSubFeatureOfKey(SubFeatureName, Quantity, Key);
      }

    public:
      ~CFGBasicBlockStaticFeatures() {}
      CFGBasicBlockStaticFeatures(FeatureInfo *Info) : 
        MapVectorFeature<void*, uint64_t>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      /// @brief This function iterates over all basic blocks and collects
      /// information about each instruction.
      void processModule(llvm::Module &Module) override;
  };

}

void CFGBasicBlockStaticFeatures::processInstructionOfBB(llvm::Instruction& Instruction, 
    llvm::BasicBlock& BasicBlock) {

  addOneToSubFeatureOfKey("bb_inst", &BasicBlock);

  switch(Instruction.getOpcode()) {
    /* Terminator Instructions */
    case llvm::Instruction::Br:
      {
        llvm::BranchInst *BI = llvm::dyn_cast<llvm::BranchInst>(&Instruction);
        if (BI->isConditional()) addOneToSubFeatureOfKey("bb_condbr_inst", &BasicBlock);
        else addOneToSubFeatureOfKey("bb_uncondbr_inst", &BasicBlock);
        break;
      }

    case llvm::Instruction::Switch:
      addOneToSubFeatureOfKey("bb_switch_inst", &BasicBlock);
      break;

    case llvm::Instruction::IndirectBr:
      addOneToSubFeatureOfKey("bb_indirectbr_inst", &BasicBlock);
      break;

    case llvm::Instruction::Ret:
    case llvm::Instruction::Invoke:
    case llvm::Instruction::Resume:
    case llvm::Instruction::Unreachable:
      addOneToSubFeatureOfKey("bb_terminator_inst", &BasicBlock);
      break;

    case llvm::Instruction::Add:
    case llvm::Instruction::Sub:
    case llvm::Instruction::Mul:
    case llvm::Instruction::UDiv:
    case llvm::Instruction::SDiv:
    case llvm::Instruction::URem:
    case llvm::Instruction::SRem:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_binop_int_inst", &BasicBlock);
      break;

    case llvm::Instruction::FAdd:
    case llvm::Instruction::FSub:
    case llvm::Instruction::FMul:
    case llvm::Instruction::FDiv:
    case llvm::Instruction::FRem:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_binop_flt_inst", &BasicBlock);
      break;
    case llvm::Instruction::Shl:
    case llvm::Instruction::LShr:
    case llvm::Instruction::AShr:
    case llvm::Instruction::And:
    case llvm::Instruction::Or:
    case llvm::Instruction::Xor:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_binop_bitw_inst", &BasicBlock);
      break;

    case llvm::Instruction::ExtractElement:
    case llvm::Instruction::InsertElement:
    case llvm::Instruction::ShuffleVector:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_vector_inst", &BasicBlock);
      break;

    case llvm::Instruction::ExtractValue:
    case llvm::Instruction::InsertValue:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_aggregate_inst", &BasicBlock);
      break;

    case llvm::Instruction::Load:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_load_inst", &BasicBlock);
      break;

    case llvm::Instruction::Store:
      addOneToSubFeatureOfKey("bb_store_inst", &BasicBlock);
      break;

    case llvm::Instruction::Fence:
      addOneToSubFeatureOfKey("bb_memory_adress_inst", &BasicBlock);
      break;

    case llvm::Instruction::Alloca:
    case llvm::Instruction::AtomicRMW:
    case llvm::Instruction::AtomicCmpXchg:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_memory_adress_inst", &BasicBlock);
      break;
    case llvm::Instruction::GetElementPtr:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_getelemptr_inst", &BasicBlock);
      break;

    case llvm::Instruction::Trunc:
    case llvm::Instruction::ZExt:
    case llvm::Instruction::SExt:
    case llvm::Instruction::UIToFP:
    case llvm::Instruction::SIToFP:
    case llvm::Instruction::PtrToInt:
    case llvm::Instruction::IntToPtr:
    case llvm::Instruction::BitCast:
    case llvm::Instruction::AddrSpaceCast:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_conv_int_inst", &BasicBlock);
      break;

    case llvm::Instruction::FPTrunc:
    case llvm::Instruction::FPExt:
    case llvm::Instruction::FPToUI:
    case llvm::Instruction::FPToSI:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_conv_flt_inst", &BasicBlock);
      break;

    case llvm::Instruction::ICmp:
    case llvm::Instruction::FCmp:
    case llvm::Instruction::Select:
    case llvm::Instruction::VAArg:
    case llvm::Instruction::LandingPad:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_other_inst", &BasicBlock);
      break;

    case llvm::Instruction::PHI:
      addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("bb_phinode_inst", &BasicBlock);
      break;

    case llvm::Instruction::Call:
      {
        addOneToSubFeatureOfKey("bb_assign_inst", &BasicBlock);
        addOneToSubFeatureOfKey("bb_call_inst", &BasicBlock);
        llvm::CallInst *CI = llvm::dyn_cast<llvm::CallInst>(&Instruction);
        llvm::Type *Ty = CI->getFunctionType()->getReturnType();

        // arguments
        if (CI->getNumArgOperands() > 4) addOneToSubFeatureOfKey("bb_callarg_g4_inst", &BasicBlock);
        for (unsigned O = 0; O < CI->getNumArgOperands(); ++O)
          if (CI->getArgOperand(O)->getType()->isPointerTy())
            addOneToSubFeatureOfKey("bb_callarg_ptr_inst", &BasicBlock);

        // return type
        if (Ty->isVoidTy()) break;
        else if (Ty->isIntegerTy()) addOneToSubFeatureOfKey("bb_callret_int_inst", &BasicBlock);
        else if (Ty->isFloatingPointTy()) addOneToSubFeatureOfKey("bb_callret_flt_inst", &BasicBlock);
        else if (Ty->isPointerTy()) addOneToSubFeatureOfKey("bb_callret_ptr_inst", &BasicBlock);
        break;
      }

    default: break;
  } 
}

void CFGBasicBlockStaticFeatures::processModule(llvm::Module& Module) {
  for (auto &Function : Module) {
    for (auto &BasicBlock : Function) {
      for (auto &Instruction : BasicBlock)
        processInstructionOfBB(Instruction, BasicBlock); 
    }
  }
}

std::unique_ptr<Feature> CFGBasicBlockStaticFeatures::clone() const {
  Feature *Clone = new CFGBasicBlockStaticFeatures(*this); 
  return std::unique_ptr<Feature>(Clone);
}

static std::map<std::string, std::string> SubFeatures = {
  { "bb_inst", "Number of Instructions" },
  { "bb_assign_inst", "Nof assignment instructions" },
  { "bb_binop_int_inst", "Nof integer binop instructions" },
  { "bb_binop_flt_inst", "Nof float binop instructions" },
  { "bb_terminator_inst", "Nof terminator instructions" },
  { "bb_binop_bitw_inst", "Nof bitwise binop instructions" },
  { "bb_vector_inst", "Nof vector instructions" },
  { "bb_memory_adress_inst", "Nof memory access and addressing instructions" },
  { "bb_aggregate_inst", "Nof aggregate instructions" },
  { "bb_conv_int_inst", "Nof integer conversion instructions" },
  { "bb_conv_flt_inst", "Nof float conversion instructions" },
  { "bb_call_inst", "Nof call instructions" },
  { "bb_callarg_ptr_inst", "Nof call instructions that has pointers as arguments" },
  { "bb_callarg_g4_inst", "Nof call instructions that have more than 4 arguments" },
  { "bb_callret_int_inst", "Nof call instructions that return an integer" },
  { "bb_callret_flt_inst", "Nof call instructions that return a float" },
  { "bb_callret_ptr_inst", "Nof call instructions that return a pointer" },
  { "bb_switch_inst", "Nof switch instructions" },
  { "bb_indirectbr_inst", "Nof indirect branches instructions" },
  { "bb_condbr_inst", "Nof conditional branches instructions" },
  { "bb_uncondbr_inst", "Nof unconditional branches instructions" },
  { "bb_load_inst", "Nof load instructions" },
  { "bb_store_inst", "Nof store instructions" },
  { "bb_getelemptr_inst", "Nof GetElemPtr instructions" },
  { "bb_phinode_inst", "Nof PHI nodes" }
};

static RegisterFeature<CFGBasicBlockStaticFeatures> 
X(new CompositeFeatureInfo("cfg_bb_static", "Static Information of BasicBlocks at CFG", 
      FeatureInfo::IntType, FeatureInfo::Static, SubFeatures));

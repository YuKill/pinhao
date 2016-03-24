/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CFGBasicBlockStaticFeatures.cpp
 * @brief This file implements the @a CFGBasicBlockStaticFeatures class.
 */

#include "pinhao/Features/MapVectorFeature.h"

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"

using namespace pinhao;

namespace {

  class CFGBasicBlockStaticFeatures : public MapVectorFeature<void*, uint64_t> {
    private:
      void processInstructionOfBB(llvm::Instruction& Instruction, llvm::BasicBlock& BasicBlock);

      void addOneToSubFeatureOfKey(std::string SubFeatureName, void* Key) {
        uint64_t Quantity = 1 + getValueOfKey(SubFeatureName, Key);
        setValueOfKey(SubFeatureName, Quantity, Key);
      }

      std::map<void*, std::pair<std::string, uint64_t>> Order; 

    public:
      ~CFGBasicBlockStaticFeatures() {}
      CFGBasicBlockStaticFeatures(FeatureInfo *Info) : 
        MapVectorFeature<void*, uint64_t>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;

      void appendYaml(YAML::Emitter &Emitter, bool printReduced) override;
  };

}

void CFGBasicBlockStaticFeatures::processInstructionOfBB(llvm::Instruction& Instruction, 
    llvm::BasicBlock& BasicBlock) {

  addOneToSubFeatureOfKey("nof_inst", &BasicBlock);

  switch(Instruction.getOpcode()) {
    /* Terminator Instructions */
    case llvm::Instruction::Br:
      {
        llvm::BranchInst *BI = llvm::dyn_cast<llvm::BranchInst>(&Instruction);
        if (BI->isConditional()) addOneToSubFeatureOfKey("nof_condbr_inst", &BasicBlock);
        else addOneToSubFeatureOfKey("nof_uncondbr_inst", &BasicBlock);
        break;
      }

    case llvm::Instruction::Switch:
      addOneToSubFeatureOfKey("nof_switch_inst", &BasicBlock);
      break;

    case llvm::Instruction::IndirectBr:
      addOneToSubFeatureOfKey("nof_indirectbr_inst", &BasicBlock);
      break;

    case llvm::Instruction::Ret:
    case llvm::Instruction::Invoke:
    case llvm::Instruction::Resume:
    case llvm::Instruction::Unreachable:
      addOneToSubFeatureOfKey("nof_terminator_inst", &BasicBlock);
      break;

    case llvm::Instruction::Add:
    case llvm::Instruction::Sub:
    case llvm::Instruction::Mul:
    case llvm::Instruction::UDiv:
    case llvm::Instruction::SDiv:
    case llvm::Instruction::URem:
    case llvm::Instruction::SRem:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_binop_int_inst", &BasicBlock);
      break;

    case llvm::Instruction::FAdd:
    case llvm::Instruction::FSub:
    case llvm::Instruction::FMul:
    case llvm::Instruction::FDiv:
    case llvm::Instruction::FRem:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_binop_flt_inst", &BasicBlock);
      break;
    case llvm::Instruction::Shl:
    case llvm::Instruction::LShr:
    case llvm::Instruction::AShr:
    case llvm::Instruction::And:
    case llvm::Instruction::Or:
    case llvm::Instruction::Xor:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_binop_bitw_inst", &BasicBlock);
      break;

    case llvm::Instruction::ExtractElement:
    case llvm::Instruction::InsertElement:
    case llvm::Instruction::ShuffleVector:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_vector_inst", &BasicBlock);
      break;

    case llvm::Instruction::ExtractValue:
    case llvm::Instruction::InsertValue:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_aggregate_inst", &BasicBlock);
      break;

    case llvm::Instruction::Load:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_load_inst", &BasicBlock);
      break;

    case llvm::Instruction::Store:
      addOneToSubFeatureOfKey("nof_store_inst", &BasicBlock);
      break;

    case llvm::Instruction::Fence:
      addOneToSubFeatureOfKey("nof_memory_adress_inst", &BasicBlock);
      break;

    case llvm::Instruction::Alloca:
    case llvm::Instruction::AtomicRMW:
    case llvm::Instruction::AtomicCmpXchg:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_memory_adress_inst", &BasicBlock);
      break;
    case llvm::Instruction::GetElementPtr:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_getelemptr_inst", &BasicBlock);
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
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_conv_int_inst", &BasicBlock);
      break;

    case llvm::Instruction::FPTrunc:
    case llvm::Instruction::FPExt:
    case llvm::Instruction::FPToUI:
    case llvm::Instruction::FPToSI:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_conv_flt_inst", &BasicBlock);
      break;

    case llvm::Instruction::ICmp:
    case llvm::Instruction::FCmp:
    case llvm::Instruction::Select:
    case llvm::Instruction::VAArg:
    case llvm::Instruction::LandingPad:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      break;

    case llvm::Instruction::PHI:
      addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
      addOneToSubFeatureOfKey("nof_phinode_inst", &BasicBlock);
      break;

    case llvm::Instruction::Call:
      {
        addOneToSubFeatureOfKey("nof_assign_inst", &BasicBlock);
        addOneToSubFeatureOfKey("nof_call_inst", &BasicBlock);
        llvm::CallInst *CI = llvm::dyn_cast<llvm::CallInst>(&Instruction);
        llvm::Type *Ty = CI->getFunctionType()->getReturnType();

        // arguments
        if (CI->getNumArgOperands() > 4) addOneToSubFeatureOfKey("nof_callarg_g4_inst", &BasicBlock);
        for (unsigned O = 0; O < CI->getNumArgOperands(); ++O)
          if (CI->getArgOperand(O)->getType()->isPointerTy())
            addOneToSubFeatureOfKey("nof_callarg_ptr_inst", &BasicBlock);

        // return type
        if (Ty->isVoidTy()) break;
        else if (Ty->isIntegerTy()) addOneToSubFeatureOfKey("nof_callret_int_inst", &BasicBlock);
        else if (Ty->isFloatingPointTy()) addOneToSubFeatureOfKey("nof_callret_flt_inst", &BasicBlock);
        else if (Ty->isPointerTy()) addOneToSubFeatureOfKey("nof_callret_ptr_inst", &BasicBlock);
        break;
      }

    default: break;
  } 
}

void CFGBasicBlockStaticFeatures::processModule(llvm::Module& Module) {
  if (this->isProcessed()) return;
  Processed = true;

  uint64_t NamelessCount = 0;
  for (auto &Function : Module) {
    uint64_t Count = 0;
    std::string FunctionName = Function.getName();
    if (FunctionName == "") FunctionName = "Nameless" + std::to_string(NamelessCount++); 
    for (auto &BasicBlock : Function) {
      Order.insert(std::make_pair(&BasicBlock, std::make_pair(FunctionName, Count++)));
      for (auto &Instruction : BasicBlock)
        processInstructionOfBB(Instruction, BasicBlock); 
    }
  }
}

std::unique_ptr<Feature> CFGBasicBlockStaticFeatures::clone() const {
  Feature *Clone = new CFGBasicBlockStaticFeatures(*this); 
  return std::unique_ptr<Feature>(Clone);
}

void CFGBasicBlockStaticFeatures::appendYaml(YAML::Emitter &Emitter, bool printReduced) {
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();

  Emitter << YAML::Key << "values";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &ValuePair : TheFeature) {
    Emitter << YAML::Key << std::to_string((uint64_t)ValuePair.first);
    Emitter << YAML::Value << YAML::BeginMap;
    for (auto &InfoPair : *(this)) {
      if (printReduced && getValueOfKey(InfoPair.first, ValuePair.first) == 0) continue;
      Emitter << YAML::Key << InfoPair.first;
      Emitter << YAML::Value << getValueOfKey(InfoPair.first, ValuePair.first);
      Emitter << YAML::Comment(InfoPair.second);
    }
    Emitter << YAML::EndMap;
  }
  Emitter << YAML::EndMap;

  Emitter << YAML::Key << "index";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &OrderPair : Order) {
    Emitter << YAML::Key << std::to_string((uint64_t)OrderPair.first);
    Emitter << YAML::Value << 
      YAML::BeginSeq << OrderPair.second.first << std::to_string(OrderPair.second.second) << YAML::EndSeq;
  }
  Emitter << YAML::EndMap;

  Emitter << YAML::EndMap;
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
  { "nof_phinode_inst", "Nof PHI nodes" }
};

static RegisterFeature<CFGBasicBlockStaticFeatures> 
X(new CompositeFeatureInfo("cfg_bb_static", "Static Information of BasicBlocks at CFG", 
      FeatureInfo::IntType, FeatureInfo::Static, SubFeatures));

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FunctionsGeneFeature.cpp
 */

#include "pinhao/Features/MapFeature.h"

#include "llvm/IR/Instruction.h"

using namespace pinhao;

namespace {

  class FunctionsGeneFeature : public MapFeature<std::string, std::string> {
    private:
      std::string getFunctionGene(llvm::Function &Function);
      std::string getInstructionGene(llvm::Instruction &Instruction);

    public:
      ~FunctionsGeneFeature() {}
      FunctionsGeneFeature(FeatureInfo *Info) : MapFeature<std::string, std::string>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;
  };

}

void FunctionsGeneFeature::processModule(llvm::Module &Module) {
  if (this->isProcessed()) return;
  Processed = true;

  uint64_t NamelessCount = 0;
  for (auto &Function : Module) {
    if (Function.getBasicBlockList().size() <= 0) continue;
    std::string FunctionName = (Function.getName() == "") ? 
      "Nameless" + std::to_string(NamelessCount++) : Function.getName().str();
    std::string FunctionGene = getFunctionGene(Function);  
    this->setValueOfKey(this->getName(), FunctionGene, FunctionName);
  }
}

std::string FunctionsGeneFeature::getFunctionGene(llvm::Function &Function) {
  std::string Dna = ""; 
  for (auto &BasicBlock : Function) {
    for (auto &Instruction : BasicBlock) {
      Dna += getInstructionGene(Instruction);
    }
  }
  return Dna;
}

std::string FunctionsGeneFeature::getInstructionGene(llvm::Instruction &Instruction) {
  switch(Instruction.getOpcode()) {

    case llvm::Instruction::Br:
      return "A";

    case llvm::Instruction::Switch:
      return "B";

    case llvm::Instruction::IndirectBr:
      return "C";

    case llvm::Instruction::Ret:
    case llvm::Instruction::Invoke:
    case llvm::Instruction::Resume:
    case llvm::Instruction::Unreachable:
      return "D";

    case llvm::Instruction::Add:
    case llvm::Instruction::Sub:
    case llvm::Instruction::Mul:
    case llvm::Instruction::UDiv:
    case llvm::Instruction::SDiv:
    case llvm::Instruction::URem:
    case llvm::Instruction::SRem:
      return "E";

    case llvm::Instruction::FAdd:
    case llvm::Instruction::FSub:
    case llvm::Instruction::FMul:
    case llvm::Instruction::FDiv:
    case llvm::Instruction::FRem:
      return "F";

    case llvm::Instruction::Shl:
    case llvm::Instruction::LShr:
    case llvm::Instruction::AShr:
    case llvm::Instruction::And:
    case llvm::Instruction::Or:
    case llvm::Instruction::Xor:
      return "G";
    case llvm::Instruction::ExtractElement:
    case llvm::Instruction::InsertElement:
    case llvm::Instruction::ShuffleVector:
      return "H";

    case llvm::Instruction::ExtractValue:
    case llvm::Instruction::InsertValue:
      return "I";

    case llvm::Instruction::Load:
      return "J";

    case llvm::Instruction::Store:
      return "K";

    case llvm::Instruction::Alloca:
      return "L";

    case llvm::Instruction::Fence:
    case llvm::Instruction::AtomicRMW:
    case llvm::Instruction::AtomicCmpXchg:
      return "M";

    case llvm::Instruction::GetElementPtr:
      return "N";

    case llvm::Instruction::Trunc:
    case llvm::Instruction::ZExt:
    case llvm::Instruction::SExt:
    case llvm::Instruction::UIToFP:
    case llvm::Instruction::SIToFP:
    case llvm::Instruction::PtrToInt:
    case llvm::Instruction::IntToPtr:
    case llvm::Instruction::BitCast:
    case llvm::Instruction::AddrSpaceCast:
      return "O";

    case llvm::Instruction::FPTrunc:
    case llvm::Instruction::FPExt:
    case llvm::Instruction::FPToUI:
    case llvm::Instruction::FPToSI:
      return "P";

    case llvm::Instruction::ICmp:
    case llvm::Instruction::FCmp:
    case llvm::Instruction::Select:
    case llvm::Instruction::VAArg:
    case llvm::Instruction::LandingPad:
      return "Q";

    case llvm::Instruction::PHI:
      return "R";

    case llvm::Instruction::Call:
      return "S";

    default: return "X";
  }
}

std::unique_ptr<Feature> FunctionsGeneFeature::clone() const {
  FunctionsGeneFeature *Clone = new FunctionsGeneFeature(*this);
  Clone->Yaml.reset(new Yamlfy<MapFeature<std::string, std::string>>(Clone));
  return std::unique_ptr<Feature>(Clone);
} 

static RegisterFeature<FunctionsGeneFeature> 
X(new FeatureInfo("function-dna", "Extract Function DNA.", FeatureInfo::StringType, FeatureInfo::Static));

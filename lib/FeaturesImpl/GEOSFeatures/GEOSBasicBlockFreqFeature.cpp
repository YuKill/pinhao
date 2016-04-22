/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSBasicBlockFreqFeature.cpp
 */

#include "pinhao/Features/MapFeature.h"
#include "pinhao/InitializeRoutines.h"
#include "pinhao/Support/YamlOptions.h"

#include "geos/ProfileModule/ProfileModule.h"
#include "geos/Profiling/GEOSProfiler.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Bitcode/ReaderWriter.h"

using namespace pinhao;

static config::YamlOpt<std::string> 
GEOSProfLibFile("geos-prof-lib", "The GEOSProfLib file name.", true, "");

static config::YamlOpt<std::string> 
ModuleName("module", "The module.", false, "");

static llvm::Module *GEOSProfLib;
static std::shared_ptr<ProfileModule> PModule;

namespace {

  class GEOSBasicBlockFreqFeature : public MapFeature<void*, uint64_t> {
    public:
      GEOSBasicBlockFreqFeature(FeatureInfo *Info) : 
        MapFeature<void*, uint64_t>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;
  };

}

void GEOSBasicBlockFreqFeature::processModule(llvm::Module &Module) {
  std::shared_ptr<ProfileModule> PM(PModule);

  if (&Module != PModule->getLLVMModule()) {
    PM.reset(PModule->getCopy(false));
    PM->setLLVMModule(&Module);
    PM->repairProfiling();
  }

  for (auto &Function : Module)
    for (auto &BasicBlock : Function) {
      uint64_t Frequency = PM->getBasicBlockFrequency(BasicBlock);
      setValueOfKey(this->getName(), Frequency, (void*)&BasicBlock); 
    }

}

std::unique_ptr<Feature> GEOSBasicBlockFreqFeature::clone() const {
  GEOSBasicBlockFreqFeature *Clone = new GEOSBasicBlockFreqFeature(*this);
  Clone->Yaml.reset(new Yamlfy<MapFeature<void*, uint64_t>>(Clone));
  return std::unique_ptr<Feature>(Clone);
}

static void propagateToInstructions(ProfileModule &PModule) {
  for (auto &Function : *PModule.getLLVMModule())
    for (auto &BasicBlock : Function)
      for (auto &Instr : BasicBlock)
        PModule.setInstructionFrequency(Instr, 
            PModule.getBasicBlockFrequency(BasicBlock));
}

void pinhao::initializeGEOSBasicBlockFreqFeature(llvm::Module &Module) {
  PModule.reset(new ProfileModule(&Module));

  llvm::SMDiagnostic Error;
  llvm::LLVMContext &Context = llvm::getGlobalContext();
  GEOSProfLib = parseIRFile(GEOSProfLibFile.get(), Error, Context).release();

  std::unique_ptr<GEOSProfiler> GProfiler(new GEOSProfiler());
  GProfiler->populateFrequency(PModule.get(), GEOSProfLib);
  propagateToInstructions(*PModule);
}

static RegisterFeature<GEOSBasicBlockFreqFeature> 
X(new FeatureInfo("geos-bbfreq", "BasicBlock frequency calculated by GEOS.", 
      ValueType::Int, FeatureInfo::Dynamic));

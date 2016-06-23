/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSBasicBlockFreqFeature.cpp
 */

#include "pinhao/InitializationRoutines.h"
#include "pinhao/Features/MapFeature.h"
#include "pinhao/PerformanceAnalyser/GEOSWrapper.h"

#include "geos/ProfileModule/ProfileModule.h"

using namespace pinhao;

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
  std::shared_ptr<ProfileModule> PM(new ProfileModule(&Module));

  PM->repairProfiling();

  for (auto &Function : Module)
    for (auto &BasicBlock : Function) {
      uint64_t Frequency = PM->getBasicBlockFrequency(BasicBlock);
      setValueOfKey(this->getName(), Frequency, (void*)&BasicBlock); 
    }

}

std::unique_ptr<Feature> GEOSBasicBlockFreqFeature::clone() const {
  GEOSBasicBlockFreqFeature *Clone = new GEOSBasicBlockFreqFeature(*this);
  return std::unique_ptr<Feature>(Clone);
}

void pinhao::initializeGEOSBasicBlockFreqFeature(llvm::Module &Module) {
  std::vector<std::string> Argv = { "geos-init" };
  initializeGEOSBasicBlockFreqFeature(Module, Argv);
}

void pinhao::initializeGEOSBasicBlockFreqFeature(llvm::Module &Module, std::vector<std::string> Argv) {
  GEOSWrapper::getFrequencies(Module, Argv);
}

static RegisterFeature<GEOSBasicBlockFreqFeature> 
X(new FeatureInfo("geos-bbfreq", "BasicBlock frequency calculated by GEOS.", 
      ValueType::Int, FeatureInfo::Dynamic));

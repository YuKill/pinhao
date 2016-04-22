/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSFeature.cpp
 */

#include "pinhao/Features/VectorFeature.h"
#include "pinhao/InitializeRoutines.h"
#include "pinhao/Support/YamlOptions.h"

#include "geos/GEOS.h"
#include "geos/CostEstimator/CostEstimatorOptions.h"
#include "geos/Profiling/CallCostReader.h"

#include <memory>

using namespace pinhao;

static config::YamlOpt<std::string>
CallCostFile("call-cost", "File with call cost of extern functions.", true, "");

namespace {

  class GEOSFeatures : public VectorFeature<double> {
    private:
      std::shared_ptr<Feature> BasicBlockFreq;

      void setProfileModuleBBFrequencies(std::shared_ptr<ProfileModule>);

    public:
      GEOSFeatures(FeatureInfo *Info) : VectorFeature<double>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;

  };

}

void GEOSFeatures::setProfileModuleBBFrequencies(std::shared_ptr<ProfileModule> PModule) {
  MappedFeature<void*, uint64_t> *BBFFeature = 
    static_cast<MappedFeature<void*, uint64_t>*>(BasicBlockFreq.get());

  for (auto &Function : *PModule->getLLVMModule()) {
    for (auto &BasicBlock : Function) {
      uint64_t Frequency = BBFFeature->getValueOfKey(BasicBlockFreq->getName(), &BasicBlock);
      PModule->setBasicBlockFrequency(BasicBlock, Frequency); 
    }
  }
}

void GEOSFeatures::processModule(llvm::Module &Module) {
  if (this->isProcessed()) return;
  Processed = true;

  if (!BasicBlockFreq.get()) {
    BasicBlockFreq.reset(FeatureRegistry::get("geos-bbfreq").release());
    BasicBlockFreq->processModule(Module);
  }

  std::shared_ptr<ProfileModule> PModule(new ProfileModule(&Module));
  setProfileModuleBBFrequencies(PModule);

  CostEstimatorOptions Opts;

  Opts.AnalysisActivated = { RegisterUse };
  setValueOf("RegisterUse", GEOS::analyseCost(PModule, Opts));

  /*
  Opts.AnalysisActivated = { InstructionCache };
  setValueOf("IntructionCache", GEOS::analyseCost(PModule, Opts));
  */

  Opts.AnalysisActivated = { StaticInstruction };
  setValueOf("StaticInstruction", GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = { TTIInstruction };
  setValueOf("TTIInstruction", GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = { Branch };
  setValueOf("Branch", GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = { Call };
  setValueOf("Call", GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = getAnalysisFor(NonArchSensitive);
  setValueOf("NoArchSensitive", GEOS::analyseCost(PModule, Opts));

  Opts.AnalysisActivated = getAnalysisFor(ArchSensitive);
  setValueOf("ArchSensitive", GEOS::analyseCost(PModule, Opts));

}

std::unique_ptr<Feature> GEOSFeatures::clone() const {
  GEOSFeatures *Clone = new GEOSFeatures(*this);
  Clone->Yaml.reset(new Yamlfy<VectorFeature<double>>(Clone));
  return std::unique_ptr<Feature>(Clone);
}

void pinhao::initializeGEOSFeatures(llvm::Module &Module) {
  ProfileModule PModule(&Module);
  loadCallCost(CallCostFile.get(), &PModule);
}

static std::map<std::string, std::string> SubFeatures = {
  { "RegisterUse", "Analysis of register use cost." },
  { "InstructionCache", "Analysis of cache use with simulation." },
  { "StaticInstruction", "Platform independent instruction weight analysis." },
  { "TTIInstruction", "Platform dependent instruction weight analysis." },
  { "Branch", "Branch misprediction cost analysis." },
  { "Call", "Extern call cost analysis." },
  { "NoArchSensitive", "Weighted sum of platform independent analysis." },
  { "ArchSensitive", "Weighted sum of platform dependent analysis." }
};

static RegisterFeature<GEOSFeatures> 
X(new CompositeFeatureInfo("geos-features", "Costs of the module calculated by GEOS.", 
      ValueType::Float, FeatureInfo::Dynamic, SubFeatures));

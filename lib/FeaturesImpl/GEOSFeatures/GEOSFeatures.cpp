/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSFeature.cpp
 */

#include "pinhao/Initialization.h"
#include "pinhao/Features/VectorFeature.h"
#include "pinhao/PerformanceAnalyser/GEOSWrapper.h"

#include <memory>

using namespace pinhao;

namespace {

  class GEOSFeatures : public VectorFeature<double> {
    public:
      GEOSFeatures(FeatureInfo *Info) : VectorFeature<double>(Info) {}

      std::unique_ptr<Feature> clone() const override;

      void processModule(llvm::Module &Module) override;

  };

}

void GEOSFeatures::processModule(llvm::Module &Module) {
  if (this->isProcessed()) return;
  Processed = true;

  std::vector<double> Cost = GEOSWrapper::repairAndAnalyse(Module);
  setValueOf("RegisterUse", Cost[0]);

  /*
  Opts.AnalysisActivated = { InstructionCache };
  setValueOf("IntructionCache", Cost[1]);
  */

  setValueOf("StaticInstruction", Cost[2]);
  setValueOf("TTIInstruction", Cost[3]);
  setValueOf("Branch", Cost[4]);
  setValueOf("Call", Cost[5]);
  setValueOf("NoArchSensitive", Cost[6]);
  setValueOf("ArchSensitive", Cost[7]);

}

std::unique_ptr<Feature> GEOSFeatures::clone() const {
  GEOSFeatures *Clone = new GEOSFeatures(*this);
  Clone->Yaml.reset(new Yamlfy<VectorFeature<double>>(Clone));
  return std::unique_ptr<Feature>(Clone);
}

void pinhao::initializeGEOSFeatures(llvm::Module &Module) {
  GEOSWrapper::loadCallCostFile(Module);
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

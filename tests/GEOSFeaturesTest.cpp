#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/Features.h"
#include "pinhao/Support/JITExecutor.h"
#include "pinhao/Support/YamlOptions.h"
#include "pinhao/Optimizer/Optimizations.h"
#include "pinhao/Optimizer/OptimizationSequence.h"
#include "pinhao/InitializeRoutines.h"

#include "ModuleReader.h"

using namespace pinhao;


YAML::Node Node;
const std::string BenchmarkPath("../../benchmark");
const std::string PolybenchPath("../../benchmark/polybench-ll");
std::shared_ptr<llvm::Module> M;

config::YamlOpt<std::string> ModuleName("module", "The target module.", true, "");

void setup() {
  Node["geos-prof-lib"] = BenchmarkPath + "/GEOSProfLib.bc";
  Node["call-cost"] = PolybenchPath + "/2mm/callcost";
  Node["module"] = PolybenchPath + "/2mm/2mm.bc";
}

TEST(GEOSFeaturesTest, InitializeTest) {
  initializeGEOSFeatures(*M.get());
  initializeGEOSBasicBlockFreqFeature(*M.get());
}

TEST(GEOSFeaturesTest, CostAnalysisTest) {
  std::unique_ptr<Feature> GF = FeatureRegistry::get("geos-features");
  GF->processModule(*M);

  for (auto Pair : *GF) {
    if (Pair.first == "InstructionCache") continue;

    LinearFeature<double> *GFCast = static_cast<LinearFeature<double>*>(GF.get());
    std::cout << "Feature: " << Pair.first << std::endl;
    ASSERT_NE(GFCast->getValueOf(Pair.first), 0); 
  }

  GF->printYaml();
}

TEST(GEOSFeaturesTest, RandomOptimizedCostAnalysisTest) {
  const uint64_t Max = 10;
  const uint64_t SetSize = 20;
  for (uint64_t I = 0; I < Max; ++I) {
    std::unique_ptr<Feature> GF = FeatureRegistry::get("geos-features");

    std::unique_ptr<OptimizationSequence> Sequence = OptimizationSequence::randomize(SetSize); 
    llvm::Module *Optimized = applyOptimizations(*M, Sequence.get());

    while (!Optimized) {
      std::cout << "BAD!! >> ";
      Sequence->print();

      Sequence.reset(OptimizationSequence::randomize(SetSize).release());
      Optimized = applyOptimizations(*M, Sequence.get());
    }

    GF->processModule(*Optimized);

    bool First = true;
    for (auto Pair : *GF) {
      if (Pair.first == "InstructionCache") continue;

      LinearFeature<double> *GFCast = static_cast<LinearFeature<double>*>(GF.get());
      if (First) {
        std::cout << Pair.first << std::endl;
        GF->printYaml();
        Sequence->print();
        First = false;
      }
      ASSERT_NE(GFCast->getValueOf(Pair.first), 0); 
    }

  }

}

int main(int argc, char **argv) {
  setup();
  initializeOptimizer();
  JITExecutor::init();
  config::parseOptions(Node);

  ModuleReader Reader(ModuleName.get());
  M = Reader.getModule();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

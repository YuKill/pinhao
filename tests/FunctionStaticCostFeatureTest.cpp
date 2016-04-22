#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureRegistry.h"

#include "sprof/InitializeRoutines.h"

#include "ModuleReader.h"

using namespace pinhao;

TEST(GeneFeaturesTest, FunctionsGeneFeatureTest) {
  std::string Benchmark("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Benchmark);
  
  std::unique_ptr<Feature> FunctionCost = FeatureRegistry::get("static-cost");
  ASSERT_NE(FunctionCost.get(), nullptr);

  FunctionCost->processModule(*(Reader.getModule().get()));
  FunctionCost->printYaml();
}

int main(int argc, char **argv) {
  llvm::PassRegistry *Registry = llvm::PassRegistry::getPassRegistry();
  llvm::initializeStaticProfilerPasses(*Registry);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureRegistry.h"

#include "ModuleReader.h"

using namespace pinhao;

TEST(GeneFeaturesTest, FunctionsGeneFeatureTest) {
  std::string Benchmark("../../benchmark/polybench-ll/2mm/2mm.bc");
  ModuleReader Reader(Benchmark);
  
  std::unique_ptr<Feature> FunctionsGene = FeatureRegistry::get("function-dna");
  ASSERT_NE(FunctionsGene.get(), nullptr);

  FunctionsGene->processModule(*(Reader.getModule().get()));
  FunctionsGene->print();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

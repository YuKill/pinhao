#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureRegistry.h"

#include "pinhao/Support/JITExecutor.h"

#include "ModuleReader.h"

using namespace pinhao;

TEST(PAPIFeaturesTest, GetFeature) {
  std::unique_ptr<Feature> Papi = FeatureRegistry::get("papi-feat-m");
  ASSERT_NE(Papi.get(), nullptr);
}

TEST(PAPIFeaturesTest, ProcessModule) {
  JITExecutor::init();

  std::string File = "../../benchmark/polybench-ll/2mm/2mm.bc";

  std::unique_ptr<Feature> PapiF = FeatureRegistry::get("papi-feat-m");
  ModuleReader Reader(File);
  PapiF->processModule(*(Reader.getModule().get()));
  PapiF->print(std::cerr);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

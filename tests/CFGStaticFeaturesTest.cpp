#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureRegistry.h"

using namespace pinhao;

TEST(CFGStaticFeaturesTest, GetFeatures) {
  std::unique_ptr<Feature> ModuleFeatures = FeatureRegistry::get("cfg_md_static");
  ASSERT_NE(ModuleFeatures.get(), nullptr);
  std::unique_ptr<Feature> FunctionFeatures = FeatureRegistry::get("cfg_fn_static");
  ASSERT_NE(FunctionFeatures.get(), nullptr);
  std::unique_ptr<Feature> BasicBlockFeatures = FeatureRegistry::get("cfg_bb_static");
  ASSERT_NE(BasicBlockFeatures.get(), nullptr);

  ModuleFeatures->print();
  FunctionFeatures->print();
  BasicBlockFeatures->print();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

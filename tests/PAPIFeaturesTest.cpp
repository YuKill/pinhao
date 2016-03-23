#include "gtest/gtest.h"

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureRegistry.h"

using namespace pinhao;

TEST(PAPIFeaturesTest, GetFeature) {
  std::unique_ptr<Feature> Papi = FeatureRegistry::get("papi-feat-m");
  ASSERT_NE(Papi.get(), nullptr);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

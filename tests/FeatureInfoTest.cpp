#include "gtest/gtest.h"

#include "pinhao/Features/FeatureInfo.h"

using namespace pinhao;

TEST(FeatureInfoTest, SimpleFeatureInfoTest) {
  FeatureInfo *FI = new FeatureInfo("theName", "theDescription", ValueType::String, FeatureInfo::Static);
  ASSERT_EQ(FI->getName(), "theName");
  ASSERT_EQ(FI->getDescription(), "theDescription");
}

TEST(FeatureInfoTest, CompositeFeatureInfoTest) {
  auto TheFeatures = std::map<std::string, std::string> {
    {"nof_bb", "Number of Basic Blocks"},
      {"nof_inst", "Number of Instructions"},
      {"nof_functions", "Number of Functions"}
  };

  FeatureInfo *FI = new CompositeFeatureInfo("theName", "theDescription", ValueType::Float, FeatureInfo::Dynamic, TheFeatures);
  ASSERT_TRUE(FI->isComposite());

  CompositeFeatureInfo *CFI = static_cast<CompositeFeatureInfo*>(FI);
  ASSERT_EQ(CFI->getName(), "theName");
  ASSERT_EQ(CFI->getDescription(), "theDescription");

  ASSERT_TRUE(CFI->hasSubFeature("nof_bb"));
  ASSERT_TRUE(CFI->hasSubFeature("nof_inst"));
  ASSERT_TRUE(CFI->hasSubFeature("nof_functions"));
  ASSERT_FALSE(CFI->hasSubFeature("nof_PHI"));

  ASSERT_EQ(CFI->getSubFeatureDescription("nof_bb"), TheFeatures["nof_bb"]);
  ASSERT_EQ(CFI->getSubFeatureDescription("nof_inst"), TheFeatures["nof_inst"]);
  ASSERT_EQ(CFI->getSubFeatureDescription("nof_functions"), TheFeatures["nof_functions"]);
}

TEST(FeatureInfoTest, CompositeFeatureInfoTmpCtorTest) {
  FeatureInfo *FI = new CompositeFeatureInfo("theName", "theDescription", ValueType::Int, FeatureInfo::Static,
      std::map<std::string, std::string> {
      {"nof_bb", "Number of Basic Blocks"},
      {"nof_inst", "Number of Instructions"},
      {"nof_functions", "Number of Functions"}
      });
  ASSERT_TRUE(FI->isComposite());

  CompositeFeatureInfo *CFI = static_cast<CompositeFeatureInfo*>(FI);
  ASSERT_EQ(CFI->getName(), "theName");
  ASSERT_EQ(CFI->getDescription(), "theDescription");

  ASSERT_TRUE(CFI->hasSubFeature("nof_bb"));
  ASSERT_TRUE(CFI->hasSubFeature("nof_inst"));
  ASSERT_TRUE(CFI->hasSubFeature("nof_functions"));
  ASSERT_FALSE(CFI->hasSubFeature("nof_PHI"));

  ASSERT_EQ(CFI->getSubFeatureDescription("nof_bb"), "Number of Basic Blocks");
  ASSERT_EQ(CFI->getSubFeatureDescription("nof_inst"), "Number of Instructions");
  ASSERT_EQ(CFI->getSubFeatureDescription("nof_functions"), "Number of Functions");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

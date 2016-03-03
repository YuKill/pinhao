#include "gtest/gtest.h"

#include "pinhao/Features/FeatureInfo.h"

using namespace pinhao;

TEST(FeatureInfoTest, SimpleFeatureInfoTest) {
  FeatureInfo *FI = new FeatureInfo("theName", "theDescription");
  ASSERT_EQ(FI->getName(), "theName");
  ASSERT_EQ(FI->getDescription(), "theDescription");
}

TEST(FeatureInfoTest, CompositeFeatureInfoTest) {
  auto TheFeatures = std::map<std::string, std::string> {
    {"nof_bb", "Number of Basic Blocks"},
      {"nof_inst", "Number of Instructions"},
      {"nof_functions", "Number of Functions"}
  };

  FeatureInfo *FI = new CompositeFeatureInfo("theName", "theDescription", TheFeatures);
  ASSERT_EQ(FI->getName(), "theName");
  ASSERT_EQ(FI->getDescription(), "theDescription");

  ASSERT_TRUE(FI->hasSubFeature("nof_bb"));
  ASSERT_TRUE(FI->hasSubFeature("nof_inst"));
  ASSERT_TRUE(FI->hasSubFeature("nof_functions"));
  ASSERT_FALSE(FI->hasSubFeature("nof_PHI"));

  ASSERT_EQ(FI->getSubFeatureDescription("nof_bb"), TheFeatures["nof_bb"]);
  ASSERT_EQ(FI->getSubFeatureDescription("nof_inst"), TheFeatures["nof_inst"]);
  ASSERT_EQ(FI->getSubFeatureDescription("nof_functions"), TheFeatures["nof_functions"]);
}

TEST(FeatureInfoTest, CompositeFeatureInfoTmpCtorTest) {
  FeatureInfo *FI = new CompositeFeatureInfo("theName", "theDescription",
      std::map<std::string, std::string> {
      {"nof_bb", "Number of Basic Blocks"},
      {"nof_inst", "Number of Instructions"},
      {"nof_functions", "Number of Functions"}
      });
  ASSERT_EQ(FI->getName(), "theName");
  ASSERT_EQ(FI->getDescription(), "theDescription");

  ASSERT_TRUE(FI->hasSubFeature("nof_bb"));
  ASSERT_TRUE(FI->hasSubFeature("nof_inst"));
  ASSERT_TRUE(FI->hasSubFeature("nof_functions"));
  ASSERT_FALSE(FI->hasSubFeature("nof_PHI"));

  ASSERT_EQ(FI->getSubFeatureDescription("nof_bb"), "Number of Basic Blocks");
  ASSERT_EQ(FI->getSubFeatureDescription("nof_inst"), "Number of Instructions");
  ASSERT_EQ(FI->getSubFeatureDescription("nof_functions"), "Number of Functions");
}


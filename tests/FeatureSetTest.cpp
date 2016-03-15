#include "gtest/gtest.h"

#include "pinhao/Features/FeatureSet.h"

#include <memory>

using namespace pinhao;

TEST(FeatureSetTest, EmptyInstantiation) {
  std::unique_ptr<FeatureSet> Set = FeatureSet::get();
  ASSERT_NE(Set.get(), nullptr);
}


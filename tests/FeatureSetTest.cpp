#include "gtest/gtest.h"

#include "pinhao/Features/FeatureSet.h"

#include "llvm/PassRegistry.h"

#include <memory>

using namespace pinhao;

TEST(FeatureSetTest, EmptyInstantiation) {

  std::shared_ptr<FeatureSet> Set(FeatureSet::get().release());
  ASSERT_NE(Set.get(), nullptr);

  FeatureSetWrapperPass *FSWP = new FeatureSetWrapperPass(&Set);
  ASSERT_NE(FSWP, nullptr);
}


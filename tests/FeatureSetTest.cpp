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

TEST(FeatureSetTest, Iterating) {
  std::vector<std::string> Names = { "cfg_md_static", "cfg_fn_static", "cfg_bb_static", "papi-feat-m", "function-dna" };
  std::vector<std::shared_ptr<Feature>> Features;
  for (auto &Name : Names) {
    FeatureSet::EnabledFeatures.insert(std::make_pair(Name, std::map<std::string, bool>()));
    Features.push_back(FeatureRegistry::get(Name));
  }

  std::unique_ptr<FeatureSet> Set(FeatureSet::get()); 

  for (auto I = Set->begin(), E = Set->end(); I != E; ++I) {
    // std::cout << I->first << " - " << I->second << std::endl;
    bool Exists = false;
    for (auto &F : Features)
      Exists = Exists || (F->getName() == I->first && (!F->isComposite() || (F->isComposite() && F->hasSubFeature(I->second))));
    ASSERT_TRUE(Exists);
  }

  for (auto &Pair : *(Set)) {
    // std::cout << Pair.first << " - " << Pair.second << std::endl;
    bool Exists = false;
    for (auto &F : Features)
      Exists = Exists || (F->getName() == Pair.first && (!F->isComposite() || (F->isComposite() && F->hasSubFeature(Pair.second))));
    ASSERT_TRUE(Exists);
  }
}

TEST(FeatureSetTest, IteratingEachFeature) {
  std::vector<std::string> Names = { "cfg_md_static", "cfg_fn_static", "cfg_bb_static", "papi-feat-m", "function-dna" };
  std::vector<std::shared_ptr<Feature>> Features;
  for (auto &Name : Names) {
    FeatureSet::EnabledFeatures.insert(std::make_pair(Name, std::map<std::string, bool>()));
    Features.push_back(FeatureRegistry::get(Name));
  }

  std::unique_ptr<FeatureSet> Set(FeatureSet::get()); 

  for (auto &Name : Names) {
    // std::cout << Name << std::endl;
    std::unique_ptr<Feature> F(FeatureRegistry::get(Name));
    for (auto I = Set->begin(Name), E = Set->end(Name); I != E; ++I) {
      ASSERT_TRUE(I->first == F->getName());
      if (F->isComposite())
        ASSERT_TRUE(F->hasSubFeature(I->second));
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

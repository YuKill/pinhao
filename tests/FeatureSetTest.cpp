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

TEST(FeatureSetTest, EnablingFeatures) {
  FeatureSet::disableAll();
  std::vector<std::string> Names = { "cfg_md_static", "cfg_fn_static", "cfg_bb_static", "papi-feat-m", "function-dna" };

  for (auto &Name : Names) {
    ASSERT_FALSE(FeatureSet::isEnabled(Name));
    FeatureSet::enable(Name);
  }
  for (auto &Name : Names) {
    ASSERT_TRUE(FeatureSet::isEnabled(Name));
    std::unique_ptr<Feature> F(FeatureRegistry::get(Name));
    if (F->isComposite())
      for (auto &Pair : *(F.get())) {
        // std::cout << "isEnabled(" << F->getName() << ", " << Pair.first << ")" << std::endl;
        ASSERT_TRUE(FeatureSet::isEnabled(F->getName(), Pair.first)); 
      }
    else
      ASSERT_TRUE(FeatureSet::isEnabled(F->getName())); 
  }


}

TEST(FeatureSetTest, EnablingSomeFeatures) {
  FeatureSet::disableAll();
  std::vector<std::string> Names = { "cfg_md_static", "cfg_fn_static", "cfg_bb_static", "papi-feat-m", "function-dna" };

  for (auto &Name : Names) {
    ASSERT_FALSE(FeatureSet::isEnabled(Name));
    FeatureSet::enable(Name);
  }

  for (auto &Name : Names) {
    ASSERT_TRUE(FeatureSet::isEnabled(Name));
    std::unique_ptr<Feature> F(FeatureRegistry::get(Name));
    if (F->isComposite()) {
      bool First = true;
      FeatureSet::enable(Name, std::vector<std::string> { F->begin()->first });

      for (auto &Pair : *(F.get())) {
        // std::cout << "isEnabled(" << F->getName() << ", " << Pair.first << ")" << std::endl;
        if (First) {
          ASSERT_TRUE(FeatureSet::isEnabled(F->getName(), Pair.first)); 
          First = false;
        } else ASSERT_FALSE(FeatureSet::isEnabled(F->getName(), Pair.first));
      }
    } else
      ASSERT_TRUE(FeatureSet::isEnabled(F->getName())); 
  }

  std::unique_ptr<FeatureSet> Set(FeatureSet::get()); 
  int Count =  0;
  for (auto &Pair : *(Set.get())) {
    std::cout << Pair.first << " - " << Pair.second << std::endl;
    Count++;
  }
  ASSERT_EQ(Count, Names.size());
}

TEST(FeatureSetTest, Iterating) {
  std::vector<std::string> Names = { "cfg_md_static", "cfg_fn_static", "cfg_bb_static", "papi-feat-m", "function-dna" };
  std::vector<std::shared_ptr<Feature>> Features;
  for (auto &Name : Names) {
    FeatureSet::enable(Name);
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
    std::cout << Pair.first << " - " << Pair.second << std::endl;
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
    FeatureSet::enable(Name);
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

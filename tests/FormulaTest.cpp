#include "gtest/gtest.h"

#include "pinhao/MachineLearning/GrammarEvolution/Formulas.h"
#include "pinhao/Support/FormulaYAMLWrapper.h"

#include <iostream>

using namespace pinhao;

TEST(FormulaTest, GenerateIntFormula) {
  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();
  auto IntFormula = generateFormula(Set.get(), ValueType::Int);
  ASSERT_NE(IntFormula.get(), nullptr);
  YAMLWrapper::print(*IntFormula);
}

TEST(FormulaTest, GenerateBoolFormula) {
  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();
  auto BoolFormula = generateFormula(Set.get(), ValueType::Bool);
  ASSERT_NE(BoolFormula.get(), nullptr);
  YAMLWrapper::print(*BoolFormula);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

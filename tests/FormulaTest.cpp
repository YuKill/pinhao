#include "gtest/gtest.h"

#include "pinhao/MachineLearning/GrammarEvolution/Formulas.h"
#include "pinhao/Support/FormulaYAMLWrapper.h"

#include <iostream>

using namespace pinhao;

TEST(FormulaTest, CreateFormulas) {

  for (auto &Pair : FormulaKindInfo) {
    auto Form = createFormula(Pair.first, ValueType::Int);
    ASSERT_NE(Form.get(), nullptr);
  }

  for (auto &Pair : FormulaKindInfo) {
    auto Form = createFormula(Pair.first, ValueType::Float);
    ASSERT_NE(Form.get(), nullptr);
  }

  for (auto &Pair : FormulaKindInfo) {
    if (Pair.first == FormulaKind::ArithBinOp) continue;
    auto Form = createFormula(Pair.first, ValueType::String);
    ASSERT_NE(Form.get(), nullptr);
  }

  for (auto &Pair : FormulaKindInfo) {
    if (Pair.first == FormulaKind::ArithBinOp) continue;
    auto Form = createFormula(Pair.first, ValueType::Bool);
    ASSERT_NE(Form.get(), nullptr);
  }

}

TEST(FormulaTest, GenerateLitFormula) {
  ValueType Type;
  FormulaKind Kind = FormulaKind::Literal;
  std::unique_ptr<FormulaBase> Form;

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  Type = getValueTypeFor<int>();
  Form.reset(createFormula(Kind, Type).release());
  Form->generate(Set.get());
  ASSERT_LE(getFormulaValue<int>(Form.get()), 100);
}

TEST(FormulaTest, GenerateArithBinOpFormula) {
  ValueType Type;
  FormulaKind Kind = FormulaKind::ArithBinOp;
  std::unique_ptr<FormulaBase> Form;

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  Type = getValueTypeFor<int>();
  Form.reset(createFormula(Kind, Type).release());
  Form->generate(Set.get());
  {
    auto FormCasted = static_cast<ArithBinOpFormula<int>*>(Form.get());
    ASSERT_GE(FormCasted->getOperator(), OperatorKind::PLUS);
    ASSERT_LE(FormCasted->getOperator(), OperatorKind::DIV);
  }

  Type = getValueTypeFor<double>();
  Form.reset(createFormula(Kind, Type).release());
  Form->generate(Set.get());
  {
    auto FormCasted = static_cast<ArithBinOpFormula<double>*>(Form.get());
    ASSERT_GE(FormCasted->getOperator(), OperatorKind::PLUS);
    ASSERT_LE(FormCasted->getOperator(), OperatorKind::DIV);
  }
}

TEST(FormulaTest, GenerateBoolBinOpFormula) {
  ValueType Type;
  FormulaKind Kind = FormulaKind::BoolBinOp;
  std::unique_ptr<FormulaBase> Form;

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  Type = getValueTypeFor<int>();
  Form.reset(createFormula(Kind, ValueType::Bool, Type).release());
  Form->generate(Set.get());
  {
    auto FormCasted = static_cast<BoolBinOpFormula<int>*>(Form.get());
    ASSERT_GE(FormCasted->getOperator(), OperatorKind::LT);
    ASSERT_LE(FormCasted->getOperator(), OperatorKind::NEQ);
  }

  Type = getValueTypeFor<double>();
  Form.reset(createFormula(Kind, ValueType::Bool, Type).release());
  Form->generate(Set.get());
  {
    auto FormCasted = static_cast<BoolBinOpFormula<double>*>(Form.get());
    ASSERT_GE(FormCasted->getOperator(), OperatorKind::LT);
    ASSERT_LE(FormCasted->getOperator(), OperatorKind::NEQ);
  }

  Type = getValueTypeFor<std::string>();
  Form.reset(createFormula(Kind, ValueType::Bool, Type).release());
  Form->generate(Set.get());
  {
    auto FormCasted = static_cast<BoolBinOpFormula<std::string>*>(Form.get());
    ASSERT_GE(FormCasted->getOperator(), OperatorKind::LT);
    ASSERT_LE(FormCasted->getOperator(), OperatorKind::NEQ);
  }

  Type = getValueTypeFor<bool>();
  Form.reset(createFormula(Kind, ValueType::Bool, Type).release());
  Form->generate(Set.get());
  {
    auto FormCasted = static_cast<BoolBinOpFormula<bool>*>(Form.get());
    ASSERT_GE(FormCasted->getOperator(), OperatorKind::EQ);
    ASSERT_LE(FormCasted->getOperator(), OperatorKind::OR);
  }
}

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

TEST(FormulaTest, GenerateSimplifyIntFormula) {
  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();
  auto IntFormula = generateFormula(Set.get(), ValueType::Int);
  ASSERT_NE(IntFormula.get(), nullptr);
  YAMLWrapper::print(*IntFormula);
  std::cout << " --------- Simplifying ------------" << std::endl;
  simplifyFormula(IntFormula);
  YAMLWrapper::print(*IntFormula);
}

TEST(FormulaTest, GenerateSimplifyBoolFormula) {
  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();
  auto BoolFormula = generateFormula(Set.get(), ValueType::Bool);
  ASSERT_NE(BoolFormula.get(), nullptr);
  YAMLWrapper::print(*BoolFormula);
  std::cout << " --------- Simplifying ------------" << std::endl;
  simplifyFormula(BoolFormula);
  YAMLWrapper::print(*BoolFormula);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

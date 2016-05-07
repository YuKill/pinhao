#include "gtest/gtest.h"

#include "pinhao/MachineLearning/GrammarEvolution/Formulas.h"
#include "pinhao/Support/FormulaYAMLWrapper.h"

#include <iostream>
#include <fstream>

using namespace pinhao;

bool filesEqual(std::string Filename1, std::string Filename2) {
  std::ifstream If1(Filename1), If2(Filename2);
  std::string Str1, Str2;
  getline(If1, Str1, '\0');
  getline(If2, Str2, '\0');
  return Str1 == Str2;
}

TEST(FormulaTest, GetLitFormula) {
  const std::string YamlFile1("lit-1-formula.yaml");
  const std::string YamlFile2("lit-2-formula.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  ValueType Type = getValueTypeFor<int>();
  ValueType OpType = getValueTypeFor<int>();
  FormulaKind Kind = FormulaKind::Literal;

  auto Form = createFormula(Kind, Type, OpType);
  ASSERT_NE(Form.get(), nullptr);
  Form->generate(Set.get());
  YAMLWrapper::print(*Form, Of1);

  YAML::Node Node = YAML::LoadFile(YamlFile1);
  Form.reset(YAMLWrapper::get<FormulaBase>(Node).release());
  YAMLWrapper::print(*Form, Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(FormulaTest, GetArithBinOpFormula) {
  const std::string YamlFile1("arith-binop-1-formula.yaml");
  const std::string YamlFile2("arith-binop-2-formula.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  ValueType Type = getValueTypeFor<int>();
  ValueType OpType = getValueTypeFor<int>();
  FormulaKind Kind = FormulaKind::ArithBinOp;

  auto Form = createFormula(Kind, Type, OpType);
  ASSERT_NE(Form.get(), nullptr);
  Form->generate(Set.get());
  YAMLWrapper::print(*Form, Of1);

  YAML::Node Node = YAML::LoadFile(YamlFile1);
  Form.reset(YAMLWrapper::get<FormulaBase>(Node).release());
  YAMLWrapper::print(*Form, Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(FormulaTest, GetBoolBinOpFormula) {
  const std::string YamlFile1("bool-binop-1-formula.yaml");
  const std::string YamlFile2("bool-binop-2-formula.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  ValueType Type = getValueTypeFor<bool>();
  ValueType OpType = getValueTypeFor<int>();
  FormulaKind Kind = FormulaKind::BoolBinOp;

  auto Form = createFormula(Kind, Type, OpType);
  ASSERT_NE(Form.get(), nullptr);
  Form->generate(Set.get());
  YAMLWrapper::print(*Form, Of1);

  YAML::Node Node = YAML::LoadFile(YamlFile1);
  Form.reset(YAMLWrapper::get<FormulaBase>(Node).release());
  YAMLWrapper::print(*Form, Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(FormulaTest, GetIfFormula) {
  const std::string YamlFile1("if-1-formula.yaml");
  const std::string YamlFile2("if-2-formula.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  ValueType Type = getValueTypeFor<int>();
  ValueType OpType = getValueTypeFor<int>();
  FormulaKind Kind = FormulaKind::If;

  auto Form = createFormula(Kind, Type, OpType);
  ASSERT_NE(Form.get(), nullptr);
  Form->generate(Set.get());
  YAMLWrapper::print(*Form, Of1);

  YAML::Node Node = YAML::LoadFile(YamlFile1);
  Form.reset(YAMLWrapper::get<FormulaBase>(Node).release());
  YAMLWrapper::print(*Form, Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

TEST(FormulaTest, GetFeatureFormula) {
  const std::string YamlFile1("feat-1-formula.yaml");
  const std::string YamlFile2("feat-2-formula.yaml");
  std::ofstream Of1(YamlFile1), Of2(YamlFile2);

  FeatureSet::disableAll();
  FeatureSet::enable("cfg_md_static");
  auto Set = FeatureSet::get();

  ValueType Type = getValueTypeFor<int>();
  ValueType OpType = getValueTypeFor<int>();
  FormulaKind Kind = FormulaKind::Feature;

  auto Form = createFormula(Kind, Type, OpType);
  ASSERT_NE(Form.get(), nullptr);
  Form->generate(Set.get());
  YAMLWrapper::print(*Form, Of1);

  YAML::Node Node = YAML::LoadFile(YamlFile1);
  Form.reset(YAMLWrapper::get<FormulaBase>(Node).release());
  YAMLWrapper::print(*Form, Of2);

  ASSERT_TRUE(filesEqual(YamlFile1, YamlFile2));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

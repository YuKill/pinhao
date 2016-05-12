/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Formula.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/Formulas.h"
#include "pinhao/Support/FormulaYAMLWrapper.h"

using namespace pinhao;

uint64_t pinhao::getFormulaKindWeight(FormulaKind Kind) {
  return FormulaKindInfo.at(Kind).second;
}

FormulaKind pinhao::getFormulaKind(std::string String) {
  for (auto &Pair : FormulaKindInfo) {
    if (Pair.second.first == String) 
      return Pair.first;
  }
  assert(false && "There is no such FormulaKind.");
}

std::string pinhao::getFormulaKindString(FormulaKind K) {
  return FormulaKindInfo.at(K).first;
}

OperatorKind pinhao::getOperatorKind(std::string String) {
  int Count = 0;
  for (auto S : OperatorKindString) {
    if (S == String) 
      return static_cast<OperatorKind>(Count);
    ++Count;
  }
  assert(false && "There is no such OperatorKindString.");
}

std::string pinhao::getOperatorKindString(OperatorKind K) {
  return OperatorKindString[static_cast<int>(K)]; 
}

std::unique_ptr<FormulaBase> pinhao::createLitFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new LitFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new LitFormula<double>());
    case ValueType::String: 
      return std::unique_ptr<FormulaBase>(new LitFormula<std::string>());
    case ValueType::Bool: 
      return std::unique_ptr<FormulaBase>(new LitFormula<bool>());
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  } 
}

std::unique_ptr<FormulaBase> pinhao::createArithBinOpFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new ArithBinOpFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new ArithBinOpFormula<double>());
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  } 
}

std::unique_ptr<FormulaBase> pinhao::createBoolBinOpFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new BoolBinOpFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new BoolBinOpFormula<double>());
    case ValueType::String: 
      return std::unique_ptr<FormulaBase>(new BoolBinOpFormula<std::string>());
    case ValueType::Bool: 
      return std::unique_ptr<FormulaBase>(new BoolBinOpFormula<bool>());
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  }
}

std::unique_ptr<FormulaBase> pinhao::createIfFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new IfFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new IfFormula<double>());
    case ValueType::String: 
      return std::unique_ptr<FormulaBase>(new IfFormula<std::string>());
    case ValueType::Bool: 
      return std::unique_ptr<FormulaBase>(new IfFormula<bool>());
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  } 
}

std::unique_ptr<FormulaBase> pinhao::createFeatureFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new FeatureFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new FeatureFormula<double>());
    case ValueType::String: 
      return std::unique_ptr<FormulaBase>(new FeatureFormula<std::string>());
    case ValueType::Bool: 
      return std::unique_ptr<FormulaBase>(new FeatureFormula<bool>());
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  } 
}

std::unique_ptr<FormulaBase> pinhao::createFormula(FormulaKind Kind, ValueType Type, ValueType OpType) {
  switch (Kind) {
    case FormulaKind::Literal:
      return createLitFormula(Type);
    case FormulaKind::BoolBinOp:
      return createBoolBinOpFormula(OpType);
    case FormulaKind::ArithBinOp:
      return createArithBinOpFormula(Type);
    case FormulaKind::If:
      return createIfFormula(Type);
    case FormulaKind::Feature:
      return createFeatureFormula(Type);
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  }
}

static FormulaKind selectFormulaKind(std::vector<FormulaKind> FormulaKinds) {
  int TotalSum = 0;
  for (auto Kind : FormulaKinds)
    TotalSum += getFormulaKindWeight(Kind);

  int PartialSum = 0;
  int Roulette = UniformRandom::getRandomInt(1, TotalSum);
  for (unsigned I = 0; I < FormulaKinds.size(); ++I) {
    PartialSum += getFormulaKindWeight(FormulaKinds[I]); 
    if (PartialSum >= Roulette)
      return FormulaKinds[I];
  }

  return FormulaKinds[FormulaKinds.size() - 1];
}

static ValueType selectValueType(std::vector<ValueType> Types) {
  return Types[UniformRandom::getRandomInt(0, Types.size()-1)];
}

static std::unique_ptr<FormulaBase> generateBoolean(FeatureSet *Set) {
  std::vector<FormulaKind> Kinds = {
    FormulaKind::Literal, FormulaKind::BoolBinOp, FormulaKind::If 
  };

  std::vector<ValueType> OpTypes = {
    ValueType::Bool, ValueType::Int 
  };

  FormulaKind Kind = selectFormulaKind(Kinds);
  ValueType OpType = selectValueType(OpTypes);

  std::unique_ptr<FormulaBase> Created(createFormula(Kind, ValueType::Bool, OpType));
  Created->generate(Set);
  return Created;
}

static std::unique_ptr<FormulaBase> generateNumerical(FeatureSet *Set) {
  std::vector<FormulaKind> Kinds = {
    FormulaKind::Literal, FormulaKind::ArithBinOp, FormulaKind::If, FormulaKind::Feature
  };

  FormulaKind Kind = selectFormulaKind(Kinds);

  std::unique_ptr<FormulaBase> Created(createFormula(Kind, ValueType::Int));
  Created->generate(Set);
  return Created;
}

std::unique_ptr<FormulaBase> pinhao::generateFormula(FeatureSet *Set, ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
    case ValueType::Float: 
      return generateNumerical(Set);

    case ValueType::String: 
      break;

    case ValueType::Bool: 
      return generateBoolean(Set);
  }
  return std::unique_ptr<FormulaBase>(nullptr);
}

void pinhao::simplifyFormula(FormulaBase* &Form) {
  FormulaBase *Simplified = Form->simplify();
  if (Simplified != nullptr) {
    delete Form;
    Form = Simplified;
  }
}

void pinhao::simplifyFormula(std::unique_ptr<FormulaBase> &Form) {
  FormulaBase *Simplified = Form->simplify();
  if (Simplified != nullptr)
    Form.reset(Simplified); 
}

/*
 * ----------------------------------=
 * Class: FomulaBase
 */
bool FormulaBase::isLiteral() const {
  return getKind() == FormulaKind::Literal;
}

bool FormulaBase::isArithBinOp() const {
  return getKind() == FormulaKind::ArithBinOp;
}

bool FormulaBase::isBoolBinOp() const {
  return getKind() == FormulaKind::BoolBinOp;
}

bool FormulaBase::isFeature() const {
  return getKind() == FormulaKind::Feature;
}

bool FormulaBase::isIf() const {
  return getKind() == FormulaKind::If;
}

void FormulaBase::print(std::ostream &Out) {
  YAMLWrapper::print(*this, Out);
}

bool FormulaBase::operator!=(const FormulaBase &Rhs) const {
  return !(*this == Rhs);
}

bool FormulaBase::operator>(const FormulaBase &Rhs) const {
  return (Rhs < *this);
}

bool FormulaBase::operator>=(const FormulaBase &Rhs) const {
  return (*this > Rhs || *this == Rhs);
}

bool FormulaBase::operator<=(const FormulaBase &Rhs) const {
  return (*this < Rhs || *this == Rhs);
}

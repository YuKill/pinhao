/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Formula.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"
#include "pinhao/Support/FormulaYAMLWrapper.h"
#include "pinhao/Support/Types.h"

using namespace pinhao;

FormulaKind pinhao::getFormulaKind(std::string String) {
  int Count = 0;
  for (auto S : FormulaKindString) {
    if (S == String) 
      return static_cast<FormulaKind>(Count);
    ++Count;
  }
  assert(false && "There is no such FormulaKindString.");
}

std::string pinhao::getFormulaKindString(FormulaKind K) {
  return FormulaKindString[static_cast<int>(K)]; 
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

std::unique_ptr<FormulaBase> pinhao::createArithmeticBinOpFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new ArithmeticBinOpFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new ArithmeticBinOpFormula<double>());
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  } 
}

std::unique_ptr<FormulaBase> pinhao::createBooleanBinOpFormula(ValueType Type) {
  switch (Type) {
    case ValueType::Int: 
      return std::unique_ptr<FormulaBase>(new BooleanBinOpFormula<int>());
    case ValueType::Float: 
      return std::unique_ptr<FormulaBase>(new BooleanBinOpFormula<double>());
    case ValueType::String: 
      return std::unique_ptr<FormulaBase>(new BooleanBinOpFormula<std::string>());
    case ValueType::Bool: 
      return std::unique_ptr<FormulaBase>(new BooleanBinOpFormula<bool>());
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
    case FormulaKind::BooleanBinOp:
      return createBooleanBinOpFormula(OpType);
    case FormulaKind::ArithmeticBinOp:
      return createArithmeticBinOpFormula(Type);
    case FormulaKind::If:
      return createIfFormula(Type);
    case FormulaKind::Feature:
      return createFeatureFormula(Type);
    default:
      return std::unique_ptr<FormulaBase>(nullptr);
  }
}

/*
 * ----------------------------------=
 * Class: FomulaBase
 */
void FormulaBase::print(std::ostream &Out) {
  YAMLWrapper::print(*this, Out);
}

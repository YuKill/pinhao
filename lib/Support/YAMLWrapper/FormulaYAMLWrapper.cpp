/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FormulaYAMLWrapper.cpp
 */

#include "pinhao/Support/FormulaYAMLWrapper.h"
#include "pinhao/Support/Types.h"

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

using namespace pinhao;

/*
 * -----------------------------------
 *  Overloading for: FormulaBase
 */
template<>
std::unique_ptr<FormulaBase> YAMLWrapper::get(ConstNode &Node) {
  ValueType Type;
  ValueType OpType;
  FormulaKind Kind;

  Type = static_cast<ValueType>(Node["type"].as<int>());
  Kind = getFormulaKind(Node["kind"].as<std::string>());

  if (Node["op-type"])
    OpType = static_cast<ValueType>(Node["op-type"].as<int>());

  FormulaBase *FB = createFormula(Kind, Type, OpType).release();
  switch (Type) {
    case ValueType::Int:    YAMLWrapper::fill(static_cast<Formula<int>&>(*FB), Node);         break;
    case ValueType::Float:  YAMLWrapper::fill(static_cast<Formula<double>&>(*FB), Node);      break;
    case ValueType::String: YAMLWrapper::fill(static_cast<Formula<std::string>&>(*FB), Node); break;
    case ValueType::Bool:   YAMLWrapper::fill(static_cast<Formula<bool>&>(*FB), Node);        break;
  }
  return std::unique_ptr<FormulaBase>(FB);
}

template<>
void YAMLWrapper::append(const FormulaBase &Value, Emitter &E) {
  switch (Value.getType()) {
    case ValueType::Int:    YAMLWrapper::append(static_cast<const Formula<int>&>(Value), E);         break;
    case ValueType::Float:  YAMLWrapper::append(static_cast<const Formula<double>&>(Value), E);      break;
    case ValueType::String: YAMLWrapper::append(static_cast<const Formula<std::string>&>(Value), E); break;
    case ValueType::Bool:   YAMLWrapper::append(static_cast<const Formula<bool>&>(Value), E);        break;
  }
}

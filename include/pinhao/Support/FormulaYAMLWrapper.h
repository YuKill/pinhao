/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FormulaYAMLWrapper.h
 */

#ifndef PINHAO_FORMULA_YAML_WRAPPER_H
#define PINHAO_FORMULA_YAML_WRAPPER_H

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/YamlOptions.h"
#include "pinhao/Support/Types.h"

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

using namespace pinhao;

/*
 * -----------------------------------
 *  Overloading for: Formula<T>
 */
template <class T>
std::unique_ptr<Formula<T>> get(ConstNode &Node) {
  FormulaKind Kind = getFormulaKind(Node["kind"].as<std::string>());
  switch (Kind) {
    case FormulaKind::Literal:
      return YAMLWrapper::get<LitFormula<T>>(Node);
    case FormulaKind::BooleanBinOp:
      {
        switch (static_cast<ValueType>(Node["op-type"].as<int>())) {
          case ValueType::Int: 
            return YAMLWrapper::get<BooleanBinOpFormula<int>>(Node);
          case ValueType::Float: 
            return YAMLWrapper::get<BooleanBinOpFormula<double>>(Node);
          case ValueType::String: 
            return YAMLWrapper::get<BooleanBinOpFormula<std::string>>(Node);
          case ValueType::Bool: 
            return YAMLWrapper::get<BooleanBinOpFormula<bool>>(Node);
        }
      }
    case FormulaKind::ArithmeticBinOp:
      return YAMLWrapper::get<ArithmeticBinOpFormula<T>>(Node);
    case FormulaKind::If:
      return YAMLWrapper::get<IfFormula<T>>(Node);
    case FormulaKind::Feature:
      return YAMLWrapper::get<FeatureFormula<T>>(Node);
  }
}

template <class T>
void append(const Formula<T> &Value, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "type" << YAML::Value;
  YAMLWrapper::append(Value.getType());
  E << YAML::Key << "kind" << YAML::Value;
  YAMLWrapper::append(getFormulaKindString(Value.getKind()));

  switch (Value.getKind()) {
    case FormulaKind::Literal:
      YAMLWrapper::append<LitFormula<T>>(Value, E);
      break;
    case FormulaKind::BooleanBinOp:
      {
        const BooleanBinOpFormula &BBOF = static_cast<const BooleanBinOpFormula&>(Value);
        E << YAML::Key << "op-type" << YAML::Value;
        YAMLWrapper::append(BBOF.getOperandType());
        switch (BBOF.getOperandType()) {
          case ValueType::Int:
            YAMLWrapper::append<BooleanBinOpFormula<int>>(Value, E);
            break;
          case ValueType::Float:
            YAMLWrapper::append<BooleanBinOpFormula<double>>(Value, E);
            break;
          case ValueType::String:
            YAMLWrapper::append<BooleanBinOpFormula<std::string>>(Value, E);
            break;
          case ValueType::Bool:
            YAMLWrapper::append<BooleanBinOpFormula<bool>>(Value, E);
            break;
        }
        break;
      }
    case FormulaKind::ArithmeticBinOp:
      YAMLWrapper::append<ArithmeticBinOpFormula<T>>(Value, E);
      break;
    case FormulaKind::If:
      YAMLWrapper::append<IfFormula<T>>(Value, E);
      break;
    case FormulaKind::Feature:
      YAMLWrapper::append<FeatureFormula<T>>(Value, E);
      break;
  }

  E << YAML::EndMap;
}

/*
 * -----------------------------------
 *  Overloading for: ArithmeticBinOpFormula
 */
template <class T>
std::unique_ptr<ArithmeticBinOpFormula<T>> get(ConstNode &Node) {
  ArithmeticBinOpFormula *ABOF = new ArithmeticBinOpFormula<T>();
  ABOF->Lhs = std::move(YAMLWrapper::get<Formula<T>>(Node["lhs"]));
  ABOF->Rhs = std::move(YAMLWrapper::get<Formula<T>>(Node["rhs"]));
  ABOF->setOperator(Node["op"].as<int>());
}

template <class T>
void append(const ArithmeticBinOpFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "op" << YAML::Value;
  YAMLWrapper::append(Value.getOperator());
  E << YAML::Key << "lhs" << YAML::Value;
  YAMLWrapper::append(*Value.Lhs);
  E << YAML::Key << "rhs" << YAML::Value;
  YAMLWrapper::append(*Value.Rhs);
}

/*
 * -----------------------------------
 *  Overloading for: BooleanBinOpFormula
 */
template <class T>
std::unique_ptr<BooleanBinOpFormula<T>> get(ConstNode &Node) {
  BooleanBinOpFormula *BBOF = new BooleanBinOpFormula<T>();
  BBOF->Lhs = std::move(YAMLWrapper::get<Formula<T>>(Node["lhs"]));
  BBOF->Rhs = std::move(YAMLWrapper::get<Formula<T>>(Node["rhs"]));
  BBOF->setOperator(Node["op"].as<int>());
}

template <class T>
void append(const BooleanBinOpFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "op" << YAML::Value;
  YAMLWrapper::append(Value.getOperator());
  E << YAML::Key << "lhs" << YAML::Value;
  YAMLWrapper::append(*Value.Lhs);
  E << YAML::Key << "rhs" << YAML::Value;
  YAMLWrapper::append(*Value.Rhs);
}

/*
 * -----------------------------------
 *  Overloading for: IfFormula
 */
template <class T>
std::unique_ptr<IfFormula<T>> get(ConstNode &Node) {
  IfFormula *IF = new IfFormula<T>();
  IfFormula->Condition = std::move(YAMLWrapper::get<Formula<bool>(Node["cond"]));
  IfFormula->ThenBody = std::move(YAMLWrapper::get<Formula<T>(Node["then"]));
  IfFormula->ElseBody = std::move(YAMLWrapper::get<Formula<T>(Node["else"]));
}

template <class T>
void append(const IfFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "cond" << YAML::Value;
  YAMLWrapper::append(*Value.Condition); 
  E << YAML::Key << "then" << YAML::Value;
  YAMLWrapper::append(*Value.ThenBody); 
  E << YAML::Key << "else" << YAML::Value;
  YAMLWrapper::append(*Value.ElseBody); 
}

/*
 * -----------------------------------
 *  Overloading for: LitFormula
 */
template <class T>
std::unique_ptr<LitFormula<T>> get(ConstNode &Node) {
  LitFormula *LF = new LitFormula<T>();
  LF->setValue(Node["value"].as<T>());
  return std::unique_ptr<LitFormula<T>>(LF);
}

template <class T>
void append(const LitFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "value" << YAML::Value;
  YAMLWrapper::append(Value.getValue());
}

#endif

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
void YAMLWrapper::fill(Formula<T> &Value, ConstNode &Node) {
  switch (Value.getKind()) {
    case FormulaKind::Literal:
      YAMLWrapper::fill(static_cast<LitFormula<T>&>(Value), Node);
      break;
    case FormulaKind::BooleanBinOp:
      {
        Formula<bool> &FormulaCast = reinterpret_cast<Formula<bool>&>(Value);
        switch (FormulaCast.getOperandsType()) {
          case ValueType::Int: 
            YAMLWrapper::fill(static_cast<BooleanBinOpFormula<int>&>(FormulaCast), Node);
            break;
          case ValueType::Float: 
            YAMLWrapper::fill(static_cast<BooleanBinOpFormula<double>&>(FormulaCast), Node);
            break;
          case ValueType::String: 
            YAMLWrapper::fill(static_cast<BooleanBinOpFormula<std::string>&>(FormulaCast), Node);
            break;
          case ValueType::Bool: 
            YAMLWrapper::fill(static_cast<BooleanBinOpFormula<bool>&>(FormulaCast), Node);
            break;
        }
      }
    case FormulaKind::ArithmeticBinOp:
      YAMLWrapper::fill(static_cast<ArithmeticBinOpFormula<T>&>(Value), Node);
      break;
    case FormulaKind::If:
      YAMLWrapper::fill(static_cast<IfFormula<T>&>(Value), Node);
      break;
    case FormulaKind::Feature:
      YAMLWrapper::fill(static_cast<FeatureFormula<T>&>(Value), Node);
      break;
  }
}

template <class T>
void YAMLWrapper::append(const Formula<T> &Value, Emitter &E) {
  E << YAML::BeginMap;
  E << YAML::Key << "type" << YAML::Value;
  YAMLWrapper::append(static_cast<int>(Value.getType()), E);
  E << YAML::Key << "kind" << YAML::Value;
  YAMLWrapper::append(getFormulaKindString(Value.getKind()), E);

  switch (Value.getKind()) {
    case FormulaKind::Literal:
      YAMLWrapper::append(static_cast<const LitFormula<T>&>(Value), E);
      break;
    case FormulaKind::BooleanBinOp:
      {
        const Formula<bool> &FormulaCast = reinterpret_cast<const Formula<bool>&>(Value);
        E << YAML::Key << "op-type" << YAML::Value;
        YAMLWrapper::append(static_cast<int>(FormulaCast.getOperandsType()), E);
        switch (FormulaCast.getOperandsType()) {
          case ValueType::Int:
            YAMLWrapper::append(static_cast<const BooleanBinOpFormula<int>&>(FormulaCast), E);
            break;
          case ValueType::Float:
            YAMLWrapper::append(static_cast<const BooleanBinOpFormula<double>&>(FormulaCast), E);
            break;
          case ValueType::String:
            YAMLWrapper::append(static_cast<const BooleanBinOpFormula<std::string>&>(FormulaCast), E);
            break;
          case ValueType::Bool:
            YAMLWrapper::append(static_cast<const BooleanBinOpFormula<bool>&>(FormulaCast), E);
            break;
        }
        break;
      }
    case FormulaKind::ArithmeticBinOp:
      YAMLWrapper::append(static_cast<const ArithmeticBinOpFormula<T>&>(Value), E);
      break;
    case FormulaKind::If:
      YAMLWrapper::append(static_cast<const IfFormula<T>&>(Value), E);
      break;
    case FormulaKind::Feature:
      YAMLWrapper::append(static_cast<const FeatureFormula<T>&>(Value), E);
      break;
  }

  E << YAML::EndMap;
}

/*
 * -----------------------------------
 *  Overloading for: ArithmeticBinOpFormula
 */
template <class T>
void YAMLWrapper::fill(ArithmeticBinOpFormula<T> &Value, ConstNode &Node) {
  Value.Lhs.reset(static_cast<Formula<T>*>(YAMLWrapper::get<FormulaBase>(Node["lhs"]).release()));
  Value.Rhs.reset(static_cast<Formula<T>*>(YAMLWrapper::get<FormulaBase>(Node["rhs"]).release()));
  Value.setOperator(Node["op"].as<int>());
}

template <class T>
void YAMLWrapper::append(const ArithmeticBinOpFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "op" << YAML::Value;
  YAMLWrapper::append(Value.getOperator(), E);
  E << YAML::Key << "lhs" << YAML::Value;
  YAMLWrapper::append(*Value.Lhs, E);
  E << YAML::Key << "rhs" << YAML::Value;
  YAMLWrapper::append(*Value.Rhs, E);
}

/*
 * -----------------------------------
 *  Overloading for: BooleanBinOpFormula
 */
template <class T>
void YAMLWrapper::fill(BooleanBinOpFormula<T> &Value, ConstNode &Node) {
  Value.Lhs.reset(static_cast<Formula<T>*>(YAMLWrapper::get<FormulaBase>(Node["lhs"]).release()));
  Value.Rhs.reset(static_cast<Formula<T>*>(YAMLWrapper::get<FormulaBase>(Node["rhs"]).release()));
  Value.setOperator(Node["op"].as<int>());
}

template <class T>
void YAMLWrapper::append(const BooleanBinOpFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "op" << YAML::Value;
  YAMLWrapper::append(Value.getOperator(), E);
  E << YAML::Key << "lhs" << YAML::Value;
  YAMLWrapper::append(*Value.Lhs, E);
  E << YAML::Key << "rhs" << YAML::Value;
  YAMLWrapper::append(*Value.Rhs, E);
}

/*
 * -----------------------------------
 *  Overloading for: IfFormula
 */
template <class T>
void YAMLWrapper::fill(IfFormula<T> &Value, ConstNode &Node) {
  Value.Condition.reset(static_cast<Formula<bool>*>(YAMLWrapper::get<FormulaBase>(Node["cond"]).release()));
  Value.ThenBody.reset(static_cast<Formula<T>*>(YAMLWrapper::get<FormulaBase>(Node["then"]).release()));
  Value.ElseBody.reset(static_cast<Formula<T>*>(YAMLWrapper::get<FormulaBase>(Node["else"]).release()));
}

template <class T>
void YAMLWrapper::append(const IfFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "cond" << YAML::Value;
  YAMLWrapper::append(*Value.Condition, E); 
  E << YAML::Key << "then" << YAML::Value;
  YAMLWrapper::append(*Value.ThenBody, E); 
  E << YAML::Key << "else" << YAML::Value;
  YAMLWrapper::append(*Value.ElseBody, E); 
}

/*
 * -----------------------------------
 *  Overloading for: LitFormula
 */
template <class T>
void YAMLWrapper::fill(LitFormula<T> &Value, ConstNode &Node) {
  Value.setValue(Node["value"].as<T>());
}

template <class T>
void YAMLWrapper::append(const LitFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "value" << YAML::Value;
  YAMLWrapper::append(Value.getValue(), E);
}

/*
 * -----------------------------------
 *  Overloading for: FeatureFormula<T>
 */
template <class T>
void YAMLWrapper::fill(FeatureFormula<T> &Value, ConstNode &Node) {
  std::pair<std::string, std::string> Pair;
  Pair.first = Node["feature"].as<std::string>();
  if (Node["sub"])
    Pair.second = Node["sub"].as<std::string>();
  Value.FeaturePair = Pair;
}

template <class T>
void YAMLWrapper::append(const FeatureFormula<T> &Value, Emitter &E) {
  E << YAML::Key << "feature" << YAML::Value;
  YAMLWrapper::append(Value.FeaturePair.first, E);
  if (Value.FeaturePair.second == "") {
    E << YAML::Key << "sub" << YAML::Value;
    YAMLWrapper::append(Value.FeaturePair.second, E);
  }
}

#endif

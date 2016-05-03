/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Formula.h
 */

#ifndef PINHAO_FORMULA_H
#define PINHAO_FORMULA_H

#include "pinhao/Features/FeatureSet.h"

#include <string>
#include <vector>

namespace pinhao {

  enum class FormulaKind {
    Literal = 0, 
    BooleanBinOp, 
    ArithmeticBinOp, 
    If,
    Feature
  };

  static const std::vector<std::string> FormulaKindString = {
    "literal", "bool-binop", "arith-binop", "if", "feature"
  }; 

  FormulaKind getFormulaKind(std::string String) {
    int Count = 0;
    for (auto S : FormulaKindString) {
      if (S == String) 
        return static_cast<FormulaKind>(Count);
      ++Count;
    }
    assert(false && "There is no such FormulaKindString.");
  }

  std::string getFormulaKindString(FormulaKind K) {
    return FormulaKindString[static_cast<int>(K)]; 
  }

  enum class OperatorKind {
    PLUS = 0, MIN, MUL, DIV, EQ, NEQ, LT, GT, LEQ, GEQ
  };

  static const std::vector<std::string> OperatorKindString = {
    "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="
  }; 

  OperatorKind getOperatorKind(std::string String) {
    int Count = 0;
    for (auto S : OperatorKindString) {
      if (S == String) 
        return static_cast<OperatorKind>(Count);
      ++Count;
    }
    assert(false && "There is no such OperatorKindString.");
  }

  std::string getOperatorKindString(OperatorKind K) {
    return OperatorKindString[static_cast<int>(K)]; 
  }

  class FormulaBase {
    public:
      virtual ~FormulaBase() {}

      virtual void solveFor(FeatureSet *Set) = 0;
      virtual ValueType getType() const = 0;
      virtual FormulaKind getKind() const = 0;

      void print(std::ostream &Out = std::cout) {
        YAMLWrapper::print(*this, Out);
      }

  };

  template <class T>
    class Formula : public FormulaBase {
      protected:
        T Value;

        static T getFormulaValue(const FormulaBase *Base) {
          const Formula<T> *F = static_cast<const Formula<T>*>(Base);
          return F->getValue();
        }

      public:
        virtual ~Formula() {}
        T &getValue() { return Value; }
        T getValue() const { return Value; }

        virtual ValueType getOperandsType() const {
          return getValueTypeFor<T>(); 
        }

    };

  template <class T, class OpT = T>
    class BinOpFormulaBase : public Formula<T> {
      protected:
        OperatorKind Operator;

        virtual T doOperation(OpT One, OpT Two) = 0;

      public:
        virtual ~BinOpFormulaBase() {}
        std::unique_ptr<Formula<OpT>> Lhs;
        std::unique_ptr<Formula<OpT>> Rhs;

        bool isArithmeticBinOp() { return this->getKind() == FormulaKind::ArithmeticBinOp; }
        bool isBooleanBinOp() { return this->getKind() == FormulaKind::BooleanBinOp; }
        void setOperator(int NewOperator) { Operator = static_cast<OperatorKind>(NewOperator); }
        int getOperator() const { return static_cast<int>(Operator); }
        ValueType getOperandsType() const override { return getValueTypeFor<OpT>(); }

        void solveFor(FeatureSet *Set) override {
          Lhs->solveFor(Set); 
          Rhs->solveFor(Set); 
          this->Value = doOperation(Lhs->getValue(), Rhs->getValue());
        }

    };

  template <class T>
    class ArithmeticBinOpFormula : public BinOpFormulaBase<T, T> {
      protected:
        T doOperation(T One, T Two) override {
          switch (this->Operator) {
            case OperatorKind::PLUS: return One + Two;
            case OperatorKind::MIN:  return One - Two;
            case OperatorKind::MUL:  return One * Two; 
            case OperatorKind::DIV:  return One / Two; 
            default: return 0;
          }
        }

      public:
        ValueType getType() const override { return getValueTypeFor<T>(); }
        FormulaKind getKind() const override { return FormulaKind::ArithmeticBinOp; }

    };

  template <class T>
    class BooleanBinOpFormula : public BinOpFormulaBase<bool, T> {
      protected:
        bool doOperation(T One, T Two) override {
          int OperatorId = static_cast<int>(this->Operator) + static_cast<int>(OperatorKind::DIV);
          switch (static_cast<OperatorKind>(OperatorId)) {
            case OperatorKind::EQ:   return One == Two; 
            case OperatorKind::NEQ:  return One != Two; 
            case OperatorKind::LT:   return One < Two; 
            case OperatorKind::GT:   return One > Two; 
            case OperatorKind::LEQ:  return One <= Two; 
            case OperatorKind::GEQ:  return One >= Two; 
            default: return false;
          }
        }

      public:
        ValueType getType() const override { return getValueTypeFor<bool>(); }
        FormulaKind getKind() const override { return FormulaKind::BooleanBinOp; }

    };

  template <class T>
    class IfFormula : public Formula<T> {
      public:
        std::unique_ptr<Formula<bool>> Condition;
        std::unique_ptr<Formula<T>> ThenBody;
        std::unique_ptr<Formula<T>> ElseBody;

        void solveFor(FeatureSet *Set) override {
          Condition->solveFor(Set);   
          if (Condition->getValue()) {
            ThenBody->solveFor(Set);
            this->Value = ThenBody->getValue();
          } else {
            ElseBody->solveFor(Set);
            this->Value = ElseBody->getValue();
          }
        }

        ValueType getType() const override { return getValueTypeFor<T>(); }
        FormulaKind getKind() const override { return FormulaKind::If; }
    };

  template <class T>
    class LitFormula : public Formula<T> {
      public:
        void setValue(T Value) { this->Value = Value; }
        ValueType getType() const override { return getValueTypeFor<T>(); }
        FormulaKind getKind() const override { return FormulaKind::Literal; }

        void solveFor(FeatureSet *Set) override {}
    };

  template <class T>
    class FeatureFormula : public Formula<T> {
      public:
        std::pair<std::string, std::string> FeaturePair;

        ValueType getType() const override { return getValueTypeFor<T>(); }
        FormulaKind getKind() const override { return FormulaKind::Feature; }

        void solveFor(FeatureSet *Set) override {
          this->Value = Set->getFeature<T>(FeaturePair); 
        }
    };

  static std::unique_ptr<FormulaBase> 
    createLitFormula(ValueType Type) {
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

  static std::unique_ptr<FormulaBase> 
    createArithmeticBinOpFormula(ValueType Type) {
      switch (Type) {
        case ValueType::Int: 
          return std::unique_ptr<FormulaBase>(new ArithmeticBinOpFormula<int>());
        case ValueType::Float: 
          return std::unique_ptr<FormulaBase>(new ArithmeticBinOpFormula<double>());
        default:
          return std::unique_ptr<FormulaBase>(nullptr);
      } 
    }

  static std::unique_ptr<FormulaBase> 
    createBooleanBinOpFormula(ValueType Type) {
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

  static std::unique_ptr<FormulaBase> 
    createIfFormula(ValueType Type) {
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

  static std::unique_ptr<FormulaBase> 
    createFeatureFormula(ValueType Type) {
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

  static std::unique_ptr<FormulaBase> 
    createFormula(FormulaKind Kind, ValueType Type, ValueType OpType = ValueType::Int) {
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

}

#endif

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file BinOpFormula.h
 */

#ifndef PINHAO_BINOP_FORMULA_H
#define PINHAO_BINOP_FORMULA_H

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"
#include "pinhao/MachineLearning/GrammarEvolution/LitFormula.h"

namespace pinhao {

  /**
   * @brief Base class for binary operations.
   */
  template <class T, class OpT = T>
    class BinOpFormulaBase : public Formula<T> {
      protected:
        /// @brief Executes the specified operation, and returns
        /// the value.
        virtual T doOperation(OpT One, OpT Two) = 0;

        /// @brief Gets the maximum value for the operator.
        virtual int getNumberOfOperators() const = 0;
        /// @brief Gets the minimum value for the operator.
        virtual int getMinOperator() const = 0;

      public:
        virtual ~BinOpFormulaBase() {}

        std::unique_ptr<FormulaBase> Lhs;
        std::unique_ptr<FormulaBase> Rhs;
        OperatorKind Operator;

        /// @brief Returns the operator with its offset.
        OperatorKind getOperator() const;
        /// @brief Returns the operator with its offset.
        void setOperator(OperatorKind);

        /// @brief Returns true if it is an @a ArithmeticBinOp.
        bool isArithmeticBinOp() const;
        /// @brief Returns true if it is an @a BooleanBinOp.
        bool isBooleanBinOp() const;
        /// @brief Sets the @a Operator to @a NewId.
        void setOperatorId(int NewId);
        /// @brief Gets the @a Operator casted to @a int.
        int getOperatorId() const; 

        ValueType getOperandsType() const override; 

        FormulaBase *simplify() override; 
        void generate(FeatureSet *Set) override; 
        void solveFor(FeatureSet *Set) override;

    };
}

template <class T, class OpT>
pinhao::OperatorKind pinhao::BinOpFormulaBase<T, OpT>::getOperator() const {
  return static_cast<OperatorKind>(
      static_cast<int>(Operator) + static_cast<int>(getMinOperator()));
}

template <class T, class OpT>
void pinhao::BinOpFormulaBase<T, OpT>::setOperator(OperatorKind K) {
  Operator = static_cast<OperatorKind>(
      static_cast<int>(K) - static_cast<int>(getMinOperator()));
}

template <class T, class OpT>
bool pinhao::BinOpFormulaBase<T, OpT>::isArithmeticBinOp() const {
  return this->getKind() == FormulaKind::ArithBinOp;
}

template <class T, class OpT>
bool pinhao::BinOpFormulaBase<T, OpT>::isBooleanBinOp() const {
  return this->getKind() == FormulaKind::BoolBinOp;
}

template <class T, class OpT>
void pinhao::BinOpFormulaBase<T, OpT>::setOperatorId(int NewId) {
  Operator = static_cast<OperatorKind>(NewId);
}

template <class T, class OpT>
int pinhao::BinOpFormulaBase<T, OpT>::getOperatorId() const {
  return static_cast<int>(Operator);
}

template <class T, class OpT>
pinhao::ValueType pinhao::BinOpFormulaBase<T, OpT>::getOperandsType() const {
  return getValueTypeFor<OpT>();
}

template <class T, class OpT>
pinhao::FormulaBase *pinhao::BinOpFormulaBase<T, OpT>::simplify() {
  simplifyFormula(Lhs);
  simplifyFormula(Rhs);
  if (Lhs->isLiteral() && Rhs->isLiteral()) {
    LitFormula<T> *Simplified = new LitFormula<T>();
    T Value = doOperation(getFormulaValue<OpT>(Lhs.get()), getFormulaValue<OpT>(Rhs.get()));
    Simplified->setValue(Value);
    return Simplified;
  }
  return nullptr;
}

template <class T, class OpT>
void pinhao::BinOpFormulaBase<T, OpT>::generate(FeatureSet *Set) {
  int RandomOperator = UniformRandom::getRandomInt(0, getNumberOfOperators()-1);
  Operator = static_cast<OperatorKind>(RandomOperator);
  Lhs = generateFormula(Set, getValueTypeFor<OpT>());
  Rhs = generateFormula(Set, getValueTypeFor<OpT>());
}

template <class T, class OpT>
void pinhao::BinOpFormulaBase<T, OpT>::solveFor(FeatureSet *Set) {
  Lhs->solveFor(Set); 
  Rhs->solveFor(Set); 
  this->Value = doOperation(getFormulaValue<OpT>(Lhs.get()), getFormulaValue<OpT>(Rhs.get()));
}

namespace pinhao {

  /**
   * @brief Represents all arithmetics operations (supported). Only numerical
   * operands type permitted.
   */
  template <class T>
    class ArithBinOpFormula : public BinOpFormulaBase<T, T> {
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

        int getNumberOfOperators() const override {
          return static_cast<int>(OperatorKind::DIV) + 1; 
        }

        int getMinOperator() const override {
          return static_cast<int>(OperatorKind::PLUS); 
        }

      public:
        ValueType getType() const override {
          return getValueTypeFor<T>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::ArithBinOp;
        }

    };

  /**
   * @brief Represents all logical operations (supported).
   */
  template <class T>
    class BoolBinOpFormula : public BinOpFormulaBase<bool, T> {
      protected:
        bool doOperation(T One, T Two) override {
          int OperatorId = this->getOperatorId() + static_cast<int>(OperatorKind::DIV);
          switch (static_cast<OperatorKind>(OperatorId)) {
            case OperatorKind::LT:   return One < Two; 
            case OperatorKind::GT:   return One > Two; 
            case OperatorKind::LEQ:  return One <= Two; 
            case OperatorKind::GEQ:  return One >= Two; 
            case OperatorKind::EQ:   return One == Two; 
            case OperatorKind::NEQ:  return One != Two; 
            default: return false;
          }
        }

        int getNumberOfOperators() const override {
          return static_cast<int>(OperatorKind::NEQ) - static_cast<int>(OperatorKind::LT) + 1; 
        }

        int getMinOperator() const override {
          return static_cast<int>(OperatorKind::LT); 
        }

      public:
        ValueType getType() const override {
          return getValueTypeFor<bool>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::BoolBinOp;
        }

    };

  /**
   * @brief The operations that can only be applied to two @a Boolean operands.
   */
  template<>
    class BoolBinOpFormula<bool> : public BinOpFormulaBase<bool, bool> {
      protected:
        bool doOperation(bool One, bool Two) override {
          int OperatorId = this->getOperatorId() + static_cast<int>(OperatorKind::DIV);
          switch (static_cast<OperatorKind>(OperatorId)) {
            case OperatorKind::EQ:  return One == Two; 
            case OperatorKind::NEQ: return One != Two; 
            case OperatorKind::AND: return One && Two; 
            case OperatorKind::OR:  return One || Two; 
            default: return false;
          }
        }

        int getNumberOfOperators() const override {
          return static_cast<int>(OperatorKind::OR) - static_cast<int>(OperatorKind::EQ) + 1; 
        }

        int getMinOperator() const override {
          return static_cast<int>(OperatorKind::EQ); 
        }

      public:
        ValueType getType() const override {
          return getValueTypeFor<bool>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::BoolBinOp;
        }

    };

}

#endif

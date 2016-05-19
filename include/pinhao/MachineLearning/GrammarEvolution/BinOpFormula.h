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

        /// @brief Checks if there is a division by zero. If there is,
        /// it replaces by another literal.
        virtual void checkZeroDivision() = 0;

        /// @brief Constructs the specific child type.
        virtual BinOpFormulaBase<T, OpT> *getChildConstructor() = 0;

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

        void evolve(FeatureSet*, Evolution*) override; 
        FormulaBase *simplify() override; 
        void generate(FeatureSet *Set) override; 
        void solveFor(FeatureSet *Set) override;

        bool operator==(const FormulaBase &Rhs) const override;
        bool operator<(const FormulaBase &Rhs) const override;

        std::unique_ptr<FormulaBase> clone() override;

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
void pinhao::BinOpFormulaBase<T, OpT>::evolve(FeatureSet *Set, Evolution *Ev) {
  Ev->evolve({&Lhs, &Rhs});
}

template <class T, class OpT>
pinhao::FormulaBase *pinhao::BinOpFormulaBase<T, OpT>::simplify() {
  simplifyFormula(Lhs);
  simplifyFormula(Rhs);
  checkZeroDivision();
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
  checkZeroDivision();
}

template <class T, class OpT>
void pinhao::BinOpFormulaBase<T, OpT>::solveFor(FeatureSet *Set) {
  Lhs->solveFor(Set); 
  Rhs->solveFor(Set); 
  this->Value = doOperation(getFormulaValue<OpT>(Lhs.get()), getFormulaValue<OpT>(Rhs.get()));
}

template <class T, class OpT>
bool pinhao::BinOpFormulaBase<T, OpT>::operator==(const FormulaBase &Rhs) const {
  auto &RhsCast = static_cast<const Formula<T>&>(Rhs);
  if (this->getKind() != RhsCast.getKind() || this->getType() != RhsCast.getType() || 
      this->getOperandsType() != RhsCast.getOperandsType())
    return false;
  auto &RhsBOCast = static_cast<const BinOpFormulaBase<T, OpT>&>(Rhs);
  return *this->Lhs == *RhsBOCast.Lhs && *this->Rhs == *RhsBOCast.Rhs;
}

template <class T, class OpT>
bool pinhao::BinOpFormulaBase<T, OpT>::operator<(const FormulaBase &Rhs) const {
  auto &RhsCast = static_cast<const Formula<T>&>(Rhs);
  if (this->getKind() != RhsCast.getKind()) 
    return static_cast<int>(this->getKind()) < static_cast<int>(RhsCast.getKind());
  if (this->getType() != RhsCast.getType()) 
    return static_cast<int>(this->getType()) < static_cast<int>(RhsCast.getType());
  if (this->getOperandsType() != RhsCast.getOperandsType()) 
    return static_cast<int>(this->getOperandsType()) < static_cast<int>(RhsCast.getOperandsType());

  auto &RhsBOCast = static_cast<const BinOpFormulaBase<T, OpT>&>(Rhs);
  if (*this->Lhs != *RhsBOCast.Lhs)
    return *this->Lhs < *RhsBOCast.Lhs;
  if (*this->Rhs != *RhsBOCast.Rhs)
    return *this->Rhs < *RhsBOCast.Rhs;
  return false;
}

template <class T, class OpT>
std::unique_ptr<pinhao::FormulaBase> pinhao::BinOpFormulaBase<T, OpT>::clone() {
  auto Clone = getChildConstructor();
  Clone->Lhs = Lhs->clone();
  Clone->Rhs = Rhs->clone();
  Clone->Operator = Operator;
  return std::unique_ptr<FormulaBase>(Clone);
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
            case OperatorKind::DIV: {
                                      if (!Two) return 0;
                                      return One / Two; 
                                    }
            default: return 0;
          }
        }

        int getNumberOfOperators() const override {
          return static_cast<int>(OperatorKind::DIV) + 1; 
        }

        int getMinOperator() const override {
          return static_cast<int>(OperatorKind::PLUS); 
        }

        void checkZeroDivision() override {
          if (this->getOperator() == OperatorKind::DIV) {
            while (this->Rhs->isLiteral() && getFormulaValue<T>(this->Rhs.get()) == 0)
              this->Rhs->generate(nullptr);
          }
        }

        BinOpFormulaBase<T, T> *getChildConstructor() {
          return new ArithBinOpFormula<T>();
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

        void checkZeroDivision() override {}

        BinOpFormulaBase<bool, T> *getChildConstructor() {
          return new BoolBinOpFormula<T>();
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

        void checkZeroDivision() override {}

        BinOpFormulaBase<bool, bool> *getChildConstructor() {
          return new BoolBinOpFormula<bool>();
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

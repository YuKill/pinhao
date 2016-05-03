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

  /**
   * @brief Enumeration of @a Formula types.
   */
  enum class FormulaKind {
    Literal = 0,      ///< Literal formula.
    BooleanBinOp,     ///< Logical binary operation formula.
    ArithmeticBinOp,  ///< Arithmetic binary operation formula.
    If,               ///< If formula.
    Feature           ///< Feature formula.
  };

  /**
   * @brief Vector with the @a std::string names of the @a FormulaKinds
   */
  static const std::vector<std::string> FormulaKindString = {
    "literal", "bool-binop", "arith-binop", "if", "feature"
  }; 

  /// @brief Gets the corresponding @a FormulaKind for the @a std::string.
  FormulaKind getFormulaKind(std::string); 
  /// @brief Gets the corresponding @a std::string for the @a FormulaKind.
  std::string getFormulaKindString(FormulaKind);

  /**
   * @brief Enumeration of the kinds of operators available.
   */
  enum class OperatorKind {
    PLUS = 0, MIN, MUL, DIV, EQ, NEQ, LT, GT, LEQ, GEQ
  };

  /**
   * @brief Vector with the @a std::string representation of its 
   * @a OperatorKind.
   */
  static const std::vector<std::string> OperatorKindString = {
    "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="
  }; 

  /// @brief Gets the corresponding @a OperatorKind for the @a std::string.
  OperatorKind getOperatorKind(std::string); 
  /// @brief Gets the corresponding @a std::string for the @a OperatorKind.
  std::string getOperatorKindString(OperatorKind);

  /**
   * @brief The base class for all @a Formula types. A @a Formula is a
   * statement which when evaluated for a @a FeatureSet, it yields a value.
   */
  class FormulaBase {
    public:
      virtual ~FormulaBase() {}

      /// @brief Gets the type of the @a Formula (what is the returning type).
      virtual ValueType getType() const = 0;
      /// @brief Gets a @a FormulaKind of this object.
      virtual FormulaKind getKind() const = 0;
      /// @brief Evaluates the @a Formula for the @a Set.
      virtual void solveFor(FeatureSet *Set) = 0;

      /// @brief Prints to an @a std::ostream object the information of this
      /// @a Formula.
      void print(std::ostream &Out = std::cout); 

  };

  /**
   * @brief The base template class for the @a Formulas.
   */
  template <class T>
    class Formula : public FormulaBase {
      protected:
        /// @brief The value that is yielded after evaluation.
        T Value;

        /// @brief A wrapper that gets the value yielded.
        static T getFormulaValue(const FormulaBase *Base) {
          const Formula<T> *F = static_cast<const Formula<T>*>(Base);
          return F->getValue();
        }

      public:
        virtual ~Formula() {}

        /// @brief Gets a reference to the value.
        T &getValue() {
          return Value;
        }

        /// @brief Gets a copy of the value.
        T getValue() const {
          return Value;
        }

        /// @brief Return the type of the operands. If it has none,
        /// it returns the type of the @a Formula.
        virtual ValueType getOperandsType() const {
          return getValueTypeFor<T>(); 
        }

    };

  /**
   * @brief Base class for binary operations.
   */
  template <class T, class OpT = T>
    class BinOpFormulaBase : public Formula<T> {
      protected:
        /// @brief Executes the specified operation, and returns
        /// the value.
        virtual T doOperation(OpT One, OpT Two) = 0;

      public:
        virtual ~BinOpFormulaBase() {}

        std::unique_ptr<Formula<OpT>> Lhs;
        std::unique_ptr<Formula<OpT>> Rhs;
        OperatorKind Operator;

        /// @brief Returns true if it is an @a ArithmeticBinOp.
        bool isArithmeticBinOp() const {
          return this->getKind() == FormulaKind::ArithmeticBinOp;
        }

        /// @brief Returns true if it is an @a BooleanBinOp.
        bool isBooleanBinOp() const {
          return this->getKind() == FormulaKind::BooleanBinOp;
        }

        /// @brief Sets the @a Operator to @a NewId.
        void setOperatorId(int NewId) {
          Operator = static_cast<OperatorKind>(NewId);
        }

        /// @brief Gets the @a Operator casted to @a int.
        int getOperatorId() const {
          return static_cast<int>(Operator);
        }

        ValueType getOperandsType() const override {
          return getValueTypeFor<OpT>();
        }

        void solveFor(FeatureSet *Set) override {
          Lhs->solveFor(Set); 
          Rhs->solveFor(Set); 
          this->Value = doOperation(Lhs->getValue(), Rhs->getValue());
        }

    };

  /**
   * @brief Represents all arithmetics operations (supported). Only numerical
   * operands type permitted.
   */
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
        ValueType getType() const override {
          return getValueTypeFor<T>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::ArithmeticBinOp;
        }

    };

  /**
   * @brief Represents all logical operations (supported).
   */
  template <class T>
    class BooleanBinOpFormula : public BinOpFormulaBase<bool, T> {
      protected:
        bool doOperation(T One, T Two) override {
          int OperatorId = this->getOperatorId() + static_cast<int>(OperatorKind::DIV);
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
        ValueType getType() const override {
          return getValueTypeFor<bool>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::BooleanBinOp;
        }

    };

  /**
   * @brief The @a If @a Formula.
   * @details
   * It evaluates the Condition, which if true, returns the value yielded
   * by the @a ThenBody @a Formula. Else, returns the value of the @a ElseBody
   * @a Formula.
   */
  template <class T>
    class IfFormula : public Formula<T> {
      public:
        std::unique_ptr<Formula<bool>> Condition;
        std::unique_ptr<Formula<T>> ThenBody;
        std::unique_ptr<Formula<T>> ElseBody;

        ValueType getType() const override {
          return getValueTypeFor<T>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::If;
        }

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

    };

  /**
   * @brief Holds literal of the types.
   */
  template <class T>
    class LitFormula : public Formula<T> {
      public:
        /// @brief Sets the literal value.
        void setValue(T Value) {
          this->Value = Value;
        }

        ValueType getType() const override {
          return getValueTypeFor<T>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::Literal;
        }

        void solveFor(FeatureSet *Set) override {}

    };

  /**
   * @brief Holds a pair of @a std::strings. The first represents the
   * feature, and the second represents the sub-feature (if there is one).
   */
  template <class T>
    class FeatureFormula : public Formula<T> {
      public:
        std::pair<std::string, std::string> FeaturePair;

        ValueType getType() const override {
          return getValueTypeFor<T>();
        }

        FormulaKind getKind() const override {
          return FormulaKind::Feature;
        }

        void solveFor(FeatureSet *Set) override {
          this->Value = Set->getFeature<T>(FeaturePair); 
        }

    };

  /// @brief Creates a @a LitFormula of type @a Type.
  std::unique_ptr<FormulaBase> createLitFormula(ValueType);
  /// @brief Creates a @a ArithmeticBinOpFormula whose result is of type @a Type.
  std::unique_ptr<FormulaBase> createArithmeticBinOpFormula(ValueType);
  /// @brief Creates a @a BooleanBinOpFormula whose operands are of type @a Type.
  std::unique_ptr<FormulaBase> createBooleanBinOpFormula(ValueType);
  /// @brief Creates a @a IfFormula whose result is of type @a Type.
  std::unique_ptr<FormulaBase> createIfFormula(ValueType);
  /// @brief Creates a @a FeatureFormula whose value is of type @a Type.
  std::unique_ptr<FormulaBase> createFeatureFormula(ValueType);

  /**
   * @brief Creates a @a Formula of a kind. For the @a BooleanBinOpFormula,
   * the operands type must be specified.
   *  
   *  @param Kind The kind of the @a Formula to be created.
   *  @param Type The type of the @a Formula.
   *  @param OpType The type of the operands.
   *
   *  @return A @a std::unique_ptr to the abstract base class (@a FeatureBase).
   */
  std::unique_ptr<FormulaBase> createFormula(FormulaKind, ValueType, ValueType = ValueType::Int); 

}

#endif

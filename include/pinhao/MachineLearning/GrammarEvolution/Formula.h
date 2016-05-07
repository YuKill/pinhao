/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Formula.h
 */

#ifndef PINHAO_FORMULA_H
#define PINHAO_FORMULA_H

#include "pinhao/Features/FeatureSet.h"
#include "pinhao/Support/Random.h"
#include "pinhao/Support/Types.h"

#include <string>
#include <vector>

namespace pinhao {

  /**
   * @brief Enumeration of @a Formula types.
   */
  enum class FormulaKind {
    Literal = 0,    ///< Literal formula.
    BoolBinOp,      ///< Logical binary operation formula between booleans.
    ArithBinOp,     ///< Arithmetic binary operation formula.
    If,             ///< If formula.
    Feature         ///< Feature formula.
  };

  /**
   * @brief Vector with the @a std::string names of the @a FormulaKinds
   */
  static const std::map<FormulaKind, std::pair<std::string, uint64_t>> FormulaKindInfo = {
    { FormulaKind::Literal,     { "literal",      5 } },
    { FormulaKind::BoolBinOp,   { "bool-binop",   2 } },
    { FormulaKind::ArithBinOp,  { "arith-binop",  2 } },
    { FormulaKind::If,          { "if",           1 } },
    { FormulaKind::Feature,     { "feature",      4 } }
  }; 

  /// @brief Gets the corresponding weight for the @a FormulaKind.
  uint64_t getFormulaKindWeight(FormulaKind); 
  /// @brief Gets the corresponding @a FormulaKind for the @a std::string.
  FormulaKind getFormulaKind(std::string); 
  /// @brief Gets the corresponding @a std::string for the @a FormulaKind.
  std::string getFormulaKindString(FormulaKind);

  /**
   * @brief Enumeration of the kinds of operators available.
   */
  enum class OperatorKind {
    PLUS = 0,
    MIN,
    MUL,
    DIV,
    LT,
    GT,
    LEQ,
    GEQ,
    EQ,
    NEQ,
    AND,
    OR
  };

  /**
   * @brief Vector with the @a std::string representation of its 
   * @a OperatorKind.
   */
  static const std::vector<std::string> OperatorKindString = {
    "+",
    "-",
    "*",
    "/",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "&&",
    "||"
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

      /// @brief Returns true if it is a Literal formula.
      virtual bool isLiteral() const;
      /// @brief Returns true if it is a ArithBinOp formula.
      virtual bool isArithBinOp() const;
      /// @brief Returns true if it is a BoolBinOp formula.
      virtual bool isBoolBinOp() const;
      /// @brief Returns true if it is a Feature formula.
      virtual bool isFeature() const;
      /// @brief Returns true if it is a If formula.
      virtual bool isIf() const;

      /// @brief Randomizes the generations of a @a Formula.
      virtual FormulaBase *simplify() = 0;
      /// @brief Randomizes the generations of a @a Formula.
      virtual void generate(FeatureSet *Set) = 0;
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
        static T getFormulaValue(const FormulaBase *Base); 

      public:
        virtual ~Formula() {}

        /// @brief Gets a reference to the value.
        T &getValue(); 
        /// @brief Gets a copy of the value.
        T getValue() const;

        /// @brief Return the type of the operands. If it has none,
        /// it returns the type of the @a Formula.
        virtual ValueType getOperandsType() const; 

    };

  /// @brief Creates a @a LitFormula of type @a Type.
  std::unique_ptr<FormulaBase> createLitFormula(ValueType);
  /// @brief Creates a @a ArithmeticBinOpFormula whose result is of type @a Type.
  std::unique_ptr<FormulaBase> createArithBinOpFormula(ValueType);
  /// @brief Creates a @a BooleanBinOpFormula whose operands are of type @a Type.
  std::unique_ptr<FormulaBase> createBoolBinOpFormula(ValueType);
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

  std::unique_ptr<FormulaBase> generateFormula(FeatureSet*, ValueType);

  template <class T> 
    T getFormulaValue(const FormulaBase*); 

  void simplifyFormula(std::unique_ptr<FormulaBase>&);

}

template <class T>
T pinhao::getFormulaValue(const FormulaBase *Base) {
  const Formula<T> *F = static_cast<const Formula<T>*>(Base);
  return F->getValue();
}

template <class T>
T &pinhao::Formula<T>::getValue() {
  return Value;
}

template <class T>
T pinhao::Formula<T>::getValue() const {
  return Value;
}

template <class T>
pinhao::ValueType pinhao::Formula<T>::getOperandsType() const {
  return getValueTypeFor<T>(); 
}

#endif

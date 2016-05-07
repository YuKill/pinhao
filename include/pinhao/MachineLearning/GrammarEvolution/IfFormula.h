/*-------------------------- PINHAO project --------------------------*/

/**
 * @file IfFormula.h
 */

#ifndef PINHAO_IF_FORMULA_H
#define PINHAO_IF_FORMULA_H

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

namespace pinhao {

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
        std::unique_ptr<FormulaBase> Condition;
        std::unique_ptr<FormulaBase> ThenBody;
        std::unique_ptr<FormulaBase> ElseBody;

        ValueType getType() const override;
        FormulaKind getKind() const override; 

        FormulaBase *simplify() override; 
        void generate(FeatureSet *Set) override; 
        void solveFor(FeatureSet *Set) override; 

    };

}

template <class T>
pinhao::ValueType pinhao::IfFormula<T>::getType() const {
  return getValueTypeFor<T>();
}

template <class T>
pinhao::FormulaKind pinhao::IfFormula<T>::getKind() const {
  return FormulaKind::If;
}

template <class T>
pinhao::FormulaBase *pinhao::IfFormula<T>::simplify() {
  simplifyFormula(Condition);
  simplifyFormula(ThenBody);
  simplifyFormula(ElseBody);
  if (Condition->isLiteral()) {
    if (getFormulaValue<bool>(Condition.get()))
      return ThenBody.release();
    return ElseBody.release();
  }
  return nullptr;
}

template <class T>
void pinhao::IfFormula<T>::generate(FeatureSet *Set) {
  Condition = generateFormula(Set, ValueType::Bool);
  ThenBody = generateFormula(Set, getValueTypeFor<T>());
  ElseBody = generateFormula(Set, getValueTypeFor<T>());
}

template <class T>
void pinhao::IfFormula<T>::solveFor(FeatureSet *Set) {
  Condition->solveFor(Set);   
  if (getFormulaValue<bool>(Condition.get())) {
    ThenBody->solveFor(Set);
    this->Value = getFormulaValue<T>(ThenBody.get());
  } else {
    ElseBody->solveFor(Set);
    this->Value = getFormulaValue<T>(ElseBody.get());
  }
}

#endif

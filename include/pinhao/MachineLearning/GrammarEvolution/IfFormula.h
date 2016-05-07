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
        std::unique_ptr<Formula<bool>> Condition;
        std::unique_ptr<Formula<T>> ThenBody;
        std::unique_ptr<Formula<T>> ElseBody;

        ValueType getType() const override;
        FormulaKind getKind() const override; 

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
void pinhao::IfFormula<T>::generate(FeatureSet *Set) {
  Condition.reset(static_cast<Formula<bool>*>(generateFormula(Set, ValueType::Bool).release()));
  ThenBody.reset(static_cast<Formula<T>*>(generateFormula(Set, getValueTypeFor<T>()).release()));
  ElseBody.reset(static_cast<Formula<T>*>(generateFormula(Set, getValueTypeFor<T>()).release()));
}

template <class T>
void pinhao::IfFormula<T>::solveFor(FeatureSet *Set) {
  Condition->solveFor(Set);   
  if (Condition->getValue()) {
    ThenBody->solveFor(Set);
    this->Value = ThenBody->getValue();
  } else {
    ElseBody->solveFor(Set);
    this->Value = ElseBody->getValue();
  }
}

#endif

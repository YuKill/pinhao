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

        void evolve(FeatureSet*, Evolution*) override; 
        FormulaBase *simplify() override; 
        void generate(FeatureSet *Set) override; 
        void solveFor(FeatureSet *Set) override; 

        bool operator==(const FormulaBase &Rhs) const override;
        bool operator<(const FormulaBase &Rhs) const override;

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
void pinhao::IfFormula<T>::evolve(FeatureSet *Set, Evolution *Ev) {
  Ev->evolve({&Condition, &ThenBody, &ElseBody});
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

template <class T>
bool pinhao::IfFormula<T>::operator==(const FormulaBase &Rhs) const {
  if (this->getKind() != Rhs.getKind() || this->getType() != Rhs.getType())
    return false;
  auto &RhsCast = static_cast<const IfFormula<T>&>(Rhs);
  return *this->Condition == *RhsCast.Condition && 
    *this->ThenBody == *RhsCast.ThenBody && 
    *this->ElseBody == *RhsCast.ElseBody;
}

template <class T>
bool pinhao::IfFormula<T>::operator<(const FormulaBase &Rhs) const {
  if (this->getKind() != Rhs.getKind()) 
    return static_cast<int>(this->getKind()) < static_cast<int>(Rhs.getKind());
  if (this->getType() != Rhs.getType()) 
    return static_cast<int>(this->getType()) < static_cast<int>(Rhs.getType());

  auto &RhsCast = static_cast<const IfFormula<T>&>(Rhs);
  if (*this->Condition != *RhsCast.Condition)
    return *this->Condition < *RhsCast.Condition;
  if (*this->ThenBody != *RhsCast.ThenBody)
    return *this->ThenBody < *RhsCast.ThenBody;
  if (*this->ElseBody != *RhsCast.ElseBody)
    return *this->ElseBody < *RhsCast.ElseBody;
  return false;
}

#endif

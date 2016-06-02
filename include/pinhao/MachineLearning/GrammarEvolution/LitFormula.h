/*-------------------------- PINHAO project --------------------------*/

/**
 * @file LitFormula.h
 */

#ifndef PINHAO_LIT_FORMULA_H
#define PINHAO_LIT_FORMULA_H

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

namespace pinhao {

  template <class T> T generateLiteral(); 

  template<> int generateLiteral<int>();
  template<> bool generateLiteral<bool>();
  template<> double generateLiteral<double>();
  template<> std::string generateLiteral<std::string>();

  /**
   * @brief Holds literal of the types.
   */
  template <class T>
    class LitFormula : public Formula<T> {
      public:
        /// @brief Sets the literal value.
        void setValue(T Value); 

        ValueType getType() const override; 

        FormulaKind getKind() const override; 

        void evolve(Evolution*) override; 
        FormulaBase *simplify() override; 
        void generate(FeatureSet *Set) override; 
        void solveFor(FeatureSet *Set) override; 

        bool operator==(const FormulaBase &Rhs) const  override;
        bool operator<(const FormulaBase &Rhs) const override;

        std::unique_ptr<FormulaBase> clone() override;

    };

}

template <class T>
void pinhao::LitFormula<T>::setValue(T Value) {
  this->Value = Value;
}

template <class T>
void pinhao::LitFormula<T>::evolve(Evolution *Ev) {
  Ev->evolve(this->Value);
}

template <class T>
pinhao::FormulaBase *pinhao::LitFormula<T>::simplify() {
  return nullptr;
}

template <class T>
pinhao::ValueType pinhao::LitFormula<T>::getType() const {
  return getValueTypeFor<T>();
}

template <class T>
pinhao::FormulaKind pinhao::LitFormula<T>::getKind() const {
  return FormulaKind::Literal;
}

template <class T>
void pinhao::LitFormula<T>::generate(FeatureSet *Set) {
  this->Value = generateLiteral<T>();
}

template <class T>
void pinhao::LitFormula<T>::solveFor(FeatureSet *Set) {

}

template <class T>
bool pinhao::LitFormula<T>::operator==(const FormulaBase &Rhs) const {
  if (this->getKind() != Rhs.getKind() || this->getType() != Rhs.getType())
    return false;
  auto &RhsCast = static_cast<const Formula<T>&>(Rhs);
  return this->getValue() == RhsCast.getValue();
}

template <class T>
bool pinhao::LitFormula<T>::operator<(const FormulaBase &Rhs) const {
  if (this->getKind() != Rhs.getKind()) 
    return static_cast<int>(this->getKind()) < static_cast<int>(Rhs.getKind());
  if (this->getType() != Rhs.getType()) 
    return static_cast<int>(this->getType()) < static_cast<int>(Rhs.getType());
  auto &RhsCast = static_cast<const Formula<T>&>(Rhs);
  return this->getValue() < RhsCast.getValue();
}

template <class T>
std::unique_ptr<pinhao::FormulaBase> pinhao::LitFormula<T>::clone() {
  auto Clone = new LitFormula<T>();
  Clone->setValue(this->getValue());
  return std::unique_ptr<FormulaBase>(Clone);
}

#endif

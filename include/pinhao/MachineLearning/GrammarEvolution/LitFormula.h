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

        void generate(FeatureSet *Set) override; 
        void solveFor(FeatureSet *Set) override; 

    };

}

template <class T>
void pinhao::LitFormula<T>::setValue(T Value) {
  this->Value = Value;
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

#endif

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureFormula.h
 */

#ifndef PINHAO_FEATURE_FORMULA_H
#define PINHAO_FEATURE_FORMULA_H

#include "pinhao/MachineLearning/GrammarEvolution/Formula.h"

namespace pinhao {

  /**
   * @brief Holds a pair of @a std::strings. The first represents the
   * feature, and the second represents the sub-feature (if there is one).
   */
  template <class T>
    class FeatureFormula : public Formula<T> {
      public:
        std::pair<std::string, std::string> FeaturePair;

        ValueType getType() const override;
        FormulaKind getKind() const override;

        void generate(FeatureSet *Set) override;
        void solveFor(FeatureSet *Set) override; 

    };

}

template <class T>
pinhao::ValueType pinhao::FeatureFormula<T>::getType() const {
  return getValueTypeFor<T>();
}

template <class T>
pinhao::FormulaKind pinhao::FeatureFormula<T>::getKind() const {
  return FormulaKind::Feature;
}

template <class T>
void pinhao::FeatureFormula<T>::generate(FeatureSet *Set) {
  auto Total = Set->count(getValueTypeFor<T>());
  auto Index = UniformRandom::getRandomInt(0, Total-1);
  FeaturePair = *Set->get(Index, getValueTypeFor<T>());
}

template <class T>
void pinhao::FeatureFormula<T>::solveFor(FeatureSet *Set) {
  this->Value = Set->getFeature<T>(FeaturePair); 
}

#endif

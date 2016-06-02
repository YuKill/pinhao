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

        void evolve(Evolution*) override;
        FormulaBase *simplify() override;
        void generate(FeatureSet *Set) override;
        void solveFor(FeatureSet *Set) override; 

        bool operator==(const FormulaBase &Rhs) const override;
        bool operator<(const FormulaBase &Rhs) const override;

        std::unique_ptr<FormulaBase> clone() override;

    };

}

template <class T>
void pinhao::FeatureFormula<T>::evolve(Evolution *Ev) {
  Ev->evolve(FeaturePair);
}

template <class T>
pinhao::FormulaBase *pinhao::FeatureFormula<T>::simplify() {
  return nullptr;
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

template <class T>
bool pinhao::FeatureFormula<T>::operator==(const FormulaBase &Rhs) const {
  if (this->getKind() != Rhs.getKind() && this->getType() != Rhs.getType())
    return false;
  auto &RhsCast = static_cast<const FeatureFormula<T>&>(Rhs);
  return this->FeaturePair == RhsCast.FeaturePair;
}

template <class T>
bool pinhao::FeatureFormula<T>::operator<(const FormulaBase &Rhs) const {
  if (this->getKind() != Rhs.getKind())
    return static_cast<int>(this->getKind()) < static_cast<int>(Rhs.getKind());
  if (this->getType() != Rhs.getType())
    return static_cast<int>(this->getType()) < static_cast<int>(Rhs.getType());
  auto &RhsCast = static_cast<const FeatureFormula<T>&>(Rhs);
  return this->FeaturePair < RhsCast.FeaturePair;
}

template <class T>
std::unique_ptr<pinhao::FormulaBase> pinhao::FeatureFormula<T>::clone() {
  auto Clone = new FeatureFormula<T>();
  Clone->FeaturePair = FeaturePair;
  return std::unique_ptr<FormulaBase>(Clone);
}

#endif

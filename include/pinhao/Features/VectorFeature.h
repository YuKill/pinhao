/*-------------------------- PINHAO project --------------------------*/

/**
 * @file VectorFeature.h
 */

#ifndef PINHAO_VECTOR_FEATURE_H
#define PINHAO_VECTOR_FEATURE_H

#include "pinhao/Features/Features.h"

using namespace pinhao;

namespace pinhao {

  /**
   * @brief Implements a template vector feature class.
   *
   * @details
   * All the vector-like features must implement this class,
   * giving its type as the template.
   */
  template <class ElemType>
    class VectorFeature : public LinearFeature<ElemType> {
      protected:
        /// @brief The vector container of the feature itself.
        std::vector<ElemType> TheFeature;

      public:
        virtual ~VectorFeature() {};

        VectorFeature(FeatureInfo *Info) : LinearFeature<ElemType>(Info) {
          assert(this->isComposite() && "VectorFeature must have CompositeFeatureInfo.");
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          TheFeature = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0); 
        }

        /**
         * @brief Sets the sub-feature with name @a FeatureName to
         * @a Elem.
         *
         * @param FeatureName The name of the desired sub-feature.
         * @param Elem The element that will replace the old element.
         */
        void setValueOf(std::string FeatureName, ElemType Elem) override;

        /**
         * @brief Gets the sub-feature, which has type @a ElemType, with
         * name @a FeatureName.
         *
         * @param FeatureName The name of the desired sub-feature.
         * @return A pointer to the sub-feature value with const modifier.
         */
        const ElemType& getValueOf(std::string FeatureName) override;

        virtual void appendYaml(YAML::Emitter &Emitter, bool printReduced) override; 
    };

}

template <class ElemType>
void VectorFeature<ElemType>::setValueOf(std::string FeatureName, ElemType Elem) {
  assert(this->hasSubFeature(FeatureName) && "VectorFeature has no such sub-feature.");
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName); 
  TheFeature[Index] = Elem;
}

template <class ElemType>
const ElemType& VectorFeature<ElemType>::getValueOf(std::string FeatureName) {
  assert(this->hasSubFeature(FeatureName) && "VectorFeature has no such sub-feature.");
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName); 
  return TheFeature[Index];
}

template <class ElemType>
void VectorFeature<ElemType>::appendYaml(YAML::Emitter &Emitter, bool printReduced) {
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();

  Emitter << YAML::Key << "features";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &InfoPair : *(this)) {
    if (printReduced && getValueOf(InfoPair.first) == 0) continue;
    Emitter << YAML::Key << InfoPair.first;
    Emitter << YAML::Value << getValueOf(InfoPair.first);
    Emitter << YAML::Comment(InfoPair.second);
  }
  Emitter << YAML::EndMap;

  Emitter << YAML::EndMap;
}

#endif

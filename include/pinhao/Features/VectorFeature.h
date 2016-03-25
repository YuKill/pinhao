/*-------------------------- PINHAO project --------------------------*/

/**
 * @file VectorFeature.h
 */

#ifndef PINHAO_VECTOR_FEATURE_H
#define PINHAO_VECTOR_FEATURE_H

#include "pinhao/Features/Features.h"

using namespace pinhao;

namespace pinhao {
  template <class ElemType> class VectorFeature;

  template <class ElemType>
    class Yamlfy<VectorFeature<ElemType>> : public YamlfyTemplateBase<VectorFeature<ElemType>> {
      public:
        Yamlfy(const VectorFeature<ElemType> *VFPtr) : YamlfyTemplateBase<VectorFeature<ElemType>>(VFPtr) {}

        void append(YAML::Emitter &Emitter, bool PrintReduced) override;
        void get(const YAML::Node &Node) override;
    };

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

        VectorFeature(FeatureInfo *Info);

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

    };

}

template <class ElemType>
void Yamlfy<VectorFeature<ElemType>>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  VectorFeature<ElemType> *Pointer = this->Value;
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << Pointer->getName();
  Emitter << YAML::Key << "features";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &InfoPair : *(Pointer)) {
    if (PrintReduced && Pointer->getValueOf(InfoPair.first) == 0) continue;
    Emitter << YAML::Key << InfoPair.first << YAML::Value;
    Yamlfy<ElemType>(&Pointer->getValueOf(InfoPair.first)).append(Emitter, PrintReduced);
    Emitter << YAML::Comment(InfoPair.second);
  }
  Emitter << YAML::EndMap;
  Emitter << YAML::EndMap;
}

template <class ElemType>
void Yamlfy<VectorFeature<ElemType>>::get(const YAML::Node &Node) {
  VectorFeature<ElemType> *Pointer = this->Value;
  YAML::Node Features = Node["features"];
  for (auto I = Features.begin(), E = Features.end(); I != E; ++I) {
    ElemType Elem;
    std::string FeatureName = I->first.as<std::string>();
    Yamlfy<ElemType>(&Elem).get(I->second); 
    Pointer->setValueOf(FeatureName, Elem);
  }
}

template <class ElemType>
VectorFeature<ElemType>::VectorFeature(FeatureInfo *Info) : LinearFeature<ElemType>(Info) {
  assert(this->isComposite() && "VectorFeature must have CompositeFeatureInfo.");
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
  TheFeature = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0); 
  if (this->Yaml.get() == nullptr)
    this->Yaml = std::shared_ptr<YamlfyBase>(new Yamlfy<VectorFeature<ElemType>>(this));
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

#endif

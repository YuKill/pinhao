/*-------------------------- PINHAO project --------------------------*/

/**
 * @file MapFeature.h
 */

#ifndef PINHAO_MAP_FEATURE_H
#define PINHAO_MAP_FEATURE_H

#include "pinhao/Features/Features.h"

using namespace pinhao;

namespace pinhao {

  /**
   * @brief This class maps a @a KeyType to a @a ElemType feature.
   *
   * @details
   * It is a composite feature type which maps a given @a KeyType to a vector
   * of sub-features of type @a ElemType. So, every @a KeyType has all sub-feature
   * values.
   */
  template <class KeyType, class ElemType>
    class MapFeature : public MappedFeature<KeyType, ElemType> {
      protected:
        /// @brief The vector container of the feature itself.
        std::map<KeyType, ElemType> TheFeature;

      public:
        virtual ~MapFeature() {}
        MapFeature(FeatureInfo *Info) : MappedFeature<KeyType, ElemType>(Info) {}

        virtual bool hasKey(const KeyType &Key) override; 

        void setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) override;

        const ElemType& getValueOfKey(std::string FeatureName, KeyType Key) override;

        virtual void appendYaml(YAML::Emitter &Emitter, bool printReduced) override;

    };

}

template <class KeyType, class ElemType>
bool MapFeature<KeyType, ElemType>::hasKey(const KeyType &Key) {
  return TheFeature.count(Key) > 0;
}

template <class KeyType, class ElemType>
void MapFeature<KeyType, ElemType>::setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) {
  assert(FeatureName == this->getName() && "FeatureName doesn't equal the name of this feature.");
  TheFeature[Key] = Elem;
}

template <class KeyType, class ElemType>
const ElemType& MapFeature<KeyType, ElemType>::getValueOfKey(std::string FeatureName, KeyType Key) {
  assert(FeatureName == this->getName() && "FeatureName doesn't equal the name of this feature.");
  return TheFeature[Key];
}

template <class KeyType, class ElemType>
void MapFeature<KeyType, ElemType>::appendYaml(YAML::Emitter &Emitter, bool printReduced) {
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();
  Emitter << YAML::Comment(this->getDescription());
  Emitter << YAML::Key << "values";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &Pair : TheFeature) {
    Emitter << YAML::Key << Pair.first;
    Emitter << YAML::Value << Pair.second;
  }
  Emitter << YAML::EndMap;
  Emitter << YAML::EndMap;
}

#endif

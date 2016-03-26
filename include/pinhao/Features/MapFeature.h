/*-------------------------- PINHAO project --------------------------*/

/**
 * @file MapFeature.h
 */

#ifndef PINHAO_MAP_FEATURE_H
#define PINHAO_MAP_FEATURE_H

#include "pinhao/Features/Features.h"

using namespace pinhao;

namespace pinhao {
  template <class KeyType, class ElemType> class MapFeature;

  template <class KeyType, class ElemType>
    class Yamlfy<MapFeature<KeyType, ElemType>> : public YamlfyTemplateBase<MapFeature<KeyType, ElemType>> {
      public:
        Yamlfy(const MapFeature<KeyType, ElemType> *MFPtr) : YamlfyTemplateBase<MapFeature<KeyType, ElemType>>(MFPtr) {}

        void append(YAML::Emitter &Emitter, bool PrintReduced) override;
        void get(const YAML::Node &Node) override;
    };

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

        friend class Yamlfy<MapFeature<KeyType, ElemType>>;
    };

}

template <class KeyType, class ElemType>
void Yamlfy<MapFeature<KeyType, ElemType>>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  MapFeature<KeyType, ElemType> *Pointer = this->Value;
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << Pointer->getName();
  Emitter << YAML::Comment(Pointer->getDescription());
  Emitter << YAML::Key << "values";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &Pair : Pointer->TheFeature) {
    Emitter << YAML::Key;
    Yamlfy<KeyType>(Pair.first).append(Emitter, PrintReduced);
    Emitter << YAML::Value;
    Yamlfy<ElemType>(Pair.second).append(Emitter, PrintReduced);
  }
  Emitter << YAML::EndMap;
  Emitter << YAML::EndMap;
}

template <class KeyType, class ElemType>
void Yamlfy<MapFeature<KeyType, ElemType>>::get(const YAML::Node &Node) {
  MapFeature<KeyType, ElemType> *Pointer = this->Value;
  YAML::Node Values = Node["values"];
  for (auto I = Values.begin(), E = Values.end(); I != E; ++I) {
    KeyType Key;
    Yamlfy<KeyType>(&Key).get(I->first);
    ElemType Elem;
    Yamlfy<KeyType>(&Elem).get(I->second);
    setValueOfKey(Pointer->getName(), Elem, Key);
  }
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


#endif

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file MapVectorFeature.h
 */

#ifndef PINHAO_MAPVECTOR_FEATURE_H
#define PINHAO_MAPVECTOR_FEATURE_H

#include "pinhao/Features/Features.h"
#include "pinhao/Support/Yamlfy.h"

using namespace pinhao;

namespace pinhao {
  template <class KeyType, class ElemType> class MapVectorFeature;

  template <class KeyType, class ElemType>
    class Yamlfy<MapVectorFeature<KeyType, ElemType>> : public YamlfyTemplateBase<MapVectorFeature<KeyType, ElemType>> {
      public:
        Yamlfy(const MapVectorFeature<KeyType, ElemType> *MVFPtr) : 
          YamlfyTemplateBase<MapVectorFeature<KeyType, ElemType>>(MVFPtr) {}

        void append(YAML::Emitter &Emitter, bool PrintReduced) override;
        void get(const YAML::Node &Node) override;
    };

  /**
   * @brief This class maps a @a KeyType to a vector of @a ElemType features.
   *
   * @details
   * It is a composite feature type which maps a given @a KeyType to a vector
   * of sub-features of type @a ElemType. So, every @a KeyType has all sub-feature
   * values.
   */
  template <class KeyType, class ElemType>
    class MapVectorFeature : public MappedFeature<KeyType, ElemType> {
      public:
        typedef StdMapKeyIterator<KeyType, std::vector<ElemType>> iterator;

      protected:
        /// @brief The vector container of the feature itself.
        std::map<KeyType, std::vector<ElemType>> TheFeature;

      private:
        /// @brief Initializes the vector of the key at the map @a TheFeature.
        void initVectorOfKey(const KeyType Key) {
          if (TheFeature.count(Key) > 0) return;
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          TheFeature[Key] = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0);
        }

      public:
        virtual ~MapVectorFeature() {}

        MapVectorFeature(FeatureInfo *Info) : MappedFeature<KeyType, ElemType>(Info) {
          assert(this->isComposite() && "MapVectorFeature must have CompositeFeatureInfo.");
        }

        virtual bool hasKey(const KeyType &Key) override;

        /**
         * @brief Sets the value of the sub-feature, with name @a FeatureName, of key @a Key,
         * to value @a Elem.
         *
         * @param FeatureName The name of the sub-feature.
         * @param Elem The value that will replace the old one.
         * @param Key The key from which it will set the sub-feature value.
         */
        void setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) override;

        /**
         * @brief Gets the sub-feature value, whose name is @a FeatureName, and
         * type @a ElemType.
         *
         * @details
         * As it is a map, it should be provided the @a Key of type @a KeyType, so
         * that it is returned the pointer to the sub-feature of that key.
         *
         * @param FeatureName The name of the sub-feature.
         * @param Key The key to get from.
         * @return A const pointer to the value of the sub-feature of the key.
         */
        const ElemType& getValueOfKey(std::string FeatureName, const KeyType Key) override;

        KeyIterator<KeyType> &beginKeys() override;
        KeyIterator<KeyType> &endKeys() override;

        friend class Yamlfy<MapVectorFeature<KeyType, ElemType>>;
    };

}

template <class KeyType, class ElemType>
void Yamlfy<MapVectorFeature<KeyType, ElemType>>::append(YAML::Emitter &Emitter, bool PrintReduced) {
  MapVectorFeature<KeyType, ElemType> *Pointer = this->Value;
  Emitter << YAML::BeginMap;
  Emitter << YAML::Key << "feature-name" << YAML::Value << Pointer->getName();
  Emitter << YAML::Key << "values";
  Emitter << YAML::Value << YAML::BeginMap;
  for (auto &I = Pointer->beginKeys(), &E = Pointer->endKeys(); I != E; ++I) {
    Emitter << YAML::Key;
    Yamlfy<KeyType>(&(*I)).append(Emitter, PrintReduced); 
    Emitter << YAML::Value << YAML::BeginMap;
    for (auto &InfoPair : *(Pointer)) {
      if (PrintReduced && Pointer->getValueOfKey(InfoPair.first, *I) == 0) continue;
      Emitter << YAML::Key << InfoPair.first << YAML::Value;
      Yamlfy<ElemType>(&Pointer->getValueOfKey(InfoPair.first, *I)).append(Emitter, PrintReduced);
      Emitter << YAML::Comment(InfoPair.second);
    }
    Emitter << YAML::EndMap;
  }
  Emitter << YAML::EndMap;
  Emitter << YAML::EndMap;
}

template <class KeyType, class ElemType>
void Yamlfy<MapVectorFeature<KeyType, ElemType>>::get(const YAML::Node &Node) {
  MapVectorFeature<KeyType, ElemType> *Pointer = this->Value;
  YAML::Node Values = Node["values"];
  for (auto I = Values.begin(), E = Values.end(); I != E; ++I) {
    KeyType Key;
    Yamlfy<KeyType>(&Key).get(I->first); 
    for (auto FI = I->second.begin(), FE = I->second.end(); FI != FE; ++FI) {
      ElemType Elem;
      std::string FeatureName = FI->first.as<std::string>();
      Yamlfy<ElemType>(&Elem).get(FI->second);
      Pointer->setValueOfKey(FeatureName, Elem, Key);
    }
  }
}

template <class KeyType, class ElemType>
bool MapVectorFeature<KeyType, ElemType>::hasKey(const KeyType &Key) {
  return TheFeature.count(Key) > 0;
}

template <class KeyType, class ElemType>
void MapVectorFeature<KeyType, ElemType>::setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) {
  assert(this->hasSubFeature(FeatureName) && "MapVectorFeature has no such sub-feature.");

  initVectorOfKey(Key);
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName);
  TheFeature[Key][Index] = Elem;
}

template <class KeyType, class ElemType>
const ElemType& MapVectorFeature<KeyType, ElemType>::getValueOfKey(std::string FeatureName, const KeyType Key) {
  assert(this->hasSubFeature(FeatureName) && "MapVectorFeature has no such sub-feature.");

  initVectorOfKey(Key);
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName); 
  return TheFeature[Key][Index];
}

template <class KeyType, class ElemType>
KeyIterator<KeyType> &MapVectorFeature<KeyType, ElemType>::beginKeys() {
  static iterator It;
  It = iterator(&TheFeature, TheFeature.begin());
  return It;
}

template <class KeyType, class ElemType>
KeyIterator<KeyType> &MapVectorFeature<KeyType, ElemType>::endKeys() {
  static iterator It;
  It = iterator(&TheFeature, TheFeature.end());
  return It;
}

#endif

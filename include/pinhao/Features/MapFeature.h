/*-------------------------- PINHAO project --------------------------*/

/**
 * @file MapFeature.h
 */

#ifndef PINHAO_MAP_FEATURE_H
#define PINHAO_MAP_FEATURE_H

#include "pinhao/Features/Features.h"
#include "pinhao/Support/FeatureYAMLWrapper.h"
#include "pinhao/Support/Iterator.h"

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
      public:
        typedef StdMapKeyIterator<KeyType, ElemType> iterator;

      protected:
        /// @brief The vector container of the feature itself.
        std::map<KeyType, ElemType> TheFeature;

      public:
        virtual ~MapFeature() {}
        MapFeature(FeatureInfo *Info) : MappedFeature<KeyType, ElemType>(Info) {}

        virtual bool hasKey(const KeyType &Key) const override; 

        void setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) override;

        const ElemType& getValueOfKey(std::string FeatureName, const KeyType Key) const override;

        KeyIterator<KeyType> &beginKeys() override;
        KeyIterator<KeyType> &endKeys() override;

        virtual void append(YAML::Emitter &Emitter) const override;  
        virtual void get(const YAML::Node &Node) override;

    };

}

template <class KeyType, class ElemType>
bool MapFeature<KeyType, ElemType>::hasKey(const KeyType &Key) const {
  return TheFeature.count(Key) > 0;
}

template <class KeyType, class ElemType>
void MapFeature<KeyType, ElemType>::setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) {
  assert(FeatureName == this->getName() && "FeatureName doesn't equal the name of this feature.");
  TheFeature[Key] = Elem;
}

template <class KeyType, class ElemType>
const ElemType& MapFeature<KeyType, ElemType>::getValueOfKey(std::string FeatureName, const KeyType Key) const {
  assert(FeatureName == this->getName() && "FeatureName doesn't equal the name of this feature.");
  return TheFeature.at(Key);
}

template <class KeyType, class ElemType>
KeyIterator<KeyType> &MapFeature<KeyType, ElemType>::beginKeys() {
  static iterator It;
  It = iterator(&TheFeature, TheFeature.begin());
  return It;
}

template <class KeyType, class ElemType>
KeyIterator<KeyType> &MapFeature<KeyType, ElemType>::endKeys() {
  static iterator It;
  It = iterator(&TheFeature, TheFeature.end());
  return It;
}

template <class KeyType, class ElemType>
void MapFeature<KeyType, ElemType>::append(YAML::Emitter &Emitter) const {
  YAMLWrapper::append(*this, Emitter);
}

template <class KeyType, class ElemType>
void MapFeature<KeyType, ElemType>::get(const YAML::Node &Node) {
  YAMLWrapper::fill(*this, Node);
}


#endif

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Features.h
 * @brief This file implements some basic feature classes that extends
 * the base class @a Feature.
 */

#ifndef PINHAO_FEATURES_H
#define PINHAO_FEATURES_H

#include "pinhao/Features/Feature.h"

namespace pinhao {

  /**
   * @brief Implements a string version of a feature.
   *
   * @details
   * It also implements the @a getShogunFeature function. 
   * As it is just a string, it can have this implementation. Classes 
   * that extend this one can override the implementation.
   */
  class StringFeature : public Feature {
    private:
      /// @brief The string feature "container".
      std::string TheFeature;
  
    public:
      virtual ~StringFeature() {};

      /// @brief Creates a StringFeature with @a Info and some @a Mode.
      /// Sets the @a FeatureKind as @a StringKind.
      StringFeature(FeatureInfo *Info) : 
        Feature(Info, FeatureKind::StringKind) {}

      /// @brief Gets the string feature.
      const std::string& getStringFeature() {
        return TheFeature;
      }

      /// @brief Sets the string feature to @a New.
      void setStringFeature(std::string New) {
        TheFeature = New;
      }

      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

  };

  /**
   * @brief Implements a template vector feature class.
   *
   * @details
   * All the vector-like features must implement this class,
   * giving its type as the template.
   */
  template <class ElemType>
  class VectorFeature : public Feature {
    private:
      /// @brief The vector container of the feature itself.
      std::vector<ElemType> TheFeature;
  
    public:
      virtual ~VectorFeature() {};

      VectorFeature(FeatureInfo *Info)
        : Feature(Info, FeatureKind::VectorKind) {
          assert(Info->isComposite() && "VectorFeature must have CompositeFeatureInfo.");
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info);
          TheFeature = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0); 
        }

      /**
       * @brief Gets the sub-feature, which has type @a ElemType, with
       * name @a SubFeatureName.
       *
       * @param SubFeatureName The name of the desired sub-feature.
       * @return A pointer to the sub-feature value with const modifier.
       */
      const ElemType& getSubFeature(std::string SubFeatureName);

      /**
       * @brief Sets the sub-feature with name @a SubFeatureName to
       * @a Elem.
       *
       * @param SubFeatureName The name of the desired sub-feature.
       * @param Elem The element that will replace the old element.
       */
      void setSubFeature(std::string SubFeatureName, ElemType Elem); 

      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

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
  class MapVectorFeature : public Feature {
    private:
      /// @brief The vector container of the feature itself.
      std::map<KeyType, std::vector<ElemType>> TheFeature;

      /// @brief Initializes the vector of the key at the map @a TheFeature.
      void initVectorOfKey(KeyType Key);

    public:
      virtual ~MapVectorFeature() {}

      MapVectorFeature(FeatureInfo *Info)
        : Feature(Info, FeatureKind::MapVectorKind) {
          assert(Info->isComposite() && "VectorFeature must have CompositeFeatureInfo.");
        }

      /**
       * @brief Sets the value of the sub-feature, with name @a SubFeatureName, of key @a Key,
       * to value @a Elem.
       *
       * @param SubFeatureName The name of the sub-feature.
       * @param Elem The value that will replace the old one.
       * @param Key The key from which it will set the sub-feature value.
       */
      virtual void setSubFeatureOfKey(std::string SubFeatureName, ElemType Elem, KeyType Key);

      /**
       * @brief Gets the sub-feature value, whose name is @a SubFeatureName, and
       * type @a ElemType.
       *
       * @details
       * As it is a map, it should be provided the @a Key of type @a KeyType, so
       * that it is returned the pointer to the sub-feature of that key.
       *
       * @param SubFeatureName The name of the sub-feature.
       * @param Key The key to get from.
       * @return A const pointer to the value of the sub-feature of the key.
       */
      const ElemType& getSubFeatureOfKey(std::string SubFeatureName, KeyType Key);

      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

  };

}

#endif

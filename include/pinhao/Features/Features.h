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
      StringFeature(FeatureInfo *Info, GatherMode Mode) : 
        Feature(Info, Mode, FeatureKind::StringKind) {}

      /// @brief Gets the string feature.
      std::string getStringFeature() {
        return TheFeature;
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

      VectorFeature(FeatureInfo *Info, GatherMode Mode)
        : Feature(Info, Mode, FeatureKind::VectorKind) {}

      /**
       * @brief Gets the sub-feature, which has type @a ElemType, with
       * name @a SubFeatureName.
       *
       * @param SubFeatureName The name of the desired sub-feature.
       * @return A const pointer to the sub-feature value.
       */
      const ElemType* getSubFeature(std::string SubFeatureName) override;

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

    public:
      virtual ~MapVectorFeature() {}

      MapVectorFeature(FeatureInfo *Info, GatherMode Mode)
        : Feature(Info, Mode, FeatureKind::MapVectorKind) {}

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
      const ElemType* getSubFeatureForKey(std::string SubFeatureName, KeyType Key) override;

      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

  };

}

#endif

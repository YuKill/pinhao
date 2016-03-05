/*-------------------------- PINHAO project --------------------------*/

/**
 * @file CompositeFeatures.h
 * @brief This file implements the base vector feature class.
 */

#ifndef PINHAO_COMPOSITE_FEATURES_H
#define PINHAO_COMPOSITE_FEATURES_H

#include "pinhao/Features/Feature.h"
#include "pinhao/Features/FeatureInfo.h"

namespace pinhao {

  /**
   * @brief Implements a template composite feature class.
   *
   * @details
   * It is implemented so that when there are lots of composite
   * features, it does not make a difference whether it is a vector,
   * map or list.
   * @a ElemType is the type of each sub-feature. And @a KeyType is
   * the type of each key which is mapped to all sub-features. Such as
   * the MapVectorFeature, MatrixFeature, etc.
   */
  template <class ElemType, class KeyType = uint64_t>
  class CompositeFeature : public Feature {
    protected:
      /// @brief Gets the index of a sub-feature in the name map.
      uint64_t getIndexOfSubFeature(std::string SubFeatureName);

    public:
      virtual ~CompositeFeature() {}

      CompositeFeature(FeatureInfo *Info, GatherMode Mode, FeatureKind Kind)
        : Feature(Info, Mode, Kind) {}

      /// @brief Gets the sub-feature element of the current feature 
      /// with name @a SubFeatureName from (line, pointer, etc.) @a Key.
      virtual const ElemType* getSubFeature(std::string SubFeatureName, KeyType Key) = 0;

  };

  /**
   * @brief Implements a template vector composite feature class.
   *
   * @details
   * All the vector-like features must implement this class,
   * giving its type as the template.
   */
  template <class ElemType>
  class VectorFeature : public CompositeFeature<ElemType> {
    private:
      /// @brief The vector container of the feature itself.
      std::vector<ElemType> TheFeature;
  
    public:
      virtual ~VectorFeature() {};

      VectorFeature(FeatureInfo *Info, Feature::GatherMode Mode)
        : CompositeFeature<ElemType>(Info, Mode, Feature::FeatureKind::VectorKind) {}

      virtual const ElemType* getSubFeature(std::string SubFeatureName, uint64_t Key = 0) override;
      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

  };

  /**
   * @brief This class is a map from a @a KeyType to a vector of @a ElemType.
   *
   * @details
   * It is a composite feature type which maps a given @a KeyType to a vector
   * of sub-features of type @a ElemType. So, every @a KeyType has all sub-feature
   * values.
   */
  template <class KeyType, class ElemType>
  class MapVectorFeature : public CompositeFeature<ElemType, KeyType> {
    private:
      /// @brief The vector container of the feature itself.
      std::map<KeyType, std::vector<ElemType>> TheFeature;

    public:
      virtual ~MapVectorFeature() {}

      MapVectorFeature(FeatureInfo *Info, Feature::GatherMode Mode)
        : CompositeFeature<ElemType, KeyType>(Info, Mode, Feature::FeatureKind::MapVectorKind) {}

      virtual const ElemType* getSubFeature(std::string SubFeatureName, KeyType Key) override;
      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

  };

}

#endif

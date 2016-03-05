/*-------------------------- PINHAO project --------------------------*/

/**
 * @file VectorFeature.h
 * @brief This file implements the base vector feature class.
 */

#ifndef PINHAO_VECTOR_FEATURE_H
#define PINHAO_VECTOR_FEATURE_H

#include "pinhao/Features/Feature.h"

namespace pinhao {

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

      /// @brief Gets the index of a sub-feature in the name map.
      uint64_t getIndexOfSubFeature(std::string Name);
  
    public:
      virtual ~VectorFeature() {};

      VectorFeature(FeatureInfo *Info, GatherMode Mode, FeatureKind Kind)
        : Feature(Info, Mode, Feature::FeatureKind::VectorKind) {}

      /// @brief Gets the sub-feature element of the vector.
      const ElemType* getSubFeature(std::string Name);

      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() override;

  };

}

#endif

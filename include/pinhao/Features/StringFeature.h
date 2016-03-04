/*-------------------------- PINHAO project --------------------------*/

/**
 * @file StringFeature.h
 * @brief This file implements a string feature class.
 */

#ifndef PINHAO_STRING_FEATURE_H
#define PINHAO_STRING_FEATURE_H

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

}

#endif

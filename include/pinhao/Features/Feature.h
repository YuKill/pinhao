/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Feature.h
 * @brief This file defines the base feature class.
 */

#ifndef PINHAO_FEATURE_H
#define PINHAO_FEATURE_H

#include "pinhao/Features/FeatureInfo.h"

#include "llvm/IR/Module.h"
#include "shogun/features/Features.h"

#include <map>
#include <cassert>

namespace pinhao {

  /**
   * @brief This is the base class for features.
   *
   * @details 
   * All new feature types must implement this class. So that they all can
   * be registered.
   */
  class Feature {
    public:
      /// @brief The kind of the feature.
      enum FeatureKind {
        StringKind,     ///< Has one string feature.
        CompositeKind   ///< Has two or more features within it.
      };
      
      /// @brief How the feature information is gathered.
      enum GatherMode {
        Static, ///< Statically gathered features.
        Dynamic ///< Dynamically gathered features.
      };

    private:
      /**
       * @brief Stores unique pointers to all registered features.
       *
       * @details
       * In order to use a registered feature, one should clone the
       * desired feature. It maps the name to the feature.
       */
      static std::map<std::string, std::unique_ptr<Feature>> RegisteredFeatures;

      GatherMode Mode; 
      FeatureKind Kind;
      std::unique_ptr<FeatureInfo> Info;

    public:
      virtual ~Feature() {};

      /// @brief Constructs a @a Feature with the information @a FeatInfo.
      Feature(std::unique_ptr<FeatureInfo> FeatInfo) : Info(std::move(FeatInfo)) {}

      /// @brief Gets the name of the feature.
      std::string getName() { return Info->getName(); }

      /// @brief Gets the description of the feature.
      std::string getDescription() { return Info->getDescription(); }

      /// @brief Returns true if it is a composite kind.
      bool isComposite() { return Kind == FeatureKind::CompositeKind; }

      /// @brief Returns true if it is a static feature.
      bool isStaticFeature() { return Mode == GatherMode::Dynamic; }

      /// @brief Returns true if it is a dynamic feature.
      bool isDynamicFeature() { return Mode == GatherMode::Static; }

      /**
       * @brief Transforms the data stored in this class to a shogun::CFeature. 
       *
       * @details 
       * It should be used whenever the machine learning algorithm used is
       * in the shogun toolbox.
       *
       * @return A unique pointer to the equivalent shogun feature.
       */
      virtual std::unique_ptr<shogun::CFeatures> getShogunFeature() {
        assert("Feature doesn't implements 'getShogunFeature' function." && false);
        return nullptr;
      }

      /**
       * @brief Gets the necessary information from a llvm::Module.
       *
       * @details
       * This function processes the @a Module in order to gather the information
       * needed.
       *
       * @param Module The module to be processed.
       */
      virtual void processModule(llvm::Module Module) = 0;

      /**
       * @brief Clones the feature.
       *
       * @return A unique pointer to the clone Feature.
       */
      virtual std::unique_ptr<Feature> clone() = 0;

      /// @brief Register a feature in order to become available.
      static void registerFeature(Feature *F);
  
  };

  /**
   * @brief Class responsible to register the new feature.
   *
   * @details
   * When it is instantiated, the constructor of this class will
   * take care and register the template @a FeatureClass.
   */
  template <class FeatureClass>
  class RegisterFeature {
    public:
      RegisterFeature(FeatureInfo *Info) {
        std::unique_ptr InfoPtr = std::unique_ptr(Info);
        FeatureClass *F = new FeatureClass(std::move(InfoPtr)); 
        Feature::registerFeature(F);
      }
  }; 

}

#endif

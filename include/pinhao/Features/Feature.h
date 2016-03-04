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
        StringKind,
        IntVector,
        FPVector
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

      /**
       * @brief Checks whether a feature (or sub-feature) is already
       * registered.
       *
       * @details
       * It iterates each element in the @a RegisteredFeatures, checking also
       * the sub-feature names. 
       *
       * @param Name The name of the feature or sub-feature.
       *
       * @return True if there already is a feature (or sub-feature) registered
       * with the same name.
       */
      static bool isNameRegistered(std::string Name);

      std::unique_ptr<FeatureInfo> Info;
      GatherMode Mode; 
      FeatureKind Kind;

    public:
      virtual ~Feature() {};

      /// @brief Constructs a @a Feature with the information @a FeatInfo.
      Feature(FeatureInfo* InfoPtr, GatherMode GMode, FeatureKind FKind) : 
        Info(std::unique_ptr<FeatureInfo>(InfoPtr)), Mode(GMode), Kind(FKind) {}

      /// @brief Gets the name of the feature.
      std::string getName() { return Info->getName(); }

      /// @brief Gets the description of the feature.
      std::string getDescription() { return Info->getDescription(); }

      /// @brief Returns true if there is a sub-feature with name @a SubFeatureName.
      bool hasSubFeature(std::string SubFeatureName) { 
        return Info->hasSubFeature(SubFeatureName); 
      }

      /// @brief Gets the description of the sub-feature with name @a SubFeatureName.
      std::string getSubFeatureDescription(std::string SubFeatureName) { 
        return Info->getSubFeatureDescription(SubFeatureName); 
      }

      /// @brief Gets the @ begin iterator of the information mapping.
      std::map<std::string, std::string>::iterator begin() { return Info->begin(); }

      /// @brief Gets the @ end iterator of the information mapping.
      std::map<std::string, std::string>::iterator end() { return Info->end(); }

      /// @brief Returns true if the feature has more than two sub-features.
      bool isComposite();

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
      RegisterFeature(FeatureInfo *InfoPtr) {
        FeatureClass *F = new FeatureClass(InfoPtr);

        Feature *BaseF = dynamic_cast<Feature*>(F);
        assert(BaseF != nullptr && "FeatureClass is not a Feature type.");

        Feature::registerFeature(BaseF);
      }
  }; 

}

#endif

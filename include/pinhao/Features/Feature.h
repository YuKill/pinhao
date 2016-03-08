/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Feature.h
 * @brief This file defines the base feature class.
 */

#ifndef PINHAO_FEATURE_H
#define PINHAO_FEATURE_H

#include "pinhao/Features/FeatureInfo.h"
#include "pinhao/Features/FeatureRegistry.h"

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
        VectorKind,
        MapVectorKind
      };

    protected:
      std::shared_ptr<FeatureInfo> Info;

    private:
      FeatureKind Kind;

    public:
      virtual ~Feature() {};

      /// @brief Constructs a @a Feature with the information @a FeatInfo.
      Feature(FeatureInfo* InfoPtr, FeatureKind FKind) : 
        Info(std::shared_ptr<FeatureInfo>(InfoPtr)), Kind(FKind) {}

      /// @brief Gets the name of the feature.
      std::string getName() { return Info->getName(); }

      /// @brief Gets the description of the feature.
      std::string getDescription() { return Info->getDescription(); }

      /// @brief Returns true if there is a sub-feature with name @a SubFeatureName.
      bool hasSubFeature (std::string SubFeatureName) const { 
        if (!isComposite()) return false;
        CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
        return CompInfo->hasSubFeature(SubFeatureName); 
      }

      /// @brief Gets the description of the sub-feature with name @a SubFeatureName.
      std::string getSubFeatureDescription(std::string SubFeatureName) { 
        assert(hasSubFeature(SubFeatureName) && "This feature does not have the sub-feature desired.");
        CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
        return CompInfo->getSubFeatureDescription(SubFeatureName); 
      }

      /// @brief Returns true if the feature has more than two sub-features.
      bool isComposite() const { return Info->isComposite(); }

      /// @brief Returns true if it is a static feature.
      bool isStaticFeature() const { return Info->isStaticFeature(); }

      /// @brief Returns true if it is a dynamic feature.
      bool isDynamicFeature() const { return Info->isDynamicFeature(); }

      /// @brief Gets the @a begin iterator of the @a Info.
      std::map<std::string, std::string>::iterator begin() {
        assert(isComposite() && "This feature is not a composite type feature."); 
        CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
        return CompInfo->begin();
      }

      /// @brief Gets the @a end iterator of the @a Info.
      std::map<std::string, std::string>::iterator end() {
        assert(isComposite() && "This feature is not a composite type feature."); 
        CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
        return CompInfo->end();
      }

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
      virtual void processModule(llvm::Module& Module) = 0;

      /**
       * @brief Clones the feature.
       *
       * @return A unique pointer to the clone Feature.
       */
      virtual std::unique_ptr<Feature> clone() const = 0;

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

          FeatureRegistry::registerFeature(BaseF);
        }   
    };
}

#endif

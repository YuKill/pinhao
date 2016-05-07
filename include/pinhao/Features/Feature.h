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

#include "yaml-cpp/yaml.h"

#include <map>
#include <cassert>
#include <iostream>

namespace pinhao {

  /// @brief The kind of the feature.
  enum class FeatureKind {
    LinearKind, ///< Can get the value only by the name of the feature.
    MappedKind  ///< Needs a key in order to get the value.
  };

  /**
   * @brief This is the base class for features.
   *
   * @details 
   * All new feature types must implement this class. So that they all can
   * be registered.
   */
  class Feature {
    public:
      typedef std::map<std::string, std::string>::iterator iterator;

    protected:
      std::shared_ptr<FeatureInfo> Info;
      bool Processed;

    private:
      FeatureKind Kind;

      /// @brief Casts @a Info to a @a CompositeFeatureInfo type, if possible.
      CompositeFeatureInfo *getCompositeFeatureInfo() const;

    public:
      virtual ~Feature() {};

      /// @brief Constructs a @a Feature with the information @a FeatInfo.
      Feature(FeatureInfo* InfoPtr, FeatureKind FKind);

      /// @brief Gets the name of the feature.
      std::string getName() const;
      /// @brief Gets the description of the feature.
      std::string getDescription() const;
      /// @brief Gets the @a ValueType of the feature.
      ValueType getType() const;

      /// @brief Gets the description of the sub-feature with name @a SubFeatureName.
      uint64_t getNumberOfSubFeatures() const;
      /// @brief Returns true if there is a sub-feature with name @a SubFeatureName.
      bool hasSubFeature (std::string SubFeatureName) const;
      /// @brief Gets the description of the sub-feature with name @a SubFeatureName.
      std::string getSubFeatureDescription(std::string SubFeatureName) const;

      /// @brief Returns the @a FeatureKind.
      FeatureKind getKind() const;
      /// @brief Returns true if the feature has more than two sub-features.
      bool isComposite() const;
      /// @brief Returns true if the feature has already been processed.
      bool isProcessed() const;
      /// @brief Returns true if it is a @a LinearFeature.
      bool isLinear() const;
      /// @brief Returns true if it is a @a MappedFeature.
      bool isMapped() const;
      /// @brief Returns true if it is a static feature.
      bool isStaticFeature() const;
      /// @brief Returns true if it is a dynamic feature.
      bool isDynamicFeature() const;

      /// @brief Gets the @a begin iterator of the @a Info.
      iterator begin() const;
      /// @brief Gets the @a end iterator of the @a Info.
      iterator end() const;

      /**
       * @brief Prints its information into a @a std::ostream object in the format yaml.
       *
       * @param Out The output stream that the function shall print.
       */
      void print(std::ostream &Out = std::cout);

      /**
       * @brief Appends to a @a YAML::Emitter this feature data.
       *
       * @param Emitter The place where the data information will be appended.
       */
      virtual void append(YAML::Emitter &Emitter) const = 0; 

      /**
       * @brief Gets the feature data from a @a YAML::Node.
       */
      virtual void get(const YAML::Node &Node) = 0;

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

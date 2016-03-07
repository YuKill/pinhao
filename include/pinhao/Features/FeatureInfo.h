/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureInfo.h
 * @brief This file defines a class that holds some informations
 * about a feature.
 */

#ifndef PINHAO_FEATURE_INFO_H
#define PINHAO_FEATURE_INFO_H

#include <map>
#include <vector>
#include <string>
#include <cassert>

namespace pinhao {

  /**
   * @brief This class is the base and most simple feature information
   * holder class.
   *
   * @details
   * This is the class for non-composite features, which do not have 
   * any features inside.
   */
  class FeatureInfo {
    public:

      /// @brief The kind of the feature info.
      enum FeatureInfoKind {
        SimpleKind, CompositeKind
      };

      /// @brief How the feature information is gathered.
      enum GatherMode {
        Static, ///< Statically gathered features.
        Dynamic ///< Dynamically gathered features.
      };

      /// @brief The type of this feature (or each sub-feature).
      enum FeatureType {
        StringType, 
        IntType, 
        FloatType 
      };

    private:
      std::string Name;
      std::string Description;

      FeatureType Type;
      GatherMode  Mode;
      FeatureInfoKind Kind;

    public:
      /**
       * @brief The standard constructor for the @a FeatureInfo class.
       *
       * @param FeatureName The name of the feature.
       * @param FeatureDesc The description of the feature.
       * @param Type The actual type of the feature (or sub-features).
       * @param Mode How this feature should be gathered.
       * @param Kind The class of this @a FeatureInfo class.
       */
      FeatureInfo(std::string FeatureName, std::string FeatureDesc, FeatureType Type, GatherMode Mode, 
          FeatureInfoKind Kind = FeatureInfoKind::SimpleKind) : 
        Name(FeatureName), Description(FeatureDesc), Type(Type), Mode(Mode), Kind(Kind) {}

      virtual ~FeatureInfo() {}

      /// @brief Gets the name of the feature.
      std::string getName() { return Name; }

      /// @brief Gets the description of the feature.
      std::string getDescription() { return Description; }

      /// @brief Returns true if the feature has more than two sub-features.
      bool isComposite() { return Kind == FeatureInfo::CompositeKind; };
      
      /// @brief Returns true if it is a static feature.
      bool isStaticFeature() { return Mode == GatherMode::Dynamic; }
      
      /// @brief Returns true if it is a dynamic feature.
      bool isDynamicFeature() { return Mode == GatherMode::Static; }

  };

  /**
   * @brief This is the composite feature's information.
   *
   * @details
   * It implements the sub-feature methods, which are suited for
   * composite features.
   */
  class CompositeFeatureInfo : public FeatureInfo {
    private:
      std::map<std::string, std::string> FeaturesInfo;

    public:
      /**
       * @brief The standard constructor of the class @a CompositeFeatureInfo.
       *
       * @param FeatureName The name of the feature.
       * @param FeatureDesc The description of the feature.
       * @param Type The actual type of the feature (or sub-features).
       * @param Mode How this feature should be gathered.
       * @param Kind The class of this @a FeatureInfo class.*
       * @param Infos A map of each name and description of the sub-features.
       */
      CompositeFeatureInfo(std::string FeatureName, std::string FeatureDesc, FeatureType Type, 
          GatherMode Mode, std::map<std::string, std::string> Infos) : 
        FeatureInfo(FeatureName, FeatureDesc, Type, Mode, FeatureInfoKind::CompositeKind), FeaturesInfo(Infos) {}
  
      /// @brief Gets the position of the sub-feature (insert order).
      uint64_t getIndexOfSubFeature(std::string SubFeatureName); 

      /// @brief Returns true if there is a sub-feature called @a SubFeatureName.
      bool hasSubFeature(std::string SubFeatureName);

      /// @brief Gets the description of a sub-feature.
      std::string getSubFeatureDescription(std::string SubFeatureName);

      /// @brief Gets the number of sub-features.
      uint64_t getNumberOfSubFeatures() { return FeaturesInfo.size(); }

      /// @brief Gets the @a begin iterator of the information mapping.
      std::map<std::string, std::string>::iterator begin() { return FeaturesInfo.begin(); }

      /// @brief Gets the @a end iterator of the information mapping.
      std::map<std::string, std::string>::iterator end() { return FeaturesInfo.end(); }
  };

}

#endif

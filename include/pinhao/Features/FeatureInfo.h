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
    private:
      std::string Name;
      std::string Description;

    public:
      FeatureInfo(std::string FeatureName, std::string FeatureDesc) :
        Name(FeatureName), Description(FeatureDesc) {}

      virtual ~FeatureInfo() {}

      /// @brief Gets the name of the feature.
      std::string getName() { return Name; }

      /// @brief Gets the description of the feature.
      std::string getDescription() { return Description; }

      /// @brief Returns true if there is a sub-feature called @a SubFeatureName.
      virtual bool hasSubFeature(std::string SubFeatureName) {
        assert("Feature doesn't have any sub-features." && false); 
      }

      /// @brief Gets the description of a sub-feature.
      virtual std::string getSubFeatureDescription(std::string SubFeatureName) {
        assert("Feature doesn't have any sub-features." && false); 
      }

      /// @brief Gets the @a begin iterator of the information mapping.
      virtual std::map<std::string, std::string>::iterator begin() {
        assert("Feature doesn't have any sub-features." && false); 
      }

      /// @brief Gets the @a end iterator of the information mapping.
      virtual std::map<std::string, std::string>::iterator end() {
        assert("Feature doesn't have any sub-features." && false); 
      }

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
      CompositeFeatureInfo(std::string FeatureName, std::string FeatureDesc, 
          std::map<std::string, std::string> Infos) : 
        FeatureInfo(FeatureName, FeatureDesc), FeaturesInfo(Infos) {}
  
      virtual bool hasSubFeature(std::string SubFeatureName) override;
      virtual std::string getSubFeatureDescription(std::string SubFeatureName) override;

      virtual std::map<std::string, std::string>::iterator begin() { return FeaturesInfo.begin(); }
      virtual std::map<std::string, std::string>::iterator end() { return FeaturesInfo.end(); }
  };

}

#endif

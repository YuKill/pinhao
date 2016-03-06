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

      /// @brief Gets the position of the sub-feature (insert order).
      virtual uint64_t getIndexOfSubFeature(std::string SubFeatureName) {
        assert("Feature doesn't have any sub-features." && false); 
      }

      /// @brief Gets the number of sub-features.
      virtual uint64_t getNumberOfSubFeatures() {
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
  
      virtual uint64_t getIndexOfSubFeature(std::string SubFeatureName) override; 
      virtual bool hasSubFeature(std::string SubFeatureName) override;
      virtual std::string getSubFeatureDescription(std::string SubFeatureName) override;

      virtual uint64_t getNumberOfSubFeatures() override { return FeaturesInfo.size(); }

      virtual std::map<std::string, std::string>::iterator begin() override { return FeaturesInfo.begin(); }
      virtual std::map<std::string, std::string>::iterator end() override { return FeaturesInfo.end(); }
  };

}

#endif

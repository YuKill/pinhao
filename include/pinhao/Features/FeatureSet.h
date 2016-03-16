/*-------------------------- PINHAO project --------------------------*/

/**
 * @file FeatureSet.h
 * @brief This file defines a class which contains the features selected by
 * the user (by some command line).
 */

#ifndef PINHAO_FEATURE_SET_H
#define PINHAO_FEATURE_SET_H

#include "pinhao/Features/Features.h"

#include <cassert>
#include <map>
#include <vector>
#include <memory>

namespace pinhao {

  /**
   * @brief This class holds all enabled features.
   */
  class FeatureSet {
    private:
      /// @brief Has the needed information to decide whether some feature must be
      /// enabled or not.
      static std::map<std::string, std::vector<bool>> EnableFeature;

      /// @brief All the enabled features.
      std::map<std::string, std::unique_ptr<Feature>> Features;

      FeatureSet() {}

    public:
      ~FeatureSet() {}

      /// @brief Constructs a @a FeatureSet object based on the @a EnableFeature.
      static std::unique_ptr<FeatureSet> get();

      /**
       * @brief Gets a feature or sub-feature which has type @a FeatureType.
       *
       * @details
       * If it should get a feature, then the variable @a SubfeatureName must be empty("").
       * Otherwise, when getting the sub-feature, it must be passed the parent @a FeatureName,
       * since there may have sub-features with the same name.
       *
       * @param FeatureName The name of the feature or the parent feature.
       * @param SubfeatureName The name of the sub-feature if there is one.
       * @return The value of the feature/sub-feature desired.
       */
      template <class FeatureType>
        FeatureType getFeature(std::string FeatureName, std::string SubfeatureName) {
          assert(Features.count(FeatureName) > 0 && "There is no such feature enabled.");
          assert(Features[FeatureName]->isLinear() > 0 && "Feature is not a Linear feature.");

          SubfeatureName = (SubfeatureName == "") ? FeatureName : SubfeatureName;
          LinearFeature<FeatureType> *LFeature = static_cast<LinearFeature<FeatureType>*>(Features[FeatureName].get());
          return LFeature->getValueOf(SubfeatureName);
        }

      /// @brief Has the same functionality as the @a getFeature method, but it should be provided a key.
      template <class FeatureType, class KeyType>
        FeatureType getSubfeatureOfKey(std::string FeatureName, std::string SubfeatureName, KeyType Key) {
          assert(Features.count(FeatureName) > 0 && "There is no such feature enabled.");
          assert(Features[FeatureName]->isMapped() && "Feature is not a Mapped feature.");

          SubfeatureName = (SubfeatureName == "") ? FeatureName : SubfeatureName;
          MappedFeature<FeatureType, KeyType> *MFeature = static_cast<MappedFeature<FeatureType, KeyType>*>(Features[FeatureName].get());
          return MFeature->getValueOfKey(SubfeatureName, Key);
        }

      template <class FeatureType>
        FeatureType getFeature(std::pair<std::string, std::string> FeaturePair) {
          getFeature<FeatureType>(FeaturePair.first, FeaturePair.second);
        }

      template <class FeatureType, class KeyType>
        FeatureType getSubfeatureOfKey(std::pair<std::string, std::string> FeaturePair, KeyType Key) {
          getFeature<FeatureType, KeyType>(FeaturePair.first, FeaturePair.second, Key);
        }

      friend class FeatureSetWrapperPass;

  };

  /**
   * @brief A @a llvm::ModulePass for the @a FeatureSet.
   *
   * @details
   * It executes the member function @a processModule for all enabled features.
   */
  class FeatureSetWrapperPass : public llvm::ModulePass {
    private:
      std::shared_ptr<FeatureSet> Set;

    public:
      static char ID;
      FeatureSetWrapperPass(std::shared_ptr<FeatureSet> *Set = nullptr) : llvm::ModulePass(ID) {
        if (Set) this->Set = *Set;
      }

      void getAnalysisUsage(llvm::AnalysisUsage &Info) const override {
        Info.setPreservesAll();
      }

      bool runOnModule(llvm::Module &Module) override;

  };

}

#endif

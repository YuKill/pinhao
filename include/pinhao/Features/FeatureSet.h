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
      static std::map<std::string, std::map<std::string, bool>> EnabledFeatures;

      /// @brief All the enabled features.
      std::map<std::string, std::unique_ptr<Feature>> Features;

      FeatureSet() {}

    public:
      /// @brief Constructs a @a FeatureSet object based on the @a EnableFeature.
      static std::unique_ptr<FeatureSet> get();

      /// @brief Returns true if the feature @a FeatureName is enabled. If @a SubFeatureName
      /// is provided, it returns true only if the sub-feature @a SubFeatureName is also enabled.
      static bool isEnabled(std::string FeatureName, std::string SubFeatureName = "");

      static bool isEnabled(std::pair<std::string, std::string> Pair);

      /// @brief Enables the feature called @a FeatureName.
      static void enable(std::string FeatureName);

      /// @brief Enables only the sub-features in the @a SubFeatures, of the feature 
      /// called @a FeatureName.
      static void enable(std::string FeatureName, std::vector<std::string> SubFeatures);

      /// @brief Disables all enabled features.
      static void disableAll();

      /**
       * @brief Iterates over the features/sub-features of the enabled features.
       * 
       * @details
       * It will iterate over the features enabled. More specifically, over a @a FeatureSet. For each
       * Feature or SubFeature found, it will return a iterator pointing to a @a std::pair.
       *
       * { "FeatureName", "SubFeatureName" }
       *
       * If there is no sub-feature in the feature, an empty string is returned.
       */
      class iterator : std::iterator<std::forward_iterator_tag, std::pair<std::string, std::string>> {
        private:
          typedef std::map<std::string, std::unique_ptr<Feature>> EnabledFeaturesMap;
          typedef value_type PairType;

          /// @brief The actual pair that will be returned.
          PairType Pair;

          /// @brief The iterator of composite features.
          Feature::iterator FeatureIterator;

          /// @brief A pointer to the map to be iterated.
          EnabledFeaturesMap *Map;

          /// @brief The current iterator position in the map.
          EnabledFeaturesMap::iterator MapIterator;

          /// @brief The name of the feature being iterated, if specified.
          std::string FeatureName;

          void incMapIterator();
          void setPair();
          void update();

        public:
          iterator() {}

          /**
           * @param M Pointer to the map that it should iterate.
           * @param It The initial iterator position.
           * @param Name A feature name that it should iterate.
           */
          iterator(EnabledFeaturesMap *M, EnabledFeaturesMap::iterator It, std::string Name = ""); 

          const PairType *operator->() const;
          PairType &operator*();
          iterator &operator++();
          iterator operator++(int);
          bool operator==(const iterator &It);
          bool operator!=(const iterator &It);
      };

    private:
      /// @brief Returns a pointer to a feature, based on the @a Iterator.
      Feature *getFeature(iterator);

    public:
      ~FeatureSet() {}

      /// @brief Returns the number of features (and subfeatures).
      uint64_t count();
      /// @brief Returns the number of features with kind @a FKind, and type @a FType.
      uint64_t count(FeatureKind, ValueType);
      /// @brief Returns the number of features with kind @a FKind.
      uint64_t count(FeatureKind);
      /// @brief Returns the number of features with type @a FType.
      uint64_t count(ValueType);

      /// @brief Gets an @a iterator to the @a Nth feature.
      iterator get(uint64_t);
      /// @brief Gets an @a iterator to the @a Nth feature with kind @a FKind, and type @a FType.
      iterator get(uint64_t, FeatureKind, ValueType);
      /// @brief Gets an @a iterator to the @a Nth feature with kind @a FKind.
      iterator get(uint64_t, FeatureKind);
      /// @brief Gets an @a iterator to the @a Nth feature with type @a FType.
      iterator get(uint64_t, ValueType);

      /// @brief Returns the @a ValueType of some feature.
      ValueType getFeatureType(std::string);

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
          return getFeature<FeatureType>(FeaturePair.first, FeaturePair.second);
        }

      template <class FeatureType, class KeyType>
        FeatureType getSubfeatureOfKey(std::pair<std::string, std::string> FeaturePair, KeyType Key) {
          return getFeature<FeatureType, KeyType>(FeaturePair.first, FeaturePair.second, Key);
        }

      /**
       * @brief Gets a iterator to the first feature.
       *
       * @details
       * If the @a FeatureName is specified, it will return a iterator pointing to the first subfeature of
       * the feature specified. If not, it will iterate over all sub-features enabled.
       *
       * @param FeatureName The name of the feature to be iterated over.
       */
      iterator begin(std::string FeatureName = "") { return iterator(&Features, Features.begin(), FeatureName); }

      /**
       * @brief Gets a iterator to the end of the features list.
       */
      iterator end(std::string FeatureName = "") { return iterator(&Features, Features.end(), FeatureName); }

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

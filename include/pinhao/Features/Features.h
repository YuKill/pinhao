/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Features.h
 * @brief This file implements some basic feature classes that extends
 * the base class @a Feature.
 */

#ifndef PINHAO_FEATURES_H
#define PINHAO_FEATURES_H

#include "pinhao/Features/Feature.h"

namespace pinhao {

  /**
   * @brief Abstract class of linear features.
   *
   * @details
   * Linear features are basically features that can be recovered with
   * only the name of the desired feature.
   */
  template <class ValueType>
    class LinearFeature : public Feature {
      public:
        virtual ~LinearFeature() {}
        LinearFeature(FeatureInfo *Info) : Feature(Info, LinearKind) {}

        /// @brief Sets the value of the feature @a FeatureName to @a Value.
        virtual void setValueOf(std::string FeatureName, ValueType Value) = 0;

        /// @brief Returns the value of a feature that has name @a FeatureName.
        /// It can be itself as well.
        virtual const ValueType& getValueOf(std::string FeatureName) = 0;
    };

  /**
   * @brief Abstract class of mapped features.
   *
   * @details
   * As the name suggests, this class covers features whose data depends
   * on some key, or in this case, are mapped somehow to some key.
   */
  template <class KeyType, class ValueType>
    class MappedFeature : public Feature {
      public:
        virtual ~MappedFeature() {}
        MappedFeature(FeatureInfo *Info) : Feature(Info, MappedKind) {}

        /// @brief Sets the value of the feature @a FeatureName of the key @a Key to @a Value.
        virtual void setValueOfKey(std::string FeatureName, ValueType Value, KeyType Key) = 0;

        /// @brief Returns the value of a feature that has name @a FeatureName
        /// and key @a Key.
        virtual const ValueType& getValueOfKey(std::string FeatureName, KeyType Key) = 0;
    };

  /**
   * @brief Implements a string version of a linear feature.
   *
   * @details
   * As it is just a string, it can have this implementation. Classes 
   * that extend this one can override the implementation.
   */
  class StringFeature : public LinearFeature<std::string> {
    private:
      /// @brief The string feature "container".
      std::string TheFeature;

    public:
      virtual ~StringFeature() {};
      StringFeature(FeatureInfo *Info) : LinearFeature<std::string>(Info) {}

      /// @brief Sets the string feature to @a New.
      void setValueOf(std::string FeatureName, std::string Value) override {
        assert(FeatureName == this->Info->getName() && "FeatureName doesn't equal the name of this feature.");
        TheFeature = Value;
      }

      /// @brief Gets the string feature (the @a FeatureName must be the same as
      /// this feature's name).
      const std::string& getValueOf(std::string FeatureName) override {
        assert(FeatureName == this->Info->getName() && "FeatureName doesn't equal the name of this feature.");
        return TheFeature;
      }

      virtual void appendYaml(YAML::Emitter &Emitter) override;

  };

  /**
   * @brief Implements a template vector feature class.
   *
   * @details
   * All the vector-like features must implement this class,
   * giving its type as the template.
   */
  template <class ElemType>
    class VectorFeature : public LinearFeature<ElemType> {
      private:
        /// @brief The vector container of the feature itself.
        std::vector<ElemType> TheFeature;

      public:
        virtual ~VectorFeature() {};

        VectorFeature(FeatureInfo *Info) : LinearFeature<ElemType>(Info) {
          assert(this->isComposite() && "VectorFeature must have CompositeFeatureInfo.");
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          TheFeature = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0); 
        }

        /**
         * @brief Sets the sub-feature with name @a FeatureName to
         * @a Elem.
         *
         * @param FeatureName The name of the desired sub-feature.
         * @param Elem The element that will replace the old element.
         */
        void setValueOf(std::string FeatureName, ElemType Elem) override {
          assert(this->hasSubFeature(FeatureName) && "VectorFeature has no such sub-feature.");
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName); 
          TheFeature[Index] = Elem;
        } 

        /**
         * @brief Gets the sub-feature, which has type @a ElemType, with
         * name @a FeatureName.
         *
         * @param FeatureName The name of the desired sub-feature.
         * @return A pointer to the sub-feature value with const modifier.
         */
        const ElemType& getValueOf(std::string FeatureName) override {
          assert(this->hasSubFeature(FeatureName) && "VectorFeature has no such sub-feature.");
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName); 
          return TheFeature[Index];
        }

        virtual void appendYaml(YAML::Emitter &Emitter) override {
          Emitter << YAML::BeginMap;
          Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();

          Emitter << YAML::Key << "features";
          Emitter << YAML::Value << YAML::BeginMap;
          for (auto &InfoPair : *(this)) {
            Emitter << YAML::Key << InfoPair.first;
            Emitter << YAML::Value << getValueOf(InfoPair.first);
            Emitter << YAML::Comment(InfoPair.second);
          }
          Emitter << YAML::EndMap;

          Emitter << YAML::EndMap;
        }
  };

  /**
   * @brief This class maps a @a KeyType to a @a ElemType feature.
   *
   * @details
   * It is a composite feature type which maps a given @a KeyType to a vector
   * of sub-features of type @a ElemType. So, every @a KeyType has all sub-feature
   * values.
   */
  template <class KeyType, class ElemType>
    class MapFeature : public MappedFeature<KeyType, ElemType> {
      private:
        /// @brief The vector container of the feature itself.
        std::map<KeyType, ElemType> TheFeature;

      public:
        virtual ~MapFeature() {}
        MapFeature(FeatureInfo *Info) : MappedFeature<KeyType, ElemType>(Info) {}

        void setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) override {
          assert(FeatureName == this->getName() && "FeatureName doesn't equal the name of this feature.");
          TheFeature[Key] = Elem;
        }

        const ElemType& getValueOfKey(std::string FeatureName, KeyType Key) override {
          assert(FeatureName == this->getName() && "FeatureName doesn't equal the name of this feature.");
          return TheFeature[Key];
        }

        virtual void appendYaml(YAML::Emitter &Emitter) override {
          Emitter << YAML::BeginMap;
          Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();
          Emitter << YAML::Comment(this->getDescription());

          Emitter << YAML::Key << "values";
          Emitter << YAML::Value << YAML::BeginMap;
          for (auto &Pair : TheFeature) {
            Emitter << YAML::Key << Pair.first;
            Emitter << YAML::Value << Pair.second;
          }
          Emitter << YAML::EndMap;
          
          Emitter << YAML::EndMap;
        }

    };

  /**
   * @brief This class maps a @a KeyType to a vector of @a ElemType features.
   *
   * @details
   * It is a composite feature type which maps a given @a KeyType to a vector
   * of sub-features of type @a ElemType. So, every @a KeyType has all sub-feature
   * values.
   */
  template <class KeyType, class ElemType>
    class MapVectorFeature : public MappedFeature<KeyType, ElemType> {
      private:
        /// @brief The vector container of the feature itself.
        std::map<KeyType, std::vector<ElemType>> TheFeature;

        /// @brief Initializes the vector of the key at the map @a TheFeature.
        void initVectorOfKey(KeyType Key) {
          if (TheFeature.count(Key) > 0) return;
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          TheFeature[Key] = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0);
        }

      public:
        virtual ~MapVectorFeature() {}

        MapVectorFeature(FeatureInfo *Info) : MappedFeature<KeyType, ElemType>(Info) {
          assert(this->isComposite() && "MapVectorFeature must have CompositeFeatureInfo.");
        }

        /**
         * @brief Sets the value of the sub-feature, with name @a FeatureName, of key @a Key,
         * to value @a Elem.
         *
         * @param FeatureName The name of the sub-feature.
         * @param Elem The value that will replace the old one.
         * @param Key The key from which it will set the sub-feature value.
         */
        void setValueOfKey(std::string FeatureName, ElemType Elem, KeyType Key) override {
          assert(this->hasSubFeature(FeatureName) && "MapVectorFeature has no such sub-feature.");

          initVectorOfKey(Key);
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName);
          TheFeature[Key][Index] = Elem;
        }

        /**
         * @brief Gets the sub-feature value, whose name is @a FeatureName, and
         * type @a ElemType.
         *
         * @details
         * As it is a map, it should be provided the @a Key of type @a KeyType, so
         * that it is returned the pointer to the sub-feature of that key.
         *
         * @param FeatureName The name of the sub-feature.
         * @param Key The key to get from.
         * @return A const pointer to the value of the sub-feature of the key.
         */
        const ElemType& getValueOfKey(std::string FeatureName, KeyType Key) override {
          assert(this->hasSubFeature(FeatureName) && "MapVectorFeature has no such sub-feature.");

          initVectorOfKey(Key);
          CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(this->Info.get());
          uint64_t Index = CompInfo->getIndexOfSubFeature(FeatureName); 
          return TheFeature[Key][Index];
        }

        virtual void appendYaml(YAML::Emitter &Emitter) override {
          Emitter << YAML::BeginMap;
          Emitter << YAML::Key << "feature-name" << YAML::Value << this->getName();

          Emitter << YAML::Key << "values";
          Emitter << YAML::Value << YAML::BeginMap;
          for (auto &ValuePair : TheFeature) {
            Emitter << YAML::Key << ValuePair.first;
            Emitter << YAML::Value << YAML::BeginMap;
            for (auto &InfoPair : *(this)) {
              Emitter << YAML::Key << InfoPair.first;
              Emitter << YAML::Value << getValueOfKey(InfoPair.first, ValuePair.first);
              Emitter << YAML::Comment(InfoPair.second);
            }
            Emitter << YAML::EndMap;
          }
          Emitter << YAML::EndMap;

          Emitter << YAML::EndMap;
        }

    };

}

#endif

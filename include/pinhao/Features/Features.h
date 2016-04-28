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

  template <class KType> class KeyIterator;

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
        virtual const ValueType& getValueOf(std::string FeatureName) const = 0;
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

        /// @brief Returns true if the key @a Key is mapped to some value.
        virtual bool hasKey(const KeyType &Key) const = 0;

        /// @brief Sets the value of the feature @a FeatureName of the key @a Key to @a Value.
        virtual void setValueOfKey(std::string FeatureName, ValueType Value, KeyType Key) = 0;

        /// @brief Returns the value of a feature that has name @a FeatureName
        /// and key @a Key.
        virtual const ValueType& getValueOfKey(std::string FeatureName, const KeyType Key) const = 0;

        /// @brief Returns the reference of the begin of an abstract iterator.
        virtual KeyIterator<KeyType> &beginKeys() = 0;
        /// @brief Returns the reference of the end of an abstract iterator.
        virtual KeyIterator<KeyType> &endKeys() = 0;
    };

}

namespace pinhao {

  /**
   * @brief Implements a string version of a linear feature.
   *
   * @details
   * As it is just a string, it can have this implementation. Classes 
   * that extend this one can override the implementation.
   */
  class StringFeature : public LinearFeature<std::string> {
    protected:
      /// @brief The string feature "container".
      std::string TheFeature;

    public:
      virtual ~StringFeature() {};
      StringFeature(FeatureInfo *Info);

      /// @brief Sets the string feature to @a New.
      void setValueOf(std::string FeatureName, std::string Value) override;

      /// @brief Gets the string feature (the @a FeatureName must be the same as
      /// this feature's name).
      const std::string& getValueOf(std::string FeatureName) const override;

      void append(YAML::Emitter &Emitter) const override;
      void get(const YAML::Node &Node) override;

  };

}

#endif

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Yamlfy.h
 */

#ifndef PINHAO_YAMLFY_H
#define PINHAO_YAMLFY_H

#include "yaml-cpp/yaml.h"

namespace pinhao {

  /**
   * @brief Base class responsible for transforming something into
   * a YAML object and the other way around.
   */
  class YamlfyBase {
    public:
      virtual ~YamlfyBase() {}

      /**
       * @brief Appends to a emitter some value.
       */
      virtual void append(YAML::Emitter &Emitter, bool PrintReduced) = 0;

      /**
       * @brief Gets from a @a YAML::Node some value.
       */
      virtual void get(const YAML::Node &Node) = 0;
  };

  /**
   * @brief The template class, implementing the function for some type.
   */
  template <class ValueType>
    class YamlfyTemplateBase : public YamlfyBase {
      protected:
        ValueType *Value;

      public:
        virtual ~YamlfyTemplateBase() {}

        /**
         * @brief Constructs the object with some reference that may be used
         * by the @a get method. 
         */
        YamlfyTemplateBase(const ValueType *Val) : Value(const_cast<ValueType*>(Val)) {}
    };

  /**
   * @brief The template class that should be specialized.
   */
  template <class ValueType>
    class Yamlfy : public YamlfyTemplateBase<ValueType> {
      public:
        Yamlfy(const ValueType *V) : YamlfyTemplateBase<ValueType>(V) {}

        virtual void append(YAML::Emitter &Emitter, bool PrintReduced) { Emitter << *(this->Value); }
        virtual void get(const YAML::Node &Node) { *(this->Value) = Node.as<ValueType>(); }
    };

  template <>
    class Yamlfy<void*> : public YamlfyTemplateBase<void*> {
      public: 
        Yamlfy(void* const* V) : YamlfyTemplateBase<void*>(V) {}

        virtual void append(YAML::Emitter &Emitter, bool PrintReduced) { Emitter << (uint64_t)*(this->Value); }
        virtual void get(const YAML::Node &Node) { *(this->Value) = (void*) Node.as<uint64_t>(); }
    };

}

#endif



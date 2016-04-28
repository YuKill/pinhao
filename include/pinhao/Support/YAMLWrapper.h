/*-------------------------- PINHAO project --------------------------*/

/**
 * @file YAMLWrapper.h
 */

#ifndef PINHAO_YAML_WRAPPER_H
#define PINHAO_YAML_WRAPPER_H

#include "yaml-cpp/yaml.h"

#include <iostream>
#include <ostream>

namespace pinhao {

  /*
   * Feature classes.
   */
  class StringFeature;
  template <class Type> class VectorFeature;
  template <class KType, class VType> class MapFeature;
  template <class KType, class VType> class MapVectorFeature;

  /*
   * Optimization classes.
   */
  class OptimizationArgBase;
  class OptimizationInfo;
  class OptimizationSequence;

  /**
   * @brief This class is a wrapper class for the yaml-cpp library.
   */
  class YAMLWrapper {
    private:
      typedef const YAML::Node ConstNode;
      typedef YAML::Emitter Emitter;

    public:
      /**
       * @brief Creates a new object of class @a T , by parsing a @a YAML::Node,
       * and returning an @a std::unique_ptr of it.
       */
      template <class T> 
        static std::unique_ptr<T> get(ConstNode &Node) {
          T *New = new T(); 
          fill(*New, Node);
          return std::unique_ptr<T>(New);
        }

      template <class T> 
        static void fill(VectorFeature<T> &Value, ConstNode &Node);
      template <class T, class U> 
        static void fill(MapFeature<T, U> &Value, ConstNode &Node);
      template <class T, class U> 
        static void fill(MapVectorFeature<T, U> &Value, ConstNode &Node);

      /**
       * @brief Parses the node @a Node, and stores the information inside
       * @a Value.
       */
      template <class T> 
        static void fill(T &Value, ConstNode &Node) {
          Value = Node.as<T>(); 
        }

      template <class T> 
        static void append(const VectorFeature<T> &Value, Emitter &E);
      template <class T, class U> 
        static void append(const MapFeature<T, U> &Value, Emitter &E);
      template <class T, class U> 
        static void append(const MapVectorFeature<T, U> &Value, Emitter &E);

      /**
       * @brief Appends the information of @a Value to the @a YAML::Emitter @a E.
       */
      template <class T> 
        static void append(const T &Value, Emitter &E) { 
          E << Value; 
        }

      /**
       * @brief Prints to an @a std::ostream object the information of @a Value.
       */
      template <class T> 
        static void print(const T &Value, std::ostream &Out = std::cout) {
          Emitter E(Out);
          append<T>(Value, E);
          Out << std::endl;
        }

  };

  template<> void YAMLWrapper::fill<void*>(void*&, ConstNode&);
  template<> void YAMLWrapper::append<void*>(void* const&, Emitter&);

  template<> void YAMLWrapper::fill<StringFeature>(StringFeature&, ConstNode&);
  template<> void YAMLWrapper::append<StringFeature>(const StringFeature&, Emitter&);

  template<> void YAMLWrapper::fill<OptimizationArgBase>(OptimizationArgBase&, ConstNode&);
  template<> void YAMLWrapper::append<OptimizationArgBase>(const OptimizationArgBase&, Emitter&);

  template<> void YAMLWrapper::fill<OptimizationInfo>(OptimizationInfo&, ConstNode&);
  template<> void YAMLWrapper::append<OptimizationInfo>(const OptimizationInfo&, Emitter&);

  template<> void YAMLWrapper::fill<OptimizationSequence>(OptimizationSequence&, ConstNode&);
  template<> void YAMLWrapper::append<OptimizationSequence>(const OptimizationSequence&, Emitter&);
}

#endif



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

  /*
   * Formula classes.
   */
  class FormulaBase;
  template <class T> class Formula;
  template <class T> class ArithBinOpFormula;
  template <class T> class BoolBinOpFormula;
  template <class T> class IfFormula;
  template <class T> class LitFormula;
  template <class T> class FeatureFormula;

  template <class T, class U> class KnowledgeBase;

  /**
   * @brief This class is a wrapper class for the yaml-cpp library.
   */
  class YAMLWrapper {
    public:
      typedef const YAML::Node ConstNode;
      typedef YAML::Emitter Emitter;

      static YAML::Node loadFile(std::string Filename) {
        return YAML::LoadFile(Filename); 
      }

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

      /**
       * @brief Parses the node @a Node, and stores the information inside
       * @a Value.
       */
      template <class T> 
        static void fill(T &Value, ConstNode &Node) {
          Value = Node.as<T>(); 
        }

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
          append(Value, E);
          Out << std::endl;
        }
    
      // fill overloads.
      template <class T> 
        static void fill(VectorFeature<T> &Value, ConstNode &Node);
      template <class T, class U> 
        static void fill(MapFeature<T, U> &Value, ConstNode &Node);
      template <class T, class U> 
        static void fill(MapVectorFeature<T, U> &Value, ConstNode &Node);

      template <class T> 
        static void fill(Formula<T> &Value, ConstNode &Node);
      template <class T> 
        static void fill(ArithBinOpFormula<T> &Value, ConstNode &Node);
      template <class T> 
        static void fill(BoolBinOpFormula<T> &Value, ConstNode &Node);
      template <class T> 
        static void fill(IfFormula<T> &Value, ConstNode &Node);
      template <class T> 
        static void fill(LitFormula<T> &Value, ConstNode &Node);
      template <class T> 
        static void fill(FeatureFormula<T> &Value, ConstNode &Node);

      template <class T, class U> 
        static void fill(KnowledgeBase<T, U> &Value, ConstNode &Node);

      // append overloads.
      template <class T> 
        static void append(const VectorFeature<T> &Value, Emitter &E);
      template <class T, class U> 
        static void append(const MapFeature<T, U> &Value, Emitter &E);
      template <class T, class U> 
        static void append(const MapVectorFeature<T, U> &Value, Emitter &E);

      template <class T> 
        static void append(const Formula<T> &Value, Emitter &E);
      template <class T> 
        static void append(const ArithBinOpFormula<T> &Value, Emitter &E);
      template <class T> 
        static void append(const BoolBinOpFormula<T> &Value, Emitter &E);
      template <class T> 
        static void append(const IfFormula<T> &Value, Emitter &E);
      template <class T> 
        static void append(const LitFormula<T> &Value, Emitter &E);
      template <class T> 
        static void append(const FeatureFormula<T> &Value, Emitter &E);

      template <class T, class U> 
        static void append(const KnowledgeBase<T, U> &Value, Emitter &E);
  };

  template<> std::unique_ptr<FormulaBase> YAMLWrapper::get<FormulaBase>(ConstNode&);
  template<> void YAMLWrapper::append<FormulaBase>(const FormulaBase&, Emitter&);

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



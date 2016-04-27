/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Types.h
 */

#ifndef PINHAO_TYPES_H
#define PINHAO_TYPES_H

namespace pinhao {

  /**
   * @brief Enum class to specify the type of values.
   */
  enum class ValueType {
    Int,
    Float,
    Bool,
    String
  };

  template <class T>
    T *createTyped(ValueType Type) {
      switch (Type) {
        case ValueType::Int:    return new T<int>();
        case ValueType::Float:  return new T<double>(); 
        case ValueType::Bool:   return new T<bool>();
        case ValueType::String: return new T<std::string>();
      }
    }

  template <class T>
    ValueType getValueTypeFor() { return 0; }

  template<> ValueType getValueTypeFor<int>()         { return ValueType::Int; }
  template<> ValueType getValueTypeFor<double>()      { return ValueType::Float; }
  template<> ValueType getValueTypeFor<std::string>() { return ValueType::String; }

}

#endif



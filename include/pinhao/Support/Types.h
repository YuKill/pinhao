/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Types.h
 */

#ifndef PINHAO_TYPES_H
#define PINHAO_TYPES_H

#include <string>

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
    ValueType getValueTypeFor() { return 0; }

  template<> ValueType getValueTypeFor<int>();
  template<> ValueType getValueTypeFor<bool>();
  template<> ValueType getValueTypeFor<double>();
  template<> ValueType getValueTypeFor<std::string>();

}

#endif



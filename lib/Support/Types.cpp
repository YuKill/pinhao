/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Types.cpp
 */

#include "pinhao/Support/Types.h"

using namespace pinhao;

template<> ValueType getValueTypeFor<int>() {
  return ValueType::Int;
}

template<> ValueType getValueTypeFor<bool>() {
  return ValueType::Bool;
}

template<> ValueType getValueTypeFor<double>() {
  return ValueType::Float;
}

template<> ValueType getValueTypeFor<std::string>() {
  return ValueType::String;
}

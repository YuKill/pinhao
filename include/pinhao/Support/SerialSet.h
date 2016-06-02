/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SerialSet.h
 */

#ifndef PINHAO_SERIAL_SET_H
#define PINHAO_SERIAL_SET_H

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/SerialSetYAMLWrapper.h"

#include <set>
#include <fstream>

namespace pinhao {

  /**
   * @brief This class is a wrapper of the 'std::set' class, that can be serialized.
   * @details
   * Its main members are @a importData and @a exportData. They manipulate the
   * data inside files by calling the YAMLWrapper methods.
   */
  template <class T, class CompareType = std::less<T>>
    class SerialSet : public std::set<T, CompareType> {
      public:
        /// @brief Returns true if this @a KnowledgeBase has the element.
        bool has(T) const;
        /// @brief Gets the @a Nth element of the set.
        T get(uint64_t) const;
        /// @brief Updates @a Value, if exists. Else, it inserts the element.
        void update(const T Value);

        /// @brief Prints the data of this @a SerialSet in an @a std::ostream.
        void print(std::ostream& = std::cout) const;

    };

}

template <class T, class CompareType>
bool pinhao::SerialSet<T, CompareType>::has(const T Value) const {
  return this->count(Value) > 0;
}

template <class T, class CompareType>
T pinhao::SerialSet<T, CompareType>::get(uint64_t N) const {
  assert(this->size() > N && "Out of SerialSet bounds.");
  for (auto &Value : *this) {
    if (!N--) return Value;
  }
}

template <class T, class CompareType>
void pinhao::SerialSet<T, CompareType>::update(const T Value) {
  if (has(Value)) this->erase(Value);
  this->insert(Value);
}

template <class T, class CompareType>
void pinhao::SerialSet<T, CompareType>::print(std::ostream &Out) const {
  YAMLWrapper::print(*this, Out);
}

#endif

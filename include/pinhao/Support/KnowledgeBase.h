/*-------------------------- PINHAO project --------------------------*/

/**
 * @file KnowledgeBase.h
 */

#ifndef PINHAO_KNOWLEDGE_BASE_H
#define PINHAO_KNOWLEDGE_BASE_H

#include "pinhao/Support/YAMLWrapper.h"
#include "pinhao/Support/KnowledgeBaseYAMLWrapper.h"

#include <set>
#include <fstream>

namespace pinhao {

  /**
   * @brief This class is an abstraction of a data base, for some entity.
   * @details
   * Its main members are @a importData and @a exportData. They manipulate the
   * data inside files by calling the YAMLWrapper methods.
   */
  template <class T, class CompareType = std::less<T>>
    class KnowledgeBase : public std::set<T, CompareType> {
      public:
        /// @brief Returns true if this @a KnowledgeBase has the element.
        bool has(T) const;
        /// @brief Gets the @a Nth element of the set.
        T get(uint64_t) const;
        /// @brief Updates some record considered equal (depends on the functor provided).
        void update(const T);

        /// @brief Imports the data inside some file with name @a Filename.
        void importData(std::string);
        /// @brief Exports the data to some file with name @a Filename.
        void exportData(std::string) const;

    };

}

template <class T, class CompareType>
bool pinhao::KnowledgeBase<T, CompareType>::has(const T Value) const {
  return count(Value) > 0;
}

template <class T, class CompareType>
T pinhao::KnowledgeBase<T, CompareType>::get(uint64_t N) const {
  assert(this->size() > N && "Out of KnowledgeBase bounds.");
  for (auto &Value : *this) {
    if (!N--) return Value;
  }
}

template <class T, class CompareType>
void pinhao::KnowledgeBase<T, CompareType>::update(const T Value) {
  if (has(Value)) erase(Value);
  insert(Value);
}

template <class T, class CompareType>
void pinhao::KnowledgeBase<T, CompareType>::importData(std::string Filename) {
  auto Node = YAMLWrapper::loadFile(Filename);
  YAMLWrapper::fill(*this, Node);
}

template <class T, class CompareType>
void pinhao::KnowledgeBase<T, CompareType>::exportData(std::string Filename) const {
  std::ofstream Out(Filename);
  YAMLWrapper::Emitter Emitter(Out);
  YAMLWrapper::append(*this, Emitter);
}

#endif

/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Iterator.h
 */

#ifndef PINHAO_ITERATOR_H
#define PINHAO_ITERATOR_H

#include "pinhao/Features/Features.h"

#include <iterator>
#include <map>

namespace pinhao {

  /**
   * @brief Base class for iterating over the keys of some @a MappedFeature.
   */
  template <class KType>
    class KeyIterator : public std::iterator<std::forward_iterator_tag, KType> {
      public:
        virtual ~KeyIterator() {}

        virtual const KType *operator->() const = 0;
        virtual const KType &operator*() = 0;

        /// @brief It will return a reference, as it can't be instantiated.
        virtual KeyIterator &operator++() = 0;

        virtual bool operator==(const KeyIterator &It) = 0;
        virtual bool operator!=(const KeyIterator &It) = 0;

    };

  /**
   * @brief Concrete class that iterates over the keys of the stl provided
   * std::map class.
   */
  template <class KType, class VType>
    class StdMapKeyIterator : public KeyIterator<KType> {
      private:
        typedef std::map<KType, VType> Map;

        Map *ThisMap;
        typename Map::iterator CurrentIt;
        const KType *CurrentPtr;

        void updateCurrentPtr();

      public:
        StdMapKeyIterator() {}
        StdMapKeyIterator(Map *M, typename Map::iterator It);

        const KType *operator->() const override;
        const KType &operator*() override;
        KeyIterator<KType> &operator++() override;
        bool operator==(const KeyIterator<KType> &It) override;
        bool operator!=(const KeyIterator<KType> &It) override;
    };

}

template <class KType, class VType>
pinhao::StdMapKeyIterator<KType, VType>::StdMapKeyIterator(Map *M, typename Map::iterator It) : 
  ThisMap(M), CurrentIt(It) {
    updateCurrentPtr(); 
  }

template <class KType, class VType>
void pinhao::StdMapKeyIterator<KType, VType>::updateCurrentPtr() {
  if (CurrentIt == ThisMap->end())
    CurrentPtr = nullptr;
  else
    CurrentPtr = &CurrentIt->first;
}

template <class KType, class VType>
const KType *pinhao::StdMapKeyIterator<KType, VType>::operator->() const {
  return CurrentPtr;
}

template <class KType, class VType>
const KType &pinhao::StdMapKeyIterator<KType, VType>::operator*() {
  return *CurrentPtr;
}

template <class KType, class VType>
pinhao::KeyIterator<KType> &pinhao::StdMapKeyIterator<KType, VType>::operator++() {
  ++CurrentIt;
  updateCurrentPtr(); 
  return *this;
}

template <class KType, class VType>
bool pinhao::StdMapKeyIterator<KType, VType>::operator==(const pinhao::KeyIterator<KType> &It) {
  const StdMapKeyIterator<KType, VType> *ItPtr = 
    static_cast<const StdMapKeyIterator<KType, VType>*>(&It);
  return (ThisMap == ItPtr->ThisMap) && (CurrentPtr == ItPtr->CurrentPtr);
} 

template <class KType, class VType>
bool pinhao::StdMapKeyIterator<KType, VType>::operator!=(const pinhao::KeyIterator<KType> &It) {
  return !(*this == It);
}

namespace pinhao {
  
  /**
   * @brief Wrapper function that casts some feature pointer to a @a MappedFeature, and
   * returns a reference to a iterator at the beginning of the map.
   */
  template <class KType, class VType>
  KeyIterator<KType> &beginKeys(Feature *F) {
    MappedFeature<KType, VType> *Mapped = static_cast<MappedFeature<KType, VType>*>(F);
    return Mapped->beginKeys();
  }

  /**
   * @brief Wrapper function that casts some feature pointer to a @a MappedFeature, and
   * returns a reference to a iterator at the end of the map.
   */
  template <class KType, class VType>
  KeyIterator<KType> &endKeys(Feature *F) {
    MappedFeature<KType, VType> *Mapped = static_cast<MappedFeature<KType, VType>*>(F);
    return Mapped->endKeys();
  }

}

#endif



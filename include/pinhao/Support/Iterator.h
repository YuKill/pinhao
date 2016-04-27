/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Iterator.h
 */

#ifndef PINHAO_ITERATOR_H
#define PINHAO_ITERATOR_H

#include <iterator>
#include <map>

namespace pinhao {

  template <class KType>
    class KeyIterator : public std::iterator<std::forward_iterator_tag, KType> {
      public:
        virtual ~KeyIterator() {}

        virtual const KType *operator->() const = 0;
        virtual const KType &operator*() = 0;
        virtual KeyIterator &operator++() = 0;
        virtual bool operator==(const KeyIterator &It) = 0;
        virtual bool operator!=(const KeyIterator &It) = 0;

    };

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

#endif



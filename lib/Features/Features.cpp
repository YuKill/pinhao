/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Features.cpp
 * @brief This file implements the functions of the basic implementations
 * of the feature classes. Such as the StringFeature , the VectorFeature and
 * the MapVectorFeature.
 */

#include "pinhao/Features/Features.h"

#include "shogun/lib/SGMatrix.h"
#include "shogun/lib/SGString.h"
#include "shogun/lib/SGStringList.h"
#include "shogun/features/DenseFeatures.h"
#include "shogun/features/StringFeatures.h"


using namespace pinhao;

/*
 * ----------------------------------=
 * Class: StringFeature
 */
std::unique_ptr<shogun::CFeatures> StringFeature::getShogunFeature() {
  unsigned Length = TheFeature.length();
  if (Length > 0) {
    char *Buffer = new char[Length + 1];
    TheFeature.copy(Buffer, Length);
    Buffer[Length] = '\0';

    shogun::SGString<char> *SGString = 
      new shogun::SGString<char>(Buffer, Length, true);
    shogun::SGStringList<char> *SGStringList = 
      new shogun::SGStringList<char>(SGString, 1, Length + 1);
    shogun::CStringFeatures<char> *StrFeatures = 
      new shogun::CStringFeatures<char>(*SGStringList, shogun::EAlphabet::UNKNOWN);

    return std::unique_ptr<shogun::CFeatures>(StrFeatures);
  }

  return std::unique_ptr<shogun::CFeatures>(new shogun::CStringFeatures<char>(shogun::EAlphabet::UNKNOWN));
}

/*
 * ----------------------------------=
 * Class: VectorFeature < ElemType >
 */
template <class ElemType>
const ElemType& VectorFeature<ElemType>::getSubFeature(std::string SubFeatureName) {
  assert(hasSubFeature(SubFeatureName) && "VectorFeature has no such sub-feature.");
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(SubFeatureName); 
  return TheFeature[Index];
}

template <class ElemType>
void VectorFeature<ElemType>::setSubFeature(std::string SubFeatureName, ElemType Elem) {
  assert(hasSubFeature(SubFeatureName) && "VectorFeature has no such sub-feature.");
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(SubFeatureName); 
  TheFeature[Index] = Elem;
}

template <class ElemType>
std::unique_ptr<shogun::CFeatures> VectorFeature<ElemType>::getShogunFeature() {
  if (!TheFeature.empty()) {
    ElemType *Vector = new ElemType[TheFeature.size()]();
    for (int I = 0, E = TheFeature.size(); I < E; ++I)
      Vector[I] = TheFeature[I];

    shogun::SGMatrix<ElemType> *SGMatrix = new shogun::SGMatrix<ElemType>(Vector, 1, TheFeature.size());
    shogun::CDenseFeatures<ElemType> *DFeatures = new shogun::CDenseFeatures<ElemType>(*SGMatrix);

    return std::unique_ptr<shogun::CFeatures>(DFeatures);
  }
  return std::unique_ptr<shogun::CFeatures>(new shogun::CDenseFeatures<ElemType>());
}

/*
 * ----------------------------------=
 * Class: MapVectorFeature < KeyType, ElemType >
 */
template <class KeyType, class ElemType>
void MapVectorFeature<KeyType, ElemType>::initVectorOfKey(KeyType Key) {

  if (TheFeature.count(Key) > 0) return;
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
  TheFeature[Key] = std::vector<ElemType>(CompInfo->getNumberOfSubFeatures(), 0);
}

template <class KeyType, class ElemType>
void MapVectorFeature<KeyType, ElemType>::setSubFeatureOfKey(std::string SubFeatureName, 
    ElemType Elem, KeyType Key) {
  assert(hasSubFeature(SubFeatureName) && "MapVectorFeature has no such sub-feature.");

  initVectorOfKey(Key);
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(SubFeatureName);
  TheFeature[Key][Index] = Elem;
}

template <class KeyType, class ElemType>
const ElemType& MapVectorFeature<KeyType, ElemType>::getSubFeatureOfKey(std::string SubFeatureName, KeyType Key) {
  assert(hasSubFeature(SubFeatureName) && "MapVectorFeature has no such sub-feature.");

  initVectorOfKey(Key);
  CompositeFeatureInfo *CompInfo = static_cast<CompositeFeatureInfo*>(Info.get());
  uint64_t Index = CompInfo->getIndexOfSubFeature(SubFeatureName); 
  return TheFeature[Key][Index];
}

template <class KeyType, class ElemType>
std::unique_ptr<shogun::CFeatures> MapVectorFeature<KeyType, ElemType>::getShogunFeature() {
  if (!TheFeature.empty()) {
    uint64_t Rows = TheFeature.size(),
             Cols = (TheFeature.begin()->second).size(),
             I    = 0;
    ElemType *Matrix = new ElemType[Rows * Cols]();
    for (auto &MapPair : TheFeature) 
      for (auto &Elem : MapPair.second) {
        uint64_t Line = I / Cols,
                 Col  = I % Cols;
        Matrix[(Line * Cols) + Col] = Elem;
        ++I;
      }

    shogun::SGMatrix<ElemType> *SGMatrix = new shogun::SGMatrix<ElemType>(Matrix, Rows, Cols);
    shogun::CDenseFeatures<ElemType> *DFeatures = new shogun::CDenseFeatures<ElemType>(*SGMatrix);

    return std::unique_ptr<shogun::CFeatures>(DFeatures);
  }
  return std::unique_ptr<shogun::CFeatures>(new shogun::CDenseFeatures<ElemType>());
}

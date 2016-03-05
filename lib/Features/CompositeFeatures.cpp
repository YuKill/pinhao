/*-------------------------- PINHAO project --------------------------*/

/**
 * @file VectorFeature.cpp
 * @brief This file implements the functions of the composite features
 * classes. Such as the CompositeFeature itself, the VectorFeature and
 * the MapVectorFeature.
 */

#include "pinhao/Features/CompositeFeatures.h"

#include "shogun/lib/SGMatrix.h"
#include "shogun/features/DenseFeatures.h"


using namespace pinhao;

/*
 * -------------------------------------=
 * Class: CompositeFeature < ElemType, KeyType >
 */
template <class ElemType, class KeyType>
uint64_t CompositeFeature<ElemType, KeyType>::getIndexOfSubFeature(std::string SubFeatureName) {
  uint64_t Index = 0;
  for (auto &InfoPair : *Info)
    if (SubFeatureName == InfoPair.first) return Index;
    else ++Index;
  return Index;
}

/*
 * ----------------------------------=
 * Class: VectorFeature < ElemType >
 */
template <class ElemType>
const ElemType* VectorFeature<ElemType>::getSubFeature(std::string SubFeatureName, uint64_t Key) {
  if (!this->hasSubFeature(SubFeatureName)) return nullptr;
  uint64_t Index = this->getIndexOfSubFeature(SubFeatureName); 
  return &TheFeature[Index];
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
const ElemType* MapVectorFeature<KeyType, ElemType>::getSubFeature(std::string SubFeatureName, KeyType Key) {
  if (this->hasSubFeature(SubFeatureName) && TheFeature.count(Key) > 0) {
    uint64_t Index = this->getIndexOfSubFeature(SubFeatureName); 
    return &(TheFeature[Key][Index]);
  }
  return nullptr;
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

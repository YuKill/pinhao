/*-------------------------- PINHAO project --------------------------*/

/**
 * @file VectorFeature.cpp
 * @brief This file implements the functions of the class @a VectorFeature.
 */

#include "pinhao/Features/VectorFeature.h"

#include "shogun/lib/SGMatrix.h"
#include "shogun/features/DenseFeatures.h"


using namespace pinhao;

template <class ElemType>
uint64_t VectorFeature<ElemType>::getIndexOfSubFeature(std::string Name) {
  uint64_t Index = 0;
  for (auto &InfoPair : *Info)
    if (Name == InfoPair.first) return Index;
    else ++Index;
  return Index;
}

template <class ElemType>
const ElemType* VectorFeature<ElemType>::getSubFeature(std::string Name) {
  if (!hasSubFeature(Name)) return nullptr;
  uint64_t Index = getIndexOfSubFeature(Name); 
  return &TheFeature[Index];
}

template <class ElemType>
std::unique_ptr<shogun::CFeatures> VectorFeature<ElemType>::getShogunFeature() {
  if (!TheFeature.empty()) {
    ElemType *Vector = new ElemType[TheFeature.size()]();
    shogun::SGMatrix<ElemType> *SGMatrix = new shogun::SGMatrix<ElemType>(Vector, 1, TheFeature.size());
    shogun::CDenseFeatures<ElemType> *DFeatures = new shogun::CDenseFeatures<ElemType>(*SGMatrix);

    return std::unique_ptr<shogun::CFeatures>(DFeatures);
  }
  return std::unique_ptr<shogun::CFeatures>(new shogun::CDenseFeatures<ElemType>());
}

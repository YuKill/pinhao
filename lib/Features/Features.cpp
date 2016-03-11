/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Features.cpp
 * @brief This file implements the functions of the basic implementations
 * of the feature classes. Such as the StringFeature , the VectorFeature and
 * the MapVectorFeature.
 */

#include "pinhao/Features/Features.h"

#include "shogun/lib/SGString.h"
#include "shogun/lib/SGStringList.h"
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

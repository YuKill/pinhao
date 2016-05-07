/*-------------------------- PINHAO project --------------------------*/

/**
 * @file LitFormula.cpp
 */

#include "pinhao/MachineLearning/GrammarEvolution/LitFormula.h"
#include "pinhao/Support/Random.h"

using namespace pinhao;

template<> int generateLiteral<int>() {
  auto Max = UniformRandom::getRandomReal() * 100;
  return UniformRandom::getRandomInt(0, Max);
}

template<> bool generateLiteral<bool>() {
  return (bool) UniformRandom::getRandomInt(0, 1);
}

template<> double generateLiteral<double>() {
  return UniformRandom::getRandomReal();
}

template<> std::string generateLiteral<std::string>() {
  return "";
}

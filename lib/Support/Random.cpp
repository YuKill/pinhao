/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Random.cpp
 */

#include "pinhao/Support/Random.h"

#include <chrono>
#include <math.h>

using namespace pinhao;

UniformRandom::UniformRandom() {
  unsigned Seed = std::chrono::system_clock::now().time_since_epoch().count();
  Rng.seed(Seed);
}

UniformRandom &UniformRandom::getUniformRandom() {
  static UniformRandom Unif;
  return Unif;
}

int UniformRandom::getRandomInt(int From, int To) {
  UniformRandom &Unif = getUniformRandom();
  return Unif.getInt(From, To);
}

double UniformRandom::getRandomReal() {
  UniformRandom &Unif = getUniformRandom();
  return Unif.getReal();
}

int UniformRandom::getInt(int From, int To) {
  double Die = getReal();
  int RandomNumber = floor(Die * (To - From + 1)) + From;
  return RandomNumber;
}

double UniformRandom::getReal() {
  return Unif(Rng);
}

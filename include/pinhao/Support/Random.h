/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Random.h
 */

#ifndef PINHAO_RANDOM_H
#define PINHAO_RANDOM_H

#include <random>

namespace pinhao {

  /**
   * @brief Standard random number generator wrapper.
   */
  class UniformRandom {
    private:
      std::default_random_engine Rng;
      std::uniform_real_distribution<double> Unif;

      /// @brief Gets a singleton reference to a @a UniformRandom object.
      static UniformRandom &getUniformRandom();

    public:
      UniformRandom();

      /// @brief Returns a random generated @a int between two values.
      int getInt(int, int);
      /// @brief Returns a random generated @a double between 0.0 and 1.0.
      double getReal();

      /// @brief Returns a random generated @a int with the singleton reference.
      static int getRandomInt(int, int);

      /// @brief Returns a random generated @a real between 0.0 and 1.0, 
      /// with the singleton reference.
      static double getRandomReal();

  };

}

#endif



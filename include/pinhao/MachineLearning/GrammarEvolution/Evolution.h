/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Evolution.h
 */

#ifndef PINHAO_EVOLUTION_H
#define PINHAO_EVOLUTION_H

#include <vector>
#include <string>
#include <memory>

namespace pinhao {
  class FormulaBase;
  class FeatureSet;
  
  /**
   * @brief This is a strategy class, that modifies the operands of each 
   * @a FormulaBase type.
   */
  class Evolution {
    public:
      FeatureSet *Set;

      Evolution(FeatureSet *Set) : Set(Set) {}
      virtual ~Evolution() {}

      /// @brief Evolves a set of @a Formula's, namely the @a BinOpFormula and the @a IfFormula.
      virtual void evolve(std::vector<std::unique_ptr<FormulaBase>*>) = 0;

      /// @brief Evolves an @a int value of a @a LitFormula<int>.
      virtual void evolve(int &Value) {} 
      /// @brief Evolves an @a bool value of a @a LitFormula<bool>.
      virtual void evolve(bool &Value) {} 
      /// @brief Evolves an @a double value of a @a LitFormula<double>.
      virtual void evolve(double &Value) {} 
      /// @brief Evolves an @a string value of a @a LitFormula<std::string>.
      virtual void evolve(std::string &Value) {} 

      /// @brief Evolves an @a FeatureFormula pair, which represents the feature and sub-feature,
      /// respectively.
      virtual void evolve(std::pair<std::string, std::string>&) = 0;
  };

}

#endif

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
  
  class Evolution {
    public:
      FeatureSet *Set;

      Evolution(FeatureSet *Set) : Set(Set) {}
      virtual ~Evolution() {}

      virtual void evolve(std::vector<std::unique_ptr<FormulaBase>*>) = 0;

      virtual void evolve(int &Value) {} 
      virtual void evolve(bool &Value) {} 
      virtual void evolve(double &Value) {} 
      virtual void evolve(std::string &Value) {} 

      virtual void evolve(std::pair<std::string, std::string>&) = 0;
  };

}

#endif

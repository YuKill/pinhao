/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Evolution.h
 */

#ifndef PINHAO_EVOLUTION_H
#define PINHAO_EVOLUTION_H

#include <vector>
#include <string>

namespace pinhao {
  class FormulaBase;
  class FeatureSet;
  
  class Evolution {
    public:
      virtual ~Evolution() {}

      virtual void evolve(std::vector<std::unique_ptr<FormulaBase>*>) = 0;

      virtual void evolve(int &Value) {} 
      virtual void evolve(bool &Value) {} 
      virtual void evolve(double &Value) {} 
      virtual void evolve(std::string &Value) {} 

      virtual void evolve(FeatureSet*, std::pair<std::string, std::string>&) = 0;
  };

}

#endif

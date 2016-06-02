/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Candidate.h
 */

#ifndef PINHAO_CANDIDATE_H
#define PINHAO_CANDIDATE_H

#include "pinhao/Support/YAMLWrapper.h"

#include <map>
#include <vector>

namespace pinhao {
  class DecisionPoint;
  class FeatureSet;
  class FormulaBase;
  class Evolution;

  struct Candidate : public std::map<DecisionPoint, FormulaBase*> {
    double Score;
    uint64_t Count;

    virtual ~Candidate();
    virtual void evolve(double, Evolution*);
    virtual void generateMissing(std::vector<DecisionPoint>, FeatureSet*);

    Candidate *clone() const;
    FormulaBase* &get(uint64_t N);
    bool operator<(const Candidate &Rhs) const; 
  };

  struct CompareByScore {
    bool operator()(const Candidate&, const Candidate&);
  };


  template<> void YAMLWrapper::append<Candidate>(const Candidate&, Emitter&);
  template<> void YAMLWrapper::fill<Candidate>(Candidate&, ConstNode&);

}

#endif

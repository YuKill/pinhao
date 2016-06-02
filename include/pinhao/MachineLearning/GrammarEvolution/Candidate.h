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

  /**
   * @brief Simple implementation of a @a Candidate class to be used within the
   * @a GrammarEvolution algorithm.
   */
  struct Candidate : public std::map<DecisionPoint, FormulaBase*> {
    double Score;
    uint64_t Count;

    virtual ~Candidate();
    /// @brief Evolves the current candidate.
    virtual void evolve(double, Evolution*);
    /// @brief Generate the decision point heuristics that are missing in this candidate.
    virtual void generateMissing(std::vector<DecisionPoint>, FeatureSet*);

    /// @brief Deep copy of this candidate.
    Candidate *clone() const;
    /// @brief Gets the Nth @a FormulaBase.
    FormulaBase* &get(uint64_t N);
    bool operator<(const Candidate &Rhs) const; 
  };

  /// @brief Class that compares the score of two @a Candidates.
  struct CompareByScore {
    /// @brief Returns true if the @a Lhs has a bigger score than the @a Rhs.
    bool operator()(const Candidate&, const Candidate&);
  };


  template<> void YAMLWrapper::append<Candidate>(const Candidate&, Emitter&);
  template<> void YAMLWrapper::fill<Candidate>(Candidate&, ConstNode&);

}

#endif

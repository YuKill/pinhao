/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SimpleGrammarEvolution.h
 */

#ifndef PINHAO_SIMPLE_GRAMMAR_EVOLUTION_H
#define PINHAO_SIMPLE_GRAMMAR_EVOLUTION_H

#include "pinhao/MachineLearning/GrammarEvolution/GrammarEvolution.h"

#include <vector>

namespace pinhao {
  class Candidate;

  class SimpleGrammarEvolution : public GrammarEvolution<Candidate> {
    private:
      virtual llvm::Module *compileWithCandidate(llvm::Module*, Candidate&, FeatureSet*) override;

    public:
      virtual ~SimpleGrammarEvolution();
      SimpleGrammarEvolution(std::shared_ptr<llvm::Module> /* Module */, std::string /* KBFilename */ = "config.yaml",
          double /* EvolveProb */ = 0.2, double /* MaxEvolutionRate */ = 0.3, double /* MutateProb */ = 0.3);

      virtual void run(int /* CandidatesNumber */, int /* GenerationsNumber */, std::shared_ptr<FeatureSet>) override;
      virtual void stop() override;

  };

}

#endif

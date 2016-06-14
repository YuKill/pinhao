/*-------------------------- PINHAO project --------------------------*/

/**
 * @file SimpleGrammarEvolution.h
 */

#ifndef PINHAO_PAR_SIMPLE_GRAMMAR_EVOLUTION_H
#define PINHAO_PAR_SIMPLE_GRAMMAR_EVOLUTION_H

#include "pinhao/MachineLearning/GrammarEvolution/SimpleGrammarEvolution.h"

namespace pinhao {

  class ParSimpleGrammarEvolution : public SimpleGrammarEvolution {
    private:
      virtual llvm::Module *compileWithCandidate(llvm::Module*, Candidate&, FeatureSet*) override;
      virtual void addPreDefinedDecisionPoints() override;

    public:
      ParSimpleGrammarEvolution(std::shared_ptr<llvm::Module> Module, std::string KBFilename = "config.yaml",
          double EvolveProb = 0.2, double MaxEvolutionRate = 0.3, double MutateProb = 0.3) :
        SimpleGrammarEvolution(Module, KBFilename, EvolveProb, MaxEvolutionRate, MutateProb) {}

  };

}

#endif

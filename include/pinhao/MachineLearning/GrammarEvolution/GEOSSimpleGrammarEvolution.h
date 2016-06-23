/*-------------------------- PINHAO project --------------------------*/

/**
 * @file GEOSSimpleGrammarEvolution.h
 */

#ifndef PINHAO_GEOS_SIMPLE_GRAMMAR_EVOLUTION_H
#define PINHAO_GEOS_SIMPLE_GRAMMAR_EVOLUTION_H

#include "pinhao/MachineLearning/GrammarEvolution/SimpleGrammarEvolution.h"

namespace pinhao {
  class Candidate;

  /**
   * @brief This is the geos version of the @a SimpleGrammarEvolution.
   */
  class GEOSSimpleGrammarEvolution : public SimpleGrammarEvolution {
    public:
      GEOSSimpleGrammarEvolution(std::shared_ptr<llvm::Module> /* Module */, std::string /* KBFilename */ = "config.yaml",
          double /* EvolveProb */ = 0.2, double /* MaxEvolutionRate */ = 0.3, double /* MutateProb */ = 0.3);

      virtual void run(int /* CandidatesNumber */, int /* GenerationsNumber */, std::shared_ptr<FeatureSet>) override;

  };

}

#endif

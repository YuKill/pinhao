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

  /**
   * @brief A simple evolution implementation.
   */
  class SimpleEvolution : public Evolution {
    private:
      double M;

    public:
      SimpleEvolution(double M, FeatureSet *Set) : Evolution(Set), M(M) {}

      /// @brief Mutates the node's @a FormulaBases.
      void mutate(std::unique_ptr<FormulaBase>*);
      /// @brief Evolves a @a FormulaBase from a node.
      void evolve(std::vector<std::unique_ptr<FormulaBase>*>) override;
      /// @brief Evolves an @a FeaturePair.
      void evolve(std::pair<std::string, std::string>&) override;
      /// @brief Evolvels an @a int value.
      void evolve(int&) override;
      /// @brief Evolves a @a bool value.
      void evolve(bool&) override;
  };

  /**
   * @brief This is a simple implementation of the @a GrammarEvolution class.
   *
   * @details
   * It uses all default implementations, and also implements a simple @a run and @a stop
   * function.
   */
  class SimpleGrammarEvolution : public GrammarEvolution<Candidate> {
    protected:
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
